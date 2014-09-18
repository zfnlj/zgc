#ifndef _KPOPUPLAYER_H
#define _KPOPUPLAYER_H

//PopupLayer.h

#include "cocos2d.h"

#include "cocos-ext.h"

USING_NS_CC;

USING_NS_CC_EXT;




class KPopupLayer : public cocos2d::extension::UILayer
{
public:

	enum DialogueType
	{
		DT_Normal = 0,
		DT_Ok,
		DT_Yes_No,
	};
	enum ResultTag
	{
		RT_Ok = 0,
		RT_YES,
		RT_NO,
	};
private:
	KPopupLayer(void);
	~KPopupLayer(void);

	CREATE_FUNC(KPopupLayer);

	virtual bool init();

public:
	static void DoNotifyDlg(int contentId);
	static void DoModal(int titleId,int contentId,DialogueType type=DT_Normal,SEL_PushEvent p=NULL,CCObject* pListener=NULL);
	static void DoModal(const char* szTitle,const char* szContent,DialogueType type=DT_Normal,SEL_PushEvent p=NULL,CCObject* pListener=NULL);
	static void DoModal(int titleId,int contentId,int val,DialogueType type=DT_Normal,SEL_PushEvent p=NULL,CCObject* pListener=NULL);
	static void HideModal();
	virtual void registerWithTouchDispatcher();
	virtual bool ccTouchBegan(CCTouch * touch,CCEvent * pevent);
	static int m_val;
private:
	void updateLayer(const char* szTitle,const char* szContent,DialogueType type,SEL_PushEvent p);
	void btn_Click( cocos2d::CCObject* pSender );
	
private:
	static KPopupLayer* s_pInstance;

	cocos2d::extension::UILabel*    m_pLabCaption;
	cocos2d::extension::UILabel*    m_pLabContent;
	cocos2d::extension::UIButton*   m_pBtn_Ok;
	cocos2d::extension::UIButton*   m_pBtn_Yes;
	cocos2d::extension::UIButton*   m_pBtn_No;
	UIWidget*						m_panel;
	static CCObject*				m_pPushListener;
	
	DialogueType            m_Type;
	SEL_PushEvent                    m_pFunc;
};

#endif // 	
