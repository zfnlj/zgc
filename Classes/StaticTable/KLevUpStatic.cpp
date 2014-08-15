#include "KLevUpStatic.h"

#include "../Inc/KTypeDef.h"
#include "KGameStatic.h"


bool KLevUpStatic::init()
{
	m_lev = m_exp = 0;
	return true;
}

KLevUpStatic* KLevUpStatic::create()
{
	KLevUpStatic* pStatic = new KLevUpStatic;
	pStatic->init();
	return pStatic;
}

void KLevUpStatic::Init(System::File::KTabFile2* fileReader)
{
	fileReader->GetInteger("Level", 0, (int*)&m_lev);
	fileReader->GetInteger("Exp", 0, (int*)&m_exp);
}
