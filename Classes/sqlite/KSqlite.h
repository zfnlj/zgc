#ifndef _KSQLITE_H
#define _KSQLITE_H

#define  SQLITE_FILE_NAME "zgc.dat"

#include "../sqlite/sqlite3.h"

#define LOAD_BLOB_FIELD(column,dbbinary,buf)			\
	dbbinary.Set(NULL,0);								\
	if(column){											\
		int len = KSqlite::loadBlobBuf(buf,2048,column);\
		if(len>=0) dbbinary.Set(buf,len);				\
	}

struct execLoadSqlStruct{
    int(*_fn_loadRecord)(void*,int,char**,char**);
    const char* _table;
    const char* _sql;
    void* _param;
};

struct execUpdateSqlStruct{
	const char* _table;
	const char* _sql_update;
};

struct execSaveSqlStruct{
    const char* _table;
	const char* _createField;
    const char* _sql_count;
    const char* _sql_insert;
    const char* _sql_update;
    void* _param;
};

class KSqlite
{
public:
	KSqlite(){}
	~KSqlite(){}
    static sqlite3* prepareTableInDB(const char* table,const char* dbFileName,const char* createField="");
    static bool saveToSQLite(execSaveSqlStruct*);
	static bool updateToSQLite(execUpdateSqlStruct*);
    static bool readFromSQLite(execLoadSqlStruct*);
	
	
	static int loadBlobBuf(char* des,int size,char* src);
	static bool updateBlobBinaryData(const char* table,const char* sqlstr,const char* buf,int len);

private:
	static bool updateBlob(const char* table,const char* sqlstr,const char* buf,int len);
	static int fillBlobBuf(char* des,int size,const char* src,int len);
};

#endif // __HELLOWORLD_SCENE_H__
