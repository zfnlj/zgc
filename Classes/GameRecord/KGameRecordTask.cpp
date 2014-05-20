#include "KGameRecordTask.h"
#include "KRecordFile.h"

#include "../Inc/KTypeDef.h"
#include "../GameRoot.h"
#include "../UI/BattleFieldScene.h"
#include "../UI/KUIAssist.h"

#define DATA_BUF_SIZE 2048

void KGameRecordTask::init()
{
	m_dataBuf = new char[DATA_BUF_SIZE];
	m_deckStream.bind(m_dataBuf,DATA_BUF_SIZE);
	m_pCurOpera = NULL;
}

KRecordDataBase* KGameRecordTask::AllocData( EGameRecordedDataType eType )
{
	KRecordDataBase* pRecordData = NULL;
	switch(eType){
	case EGRDT_UIMouseInput:
		pRecordData = new KRecordUIMouseData;
		break;
	case EGRDT_PlayOp:
		pRecordData = new KRecordPlayOpData;
		break;
	}
	pRecordData->init(m_dlgInc++);
	return pRecordData;
}

void KGameRecordTask::RecordPlayOp(int src,int des,int slot)
{
	KRecordPlayOpData* pRecord = (KRecordPlayOpData*)AllocData(EGRDT_PlayOp);
	pRecord->RecordPlayOp(src,des,slot,m_startRecordTime);
	m_FrameData.push_back(pRecord);
}

void KGameRecordTask::RecordMouseEvt(KRecordUIMouseData::Mouse_evt evt)
{
	KRecordUIMouseData* pRecord = (KRecordUIMouseData*)AllocData(EGRDT_UIMouseInput);
	pRecord->RecordMouseEvt(evt,m_startRecordTime);
	m_FrameData.push_back(pRecord);
}

bool KGameRecordTask::Load( const char* szFileName)
{
	KInputFileStream inputStream;
	KRecordFile file;
	if(!file.LoadFile(szFileName,&inputStream)) return false;
	DeserializeData(&inputStream);
	return true;
}

bool KGameRecordTask::Save( const char* szFileName)
{
	KGameRecordDataSaveStream* pMemoryStream = new KGameRecordDataSaveStream;
	if( !SerializeData( pMemoryStream ) ){
		delete pMemoryStream;
		return false;
	}

	KRecordFile file;
	file.SaveToFile(szFileName,pMemoryStream->GetTargetDataStream(), pMemoryStream->GetTargetDataStreamSize());
	delete pMemoryStream;
	return true;
}

bool KGameRecordTask::SerializeData( StreamInterface* pStream )
{
	pStream->WriteData( &m_head, sizeof(m_head));
	unsigned int deckSize = m_deckStream.size();
	pStream->WriteData( &deckSize, 4 );
	pStream->WriteData(m_deckStream.data(),deckSize);

	unsigned int count = m_FrameData.size();
	pStream->WriteData( &count, 4 );
	for(KRecordDataList::iterator it = m_FrameData.begin();it!=m_FrameData.end();++it){
		KRecordDataBase* pData = *it;
		char cDataType = (char)pData->GetClassType();
		pStream->WriteData( &cDataType, 1 );
		pData->Serialize( pStream );
	}
	return true;
}
bool KGameRecordTask::DeserializeData( StreamInterface* pStream )
{
	pStream->ReadData( &m_head, sizeof(m_head));
	unsigned int deckSize,uDataCount;
	char cDataType;
	m_deckStream.clear();
	pStream->ReadData( &deckSize, 4 );
	pStream->ReadData( m_deckStream.m_pBuf, deckSize);
	
	pStream->ReadData( &uDataCount, 4 );
	for ( unsigned int ii=0; ii<uDataCount; ii++ )
	{
		pStream->ReadData( &cDataType, 1 );
		KRecordDataBase* pData = AllocData( (EGameRecordedDataType)cDataType );
		if( !pData )
		{
			return false;
		}
		m_FrameData.push_back(pData);
		pData->Deserialize( pStream );
	}
	return true;
}

void KGameRecordTask::Empty()
{
	for(KRecordDataList::iterator it = m_FrameData.begin();it!=m_FrameData.end();){
		KRecordDataBase* pData = *it;
		it++;
		delete pData;
	}
	m_FrameData.clear();
	m_deckStream.clear();
}

void KGameRecordTask::StartRecrod(int dlgInc)
{
	m_dlgInc = dlgInc;
	Empty();
	m_startRecordTime = GetTickCount();
	GameRoot::getSingleton().BattleCtrl().serializeAll(&m_deckStream);
}

void KGameRecordTask::StartPlay()
{
	GameRoot::getSingleton().BattleCtrl().deserializeAll(&m_deckStream);
	GameRoot::getSingleton().getBattleScene()->ReGenerateAllCard();
	m_timeline = 0;
}

void KGameRecordTask::onPlayStepOn()
{
	m_timeline = m_pCurOpera->GetElapsed()*0.001;
	CC_SAFE_DELETE(m_pCurOpera);
	KUIAssist::_stopClickAction();
	KUIAssist::_stopAdviceMsg();
}

bool KGameRecordTask::Play(float elapsed)
{
	m_timeline+= elapsed;
	if(m_pCurOpera){ 
		m_pCurOpera->Replay(m_timeline*1000,m_head._mode);
	}
	if(!m_pCurOpera &&m_FrameData.size()>0){
		KRecordDataList::iterator it = m_FrameData.begin();
		m_pCurOpera = *it;
		m_FrameData.erase(it);
	}
	return (m_pCurOpera!=NULL);
}

void KGameRecordTask::Stop()
{
	CC_SAFE_DELETE(m_pCurOpera);
}

bool KGameRecordTask::IsClickCardValidate(KCardInst* card)
{
	if(!m_pCurOpera) return true;
	return m_pCurOpera->IsClickCardValidate(card);
}

bool KGameRecordTask::IsClickFightAreaValidate(int slot)
{
	if(!m_pCurOpera) return true;
	return m_pCurOpera->IsClickFightAreaValidate(slot);
}

bool KGameRecordTask::IsClickButValidate(cocos2d::CCObject* obj)
{
	if(!m_pCurOpera) return true;
	return m_pCurOpera->IsClickButValidate(obj);
}