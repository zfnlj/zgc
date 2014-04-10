#ifndef _K_DATABASE_H_
#define _K_DATABASE_H_

#include <System/KType.h>
#include <System/Collections/KString.h>
#include <System/Collections/KVector.h>
#include <System/Misc/KDatetime.h>

#if defined(WIN32) || defined(WINDOWS)
#include <WinSock2.h>
#else

#endif

#include <mysql/mysql.h>
#include <System/KPlatform.h>

/**
 * ���ݿ������
 */

class KSQLCommand;
class KSQLResultSet;
class KSQLTableSchema;

class KSQLTypes
{
public:
	enum eCDataType
	{
		sql_c_invalid = -1,
		sql_c_int8      = 0,
		sql_c_uint8     = 1,
		sql_c_int16     = 2,
		sql_c_uint16    = 3,
		sql_c_int32     = 4,
		sql_c_uint32    = 5,
		sql_c_int64     = 6,
		sql_c_uint64    = 7,
		sql_c_float32   = 8,
		sql_c_float64   = 9,
		sql_c_string    = 10,
		sql_c_varstring = 11,
		sql_c_binary    = 12,
		sql_c_timestamp = 13,
		sql_c_varbinary	= 14,
		sql_c_max
	};

public:
	static eCDataType MapToCType(enum_field_types sqlDatatype, DWORD fldFlags, DWORD length);
	static enum_field_types MapToSqlType(eCDataType ctype);
	static BOOL IsUnsignedInteger(eCDataType ctype);
	static int GetCDataWidth(eCDataType ctype);
	static eCDataType MapToCType(const char* typeName);
};

// �ڿ��ٲ�ѯʱ�ɵ������ṩÿ���ֶε���Ϣ
struct KSQLFieldDescribe
{
	int m_length;
	System::Collections::KString<32> m_name;
	KSQLTypes::eCDataType m_cdt;

	// �����ֶ����ڲ�ʹ�ã���Ҫֱ�Ӷ���Щ�ֶθ�ֵ
	void*                 _buffer;
	unsigned long         _actualLength;
	my_bool               _isnull;
	my_bool               _iserror;
};

class KSQLConnParam
{
public:
	enum { MYSQL_DEFAULT_PORT=3306 };

public:
	KSQLConnParam();
	KSQLConnParam(const KSQLConnParam& o);
	~KSQLConnParam();
	KSQLConnParam& operator=(const KSQLConnParam& o);

public:
	void SetHost(const char* host);
	void SetServerPort(unsigned int serverPort);
	void SetDbName(const char* dbName);
	void SetUserName(const char* userName);
	void SetPassword(const char* passwd);
	void SetCharset(const char* charset);

public:
	unsigned int GetServerPort() const;
	const char* GetHost() const;
	const char* GetDbName() const;
	const char* GetUserName() const;
	const char* GetPasswd() const;
	const char* GetCharset() const;

public:
	unsigned int m_serverPort;
	System::Collections::KString<32> m_host;
	System::Collections::KString<32> m_dbName;
	System::Collections::KString<32> m_userName;
	System::Collections::KString<32> m_passwd;
	System::Collections::KString<32> m_charset;
};

class KSQLConnection
{
private:
	friend class KSQLCommand;
	friend class KSQLResultSet;
	friend class KTableCache;

public:
	KSQLConnection();
	~KSQLConnection();

public:
	BOOL SetDefaultCharset(const char* charset);
	BOOL SetAutoReconnect(BOOL autoReconnect);
	BOOL SetConnectTimeout(int sec);

public:
	void SetLifetime(int lifetime);
	// ʹ������ӱ������ʵ�״̬��������ʹ����һ��ʱ�������һ��
	BOOL KeepConnectionFresh();

	BOOL IsActive() const;
	BOOL Connect(const KSQLConnParam& connParam);
	BOOL Reconnect();
	BOOL SelectDatabase(const char* dbName);
	void Close();

public:
	int GetLastError() const;
	const char* GetLastErrorMsg() const;

public:
	// ����������԰������ݿ����ƣ���: DBGame.tb_Skill
	BOOL QueryTableSchema(const char* tableName, KSQLTableSchema& tableSchema);
	UINT64 GetLastAutoIncID();

public:
	void ClearError();

private:
	void AcquireErrorInfo();
	void AcquireStmtError(MYSQL_STMT* stmt);
	void SetLastError(DWORD errCode, const char* fmt, ...);

private:
	int m_connTimeout;
	BOOL m_autoReconnect;
	System::Collections::KString<32> m_defCharset;

	MYSQL* m_pMysql;
	KSQLConnParam m_connParam;

	DWORD m_errCode;
	System::Collections::KDString<512> m_errmsg;

	TIME_T m_cnnTime;   // �������ӵ�ʱ��
	int m_cnnLifetime;  // һ�����ӿɳ���ʹ�õ�ʱ��(��)
};

class KSQLCommand
{
public:
	friend class KTableCache;
	enum
	{
		c_def_param_buffer_len = 1024,
		c_def_bind_num = 10,
	};

public:
	KSQLCommand();
	KSQLCommand(KSQLConnection& conn);
	~KSQLCommand();

public:
	void SetConnection(KSQLConnection& cnn);
	int GetLastError() const;
	const char* GetLastErrorMsg() const;

public:
	BOOL SetSQLStatement(const char* sql);

	// ���SQL������Ǵ�������(������ ?)������Ĳ�����Execute֮ǰ�Ǳ����
	BOOL DescribeParameters(KSQLFieldDescribe* describes, int num);

	// Ϊ��������ֵ
	BOOL SetNull(const char* paramName);
	BOOL SetInteger(const char* paramName, int value);
	BOOL SetInt64(const char* paramName, INT64 value);
	BOOL SetFloat(const char* paramName, float value);
	BOOL SetFloat64(const char* paramName, double value);
	BOOL SetString(const char* paramName, const char* value, int len=0);
	BOOL SetBinary(const char* paramName, const void* value, int len);
	BOOL SetTimestamp(const char* paramName, TIME_T value);
	BOOL SetTimestamp(const char* paramName, const char* strDatetime);

	BOOL SetNull(int paramIdx);
	BOOL SetInteger(int paramIdx, int value);
	BOOL SetInt64(int paramIdx, INT64 value);
	BOOL SetFloat(int paramIdx, float value);
	BOOL SetFloat64(int paramIdx, double value);
	BOOL SetString(int paramIdx, const char* value, int len=0);
	BOOL SetBinary(int paramIdx, const void* value, int len);
	BOOL SetTimestamp(int paramIdx, TIME_T value);
	BOOL SetTimestamp(int paramIdx, const char* strDatetime);

	// һ��SQL�����Ա����ִ�У�ʹ�ò�ͬ�Ĳ���ֵ
	BOOL Execute();
	BOOL ExecuteQuery(KSQLResultSet& recordset);

	// ֱ��ָ��SQL���ִ�У�SQL��䲻�ܴ�����
	BOOL Execute(const char* sql);
	BOOL ExecuteQuery(const char* sql, KSQLResultSet& rs);

public:
	int GetParamIndex(const char* paramName);
	void Reset();          // �����׼��ִ����һ��SQL
	BOOL Prepare();        // Ϊ�µ�SQL�����׼��
	void Finalize();       // �ͷ����е���Դ
	BOOL BindParameters(); // �󶨲���

public:
	KSQLFieldDescribe* _getParameter(int paramIdx);

public:
	char* m_buffer;
	int m_bufferCapacity;
	char* m_dynamicBuffer;
	char  m_staticBuffer[c_def_param_buffer_len];

	MYSQL_BIND* m_binds;
	int m_bindCapacity;
	MYSQL_BIND* m_dynamicBinds;
	MYSQL_BIND m_staticBinds[c_def_bind_num];

	int m_paramNum;
	KSQLConnection* m_pConn;
	KSQLFieldDescribe* m_params;

	MYSQL_STMT* m_stmt;
	System::Collections::KDString<256> m_sqlStatement;
};

class KSQLResultSet
{
public:
	friend class KSQLCommand;
	friend class KTableCache;
	enum { def_string_len=256, def_binary_len=1024 };
	enum { c_def_param_buffer_len = 1024, c_def_bind_num = 10 };

public:
	KSQLResultSet();
	~KSQLResultSet();

public:
	int GetLastError() const;
	const char* GetLastErrorMsg() const;

public:
	BOOL DescribeColumns(KSQLFieldDescribe* cols, int num);
	BOOL BuildDescribes(); // ʹ��mysql_stmt_result_metadata�����ֶ�����

	// һ��ֻ��Ҫ��BLOB�ֶν������ã���ΪBLOB�����ݿ��ж���ĳ��Ȼ�ܴ�
	// ��ʵ���ϴ洢������ȴ����̫��
	// ֻ�����õĳ��ȱȱ��ж�����ֶγ��ȶ̣��Ż���������
	BOOL SetColumnLength(const char* colName, int len);
	BOOL SetColumnLength(int colIdx, int len);

	BOOL Begin();
	BOOL Next();
	
public:
	BOOL IsNull(const char* colName, BOOL& isnull);
	BOOL GetInteger(const char* colName, int& value, int defVal=0);
	BOOL GetInt64(const char* colName, INT64& value, INT64 defVal=0);
	BOOL GetFloat(const char* colName, float& value, float defVal=0.0f);
	
	// len Ϊ�����������������ʱΪ���������ȣ����ʱΪʵ�ʿ������ֽ���
	BOOL GetString(const char* colName, char* value, int& len);
	BOOL GetBinary(const char* colName, void* value, int& len);

	// len Ϊ���������ΪҪ����ֽ���
	BOOL GetString(const char* colName, int offset, char* value, int len);
	BOOL GetBinary(const char* colName, int offset, void* value, int len);

	BOOL IsNull(int colIndex, BOOL& isnull);
	BOOL GetInteger(int colIndex, int& value, int defVal=0);
	BOOL GetInt64(int colIdx, INT64& value, INT64 defVal=0);
	BOOL GetFloat(int colIndex, float& value, float defVal=0.0f);
	BOOL GetString(int colIndex, char* value, int& len);
	BOOL GetBinary(int colIndex, void* value, int& len);
	BOOL GetString(int colIndex, int offset, char* value, int len);
	BOOL GetBinary(int colIndex, int offset, void* value, int len);

	// get binary data pointer and return data length
	// if failed, return -1
	int _getBinary(int colIndex, void** pp);

	void* _GetRecvBuffer();
	
private:
	int GetColumnIndex(const char* colName);
	void Reset();              // ������������һ�������
	void Finalize();
	BOOL BindResult();         // ���ڲ��������󶨵�����������

private:
	MYSQL_STMT* m_stmt;                           // �����洫�룬������ر�
	KSQLConnection* m_pconn;

	char* m_buffer;
	int m_bufferCapacity;
	char* m_dynamicBuffer;
	char  m_staticBuffer[c_def_param_buffer_len];

	MYSQL_BIND* m_binds;
	int m_bindCapacity;
	MYSQL_BIND* m_dynamicBinds;
	MYSQL_BIND m_staticBinds[c_def_bind_num];

	int m_colNum;                   // ������еļ�¼�ж�����
	int m_colCapacity;              // ���������
	KSQLFieldDescribe* m_describes; // ������е��ֶ���������
	KSQLFieldDescribe* m_outDescribes;
	KSQLFieldDescribe* m_myDescribes;
};

class KSQLTableField
{
public:
	KSQLTableField();
	KSQLTableField(const KSQLTableField& o);
	~KSQLTableField();
	KSQLTableField& operator=(const KSQLTableField& o);

public:
	int  m_length;
	BOOL m_nullable;
	BOOL m_autoIncrement;
	BOOL m_primaryKey;
	KSQLTypes::eCDataType m_cdt;
	System::Collections::KString<32> m_name;
	System::Collections::KString<32> m_defaultVal;
};

class KSQLTableSchema
{
public:
	KSQLTableSchema();
	~KSQLTableSchema();

public:
	KSQLTableSchema& Append(const KSQLTableField& field);
	bool SetFieldLength(const char* colName, int len);
	bool SetFieldLength(int colIndex, int len);

public:
	int GetFieldCount() const;
	const KSQLTableField* GetField(const char* fieldName) const;
	const KSQLTableField* GetField(int fieldIndex) const;

	// �ٶ����������������ֶε�ǰ����������
	// �����ɶ����ֶ����
	int GetPrimaryKeyFieldCount() const;

	void Clear();

public:
	System::Collections::KString<32> m_name;
	System::Collections::KString<32> m_dbName;
	System::Collections::KVector<KSQLTableField> m_fields;
};

#endif
