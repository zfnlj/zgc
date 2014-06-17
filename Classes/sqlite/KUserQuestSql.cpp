#include "KUserQuestSql.h"
#include <string>
#include "ccMacros.h"
#include "KSqlite.h"
#include "CommonLogic/KDBDataStruct.h"

#define  SQLITE_QUEST_TABLE "quest_table"
//#define  SQLITE_USER_CREATE "(name char(80) primary key,money integer)";
#define  SQLITE_QUEST_CREATE "(name char(80) primary key,Qid_0 integer, QState_0 blob,Qid_1 integer, QState_1 blob,\
								Qid_2 integer, QState_2 blob,QHistory blob,QDaily blob)";

enum quest_field_enum
{
	f_name,
	f_qid0,
	f_qstate0,
	f_qid1,
	f_qstate1,
	f_qid2,
	f_qstate2,
	f_qhistory,
	f_qdaily,
};

								

int loadUserQuest(void* para,int n_cloumn,char** column_value,char** column_name)
{
	char buf[2048];
	int len = 0;
	tb_playerquest_record* record = (tb_playerquest_record*)para;

	for(int i=0;i<MAX_PLAYER_QUEST_NUM;i++){
		sscanf(column_value[f_qid0+i*2],"%d",&record->qid[i]);
		LOAD_BLOB_FIELD(column_value[f_qstate0+i*2],record->qstate[i],buf)
	}
	LOAD_BLOB_FIELD(column_value[f_qhistory],record->qhistory,buf)
	LOAD_BLOB_FIELD(column_value[f_qdaily],record->qdaily,buf)
	return 0;
}

bool KUserQuestSql::UpdateIntegerVal(const char* userName,const char* field,int val)
{
	execUpdateSqlStruct execSql;
	char sqlUpdate[128];
	sprintf(sqlUpdate,"update %s set %s=%d where name='%s'",SQLITE_QUEST_TABLE,field,val,userName);
	execSql._sql_update = sqlUpdate;
	execSql._table = SQLITE_QUEST_TABLE;
	return KSqlite::updateToSQLite(&execSql);
}

bool KUserQuestSql::UpdateQuestHistory(const char* userName,tb_playerquest_record* record)
{
	char sqlstr[128];
	sprintf(sqlstr,"update %s set QHistory=? where name='%s'",SQLITE_QUEST_TABLE,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_QUEST_TABLE,sqlstr,record->qhistory.binData,record->qhistory.actualLength);
}

bool KUserQuestSql::UpdateQuestDaily(const char* userName,tb_playerquest_record* record)
{
	char sqlstr[128];
	sprintf(sqlstr,"update %s set QDaily=? where name='%s'",SQLITE_QUEST_TABLE,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_QUEST_TABLE,sqlstr,record->qdaily.binData,record->qdaily.actualLength);
}

bool KUserQuestSql::UpdateQuestState(const char* userName,int index,tb_playerquest_record* record)
{
	char sqlstr[128];
	sprintf(sqlstr,"update %s set QState_%d=? where name='%s'",SQLITE_QUEST_TABLE,index,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_QUEST_TABLE,sqlstr,record->qstate[index].binData,record->qstate[index].actualLength);
}

bool KUserQuestSql::LoadUserQuestData(const char* userName,tb_playerquest_record* record)
{

	char sqlstr[128];

	sprintf(sqlstr,"select * from %s where name='%s'",SQLITE_QUEST_TABLE,userName);
	execLoadSqlStruct execSql;
	execSql._table = SQLITE_QUEST_TABLE;
	execSql._sql = sqlstr;
	execSql._fn_loadRecord = loadUserQuest;
	execSql._param = record;
	return KSqlite::readFromSQLite(&execSql);
}

bool KUserQuestSql::InsertUserQuestData(const char* userName)
{
	char sqlcount[128];
	sprintf(sqlcount,"select count(*) from %s where name = '%s'",SQLITE_QUEST_TABLE,userName);
	char sqlInsert[128];
	sprintf(sqlInsert,"insert into %s (name,Qid_0,Qid_1,Qid_2) values ( '%s',0,0,0)",SQLITE_QUEST_TABLE,userName);
	execSaveSqlStruct execSql;
	execSql._table = SQLITE_QUEST_TABLE;
	execSql._sql_count = sqlcount;
	execSql._createField = SQLITE_QUEST_CREATE;
	execSql._sql_insert = sqlInsert;
	execSql._sql_update = "";
	return  KSqlite::saveToSQLite(&execSql);
}
