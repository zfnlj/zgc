#ifndef _KUSERQUESTSQL_H
#define _KUSERQUESTSQL_H

struct tb_player_record;
struct tb_playerquest_record;
class KUserQuestSql
{
public:
	KUserQuestSql(){}
	~KUserQuestSql(){}
	static bool LoadUserQuestData(const char* userName,tb_playerquest_record* record);
	static bool InsertUserQuestData(const char* userName);
	static bool UpdateQuestState(const char* userName,int index,tb_playerquest_record* record);
	static bool UpdateIntegerVal(const char* userName,const char* field,int val);
	static bool UpdateQuestHistory(const char* userName,tb_playerquest_record* record);
	static bool UpdateQuestDaily(const char* userName,tb_playerquest_record* record);
};

#endif // __HELLOWORLD_SCENE_H__
