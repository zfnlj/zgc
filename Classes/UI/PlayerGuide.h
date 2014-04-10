#ifndef Good_PlayGuide_h 
#define Good_PlayGuide_h 

#include "cocos2d.h" 


using namespace cocos2d; 


class PlayerGuide:public CCSprite , public CCTargetedTouchDelegate 
{ 
public: 
    PlayerGuide(); 

    virtual ~PlayerGuide(); 


    virtual bool init(); 

    virtual void onEnter(); 

    virtual void onExit(); 


    virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event); 

    virtual void ccTouchMoved(CCTouch *touch, CCEvent *event); 

    virtual void ccTouchEnded(CCTouch *touch, CCEvent *event); 


    void onSetSpriteAndPosition(float width,float height,CCPoint point); 

    CCLayerColor *m_layer; // color layer 

    float m_layerWidth; //layer width  

    float m_layerHeight; // layer height  

    CCRenderTexture *m_pTarget; // render texture  

}; 

#endif 

