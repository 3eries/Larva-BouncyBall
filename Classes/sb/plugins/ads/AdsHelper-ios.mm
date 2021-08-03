//
//  AdsHelper-ios.mm
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#include "AdsHelper.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "ios/AdsManager.h"

#include "../../platform/SBPlatformMacros.h"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

/**
 * 광고 모듈 초기화
 */
void AdsHelper::initImpl(const AdsConfig &config) {
    
#if SB_PLUGIN_USE_ADS
    // 매니저 초기화
    AdsManager *mgr = [AdsManager getInstance];
    [mgr initAd:config];
#endif
}

/**
 * 배너 광고 로드
 */
bool AdsHelper::loadBannerImpl() {
#if SB_PLUGIN_USE_ADS
    return [[AdsManager getInstance] loadBanner];
#endif
    return false;
}

/**
 * 전면 광고 로드
 */
bool AdsHelper::loadInterstitialImpl() {
#if SB_PLUGIN_USE_ADS
    return [[AdsManager getInstance] loadInterstitial];
#endif
    return false;
}

/**
 * 보상형 동영상 광고 로드
 */
bool AdsHelper::loadRewardedVideoImpl() {
#if SB_PLUGIN_USE_ADS
    return [[AdsManager getInstance] loadRewardedVideo];
#endif
    return false;
}

/**
 * 배너 광고
 */
void AdsHelper::showBannerImpl() {
#if SB_PLUGIN_USE_ADS
    [[AdsManager getInstance] showBanner];
#endif
}

void AdsHelper::hideBannerImpl() {
#if SB_PLUGIN_USE_ADS
    [[AdsManager getInstance] hideBanner];
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
    
    [[AdsManager getInstance] showInterstitial];
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
    
    [[AdsManager getInstance] showRewardedVideo];
#endif
}

float AdsHelper::getBannerWidth() {
#if SB_PLUGIN_USE_ADS
    float w = ptToPx([[AdsManager getInstance] getBannerWidth]);
    w /= Director::getInstance()->getOpenGLView()->getScaleX();
    
    return w;
#else
    return 0;
#endif
}

float AdsHelper::getBannerHeight() {
#if SB_PLUGIN_USE_ADS
    float h = ptToPx([[AdsManager getInstance] getBannerHeight]);
    h /= Director::getInstance()->getOpenGLView()->getScaleY();
    
    return h;
#else
    return 0;
#endif
}

NS_SB_END;

#endif // CC_PLATFORM_IOS
