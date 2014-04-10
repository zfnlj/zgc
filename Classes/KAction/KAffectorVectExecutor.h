/********************************************************************
	created:	
	created:	
	filename: 	
	file path:	
	file base:	
	file ext:	
	author:		
	
	purpose:	
*********************************************************************/
#pragma once

#include "ParallelExecutorContainer.h"
#include "KAffectorAdvExecutor.h"
#include "K3DActionParam.h"
#include "KActionStaticMgr.h"

class KAction;
class KActionStatic;

class KAffectorVectExecutor :public ParallelExecutorContainer
{
protected:
	KAction*  m_pAction;
public:
	KAffectorVectExecutor();
	virtual ~KAffectorVectExecutor(){}

	void Init(KAction*);
	virtual void OnPlayExecutor(AdvExecutor* ); 
	virtual void OnStopExecutor(AdvExecutor* ); //
	KAffectorExecutor* GetExecutor(AffectorType);
protected:
	KAffectorExecutor* CreateExecFactory(KAffectorStatic* st);
	friend KAction;
};