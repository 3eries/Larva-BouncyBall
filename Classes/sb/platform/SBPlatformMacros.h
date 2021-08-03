//
//  SBPlatformMacros.h
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#ifndef SBPlatformMacros_h
#define SBPlatformMacros_h

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#define NS_STRING(__str__)      [NSString stringWithUTF8String:__str__]

#define ROOT_VIEW_CONTROLLER \
(((AppController *)[UIApplication sharedApplication].delegate).viewController)

#define pxToPt(__x__)           (__x__ / [UIScreen mainScreen].scale)
#define ptToPx(__x__)           (__x__ * [UIScreen mainScreen].scale)

#endif

#endif /* SBPlatformMacros_h */
