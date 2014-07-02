#include "KCardAttr.h"
#include "../StaticTable/KGameStatic.h"

#pragma warning(disable:4355)
KCardAttr::KCardAttr(void):m_attrs(*this)
#pragma warning(default:4355)
{
	mUpdateMask=0;
	m_deadTime=0;
	m_lastAttackTime=0;
}

KCardAttr::~KCardAttr(void)
{
}
void KCardAttr::clearAttrs()
{
	m_attrs.clearAttrs();
	m_bufList.clear();
}

bool KCardAttr::hasAttr(int attrId) const
{
	return m_attrs.hasAttr(attrId);
}

int KCardAttr::getAttrValue(int attrId, int defVal)
{
	return m_attrs.getAttrValue(attrId, defVal);
}

void KCardAttr::setAttrValue(int attrId, int val,bool first)
{
	m_attrs.setAttrValue(attrId, val,first);
}

void KCardAttr::init(int realId,KCardStatic* st)
{
	clearAttrs();
	setRealId(realId);
	setCardId(st->GetID());
	setReady(0);
	setMaxHp(st->GetHp());
	setCurHp(st->GetHp());
}

void KCardAttr::clone(KCardAttr* attr)
{
	m_bufList.clear();
	KCardBufferList::iterator it = attr->m_bufList.begin();
	while(it != attr->m_bufList.end()){
		m_bufList.push_back(*it);
		it++;
	}

	setRealId(attr->getRealID());
	setAddAtk(attr->getAddAtk());
	setCurHp(attr->getCurHp());
	setMaxHp(attr->getMaxHp());
	setCardId(attr->getCardId());
	setReady(attr->getReady());
	setSlot(attr->getSlot());
}

void KCardAttr::updateBufList()
{
	for(KCardBufferList::iterator it= m_bufList.begin();it!=m_bufList.end();){
		KCardBuffer& buf = *it;
		buf._loop++;
		if(buf.IsLoopOver()){
			it = m_bufList.erase(it);
			this->updateMask(KCardAttr::BUF);
		}else{
			it++;
		}
	}
}
int KCardAttr::setAttrParam(int attrId, int paramId, int val,bool first)
{
	return m_attrs.setAttrParam(attrId, paramId, val,first);
}

void KCardAttr::setAttrObserver(int attrId, IAttrObserver* obv)
{
	return m_attrs.setAttrObserver(attrId, obv);
}

void KCardAttr::onAttrChanged( int attrId, int paramId, int val )
{
	switch(attrId)
	{
	case ca_realId:
		this->updateMask(KCardAttr::REAL_ID);
		return;
	case ca_cur_hp:
		this->updateMask(KCardAttr::CUR_HP);
		return;
	case ca_max_hp:
		this->updateMask(KCardAttr::MAX_HP);
		return;
	case ca_add_atk:
		this->updateMask(KCardAttr::ADD_ATK);
		return;
	case ca_cardId:
		this->updateMask(KCardAttr::CARD_ID);
		return;
	case ca_ready:
		this->updateMask(KCardAttr::READY);
		return;
	case ca_slot:
		this->updateMask(KCardAttr::SLOT);
		return;
	}
}

bool KCardAttr::writePacket(KMemoryStream* msg)
{
	return writePacketFilter(msg,0);
}

bool KCardAttr::writePacketAll(KMemoryStream* msg,bool clear)
{
	DWORD mask =0xFFFFFFFF;
	return writePacketFilter(msg,mask,clear);
}

bool KCardAttr::writePacketFilter( KMemoryStream* msg,DWORD mask,bool clear)
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

	if(mask&KCardAttr::REAL_ID)
	{
		msg->WriteWord((WORD)getAttrValue(ca_realId));
		if(clear) updateUnMask(REAL_ID);
	}
	
	if(mask&KCardAttr::CUR_HP)
	{
		msg->WriteShort((short)getAttrValue(ca_cur_hp));
		if(clear) updateUnMask(CUR_HP);
	}

	if(mask&KCardAttr::MAX_HP)
	{
		msg->WriteByte((BYTE)getAttrValue(ca_max_hp));
		if(clear) updateUnMask(MAX_HP);
	}

	if(mask&KCardAttr::ADD_ATK)
	{
		msg->WriteShort((short)getAttrValue(ca_add_atk));
		if(clear) updateUnMask(ADD_ATK);
	}

	if(mask&KCardAttr::CARD_ID)
	{
		msg->WriteInt(getAttrValue(ca_cardId));
		if(clear) updateUnMask(CARD_ID);
	}

	if(mask&KCardAttr::READY)
	{
		msg->WriteByte((BYTE)getAttrValue(ca_ready));
		if(clear) updateUnMask(READY);
	}

	if(mask&KCardAttr::SLOT)
	{
		msg->WriteByte((BYTE)getAttrValue(ca_slot));
		if(clear) updateUnMask(SLOT);
	}
	if(mask&KCardAttr::POS)
	{
		msg->WriteByte((BYTE)getAttrValue(ca_pos));
		if(clear) updateUnMask(POS);
	}
	if(mask&KCardAttr::BUF)
	{
		BYTE n = (BYTE)m_bufList.size();
		if(!msg->WriteByte(n))
			return false;
		KCardBufferList::iterator it = m_bufList.begin();
		while(it != m_bufList.end()){
			KCardBuffer& buf = *it;
			msg->WriteInt(buf._pST->GetId());
			msg->WriteByte((BYTE)buf._loop);
			it++;
		}
	}

	return true;
}
bool KCardAttr::readPacket( KMemoryStream* msg ,bool first)
{
	DWORD mask=0;
	int iValue = 0;
	WORD wValue = 0;
	BYTE bValue = 0;
	short sValue = 0;
	msg->ReadDword(mask);

	if(mask&KCardAttr::REAL_ID)
	{
		msg->ReadWord(wValue);
		setAttrValue(ca_realId,wValue,first);
		updateMask(KCardAttr::REAL_ID);
	}
	if(mask&KCardAttr::CUR_HP)
	{
		msg->ReadShort(sValue);
		setAttrValue(ca_cur_hp,sValue,first);
		updateMask(KCardAttr::CUR_HP);
	}
	if(mask&KCardAttr::MAX_HP)
	{
		msg->ReadByte(bValue);
		setAttrValue(ca_max_hp,bValue,first);
		updateMask(KCardAttr::MAX_HP);
	}
	if(mask&KCardAttr::ADD_ATK)
	{
		msg->ReadShort(sValue);
		setAttrValue(ca_add_atk,sValue,first);
		updateMask(KCardAttr::ADD_ATK);
	}
	if(mask&KCardAttr::CARD_ID)
	{
		msg->ReadInt(iValue);
		setAttrValue(ca_cardId,iValue,first);
		updateMask(KCardAttr::CARD_ID);
	}
	if(mask&KCardAttr::READY)
	{
		msg->ReadByte(bValue);
		setAttrValue(ca_ready,bValue,first);
		updateMask(KCardAttr::READY);
	}
	if(mask&KCardAttr::SLOT)
	{
		msg->ReadByte(bValue);
		setAttrValue(ca_slot,bValue,first);
		updateMask(KCardAttr::SLOT);
	}
	if(mask&KCardAttr::POS)
	{
		msg->ReadByte(bValue);
		setAttrValue(ca_pos,bValue,first);
		updateMask(KCardAttr::POS);
	}
	if(mask&KCardAttr::BUF)
	{
		BYTE n;
		BYTE loop;
		msg->ReadByte(n);
		
		m_bufList.clear();
		for(int i=0;i<n;i++){
			int bufId;
			msg->ReadInt(bufId);
			msg->ReadByte(loop);
			KAbilityStatic* buf = KGameStaticMgr::getSingleton().GetAbilityOnId(bufId);
			AddBuf(buf,loop);
		}
	}
	return true;
}
void KCardAttr::reset()
{
	mUpdateMask=0;
	m_deadTime=0;
	m_lastAttackTime=0;
	m_attrs.clearAttrs();
}

void KCardAttr::updateMask( DWORD v )
{
	 mUpdateMask=mUpdateMask|v;
}

void KCardAttr::updateUnMask( DWORD v )
{
	mUpdateMask=mUpdateMask&~v;
}

void KCardAttr::clearBit()
{
	mUpdateMask=0;
}

void KCardAttr::DelBuf(KAbilityStatic::Enum_What what)
{
	updateMask(KCardAttr::BUF);
	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KCardBuffer& buf = *it;

		if(buf._pST->GetWhat() == what){
			it = m_bufList.erase(it);
		}else{
			it++;
		}
	}
}

void KCardAttr::DelBuf(int id)
{
	updateMask(KCardAttr::BUF);
	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetId() == id){
			m_bufList.erase(it);
			return;
		}
		it++;
	}
}

void KCardAttr::DelBuf(KAbilityStatic* pBuf)
{
	updateMask(KCardAttr::BUF);
	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST == pBuf){
			m_bufList.erase(it);
			return;
		}
		it++;
	}
}

void KCardAttr::AddBuf(KAbilityStatic* pBuf,int loopNum)
{
	if(!pBuf) return;
	if(HasBuf(pBuf)) return;
	m_bufList.push_back(KCardBuffer(pBuf,loopNum));
	updateMask(KCardAttr::BUF);
}

void KCardAttr::clearBuf()
{
	m_bufList.clear();
	updateMask(KCardAttr::BUF);
}

bool KCardAttr::HasBuf(KAbilityStatic* pBuf)
{
	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhat()==KAbilityStatic::what_buf){
			if(buf._pST == pBuf) return true;
		}else if(buf._pST->GetWhat()==pBuf->GetWhat()){
			return true;
		}
		
		it++;
	}
	return false;
}

int KCardAttr::GetBufVal()
{
	int val = 0;
	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhat()==KAbilityStatic::what_buf){
			KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(buf._pST->GetNormalVal());
			val += pBuf->GetBufVal();
		}else{
			val += buf._pST->GetBufVal();
		}
		it++;
	}
	return val;
}

KAbilityStatic* KCardAttr::FindRealBuf(KAbilityStatic::Enum_What what)
{
	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhat()== what) return buf._pST;
		it++;
	}
	return NULL;
}

KAbilityStatic* KCardAttr::FindBufAbility(KAbilityStatic::Enum_What what)
{
	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KCardBuffer& buf = *it;
		if(buf._pST->GetWhat()==KAbilityStatic::what_buf){
			KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(buf._pST->GetNormalVal());
			if(pBuf && pBuf->GetWhat()==what && pBuf->GetWhich()==KAbilityStatic::which_i) return pBuf;
		}
		it++;
	}
	return NULL;
}

KAbilityStatic* KCardAttr::FindBufAbility(KAbilityStatic::Enum_When when)
{
	//KCardBufferList abilityList;
	//KGameStaticMgr::getSingleton().GetAbilityList(getCardId(),abilityList,when);
	//if(!abilityList.empty()) return *(abilityList.begin());

	KCardBufferList::iterator it = m_bufList.begin();
	while(it != m_bufList.end()){
		KAbilityStatic* pAbility = (*it)._pST;
		if(pAbility->GetWhat()==KAbilityStatic::what_buf){
			KAbilityStatic* pBuf = KGameStaticMgr::getSingleton().GetAbilityOnId(pAbility->GetNormalVal());
			if(pBuf && pBuf->GetWhen()==when ) return pBuf;
		}
		it++;
	}
	return NULL;
}

void KCardAttr::setSlot(int slot)
{ 
	setAttrValue(ca_oldSlot,getSlot());
	setAttrValue(ca_slot,slot);
}