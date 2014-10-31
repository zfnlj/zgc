//
//  StaticData.cpp
//  FishingJoy
//
//  Created by Ringo_D on 12-9-25.
//
//

#include "StaticData.h"
USING_NS_CC;
using namespace std;

#define STATIC_DATA_NAME "static_data_1_1.plist"

static StaticData* _sharedStaticData = NULL;
StaticData* StaticData::sharedStaticData()
{
    if(_sharedStaticData == NULL){
        _sharedStaticData = new StaticData();
        _sharedStaticData->init();
    }
    return _sharedStaticData;
}
StaticData::StaticData()
{
    _staticDataPath = STATIC_DATA_NAME;
}
StaticData::~StaticData()
{
    CC_SAFE_RELEASE_NULL(_dictionary);
}

void StaticData::init(const char* userName,const char* pwd)
{
	std::string writablePath = CCFileUtils::sharedFileUtils()->getWritablePath();
	sprintf(m_fullPath,"%s%s_%s",writablePath.c_str(),userName,STATIC_DATA_NAME);

	if(!CCFileUtils::sharedFileUtils()->isFileExist(m_fullPath)){
		_dictionary = CCDictionary::create();
		set("user",userName);
		set("pwd",pwd);
		set("player",userName);
		set("recInc",1);
		set("Inc",1);
		_dictionary->writeToFile(m_fullPath);
	}else{
		_dictionary = CCDictionary::createWithContentsOfFile(m_fullPath);
	}
    CC_SAFE_RETAIN(_dictionary);
}

void StaticData::createDefault(const char* userName,const char* pwd)
{
	_dictionary = CCDictionary::create();
}

bool StaticData::init()
{
    return true;
}

void StaticData::set(const char* keyStr,int val)
{
	char buf[24];
	sprintf(buf,"%d",val);
	set(keyStr,buf);
}

void StaticData::set(const char* keyStr,const char* val)
{
	CCString* pValue1 = CCString::create(val);
	_dictionary->setObject(pValue1,keyStr);
}

const char* StaticData::stringFromKey(string key)
{
    return _dictionary->valueForKey(key)->getCString();
}
int StaticData::intFromKey(string key)
{
    return _dictionary->valueForKey(key)->intValue();
}
float StaticData::floatFromKey(string key)
{
    return _dictionary->valueForKey(key)->floatValue();
}
bool StaticData::booleanFromKey(string key)
{
    return _dictionary->valueForKey(key)->boolValue();
}
cocos2d::CCPoint StaticData::pointFromKey(string key)
{
    return CCPointFromString(_dictionary->valueForKey(key)->getCString());
}
cocos2d::CCRect StaticData::rectFromKey(string key)
{
    return CCRectFromString(_dictionary->valueForKey(key)->getCString());
}
cocos2d::CCSize StaticData::sizeFromKey(string key)
{
    return CCSizeFromString(_dictionary->valueForKey(key)->getCString());
}
void StaticData::purge()
{
    CC_SAFE_RELEASE_NULL(_sharedStaticData);
}

void StaticData::setKeyVal(const char* keyStr,int val)
{
	std::string key = keyStr;
	char buf[24];
	sprintf(buf,"%d",val);

	CCString* pValue1 = CCString::create(buf);

	_dictionary->setObject(pValue1,keyStr);
	//pStr->initWithFormat("%d",val);//
	_dictionary->writeToFile(m_fullPath);
}

int StaticData::GetInc()
{
	int val = intFromKey("Inc");
	setKeyVal("Inc",val+1);
	return val;
}
