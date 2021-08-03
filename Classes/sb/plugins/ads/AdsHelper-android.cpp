//
//  AdsHelper-android.cpp
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#include "AdsHelper.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define JNI_CLASS_NAME     "com/superbomb/plugins/ads/AdsManager"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

/**
 * 광고 모듈 초기화
 */
void AdsHelper::initImpl(const AdsConfig &config) {
#if SB_PLUGIN_USE_ADS
    // Android 매니저 초기화
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "initAd",
                                             config.appId,
                                             config.bannerUnitId,
                                             config.interstitialUnitId,
                                             config.rewardedVideoUnitId);
#endif
}

/**
 * 배너 광고 로드
 */
bool AdsHelper::loadBannerImpl() {
#if SB_PLUGIN_USE_ADS
    return cocos2d::JniHelper::callStaticBooleanMethod(JNI_CLASS_NAME, "loadBanner");
#endif
    return false;
}

/**
 * 전면 광고 로드
 */
bool AdsHelper::loadInterstitialImpl() {
#if SB_PLUGIN_USE_ADS
    return cocos2d::JniHelper::callStaticBooleanMethod(JNI_CLASS_NAME, "loadInterstitial");
#endif
    return false;
}

/**
 * 보상형 동영상 광고 로드
 */
bool AdsHelper::loadRewardedVideoImpl() {
#if SB_PLUGIN_USE_ADS
    return cocos2d::JniHelper::callStaticBooleanMethod(JNI_CLASS_NAME, "loadRewardedVideo");
#endif
    return false;
}

/**
 * 배너 광고
 */
void AdsHelper::showBannerImpl() {
#if SB_PLUGIN_USE_ADS
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "showBanner");
#endif
}

void AdsHelper::hideBannerImpl() {
#if SB_PLUGIN_USE_ADS
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "hideBanner");
#endif
}

/**
 * 전면 광고
 */
void AdsHelper::showInterstitial(AdListener *listener) {
    
#if SB_PLUGIN_USE_ADS
    if( !getInstance()->getInterstitial()->isEnabled() ) {
        return;
    }
    
    getInstance()->getEventDispatcher()->addListener(AdType::INTERSTITIAL, listener);
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "showInterstitial");
#endif
}

/**
 * 보상형 비디오
 */
void AdsHelper::showRewardedVideo(AdListener *listener) {
    
#if SB_PLUGIN_USE_ADS
    if( !getInstance()->getRewardedVideo()->isEnabled() ) {
        return;
    }
    
    getInstance()->getEventDispatcher()->addListener(AdType::REWARDED_VIDEO, listener);
    
    cocos2d::JniHelper::callStaticVoidMethod(JNI_CLASS_NAME, "showRewardedVideo");
#endif
}

float AdsHelper::getBannerWidth() {

#if SB_PLUGIN_USE_ADS
    float w = cocos2d::JniHelper::callStaticFloatMethod(JNI_CLASS_NAME, "getBannerWidth");
    w /= Director::getInstance()->getOpenGLView()->getScaleX();
    
    return w;
#else
    return 0;
#endif
}

float AdsHelper::getBannerHeight() {

#if SB_PLUGIN_USE_ADS
    float h = cocos2d::JniHelper::callStaticFloatMethod(JNI_CLASS_NAME, "getBannerHeight");
    h /= Director::getInstance()->getOpenGLView()->getScaleY();
    
    return h;
#else
    return 0;
#endif
}

#if SB_PLUGIN_USE_ADS
extern "C" {

    /**
     * Banner
     */
    void Java_com_superbomb_plugins_ads_AdsManager_nativeBannerOnAdLoaded(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getBanner()->onAdLoaded();
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeBannerOnAdFailedToLoad(JNIEnv *env, jobject obj, jint error) {
        AdsHelper::getInstance()->getBanner()->onAdFailedToLoad(error);
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeBannerOnAdOpened(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getBanner()->onAdOpened();
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeBannerOnAdClosed(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getBanner()->onAdClosed();
    }
    
    /**
     * Interstitial
     */
    void Java_com_superbomb_plugins_ads_AdsManager_nativeInterstitialOnAdLoaded(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getInterstitial()->onAdLoaded();
    }

    void Java_com_superbomb_plugins_ads_AdsManager_nativeInterstitialOnAdFailedToLoad(JNIEnv *env, jobject obj, jint error) {
        AdsHelper::getInstance()->getInterstitial()->onAdFailedToLoad(error);
    }

    void Java_com_superbomb_plugins_ads_AdsManager_nativeInterstitialOnAdOpened(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getInterstitial()->onAdOpened();
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeInterstitialOnAdClosed(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getInterstitial()->onAdClosed();
    }
    
    /**
     * Rewarded Video
     */
    void Java_com_superbomb_plugins_ads_AdsManager_nativeRewardedVideoOnAdLoaded(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getRewardedVideo()->onAdLoaded();
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeRewardedVideoOnAdFailedToLoad(JNIEnv *env, jobject obj, jint error) {
        AdsHelper::getInstance()->getRewardedVideo()->onAdFailedToLoad(error);
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeRewardedVideoOnAdOpened(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getRewardedVideo()->onAdOpened();
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeRewardedVideoOnAdClosed(JNIEnv *env, jobject obj) {
        AdsHelper::getInstance()->getRewardedVideo()->onAdClosed();
    }
    
    void Java_com_superbomb_plugins_ads_AdsManager_nativeRewardedVideoOnRewarded(JNIEnv *env, jobject obj, jstring jtype, jint rewardAmount) {
        string type = JniHelper::jstring2string(jtype);
        AdsHelper::getInstance()->getRewardedVideo()->onRewarded(type, rewardAmount);
    }
}
#endif

NS_SB_END;

#endif // CC_PLATFORM_ANDROID
