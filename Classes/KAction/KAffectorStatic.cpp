#include "KAffectorStatic.h"
#include <System/tinyxml/xmldom.h>


KAffectorStatic* KAffectorStatic::CreateAffectorStatic(AffectorType tp)
{
	KAffectorStatic* pAffector=NULL;
	switch ( tp )
	{
	//case Affector_missile:
	//	pAffector = new KAffectorTBrightStatic;
	//	break;
	default:
		pAffector = new KAffectorStatic(tp);
 	}
	return pAffector;
}

KAffectorStatic::~KAffectorStatic()
{
}

void KAffectorStatic::init(XMLElementNode* pDataXml)
{
	parseScript(pDataXml);
}

bool KAffectorStatic::parseScript(XMLElementNode* pDataXml)
{
	memset(&m_BaseData,0,sizeof(AffBaseData));
	const StringValue* pClass = pDataXml->QueryAtt("class");
	m_BaseData.m_eType = GetEnumByTypeName(pClass->c_str());

	const StringValue* pObj = pDataXml->QueryAtt("obj");
	if(pObj){
		strcpy(m_BaseData.m_obj,pObj->c_str());
	}
	const StringValue* pSlot = pDataXml->QueryAtt("slot");
	if(pSlot){
		strcpy(m_BaseData.m_slot,pSlot->c_str());
	}
	const StringValue* pCreateTime = pDataXml->QueryAtt("createtime");
	m_BaseData.m_createTime = (pCreateTime)?pCreateTime->Float():0.0f;

	const StringValue* plifeTime = pDataXml->QueryAtt("life");
	m_BaseData.m_surviveTime = (plifeTime)? atof(plifeTime->c_str()):1.0f;

	const StringValue* pVal = pDataXml->QueryAtt("fVal");
	m_BaseData.m_fVal = (pVal)? pVal->Float():0.0f;

	pVal = pDataXml->QueryAtt("iVal");
	m_BaseData.m_nVal = (pVal)? pVal->Integer():0.0f;

	return true;
}

////////////////////////////////////////////////////////////////////////
KAffectorActionStatic::KAffectorActionStatic()
{
	m_BaseData.m_eType = Affector_bright;	
}

void KAffectorActionStatic::init(XMLElementNode* pDataXml)
{
	parseScript(pDataXml);
}

bool KAffectorActionStatic::parseScript(XMLElementNode* pDataXml)
{
	const StringValue* pVal = pDataXml->QueryAtt("tarFlag");
	m_Data.m_tarFlag = (KAffectorActionStatic::tar_flag_enum)((pVal)? pVal->Integer():1);
	pVal = pDataXml->QueryAtt("pre");
	m_Data.m_preId = (pVal)? pVal->Integer():0;

	KAffectorStatic::parseScript(pDataXml);
	//const StringValue* pValue = pDataXml->QueryAtt("StartColor");
	return true;
}


////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
AffectorType KAffectorStatic::GetEnumByTypeName( const char * szName )
{
	AffectorType eType = Affector_null;
	if ( strcmp(szName,"layer") == 0 ){
		eType = Affector_layer;
	}else if ( strcmp(szName,"scale") == 0 ){
		eType = Affector_scale;
	}else if ( strcmp(szName,"scaleX") == 0 ){
		eType = Affector_scaleX;
	}else if ( strcmp(szName,"visible") == 0 ){
		eType = Affector_visible;
	}else if ( strcmp(szName,"fadein") == 0 ){
		eType = Affector_fadein;
	}else if ( strcmp(szName,"createSprite") == 0 ){
		eType = Affector_createSprite;
	}else if ( strcmp(szName,"armature") == 0 ){
		eType = Affector_armature;
	}else if ( strcmp(szName,"addwidget") == 0 ){
		eType = Affector_addwidget;
	}else if ( strcmp(szName,"delwidget") == 0 ){
		eType = Affector_delwidget;
	}else if ( strcmp(szName,"fadeout") == 0 ){
		eType = Affector_fadeout;
	}else if ( strcmp(szName,"breathe") == 0 ){
		eType = Affector_breathe;
	}else if ( strcmp(szName,"atkMove") == 0 ){
		eType = Affector_atkMove;
	}else if ( strcmp(szName,"moveback") == 0 ){
		eType = Affector_moveback;
	}else if ( strcmp(szName,"shakex") == 0 ){
		eType = Affector_shakex;
	}else if ( strcmp(szName,"move") == 0 ){
		eType = Affector_move;
	}else if ( strcmp(szName,"eff") == 0 ){
		eType = Affector_eff;
	}else if ( strcmp(szName,"missile") == 0 ){
		eType = Affector_missile;
	}else if ( strcmp(szName,"indicate") == 0 ){
		eType = Affector_indicate;
	}else if ( strcmp(szName,"timerBar") == 0 ){
		eType = Affector_timerBar;
	}else if ( strcmp(szName,"update") == 0 ){
		eType = Affector_update;
	}else if ( strcmp(szName,"updateHit") == 0 ){
		eType = Affector_updateHit;
	}else if ( strcmp(szName,"moveOnHit") == 0 ){
		eType = Affector_moveOnHit;
	}else if ( strcmp(szName,"hit") == 0 ){
		eType = Affector_hit;
	}else if ( strcmp(szName,"summon") == 0 ){
		eType = Affector_summon;
	}else if ( strcmp(szName,"summonSelf") == 0 ){
		eType = Affector_summonSelf;
	}else if ( strcmp(szName,"resortHand") == 0 ){
		eType = Affector_resortHand;
	}else if ( strcmp(szName,"anim") == 0 ){
		eType = Affector_anim;
	}else if ( strcmp(szName,"replace") == 0 ){
		eType = Affector_replace;
	}else if ( strcmp(szName,"action") == 0 ){
		eType = Affector_action;
	}else if ( strcmp(szName,"addWidget") == 0 ){
		eType = Affector_addWidget;
	}else if ( strcmp(szName,"delWidget") == 0 ){
		eType = Affector_delWidget;
	}else if ( strcmp(szName,"updateSecret") == 0 ){
		eType = Affector_updateSecret;
	}else if ( strcmp(szName,"null") == 0 ){
		eType = Affector_null;
	}else if ( strcmp(szName,"drawCard") == 0 ){
		eType = Affector_drawCard;
	}
	return eType;
}

