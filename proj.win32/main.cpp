#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include <System/KPlatform.h>
#include "../Classes/GameRoot.h"

USING_NS_CC;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
	setProcessEncoding(encode_utf8);
    UNREFERENCED_PARAMETER(hPrevInstance);
    //UNREFERENCED_PARAMETER(lpCmdLine);
	char sz[128];
	WideCharToMultiByte( CP_ACP, 0, lpCmdLine, -1,   
        sz, 127, NULL, NULL );   
	GameRoot::getSingleton().ParseCmd(sz);
    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("HelloCpp");
    eglView->setFrameSize(1024, 768);
    return CCApplication::sharedApplication()->run();
}
