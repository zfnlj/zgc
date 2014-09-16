#ifndef _KTOWERAWARDSTATIC_H
#define _KTOWERAWARDSTATIC_H

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>

#include <System/File/KTabfileLoader.h>



class KTowerAwardStatic
{
public:

	static KTowerAwardStatic* create();
	bool init();
	void Init(System::File::KTabFile2* fileReader);
	int GetLev(){ return m_lev;}
	int GetMoney(){ return m_money;}
	int GetFateStone(){return m_fateStone;}
private:
	int m_lev;
	int m_money;
	int m_fateStone;
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KTowerAwardStatic*> KTowerAwardStaticMap;

#endif // __HELLOWORLD_SCENE_H__
