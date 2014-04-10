
#ifndef __Test__CardSprite__
#define __Test__CardSprite__

#include "cocos2d.h"
USING_NS_CC;

class CardSprite : public CCSprite
{
public:
    CardSprite();
    ~CardSprite();
    static CardSprite* create(const char* inCardImageName, const char* outCardImageName, float duration);
    virtual bool init(const char* inCardImageName, const char* outCardImageName, float duration);
    
private:
    bool m_isOpened;
    CCActionInterval* m_openAnimIn;
    CCActionInterval* m_openAnimOut;
    
    
    void initData(const char* inCardImageName, const char* outCardImageName, float duration);
    
public:
    void openCard();
};

#endif /* defined(__Test__CardSprite__) */
