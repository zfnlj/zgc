#include "IOSFunc_Bridge.h"
#include "cocos-ext.h"

USING_NS_CC;



IOSFunc_Bridge::IOSFunc_Bridge()
{
    m_ios = new IOSFunc();
}

IOSFunc_Bridge::~IOSFunc_Bridge()
{
    delete m_ios;
}


void IOSFunc_Bridge::OpenFeedback()
{
	m_ios->OpenFeedback();
}

void IOSFunc_Bridge::OpenUrl(const char* pszUrl)
{
	m_ios->OpenUrl(pszUrl);

	//[[UIApplication sharedApplication]openURL:[NSURL URLWithString:@"http://www.baidu.com"]];
}

