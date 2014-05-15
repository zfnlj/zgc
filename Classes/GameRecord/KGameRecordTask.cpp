#include "KGameRecordTask.h"
#include "KRecordFile.h"
#include "System/Misc/KStream.h"

KRecordDataBase* KGameRecordTask::AllocData( EGameRecordedDataType eType )
{
	KRecordDataBase* pRecordData = NULL;
	switch(eType){
	case EGRDT_UIMouseInput:
		pRecordData = new KRecordPlayOpData;
		break;
	case EGRDT_PlayOp:
		pRecordData = new KRecordUIMouseData;
		break;
	}
	pRecordData->init();
	return pRecordData;
}

void KGameRecordTask::RecordPlayOp(int src,int des,int slot)
{
	KRecordDataBase* pRecord = AllocData(EGRDT_PlayOp);
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
	int count = m_FrameData.size();
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
	unsigned int uDataCount;
	char cDataType;
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
	m_FrameData.empty();
}