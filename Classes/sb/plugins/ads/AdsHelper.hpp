//
//  AdsHelper.hpp
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#ifndef AdsHelper_hpp
#define AdsHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"
#include "../SBPluginConfig.h"

#include "AdsDefine.h"
#include "AdsEventDispatcher.hpp"
#include "AdUnit.hpp"

NS_SB_BEGIN;

class AdsHelper : public cocos2d::Ref {
public:
    static AdsHelper* getInstance();
    static void destroyInstance();
    
    ~AdsHelper();
    
private:
    AdsHelper();
    
    void initUnits();
    
public:
    void init(const AdsConfig &config);
    void initImpl(const AdsConfig &config);
    
    void setBannerEnabled(bool isEnabled);
    void setInterstitialEnabled(bool isEnabled);
    void setRewardedVideoEnabled(bool isEnabled);
    
    void setAutoLoad(float interval);
    
    void loadBanner();
    void loadInterstitial();
    void loadRewardedVideo();

    bool loadBannerImpl();
    bool loadInterstitialImpl();
    bool loadRewardedVideoImpl();
    
    static void showBanner(AdListener *listener = nullptr);
    static void hideBanner();
    
    static void showBannerImpl();
    static void hideBannerImpl();
    
    static void showInterstitial(AdListener *listener = nullptr);
    static void showRewardedVideo(AdListener *listener = nullptr);
    
    static bool isBannerVisible();
    
    static bool isBannerLoaded();
    static bool isInterstitialLoaded();
    static bool isRewardedVideoLoaded();
    
    static float getBannerWidth();
    static float getBannerHeight();
    
private:
    CC_SYNTHESIZE_READONLY(AdsConfig, config, Config);
    
    bool isInitialized;
    
    CC_SYNTHESIZE_READONLY(AdsEventDispatcher*, eventDispatcher, EventDispatcher);
    CC_SYNTHESIZE_READONLY(AdUnit*, banner, Banner);
    CC_SYNTHESIZE_READONLY(AdUnit*, interstitial, Interstitial);
    CC_SYNTHESIZE_READONLY(AdUnit*, rewardedVideo, RewardedVideo);
    
    float autoLoadInterval;     // 광고 자동 로드 시간 간격 (단위 : 초)
    // SB_SYNTHESIZE_READONLY_BOOL(adOpened, AdOpened); // 광고 오픈 여부
};

NS_SB_END;

#endif /* AdsHelper_hpp */
