//
//  FirebaseManager-android.cpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "FirebaseManager.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define JNI_CLASS_NAME     "com/superbomb/plugins/firebase/FirebaseManager"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

void FirebaseManager::init() {
    
}

NS_SB_END;

#endif // CC_PLATFORM_ANDROID

