//
//  FirebaseAnalytics-android.cpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "FirebaseAnalytics.hpp"

#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "../../platform/android/SBJniHelper.hpp"
#include "../../json/SBJSON.h"

#include "platform/android/jni/JniHelper.h"

#define JNI_CLASS_NAME     "com/superbomb/plugins/firebase/Analytics"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;
    
void FirebaseAnalytics::setCurrentScreen(const string &screen, const string &screenClass) {

    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "setCurrentScreen", screen, screenClass);
}

void FirebaseAnalytics::logEvent(const string &event, const EventParams &params) {

    string jsonParam = "";
    
    if( params.size() > 0 ) {
        jsonParam = SBJSON::toJSON(Value(params));
    }
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "logEvent", event, jsonParam);
}

NS_SB_END;

#endif // CC_PLATFORM_ANDROID
#endif // SB_PLUGIN_USE_FIREBASE_ANALYTICS
