#include "KQuestManager.h"
#include <System/File/KTabfileLoader.h>
#include "KGlobalFunction.h"
//#include "../KGameServerApp.h"
#include "../WorldObject/KPlayer.h"
//#include <Attribute/KAttrDefine.h>
#ifdef _USE_COCOS2DX
#include "platform/CCFileUtils.h"
#include "ccMacros.h"
#endif
#include "../common/KCommonObj.h"
#include "../WorldObject/KMainPlayer.h"
#include <string>
#include "../KNet/KNetMsgFacade.h"
using namespace KAttributeAbout;
using namespace System::File;

bool KQuestManager::m_initFlag = false;	// 初始化标志
KQuestManager::KQuestManager(void)
{
}

KQuestManager::~KQuestManager(void)
{
}

KQuestManager* KQuestManager::GetInstance()
{
	static KQuestManager* p = new KQuestManager();
	return p;
}

bool KQuestManager::Initialize(void)
{
	if (m_initFlag)
	{
		return true;
	}
	//todo: load quest scripts

	// 临时通过读取所有任务的
	LoadLuaScript("QuestUtils.lua");
	doDirQuest("Quest");
	LoadAvailQuestList();
	m_initFlag = true;	// 设置初始化标志
	return true;
}

void KQuestManager::Breathe(int interval)
{
	// 
}

KQuestNew* KQuestManager::GetQuest(int id)
{
	// 先看是否是普通任务
	QuestMap::iterator it = m_mapQuest.find(id);
	if(it != m_mapQuest.end() )
	{
		return it->second;
	}
	if(!this->ReloadQuest(id))
	{
		return NULL;
	}

	return m_mapQuest[id];
}

void KQuestManager::LoadLuaScript(const char* fileName)
{
	static char scriptFileName[256];
	sprintf(scriptFileName, "data/Quest/%s", fileName);

	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(scriptFileName);
#else
	fullPath = scriptFileName;
#endif
	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	StreamInterface* pStream = KTabfileLoader::GetInstance().GetFileStream(fullPath.c_str(), "Quest");
	if(pStream) LuaWraper.doStream(*pStream);
	KTabfileLoader::GetInstance().CloseFileStream(pStream);
}


bool KQuestManager::ReloadQuest(int qid)
{
    KQuestNew* pQuest = m_mapQuest[qid];
	if(pQuest)
	{
		pQuest->Reset();
		KQuestNew::Free(pQuest);

		m_mapQuest[qid] = NULL;
		pQuest = NULL;
	}

	static char achQuestFileName[256];
	sprintf(achQuestFileName, "data/Quest/QuestScripts/q_%d.lua", qid);

	//sprintf(achQuestFileName, "../../data/configs/Quest/QuestScripts/Q_%d.lua", qid);
	std::string fullPath;
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(achQuestFileName);
#else
	fullPath = achQuestFileName;
#endif
	KTabfileLoader& loader = KTabfileLoader::GetInstance();
	StreamInterface* pStream = KTabfileLoader::GetInstance().GetFileStream(fullPath.c_str(), "Quest");
	if(!pStream || LuaWraper.doStream(*pStream) == FALSE)
	{
		Log(LOG_ERROR, "error: quest %s", achQuestFileName);
		if (pQuest)
		{
			pQuest->Reset();
			KQuestNew::Free(pQuest);
			pQuest = NULL;
		}
		if(pStream) KTabfileLoader::GetInstance().CloseFileStream(pStream);
		return false;
	}
	KTabfileLoader::GetInstance().CloseFileStream(pStream);

	char tableName[256];
	sprintf(tableName, "Quest_%d", qid);
	pQuest = KQuestNew::Alloc(); pQuest->Reset();
	if(!pQuest->Initialize(qid, tableName))
	{
		pQuest->Reset();
		KQuestNew::Free(pQuest);
		return false;
	}

	ASSERT(!m_mapQuest[qid]);
	m_mapQuest[qid] = pQuest;

	return true;
}

int KQuestManager::doDirQuest(const char* resGrpName)
{
	//int ret = lwp_error;
	//ASSERT_RETURN(resGrpName, lwp_error);
	//FileInfoListPtr ptr = ResourceGroupManager::getSingleton().findFileFromLocationIndex(resGrpName, "*.lua");
	//for (FileInfoList::iterator itfile = ptr->begin(); itfile != ptr->end(); itfile++)
	//{
	//	FileInfo& fileInfo = (*itfile);
	//	String sFullPathName = fileInfo.archive->getName();
	//	sFullPathName += "/";
	//	sFullPathName += fileInfo.filename;

	//	int qid = 0;
	//	if(sscanf(fileInfo.filename.c_str(), "q_%d.lua", &qid) > 0)
	//	{	// 如果是任务脚本则导入任务
	//		String sWorldFilter = "*/";
	//		sWorldFilter += "ProfessionQuest";
	//		sWorldFilter += "/*";
	//		if (StringUtil::match(sFullPathName, sWorldFilter, false))
	//		{
	//			if (!ReloadProfessionQuest(qid))
	//			{
	//				Log(LOG_ERROR, "error: load profquest %d", qid);
	//			}
	//		}
	//		else if(!ReloadQuest(qid))
	//		{
	//			Log(LOG_ERROR, "error: load quest %d", qid);
	//		}
	//	}
	//	else
	//	{	// 否则直接导入任务功能脚本
	//		StreamInterface* pStream = KTabfileLoader::GetInstance().GetFileStream(fileInfo.filename.c_str(), resGrpName);
	//		if(!pStream || LuaWraper.doStream(*pStream) == FALSE)
	//		{
	//			Log(LOG_ERROR, "error: quest %s", sFullPathName.c_str());
	//		}
	//		if(pStream) KTabfileLoader::GetInstance().CloseFileStream(pStream);
	//	}
	//}

	//return ret;
	return 0;
}


quest_array_header* KQuestManager::get_quest_array(DWORD npcTypeID)
{
	NpcQuestMap::iterator it = m_npcQuestMap.find(npcTypeID);
	if(it == m_npcQuestMap.end()) return NULL;
	return it->second;
}

bool KQuestManager::IsDynTalkQuest( int qid )
{
	KQuestNew* pQuest = GetQuest(qid);
	return IsDynTalkQuest(pQuest);
}

bool KQuestManager::IsDynTalkQuest( KQuestNew* pQuest )
{
	return false;
}

void KQuestManager::GetFigureAward( KDString<512>& GiftStr, KQuestNew* pQuest )
{
	//const char* tmp = LuaWraper.Call<const char* ,KQuestNew*>("GetQuestFigureAwardStr", pQuest);
	//GiftStr.append(tmp);
	return;
}

bool KQuestManager::syncAvailQuests()
{
	KMsgOutputStream& so = g_ostream;
	so.Seek(0);

	KWorldObjAbout::KPlayer* pPlayer = KMainPlayer::RealPlayer();
	int level = pPlayer->getLevel();

	AvailQuestMap::iterator it = m_availQuestMap.begin();
	for(; it!= m_availQuestMap.end(); it++)
	{
		AvailQuestDesc& availDesc = it->second;
		KQuestNew* pQuest = GetQuest(availDesc.m_nQuestId);
		if(!pQuest) continue;

		// 等级
		if (level < availDesc.m_nMinLevelLimit ||
			level > availDesc.m_nMaxLevelLimit)
			continue;
		// 已完成或已接
		if (pPlayer->GetActiveQuest(availDesc.m_nQuestId) || 
			pPlayer->IsQuestEverDone(availDesc.m_nQuestId))
			continue;
		// 有前置任务但没完成
		int prevQuestId = pQuest->m_prevQuest;
		if(prevQuestId && !pPlayer->IsQuestEverDone(prevQuestId))
			continue;

		so.WriteInt(availDesc.m_nQuestId);
	}

	int len = so.m_pos;
	char* buf = so.m_pBuf;
	KNetMsgFacade::syncAvailQuests(buf,len);
	return TRUE;
}

bool KQuestManager::LoadAvailQuestList(void)
{
	std::string fullPath = "data/AcceptQuestList.txt";
#ifdef _USE_COCOS2DX
	fullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fullPath.c_str());
#endif

	KTabfileLoader& loader = KTabfileLoader::GetInstance();

	KTabFile2* fileReader = loader.GetFileReader(fullPath.c_str());
	if (!fileReader)
	{
		AssertFile(fullPath.c_str());
		return FALSE;
	}

	AvailQuestDesc availDesc;

	while (TRUE)
	{
		int nRet = fileReader->ReadLine();
		if (nRet == -1) { loader.CloseFileReader(fileReader); return FALSE; }
		if (nRet == 0) break;

		memset(&availDesc, 0, sizeof(availDesc));

		fileReader->GetInteger("QuestID", 0, (int*)&availDesc.m_nQuestId);
		fileReader->GetInteger("Level", 0, &availDesc.m_nLevel);
		fileReader->GetInteger("MinLevelLimit", 0, &availDesc.m_nMinLevelLimit);
		fileReader->GetInteger("MaxLevelLimit", 0, &availDesc.m_nMaxLevelLimit);

		m_availQuestMap[availDesc.m_nQuestId] = availDesc;
	}

	loader.CloseFileReader(fileReader);
	return TRUE;
}
