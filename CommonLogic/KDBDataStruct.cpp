#include "KDBDataStruct.h"


static ::System::Sync::KThreadMutex _mx_DBData;


void tb_playerquest_record::updateMask( DWORD v )
{
	System::Sync::KSync_CSAuto ______(_mx_DBData);
	_mUpdateMask=_mUpdateMask|v;
}
void tb_playerquest_record::clearMask()
{
	System::Sync::KSync_CSAuto ______(_mx_DBData);
	_mUpdateMask=0;
}
void tb_playerquest_record::updateUnMask(DWORD v)
{
	System::Sync::KSync_CSAuto ______(_mx_DBData);
	_mUpdateMask=_mUpdateMask&~v;
}