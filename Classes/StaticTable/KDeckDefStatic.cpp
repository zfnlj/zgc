#include "KDeckDefStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>

bool KDeckDefStatic::init()
{
	m_heroID = m_heroHp = m_res = 0;
	return true;
}

KDeckDefStatic* KDeckDefStatic::create()
{
	KDeckDefStatic* pDeck = new KDeckDefStatic;
	pDeck->init();
	return pDeck;
}

void KDeckDefStatic::setDeck(const char* buf)
{
	char strBuf[1024];
	strcpy(strBuf,buf);

	m_cardList.clear();
	char* ss[64];
	int ns = split(strBuf, ";", ss, 64);

	for(int i=0; i<ns; i++)
	{
		char* s = _trim(ss[i]);
		int id = atoi(s);
		if(id>0) m_cardList.push_back(id);
	}
}

void KDeckDefStatic::Init(System::File::KTabFile2* fileReader)
{
	char buf[1024];
	fileReader->GetInteger("ID", 0, (int*)&m_Id);
	fileReader->GetInteger("HeroID", 0, (int*)&m_heroID);
	fileReader->GetInteger("HeroHp", 0, (int*)&m_heroHp);
	fileReader->GetInteger("Res", 0, (int*)&m_res);
	fileReader->GetString("DECK", "", buf, 1023);
	fileReader->GetInteger("DrawNum", 0, (int*)&m_drawNum);
	setDeck(buf);
}
