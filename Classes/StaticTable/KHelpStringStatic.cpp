#include "KHelpStringStatic.h"
#include "string.h"
#include <System/Log/log.h>
#include "../Inc/KTypeDef.h"
#include <System/Misc/StrUtil.h>

bool KHelpStringStatic::init()
{
	return true;
}

KHelpStringStatic* KHelpStringStatic::create()
{
	KHelpStringStatic* pDeck = new KHelpStringStatic;
	pDeck->init();
	return pDeck;
}

void KHelpStringStatic::Init(System::File::KTabFile2* fileReader)
{
	fileReader->GetInteger("ID", 0, (int*)&m_Id);
	char buf[512]={0};
	fileReader->GetString("String", "", buf, 511);
	m_des = buf;

	fileReader->GetString("Author", "", buf, 511);
	if(strlen(buf)>2) m_author = buf;
}
