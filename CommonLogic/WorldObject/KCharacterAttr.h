#pragma once
#include "IAttrSet.h"
#include "KAttrSetCombo.h"
#include "Inc/KCommonDefine.h"
#include "KAttrDefines.h"
#include <System/Misc/KStream.h>

class KCharacterAttr : public IAttrSet
{
public:
	enum MaskBit
	{
		HP			= BIT(0),	//血量修改
		MP			= BIT(1),	//魔法修改
		STR			= BIT(2),	//体力值
		STR_ATTACK	= BIT(3),	//攻击强度
		STR_DEF		= BIT(4),
		MAG			= BIT(5),
		MAG_ATTACK	= BIT(6),
		MAG_DEF		= BIT(7),
		HIT			= BIT(8),
		BASE_HIT	= BIT(9),
		CRIT		= BIT(10),
		CRIT_NUM	= BIT(11),
		ATTACK_SPEED= BIT(12),
		ATTACK_RANG	= BIT(13),
		MOVE_SPEED	= BIT(14),
		MISS		= BIT(15),
		TOUGH		= BIT(16),
		EXP			= BIT(17),
		MONEY1		= BIT(18),
		MONEY2		= BIT(19),
		MAX_HP		= BIT(20),
		MAX_MP		= BIT(21),
		DEBUFF_STUN	= BIT(22),
		LEVEL		= BIT(23),
		DEBUFF_FIXED= BIT(24),
	};

private:
	typedef KAttrSet_1<KAttributeAbout::ca_1Begin,KAttributeAbout::ca_1End> AttrSet1;
	typedef KAttrSet_1<KAttributeAbout::ca_2Begin,KAttributeAbout::ca_2End> AttrSet2;
	typedef KAttrSet_1<KAttributeAbout::ca_3Begin,KAttributeAbout::ca_3End> AttrSet3;
	
public:
	DWORD mUpdateMask;
	KAttrSetCombo<AttrSet1,AttrSet2,AttrSet3> m_attrs;
	DWORD m_deadTime;
	DWORD m_lastAttackTime;

private:
	void updateMask(DWORD v);
	void updateUnMask(DWORD v);

public:
	KCharacterAttr(void);
	~KCharacterAttr(void);

	void reset();
	void clearBit();

public: // IAttrSet
	void clearAttrs();
	bool hasAttr(int attrId) const;
	int getAttrValue(int attrId, int defVal=0);
	void setAttrValue(int attrId, int val,bool first=false);
	int setAttrParam(int attrId, int paramId, int val,bool first=false);

	void setAttrObserver(int attrId, IAttrObserver* obv);
	void onAttrChanged(int attrId, int paramId, int val);

	//将修改过的数据写入到msg中
	bool writePacket(KMsgOutputStream& msg);
	bool writePacketAll(KMsgOutputStream& msg,bool clear=false);
	bool writePacketFilter(KMsgOutputStream& msg,DWORD mask,bool clear=false);

	bool readPacket(KMsgInputStream& msg,bool first=false);

	//特殊的的属性方法
	bool isDead();
	void setDead();

	//减少血量,返回减少后的血量
	int downHP(int hp);//
	float getAttackSpeed();
	void setAttackSpeed(float attackSpeed);
	float getAttackRange();
	float getMoveSpeed();
	void setMoveSpeed(float attackSpeed);

	float getHit();
	float getBaseHit();
	float getMiss();

	float getCrit();
	float getTough();
	float getCritNum();

	DWORD getTotalNormalAttack();
	DWORD getTotalMagAttack();
	DWORD getTotalNormalDef();
	DWORD getTotalMagDef();

	DWORD getStr() { return getAttrValue(KAttributeAbout::ca_str); }
	void  setStr(int strValue, bool first=false) { setAttrValue(KAttributeAbout::ca_str,strValue, first ); }
	DWORD getStrAttack(){return getAttrValue(KAttributeAbout::ca_strAttack);}
	void setStrAttack(int strAttack,bool first=false){ setAttrValue(KAttributeAbout::ca_strAttack,strAttack,first);}
	DWORD getStrDef() { return getAttrValue(KAttributeAbout::ca_strDef); }
	DWORD getMag(){ return getAttrValue(KAttributeAbout::ca_mag);}
	DWORD getMagAttack(){return getAttrValue(KAttributeAbout::ca_magAttack);}
	DWORD getMagDef(){ return getAttrValue(KAttributeAbout::ca_magDef);}

	DWORD getLevel(){ return getAttrValue(KAttributeAbout::ca_level);}
};
