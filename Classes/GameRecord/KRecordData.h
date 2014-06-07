#ifndef _KRECORDDATA_H
#define _KRECORDDATA_H
#include <System/Misc/StreamInterface.h>
#include <list>
#include "cocos2d.h"

#define REC_WIDGET_NAME_LEN 32
enum EGameRecordedDataType
{
	EGRDT_UIMouseInput	= 0,
	EGRDT_PlayOp		= 1,
	EGRDT_UIClickWidget = 2,
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
	virtual void init(int id){
		m_dlgId =id;
		m_elapsed = 0;
	}
	virtual bool Replay(DWORD timeline,int op)=0;
	virtual EGameRecordedDataType GetClassType()=0;
	virtual bool IsClickCardValidate(DWORD,KCardInst* card){return false;}
	virtual bool IsClickFightAreaValidate(DWORD,int slot){return false;}
	virtual bool IsClickWidgetValidate(cocos2d::CCObject* layer,cocos2d::CCPoint& pt,DWORD){return false;}
	virtual bool IsClickButValidate(DWORD,cocos2d::CCObject* obj){return false;}
	virtual bool IsClickWidgetValidate(cocos2d::CCPoint& pt){return false;}
	DWORD GetElapsed(){ return m_elapsed;}

protected:
	unsigned int m_dlgId;
	DWORD		 m_elapsed;
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
	virtual void init(int id){
		KRecordDataBase::init(id);
		m_evt = evt_null;
	}
	void RecordMouseEvt(Mouse_evt evt,DWORD initTime);
	virtual bool Replay(DWORD timeline,int op);
	virtual EGameRecordedDataType GetClassType(){ return EGRDT_UIMouseInput;}
	virtual bool IsClickButValidate(DWORD timeline,cocos2d::CCObject* obj);
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
	virtual void init(int id){
		KRecordDataBase::init(id);
		memset(&m_data,0,sizeof(m_data));
		m_pActiveCard = NULL;
	}
	void RecordPlayOp(int src,int des,int slot,DWORD initTime);
	virtual bool Replay(DWORD timeline, int op);
	virtual EGameRecordedDataType GetClassType(){ return EGRDT_PlayOp;}
	virtual bool IsClickCardValidate(DWORD,KCardInst* card);
	virtual bool IsClickFightAreaValidate(DWORD,int slot);
private:
	void ActiveCard(int);
	OpStruct m_data;
	KCardInst* m_pActiveCard;
};

class KRecordClickWidget :public KRecordDataBase
{
public:
	KRecordClickWidget(){}
	~KRecordClickWidget(){}
	virtual bool Deserialize( StreamInterface* pDataStream );
	virtual bool Serialize( StreamInterface* pDataStream );
	virtual void init(int id){
		KRecordDataBase::init(id);
		memset(m_widgetName,0,sizeof(m_widgetName));
		m_index = -1;
	}
	void RecordMouseEvt(const char* widgetName,int index,DWORD initTime);
	virtual bool Replay(DWORD timeline,int op);
	virtual EGameRecordedDataType GetClassType(){ return EGRDT_UIClickWidget;}
	virtual bool IsClickWidgetValidate(cocos2d::CCObject* layer,cocos2d::CCPoint& pt,DWORD);
private:
	char m_widgetName[REC_WIDGET_NAME_LEN];
	int m_index;
};

typedef std::list<KRecordDataBase*> KRecordDataList;
#endif // __HELLOWORLD_SCENE_H__
