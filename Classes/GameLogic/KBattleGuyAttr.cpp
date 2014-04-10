#include "KBattleGuyAttr.h"
#include "../StaticTable/KGameStatic.h"

#pragma warning(disable:4355)
KBattleGuyAttr::KBattleGuyAttr(void):m_attrs(*this)
#pragma warning(default:4355)
{
	mUpdateMask=0;
	m_deadTime=0;
	m_lastAttackTime=0;
}

KBattleGuyAttr::~KBattleGuyAttr(void)
{
}
void KBattleGuyAttr::clearAttrs()
{
	m_attrs.clearAttrs();
}

bool KBattleGuyAttr::hasAttr(int attrId) const
{
	return m_attrs.hasAttr(attrId);
}

int KBattleGuyAttr::getAttrValue(int attrId, int defVal)
{
	return m_attrs.getAttrValue(attrId, defVal);
}

void KBattleGuyAttr::setAttrValue(int attrId, int val,bool first)
{
	m_attrs.setAttrValue(attrId, val,first);
}

void KBattleGuyAttr::clone(KBattleGuyAttr* attr)
{
	setCurRes(attr->getCurRes());
	setMaxRes(attr->getMaxRes());
}

int KBattleGuyAttr::setAttrParam(int attrId, int paramId, int val,bool first)
{
	return m_attrs.setAttrParam(attrId, paramId, val,first);
}

void KBattleGuyAttr::setAttrObserver(int attrId, IAttrObserver* obv)
{
	return m_attrs.setAttrObserver(attrId, obv);
}

void KBattleGuyAttr::onAttrChanged( int attrId, int paramId, int val )
{
	switch(attrId)
	{
	case ca_curRes:
		this->updateMask(KBattleGuyAttr::CUR_RES);
		return;
	case ca_maxRes:
		this->updateMask(KBattleGuyAttr::MAX_RES);
		return;
	}
}

bool KBattleGuyAttr::writePacket(KMemoryStream* msg)
{

	return writePacketFilter(msg,0);
}

bool KBattleGuyAttr::writePacketAll(KMemoryStream* msg,bool clear)
{
	DWORD mask =0xFFFFFFFF;
	return writePacketFilter(msg,mask,clear);
}

bool KBattleGuyAttr::writePacketFilter( KMemoryStream* msg,DWORD mask,bool clear)
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

	msg->WriteDword(mask);

	if(mask==0)
		return false;

	if(mask&KBattleGuyAttr::CUR_RES)
	{
		msg->WriteInt(getAttrValue(ca_curRes));
		if(clear) updateUnMask(CUR_RES);
	}
	
	if(mask&KBattleGuyAttr::MAX_RES)
	{
		msg->WriteInt(getAttrValue(ca_maxRes));
		if(clear) updateUnMask(MAX_RES);
	}

	return true;
}
bool KBattleGuyAttr::readPacket( KMemoryStream* msg ,bool first)
{
	DWORD mask=0;

	int iValue = 0;

	msg->ReadDword(mask);

	if(mask&KBattleGuyAttr::CUR_RES)
	{
		msg->ReadInt(iValue);
		setAttrValue(ca_curRes,iValue,first);
		updateMask(KBattleGuyAttr::CUR_RES);
	}
	if(mask&KBattleGuyAttr::MAX_RES)
	{
		msg->ReadInt(iValue);
		setAttrValue(ca_maxRes,iValue,first);
		updateMask(KBattleGuyAttr::MAX_RES);
	}
	return true;
}
void KBattleGuyAttr::reset()
{
	mUpdateMask=0;
	m_deadTime=0;
	m_lastAttackTime=0;
	m_attrs.clearAttrs();
}

void KBattleGuyAttr::updateMask( DWORD v )
{
	 mUpdateMask=mUpdateMask|v;
}

void KBattleGuyAttr::updateUnMask( DWORD v )
{
	mUpdateMask=mUpdateMask&~v;
}

void KBattleGuyAttr::clearBit()
{
	mUpdateMask=0;
}

