#ifndef _KRECORDDATA_H
#define _KRECORDDATA_H
#include <System/Misc/StreamInterface.h>
#include <list>

enum EGameRecordedDataType
{
	EGRDT_UIMouseInput	= 0,
	EGRDT_PlayOp		= 1,
	EGRDT_End,
};

class KRecordDataBase
{
public:
	KRecordDataBase(){}
	~KRecordDataBase(){}
	virtual bool Deserialize( StreamInterface* pDataStream );
	virtual bool Serialize( StreamInterface* pDataStream );
	virtual void init()=0;
	virtual EGameRecordedDataType GetClassType()=0;
private:
	unsigned int m_dlgId;
};

class KRecordUIMouseData :public KRecordDataBase
{
public:
	KRecordUIMouseData(){}
	~KRecordUIMouseData(){}
	virtual bool Deserialize( StreamInterface* pDataStream );
	virtual bool Serialize( StreamInterface* pDataStream );
	virtual void init(){}
	virtual EGameRecordedDataType GetClassType(){ return EGRDT_UIMouseInput;}
};

class KRecordPlayOpData :public KRecordDataBase
{
public:
	struct OpStruct{
		int _src;
		int _des;
		int _slot;
	};
	KRecordPlayOpData(){}
	~KRecordPlayOpData(){}
	virtual bool Deserialize( StreamInterface* pDataStream );
	virtual bool Serialize( StreamInterface* pDataStream );
	virtual void init(){}
	virtual EGameRecordedDataType GetClassType(){ return EGRDT_PlayOp;}
private:
	OpStruct m_data;
};

typedef std::list<KRecordDataBase*> KRecordDataList;
#endif // __HELLOWORLD_SCENE_H__
