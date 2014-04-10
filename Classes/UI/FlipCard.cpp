<SPAN style="FONT-SIZE: 14px">//
//  CardSprite.cpp
//  Cocos2dev.com
//
//  Created by LiuYanghui on 13-6-17.
//
//

#include "CardSprite.h"

#define kInAngleZ        270 //���濨�Ƶ���ʼZ��Ƕ�
#define kInDeltaZ        90  //���濨����ת��Z��ǶȲ�

#define kOutAngleZ       0   //���濨�Ƶ���ʼZ��Ƕ�
#define kOutDeltaZ       90  //���濨����ת��Z��ǶȲ�

enum {
    tag_inCard = 1,
    tag_outCard
};

CardSprite::CardSprite()
{
    
}

CardSprite::~CardSprite()
{
    m_openAnimIn->release();
    m_openAnimOut->release();
}

CardSprite* CardSprite::create(const char* inCardImageName, const char* outCardImageName, float duration)
{
    CardSprite *pSprite = new CardSprite();
    if (pSprite && pSprite->init(inCardImageName, outCardImageName, duration))
    {
        pSprite->autorelease();
        return pSprite;
    }
    CC_SAFE_DELETE(pSprite);
    return NULL;
}

bool CardSprite::init(const char* inCardImageName, const char* outCardImageName, float duration)
{
    if (!CCSprite::init())
    {
        return false;
    }
    initData(inCardImageName, outCardImageName, duration);
    return true;
}

#pragma mark - initData
void CardSprite::initData(const char* inCardImageName, const char* outCardImageName, float duration)
{
    m_isOpened = false;
    
    CCSprite* inCard = CCSprite::create(inCardImageName);
    inCard->setPosition(CCPointZero);
    inCard->setVisible(false);
    inCard->setTag(tag_inCard);
    addChild(inCard);
    
    CCSprite* outCard = CCSprite::create(outCardImageName);
    outCard->setPosition(CCPointZero);
    outCard->setTag(tag_outCard);
    addChild(outCard);
    
    m_openAnimIn = (CCActionInterval*)CCSequence::create(CCDelayTime::create(duration * .5),
                                                         CCShow::create(),
                                                         CCOrbitCamera::create(duration * .5, 1, 0, kInAngleZ, kInDeltaZ, 0, 0),
                                                         NULL);
    m_openAnimIn->retain();
    
    m_openAnimOut = (CCActionInterval *)CCSequence::create(CCOrbitCamera::create(duration * .5, 1, 0, kOutAngleZ, kOutDeltaZ, 0, 0),
                                                           CCHide::create(),
                                                           CCDelayTime::create(duration * .5),
                                                           NULL);
    m_openAnimOut->retain();
}

#pragma mark - public func
void CardSprite::openCard()
{
    CCSprite* inCard = (CCSprite*)getChildByTag(tag_inCard);
    CCSprite* outCard = (CCSprite*)getChildByTag(tag_outCard);
    inCard->runAction(m_openAnimIn);
    outCard->runAction(m_openAnimOut);
}

