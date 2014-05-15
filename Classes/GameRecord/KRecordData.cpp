#include "KRecordData.h"


bool KRecordDataBase::Deserialize( StreamInterface* pDataStream )
{
	pDataStream->ReadData(&m_dlgId,4);
	return true;
}
bool KRecordDataBase::Serialize( StreamInterface* pDataStream )
{
	pDataStream->WriteData(&m_dlgId,4);
	return true;
}

bool KRecordPlayOpData::Deserialize(StreamInterface* pDataStream)
{
	KRecordDataBase::Deserialize(pDataStream);
	pDataStream->ReadData(&m_data,sizeof(m_data));
	return true;
}

bool KRecordPlayOpData::Serialize( StreamInterface* pDataStream )
{
	KRecordDataBase::Serialize(pDataStream);
	pDataStream->WriteData(&m_data,sizeof(m_data));
	return true;
}

bool KRecordUIMouseData::Deserialize(StreamInterface* pDataStream)
{
	KRecordDataBase::Deserialize(pDataStream);

	return true;
}

bool KRecordUIMouseData::Serialize( StreamInterface* pDataStream )
{
	KRecordDataBase::Serialize(pDataStream);
	return true;
}