
#import "../IOSFunc.h"
#import "UMFeedback.h"
#import "GameCenterManager.h"

void IOSFunc::OpenFeedback()
{
    UIViewController* viewCtrl = [UIApplication sharedApplication].keyWindow.rootViewController;
     [UMFeedback showFeedback:viewCtrl withAppkey:UMENG_APPKEY];
}

void IOSFunc::OpenUrl(const char* pszUrl)
{
    NSString* url = [NSString stringWithUTF8String:pszUrl];
     [[UIApplication sharedApplication] openURL:[NSURL URLWithString: url]];
}

void IOSFunc::CheckGCAvailability()
{
    [[GameCenterManager sharedManager] checkGameCenterAvailability];
}