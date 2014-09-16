#include "KTowerAwardStatic.h"

#include "../Inc/KTypeDef.h"
#include "KGameStatic.h"


bool KTowerAwardStatic::init()
{
	m_lev = m_money = m_fateStone = 0;
	return true;
}

KTowerAwardStatic* KTowerAwardStatic::create()
{
	KTowerAwardStatic* pStatic = new KTowerAwardStatic;
	pStatic->init();
	return pStatic;
}

void KTowerAwardStatic::Init(System::File::KTabFile2* fileReader)
{
	fileReader->GetInteger("Level", 0, (int*)&m_lev);
	fileReader->GetInteger("Money", 0, (int*)&m_money);
	fileReader->GetInteger("Stone", 0, (int*)&m_fateStone);
}
