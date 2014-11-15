
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

void IOSFunc::ShowLeaderboard()
{
    UIViewController* viewCtrl = [UIApplication sharedApplication].keyWindow.rootViewController;
    [[GameCenterManager sharedManager] presentLeaderboardsOnViewController:viewCtrl];
}

void IOSFunc::ReportScore(const char* str,int val)
{
    
    NSString* nsStr = [NSString stringWithUTF8String:str];
     [[GameCenterManager sharedManager] saveAndReportScore:val leaderboard:nsStr sortOrder:GameCenterSortOrderHighToLow];
}

void IOSFunc::ReportAchievement(const char* str,double percent,bool bNotify)
{
    NSString* nsStr = [NSString stringWithUTF8String:str];
    [[GameCenterManager sharedManager] saveAndReportAchievement:nsStr percentComplete:percent shouldDisplayNotification:bNotify];
}