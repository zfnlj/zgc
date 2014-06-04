
#include "KPopupLayer.h"

USING_NS_CC;




// ���캯���б������ֵ

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

	//��ʼ����Ҫ��Menu
	CCMenu* menu = CCMenu::create();
	menu->setPosition(CCPointZero);
	setMenuButton(menu);  //set()����
	setTouchEnabled(true);  //����������Ӧ
	return true;
}

//��д����ע�ắ�������¸�����������
void PopupLayer::registerWithTouchDispatcher(){
	// ����Ĵ������ȼ�����Ϊ-128����CCMenuͬ������֤�������·��Ĵ���
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

//��������ccTouchBegan������true
bool PopupLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent ){
	return true;
}

//����һ�������㣬���������������ֵ
PopupLayer* PopupLayer::create( const char* backgroundImage ){
	PopupLayer* popup = PopupLayer::create();
	popup->setSpriteBackGround(CCSprite::create(backgroundImage));
	popup->setSprite9BackGround(CCScale9Sprite::create(backgroundImage));
	return popup;
}

//�����������ֵ
void PopupLayer::setTitle( const char* title, int fontsize ){
	CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "Arial", fontsize);
	ltfTitle->setColor(ccc3(0, 0, 0));
	setLabelTitle(ltfTitle);
}

//���ı�������ֵ
void PopupLayer::setContentText( const char* text, int fontsize, int padding, int paddingTop ){
	CCLabelTTF* content = CCLabelTTF::create(text, "Arial", fontsize);
	content->setColor(ccc3(0, 0, 0));
	setLabelContentText(content);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}


//���²������ͻص�����������ֵ
void PopupLayer::setCallBackFunc( CCObject* target, SEL_CallFuncN callfun ){
	m_callbackListener = target;
	m_callback = callfun;
}

//��menu�˵��������Item
bool PopupLayer::addButton( const char* normalImage, const char* selectedImage, const char* title, int tag ){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint center = ccp(winSize.width/2, winSize.height/2);

	// ����ͼƬ�˵���ť
	CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage,
															this, menu_selector(PopupLayer::buttonCallBack));
	menuImage->setTag(tag);
	menuImage->setPosition(center);

	// �������˵��������λ��
	CCSize menuSize = menuImage->getContentSize();
	CCLabelTTF* ttf = CCLabelTTF::create(title, "Arial", 15);
	ttf->setColor(ccc3(0, 0, 0));
	ttf->setPosition(ccp(menuSize.width/2, menuSize.height/2));
	menuImage->addChild(ttf);
	getMenuButton()->addChild(menuImage);
	return true;
}

//���ٵ����򣬴��ݲ���node���²�
void PopupLayer::buttonCallBack( CCObject* pSender ){
	CCNode* node = (CCNode*)pSender;
	CCLog("touch tag: %d", node->getTag());
	if (m_callback && m_callbackListener) (m_callbackListener->*m_callback)(node);
	removeFromParentAndCleanup(true);
}

//ȫ���������趨�ú�������ʱ��̬����
void PopupLayer::onEnter(){
	CCLayer::onEnter();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint center = ccp(winSize.width/2, winSize.height/2);
	CCSize contentSize;
	
	// �趨�ò�����������ʱ����
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

//��Ӱ�ť��������Item�ĸ���������λ��
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

	// ��ʾ�Ի������
	if (getLabelTitle()){
		getLabelTitle()->setPosition(ccpAdd(center, ccp(0, contentSize.height/2 - 25.0f)));
		addChild(getLabelTitle());
	}

	//��ʾ�ı�����
	if (getLabelContentText()){
		CCLabelTTF* ltf = getLabelContentText();
		ltf->setPosition(center);
		ltf->setDimensions(CCSizeMake(contentSize.width - m_contentPadding*2, contentSize.height - m_contentPaddingTop));
		ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
		addChild(ltf);
	}

	//����Ч��
	CCSequence *popupActions = CCSequence::create(
	CCScaleTo::create(0.0f, 0.0f), 
	CCScaleTo::create(0.06f, 1.05f),
	CCScaleTo::create(0.08f, 0.95f),
	CCScaleTo::create(0.08f, 1.0f), NULL);
	runAction(popupActions);
}

//�˳�
void PopupLayer::onExit(){
	CCLayer::onExit();
}

/*
void test()
{
	PopupLayer* popup = PopupLayer::create("popupBackGround.png");

	// ContentSize�ǿ�ѡ�����ã����Բ����ã�����������������9ͼ����
	//popup->setContentSize(CCSizeMake(400, 360));
	popup->setTitle("Message");
	popup->setContentText("Most people... blunder round this city.", 20, 50, 150);
	
	// ���ûص��������ص�����һ��CCNode�Ի�ȡtag�жϵ���İ�ť
	// ��ֻ����Ϊһ�ַ�װʵ�֣����ʹ��delegate�Ǿ��ܹ������Ŀ��Ʋ�����
	popup->setCallBackFunc(this, callfuncN_selector(HelloWorld::buttonCallBack));
	//��Ӱ�ť������ͼƬ�����֣�tag��Ϣ
	popup->addButton("button.png", "button.png", "Ok", 0);
	popup->addButton("button.png", "button.png", "Cancel", 1);
	this->addChild(popup);
}
*/

