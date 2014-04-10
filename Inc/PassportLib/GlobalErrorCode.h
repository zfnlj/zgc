#pragma once

enum KGlobalErrorCodeDefine_base	// 0-999
{
	// �벻Ҫ���м����
	err_NormalSuccess = 0,			// �ɹ�(����)
	err_NormalFailed,				// ʧ��(����)
	err_StringLen_error,			// �ַ������ȴ���
	err_PacketLen_error,			// ���ݰ����ȴ���
	err_DB_error,					// ���ݿ����
	err_Unknown,					// δ֪����

	err_basecount,
	err_base_end = 1000,
};

enum KGlobalErrorCodeDefine_login	// 1000-9999
{
	// �벻Ҫ���м����
	err_Account_notfound = err_base_end,			// �û���δ�ҵ�
	err_Password_error,				// ���벻��ȷ
	err_Already_online,				// �Ѿ���½
	err_OTP_error,					// OTP����
	err_Version_error,				// �汾����
	err_DateBase_error,				// ���ݿ����
	err_IllegalCharValid,			// ���������ַ�
	err_ServerClose,				// ϵͳά����
	err_Validate_Error,				// ��֤����
	err_Account_Problem,			// ����˺Ŵ�������
	err_Account_Unuseful,			// ��ҵ��˺Ų�������
	err_NotActiveAccount,			// δ����
	err_LoginFaild,					// ��¼ʧ��

	err_BindIPError,				// ��IP������Ҫ��
	err_AccountBlocked,				// �ʺű���ͣ
	err_AccountBlockedForver,		// �ʺű����÷�ͣ
	err_WallowProtect,				// ������
	err_AdultUnApply,				// δ�Ǽ�1
	err_NotAdultUnApply,			// δ�Ǽ�2

	err_Character_exist,			// ��ɫ���Ѵ���
	err_Character_notexist,			// ��ɫ��������
	err_Character_namenull,			// ��ɫ����
	err_Character_notfound,			// ��ɫδ�ҵ�
	err_Character_overcount,		// ��ɫ����������
	err_Equip_error,				// װ������
	err_CharacterInfo_error,		// ��ɫ��Ϣ����
	err_Nationality_full,			// ѡ��Ĺ�������Ϊ��
	err_Guild_Master,				// �ǰ�����������
	err_Guild_Answer,				// �ǹ������Ӧ��־
	err_Character_NeedChangeName,	// ��ɫ��Ҫ����
	err_CanNot_ChangeName,			// ���������

	err_EnterGame_nopoint,			// ��ҽ���ʱworldserver�Ĵ������
	err_EnterGame_nobornpoint,		// �Ҳ���������
	err_EnterGame_noworldtypefound,	// �Ҳ�����ͼ��Ӧ��world
	err_EnterGame_not_allowCreate,	// ���ܴ����¸���
	err_EnterGame_mapctrlnotfound,	// �Ҳ�����ͼ�ű�
	err_EnterGame_no_world_found,	// �Ҳ���ָ����world
	err_EnterGame_game_server_full,	// ��Ϸ����������
	err_EnterGame_wplayer_notfound,	// worldserver����Ҳ�����(�϶����߼���������)
	err_EnterGame_wplayer_state_error,		// worldserver�����״̬����
	err_EnterGame_playerstoragefailed,		// ������Ҹ���״̬��Ϣ����

	err_LinePlayerFull,						// ��·����,�����Ŷ�
	err_PASSWORDPROTECT_NOT_VALIDATION,		// �ܱ�δ��֤
	err_PASSWORDPROTECT_ERROR,				// �ܱ�����
	err_GamePlaying_NotFind_Rsp,			// ��Ҳ�����(Ӧ�������ش���)
	err_EnterGameError,						// ������Ϸ����������

	err_LoadPlayerData_timeout,		// ��ȡ������ݳ�ʱ
	err_LoadActiveData,				// ��DataServer�ϼ���Active������
	err_LoadData_NoCapacity,		// cache�������ù���
	err_LoadData_Sql,				// sql����
	err_DataServer_NotAvail,		// data server not available

	// ��ͨ��¼����������֮ǰ��� �����������
	err_EnterGame_unionMap,				// ����ֱ�ӽ�����ս��
	err_EnterGame_unionServer_notAvail,	// ���������������
	err_wplayer_already_exist,			// worldserver������Ѿ�����

	err_DeleteChar_MatchTeam_Leader,	// ս�Ӷӳ�����ɫ	
	err_LinePlayerFull_ask,						// ��·����,�����Ŷ�

	// �ձ� �����
	err_JAPAN_08					= 3000,		// ��֤����

	// JJƽ̨�����
	err_JJ_PARAM_ERROR				= 3100,		// ��������		HTTP 400
	err_JJ_APP_CHECK_ERROR,						// Ӧ����֤ʧ�� HTTP 401
	err_JJ_CALL_SERVICE_ERROR,					// ���÷���ʧ�� -99

	// ���� �����
	err_KOREA_VALIDATE_ERROR		= 3200,		// ��֤ʧ��
	err_KOREA_OTHER_ERROR,						// ��������
	err_KOREA_SRDPASS_NOT_EXISTS,				// �������벻����

	err_Unknown_Language,

	err_login_end = 10000,
};

enum KGlobalErrorCodeDefine_baselogic	// 10000-10999
{
	err_player_not_found = err_login_end,

	err_FlushRecord_NotFound,					// DataServer��û�ҵ���¼
	err_DeleteRecord_NotFound,					// Ҫɾ���ļ�¼û���ҵ�
	err_FlushRecord_Inactive,					// flushһ����DataServer�Ǽ���ļ�¼
	err_FlushRecord_Mismatch,					// loadTime��ƥ��

	err_TablceCache_NoCapacity,					// TablceCache��������
	err_DeleteActiveRecord,						// ������ɾ������ļ�¼
	err_DirectDelete_ActiveRecord,				// ������ֱ��ɾ������ļ�¼
	err_DirectUpdate_ActiveRecord,				// ������ֱ�Ӹ��¼���ļ�¼
	err_DirectInsert_AlreadyExist,				// Ҫ����ļ�¼�Ѵ���

	err_baselogic_end = 11000,
};

enum KGlobalErrorCodeDefine_logic	// 11000-19999
{
	err_player_logic_begin = err_baselogic_end,

	// cash result
	err_Prepaid_Result_Success,	// Ԥ���ɹ�
	err_Prepaid_Result_Failed,	// Ԥ��ʧ��
	err_AddCash_Result_Success,	// ��ֵ�ɹ�
	err_AddCash_Result_Failed,	// ��ֵʧ��

	// NPC fight
	err_Npc_CanNotFight,		// ��ǰ����ս��
	err_Npc_CastSkill,			// ����ʩ�ż���

	// money result
	err_AddMoney_Success,		// ����Money�ɹ�
	err_AddMoney_Failed,		// ����Moneyʧ��

	// friend result
	err_target_Not_Online,		// Ŀ�겻����

	// mail result
	err_M_Net_Error,			// �������
	err_M_Db_Error,				// ���ݿ����
	err_M_Query_In_Part,		// ���������ʼ�δ����
	err_M_Query_Accomplish,		// ���������ʼ�δ����
	err_M_Send_No_Space,		// �ռ�����������
	err_M_Send_No_Space_RTS,	// �ռ����������������˸�������
	err_M_Send_No_Receiver,		// �ռ��˲�����
	err_M_Send_No_Receiver_RTS,	// �ռ��˲����ڣ����˸�������
	err_M_Send_No_Enough_Money,	// �޷��۳��㹻�Ľ�Ǯ
	err_M_Send_Attachment_Oper,	// ������Ʒ��������
	err_M_Send_Level_Limit,		// ���ʼ��ȼ�����
	err_M_Send_MailNum_Limit,	// ��������ɷ��ʼ���
	err_M_Send_Blacklist_Block,	// ��������������
	err_M_Send_Switch_Off,		// �ʼ����Ϳ��عر�
	err_M_Delete_Fee_Nonzero,	// ���и���������Ҫ��ȡ���õ��ʼ�������ֱ��ɾ��
	err_M_Fetch_Bag_No_Space,	// �����������޿ռ�
	err_M_Fetch_No_Enough_Money,// �޷��۳��㹻����ȡ����
	err_M_Fetch_Attach_Error,	// ������ȡʧ��
	err_M_Fetch_Switch_Off,		// ������ȡ���عر�
	err_M_RTS_Type_Unmatch,		// �����ʼ���֧������
	err_M_RTS_Type_No_response,	// �����ʼ������Ӧ
	err_M_RTS_OK,				// ���ʼ�������
	err_M_AttachType_Unsupport,	// �����ݵĸ�������

	err_M_Select_Attach_Unknown,	//��ɾ����δ֪����
	err_M_Select_Attach_ItemLack,	//������Ʒȱʧ
	err_M_Select_Attach_ItemLock,	//������Ʒ������
	err_M_Select_Attach_ItemBind,	//������Ʒ����
	err_M_Select_Attach_CannotMail,	//��Ʒ�����ʼ�
	err_M_Select_Attach_InsertFail,	//���븽��ʧ��
	err_M_Select_Attach_DeleteFail,	//ɾ������ʧ��
	err_M_Select_Attach_LockFail,	//��������ʧ��
	err_M_Select_Attach_UnlockFail,	//��������ʧ��
	err_M_Select_Attach_OverFlow,	//����buffer���

	// chat result

	// item result

	err_logiccount,

	err_unionWorldServer_notAvail,
	err_unionWorld_minLevel,
	err_unionWorld_maxLevel,
	err_unionWorld_notFound,
	err_unionWorld_alreadyApplied,
	err_unionWorld_player_offline,
	err_unionWorld_tooMuch_member,
	err_unionWorld_noTeamFound,
	err_unionWorld_withdrawNotAllowed,

	err_logic_end,
};
