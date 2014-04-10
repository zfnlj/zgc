#include "KCharacterAttr.h"
#include "System/Log/log.h"

#pragma warning(disable:4355)
KCharacterAttr::KCharacterAttr(void):m_attrs(*this)
#pragma warning(default:4355)
{
	mUpdateMask=0;
	m_deadTime=0;
	m_lastAttackTime=0;
}

KCharacterAttr::~KCharacterAttr(void)
{
}
void KCharacterAttr::clearAttrs()
{
	m_attrs.clearAttrs();
}

bool KCharacterAttr::hasAttr(int attrId) const
{
	return m_attrs.hasAttr(attrId);
}

int KCharacterAttr::getAttrValue(int attrId, int defVal)
{
	return m_attrs.getAttrValue(attrId, defVal);
}

void KCharacterAttr::setAttrValue(int attrId, int val,bool first)
{
	m_attrs.setAttrValue(attrId, val,first);
}

int KCharacterAttr::setAttrParam(int attrId, int paramId, int val,bool first)
{
	return m_attrs.setAttrParam(attrId, paramId, val,first);
}

void KCharacterAttr::setAttrObserver(int attrId, IAttrObserver* obv)
{
	return m_attrs.setAttrObserver(attrId, obv);
}

void KCharacterAttr::onAttrChanged( int attrId, int paramId, int val )
{
	switch(attrId)
	{
	case KAttributeAbout::ca_level:
		this->updateMask(KCharacterAttr::LEVEL);
		return;
	case KAttributeAbout::ca_hp:
		this->updateMask(KCharacterAttr::HP);
		return;
	case KAttributeAbout::ca_mp:
		this->updateMask(KCharacterAttr::MP);
		return;
	case KAttributeAbout::ca_exp:
		this->updateMask(KCharacterAttr::EXP);
		return;
	case KAttributeAbout::ca_strAttack:
		this->updateMask(KCharacterAttr::STR_ATTACK);
		return ;
	case KAttributeAbout::ca_strDef:
		this->updateMask(KCharacterAttr::STR_DEF);
		return ;
	case KAttributeAbout::ca_mag:
		this->updateMask(KCharacterAttr::MAG);
		return ;
	case KAttributeAbout::ca_magAttack:
		this->updateMask(KCharacterAttr::MAG_ATTACK);
		return ;
	case KAttributeAbout::ca_magDef:
		this->updateMask(KCharacterAttr::MAG_DEF);
		return ;
	case KAttributeAbout::ca_attackSpeed:
		this->updateMask(KCharacterAttr::ATTACK_SPEED);
		return ;
	case KAttributeAbout::ca_attackRange:
		this->updateMask(KCharacterAttr::ATTACK_RANG);
		return ;
	case KAttributeAbout::ca_moveSpeed:
		this->updateMask(KCharacterAttr::MOVE_SPEED);
		return ;
	case KAttributeAbout::ca_hit:
		this->updateMask(KCharacterAttr::HIT);
		return ;
	case KAttributeAbout::ca_baseHit:
		this->updateMask(KCharacterAttr::BASE_HIT);
		return ;
	case KAttributeAbout::ca_miss:
		this->updateMask(KCharacterAttr::MISS);
		return ;
	case KAttributeAbout::ca_crit:
		this->updateMask(KCharacterAttr::CRIT);
		return ;
	case KAttributeAbout::ca_critNum:
		this->updateMask(KCharacterAttr::CRIT_NUM);
		return ;
	case KAttributeAbout::ca_tough:
		this->updateMask(KCharacterAttr::TOUGH);
		return ;
	case KAttributeAbout::ca_maxHP:
		this->updateMask(KCharacterAttr::MAX_HP);
		return ;
	case KAttributeAbout::ca_maxMP:
		this->updateMask(KCharacterAttr::MAX_MP);
		return ;
	case KAttributeAbout::ca_Stun:
		this->updateMask(KCharacterAttr::DEBUFF_STUN);
		return ;
	case KAttributeAbout::ca_CanNotMove:
		this->updateMask(KCharacterAttr::DEBUFF_FIXED);
		return ;
	}
}

bool KCharacterAttr::writePacket(KMsgOutputStream& msg)
{
	return writePacketFilter(msg,0);
}

bool KCharacterAttr::writePacketAll(KMsgOutputStream& msg,bool clear)
{
	DWORD mask =0xFFFFFFFF;
	return writePacketFilter(msg,mask,clear);
}

bool KCharacterAttr::writePacketFilter( KMsgOutputStream& msg,DWORD mask,bool clear)
{
	if(mask==0)
	{
		mask=mUpdateMask;
	}
	else if(mask==0xFFFFFFFF)
	{

	}
	else
		mask = mUpdateMask&mask;

	msg.WriteDword(mask);

	if(mask==0)
		return false;

	if(mask&KCharacterAttr::HP)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_hp));
		if(clear) updateUnMask(KCharacterAttr::HP);
	}
	if(mask&KCharacterAttr::MP)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_mp));
		if(clear) updateUnMask(KCharacterAttr::MP);
	}

	if(mask&KCharacterAttr::STR_DEF)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_strDef));
		if(clear) updateUnMask(KCharacterAttr::STR_DEF);
	}
	if(mask&KCharacterAttr::STR_ATTACK)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_strAttack));
		if(clear) updateUnMask(KCharacterAttr::STR_ATTACK);
	}
	if(mask&KCharacterAttr::MAG)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_mag));
		if(clear) updateUnMask(KCharacterAttr::MAG);
	}
	if(mask&KCharacterAttr::MAG_DEF)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_magDef));
		if(clear) updateUnMask(KCharacterAttr::MAG_DEF);
	}
	if(mask&KCharacterAttr::MAG_ATTACK)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_magAttack));
		if(clear) updateUnMask(KCharacterAttr::MAG_ATTACK);
	}

	if(mask&KCharacterAttr::MOVE_SPEED)
	{
 		float speed = (float)getAttrValue(KAttributeAbout::ca_moveSpeed);
		speed=speed/100;
		msg.WriteFloat(speed);
		if(clear) updateUnMask(KCharacterAttr::MOVE_SPEED);
	}

	if(mask&KCharacterAttr::ATTACK_SPEED)
	{
		float speed = (float)getAttrValue(KAttributeAbout::ca_attackSpeed);
		speed=speed/100;
		msg.WriteFloat(speed);
		if(clear) updateUnMask(KCharacterAttr::ATTACK_SPEED);
	}

	if(mask&KCharacterAttr::ATTACK_RANG)
	{
		float range = (float)getAttrValue(KAttributeAbout::ca_attackRange);
		range=range/100;
		msg.WriteFloat(range);
		if(clear) updateUnMask(KCharacterAttr::ATTACK_RANG);
	}

	if(mask&KCharacterAttr::HIT)
	{
		float value = (float)getAttrValue(KAttributeAbout::ca_hit);
		value=value/100;
		msg.WriteFloat(value);
		if(clear) updateUnMask(KCharacterAttr::HIT);
	}
	if(mask&KCharacterAttr::BASE_HIT)
	{
		float value = (float)getAttrValue(KAttributeAbout::ca_baseHit);
		value=value/100;
		msg.WriteFloat(value);
		if(clear) updateUnMask(KCharacterAttr::BASE_HIT);
	}
	if(mask&KCharacterAttr::MISS)
	{
		float value = (float)getAttrValue(KAttributeAbout::ca_miss);
		value=value/100;
		msg.WriteFloat(value);
		if(clear) updateUnMask(KCharacterAttr::MISS);
	}
	if(mask&KCharacterAttr::CRIT)
	{
		float value = (float)getAttrValue(KAttributeAbout::ca_crit);
		value=value/100;
		msg.WriteFloat(value);
		if(clear) updateUnMask(KCharacterAttr::CRIT);
	}
	if(mask&KCharacterAttr::CRIT_NUM)
	{
		float value = (float)getAttrValue(KAttributeAbout::ca_critNum);
		value=value/100;
		msg.WriteFloat(value);
		if(clear) updateUnMask(KCharacterAttr::CRIT_NUM);
	}
	if(mask&KCharacterAttr::TOUGH)
	{
		float value = (float)getAttrValue(KAttributeAbout::ca_tough);
		value=value/100;
		msg.WriteFloat(value);
		if(clear) updateUnMask(KCharacterAttr::TOUGH);
	}

	if(mask&KCharacterAttr::MAX_HP)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_maxHP));
		if(clear) updateUnMask(KCharacterAttr::MAX_HP);
	}
	if(mask&KCharacterAttr::MAX_MP)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_maxMP));
		if(clear) updateUnMask(KCharacterAttr::MAX_MP);
	}

	if(mask&KCharacterAttr::DEBUFF_FIXED)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_CanNotMove));
		if(clear) updateUnMask(KCharacterAttr::DEBUFF_FIXED);
	}

	if(mask&KCharacterAttr::DEBUFF_STUN)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_Stun));
		if(clear) updateUnMask(KCharacterAttr::DEBUFF_STUN);
	}

	if(mask&KCharacterAttr::DEBUFF_FIXED)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_CanNotMove));
		if(clear) updateUnMask(KCharacterAttr::DEBUFF_FIXED);
	}

	if(mask&KCharacterAttr::LEVEL)
	{
		msg.WriteDword(getAttrValue(KAttributeAbout::ca_level));
		if(clear) updateUnMask(KCharacterAttr::LEVEL);
	}
	
	return true;
}
bool KCharacterAttr::readPacket( KMsgInputStream& msg ,bool first)
{
	DWORD mask=0;
	DWORD dwValue=0;
	float fValue=0;
	msg.ReadDword(mask);

	if(mask&KCharacterAttr::HP)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_hp,dwValue,first);
		updateMask(KCharacterAttr::HP);
	}
	if(mask&KCharacterAttr::MP)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_mp,dwValue,first);
		updateMask(KCharacterAttr::MP);
	}

	if(mask&KCharacterAttr::STR_DEF)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_strDef,dwValue,first);
		updateMask(KCharacterAttr::STR_DEF);
	}
	if(mask&KCharacterAttr::STR_ATTACK)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_strAttack,dwValue,first);
		updateMask(KCharacterAttr::STR_ATTACK);
	}
	if(mask&KCharacterAttr::MAG)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_mag,dwValue,first);
		updateMask(KCharacterAttr::MAG);
	}
	if(mask&KCharacterAttr::MAG_DEF)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_magDef,dwValue,first);
		updateMask(KCharacterAttr::MAG_DEF);
	}
	if(mask&KCharacterAttr::MAG_ATTACK)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_magAttack,dwValue,first);
		updateMask(KCharacterAttr::MAG_ATTACK);
	}

	if(mask&KCharacterAttr::MOVE_SPEED)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_moveSpeed,(int)fValue*100,first);
		updateMask(KCharacterAttr::MOVE_SPEED);
	}

	if(mask&KCharacterAttr::ATTACK_SPEED)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_attackSpeed,(int)fValue*100,first);
		updateMask(KCharacterAttr::ATTACK_SPEED);
	}

	if(mask&KCharacterAttr::ATTACK_RANG)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_attackRange,(int)fValue*100,first);
		updateMask(KCharacterAttr::ATTACK_RANG);
	}

	if(mask&KCharacterAttr::HIT)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_hit,(int)fValue*100,first);
		updateMask(KCharacterAttr::HIT);
	}
	if(mask&KCharacterAttr::BASE_HIT)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_baseHit,(int)fValue*100,first);
		updateMask(KCharacterAttr::BASE_HIT);
	}
	if(mask&KCharacterAttr::MISS)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_miss,(int)fValue*100,first);
		updateMask(KCharacterAttr::MISS);
	}
	if(mask&KCharacterAttr::CRIT)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_crit,(int)fValue*100,first);
		updateMask(KCharacterAttr::CRIT);
	}
	if(mask&KCharacterAttr::CRIT_NUM)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_critNum,(int)fValue*100,first);
		updateMask(KCharacterAttr::CRIT_NUM);
	}
	if(mask&KCharacterAttr::TOUGH)
	{
		msg.ReadFloat(fValue);
		setAttrValue(KAttributeAbout::ca_tough,(int)fValue*100,first);
		updateMask(KCharacterAttr::TOUGH);
	}

	if(mask&KCharacterAttr::MAX_HP)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_maxHP,dwValue,first);
		updateMask(KCharacterAttr::MAX_HP);
	}
	if(mask&KCharacterAttr::MAX_MP)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_maxMP,dwValue,first);
		updateMask(KCharacterAttr::MAX_MP);
	}

	if(mask&KCharacterAttr::DEBUFF_STUN)
	{
		msg.ReadDword(dwValue);
		int oldVal = getAttrValue(KAttributeAbout::ca_Stun);
		if (oldVal != dwValue) 
		{
			setAttrValue(KAttributeAbout::ca_Stun,dwValue,first);
		}
		updateMask(KCharacterAttr::DEBUFF_STUN);
	}

	if(mask&KCharacterAttr::DEBUFF_FIXED)
	{
		msg.ReadDword(dwValue);
		int oldVal = getAttrValue(KAttributeAbout::ca_CanNotMove);
		if (oldVal != dwValue) 
		{
			setAttrValue(KAttributeAbout::ca_CanNotMove,dwValue,first);
		}
		updateMask(KCharacterAttr::DEBUFF_FIXED);
	}

	if(mask&KCharacterAttr::LEVEL)
	{
		msg.ReadDword(dwValue);
		setAttrValue(KAttributeAbout::ca_level,dwValue,first);
		updateMask(KCharacterAttr::LEVEL);
	}
	
	return true;
}
void KCharacterAttr::reset()
{
	mUpdateMask=0;
	m_deadTime=0;
	m_lastAttackTime=0;
	m_attrs.clearAttrs();
}

bool KCharacterAttr::isDead()
{
	return (m_attrs.getAttrValue(KAttributeAbout::ca_hp)==0);
}

void KCharacterAttr::setDead()
{
	setAttrValue(KAttributeAbout::ca_hp,0);
}

int KCharacterAttr::downHP(int hp)
{	
	int curHP = this->getAttrValue(KAttributeAbout::ca_hp);
	if(hp >= curHP)
	{
		setAttrValue(KAttributeAbout::ca_hp, 0);
		return 0;
	}
	else
	{
		this->setAttrValue(KAttributeAbout::ca_hp,curHP-hp);
		return curHP-hp;
	}
}

float KCharacterAttr::getAttackSpeed()
{
	float speed = (float)(this->getAttrValue(KAttributeAbout::ca_attackSpeed));
	return speed/100;
}

void KCharacterAttr::setAttackSpeed(float attackSpeed)
{
	int speed = (int)attackSpeed*100;
	setAttrValue(KAttributeAbout::ca_attackSpeed,speed,false);
}

float KCharacterAttr::getAttackRange()
{
	float range  = (float)(getAttrValue(KAttributeAbout::ca_attackRange));
	return range/100;
}

DWORD KCharacterAttr::getTotalNormalAttack()
{
	DWORD s = getAttrValue(KAttributeAbout::ca_str);
	DWORD sa = getAttrValue(KAttributeAbout::ca_strAttack);
	DWORD strAttack = sa+s*5;
	return strAttack;
}

DWORD KCharacterAttr::getTotalMagAttack()
{
	DWORD m = getAttrValue(KAttributeAbout::ca_mag);
	DWORD ma = getAttrValue(KAttributeAbout::ca_magAttack);
	DWORD magAttack = ma+m*5;
	return magAttack;
}

DWORD KCharacterAttr::getTotalNormalDef()
{
	DWORD s = getAttrValue(KAttributeAbout::ca_str);
	DWORD sd = getAttrValue(KAttributeAbout::ca_strDef);
	DWORD strDef = sd+s*5;
	return strDef;
}

DWORD KCharacterAttr::getTotalMagDef()
{
	DWORD m = getAttrValue(KAttributeAbout::ca_mag);
	DWORD md = getAttrValue(KAttributeAbout::ca_magDef);

	DWORD magDef = md+m*5;
	return magDef;

}

void KCharacterAttr::updateMask( DWORD v )
{
	 mUpdateMask=mUpdateMask|v;
}

void KCharacterAttr::updateUnMask( DWORD v )
{
	mUpdateMask=mUpdateMask&~v;
}

void KCharacterAttr::clearBit()
{
	mUpdateMask=0;
}

float KCharacterAttr::getMoveSpeed()
{
	float speed  = (float)(getAttrValue(KAttributeAbout::ca_moveSpeed));
	return speed/100;
}

void KCharacterAttr::setMoveSpeed( float moveSpeed )
{
	int speed = (int)moveSpeed*100;
	setAttrValue(KAttributeAbout::ca_moveSpeed,speed,false);
}

float KCharacterAttr::getHit()
{
	float value  = (float)(getAttrValue(KAttributeAbout::ca_hit));
	return value/100;
}

float KCharacterAttr::getBaseHit()
{
	float value  = (float)(getAttrValue(KAttributeAbout::ca_baseHit));
	return value/100;
}

float KCharacterAttr::getMiss()
{
	float value  = (float)(getAttrValue(KAttributeAbout::ca_miss));
	return value/100;
}

float KCharacterAttr::getTough()
{
	float value  = (float)(getAttrValue(KAttributeAbout::ca_tough));
	return value/100;
}

float KCharacterAttr::getCrit()
{
	float value  = (float)(getAttrValue(KAttributeAbout::ca_crit));
	return value/100;
}

float KCharacterAttr::getCritNum()
{
	float value  = (float)(getAttrValue(KAttributeAbout::ca_critNum));
	return value/100;
}
