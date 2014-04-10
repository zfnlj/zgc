#include "KGameDef.h"
#include "cocos2d.h"


strCardAbilityResult::strCardAbilityResult()
{
	memset(this,0,sizeof(strCardAbilityResult));
}

void strCardAbilityResult::init(int src,KAbilityStatic* ability)
{
	_src = src;
	_pAbility = ability;
}

void strCardAbilityResult::SetDestVal(int dest,int val)
{
	for(int i=0;i<MAX_ACTION_TARGET_NUM;i++){
		if(_destArr[i]==0){
			_destArr[i] = dest;
			_destValArr[i] = val;
			break;
		}
	}
}