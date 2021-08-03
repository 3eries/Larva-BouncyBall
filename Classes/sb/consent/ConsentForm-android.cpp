//
//  ConsentForm.cpp
//
//  Created by seongmin hwang on 2018. 9. 26..
//

#include "ConsentForm.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"
#include "ConsentManager.hpp"

#define JNI_CLASS_NAME     "com/superbomb/consent/ConsentFormHelper"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

void ConsentForm::show() {
    
#if SB_PLUGIN_USE_IAP
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "show");
#endif // SB_PLUGIN_USE_IAP
}

extern "C" {
    void Java_com_superbomb_consent_ConsentFormHelper_nativeOnClickPrivacyPolicy(JNIEnv *env, jobject obj) {
        ConsentManager::setPrivacyPolicyChecked(true);
    }
    
    void Java_com_superbomb_consent_ConsentFormHelper_nativeOnClickOk(JNIEnv *env, jobject obj) {
        ConsentManager::setPrivacyPolicyChecked(true);
    }
}

NS_SB_END;

#endif // CC_PLATFORM_ANDROID
