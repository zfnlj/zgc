#pragma once
#ifdef _USE_COCOS2DX
#include "../common/IAttrSet.h"
#include "../common/KAttrSetCombo.h"
#else
#include <WorldObject/IAttrSet.h>
#include <WorldObject/KAttrSetCombo.h>
#endif
#include "Inc/KCommonDefine.h"
#include "../StaticTable/KAbilityStatic.h"
#include <System/Misc/KStream.h>

class KBattleGuyAttr : public IAttrSet
{
public:
	enum ATTR_1
	{
		ca_1Begin,
		ca_curRes,
		ca_maxRes,
		ca_1End
	};

	enum MaskBit
	{
		CUR_RES		= BIT(0),	//
		MAX_RES		= BIT(1),	//
	};

private:
	typedef KAttrSet_1<ca_1Begin,ca_1End> AttrSet;
	
public:
	DWORD mUpdateMask;
	AttrSet m_attrs;
	DWORD m_deadTime;
	DWORD m_lastAttackTime;
	KCardAbilityList m_bufList;
private:
	void updateMask(DWORD v);
	void updateUnMask(DWORD v);

public:
	KBattleGuyAttr(void);
	~KBattleGuyAttr(void);

	void reset();
	void clearBit();

public: // IAttrSet
	bool IsDirty(){ return mUpdateMask>0;}
	void clearAttrs();
	bool hasAttr(int attrId) const;
	int getAttrValue(int attrId, int defVal=0);
	void setAttrValue(int attrId, int val,bool first=false);
	int setAttrParam(int attrId, int paramId, int val,bool first=false);

	void setAttrObserver(int attrId, IAttrObserver* obv);
	void onAttrChanged(int attrId, int paramId, int val);
	void clone(KBattleGuyAttr*);
	//将修改过的数据写入到msg中
	bool writePacket(KMemoryStream* msg);
	bool writePacketAll(KMemoryStream* msg,bool clear=false);
	bool writePacketFilter(KMemoryStream* msg,DWORD mask,bool clear=false);

	bool readPacket(KMemoryStream* msg,bool first=false);

	int getCurRes(){ return getAttrValue(ca_curRes);}
	void setCurRes(int res){ setAttrValue(ca_curRes,res);}

	int getMaxRes(){ return getAttrValue(ca_maxRes);}
	void setMaxRes(int val){ setAttrValue(ca_maxRes,val);}
};
