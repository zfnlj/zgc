#include "KSqlite.h"
#include <string>
#include "platform/CCFileUtils.h"
#include "ccMacros.h"
#include "CommonLogic/KCacheObject.h"

using namespace std;





/*
int loadUserRecord(void* para,int n_cloumn,char** column_value,char** column_name)
{
    UserRecord* record = (UserRecord*)para;
    int coin,experience;
    sscanf(column_value[1],"%d",&coin);
    sscanf(column_value[2],"%d",&experience);
    record->setCoin(coin);
    record->setExp(experience);
}
*/
int loadRecordCount(void* param,int n_column,char** column_value,char** column_name)
{
    int * pCount = (int*)param;
    sscanf(column_value[0],"%d", pCount);
    return 0;
}

sqlite3* KSqlite::prepareTableInDB(const char* table,const char* dbFileName,const char* createField)
{
    sqlite3 *pDB = NULL;
    char* errorMsg = NULL;
    if(SQLITE_OK!= sqlite3_open(dbFileName,&pDB)){
        CCLOG("open sql file failed");
        return NULL;
    }
	if(strlen(createField)==0) return pDB;

    string sql = "create table if not exists " + string(table) + string(createField);
                //"(id char(80) primary key,coin integer,experience integer)";
    sqlite3_exec(pDB,sql.c_str(),NULL,NULL,&errorMsg);
    if(errorMsg != NULL){
        CCLOG("exec sql %s fail with msg: %s",sql.c_str(),errorMsg);
        sqlite3_close(pDB);
        return NULL;
    }
    return pDB;
}

bool KSqlite::updateToSQLite(execUpdateSqlStruct* exec)
{
	char sql[1024];
	std::string fullPath;
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(SQLITE_FILE_NAME);
	sqlite3* pDB = prepareTableInDB(exec->_table, fullPath.c_str());
	if(pDB){
		char* errorMsg;
		sqlite3_exec(pDB,exec->_sql_update,NULL,NULL,&errorMsg);
		if(errorMsg){
			CCLOG("exec sql %s fail with msg: %s",sql ,errorMsg);
			sqlite3_close(pDB);
			return false;
		}
		sqlite3_close(pDB);
		return true;
	}
	return false;
}



bool KSqlite::updateBlobBinaryData(const char* table,const char* sqlstr,const char* data,int actualLength)
{
	char buf[2048];
	int len =KSqlite::fillBlobBuf(buf,2048,data,actualLength);
	if(len<0) return false;

	return KSqlite::updateBlob(table,sqlstr,buf,len);
}

bool KSqlite::updateBlob(const char* table,const char* sqlstr,const char* buf,int len)
{
	std::string fullPath;
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(SQLITE_FILE_NAME);
	sqlite3* pDB = prepareTableInDB(table, fullPath.c_str());
	if(!pDB) return false;
	sqlite3_stmt      *pstmt = 0;
	int ret;
	ret = sqlite3_prepare(pDB, sqlstr, -1, &pstmt, NULL);
	if( ret != SQLITE_OK ){
		sqlite3_close(pDB);
		return false;
	}
	ret = sqlite3_bind_blob(pstmt, 1, buf, len, NULL);
	if( ret != SQLITE_OK ){
		sqlite3_close(pDB);
		return false;
	}
	ret = sqlite3_step(pstmt);
	sqlite3_close(pDB);
	return (ret==SQLITE_DONE);
}

bool KSqlite::saveToSQLite(execSaveSqlStruct*exec)
{
    char sql[1024];
	std::string fullPath;
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(SQLITE_FILE_NAME);
    sqlite3* pDB = prepareTableInDB(exec->_table, fullPath.c_str(),exec->_createField);
    if(pDB){
        int count = 0 ;
        char* errorMsg;
        sqlite3_exec(pDB,exec->_sql_count,loadRecordCount,&count,&errorMsg);
        if(count){
            sqlite3_exec(pDB,exec->_sql_update,NULL,NULL,&errorMsg);
        }else{
            sqlite3_exec(pDB,exec->_sql_insert,NULL,NULL,&errorMsg);
        }
        if(errorMsg){
            CCLOG("exec sql %s fail with msg: %s",sql ,errorMsg);
            sqlite3_close(pDB);
            return false;
        }
        sqlite3_close(pDB);
		return true;
    }
	return false;
}

bool KSqlite::readFromSQLite(execLoadSqlStruct* exec)
{
    char sql[1024];
    sqlite3* pDB = prepareTableInDB(exec->_table,SQLITE_FILE_NAME);
    if(pDB != NULL){
        char* errorMsg;
        sqlite3_exec(pDB,exec->_sql,exec->_fn_loadRecord,exec->_param,&errorMsg);
        
        if(errorMsg != NULL){
            CCLOG("exec sql %s fail with msg:%s", sql,errorMsg);
            sqlite3_close(pDB);
            return false;
        }
		sqlite3_close(pDB);
		return true;
    }
    return false;
}

int KSqlite::fillBlobBuf(char* des,int size,const char* src,int len)
{
	if(size < (len+sizeof(KDbBinaryHead))) return -1;
	KDbBinaryHead* pHead = (KDbBinaryHead*)des;
	pHead->length = len;
	memcpy(des+sizeof(KDbBinaryHead),src,len);
	return len+sizeof(KDbBinaryHead);
}

int KSqlite::loadBlobBuf(char* des,int size,char* src)
{
	KDbBinaryHead head;
	memcpy(&head,src,sizeof(head));
	if(size <head.length) return -1;
	memcpy(des,src+sizeof(head),head.length);
	return head.length;
}

