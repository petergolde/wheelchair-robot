//
//  JoystickView.m
//  SimpleChat
//
//  Created by Peter Golde on 1/17/15.
//  Copyright (c) 2015 redbear. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "RBLViewController.h"
#import "JoystickView.h"

@implementation JoystickView

- (id)initWithOuter:(RBLViewController *)outer frame:(CGRect)f
{
    self = [super initWithFrame:f];
    
    self.outer = outer;
    self.image = [UIImage imageNamed: @"./joystick.png"];
    self.contentMode = UIViewContentModeScaleAspectFit;
    self.userInteractionEnabled = YES;
    
    return self;
}

- (void) touch:(UITouch *)touch
{
    CGPoint loc = [touch locationInView:self];
    
    float xPercent = (float)(loc.x - self.bounds.size.width / 2) / (self.bounds.size.width / 2);
    float yPercent = (float)(loc.y - self.bounds.size.height / 2) / (self.bounds.size.height / 2);
    
    if (xPercent > 0)
        xPercent -= MIN(xPercent, 0.2F);
    else if (xPercent < 0)
        xPercent += MAX(xPercent, -0.2F);

    if (yPercent > 0)
        yPercent -= MIN(yPercent, 0.2F);
    else if (yPercent < 0)
        yPercent += MAX(yPercent, -0.2F);
    
    yPercent *= -1.0F;
    
    [((RBLViewController *) self.outer) joystickTouchAtX: xPercent y:yPercent];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
    [self touch: (UITouch*)[touches anyObject]];
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
       [self touch: (UITouch*)[touches anyObject]];
}


@end