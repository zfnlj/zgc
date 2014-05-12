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

class KCardStatic;

struct KCardBuffer{
	KAbilityStatic* _pST;
	int _loop;
	KCardBuffer(KAbilityStatic* pST,int loopNum){
		_pST = pST;
		_loop = loopNum;
	}
	bool IsLoopOver(){
		if(_pST->IsLoop()) return false;
		return (_loop >=_pST->LoopNum());
	}
	bool IsLoop(){ return _pST->IsLoop();}
	KAbilityStatic* GetAbility(){ return _pST;}
};

typedef std::list<KCardBuffer> KCardBufferList;

class KCardAttr : public IAttrSet
{
public:
	enum ATTR_1
	{
		ca_1Begin,
		ca_realId,
		ca_cur_hp,
		ca_max_hp,
 		ca_add_atk,
		ca_cardId,
		ca_buf,
		ca_ready,
		ca_slot,
        ca_pos,
		ca_oldSlot,
		ca_1End
	};

	enum MaskBit
	{
		REAL_ID		= BIT(0),	//
		CUR_HP		= BIT(1),	//
		MAX_HP		= BIT(2),	//
		ADD_ATK		= BIT(3),	//
		CARD_ID		= BIT(4),	
		BUF			= BIT(5),
		READY		= BIT(6),
		SLOT		= BIT(7),
        POS         = BIT(8),
		OLDSLOT		= BIT(9),
	};

private:
	typedef KAttrSet_1<ca_1Begin,ca_1End> AttrSet;
	
public:
	DWORD mUpdateMask;
	AttrSet m_attrs;
	DWORD m_deadTime;
	DWORD m_lastAttackTime;
	KCardBufferList m_bufList;
private:
	void updateMask(DWORD v);
	void updateUnMask(DWORD v);

public:
	KCardAttr(void);
	~KCardAttr(void);

	void reset();
	void clearBit();

public: // IAttrSet
	bool IsDirty(){ return mUpdateMask>0;}
	void clearAttrs();
	void clearBuf();
	bool hasAttr(int attrId) const;
	void init(int realId,KCardStatic* st);
	int getAttrValue(int attrId, int defVal=0);
	void setAttrValue(int attrId, int val,bool first=false);
	int setAttrParam(int attrId, int paramId, int val,bool first=false);

	void setAttrObserver(int attrId, IAttrObserver* obv);
	void onAttrChanged(int attrId, int paramId, int val);
	void clone(KCardAttr*);
	//将修改过的数据写入到msg中
	bool writePacket(KMemoryStream* msg);
	bool writePacketAll(KMemoryStream* msg,bool clear=false);
	bool writePacketFilter(KMemoryStream* msg,DWORD mask,bool clear=false);

	bool readPacket(KMemoryStream* msg,bool first=false);
	void updateBufList();
	void DelBuf(int id);
	void DelBuf(KAbilityStatic* pBuf);
	void DelBuf(KAbilityStatic::Enum_What what);
	void AddBuf(KAbilityStatic* pBuf,int loopNum);
	bool HasBuf(KAbilityStatic* pBuf);

	KAbilityStatic* FindRealBuf(KAbilityStatic::Enum_What what);
	KAbilityStatic* FindBufAbility(KAbilityStatic::Enum_What what);
	KAbilityStatic* FindBufAbility(KAbilityStatic::Enum_When when);

	int getRealID(){ return getAttrValue(ca_realId);}
	void setRealId(int val){ setAttrValue(ca_realId,val);}
    
    int getPos(){ return getAttrValue(ca_pos);}
    void setPos(int pos){setAttrValue(ca_pos, pos);}
	void setSlot(int slot);
	int getSlot(){ return getAttrValue(ca_slot);}
	int getOldSlot(){ return getAttrValue(ca_oldSlot);}
	void setMaxHp(int val){setAttrValue(ca_max_hp,val);}
	int getMaxHp(){ return getAttrValue(ca_max_hp);}
	void setCurHp(int val){ setAttrValue(ca_cur_hp,val);}
	int getCurHp(){ return getAttrValue(ca_cur_hp);}
	void setAddAtk(int val){ setAttrValue(ca_add_atk,val);}
	int getAddAtk(){ return getAttrValue(ca_add_atk);}
	void setReady(int val){ setAttrValue(ca_ready,val);}
	int getReady(){ return getAttrValue(ca_ready);}
	void setCardId(int val){ setAttrValue(ca_cardId,val);}
	int getCardId(){ return getAttrValue(ca_cardId);}
};
