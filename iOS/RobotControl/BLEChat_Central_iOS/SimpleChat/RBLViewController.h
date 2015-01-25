//
//  RBLViewController.h
//  SimpleChat
//
//  Created by redbear on 14-4-8.
//  Copyright (c) 2014年 redbear. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "BLE.h"
#import "JoystickView.h"

@interface RBLViewController : UIViewController <UITableViewDataSource, UITableViewDelegate, UITextFieldDelegate, BLEDelegate>
{
    BLE *bleShield;
    UIActivityIndicatorView *activityIndicator;
}

@property (nonatomic, weak) IBOutlet UITableView *tableView;
@property (nonatomic, weak) IBOutlet UITextField *text;
@property (strong, nonatomic) IBOutlet UISegmentedControl *uiPickerSegmented;
@property (strong, nonatomic) IBOutlet UIButton *releaseFailsafeButton;

@property (strong, nonatomic) IBOutlet UIView *tankView;
@property (strong, nonatomic) UIView *joystickView;

@property (strong, nonatomic) IBOutlet UISlider *leftMotorSlider;
@property (strong, nonatomic) IBOutlet UISlider *rightMotorSlider;
@property (strong, nonatomic) IBOutlet UIView *allStopButton;

- (void) joystickTouchAtX: (float) x y:(float) y;
@end
