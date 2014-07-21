//
//  ProgressBar.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-30.
//
//

#ifndef _KLOADINGBAR_H
#define _KLOADINGBAR_H
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d::extension;

class KLoadingBar
{
public:
	KLoadingBar():m_widget(NULL){}
	void init(UILoadingBar* widget,float p1,float p2,float interval,float delay=0.0f);
	bool update(float dt);
	
protected:
	UILoadingBar* m_widget;
	float m_p1;
	float m_p2;
	float m_elapsed;
	float m_interval;
	float m_lastp;
	float m_delay;
};

#endif /* defined(__FishingJoy__ProgressBar__) */