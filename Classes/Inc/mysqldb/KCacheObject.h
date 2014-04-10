#ifndef _K_CACHE_OBJECT_H_
#define _K_CACHE_OBJECT_H_

#include <System/KType.h>
#include "KTableCache.h"

/**
 * �����ڴ���һ����¼���ڴ�ӳ��ĸ�ʽ
 *
 * 1. �߽�����BYTE����� pack(1)
 * 2. �ַ������͵��ֶβ��õ�ǰ�������ֽڵĳ��ȣ������û�н��������ַ�������
 * 3. �����Ƶ��ֶβ���ǰ�������ֽڵĳ��ȣ����������
 * 4. �ַ���������Ҫע�����ݿ��ʹ�õ��ַ�������GB2312��һ���ַ�������BYTE
 * 
 * �����ѹ����ڴ��еļ�¼ֱ��ӳ��ΪӦ�ó����еĽṹ��
 * �������������ϸ�ԭ�򣬷���������Ԥ��
 */

#pragma pack(push,1)
typedef HANDLE KDBVarBinary;			// var-binary
template <int size> struct KDBString	// size �����ɵ����ݳ���
{
	WORD actualLength;  // ʵ�����ݳ���
	char strData[size]; // �����������ַ�
	BOOL Set(const char* str, int len=0)
	{
		if(!len) len = strlen(str);
		if(len < 0 || len > size) return FALSE;
		memcpy(strData, str, len);
		actualLength = len;
		return TRUE;
	}
	const char* Get(char* buf, int bufferLen) const
	{
		ASSERT_RETURN(bufferLen >= actualLength + 1, "");
		memcpy(buf, strData, actualLength);
		buf[actualLength] = '\0';
		return buf;
	}
};
template <int size> struct KDBBinary // size �����ɵ����ݳ���
{
	WORD actualLength;  // ʵ�����ݳ���
	char binData[size];
	BOOL Set(void* pData, int len)
	{
		if(len < 0 || len > size) return FALSE;
		memcpy(binData, pData, len);
		actualLength = len;
		return TRUE;
	}
	int Get(void*& buf)
	{
		buf = &binData[0];
		return actualLength;
	}
	int GetSize() const
	{
		return size;
	}
};
struct KDbBinaryHead
{
	WORD length;
	char data[2];
};
struct KDBTimestamp
{
	MYSQL_TIME val;
	BOOL Set(TIME_T t);
	TIME_T Get() const;
};
#pragma pack(pop)

class KCacheObject
{
public:
	KCacheObject();
	virtual ~KCacheObject();

public:
	// �����¼�����ݣ���Ҫ����������Binary�ֶε���ʵ����
	// ��������԰�Binary�ֶεĳ��ȵ���������Ϸ��ʵ���õ������ݳ���һ��
	virtual void TrimRecordData();

public:
	BOOL Attach(KTableCache* pCache, HANDLE hRec);
	void Detach();

public:
	BOOL BeginUpdate();
	BOOL EndUpdate();
	BOOL BeginUpdate(int fieldIdx);
	BOOL EndUpdate(int fieldIdx);
	BOOL Delete();
	BOOL Checkout();

public:
	KVarBinary GetVarBinary(int colIdx);
	KVarBinary GetVarBinary(const char* colName);

public:
	BOOL GetInteger(const char* colName, int& value);
	BOOL GetInt64(const char* colName, INT64& value);
	BOOL GetFloat32(const char* colName, float& value);
	BOOL GetFloat64(const char* colName, double& value);
	BOOL GetString(const char* colName, char* value, int len, int& readed);
	BOOL GetBinary(const char* colName, void* value, int len, int& readed);
	BOOL GetBinary(const char* colName, int offset, void* value, int len, int& readed);

public:
	BOOL GetInteger(int colIdx, int& value);
	BOOL GetInt64(int colIdx, INT64& value);
	BOOL GetFloat32(int colIdx, float& value);
	BOOL GetFloat64(int colIdx, double& value);
	BOOL GetString(int colIdx, char* value, int len, int& readed);
	BOOL GetBinary(int colIdx, void* value, int len, int& readed);
	BOOL GetBinary(int colIdx, int offset, void* value, int len, int& readed);

public:
	BOOL SetInteger(const char* colName, int value);
	BOOL SetInt64(const char* colName, INT64 value);
	BOOL SetFloat32(const char* colName, float  value);
	BOOL SetFloat64(const char* colName, double value);
	BOOL SetString(const char* colName, const char* value, int len=0);
	BOOL SetBinary(const char* colName, const void* value, int len);
	BOOL SetBinary(const char* colName, int offset, const void* value, int len);

public:
	BOOL SetInteger(int colIdx, int value);
	BOOL SetInt64(int colIdx, INT64 value);
	BOOL SetFloat32(int colIdx, float value);
	BOOL SetFloat64(int colIdx, double value);
	BOOL SetString(int colIdx, const char* value, int len=0);
	BOOL SetBinary(int colIdx, const void* value, int len);
	BOOL SetBinary(int colIdx, int offset, const void* value, int len);

protected:
	HANDLE m_hRec;
	KTableCache* m_pCache;
	int m_updateLevel;

public:
	// �Ѽ�¼���ڴ�ӳ��ֱ��ӳ��Ϊһ�����ݽṹ
	// ���ã���ο������ᵽ�ļ���ԭ��
	template <typename T> BOOL BindRecordTo(T*& ptr)
	{
		int recSize = m_pCache->GetRecordDataLength();
		if(recSize < sizeof(T)) return FALSE;

		void* pRecData = m_pCache->GetRecordData(m_hRec);
		if(!pRecData) return FALSE;
		ptr = (T*)pRecData;
		return TRUE;
	}

	// ���ֶε��ڴ�󶨵�һ���������͵�ָ��
	// ����String��Binary���ͣ���ʹ��KDBString��KDBBinary
	template <typename T> BOOL BindColumnTo(const char* colName, T*& ptr)
	{
		int fldIndex = m_pCache->GetFieldIndex(colName);
		if(fldIndex < 0) return FALSE;
		return this->BindColumnTo(fldIndex, ptr);
	}

	template <typename T> BOOL BindColumnTo(int colIdx, T*& ptr)
	{
		int colCount = m_pCache->m_schema.GetFieldCount();
		if(colIdx < 0 || colIdx >= colCount) return FALSE;

		const KSQLTableField* pfld = m_pCache->m_schema.GetField(colIdx);
		switch(pfld->m_cdt)
		{
		case KSQLTypes::sql_c_string:
		case KSQLTypes::sql_c_varstring:
		case KSQLTypes::sql_c_binary:
			{
				if(pfld->m_length+sizeof(WORD) < sizeof(T)) return FALSE;
				break;
			}
		default:
			{
				if(pfld->m_length < sizeof(T)) return FALSE;
				break;
			}
		}

		KTableCache::KRecordHead* pHead =  m_pCache->GetRecordHead(m_hRec);
		if(!pHead) return FALSE;

		char* pColData = (char*)pHead + m_pCache->m_dataOffsets[colIdx];
		ptr = (T*)pColData;

		return TRUE;
	}

	// ��Binary���͵��ֶΰ󶨵��ṹ�ϣ�����Ľṹ������ǰ���WORD (actualLength)
	// ����ɺ󣬻��actualLength��Ϊsizeof(T)
	template <typename T> BOOL BindBinaryTo(const char* colName, T*& ptr)
	{
		int fldIndex = m_pCache->GetFieldIndex(colName);
		if(fldIndex < 0) return FALSE;
		return this->BindBinaryTo(fldIndex, ptr);
	}

	template <typename T> BOOL BindBinaryTo(int colIdx, T*& ptr)
	{
		int colCount = m_pCache->m_schema.GetFieldCount();
		if(colIdx < 0 || colIdx >= colCount) return FALSE;

		const KSQLTableField* pfld = m_pCache->m_schema.GetField(colIdx);
		if(pfld->m_cdt != KSQLTypes::sql_c_binary) return FALSE;

		if(pfld->m_length < sizeof(T)) return FALSE;

		KTableCache::KRecordHead* pHead =  m_pCache->GetRecordHead(m_hRec);
		if(!pHead) return FALSE;

		char* pColData = (char*)pHead + m_pCache->m_dataOffsets[colIdx];
		ptr = (T*)(pColData+sizeof(WORD));

		if(this->BeginUpdate(colIdx))
		{
			*(WORD*)pColData = sizeof(T);
			this->EndUpdate(colIdx);
			return TRUE;
		}
		return FALSE;
	}
};

#endif
