//
//  StaticData.h
//  FishingJoy
//
//  Created by Ringo_D on 12-9-25.
//
//

#ifndef __FishingJoy__StaticData__
#define __FishingJoy__StaticData__

#include "cocos2d.h"
#define STATIC_DATA_INIT(user,pwd) StaticData::sharedStaticData()->init(user,pwd)
#define STATIC_DATA_STRING(key) StaticData::sharedStaticData()->stringFromKey(key)
#define STATIC_DATA_INT(key) StaticData::sharedStaticData()->intFromKey(key)
#define STATIC_DATA_FLOAT(key) StaticData::sharedStaticData()->floatFromKey(key)
#define STATIC_DATA_BOOLEAN(key) StaticData::sharedStaticData()->booleanFromKey(key)
#define STATIC_DATA_POINT(key) StaticData::sharedStaticData()->pointFromKey(key)
#define STATIC_DATA_RECT(key) StaticData::sharedStaticData()->rectFromKey(key)
#define STATIC_DATA_SIZE(key) StaticData::sharedStaticData()->sizeFromKey(key)
#define STATIC_DATA_SET(key,val) StaticData::sharedStaticData()->setKeyVal(key,val)
#define STATIC_DATA_INC() StaticData::sharedStaticData()->GetInc()
#define STATIC_DATA_SET_KEYVAL(str) StaticData::sharedStaticData()->setKeyVal(str,1)
class StaticData : public cocos2d::CCObject
{
public:    
    static StaticData* sharedStaticData();

    /**
     *	@brief	 外部访问接口
     *
     *	@param 	key 	对应的static_data.plist中的Key
     *
     *	@return	对应的Value
     */
    const char* stringFromKey(std::string key);
    int intFromKey(std::string key);
    float floatFromKey(std::string key);
    bool booleanFromKey(std::string key);
    cocos2d::CCPoint pointFromKey(std::string key);
    cocos2d::CCRect rectFromKey(std::string key);
    cocos2d::CCSize sizeFromKey(std::string key);
    
	void setKeyVal(const char* keyStr,int val);
    /**
     *	@brief	 内存不足时调用
     */
    void purge();
    
    CC_SYNTHESIZE_READONLY(std::string, _staticDataPath, StaticDataPath);

	int GetInc();
	void createDefault(const char* userName,const char* pwd);
	void init(const char* userName,const char* pwd);
protected:
    cocos2d::CCDictionary* _dictionary;
private:
    StaticData();
    ~StaticData();
    bool init();
	void set(const char* keyStr,const char* val);
	void set(const char* keyStr,int val);
	char m_fullPath[256];
};
#endif /* defined(__FishingJoy__StaticData__) */
