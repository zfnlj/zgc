//////////////////////////////////////////////////////////////////////////////
//
// FileName	:	KAction.h
// Creator	:	zhufanan
// Date		:	2008-4-28 13:58:32
// Comment	:	KActionParam类
//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cocos2d.h"
#include "../GameLogic/KGameDef.h"
#define MAX_ACTION_NAME_LEN 63

USING_NS_CC;

struct strCardAbilityResult;
struct K3DActionParam 
{
	char _name[MAX_ACTION_NAME_LEN+1];	// 动作名称
	int _bulletID;
	int _srcID;
	int _srcVal;
	int _desArr[MAX_ACTION_TARGET_NUM];
	int _desValArr[MAX_ACTION_TARGET_NUM];
	void*         _desPtr;
//	float _destPosX;
//	float _destPosY;
	float _paramFloat;
	int  _paramInt;
	//union
	//{
	//	struct{
	//		int _p1;
	//		int _p2;
	//	};
	//	struct
	//	{
	//		float _fp1;
	//		float _fp2;
	//	};							// 目标位置信息
	//};

	K3DActionParam();
	void init();
	int GetVal(int id);
	int GetDesId(int index);
	void SetSrcVal(int src,int val);
	void SetDestVal(int target,int val);
	void init(K3DActionParam*);
	void init(const char* name);
	void SetAction(const char* name);
	~K3DActionParam();
	void clone(K3DActionParam*);
	void Copy(strCardAbilityResult*);
	int BulletID(){ return _bulletID;}

	bool IsEmpty();

	int SrcID(){ return _srcID;}
};

