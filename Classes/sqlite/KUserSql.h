#ifndef _KUSERSQL_H
#define _KUSERSQL_H

struct tb_player_record;

struct CriPlayerInfo{
	int _money;
	int _pvpVal;
	int _exp;
	int _crc;
	CriPlayerInfo(int money,int pvp,int exp):_money(money),_pvpVal(pvp),_exp(exp)
	{}
};
class KUserSql
{
public:
	KUserSql(){}
	~KUserSql(){}
	static bool LoadUserData(const char* userName,tb_player_record* record);
	static bool InsertUserData(const char* userName);
	static bool UpdateDeckStore(const char* userName,tb_player_record* record);
	static bool UpdateCardDeck(const char* userName,int index,const char* buf,int len);
	static bool UpdateIntegerVal(const char* userName,const char* field,int val);
	static bool UpdateNormalBag(const char* userName,tb_player_record* record);
	static bool UpdateCriVal(const char* userName,int money,int exp,int pvpVal);
};

#endif // __HELLOWORLD_SCENE_H__
