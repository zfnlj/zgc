#include "KUserSql.h"
#include <string>
#include "ccMacros.h"
#include "KSqlite.h"
#include "CommonLogic/KDBDataStruct.h"
#include "KSqlEnc.h"
#include "../DB/KPlayerDBMgr.h"

#define  SQLITE_USER_TABLE "user_table"
//#define  SQLITE_USER_CREATE "(name char(80) primary key,money integer)";
#define  SQLITE_USER_CREATE "(name char(80) primary key ,\
								cardStore blob,cardDeck0 blob,cardDeck1 blob,cardDeck2 blob,cardDeck3 blob,cardDeck4 blob,\
								curDeck integer,criInfo blob,normalItem blob,heroData blob)";

enum player_field_enum
{
	f_name,
	f_cardStore,
	f_cardDeck0,
	f_cardDeck1,
	f_cardDeck2,
	f_cardDeck3,
	f_cardDeck4,
	f_curDeck,
	f_criInfo,
	f_normalItem,
	f_heroData,
};

int loadUserRecord(void* para,int n_cloumn,char** column_value,char** column_name)
{
	char buf[2048];
	tb_player_record* record = (tb_player_record*)para;

	LOAD_BLOB_FIELD(column_value[f_cardStore],record->_cardStore,buf)
	LOAD_BLOB_FIELD(column_value[f_normalItem],record->_normalItem,buf)
	LOAD_BLOB_FIELD(column_value[f_heroData],record->_heroData,buf)

	if(column_value[f_criInfo]){
		CriPlayerInfo decInfo;
		int len = KSqlite::loadBlobBuf((char*)&decInfo,sizeof(decInfo),column_value[f_criInfo]);
		if(len==sizeof(decInfo)){
			record->_exp = decInfo._exp;
			record->_money = decInfo._money;
			record->_pvpVal = decInfo._pvpVal;
			record->_mercy = decInfo._mercy;
			record->_power = decInfo._power;
			memcpy(&record->_gameData,&decInfo._gameData,sizeof(Player_GameData));
		}
	}


	for(int i=0;i<MAX_DECK_NUM;i++){
		LOAD_BLOB_FIELD(column_value[f_cardDeck0+i],record->_cardDeck[i],buf)
	}
	if(column_value[f_curDeck]){
		sscanf(column_value[f_curDeck],"%d",&record->_curDeck);
	}
	
	return 0;
}

CriPlayerInfo::CriPlayerInfo()
{
	memset(this,0,sizeof(CriPlayerInfo));
}

CriPlayerInfo::CriPlayerInfo(tb_player_record* record)
{
	_money = record->_money;
	_pvpVal = record->_pvpVal;
	_exp = record->_exp;
	_power = record->_power;
	_mercy = record->_mercy;
	memcpy(&_gameData,&record->_gameData,sizeof(Player_GameData));
}

bool KUserSql::UpdateCriVal(const char* userName,tb_player_record* record)
{
	CriPlayerInfo criInfo(record);
	char sqlstr[128];
	sprintf(sqlstr,"update %s set criInfo=? where name='%s'",SQLITE_USER_TABLE,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,(const char *)&criInfo,sizeof(criInfo));
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
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,record->_cardStore.binData,record->_cardStore.actualLength);
}

bool KUserSql::UpdateCardDeck(const char* userName,tb_player_record* record,int index)
{
	char sqlstr[128];

	sprintf(sqlstr,"update %s set cardDeck%d=? where name='%s'",SQLITE_USER_TABLE,index,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,record->_cardDeck[index].binData,record->_cardDeck[index].actualLength);
}

bool KUserSql::UpdateHeroData(const char* userName,tb_player_record* record)
{
	char sqlstr[128];

	sprintf(sqlstr,"update %s set heroData=? where name='%s'",SQLITE_USER_TABLE,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,record->_heroData.binData,record->_heroData.actualLength);
}

bool KUserSql::UpdateNormalBag(const char* userName,tb_player_record* record)
{
	char sqlstr[128];
	sprintf(sqlstr,"update %s set normalItem=? where name='%s'",SQLITE_USER_TABLE,userName);
	return KSqlite::updateBlobBinaryData(SQLITE_USER_TABLE,sqlstr,record->_normalItem.binData,record->_normalItem.actualLength);
}

bool KUserSql::LoadUserData(const char* userName,tb_player_record* record)
{

	record->init();
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
	sprintf(sqlInsert,"insert into %s (name,curDeck) values ( '%s',-1)",SQLITE_USER_TABLE,userName);
	execSaveSqlStruct execSql;
	execSql._table = SQLITE_USER_TABLE;
	execSql._sql_count = sqlcount;
	execSql._createField = SQLITE_USER_CREATE;
	execSql._sql_insert = sqlInsert;
	execSql._sql_update = "";
	return  KSqlite::saveToSQLite(&execSql);
}
