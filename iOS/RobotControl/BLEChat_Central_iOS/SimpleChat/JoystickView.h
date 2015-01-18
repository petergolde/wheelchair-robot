//
//  JoystickView.h
//  SimpleChat
//
//  Created by Peter Golde on 1/17/15.
//  Copyright (c) 2015 redbear. All rights reserved.
//

#ifndef SimpleChat_JoystickView_h
#define SimpleChat_JoystickView_h

#import "RBLViewController.h"

@interface JoystickView : UIImageView
{
}

@property (weak, nonatomic) UIViewController *outer;

- (id)initWithOuter:(UIViewController *) outer frame:(CGRect) f;


@end

#endif