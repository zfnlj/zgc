#ifndef _KCARDLAYOUTSTATIC_H_
#define _KCARDLAYOUTSTATIC_H_

#include <System/Collections/DynArray.h>
#include "System/Collections/KMapByVector.h"
#include <list>
#include "../common/KCommonObj.h"
#include <System/File/KTabfileLoader.h>

#define MAX_PNG_FILE_NAME 36
class KCardLayoutStatic
{
public:
    virtual bool init();
	static KCardLayoutStatic* create();
	const char* GetBg(){ return m_bg;}
	const char* GetMask(){return m_mask;}
	bool IsShowCost(){ return m_ShowCost>0;}
	bool IsShowAtk(){ return m_ShowAtk>0;}
	bool IsShowHp(){ return m_ShowHp>0;}
	void Init(System::File::KTabFile2* fileReader);
private:
	int m_Id;
	char m_bg[MAX_PNG_FILE_NAME];
	char m_mask[MAX_PNG_FILE_NAME];
	int  m_ShowCost;
	int  m_ShowAtk;
	int  m_ShowHp;
	
	friend class KGameStaticMgr;
};
typedef System::Collections::KMapByVector<int,KCardLayoutStatic*> KCardLayoutMap;

#endif // __HELLOWORLD_SCENE_H__
