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
	void init(UILoadingBar* widget,int p1,int p2,float elapsed);
	void update(float dt);
protected:
	UILoadingBar* m_widget;
	int m_p1;
	int m_p2;
	float m_elapsed;
};

#endif /* defined(__FishingJoy__ProgressBar__) */