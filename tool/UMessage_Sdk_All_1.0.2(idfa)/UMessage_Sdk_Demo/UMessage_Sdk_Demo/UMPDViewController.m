//
//  UMPDViewController.m
//  UMessage_Sdk_Demo
//
//  Created by luyiyuan on 13-10-25.
//  Copyright (c) 2013年 umeng.com. All rights reserved.
//

#import "UMPDViewController.h"
#import "UMessage.h"

@interface UMPDViewController ()
@property (nonatomic,strong) NSArray *types;
@end

@implementation UMPDViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
    self.types = @[kUMessageAliasTypeSina,kUMessageAliasTypeTencent,kUMessageAliasTypeQQ,kUMessageAliasTypeWeiXin,kUMessageAliasTypeBaidu,kUMessageAliasTypeRenRen,kUMessageAliasTypeKaixin,kUMessageAliasTypeDouban,kUMessageAliasTypeFacebook,kUMessageAliasTypeTwitter];
    
    
    UITapGestureRecognizer *tapRecognizer = [[UITapGestureRecognizer alloc]
                                             initWithTarget:self action:@selector(respondToTapGesture:)];
    
    // Specify that the gesture must be a single tap
    tapRecognizer.numberOfTapsRequired = 1;
    
    // Add the tap gesture recognizer to the view
    [self.view addGestureRecognizer:tapRecognizer];
    
    //init string
    [self.selTypeButton setTitle:[self.types objectAtIndex:0] forState:UIControlStateNormal];
    [self.remainLabel setText:[NSString stringWithFormat:@"剩余:-1"]];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (void)respondToTapGesture:(id)sender
{
    
    if(!self.typePickView.hidden)
        {
        [self.typePickView setHidden:YES];
        }
    
    if([self.tagsTextField isFirstResponder])
        {
        [self.tagsTextField resignFirstResponder];
        }
    
    if([self.nameTextField isFirstResponder])
        {
        [self.nameTextField resignFirstResponder];
        }
}

-(IBAction)actAddTags:(id)sender
{
    
    [UMessage addTag:[self getTagsWithString:self.tagsTextField.text]
            response:^(id responseObject, NSInteger remain, NSError *error) {
                [self.remainLabel setText:[NSString stringWithFormat:@"剩余:%ld",(long)remain]];
                [self.remainLabel setNeedsDisplay];
                if(responseObject)
                    {
                    [self showMessageAlert:@"添加成功！"];
                    }
                else
                    {
                    [self showMessageAlert:error.localizedDescription];
                    }
                
            }];
}
-(IBAction)removeTags:(id)sender
{
    [UMessage removeTag:[self getTagsWithString:self.tagsTextField.text]
               response:^(id responseObject, NSInteger remain, NSError *error) {
                   [self.remainLabel setText:[NSString stringWithFormat:@"剩余:%ld",(long)remain]];
                   [self.remainLabel setNeedsDisplay];
                   if(responseObject)
                       {
                       [self showMessageAlert:@"删除成功！"];
                       }
                   else
                       {
                       [self showMessageAlert:error.localizedDescription];
                       }
                   
               }];
}
-(IBAction)resetTags:(id)sender
{
    [UMessage removeAllTags:^(id responseObject, NSInteger remain, NSError *error) {
        [self.remainLabel setText:[NSString stringWithFormat:@"剩余:%ld",(long)remain]];
        [self.remainLabel setNeedsDisplay];
        if(responseObject)
            {
            [self showMessageAlert:@"重置成功！"];
            }
        else
            {
            [self showMessageAlert:error.localizedDescription];
            }
    }];
}

//alias

-(IBAction)actSelType:(id)sender
{
    [self.typePickView setHidden:NO];
}

- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)thePickerView
{
    return 1;
}

- (NSInteger)pickerView:(UIPickerView *)thePickerView numberOfRowsInComponent:(NSInteger)component
{
    return [self.types count];//将数据建立一个数组
}
- (NSString *)pickerView:(UIPickerView *)thePickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [self.types objectAtIndex:row];
}

- (void)pickerView:(UIPickerView *)thePickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    [self.selTypeButton setTitle:[self.types objectAtIndex:row] forState:UIControlStateNormal];
    [self.selTypeButton setNeedsDisplay];
}

-(IBAction)actAddAlias:(id)sender
{
    NSLog(@"self nameTextField [%@]",self.nameTextField.text);
    
    [UMessage addAlias:self.nameTextField.text type:[self.selTypeButton titleForState:UIControlStateNormal] response:^(id responseObject, NSError *error) {
        if(responseObject)
        {
            [self showMessageAlert:@"绑定成功！"];
        }
        else
        {
            [self showMessageAlert:error.localizedDescription];
        }
    }];
}

-(IBAction)actRemoveAlias:(id)sender
{
    NSLog(@"self nameTextField [%@]",self.nameTextField.text);
    
    [UMessage removeAlias:self.nameTextField.text type:[self.selTypeButton titleForState:UIControlStateNormal] response:^(id responseObject, NSError *error) {
        if(responseObject)
        {
            [self showMessageAlert:@"删除成功！"];
        }
        else
        {
            [self showMessageAlert:error.localizedDescription];
        }
    }];
}

- (NSArray *)getTagsWithString:(NSString *)string
{
    if(string == nil)
        {
        return nil;
        }
    
    NSArray *sepArray = [string componentsSeparatedByString:@","];
    
    __autoreleasing NSMutableArray *array = [[NSMutableArray alloc] initWithArray:sepArray];
    
    for(NSString *str in sepArray)
        {
        if([str length]==0)
            {
            [array removeObject:str];
            }
        }
    
    return array;
}

- (void)showMessageAlert:(NSString *)message
{
    if([message length])
        {
        //处理前台消息框弹出
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:NSLocalizedString(@"Notification",@"Notification") message:message delegate:self cancelButtonTitle:NSLocalizedString(@"OK", @"OK") otherButtonTitles:nil];
        [alert show];
        }
    else
        {
        NSLog(@"showMessageAlert: Message is nil...[%@]",message);
        }
    
}

-(BOOL)shouldAutorotate
{
    return NO;
}

- (NSUInteger)supportedInterfaceOrientations {
    return UIInterfaceOrientationMaskPortrait;
}

@end
