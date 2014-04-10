/*************************************************************************
FileName		:	KSoundStatic.h

Author			:	DuanGuangxiang

Date			:	2012 3 27

Description		:  存取声音效果的属性 (跟特效效果差不多)

CppText			:  KSoundStatic.cpp

***************************************************************************/

#ifndef __KSOUNDSTATIC_H__
#define __KSOUNDSTATIC_H__
#include "cocoa/CCObject.h"
#include <string>

class XMLElementNode;
USING_NS_CC;

class KSoundStatic : public CCObject
{
	KSoundStatic();
public:
	
	~KSoundStatic(){}

	static KSoundStatic* create(XMLElementNode *pDataXml);
	const char* GetName();
	const char* getSoundEffectName()const;	
	bool IsLoop()const{ return m_bLoop;}
	void init();

	float  CreateTime(){ return m_createTime;}
	float SurviveTime(){ return mLifeTime;}
private:
	std::string		mSoundEffectName;
	float			mLifeTime;	//特效持续时间
	float			m_createTime; //生成时间
	bool			m_bLoop;
};

#endif