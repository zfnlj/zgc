#ifndef _KLVEUPSTATIC_H
#define _KLVEUPSTATIC_H

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>

#include <System/File/KTabfileLoader.h>



class KLevUpStatic
{
public:

	static KLevUpStatic* create();
	bool init();
	void Init(System::File::KTabFile2* fileReader);
	int GetLev(){ return m_lev;}
	int GetExp(){ return m_exp;}
private:
	int m_lev;
	int m_exp;
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KLevUpStatic*> KLevUpStaticMap;

#endif // __HELLOWORLD_SCENE_H__
