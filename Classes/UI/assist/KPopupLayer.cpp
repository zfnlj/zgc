
#include "KPopupLayer.h"

USING_NS_CC;




// 构造函数中变量设初值

PopupLayer::PopupLayer()
{
	m_contentPadding = 0;
	m_contentPaddingTop = 0;
	m_pMenu = NULL;
	m_callbackListener = NULL;
	m_callback = NULL;
	m_sfBackGround = NULL;
	m_s9BackGround = NULL;
	m_ltContentText = NULL;
	m_ltTitle = NULL;
}

PopupLayer::~PopupLayer()
{
	CC_SAFE_RELEASE(m_pMenu);
	CC_SAFE_RELEASE(m_sfBackGround);
	CC_SAFE_RELEASE(m_s9BackGround);
	CC_SAFE_RELEASE(m_ltContentText);
	CC_SAFE_RELEASE(m_ltTitle);
}

bool PopupLayer::init()
{
	if ( !CCLayer::init() ){
		return false;
	}

	this->setContentSize(CCSizeZero);

	//初始化需要的Menu
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	setMenuButton(menu);  //set()方法
	setTouchEnabled(true);  //开启触摸响应
	return true;
}

//重写触摸注册函数，重新给定触摸级别
void PopupLayer::registerWithTouchDispatcher(){
	// 这里的触摸优先级设置为-128，与CCMenu同级，保证了屏蔽下方的触摸
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

//触摸函数ccTouchBegan，返回true
bool PopupLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent ){
	return true;
}

//创建一个弹出层，给背景精灵变量赋值
PopupLayer* PopupLayer::create( const char* backgroundImage ){
	PopupLayer* popup = PopupLayer::create();
	popup->setSpriteBackGround(CCSprite::create(backgroundImage));
	popup->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	return popup;
}

//给标题变量赋值
void PopupLayer::setTitle( const char* title, int fontsize ){
	CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "Arial", fontsize);
	ltfTitle->setColor(ccc3(0, 0, 0));
	setLabelTitle(ltfTitle);
}

//给文本变量赋值
void PopupLayer::setContentText( const char* text, int fontsize, int padding, int paddingTop ){
	CCLabelTTF* content = CCLabelTTF::create(text, "Arial", fontsize);
	content->setColor(ccc3(0, 0, 0));
	setLabelContentText(content);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}


//给下层层变量和回调函数变量赋值
void PopupLayer::setCallBackFunc( CCObject* target, SEL_CallFuncN callfun ){
	m_callbackListener = target;
	m_callback = callfun;
}

//给menu菜单变量添加Item
bool PopupLayer::addButton( const char* normalImage, const char* selectedImage, const char* title, int tag ){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint center = ccp(winSize.width/2, winSize.height/2);

	// 创建图片菜单按钮
	CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage,
															this, menu_selector(PopupLayer::buttonCallBack));
	menuImage->setTag(tag);
	menuImage->setPosition(center);

	// 添加文字说明并设置位置
	CCSize menuSize = menuImage->getContentSize();
	CCLabelTTF* ttf = CCLabelTTF::create(title, "Arial", 15);
	ttf->setColor(ccc3(0, 0, 0));
	ttf->setPosition(ccp(menuSize.width/2, menuSize.height/2));
	menuImage->addChild(ttf);
	getMenuButton()->addChild(menuImage);
	return true;
}

//销毁弹出框，传递参数node给下层
void PopupLayer::buttonCallBack( CCObject* pSender ){
	CCNode* node = (CCNode*)pSender;
	CCLog("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener) (m_callbackListener->*m_callback)(node);
	removeFromParentAndCleanup(true);
}

//全部参数都设定好后，在运行时动态加载
void PopupLayer::onEnter(){
	CCLayer::onEnter();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint center = ccp(winSize.width/2, winSize.height/2);
	CCSize contentSize;
	
	// 设定好参数，在运行时加载
	if (getContentSize().equals(CCSizeZero)){
		getSpriteBackGround()->setPosition(center);
		addChild(getSpriteBackGround(),0,0);
		contentSize = getSpriteBackGround()->getTexture()->getContentSize();
	}else{
		CCScale9Sprite* background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(center);
		addChild(background, 0);
		contentSize = getContentSize();
	}

//添加按钮，并根据Item的个数设置其位置
	addChild(getMenuButton());
	float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount()+1);
	CCArray* array = getMenuButton()->getChildren();
	CCObject* pObj = NULL;
	
	int i = 0;
	CCARRAY_FOREACH(array, pObj){
		CCNode* node = (CCNode*)pObj;
		node->setPosition(ccp(winSize.width/2 - contentSize.width/2 + btnWidth*(i+1), 
                               winSize.height/2 - contentSize.height/3));
		i++;
	}

	// 显示对话框标题
	if (getLabelTitle()){
		getLabelTitle()->setPosition(ccpAdd(center, ccp(0, contentSize.height/2 - 25.0f)));
		addChild(getLabelTitle());
	}

	//显示文本内容
	if (getLabelContentText()){
		CCLabelTTF* ltf = getLabelContentText();
		ltf->setPosition(center);
		ltf->setDimensions(CCSizeMake(contentSize.width - m_contentPadding*2, contentSize.height - m_contentPaddingTop));
		ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
		addChild(ltf);
	}

	//弹出效果
	CCSequence *popupActions = CCSequence::create(
	CCScaleTo::create(0.0f, 0.0f), 
	CCScaleTo::create(0.06f, 1.05f),
	CCScaleTo::create(0.08f, 0.95f),
	CCScaleTo::create(0.08f, 1.0f), NULL);
	runAction(popupActions);
}

//退出
void PopupLayer::onExit(){
	CCLayer::onExit();
}

/*
void test()
{
	PopupLayer* popup = PopupLayer::create("popupBackGround.png");

	// ContentSize是可选的设置，可以不设置，如果设置则把它当做9图缩放
	//popup->setContentSize(CCSizeMake(400, 360));
	popup->setTitle("Message");
	popup->setContentText("Most people... blunder round this city.", 20, 50, 150);
	
	// 设置回调函数，回调传回一个CCNode以获取tag判断点击的按钮
	// 这只是作为一种封装实现，如果使用delegate那就能够更灵活的控制参数了
	popup->setCallBackFunc(this, callfuncN_selector(HelloWorld::buttonCallBack));
	//添加按钮，设置图片、文字，tag信息
	popup->addButton("button.png", "button.png", "Ok", 0);
	popup->addButton("button.png", "button.png", "Cancel", 1);
	this->addChild(popup);
}
*/

