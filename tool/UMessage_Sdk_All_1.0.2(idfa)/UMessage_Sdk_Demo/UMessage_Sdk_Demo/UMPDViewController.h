//
//  UMPDViewController.h
//  UMessage_Sdk_Demo
//
//  Created by luyiyuan on 13-10-25.
//  Copyright (c) 2013年 umeng.com. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface UMPDViewController : UIViewController
//tags
@property (nonatomic,strong) IBOutlet UITextField *tagsTextField;
@property (nonatomic,strong) IBOutlet UILabel *remainLabel;

-(IBAction)actAddTags:(id)sender;
-(IBAction)removeTags:(id)sender;
-(IBAction)resetTags:(id)sender;

//alias
@property (nonatomic,strong) IBOutlet UITextField *nameTextField;
@property (nonatomic,strong) IBOutlet UIPickerView *typePickView;
@property (nonatomic,strong) IBOutlet UIButton *selTypeButton;

-(IBAction)actSelType:(id)sender;
-(IBAction)actAddAlias:(id)sender;
-(IBAction)actRemoveAlias:(id)sender;
@end
