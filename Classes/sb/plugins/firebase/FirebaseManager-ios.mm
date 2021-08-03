//
//  FirebaseManager-ios.m
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "FirebaseManager.hpp"
#include "../SBPluginConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>

#if SB_PLUGIN_USE_FIREBASE
#import <Firebase/Firebase.h>
#endif

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

void FirebaseManager::init() {

#if SB_PLUGIN_USE_FIREBASE
    [FIRApp configure];
#endif
}

NS_SB_END;

#endif // CC_PLATFORM_IOS
