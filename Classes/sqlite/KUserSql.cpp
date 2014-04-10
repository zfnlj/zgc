#include "KUserSql.h"
#include <string>
#include "ccMacros.h"
#include "KSqlite.h"
#include "CommonLogic/KDBDataStruct.h"

#define  SQLITE_USER_TABLE "user_table"
//#define  SQLITE_USER_CREATE "(name char(80) primary key,money integer)";
#define  SQLITE_USER_CREATE "(name char(80) primary key,money integer, \
								cardStore blob,cardDeck0 blob,cardDeck1 blob,cardDeck2 blob,cardDeck3 blob,cardDeck4 blob,\
								curDeck integer,pvpVal integer,exp integer,normalItem blob)";

enum player_field_enum
{
	f_name,
	f_money,
	f_cardStore,
	f_cardDeck0,
	f_cardDeck1,
	f_cardDeck2,
	f_cardDeck3,
	f_cardDeck4,
	f_curDeck,
	f_pvpVal,
	f_exp,
	f_normalItem,
};

int loadUserRecord(void* para,int n_cloumn,char** column_value,char** column_name)
{
	char buf[2048];
	tb_player_record* record = (tb_player_record*)para;
	sscanf(column_value[f_money],"%d",&record->money);
	
	LOAD_BLOB_FIELD(column_value[f_cardStore],record->cardStore,buf)
	LOAD_BLOB_FIELD(column_value[f_normalItem],record->normalItem,buf)

	for(int i=0;i<MAX_DECK_NUM;i++){
		LOAD_BLOB_FIELD(column_value[f_cardDeck0+i],record->cardDeck[i],buf)
	}
	if(column_value[f_curDeck]){
		sscanf(column_value[f_curDeck],"%d",&record->curDeck);
	}
	if(column_value[f_pvpVal]){
		sscanf(column_value[f_pvpVal],"%d",&record->pvpVal);
	}
	if(column_value[f_exp]){
		sscanf(column_value[f_exp],"%d",&record->exp);
	}
	return 0;
}

bool KUserSql::UpdateIntegerVal(const char* userName,const char* field,int val)
{
	execUpdateSqlStruct execSql;
	char sqlUpdate[128];
	sprintf(sqlUpdate,"update %s set %s=%d where name='%s'",SQLITE_USER_TABLE,field,val,userName);
	execSql._sql_update = sqlUpdate;
	execSql._table = SQLITE_USER_TABLE;
	return KSqlite::updateToSQLite(&execSql);
}

bool KUserSql::UpdateDeckStore(const char* userName,tb_player_record* record)
{
	char sqlstr[128];
	sprintf(sqlstr,"update %s set cardStore=? where name='%s'",SQLITE_USER_TABLE,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,record->cardStore.binData,record->cardStore.actualLength);
}

bool KUserSql::UpdateCardDeck(const char* userName,int index,const char* buf,int len)
{
	char sqlstr[128];

	sprintf(sqlstr,"update %s set cardDeck%d=? where name='%s'",SQLITE_USER_TABLE,index,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,buf,len);
}

bool KUserSql::UpdateNormalBag(const char* userName,tb_player_record* record)
{
	char sqlstr[128];
	sprintf(sqlstr,"update %s set normalItem=? where name='%s'",SQLITE_USER_TABLE,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,record->normalItem.binData,record->normalItem.actualLength);
}

bool KUserSql::LoadUserData(const char* userName,tb_player_record* record)
{

	char sqlstr[128];

	sprintf(sqlstr,"select * from %s where name='%s'",SQLITE_USER_TABLE,userName);
	execLoadSqlStruct execSql;
	execSql._table = SQLITE_USER_TABLE;
	execSql._sql = sqlstr;
	execSql._fn_loadRecord = loadUserRecord;
	execSql._param = record;
	return KSqlite::readFromSQLite(&execSql);
}

bool KUserSql::InsertUserData(const char* userName)
{
	char sqlcount[128];
	sprintf(sqlcount,"select count(*) from %s where name = '%s'",SQLITE_USER_TABLE,userName);
	char sqlInsert[128];
	sprintf(sqlInsert,"insert into %s (name,money,curDeck,pvpVal,exp) values ( '%s',911,0,0,0)",SQLITE_USER_TABLE,userName);
	execSaveSqlStruct execSql;
	execSql._table = SQLITE_USER_TABLE;
	execSql._sql_count = sqlcount;
	execSql._createField = SQLITE_USER_CREATE;
	execSql._sql_insert = sqlInsert;
	execSql._sql_update = "";
	return  KSqlite::saveToSQLite(&execSql);
}
