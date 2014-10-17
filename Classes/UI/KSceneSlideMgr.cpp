#include "KSceneSlideMgr.h"
#include "ccMacros.h"
#include "../Inc/KTypeDef.h"

USING_NS_CC;

IMPLEMENT_SIMPLE_SINGLETON(KSceneSlideMgr)


void KSceneSlideMgr::Clear()
{
	m_firstId = m_lastId = m_curId = 0;
}

void KSceneSlideMgr::OpenSceneSlide(int first,int last)
{
	m_firstId = first;
	m_lastId = last;
	m_curId = first;
}

bool KSceneSlideMgr::NextSlide()
{
	if( m_curId>=m_lastId) return false;
	m_curId++;
	return true;
}

bool KSceneSlideMgr::PrevSlide()
{
	if( m_curId<=m_firstId) return false;
	m_curId--;
	return true;
}

int KSceneSlideMgr::GetSlideNum()
{
	return (m_lastId-m_firstId+1);
}