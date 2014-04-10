#include "KQuestPersist.h"
#include "KQuestAttrs.h"

using namespace ::System::Collections;

KPersistQuestAttr* KPersistAttr::toQuestAttr()
{
	if(!this->flag) return (KPersistQuestAttr*)this;
	return NULL;
}

KPersistTargetAttr* KPersistAttr::toTargetAttr()
{
	if(this->flag) return (KPersistTargetAttr*)this;
	return NULL;
}

KPersistAttr* KPersistAttr::next()
{
	KPersistQuestAttr* pQattr = this->toQuestAttr();
	if(pQattr) return (KPersistAttr*)((char*)pQattr + pQattr->size());

	KPersistTargetAttr* pTattr = this->toTargetAttr();
	return (KPersistAttr*)((char*)pTattr + pTattr->size());
}

int KPersistAttr::size()
{
	KPersistQuestAttr* pQattr = this->toQuestAttr();
	if(pQattr) return pQattr->size();

	KPersistTargetAttr* pTattr = this->toTargetAttr();
	return pTattr->size();
}

void KPersistQuestAttr::hold()
{
	this->flag = 0;
}

BOOL KPersistQuestAttr::isQuestAttr() const
{
	return this->flag == 0;
}

void KPersistQuestAttr::setAttr(int attrId, const LuaString& val)
{
	ASSERT(attrId >= 0 && attrId < 128);
	ASSERT(val.size() < 256);
	this->attrId = attrId;
	this->length = val.size();
	memcpy(this->_valPtr(), val.c_str(), val.size());
}

int KPersistQuestAttr::getAttr(LuaString& val)
{
	ASSERT(this->length > 0 && this->length < 128);
	ASSERT(this->length < 512);
	val.assign(this->_valPtr(), this->length);
	return this->attrId;
}

int KPersistQuestAttr::size() const
{
	return 2 + this->length;
}

char* KPersistQuestAttr::_valPtr()
{
	return (char*)this+2;
}

void KPersistTargetAttr::hold()
{
	this->flag = 1;
}

BOOL KPersistTargetAttr::isTargetAttr(int group, int target, const char* name)
{
	if(this->group != group) return FALSE;
	if(this->target != target) return FALSE;
	
	int len = strlen(name);
	if(this->nlen != len) return FALSE;

	char* pn = this->_namePtr();
	return _strnicmp(pn, name, len) == 0;
}

void KPersistTargetAttr::setAttr(int group, int target, const LuaString& name, const LuaString& val)
{
	this->group = group;
	this->target = target;
	this->nlen = name.size();
	this->vlen = val.size();
	memcpy(this->_namePtr(), name.c_str(), name.size());
	memcpy(this->_valPtr(), val.c_str(), val.size());
}

LuaString KPersistTargetAttr::getAttr(LuaString& val)
{
	val.assign(this->_valPtr(), this->vlen);
	return LuaString(this->_namePtr(), this->nlen);
}

int KPersistTargetAttr::size() const
{
	return sizeof(DWORD) + this->nlen + this->vlen;
}

char* KPersistTargetAttr::_namePtr()
{
	return (char*)this+sizeof(DWORD);
}

char* KPersistTargetAttr::_valPtr()
{
	return this->_namePtr() + this->nlen;
}

int KPersistQuestBlock::size() const
{
	return sizeof(DWORD)+sizeof(WORD)+this->length;
}

KPersistAttr* KPersistQuestBlock::beginAttr()
{
	if(!this->length) return NULL;
	return (KPersistAttr*)((char*)this + sizeof(DWORD) + sizeof(WORD));
}

KPersistAttr* KPersistQuestBlock::nextAttr(KPersistAttr* pAttr)
{
	char* ep = (char*)this + this->size();
	KPersistAttr* pNext = pAttr->next();
	if((char*)pNext < ep) return pNext;
	return NULL;
}

KPersistQuestBlock* KPersistQuestBlock::next()
{
	return (KPersistQuestBlock*)((char*)this + this->size());
}

BOOL KPersistQuestBlock::validate()
{
	int pos = 0;
	KPersistAttr* pAttr = this->beginAttr();
	while(pAttr)
	{
		int len = pAttr->size();
		if(pos + len > this->length)
		{
			return FALSE;
		}
		pos += len;
		pAttr = this->nextAttr(pAttr);
	}
	return pos == this->length;
}

KDumpPersistObserver::KDumpPersistObserver(KDString<512>& str):m_qid(0),m_str(str)
{

}

KDumpPersistObserver::~KDumpPersistObserver()
{

}

void KDumpPersistObserver::OnQuestAttr(int qid, int attrId, const LuaString& val)
{
	if(m_qid != qid)
	{
		m_qid = qid;
		m_str.append("Quest ").append(qid).append("\n");
	}
	const char* name = KQuestAttr::GetAttrname(attrId);
	m_str.append("\t").append(name).append("=").append(val.c_str()).append("\n");
}

void KDumpPersistObserver::OnTargetAttr(int qid, int group, int target, const LuaString& name, const LuaString& val)
{
	if(m_qid != qid)
	{
		m_qid = qid;
		m_str.append("Quest ").append(qid).append("\n");
	}
	m_str.append("\tgroup_").append(group).append(".target_");
	m_str.append(target).append(".").append(name.c_str());
	m_str.append("=").append(val.c_str()).append("\n");
}

KQuestPersist::KQuestPersist()
{

}

KQuestPersist::~KQuestPersist()
{

}

void KQuestPersist::attach(const void* buf, int len, int capacity)
{
	m_stream.m_pBuf = (char*)buf;
	m_stream.m_pos = len;
	m_stream.m_bufLength = capacity;
}

KDString<512> KQuestPersist::dumpString()
{
	KDString<512> str;
	KDumpPersistObserver obv(str);
	this->goThrough(obv);
	return str;
}

int KQuestPersist::getPersistData(const char** ppData)
{
	*ppData = m_stream.m_pBuf;
	return m_stream.m_pos;
}

void KQuestPersist::goThrough(IPersistObserver& observer)
{
	LuaString name;
	LuaString val;

	KPersistQuestBlock* bptr = (KPersistQuestBlock*)m_stream.m_pBuf;
	KPersistQuestBlock* eptr = (KPersistQuestBlock*)(m_stream.m_pBuf+m_stream.m_pos);
	while(bptr < eptr)
	{
		int qid = bptr->qid;
		KPersistAttr* pAttr = bptr->beginAttr();
		while(pAttr)
		{
			KPersistQuestAttr* pQattr = pAttr->toQuestAttr();
			KPersistTargetAttr* pTattr = pAttr->toTargetAttr();
			if(pQattr)
			{
				int attrId = pQattr->getAttr(val);
				observer.OnQuestAttr(qid, attrId, val);
			}
			else
			{
				name = pTattr->getAttr(val);
				observer.OnTargetAttr(qid, pTattr->group, pTattr->target, name, val);
			}
			pAttr = bptr->nextAttr(pAttr);
		}
		bptr = bptr->next();
	}
}

int KQuestPersist::getQuests(int* qids, int count)
{
	int n = 0;
	KPersistQuestBlock* bptr = (KPersistQuestBlock*)m_stream.m_pBuf;
	KPersistQuestBlock* eptr = (KPersistQuestBlock*)(m_stream.m_pBuf+m_stream.m_pos);
	while(bptr < eptr)
	{
		qids[n++] = bptr->qid;
		if(n >= count) break;
		bptr = bptr->next();
	}
	return n;
}

BOOL KQuestPersist::setQuestAttr(int qid, int attrId, const char* val, int vlen)
{
	BOOL bRet;
	int pos, qpos, len;
	KPersistQuestBlock* pQuest = this->findQuest(qid);
	if(!pQuest)
	{
		pQuest = this->writeQuest(qid);
		if(!pQuest) return FALSE;
	}

	qpos = this->getPos(pQuest);
	
	char buf[256];
	KPersistQuestAttr* pQattr = (KPersistQuestAttr*)&buf;

	pQattr->hold();

	if(!vlen) vlen = strlen(val);
	pQattr->setAttr(attrId, LuaString(val, vlen));

	KPersistAttr* pAttr = pQuest->beginAttr();
	while(pAttr)
	{
		KPersistQuestAttr* pqa = pAttr->toQuestAttr();
		if(pqa)
		{
			if(pqa->attrId == attrId)
			{
				pos = this->getPos(pqa);
				len = pqa->size();

				this->beginUpdate();
				bRet = m_stream.Remove(pos, len);
				if(bRet)
				{
					pQuest = (KPersistQuestBlock*)(m_stream.m_pBuf+qpos);
					pQuest->length -= len;
				}
				this->endUpdate();

				break;
			}
		}
		pAttr = pQuest->nextAttr(pAttr);
	}

	qpos = this->getPos(pQuest);
	pos = qpos + pQuest->size();

	this->beginUpdate();
	bRet = m_stream.Insert(pos, pQattr, pQattr->size());
	if(bRet)
	{
		pQuest = (KPersistQuestBlock*)(m_stream.m_pBuf+qpos);
		pQuest->length += pQattr->size();
	}
	this->endUpdate();

	return bRet;
}

BOOL KQuestPersist::setTargetAttr(int qid, int group, int target, const char* name, const char* val)
{
	BOOL bRet;
	int pos, qpos, len;
	KPersistQuestBlock* pQuest = this->findQuest(qid);
	if(!pQuest)
	{
		pQuest = this->writeQuest(qid);
		if(!pQuest) return FALSE;
	}

	qpos = this->getPos(pQuest);

	char buf[256];
	KPersistTargetAttr* pTattr = (KPersistTargetAttr*)&buf;

	pTattr->hold();
	pTattr->setAttr(group, target, LuaString(name), LuaString(val));

	KPersistAttr* pAttr = pQuest->beginAttr();
	while(pAttr)
	{
		KPersistTargetAttr* pqa = pAttr->toTargetAttr();
		if(pqa)
		{
			if(pqa->isTargetAttr(group, target, name))
			{
				pos = this->getPos(pqa);
				len = pqa->size();

				this->beginUpdate();
				bRet = m_stream.Remove(pos, len);
				if(bRet)
				{
					pQuest = (KPersistQuestBlock*)(m_stream.m_pBuf+qpos);
					pQuest->length -= len;
				}
				this->endUpdate();

				break;
			}
		}
		pAttr = pQuest->nextAttr(pAttr);
	}

	qpos = this->getPos(pQuest);
	pos = this->getPos(pQuest) + pQuest->size();
	this->beginUpdate();
	bRet = m_stream.Insert(pos, pTattr, pTattr->size());
	if(bRet)
	{
		pQuest = (KPersistQuestBlock*)(m_stream.m_pBuf+qpos);
		pQuest->length += pTattr->size();
	}
	this->endUpdate();
	
	return bRet;
}

LuaString KQuestPersist::getQuestAttr(int qid, int attrId)
{
	LuaString name;
	LuaString val;

	KPersistQuestBlock* questBlock = this->findQuest(qid);
	if(!questBlock) return LuaString();

	KPersistAttr* pAttr = questBlock->beginAttr();
	while(pAttr)
	{
		KPersistQuestAttr* pQattr = pAttr->toQuestAttr();
		if(pQattr && pQattr->attrId == attrId)
		{
			pQattr->getAttr(val);
			return val;
		}
		pAttr = questBlock->nextAttr(pAttr);
	}
	return LuaString();
}

void KQuestPersist::removeQuest(int qid)
{
	KPersistQuestBlock* pQuest = this->findQuest(qid);
	if(!pQuest) return;

	int pos = this->getPos(pQuest);
	int len = pQuest->size();

	this->beginUpdate();
	m_stream.Remove(pos, len);
	this->endUpdate();
}

int KQuestPersist::size() const
{
	return m_stream.m_pos;
}

void KQuestPersist::beginUpdate()
{
	return;
}

void KQuestPersist::endUpdate()
{
	return;
}

KPersistQuestBlock* KQuestPersist::findQuest(int qid)
{
	KPersistQuestBlock* bptr = (KPersistQuestBlock*)m_stream.m_pBuf;
	KPersistQuestBlock* eptr = (KPersistQuestBlock*)(m_stream.m_pBuf+m_stream.m_pos);
	while(bptr < eptr)
	{
		if(bptr->qid == qid) return bptr;
		bptr = bptr->next();
	}
	return NULL;
}

BOOL KQuestPersist::validate()
{
	int pos = 0;
	int capacity = m_stream.m_pos;
	KPersistQuestBlock* bptr = (KPersistQuestBlock*)m_stream.m_pBuf;
	KPersistQuestBlock* eptr = (KPersistQuestBlock*)(m_stream.m_pBuf+m_stream.m_pos);
	while(bptr < eptr)
	{
		int len = bptr->size();
		if(pos + len > capacity)
		{
			return FALSE;
		}
		if(!bptr->validate())
		{
			return FALSE;
		}
		pos += len;
		bptr = bptr->next();
	}
	return pos == m_stream.m_pos;
}

KPersistQuestBlock* KQuestPersist::writeQuest(int qid)
{
	char buf[256];
	KPersistQuestBlock* pQuest = (KPersistQuestBlock*)&buf;
	pQuest->qid = qid;
	pQuest->length = 0;

	int pos = this->size();
	int len = pQuest->size();

	this->beginUpdate();
	BOOL bRet = m_stream.Insert(pos, pQuest, len);
	this->endUpdate();

	return bRet?(KPersistQuestBlock*)(m_stream.m_pBuf + pos):NULL;
}

int KQuestPersist::getPos(const void* p)
{
	return (int)((char*)p - m_stream.m_pBuf);
}
