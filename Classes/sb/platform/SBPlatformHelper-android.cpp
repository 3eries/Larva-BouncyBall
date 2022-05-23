//
//  SBPlatformHelper.cpp
//
//  Created by seongmin hwang
//

#include "SBPlatformHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define JNI_CLASS_NAME     "com/superbomb/SBPlatformHelper"

USING_NS_CC;
using namespace std;

bool SBPlatformHelper::isNetworkOnline() {
    return cocos2d::JniHelper::callStaticBooleanMethod(JNI_CLASS_NAME, "isNetworkOnline");
}

#endif // CC_PLATFORM_ANDROID
