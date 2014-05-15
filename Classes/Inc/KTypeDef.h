#pragma once

typedef unsigned char	uchar;
typedef unsigned short  ushort;

#define IMPLEMENT_SINGLETON(__classname ) \
	template<> __classname* Singleton<__classname>::mSingleton = 0; \
	__classname& __classname::getSingleton(void) \
{ \
	if(NULL == mSingleton)\
	{\
	mSingleton = new __classname;\
	mSingleton->init();\
	}\
	return (*mSingleton);\
}

#define IMPLEMENT_SIMPLE_SINGLETON(__classname ) \
	template<> __classname* Singleton<__classname>::mSingleton = 0; \
	__classname& __classname::getSingleton(void) \
{ \
	if(NULL == mSingleton)\
{\
	mSingleton = new __classname;\
}\
	return (*mSingleton);\
}

#ifndef _USE_COCOS2DX
inline void CCLog(const char * pszFormat, ...)
{
	char szBuf[128];

	va_list ap;
	va_start(ap, pszFormat);
	vsnprintf_s(szBuf, 128, 128, pszFormat, ap);
	va_end(ap);

	WCHAR wszBuf[128] = {0};
	MultiByteToWideChar(CP_UTF8, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
	OutputDebugStringW(wszBuf);
	OutputDebugStringA("\n");

	WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
	printf("%s\n", szBuf);
}

#define CCAssert(cond, msg)  
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#else
#include "ccMacros.h"
USING_NS_CC;
#endif