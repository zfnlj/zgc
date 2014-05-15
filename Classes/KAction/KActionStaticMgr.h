/********************************************************************
	created:	
	created:	
	filename: 	
	file path:	
	file base:	
	file ext:	
	author:		
	
	purpose:	action �ľ�̬�ļ���
*********************************************************************/
#pragma once

#include "KAffectorStatic.h"
#include "KSoundStatic.h"

#include "System/Collections/KHashTable.h"
#include "cocos2d.h"
#include "System/Singleton.h"

class ConfigFile;
USING_NS_CC;

class KActionStatic :public CCObject
{


	
	
public:
	enum classEnum{
		class_cast,  //������
		class_hit, //ʩ����
		class_move,//״̬��
		class_null, //
	};
	struct SActionData 
	{
		SActionData();	
		char    m_name[32];
		unsigned long  m_crc; 
		classEnum m_class;
		unsigned long   m_loop               : 1;                      //ѭ��
		unsigned long   m_slot				 : 4;
		SActionData &operator =( const SActionData& data)
		{
			memcpy(this,&data,sizeof(SActionData));
			return *this;
		}
	};
	KActionStatic();
	KActionStatic(const char*);
	
	KActionStatic(const KActionStatic& );
	KActionStatic& operator=( const KActionStatic& );
	void setName(const char* name);
	unsigned long GetCRC() const{
		return m_ActionData.m_crc;
	}
	unsigned long CalcCRC();
	classEnum GetClass(void) const{ return m_ActionData.m_class;}
	void ParseClass(const char* className);
	int GetSlot(){
		return m_ActionData.m_slot;
	}
	bool IsLoop(void) const
	{
		return m_ActionData.m_loop>0;
	}
	const char* GetName(void) const;
	const char* GetDefAnim(void) const;
	void AddAffector(KAffectorStatic *obj)
	{
		if(obj)
		{
			m_AffectorVect.addObject(obj);
		}
	}

	void AddSoundEffect(KSoundStatic* soundEffect)
	{
		m_SoundEffectVec.addObject(soundEffect);
	}

	CCArray m_effectSTVect;      //��Ч��Ϣ
	CCArray m_AffectorVect;      //Ч����Ϣ
	CCArray m_SoundEffectVec;
	SActionData		m_ActionData;	// ��װһ���ȹص�����,Ȩ�������л���
};

//����action�ļ�����������action�ľ�̬��Ϣ��

namespace Ogre {
	class DataStream;
}

class KActionStaticMgr : public Singleton<KActionStaticMgr>
{
public:
	KActionStaticMgr(){};
	~KActionStaticMgr()
	{
		Clear();
	};
	void init(){}
	void Clear();
	KActionStatic* GetAction(const char* name);
	static KActionStaticMgr& getSingleton(void);
	KActionStatic* NewAction(const char* name);
	
	KActionStatic* LoadActionXmlFile(const char* name);
	void AddAction(KActionStatic* pST);

	static bool m_bNoAction;

private:
	KActionStatic* LoadXmlActionData(const char* name,Ogre::DataStream* dataStream);
	CCDictionary m_ActionDict; 
};
