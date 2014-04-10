#ifndef _K_TABLE_CACHE_H_
#define _K_TABLE_CACHE_H_

#include <System/File/KMMap.h>
#include <System/Collections/KMapByVector.h>
#include <System/Sync/KSync.h>
#include <System/Thread/KThread.h>
#include <System/Log/log.h>
#include <System/Collections/KQueue.h>
#include "KDatabase.h"
#include "KVarBinary.h"

class KCacheObject;

/**
* 紧急处理记录被处理后，Oberver将会得到通知
*/
struct KEmergencyEvent
{
	HANDLE hRec;
	void* pUserData;
};
class IEmergencyObserver
{
public:
	virtual ~IEmergencyObserver() { }
	virtual BOOL OnEmergencyFinished(BOOL success, void* pUserData) = 0;
};

/**
* 数据库表的内存映射文件Cache
*/

/**
 * 作为 Cache Table 的主键的数据类型需要满足几个条件
 *
 * 1. public inner type (1) key_type (2) key_cmp
 * 2. BOOL SetPrimaryKey(KCacheObject& cache, const key_type& val)
 * 3. BOOL GetPrimaryKey(KCacheObject& cache, key_type& val) const
 * 4. BOOL SetSQLParam(KSQLCommand& cmd, int paramBaseIdx, KCacheObject& cache)
 * 5. KString<512> ToString(const key_type& val) const
 *
 * 这里假定表的主键一定是从第一个字段开始并且字段之间是连续的
 *
 *
 * 缺省主键数据类型使用规则
 *
 * 单个数值型(非浮点)主键
 * 1. 长度小于等于4字节时使用DWORD为主键数据类型
 * 2. 长度大于4字节时使用UINT64为主键数据类型
 * 3. 字符串主键当长度小于64时，使用KString<64>为主键数据类型
 *
 * 两个字段联合主键
 * KPair<first, second>
 * 其中first和second的选取规则同上
 *
 * 非以上情况或者不愿使用上面的规则，需要自己扩展
 *
 *
 * KTableCache的具体用法请参看 petsworkshop\V001\Test\tsqldb
 * 
 */

#define INVALID_HREC ((HANDLE)KTableCache::invalid_hrec)

class ITableCacheCreator;
class KTableCache
{
public:
	friend class KCacheObject;
	friend class KCacheRefreshThread;
	friend class KCacheManager;

	enum
	{
		invalid_hrec = 0x7fffffff,
		invalid_recno = 0x7fffffff
	};

	enum eRecordStatusBit
	{
		c_new_created		= 0,  // 这条记录仅仅存在内存中，数据库中还没有
		c_updating			= 1,  // 应用程序正在修改记录中的数据
		c_modified			= 2,  // 这条记录被修改过了
		c_deleted			= 3,  // 这条记录需要删除
		c_checkout			= 4,  // 不需要保存在Cache里面了
		c_saving			= 5,  // 正在把记录保存到数据库
		c_loading			= 6,  // 正在从数据库加载
		c_partial_modified  = 7,  // 这条记录有部分字段被修改了
		c_new_record		= 8,  // 内存中新创建的记录，等待后天线程写入
		c_status_max
	};

	struct KRecordHead
	{
		union
		{
			struct
			{
				uint_r used : 1;
				uint_r nextNo : 31;
			} m_freeHead;
			struct
			{
				uint_r used : 1;
				uint_r version : 15;
				uint_r status : 16;
			} m_usedHead;
		};

		inline BOOL IsFree()
		{
			return !m_freeHead.used;
		}
		inline void SetUsed()
		{
			m_usedHead.used = 1;
			m_usedHead.version = 0;
			m_usedHead.status = 0;
		}
		inline void SetFree(uint_r nextNo)
		{
			m_freeHead.used = 0;
			m_freeHead.nextNo = nextNo;
		}
		inline uint_r GetNextNo()
		{
			ASSERT(!m_freeHead.used);
			return m_freeHead.nextNo;
		}
		inline void SetStatusBit(eRecordStatusBit bitNo)
		{
			m_usedHead.status |= (DWORD)(1 << bitNo);
		}
		inline void ClearStatusBit(eRecordStatusBit bitNo)
		{
			m_usedHead.status &= ~(1<<bitNo);
		}
		inline void ClearStatus()
		{
			m_usedHead.status = 0;
		}
		inline BOOL IsStatus(eRecordStatusBit statusBit)
		{
			return (m_usedHead.used) && (m_usedHead.status&(DWORD)(1<<statusBit));
		}
	};

	struct KFileHead
	{
		volatile DWORD usedCount;
		HANDLE firstFree;
		DWORD recSize;        // 记录的长度，包括记录的头部
		DWORD capacity;       // 最多可容纳多少条记录
	};

	struct CreateParam
	{
		BOOL refreshThread;
		BOOL resetMemory;
		int flushModSec, flushDelSec, flushCoSec;
		IEmergencyObserver* pObserver;
		System::Collections::KString<512> mmapfile;
		DWORD capacity;
	};

	class KRefreshThread : public System::Thread::KThread
	{
	public:
		KRefreshThread(KTableCache& cache)
			: m_cache(cache)
			, m_refreshUpdateInterval(5)
			, m_refreshDeleteInterval(5)
			, m_refreshCheckoutInterval(5)
			, m_lastRefreshModify(0)
			, m_lastRefreshCheckOut(0)
			, m_lastRefreshDelete(0)
			, m_stopDesired(FALSE)
		{

		}

		~KRefreshThread()
		{
			this->Stop();
		}

	public:
		void Execute();  // 线程函数
		void Stop();

	public:
		KTableCache& m_cache;
		int m_refreshUpdateInterval;
		int m_refreshDeleteInterval;
		int m_refreshCheckoutInterval;
		TIME_T m_lastRefreshModify;
		TIME_T m_lastRefreshCheckOut;
		TIME_T m_lastRefreshDelete;
		volatile BOOL m_stopDesired;
	};

public:
	KTableCache();
	KTableCache(KVarBinaryMemory* vbm);
	virtual ~KTableCache();

public:
	static KTableCache* CreateInstance(KSQLTableSchema& schema, KVarBinaryMemory& vbm, ITableCacheCreator* pCreator=NULL);
	static KTableCache* CreateInstance(KSQLTableSchema& schema, ITableCacheCreator* pCreator=NULL,KVarBinaryMemory* vbm=NULL);

	// 对外接口
public:
	BOOL Initialize(const KSQLConnParam& cnnParam, const KSQLTableSchema& schema, const CreateParam& createParam);
	void Finalize();

public: // var-binary字段的操作接口
	KVarBinary GetVarBinary(HANDLE hRec, int colIdx);
	KVarBinary GetVarBinary(HANDLE hRec, const char* colName);

private: // 内部使用
	KVarBinary _getVarBinary(KRecordHead* pHead, int colIdx);

public: // 内部使用
	void _markAllPartialModified(HANDLE hRec);
	int _getPartialModifiedCount(HANDLE hRec);
	int _getVarBinaryCols(int* cols, int count);
	void _assureVarBinaryCol(HANDLE hRec, int col); // 检查varbinary字段的存储，如果不一致，则设置改字段未改变
	void _assureVarBinaryCols(HANDLE hRec, int* cols, int count);

public:
	/**
	 * 参数中带上主键数据类型的长度是为了安全上的考虑
	 * 可以在模板派生类中进行长度检查，以尽早发现使用上的错误
	 */
	HANDLE FindRecord(const void* pKey, int len);
	HANDLE LoadRecord(const void* pKey, int len);
	HANDLE CreateRecord(const void* pKey, int len);
	BOOL DiscardRecord(const void* pKey, int len);
	BOOL DeleteRecord(const void* pKey, int len);
	BOOL CheckoutRecord(const void* pKey, int len);
	BOOL EmergencyCheckout(const void* pKey, int len, void* pUserData);

	BOOL DiscardRecord2(HANDLE hRec);
	BOOL DeleteRecord2(HANDLE hRec);
	BOOL CheckoutRecord2(HANDLE hRec);

	HANDLE CreateRecordDirect(void* pKey, int len);

	// 以下部分内部使用
public:
	virtual BOOL SetPrimaryKey(KCacheObject& cache, const void* pKey) = 0;
	virtual BOOL SetSQLParam(KSQLCommand& cmd, int paramBaseIdx, KCacheObject& cache) = 0;

	virtual System::Collections::KString<512> Key2String(const void* pKey) const = 0;
	virtual System::Collections::KString<512> GetKeyString(HANDLE hRec) const = 0;

	virtual BOOL GetPrimaryKey(KCacheObject& cache, void* pKey) = 0;
	virtual BOOL IsPrimaryKey(KCacheObject& cache, const void* pKey) = 0;
	virtual BOOL SetAutoIncreaseKey(KCacheObject& cache, UINT64 key) = 0;
	
public:
	BOOL IsValidHandle(HANDLE hRec);
	int GetFieldIndex(const char* fieldName) const;
	const char* GetTableName() const;
	int GetRecordDataLength() const;
	void* GetRecordData(HANDLE hRec);
	KRecordHead* GetRecordHead(HANDLE hRec);

protected:
	// 检查PKEY::key_type的长度
	virtual BOOL _checkKeyLength(int len) const = 0;

	virtual HANDLE _FindRecord(const void* pKey) = 0;

	// 加载了一条新的记录，添加到内部的MAP中
	virtual BOOL OnRecordLoaded(HANDLE hRec) = 0;
	
	// 卸载了一条记录，从MAP中删除
	virtual BOOL OnRecordUnload(HANDLE hRec) = 0;

protected:
	// 数据库操作，加载一条记录到内存中
	BOOL _LoadRecord(HANDLE hRec);

	// 数据库操作，插入一条记录
	BOOL _InsertRecord(HANDLE hRec);

	// 数据库操作，以内存内容更新数据库中的记录
	BOOL _UpdateRecord(HANDLE hRec);

	// 数据库操作，以内存内容更新数据库中的记录，仅更新标记为修改了的字段
	BOOL _PartialUpdateRecord(HANDLE hRec);

	// 数据库操作，construction数据库中删除记录
	BOOL _DeleteRecord(HANDLE hRec);

protected: // 同步线程使用
	BOOL RealInsertRecord(HANDLE hRec);
	BOOL RealUpdateRecord(HANDLE hRec);
	BOOL RealPartialUpdateRecord(HANDLE hRec);
	BOOL RealDeleteRecord(HANDLE hRec);
	BOOL RealCheckoutRecord(HANDLE hRec);

public:
	void ClearSqlError();
	DWORD GetLastSqlError() const;
	const char* GetLastSqlErrorMessage() const;

public:
	BOOL isConnActive();
	BOOL reconnect();
	BOOL keepConnectionFresh();

protected:
	// 从紧急队列中取出一个
	BOOL QueuePush(KEmergencyEvent& event);
	BOOL QueuePop(KEmergencyEvent& event);
	BOOL QueueWait(int ms);
	void OnEmergencyFinished(BOOL success, void* pUserData);

private:
	BOOL _CreateInsertSql();
	BOOL _CreateUpdateSql();
	int  _CreatePartialUpdateSQL(HANDLE hRec); // 返回参数个数
	BOOL _CreateDeleteSql();
	BOOL _CreateSelectSql();

private:
	HANDLE  _AllocRecord();
	HANDLE  _AllocFromBottom();
	HANDLE  _AllocFromFree();
	void    _FreeRecord(HANDLE hRec);
	void	_initRecord(KRecordHead* pHead);
	void	_initVbColsInfo();

	inline BOOL   _IsValidHandle(HANDLE hRec);
	KRecordHead*  _GetRecordHead(HANDLE recNo);

	BOOL _GetBinary(HANDLE hRec, int fldIdx, int offset, void* value, int len, int& readed);
	BOOL _SetBinary(HANDLE hRec, int fldIdx, int offset, const void* value, int len);

private:
	KMMap m_mmap;
	KFileHead* m_pFileHead;

	int m_recSize;
	DWORD m_recordCapacity;
	int m_recordDataLength;
	KSQLTableSchema m_schema;

	int m_modifyFlagSize;
	int* m_dataOffsets;

	char* m_pStartMemory;
	char* m_pEndMemory;

	KSQLCommand m_sqlCmd;
	KSQLResultSet m_sqlRs;
	KSQLConnection m_sqlCnn;
	System::Collections::KDString<256> m_sqlInsert;
	System::Collections::KDString<256> m_sqlUpdate;
	System::Collections::KDString<256> m_sqlPartialUpdate;
	System::Collections::KDString<256> m_sqlDelete;
	System::Collections::KDString<256> m_sqlSelect;
	KSQLFieldDescribe* m_sqlParams;

	System::Sync::KThreadMutex m_mx;
	KRefreshThread m_refreshThread;

	BOOL m_resetMemory;
	BOOL m_doFlush;

	// 及时处理队列
	System::Collections::KQueueFifo<KEmergencyEvent,System::Sync::KMTLock,512> m_emergencyQueue;
	IEmergencyObserver* m_pEmergencyObserver;

	// 提供varbinary的存储空间
	KVarBinaryMemory* m_vbm;
	int m_vbColNum;				// var binary 字段数量
	int* m_vbCols;				// var binary 字段位置
};

#endif
