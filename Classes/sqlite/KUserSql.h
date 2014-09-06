#ifndef _KUSERSQL_H
#define _KUSERSQL_H

struct tb_player_record;

struct CriPlayerInfo{
	int _money;
	int _pvpVal;
	int _exp;
	int _power;
	int _mercy;
	int _dailyStageLev;
	int _winDailyStageNum;
	int _dump[20];
	CriPlayerInfo();
	CriPlayerInfo(tb_player_record* record);
};
class KUserSql
{
public:
	KUserSql(){}
	~KUserSql(){}
	static bool LoadUserData(const char* userName,tb_player_record* record);
	static bool InsertUserData(const char* userName);
	static bool UpdateDeckStore(const char* userName,tb_player_record* record);
	static bool UpdateCardDeck(const char* userName,tb_player_record* record,int index);
	static bool UpdateIntegerVal(const char* userName,const char* field,int val);
	static bool UpdateNormalBag(const char* userName,tb_player_record* record);
	static bool UpdateCriVal(const char* userName,tb_player_record* record);
	static bool UpdateHeroData(const char* userName,tb_player_record* record);
};

#endif // __HELLOWORLD_SCENE_H__
