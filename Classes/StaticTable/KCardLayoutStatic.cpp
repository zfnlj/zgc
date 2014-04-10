#include "KCardLayoutStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>

bool KCardLayoutStatic::init()
{
	memset(m_bg,0,sizeof(m_bg));
	memset(m_mask,0,sizeof(m_mask));
	m_Id = m_ShowCost = m_ShowAtk = m_ShowHp = 0;
	return true;
}

KCardLayoutStatic* KCardLayoutStatic::create()
{
	KCardLayoutStatic* pDeck = new KCardLayoutStatic;
	pDeck->init();
	return pDeck;
}

void KCardLayoutStatic::Init(System::File::KTabFile2* fileReader)
{
	fileReader->GetInteger("ID", 0, (int*)&m_Id);
	fileReader->GetString("BG", "", m_bg, MAX_PNG_FILE_NAME);
	fileReader->GetString("CARD_MASK", "", m_mask, MAX_PNG_FILE_NAME);
	fileReader->GetInteger("COST", 0, (int*)&m_ShowCost);
	fileReader->GetInteger("ATK", 0, (int*)&m_ShowAtk);
	fileReader->GetInteger("HP", 0, (int*)&m_ShowHp);
}
