//#pragma once
///********************************************************************
//history:
//created:	2013/05/15
//created:	05:15:2013
//
//filename: 	LoginPacket.h
//file path:	
//file base:	LoginPacket
//file ext:	h
//
//author:		Calvin
//copyright:  
//
//purpose:	LoginServer Packet Defines
//comment:    
//*********************************************************************/
//
//#include "GlobalDefine.h"
//#include "passportDefine.h"
//// Login -> Client
//#define		LC_NOTIFY_MSG_LEN		256
//enum LC_PROTOCOL
//{
//	s_nLC_PacketStart = 1 * 0x100 + 0x10,
//
//
//	s_nLC_Connected = s_nLC_PacketStart,
//
//
//	s_nLC_ConnError,
//	s_nLC_ConnClosed,
//	s_nLC_LoginRsp,
//	s_nLC_ServerAreaInfos,
//
//	s_nLC_LOGIN_NEED_QUEUE_RSP,		// 登陆应答 正在排队
//	s_nLC_LOGIN_PASS_QUEUE_RSP,		// 登陆应答 更新排队信息
//	s_nLC_LOGOUT_RSP,				// 登出应答
//
//	s_nLC_PasswordProtect_Valid,	// 验证密保数据
//	s_nLC_PasswordProtect_Error,	// 密保验证失败了
//
//	// 客户端连上后，首先发防盗号数据
//	s_nLC_ProtectPack,
//
//	// 客户端发完TabFile文件后，再发服务器列表
//	s_nLC_DIFF_TABFILE_RSP_NoneUsed,			// TabFile移到CharacterServer上发送了，这里没用了
//	s_nLC_DIFF_TABFILE_COUNT_RSP_NoneUsed,	// TabFile移到CharacterServer上发送了，这里没用了
//	s_nLC_LAST_CHARACTERSERVER,
//
//	s_nLC_AreaNameAlias,			// 大区名字
//
//	s_nLC_NotifyMsg,				// 提示信息
//
//	s_nLC_RecommendServer,			// 推荐服务器信息
//
//	s_nLC_SessionKey_Rsp,			// 回复SessionKey
//
//	s_nLC_EncryptScript_Rsp,		// 回复EncryptScript
//
//
//	s_nLC_3rdCommonMsg,				// 从Passport返回的第三方平台信息
//
//	s_nLC_ExtraOperation_Rsp,		// 额外操作应答
//
//
//	s_nLC_PacketEnd,
//};
//
//// Client -> Login
//enum CL_PROTOCOL
//{
//	s_nCL_Connected =  1 * 0x100 + 0x10,
//	s_nCL_ConnError,
//	s_nCL_ConnClosed,
//	s_nCL_LoginReq,
//	s_nCL_Message, // testing
//
//
//	s_nCL_LOGOUT_REQ,				// 请求登出
//	s_nCL_LINK_START,				// 连接成功
//	s_nCL_LINK_OVER,				// 断开连接
//	s_nCL_WORLD_LINK_START,			// World连接成功
//	s_nCL_PasswordProtect_Rsp,		// 密保结果回复
//	s_nCL_PasswordProtect_Cancel,	// 取消密保输入
//	s_nCL_RecommendServer,			// 玩家对推荐服务器的选择
//	s_nCL_SessionKey_Req,			// 请求SessionKey
//	s_nCL_EncryptScript_Req,		// 请求EncryptScript
//	s_nCL_ExtraOperation_Req,		// 请求额外操作
//	s_nCL_LoginExtra_Req,			// 请求登录包（带版本号，为了向下兼容）
//	s_nCL_PacketEnd,
//};
//
//// Login ->Character
//enum LCh_PROTOCOL
//{
//	s_nLCh_PacketStart = s_nCL_PacketEnd,
//	s_nLCh_Connected = s_nLCh_PacketStart,
//
//	s_nLCh_Character_Count_Req,
//	s_nLCh_Character_Index,
//
//	s_nLCh_ConnClosed,
//	s_nLCh_PacketEnd
//};
//
//// Character -> Login
//enum ChL_PROTOCOL
//{
//	s_nChL_PacketStart = s_nLCh_PacketEnd,
//	s_nChL_CharacterServer_Notarize,
//	s_nChL_Character_Count_Rsp,
//	s_nChL_PacketEnd
//};
//
//#pragma pack(push,1)
//// Login Paket Client - Server
//struct CL_LoginReq
//{
//	enum { s_nCode = s_nCL_LoginReq };
//	char acctName[64];
//	unsigned char passwdMd5[16];
//};
//
//struct CL_LoginExtra_Req 
//{
//	enum { s_nCode = s_nCL_LoginExtra_Req, };
//	INT64 m_LoginPacketVer;	
//};
//
//// Login Paket Server - Client
//struct LC_LoginRsp
//{
//	enum { s_nCode = s_nLC_LoginRsp, };
//	enum 
//	{
//		error_Succeed = 0,
//		error_User_Not_Found,	// 用户名未找到
//		error_Psd_Error,		// 密码不正确
//		error_Already_Online,	// 已经登陆
//		error_OTP_error,		// OTP错误
//		error_Version_error,	// 版本错误
//		error_DB_error,
//		error_IllegalCharValid,	// 含有敏感字符
//		error_ServerClose,		// 系统维护中
//		error_AccountBlocked,	// 帐号被封停
//		error_WallowProtect,	// 盛大防沉迷
//		error_AdultUnApply,		// 盛大未登记
//		error_NoGetway,			// 没有网关可用
//	};
//	int result;
//	DWORD otpCode;
//	KPassport passport;
//};
//
//// Extra Login Operation Paket Client - Server
//struct CL_ExtraOperationReq
//{
//	enum { s_nCode = s_nCL_ExtraOperation_Req, };
//	st_LoginInfo loginInfo;
//	DWORD m_dwMainVersion;
//	DWORD m_dwSubVersion;
//	FLOAT m_fVersion;
//	DWORD m_dwOperationType;
//	enum { enum_Mac_Addr_Len = 32};
//	char m_MacAddr[enum_Mac_Addr_Len];
//	st_Accelerate_Platform m_AccePlatform;
//};
//
//// Extra Login Operation Server - Client
//struct LC_ExtraOperationRsp
//{
//	enum { s_nCode = s_nLC_ExtraOperation_Rsp, };
//	st_PassportKey m_passportKey;
//	DWORD m_dwOTPCode;
//	INT miResult;
//	DWORD m_dwWallowSec;
//	DWORD m_dwOperationType;
//	st_Accelerate_Platform m_AccePlatform;
//};
//
//struct LC_LoginNeedQueueRsp
//{
//	enum { s_nCode = s_nLC_LOGIN_NEED_QUEUE_RSP, };
//	st_PassportKey m_passportKey;
//	DWORD m_CurrentQueueCount;		// 当前队列总数
//};
//
//struct LC_LoginPlayerPassQueueCountRsp
//{
//	enum { s_nCode = s_nLC_LOGIN_PASS_QUEUE_RSP, };
//	DWORD	m_PassQueueCount;		// 从队列中登陆游戏成功的人数
//};
//
//struct LC_PasswordProtect_Valid 
//{
//	enum { s_nCode = s_nLC_PasswordProtect_Valid,};
//	KProtectAskInfo m_ProtectResult;
//};
//
//// Login Paket Client - Server
//struct CL_Link_Over //: public KPacket
//{
//	enum { s_nCode = s_nCL_LINK_OVER, };
//	DWORD m_dwConnectIndex;
//};
//
//struct CL_World_Link_Start //: public KPacket
//{
//	enum { s_nCode = s_nCL_WORLD_LINK_START, };
//	DWORD m_dwConnectIndex;
//};
//
//struct CL_Link_Start //: public KPacket
//{
//	enum { s_nCode = s_nCL_LINK_START, };
//	DWORD m_dwConnectIndex;
//};
//
//struct LC_ServerInfoRsp
//{
//	enum { s_nCode = s_nLC_ServerAreaInfos, };
//	STRU_SERVER_INFO m_struServerInfo;
//};
//
//struct LC_LAST_CHARACTERSERVER
//{
//	enum { s_nCode = s_nLC_LAST_CHARACTERSERVER, };
//	INT m_nAreaID;
//	INT	m_nServerID;
//};
//
//struct LC_ProtectPack
//{
//	enum { s_nCode = s_nLC_ProtectPack };
//	// byte sequence of protect pack data
//};
//
//struct LC_PasswordProtect_Error
//{
//	enum { s_nCode = s_nLC_PasswordProtect_Error, };
//	int byResult;
//};
//
//struct CL_LogoutReq
//{
//	enum { s_nCode = s_nCL_LOGOUT_REQ, };
//};
//
//struct LC_LogoutRsp
//{
//	enum { s_nCode = s_nLC_LOGOUT_RSP, };
//};
//
//struct CL_PasswordProtect_Rsp 
//{
//	enum { s_nCode = s_nCL_PasswordProtect_Rsp,	};
//	int	m_Result[3];
//};
//
//struct CL_PasswordProtect_Cancel
//{
//	enum { s_nCode = s_nCL_PasswordProtect_Cancel,	};
//};
//
//
//struct LC_AreaNameAlias 
//{
//	enum { s_nCode = s_nLC_AreaNameAlias,			};
//};
//
//struct LC_NotifyMsg 
//{
//	enum { s_nCode = s_nLC_NotifyMsg,			};
//	LC_NotifyMsg()
//	{
//		memset(this, 0 ,sizeof(LC_NotifyMsg));
//	}
//	DWORD m_dwMsgID;
//	CHAR m_achMsg[LC_NOTIFY_MSG_LEN];
//};
//
//struct LC_RecommendServer 
//{
//	enum { s_nCode = s_nLC_RecommendServer,		};
//	LC_RecommendServer()
//	{
//		memset(this, 0 , sizeof(LC_RecommendServer));
//	}
//};
//
//struct LC_SessionKey_Rsp 
//{
//	enum { s_nCode = s_nLC_SessionKey_Rsp,		};
//	LC_SessionKey_Rsp()
//	{
//		memset(this, 0 , sizeof(LC_SessionKey_Rsp));
//	}
//	st_OP_KEY ucPubKey;
//};
//
//
//struct LC_EncryptScript_Rsp 
//{
//	enum { s_nCode = s_nLC_EncryptScript_Rsp,		};
//	LC_EncryptScript_Rsp()
//	{
//		memset(this, 0 , sizeof(LC_EncryptScript_Rsp));
//	}
//};
//
//
//
//struct CL_RecommendServer
//{
//	enum { s_nCode = s_nCL_RecommendServer,		};
//	BYTE mbySelectRecommendServer;
//};
//
//struct CL_SessionKey_Req
//{
//	enum { s_nCode = s_nCL_SessionKey_Req,		};
//};
//
//struct CL_EncryptScript_Req
//{
//	enum { s_nCode = s_nCL_EncryptScript_Req,		};
//	int		s_nRandomKey;		//迷惑性的随机数
//	char	s_chRecorver[16];
//	CL_EncryptScript_Req()
//		: s_nRandomKey(0)
//	{
//		memset( s_chRecorver, 0, sizeof(s_chRecorver) );
//	}
//};
//
//struct LCh_CharacterCountReq
//{
//	enum { s_nCode = s_nLCh_Character_Count_Req, };
//	st_PassportKey m_passportKey;
//	DWORD m_dwAccountType;
//};
//
//struct LCh_Character_Index
//{
//	enum { s_nCode = s_nLCh_Character_Index, };
//	DWORD m_nIndex;
//};
//
//struct ChL_CharacterServer_NotarizeReq
//{
//	enum { s_nCode = s_nChL_CharacterServer_Notarize, };
//	DWORD m_dwIndex;
//};
//
//struct ChL_CharacterCountRsp
//{
//	enum { s_nCode = s_nChL_Character_Count_Rsp, };
//	st_PassportKey m_passportKey;
//	STRU_SERVER_INFO m_struServerInfo;
//	int m_Result;
//};
//
//
//struct LC_3rdCommonMsg : public K3rdCommonMsg
//{
//	enum { s_nCode = s_nLC_3rdCommonMsg};
//};
//
//
//
//#pragma pack(pop)
//
