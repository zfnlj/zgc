#ifndef _KRECORDDATA_H
#define _KRECORDDATA_H
#include <System/Misc/StreamInterface.h>
#include <list>
#include "cocos2d.h"

enum EGameRecordedDataType
{
	EGRDT_UIMouseInput	= 0,
	EGRDT_PlayOp		= 1,
	EGRDT_End,
};

class KCardInst;
class KRecordDataBase
{
public:
	KRecordDataBase():m_dlgId(0){}
	~KRecordDataBase(){}
	virtual bool Deserialize( StreamInterface* pDataStream );
	virtual bool Serialize( StreamInterface* pDataStream );
	virtual void init(){
		m_dlgId=0;
	}
	virtual bool Replay(int op)=0;
	virtual EGameRecordedDataType GetClassType()=0;
	virtual bool IsClickCardValidate(KCardInst* card)=0;
	virtual bool IsClickFightAreaValidate(int slot)=0;
	virtual bool IsClickButValidate(cocos2d::CCObject* obj)=0;
private:
	unsigned int m_dlgId;
};

class KRecordUIMouseData :public KRecordDataBase
{
public:
	enum Mouse_evt{
		evt_turn_end,
		evt_null,
	};
	KRecordUIMouseData(){}
	~KRecordUIMouseData(){}
	virtual bool Deserialize( StreamInterface* pDataStream );
	virtual bool Serialize( StreamInterface* pDataStream );
	virtual void init(){m_evt = evt_null;}
	void RecordMouseEvt(Mouse_evt evt);
	virtual bool Replay(int op);
	virtual EGameRecordedDataType GetClassType(){ return EGRDT_UIMouseInput;}
	virtual bool IsClickCardValidate(KCardInst* card){ return false;}
	virtual bool IsClickFightAreaValidate(int slot){ return false;}
	virtual bool IsClickButValidate(cocos2d::CCObject* obj);
private:
	Mouse_evt m_evt;
};

class KRecordPlayOpData :public KRecordDataBase
{
public:
	struct OpStruct{
		int _src;
		int _des;
		int _slot;
		void set(int src,int des,int slot){
			_src = src;
			_des = des;
			_slot =slot;
		}
	};
	KRecordPlayOpData(){}
	~KRecordPlayOpData(){}
	virtual bool Deserialize( StreamInterface* pDataStream );
	virtual bool Serialize( StreamInterface* pDataStream );
	virtual void init(){
		memset(&m_data,0,sizeof(m_data));
		KRecordDataBase::init();
		m_pActiveCard = NULL;
	}
	void RecordPlayOp(int src,int des,int slot);
	virtual bool Replay(int op);
	virtual EGameRecordedDataType GetClassType(){ return EGRDT_PlayOp;}
	virtual bool IsClickCardValidate(KCardInst* card);
	virtual bool IsClickFightAreaValidate(int slot);
	virtual bool IsClickButValidate(cocos2d::CCObject* obj){ return false;}
private:
	void ActiveCard(int);
	OpStruct m_data;
	KCardInst* m_pActiveCard;
};

typedef std::list<KRecordDataBase*> KRecordDataList;
#endif // __HELLOWORLD_SCENE_H__
