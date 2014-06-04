#ifndef _KPOPUPLAYER_H
#define _KPOPUPLAYER_H

//PopupLayer.h

#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;

USING_NS_CC_EXT;




class PopupLayer : public CCLayer{

public:

	PopupLayer();
	~PopupLayer();

	virtual bool init();

	//��Ҫ��д����ע�ắ�������¸�����������
	virtual void registerWithTouchDispatcher();

	//��д��������������true�����������㣬�ﵽ��ģ̬��Ч��
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);



	//��̬��������������һ�������㣬���ñ���ͼƬ
	static PopupLayer* create(const char* backgroundImage);

	void setTitle(const char* title, int fontsize = 20);

    //�����ı����ݣ�padding Ϊ���ֵ��Ի�������Ԥ���ľ��룬���ǿɿصģ����Ϸ��ľ����������
	void setContentText(const char* text, int fontsize=20, int padding=50, int paddingTop=100);

	//�����ϲ������ϲ�ص����������ڻص�ʱ����CCNode����
	void setCallBackFunc(CCObject* target, SEL_CallFuncN callfun);

	//���menuItem��ť����װ��һ������������Щ��Ҫ�Ĳ���
	bool addButton(const char* normalImage, const char* selectedImage, const char* title, int tag=0);

	//Ϊ������ʾ��ʱ��������Ч��ѡ����onEnter�ﶯ̬����
	virtual void onEnter();

	virtual void onExit();

	CREATE_FUNC(PopupLayer);
private:
	void buttonCallBack(CCObject* pSender);
	//�����������ߵĿհ�����
	int m_contentPadding;
	int m_contentPaddingTop;
	CCObject* m_callbackListener;
	SEL_CallFuncN m_callback;
	
	//������CCMenu*���ͱ���m_pMenu, ����ֱ�Ӷ���Ĭ�ϵ�set/get����
	CC_SYNTHESIZE_RETAIN(CCMenu*, m_pMenu, MenuButton);
	CC_SYNTHESIZE_RETAIN(CCSprite*, m_sfBackGround, SpriteBackGround);
	CC_SYNTHESIZE_RETAIN(CCScale9Sprite*, m_s9BackGround, Sprite9BackGround);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_ltTitle, LabelTitle);
	CC_SYNTHESIZE_RETAIN(CCLabelTTF*, m_ltContentText, LabelContentText);
};

#endif // 	
