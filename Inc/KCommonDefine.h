#pragma once

#define BIT(x) (1 << (x))
#define max_cmd_bits (13)
#define MAX_CLIENT_IN_GATEWAY (20480)
#define MAX_NAME_DEF (64)
#define MAX_PLAYER_COUNT_PRE_ACCOUNT (8)
#define MAX_BUS_LINE_NUM 20
#define MAX_CHANGE_MAP_HISTORY 5 
#define _str_(x) #x
#define REGION_WIDTH_BITS 4     // (1<<4) = 16��
#define REGION_WIDTH (1 << REGION_WIDTH_BITS)
#define SYNC_RADIUS_DEFAULT 3   // Ĭ��ͬ���뾶
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
 * ʮ����:aabbbcccccccccccccc
 * aa:type id
 * bbb:server id
 * cccccccccccccc:14λ����ID
 */
#define TYPE_BASE		100000000000000000ll
#define SERVER_ID_BASE	100000000000000ll
#define GENERATE_OBJ_ID(id, _typeid, _serverid) (id + _typeid * TYPE_BASE + _serverid * SERVER_ID_BASE)
#define GET_OBJ_TYPE(id) (id / TYPE_BASE)
#define GET_SERVER_ID(id) ((id / SERVER_ID_BASE) % 1000)
#define GET_OBJ_ID(id) (id % SERVER_ID_BASE)

const int defObjDirctionCount = 65536;
const int defObjDefaultDirction = 16384;

// ������ұ���������ʱ��(second)
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
	ECSBT_ALLOK,	// ������
	ECSBT_Normal,   // ������
	ECSBT_Dup,		// ����
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
	wokCount,	//�뱣�������һ��
};

enum eSyncType
{
	eSyncNone,					// ��ͬ��
	
	eSyncMyself,				// ͬ�����Լ�
	eSyncAllSightPlayer,		// ͬ����������Ұ��Χ�ڵ���ң������Լ���
	eSyncAllSightPlayerButMe,	// ͬ����������Ұ��Χ�ڵ���ң��������Լ���
	eSyncNearby,				// ͬ���������������Լ���
	eSyncNearbyExceptMe,		// ͬ�����������������Լ���
	eSyncArea,					// ͬ�������򣨰����Լ���
	eSyncAreaExceptMe,			// ͬ�������򣨲������Լ���
	eSyncToAI,					// ͬ����������AIServer

	eSyncToCurrentScene,		// ����ǰ����ͬ��
	eSyncToCurrentGame,			// ����ǰgameserverͬ��
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

// ���������������Ϣ
enum ENUM_CHAT_GROUP
{
	enumECG_Invalid = 0,
	enumECG_System,					// ϵͳ����ʾ���桢ϵͳ��Ϣ�ȣ����󲿷���ʾ�����Ϸ��Ĺ������У�
	enumECG_Info,					// ��Ϣ����ʾ������Ϣ By Client��
	enumECG_BattleInfo,				// ս����Ϣ����ʾս����Ϣ By Client��
	enumECG_Private,				// ˽�ģ�һ��һ˽�� By ChatServer��
	enumECG_Team,					// ���飨�������� By ChatServer��
	enumECG_Current,				// ������ͬ����Χ�� by GameServer��
	enumECG_Map,					// ����ͬ����Χ������ͼ By GameServer��
	enumECG_Guild,					// ���ɣ��������� By ChatServer��
	enumECG_Nation,					// ���ң��������� By ChatServer��
	enumECG_World,					// ���磨������������By ChatServer��
	enumECG_BattleCry,				// �ź���������ͼ By GameServer��������ͼ��
	enumECG_BattleField,			// ս��Ƶ��������ս����ͼ By GameServer����ʵ����������ͼ��
	enumECG_MainFrameInfomation,	//��������Ϣ����ɫͷ����
	enumECG_MainFrameTopInfomation,	//�������ö���Ϣ
	enumECG_MessageBox,				// �Ի���
	enumECG_SystemAndMainFrame,		// ϵͳƵ����ͷ������ʾ
	enumECG_ShuiGuo,				// ˮ��
	enumECG_XueGuo,					// ѩ��
	enumECG_ShanGuo,				// ɽ��
	enumECG_SingletonMessageBox,	// �Ի���
	enumECG_FindTeam,				// Ѱ����飨���ϵȼ������� By ChatServer��
	enumECG_CustomChannel,			// �Զ���Ƶ����ʼ
	enumECG_IMGuild,				// ���ɣ�IM��
	enumECG_IMPrivate,				// ˽�ģ�IM��
	enumECG_Count,					// Ƶ��������
};

//  ��Ʒ����
enum eItem_Operation_Result
{
	enumIOR_BEGIN = 0,
	enumIOR_SUCCESS,
	enumIOR_FAILED,
	enumIOR_ADD,			// �����Ʒ
	enumIOR_UPDATE,			// �޸���Ʒ
	enumIOR_DELETE,			// ɾ����Ʒ	
	enumIOR_DEL_SPC,		// ɾ����Ʒ�ϵĸ��Ի���Ϣ
	enumIOR_USE,			// ʹ����Ʒ
	enumIOR_MOVE,			// �ƶ���Ʒ
	enumIOR_GETINFO,		// �ͻ���ȡ����Ʒ������Ϣ
	enumIOR_BaudBag,		// ��Ʒ��Ƕ
	enumIOR_MOSAIC,			// ��Ʒ��Ƕ
	enumIOR_ENCHANT,		// ��Ʒ��ħ
	enumIOR_EQUIP,			// װ����Ʒ
	enumIOR_UNEQUIP,		// ж��װ����Ʒ


	enumIOR_GETCREATEINFO,	// ��ȡ��Ʒ������Ϣ
	enumIOR_SETSHORTCUTITEM,// ������Ʒ���
	enumIOR_MOVESHORTCUTITEM,// ������Ʒ���
	enumIOR_DELETESHORTCUTITEM,// ������Ʒ���
	enumIOR_MONEYUPDATE,		//��Ǯˢ��
	enumIOR_USE_RESULT,			// ʹ�ý��
	enumIOR_MOVE_RESULT,		// �ƶ����
	enumIOR_UPDATE_ATTRIBUTE,		// ��������
	enumIOR_UPDATE_ATTRIBUTE_STR,	// ��������

	enumIOR_ChangePage,				// �����и���ҳ��

	enumIOR_LoadTargetEquip,		// ����Ŀ��װ��


	enumIOR_GET_FIRST_ITEM,				// ��ȡ������һ����Ʒ
	enumIOR_ITEM_IS_IN_POSTYPE,			// ĳ��Ʒ�Ƿ���ĳ����
	enumIOR_GET_SETCOUNT,				// ĳID��װ����װ����


	enumIOR_Compose_DO,					// ��ʼװ���ϳ�
	enumIOR_Compose_CANCEL,				// ȡ��װ���ϳ�
	enumIOR_Compose_RESULT,				// װ���ϳɽ��
	enumIOR_Compose_CLEAR,				// ���װ���ϳ���Ʒ
	enumIOR_Compose_PUT_ON,				// ������Ʒ
	enumIOR_Compose_GET_BACK,			// ȡ����Ʒ

	enumIOR_COUNT,
};

