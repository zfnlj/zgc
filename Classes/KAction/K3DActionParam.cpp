#include "K3DActionParam.h"
#include "../GameLogic/KGameDef.h"
#include "../StaticTable/KAbilityStatic.h"

K3DActionParam::K3DActionParam()
{
	
}

void K3DActionParam::init()
{
	memset(this,0,sizeof(K3DActionParam));
}

void K3DActionParam::init(K3DActionParam* pParam)
{
	init();
	if(pParam) memcpy(this,pParam,sizeof(K3DActionParam));
}
void K3DActionParam::clone(K3DActionParam* pParam)
{
	memcpy(this,pParam,sizeof(K3DActionParam));
}


K3DActionParam::~K3DActionParam()
{
}

bool K3DActionParam::IsEmpty()
{
	return (strlen(_name)==0);

}

void K3DActionParam::SetAction(const char* name)
{
	strcpy(_name,name);
}
void K3DActionParam::init(const char* name)
{
	init();
	SetAction(name);
}

void K3DActionParam::SetSrcVal(int src,int val)
{
	_srcID = src;
	_srcVal = val;
}

int K3DActionParam::GetVal(int id)
{
	if(_srcID==id){
		return _srcVal;
	}else{
		for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
			if(_desArr[i]==id){
				return _desValArr[i];
			}
		}
	}
	return -9999;
}

void K3DActionParam::SetDestVal(int dest,int val)
{
	for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
		if(_desArr[i]==0){
			_desArr[i] = dest;
			_desValArr[i] = val;
			break;
		}
	}
}
int K3DActionParam::GetDesId(int index)
{
	return _desArr[index];
}

void K3DActionParam::Copy(strCardAbilityResult* result)
{
	init(result->_pAbility->GetAction());
	_srcID = result->_src;
	for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
		if(result->_destArr[i]==0) break;
		SetDestVal(result->_destArr[i],result->_destValArr[i]);
	}
}