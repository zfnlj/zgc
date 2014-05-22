#ifndef _KPLAYERDBMGR_H
#define _KPLAYERDBMGR_H

#include "System/Singleton.h"
#include "KCrcAssist.h"


class KPlayerDBMgr : public Singleton<KPlayerDBMgr>
{
public:

	KPlayerDBMgr(){}
	~KPlayerDBMgr(){}
	static KPlayerDBMgr& getSingleton(void);
	void init(){
		m_crc.init();
	}
	unsigned short GetCrc_16(unsigned char * pData, int nLength){
		return m_crc.GetCrc_16(pData,nLength);
	}
private:
	KCrcAssist m_crc;
};

#endif // __HELLOWORLD_SCENE_H__
