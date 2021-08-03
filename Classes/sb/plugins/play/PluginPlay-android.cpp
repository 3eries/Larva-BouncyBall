//
//  PluginPlay-android.cpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "PluginPlay.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define JNI_CLASS_NAME     "com/superbomb/plugins/gps/GameHelper"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

void PluginPlay::init() {
    
#if SB_PLUGIN_USE_PLAY
#endif
}

void PluginPlay::signIn(bool showLoginUI) {

#if SB_PLUGIN_USE_PLAY
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "signIn", showLoginUI);
#endif
}

void PluginPlay::signOut() {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "signOut");
#endif
}

bool PluginPlay::isSignedIn() {
    
#if SB_PLUGIN_USE_PLAY
    return cocos2d::JniHelper::callStaticBooleanMethod(JNI_CLASS_NAME, "isSignedIn");
#else
    return false;
#endif
}

string PluginPlay::getPlayerId() {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return "";
    }
    
    return cocos2d::JniHelper::callStaticStringMethod(JNI_CLASS_NAME, "getPlayerId");
#else
    return "";
#endif
}

void PluginPlay::showAllLeaderboards() {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "showAllLeaderboards");
#endif
}

void PluginPlay::showLeaderboard(const string &leaderboardName) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "showLeaderboard", leaderboardName);
#endif
}

void PluginPlay::submitScore(const string &leaderboardName, long score) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    // long to jlong issue
    // cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "submitScore", leaderboardName, score);
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "submitScore", leaderboardName, (int)score);
    
    /*
    JniMethodInfo t;
    
    const char *methodName = "submitScore";
    const char *paramCode = "(Ljava/lang/String;J)V";
    
    bool find = cocos2d::JniHelper::getStaticMethodInfo(t, JNI_CLASS_NAME, methodName, paramCode);
    CCAssert(find, "PluginPlay::submitScore jni error.");
    
    jstring jstr = t.env->NewStringUTF(leaderboardName.c_str());
    jlong jscore = (jlong)score;
    
    t.env->CallStaticVoidMethodA(t.classID, t.methodID, jstr, (jvalue*)jscore);
    
    t.env->DeleteLocalRef(jstr);
    t.env->DeleteLocalRef(t.classID);
     */
#endif
}

void PluginPlay::showAchievements() {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "showAchievements");
#endif
}

void PluginPlay::loadAchievements(bool forceReload) {
    
    // TODO:
}

void PluginPlay::unlockAchievement(const string &achievementName) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "unlockAchievement", achievementName);
#endif
}

void PluginPlay::incrementAchievement(const string &achievementName, int increment) {
   
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "incrementAchievement", achievementName, increment);
#endif
}

void PluginPlay::reveal(const string &achievementName) {
 
    // TODO:
}

void PluginPlay::setSteps(const string &achievementName, double steps) {
    
    // TODO:
}

NS_SB_END;

#endif // CC_PLATFORM_ANDROID
