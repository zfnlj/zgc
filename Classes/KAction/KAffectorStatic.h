/********************************************************************
	created:	2012/01/05
	created:	5:1:2012   10:51
	filename: 	i:\SVN\JiGuang2\Athena\KGame3DBase\KAction\KAffectorStatic.h
	file path:	i:\SVN\JiGuang2\Athena\KGame3DBase\KAction
	file base:	KAffectorStatic
	file ext:	h
	author:		dengkai
	
	purpose:	动作组效果器(禁帧、透明、加亮、变材质)的静态数据读取
*********************************************************************/

#pragma once

#include "KActionElemBase.h"
#include "cocoa/CCString.h"
class XMLElementNode;


enum AffectorType
{
	Affector_null = 0,
	Affector_layer, //改层
	Affector_scale, //缩放
	Affector_visible, //缩放
	Affector_fadein,
	Affector_createSprite,
	Affector_armature,
	Affector_addwidget,//向层增加widget
	Affector_delwidget,//删除widget
	Affector_fadeout,
	Affector_atkMove,
	Affector_shakex, //震动
	Affector_move, //移动
	Affector_moveback, //移回
	Affector_update, //更新
	Affector_hit, //更新
	Affector_bright,
	Affector_eff, //移动
	Affector_missile,
	Affector_summon,
	Affector_summonSelf,
	Affector_anim,
	Affector_replace,
	Affector_action,
	Affector_draw,
	Affector_indicate,
};


class KAffectorStatic :public KActionElemBase
{
public:
	KAffectorStatic()	
	{	
	}
	KAffectorStatic(AffectorType tp)	
	{	
		m_BaseData.m_eType = tp;
	}
	void init(XMLElementNode* pDataXml);
	virtual ~KAffectorStatic();								

	static AffectorType GetEnumByTypeName( const char * szName );

	virtual bool parseScript(XMLElementNode* pDataXml);
	static KAffectorStatic* CreateAffectorStatic(AffectorType tp);
	virtual const char*	TypeName(void)					
	{
		static char buf[128]={0};
		switch (m_BaseData.m_eType)
		{
		case Affector_bright:
			strcpy(buf,"bright");
			break;
		case Affector_null:
			strcpy(buf,"null");
			break;
		default:
			strcpy(buf,"Affector_null");
			break;	
		}
		return buf;
	};
	virtual AffectorType Type(void)							{return  m_BaseData.m_eType;};
	const char* GetObj(){ return m_BaseData.m_obj;}
	const char* GetSlot(){ return m_BaseData.m_slot;}
	void SetSurviveTime(float fLife)
	{
		m_BaseData.m_surviveTime = fLife;
	}

	float GetSurviveTime()
	{
		return m_BaseData.m_surviveTime;
	}
	float GetCreateTime(){
		return m_BaseData.m_createTime;
	}
	int GetIntVal(){ return m_BaseData.m_nVal;}
	float GetFloatVal(){ return m_BaseData.m_fVal;}
	struct AffBaseData 
	{
		float m_surviveTime;
		float m_createTime;
		AffectorType m_eType;
		int   m_nVal;
		float m_fVal;
		char m_obj[64];
		char m_slot[64];
		AffBaseData():
		m_eType(Affector_null),
		m_surviveTime(0)
		,m_createTime(0.0f)
		,m_nVal(0)
		,m_fVal(0.0f)
		{
			memset(m_obj,0,sizeof(m_obj));
			memset(m_slot,0,sizeof(m_slot));
		}

		AffBaseData &operator =( const AffBaseData& data)
		{
			memcpy(this,&data,sizeof(data));
			return *this;
		}
	};
	virtual void* GetBaseData(){ return &m_BaseData;}
	virtual int GetBaseSize(){ return sizeof(m_BaseData);}
	virtual void* GetExData(){ return NULL;}
	virtual int GetExSize() { return 0;}

	void	SetBaseData( const AffBaseData &pBD )
	{
		m_BaseData = pBD;
	}
protected:	
	AffBaseData m_BaseData;
};

class KAffectorActionStatic : public KAffectorStatic
{
public:
    enum tar_flag_enum{
        tar_src,
        tar_des,
        tar_my,
        tar_your,
    };
	KAffectorActionStatic()										;
	virtual ~KAffectorActionStatic()								{}

	void init(XMLElementNode* pDataXml);
	virtual bool parseScript(XMLElementNode* pDataXml)		;
	int GetPreId(){ return m_Data.m_preId;}
    tar_flag_enum GetTarFlag(){ return m_Data.m_tarFlag;}
	struct SActionData 
	{
		int m_preId;
		tar_flag_enum m_tarFlag;
		SActionData():m_preId(0),m_tarFlag(tar_src)
		{
		}
		SActionData &operator= ( const SActionData &data )
		{
			memcpy(this,&data,sizeof(data));
			return *this;
		}
		
	};

	SActionData *GetData()
	{
		return &m_Data;
	}

	void SetData( const SActionData &data )
	{
		m_Data = data; 
	}

	virtual void* GetExData(){ return &m_Data;}
	virtual int GetExSize() { return sizeof(m_Data);}

private:
	SActionData m_Data;
};


