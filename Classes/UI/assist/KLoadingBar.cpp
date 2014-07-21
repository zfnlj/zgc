

#include "KLoadingBar.h"


void KLoadingBar::init(UILoadingBar* widget,float p1,float p2,float interval,float delay)
{
	m_widget = widget;
	m_p1 = p1;
	m_p2 = p2;
	m_interval = interval;
	m_elapsed = 0.0f;
	m_delay = delay;
}

bool KLoadingBar::update(float dt)
{
	m_delay -= dt;
	if(m_delay>0) return false;
	bool ret = false;
	if(m_widget){
		m_elapsed += dt;
		if(m_elapsed>m_interval) m_elapsed = m_interval;

		float dist = (m_p2>m_p1)? m_p2 - m_p1: (1-m_p1+m_p2);
		float p = m_p1+ dist*m_elapsed/m_interval;
		if(p>1.0f) p -= 1.0f;
		m_widget->setPercent(p*100);
		if(m_lastp > p) ret = true;
		m_lastp = p;
		if(m_elapsed>=m_interval){
			m_widget = NULL;
			m_p1 = m_p2 = m_interval = m_elapsed = 0.0f;
		}

	}
	return ret;
}