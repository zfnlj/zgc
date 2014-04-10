#ifndef __KDECKDEFSTATIC_H__
#define __KDECKDEFSTATIC_H__

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>

class KDeckDefStatic
{
public:
    virtual bool init();
	static KDeckDefStatic* create();
	void setDeck(const char* buf);
	int getHero(){ return m_heroID;}
	int getRes(){ return m_res;}
	int getHeroHp(){ return m_heroHp;}
	int getDrawNum(){return m_drawNum;}
	KIntegerList& getCardList(){return m_cardList;}
	void Init(System::File::KTabFile2* fileReader);
private:
	int m_Id;
	int m_heroID;
	int m_heroHp;
	int m_res;
	int m_drawNum;
	
	KIntegerList m_cardList;
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KDeckDefStatic*> KDeckDefMap;

#endif // __HELLOWORLD_SCENE_H__
