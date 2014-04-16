
#pragma once

enum LOGIC_MSG_SECTION
{
	LOGIC_BATTLE_BEGIN							= 0x00000000,
	LOGIC_BATTLE_END							= 0x00000FFF,

	LOGIC_CHARACTOR_BEGIN						= 0x00001000,
	LOGIC_CHARACTOR_END							= 0x00001FFF,

	LOGIC_UI_BEGIN								= 0x00002000,
	LOGIC_UI_END								= 0x00002FFF,

	LOGIC_LOGIN_BEGIN							= 0x00003000,
	LOGIC_LOGIN_END								= 0x00003FFF,

	LOGIC_GATEWAY_BEGIN							= 0x00004000,
	LOGIC_GATEWAY_END							= 0x00004FFF,

	LOGIC_MAIL_BEGIN							= 0x00005000,
	LOGIC_MAIL_END								= 0x00005FFF,

	LOGIC_WORLD_MSG_BEGIN						= 0x00006000,
	LOGIC_WORLD_MSG_END							= 0x00006FFF,

	LOGIC_RUNBUSINESS_BEGIN						= 0x00007000,
	LOGIC_RUNBUSINESS_END						= 0x00007FFF,

	LOGIC_ARENA_BEGIN							= 0x00008000,
	LOGIC_ARENA_END								= 0x00008FFF,
};

enum LOGIC_BATTLE_MSG
{
	LOGIC_BATTLE_TURNBEGIN					= LOGIC_BATTLE_BEGIN,	// 增加主角消息, uParam:id(UINT64), lParam:0
	LOGIC_BATTLE_TURNEND,		//回合
	LOGIC_BATTLE_INIT,			//战斗开始
	LOGIC_BATTLE_HANDCARD_READY,
	LOGIC_BATTLE_DRAWCARD,		//抓牌----------
	LOGIC_BATTLE_DUELRESULT,    //卡片对打
	LOGIC_BATTLE_ABILITYRESULT,
	LOGIC_BATTLE_ADDBUFRESULT,    //
	LOGIC_BATTLE_DELBUFRESULT,
	LOGIC_BATTLE_CARDMOVE,      //卡片移动槽位
	LOGIC_BATTLE_USERES,      //
	LOGIC_BATTLE_USE_SECRET, //奥秘进场
	LOGIC_BATTLE_SELECTCARD_OK,//选牌完成
	LOGIC_BATTLE_OPDONE, //单次操作完成
	LOGIC_BATTLE_CARD2TOMB,      //卡片移动槽位
	LOGIC_BATTLE_SECRET2TOMB,      //卡片移动槽位
	LOGIC_BATTLE_UPDATEINFO,
	LOGIC_BATTLE_DECKINIT,

	LOGIC_BATTLE_DIRTY,
	LOGIC_BATTLE_GAMEEND,

	LOGIC_WORLD_GEN_PLAYERCARD,
	LOGIC_WORLD_USE_ITEM,
	LOGIC_WORLD_NORMAL_BAG_ITEM_CHANGE,
	LOGIC_WORLD_MAINPLAYER_QUEST_UPDATELIST,

	LOGIC_BATTLE_COUNT,
};

enum LOGIC_LOGIN_MSG
{
	LOGIC_LOGIN_SERVER_CONNECTED				= LOGIC_LOGIN_BEGIN,	// LoginServer连接成功
	LOGIC_LOGIN_SERVER_CONNERROR,				// LoginServer连接失败
	LOGIC_LOGIN_SERVER_CONNCLOSED,				// LoginServer连接失败
	LOGIC_LOGIN_LOGIN_RSP,						// 登录成功		
	LOGIC_LOGIN_LOGOUT_RSP,						// 退出登录		

	LOGIC_LOGIN_SERVER_LIST_OVER,				// 服务器列表发送完毕
	LOGIC_LOGIN_SERVER_LAST_SELECT,				// 上次选择的服务器
	LOGIC_LOGIN_SERVER_RECOMMEND,				// 本次推荐的服务器
	LOGIC_LOGIN_NOTIFY_MSG,						// 发来信息提示
	LOGIC_LOGIN_AREANAMEALIAS,					// 发来区域信息

	LOGIC_LOGIN_OTPCODE_RSP,					// 验证码返回
	LOGIC_LOGIN_RECV_CHARACTER_INFO,			// 接收到一条角色信息
	LOGIC_LOGIN_CHARACTER_ENTER_RSP,			// 选择登录角色后
	LOGIC_LOGIN_CHECKNAME_RESULT,				// 名字检查
	LOGIC_LOGIN_CREATE_RESULT,					// 
	LOGIC_LOGIN_DELETE_RESULT,					// 
	LOGIC_LOGIN_CHANGENAME_RESULT,				// 
	LOGIC_LOGIN_REQUIRE_SELECT_LINE,			// 需要玩家选择条分线

	// ----在上面添加消息类型-------------------------------------------
	LOGIC_LOGIN_COUNT
};

enum LOGIC_GATEWAY_MSG
{
	LOGIC_GW_CONNECTED							= LOGIC_GATEWAY_BEGIN,
	LOGIC_GW_CONNERROR,
	LOGIC_GW_CONNCLOSED,
	LOGIC_GW_ACCEPTED,

	LOGIC_GW_CHARINFO,

	LOGIC_WC_CONNECTED,

	LOGIC_GW_COUNT
};

enum LOGIC_CHAR_MSG
{
	LOGIC_CHAR_CONNECTED						= LOGIC_CHARACTOR_BEGIN,
	LOGIC_CHAR_CONNERROR,
	LOGIC_CHAR_CONNCLOSED,

	
};
