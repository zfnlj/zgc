//
//  ViewController.m
//  Demo
//
//  Created by iOS@Umeng on 9/27/12.
//  Copyright (c) 2012 iOS@Umeng. All rights reserved.
//

#import "ViewController.h"
#import "UMFeedbackViewController.h"


@interface ViewController ()

@end

@implementation UINavigationBar (CustomImage)
- (void)drawRect:(CGRect)rect {
  UIImage *image = [UIImage imageNamed:@"nav_bar_bg@2x.png"];
  [image drawInRect:CGRectMake(0, 0, self.frame.size.width, self.frame.size.height)];
}
@end

@implementation ViewController
@synthesize umFeedback = _umFeedback;

- (void)viewDidLoad {
  [super viewDidLoad];
    [UMFeedback setLogEnabled:YES];
  _umFeedback = [UMFeedback sharedInstance];
  [_umFeedback setAppkey:UMENG_APPKEY delegate:self];

  // Do any additional setup after loading the view, typically from a nib.
}

- (void)viewDidUnload {
  [super viewDidUnload];
  // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
  if ([[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone) {
    return (interfaceOrientation != UIInterfaceOrientationPortraitUpsideDown);
  } else {
    return YES;
  }
}

- (IBAction)nativeFeedback:(id)sender {
    [self showNativeFeedbackWithAppkey:UMENG_APPKEY];
}

- (void)showNativeFeedbackWithAppkey:(NSString *)appkey {
    UMFeedbackViewController *feedbackViewController = [[UMFeedbackViewController alloc] initWithNibName:@"UMFeedbackViewController" bundle:nil];
    feedbackViewController.appkey = appkey;
    UINavigationController *navigationController = [[UINavigationController alloc] initWithRootViewController:feedbackViewController];
    navigationController.navigationBar.barStyle = UIBarStyleBlack;
    navigationController.navigationBar.translucent = NO;
    [self presentModalViewController:navigationController animated:YES];
}




- (IBAction)webFeedback:(id)sender {
  [UMFeedback showFeedback:self withAppkey:UMENG_APPKEY];
//    [UMFeedback showFeedback:self withAppkey:UMENG_APPKEY dictionary:[NSDictionary dictionaryWithObject:[NSArray arrayWithObjects:@"a", @"b", @"c", nil] forKey:@"hello"]];
}

- (IBAction)checkNewReplies:(id)sender {
  [_contentField resignFirstResponder];
  [UMFeedback checkWithAppkey:UMENG_APPKEY];
}

- (IBAction)editingEnded:(id)sender {
  [sender resignFirstResponder];
}

- (void)dealloc {
  _umFeedback.delegate = nil;
}


@end
