/*************************************************************************
FileName		:	KSoundStatic.h

Author			:	DuanGuangxiang

Date			:	2012 3 27

Description		:  ��ȡ����Ч�������� (����ЧЧ�����)

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
	float			mLifeTime;	//��Ч����ʱ��
	float			m_createTime; //����ʱ��
	bool			m_bLoop;
};

#endif