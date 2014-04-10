#ifndef _TMP_PLAYER_SESSIONS_H_
#define _TMP_PLAYER_SESSIONS_H_

#include <System/KPlatform.h>
#include <lua/KLuaWrap.h>
#include <Packetgate/common_packet.h>
#include <KCommonStruct.h>

enum 
{
	world_server_tmp_session_length = 1024,
};

//----------------------------------------------------------------
// 玩家登陆或者切服时，从GameServer传递给WorldServer的SessionKey
// 在这里定义
//----------------------------------------------------------------

const DWORD tmp_player_s_take_bus		= 1;
const DWORD tmp_player_s_carriee_tc		= 2;
const DWORD tmp_player_s_summon_monster	= 3;
const DWORD tmp_player_s_force_position	= 4;
const DWORD tmp_player_s_entercount		= 5;
const DWORD tmp_player_s_playerInfo		= 6;
const DWORD tmp_player_s_gameGroup		= 7;
const DWORD tmp_player_s_match_supportid= 9;
const DWORD tmp_player_s_match_betmoney	= 10;
const DWORD tmp_player_s_playersouls	= 11;
const DWORD tmp_player_s_from_pos		= 12; // s_map_position
const DWORD tmp_player_s_changescene	= 13;
const DWORD tmp_player_s_backtomap		= 14;

inline void export_tmpPlayerSessions_to_lua()
{
	export_const_to_lua(tmp_player_s_take_bus);
	export_const_to_lua(tmp_player_s_carriee_tc);
	export_const_to_lua(tmp_player_s_summon_monster);
	export_const_to_lua(tmp_player_s_force_position);
	export_const_to_lua(tmp_player_s_entercount);
	export_const_to_lua(tmp_player_s_playerInfo);
	export_const_to_lua(tmp_player_s_gameGroup);
	export_const_to_lua(tmp_player_s_match_supportid);
	export_const_to_lua(tmp_player_s_match_betmoney);
	export_const_to_lua(tmp_player_s_from_pos);
}

#pragma pack(push,1)
struct s_carriee_tc
{
	DWORD tcType;
};

struct s_summon_monster
{
	DWORD id;
	int level;
	int hp, mp;
};

struct s_entercount
{
	int map,day, week, month;
};

struct s_force_position
{
	_MoveNode mPos;
	USHORT mAngle;
	USHORT mAngleZ;
};

struct s_map_position
{
	int mapID;
	float x,y,z;
};

#pragma pack(pop)

#endif