//
//  SBSecurity.cpp
//
//  Created by seongmin hwang on 2018. 9. 19..
//

#include "SBSecurity.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define JNI_CLASS_NAME     "com/superbomb/security/SBSecurity"

USING_NS_CC;
using namespace std;

/**
 *  AES256 암호화
 */
string SBSecurity::encryptAES256(const string &str, const string &key) {
    
    return cocos2d::JniHelper::callStaticStringMethod(JNI_CLASS_NAME, "encryptAES256", str, key);
}

/**
 *  AES256 복호화
 */
string SBSecurity::decryptAES256(const string &str, const string &key) {
    
    return cocos2d::JniHelper::callStaticStringMethod(JNI_CLASS_NAME, "decryptAES256", str, key);
}

#endif // CC_PLATFORM_ANDROID
