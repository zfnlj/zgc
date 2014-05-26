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

#define STATIC_DATA_PATH "static_data.plist"

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
    _staticDataPath = STATIC_DATA_PATH;
}
StaticData::~StaticData()
{
    CC_SAFE_RELEASE_NULL(_dictionary);
}
bool StaticData::init()
{
	std::string writablePath = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
	sprintf(m_fullPath,"%s\\%s",writablePath.c_str(),STATIC_DATA_PATH);

    _dictionary = CCDictionary::createWithContentsOfFile(m_fullPath);
    CC_SAFE_RETAIN(_dictionary);
    return true;
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