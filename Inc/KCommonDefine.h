#pragma once

#define BIT(x) (1 << (x))
#define max_cmd_bits (13)
#define MAX_CLIENT_IN_GATEWAY (20480)
#define MAX_NAME_DEF (64)
#define MAX_PLAYER_COUNT_PRE_ACCOUNT (8)
#define MAX_BUS_LINE_NUM 20
#define MAX_CHANGE_MAP_HISTORY 5 
#define _str_(x) #x
#define REGION_WIDTH_BITS 4     // (1<<4) = 16米
#define REGION_WIDTH (1 << REGION_WIDTH_BITS)
#define SYNC_RADIUS_DEFAULT 3   // 默认同步半径
#define NORMAL_MAP_REGIONS	(16*16)
#define NPC_ID_BASE_START			(10000LL*10000LL*100LL)
#define NPC_ID_LIMITED				(10000LL*10000LL*10LL)
#define TRIGGER_ID_BASE_START		(10000LL*10000LL*100LL)
#define TRIGGER_ID_LIMITED			(10000LL*10000LL*10LL)
#define GROUNDITEM_ID_BASE_START	(10000LL*10000LL*110LL)
#define GROUND_ID_LIMITED			(10000LL*10000LL*10LL)

#define MAX_SCENE_XML_NAME			64
#define CLIENT_PING_INTERVAL 10*1000
/**
 * 十进制:aabbbcccccccccccccc
 * aa:type id
 * bbb:server id
 * cccccccccccccc:14位对象ID
 */
#define TYPE_BASE		100000000000000000ll
#define SERVER_ID_BASE	100000000000000ll
#define GENERATE_OBJ_ID(id, _typeid, _serverid) (id + _typeid * TYPE_BASE + _serverid * SERVER_ID_BASE)
#define GET_OBJ_TYPE(id) (id / TYPE_BASE)
#define GET_SERVER_ID(id) ((id / SERVER_ID_BASE) % 1000)
#define GET_OBJ_ID(id) (id % SERVER_ID_BASE)

const int defObjDirctionCount = 65536;
const int defObjDefaultDirction = 16384;

// 掉线玩家保持上下文时间(second)
const int DROPPED_KEEP_SECONDS = 5*60;

enum EnumServerIndicator
{
	enum_world_server_flag		= 1,
	enum_character_server_flag	= 2,
	enum_game_server_flag		= 3,
	enum_chat_server_flag		= 4,

	enum_world_server_indp		= (1 << max_cmd_bits),
	enum_character_server_indp	= (2 << max_cmd_bits),
	enum_game_server_indp		= (3 << max_cmd_bits),
	enum_chat_server_indp		= (4 << max_cmd_bits),
};

enum EChSceneBackType
{
	ECSBT_ALLOK,	// 都可以
	ECSBT_Normal,   // 大世界
	ECSBT_Dup,		// 副本
	ECSBT_MAX
};

enum eWorldObjectKind
{
	wokWorldObj			= 0,
	wokCharacter		= 1,
	wokPlayer			= 2,
	wokNpc				= 3,
	wokGroundItem		= 4,
	wokTrigger			= 5,
	wokCount,	//请保持在最后一个
};

enum eSyncType
{
	eSyncNone,					// 不同步
	
	eSyncMyself,				// 同步给自己
	eSyncAllSightPlayer,		// 同步给所有视野范围内的玩家（包括自己）
	eSyncAllSightPlayerButMe,	// 同步给所有视野范围内的玩家（不包括自己）
	eSyncNearby,				// 同步给附近（包括自己）
	eSyncNearbyExceptMe,		// 同步给附近（不包括自己）
	eSyncArea,					// 同步给区域（包括自己）
	eSyncAreaExceptMe,			// 同步给区域（不包括自己）
	eSyncToAI,					// 同步给关联的AIServer

	eSyncToCurrentScene,		// 给当前场景同步
	eSyncToCurrentGame,			// 给当前gameserver同步
	e_SyncToSyncServer,
};

enum
{
	REGION_LEFT,
	REGION_TOP,
	REGION_RIGHT,
	REGION_BOTTOM,
};

namespace ObjectKind
{
	enum
	{
		invalid,
		player,
		npc,
		ground_item,
		count,
	};
}

namespace MapObjAttr
{
	enum
	{
		POS = BIT(0),
	};
}

namespace AppearFlagSet
{
	enum
	{
		login = BIT(0),
	};
}

// 聊天服务器分组信息
enum ENUM_CHAT_GROUP
{
	enumECG_Invalid = 0,
	enumECG_System,					// 系统（显示公告、系统信息等，绝大部分显示在最上方的滚动条中）
	enumECG_Info,					// 信息（显示个人信息 By Client）
	enumECG_BattleInfo,				// 战斗信息（显示战斗信息 By Client）
	enumECG_Private,				// 私聊（一对一私聊 By ChatServer）
	enumECG_Team,					// 队伍（整个队伍 By ChatServer）
	enumECG_Current,				// 附近（同步范围内 by GameServer）
	enumECG_Map,					// 区域（同步范围整个地图 By GameServer）
	enumECG_Guild,					// 帮派（整个帮派 By ChatServer）
	enumECG_Nation,					// 国家（整个国家 By ChatServer）
	enumECG_World,					// 世界（整个服务器，By ChatServer）
	enumECG_BattleCry,				// 呐喊（整个地图 By GameServer，整个地图）
	enumECG_BattleField,			// 战场频道（整个战场地图 By GameServer，其实就是整个地图）
	enumECG_MainFrameInfomation,	//主界面信息（角色头顶）
	enumECG_MainFrameTopInfomation,	//主界面置顶信息
	enumECG_MessageBox,				// 对话框
	enumECG_SystemAndMainFrame,		// 系统频道和头顶都显示
	enumECG_ShuiGuo,				// 水国
	enumECG_XueGuo,					// 雪国
	enumECG_ShanGuo,				// 山国
	enumECG_SingletonMessageBox,	// 对话框
	enumECG_FindTeam,				// 寻求队伍（符合等级条件的 By ChatServer）
	enumECG_CustomChannel,			// 自定义频道开始
	enumECG_IMGuild,				// 帮派（IM）
	enumECG_IMPrivate,				// 私聊（IM）
	enumECG_Count,					// 频道总数量
};

//  物品操作
enum eItem_Operation_Result
{
	enumIOR_BEGIN = 0,
	enumIOR_SUCCESS,
	enumIOR_FAILED,
	enumIOR_ADD,			// 添加物品
	enumIOR_UPDATE,			// 修改物品
	enumIOR_DELETE,			// 删除物品	
	enumIOR_DEL_SPC,		// 删除物品上的个性化信息
	enumIOR_USE,			// 使用物品
	enumIOR_MOVE,			// 移动物品
	enumIOR_GETINFO,		// 客户端取得物品所有信息
	enumIOR_BaudBag,		// 物品镶嵌
	enumIOR_MOSAIC,			// 物品镶嵌
	enumIOR_ENCHANT,		// 物品附魔
	enumIOR_EQUIP,			// 装备物品
	enumIOR_UNEQUIP,		// 卸下装备物品


	enumIOR_GETCREATEINFO,	// 获取物品基础信息
	enumIOR_SETSHORTCUTITEM,// 设置物品快捷
	enumIOR_MOVESHORTCUTITEM,// 设置物品快捷
	enumIOR_DELETESHORTCUTITEM,// 设置物品快捷
	enumIOR_MONEYUPDATE,		//金钱刷新
	enumIOR_USE_RESULT,			// 使用结果
	enumIOR_MOVE_RESULT,		// 移动结果
	enumIOR_UPDATE_ATTRIBUTE,		// 更新属性
	enumIOR_UPDATE_ATTRIBUTE_STR,	// 更新属性

	enumIOR_ChangePage,				// 背包中更换页面

	enumIOR_LoadTargetEquip,		// 加载目标装备


	enumIOR_GET_FIRST_ITEM,				// 获取背包第一个物品
	enumIOR_ITEM_IS_IN_POSTYPE,			// 某物品是否在某栏里
	enumIOR_GET_SETCOUNT,				// 某ID套装的已装备数


	enumIOR_Compose_DO,					// 开始装备合成
	enumIOR_Compose_CANCEL,				// 取消装备合成
	enumIOR_Compose_RESULT,				// 装备合成结果
	enumIOR_Compose_CLEAR,				// 清除装备合成物品
	enumIOR_Compose_PUT_ON,				// 放入物品
	enumIOR_Compose_GET_BACK,			// 取出物品

	enumIOR_COUNT,
};

