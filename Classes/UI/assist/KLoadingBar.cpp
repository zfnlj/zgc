

#include "KLoadingBar.h"


void KLoadingBar::init(UILoadingBar* widget,int p1,int p2,float elapsed)
{
	m_widget = widget;
	m_p1 = p1;
	m_p2 = p2;
	m_elapsed = elapsed;
}

void KLoadingBar::update(float dt)
{
}