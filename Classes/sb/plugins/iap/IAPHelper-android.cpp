//
//  IAPHelper-android.cpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "IAPHelper.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define JNI_CLASS_NAME     "com/superbomb/plugins/iap/IAPHelper"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;
NS_IAP_BEGIN;

void IAPHelper::initImpl(const Config &config, const string &originalJson) {
    
#if SB_PLUGIN_USE_IAP
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "initIAP", originalJson);
#endif
}

void IAPHelper::purchaseImpl(const Item &item) {

#if SB_PLUGIN_USE_IAP
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "purchase", item.itemId);
#endif
}

void IAPHelper::restoreImpl() {

#if SB_PLUGIN_USE_IAP
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "restore");
#endif
}

void IAPHelper::consumeAll() {

#if SB_PLUGIN_USE_IAP
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "consumeAll");
#endif
}

bool IAPHelper::isReady() {

#if SB_PLUGIN_USE_IAP
    return cocos2d::JniHelper::callStaticBooleanMethod(JNI_CLASS_NAME, "isReady");
#else
    return false;
#endif
}

#if SB_PLUGIN_USE_IAP
extern "C" {
    void Java_com_superbomb_plugins_iap_IAPHelper_nativeOnSetupFinished(JNIEnv *env, jobject obj) {
        
        if( IAPHelper::getInstance()->isAutoRestore() ) {
            IAPHelper::restore();
        }
    }
    
    void Java_com_superbomb_plugins_iap_IAPHelper_nativeOnPurchased(JNIEnv *env, jobject obj, jstring jitemId) {
        
        string itemId = JniHelper::jstring2string(jitemId);
        IAPHelper::getInstance()->onPurchased(itemId);
    }
    
    void Java_com_superbomb_plugins_iap_IAPHelper_nativeOnPurchaseError(JNIEnv *env, jobject obj, jstring jerrorMsg) {
        
        string errorMsg = JniHelper::jstring2string(jerrorMsg);
        IAPHelper::getInstance()->onPurchaseError(errorMsg);
    }
    
    void Java_com_superbomb_plugins_iap_IAPHelper_nativeOnPurchaseCanceled(JNIEnv *env, jobject obj) {
        
        IAPHelper::getInstance()->onPurchaseCanceled();
    }
    
    void Java_com_superbomb_plugins_iap_IAPHelper_nativeOnRestored(JNIEnv *env, jobject obj, jstring jitemId) {
        
        string itemId = JniHelper::jstring2string(jitemId);
        IAPHelper::getInstance()->onRestored(itemId);
    }
    
    void Java_com_superbomb_plugins_iap_IAPHelper_nativeOnRestoreFinished(JNIEnv *env, jobject obj, jboolean jresult) {
        
        IAPHelper::getInstance()->onRestoreFinished(jresult);
    }
}
#endif

NS_IAP_END;
NS_SB_END;

#endif // CC_PLATFORM_ANDROID
