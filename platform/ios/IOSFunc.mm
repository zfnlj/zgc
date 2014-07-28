
#import "../IOSFunc.h"
#import "UMFeedback.h"

void IOSFunc::OpenFeedback()
{
    UIViewController* viewCtrl = [UIApplication sharedApplication].keyWindow.rootViewController;
     [UMFeedback showFeedback:viewCtrl withAppkey:UMENG_APPKEY];
}