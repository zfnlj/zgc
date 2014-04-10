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
* ���������¼�������Oberver����õ�֪ͨ
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
* ���ݿ����ڴ�ӳ���ļ�Cache
*/

/**
 * ��Ϊ Cache Table ������������������Ҫ���㼸������
 *
 * 1. public inner type (1) key_type (2) key_cmp
 * 2. BOOL SetPrimaryKey(KCacheObject& cache, const key_type& val)
 * 3. BOOL GetPrimaryKey(KCacheObject& cache, key_type& val) const
 * 4. BOOL SetSQLParam(KSQLCommand& cmd, int paramBaseIdx, KCacheObject& cache)
 * 5. KString<512> ToString(const key_type& val) const
 *
 * ����ٶ��������һ���Ǵӵ�һ���ֶο�ʼ�����ֶ�֮����������
 *
 *
 * ȱʡ������������ʹ�ù���
 *
 * ������ֵ��(�Ǹ���)����
 * 1. ����С�ڵ���4�ֽ�ʱʹ��DWORDΪ������������
 * 2. ���ȴ���4�ֽ�ʱʹ��UINT64Ϊ������������
 * 3. �ַ�������������С��64ʱ��ʹ��KString<64>Ϊ������������
 *
 * �����ֶ���������
 * KPair<first, second>
 * ����first��second��ѡȡ����ͬ��
 *
 * ������������߲�Ըʹ������Ĺ�����Ҫ�Լ���չ
 *
 *
 * KTableCache�ľ����÷���ο� petsworkshop\V001\Test\tsqldb
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
		c_new_created		= 0,  // ������¼���������ڴ��У����ݿ��л�û��
		c_updating			= 1,  // Ӧ�ó��������޸ļ�¼�е�����
		c_modified			= 2,  // ������¼���޸Ĺ���
		c_deleted			= 3,  // ������¼��Ҫɾ��
		c_checkout			= 4,  // ����Ҫ������Cache������
		c_saving			= 5,  // ���ڰѼ�¼���浽���ݿ�
		c_loading			= 6,  // ���ڴ����ݿ����
		c_partial_modified  = 7,  // ������¼�в����ֶα��޸���
		c_new_record		= 8,  // �ڴ����´����ļ�¼���ȴ������߳�д��
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
		DWORD recSize;        // ��¼�ĳ��ȣ�������¼��ͷ��
		DWORD capacity;       // �������ɶ�������¼
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
		void Execute();  // �̺߳���
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

	// ����ӿ�
public:
	BOOL Initialize(const KSQLConnParam& cnnParam, const KSQLTableSchema& schema, const CreateParam& createParam);
	void Finalize();

public: // var-binary�ֶεĲ����ӿ�
	KVarBinary GetVarBinary(HANDLE hRec, int colIdx);
	KVarBinary GetVarBinary(HANDLE hRec, const char* colName);

private: // �ڲ�ʹ��
	KVarBinary _getVarBinary(KRecordHead* pHead, int colIdx);

public: // �ڲ�ʹ��
	void _markAllPartialModified(HANDLE hRec);
	int _getPartialModifiedCount(HANDLE hRec);
	int _getVarBinaryCols(int* cols, int count);
	void _assureVarBinaryCol(HANDLE hRec, int col); // ���varbinary�ֶεĴ洢�������һ�£������ø��ֶ�δ�ı�
	void _assureVarBinaryCols(HANDLE hRec, int* cols, int count);

public:
	/**
	 * �����д��������������͵ĳ�����Ϊ�˰�ȫ�ϵĿ���
	 * ������ģ���������н��г��ȼ�飬�Ծ��緢��ʹ���ϵĴ���
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

	// ���²����ڲ�ʹ��
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
	// ���PKEY::key_type�ĳ���
	virtual BOOL _checkKeyLength(int len) const = 0;

	virtual HANDLE _FindRecord(const void* pKey) = 0;

	// ������һ���µļ�¼����ӵ��ڲ���MAP��
	virtual BOOL OnRecordLoaded(HANDLE hRec) = 0;
	
	// ж����һ����¼����MAP��ɾ��
	virtual BOOL OnRecordUnload(HANDLE hRec) = 0;

protected:
	// ���ݿ����������һ����¼���ڴ���
	BOOL _LoadRecord(HANDLE hRec);

	// ���ݿ����������һ����¼
	BOOL _InsertRecord(HANDLE hRec);

	// ���ݿ���������ڴ����ݸ������ݿ��еļ�¼
	BOOL _UpdateRecord(HANDLE hRec);

	// ���ݿ���������ڴ����ݸ������ݿ��еļ�¼�������±��Ϊ�޸��˵��ֶ�
	BOOL _PartialUpdateRecord(HANDLE hRec);

	// ���ݿ������construction���ݿ���ɾ����¼
	BOOL _DeleteRecord(HANDLE hRec);

protected: // ͬ���߳�ʹ��
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
	// �ӽ���������ȡ��һ��
	BOOL QueuePush(KEmergencyEvent& event);
	BOOL QueuePop(KEmergencyEvent& event);
	BOOL QueueWait(int ms);
	void OnEmergencyFinished(BOOL success, void* pUserData);

private:
	BOOL _CreateInsertSql();
	BOOL _CreateUpdateSql();
	int  _CreatePartialUpdateSQL(HANDLE hRec); // ���ز�������
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

	// ��ʱ�������
	System::Collections::KQueueFifo<KEmergencyEvent,System::Sync::KMTLock,512> m_emergencyQueue;
	IEmergencyObserver* m_pEmergencyObserver;

	// �ṩvarbinary�Ĵ洢�ռ�
	KVarBinaryMemory* m_vbm;
	int m_vbColNum;				// var binary �ֶ�����
	int* m_vbCols;				// var binary �ֶ�λ��
};

#endif
