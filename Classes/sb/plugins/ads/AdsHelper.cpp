//
//  AdsManager.cpp
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#include "AdsHelper.hpp"

#include "../../base/SBDebug.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

static const string AUTO_LOAD_SCHEDULER = "AUTO_LOAD_SCHEDULER";

static AdsHelper *instance = nullptr;
AdsHelper* AdsHelper::getInstance() {
    
    if( !instance ) {
        instance = new AdsHelper();
    }
    
    return instance;
}

void AdsHelper::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

AdsHelper::AdsHelper() :
isInitialized(false),
eventDispatcher(nullptr),
banner(nullptr),
interstitial(nullptr),
rewardedVideo(nullptr),
autoLoadInterval(0) {
}

AdsHelper::~AdsHelper() {
    
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    
    CC_SAFE_DELETE(banner);
    CC_SAFE_DELETE(interstitial);
    CC_SAFE_DELETE(rewardedVideo);
    CC_SAFE_DELETE(eventDispatcher);
}

/**
 * 광고 모듈 초기화
 */
void AdsHelper::init(const AdsConfig &config) {
    
#if SB_PLUGIN_USE_ADS
    if( isInitialized ) {
        return;
    }
    
    this->isInitialized = true;
    this->config = config;
    this->eventDispatcher = new AdsEventDispatcher();
    
    initUnits();
    initImpl(config);
    
    setAutoLoad(config.autoLoadInterval);
#endif
}

void AdsHelper::initUnits() {

#if SB_PLUGIN_USE_ADS
    // Banner
    banner = new AdUnit(AdType::BANNER, config.bannerUnitId);
    banner->setLoading(true);
    
    auto bannerListener = AdListener::create(AdType::BANNER);
    bannerListener->onAdLoaded = [=]() {
        
        if( banner->isVisible() ) {
            this->showBanner();
        }
        
        this->eventDispatcher->dispatchLoadedEvent(AdType::BANNER);
    };
    bannerListener->onAdFailedToLoad = [=](int errorCode) {
        this->eventDispatcher->dispatchFailedToLoadEvent(AdType::BANNER, errorCode);
    };
    bannerListener->onAdOpened = [=]() {
        this->eventDispatcher->dispatchOpenedEvent(AdType::BANNER);
    };
    bannerListener->onAdClosed = [=]() {
        this->eventDispatcher->dispatchClosedEvent(AdType::BANNER);
    };
    banner->setListener(bannerListener);

    // Interstitial
    interstitial = new AdUnit(AdType::INTERSTITIAL, config.interstitialUnitId);
    interstitial->setLoading(true);
    
    auto interstitialListener = AdListener::create(AdType::INTERSTITIAL);
    interstitialListener->onAdLoaded = [=]() {
        this->eventDispatcher->dispatchLoadedEvent(AdType::INTERSTITIAL);
    };
    interstitialListener->onAdFailedToLoad = [=](int errorCode) {
        this->eventDispatcher->dispatchFailedToLoadEvent(AdType::INTERSTITIAL, errorCode);
    };
    interstitialListener->onAdOpened = [=]() {
        this->eventDispatcher->dispatchOpenedEvent(AdType::INTERSTITIAL);
    };
    interstitialListener->onAdClosed = [=]() {
        this->eventDispatcher->dispatchClosedEvent(AdType::INTERSTITIAL);
    };
    interstitial->setListener(interstitialListener);
    
    // Rewarded Video
    rewardedVideo = new AdUnit(AdType::REWARDED_VIDEO, config.rewardedVideoUnitId);
    rewardedVideo->setLoading(true);
    
    auto rewardedVideoAdListener = RewardedVideoAdListener::create();
    rewardedVideoAdListener->onAdLoaded = [=]() {
        this->eventDispatcher->dispatchLoadedEvent(AdType::REWARDED_VIDEO);
    };
    rewardedVideoAdListener->onAdFailedToLoad = [=](int errorCode) {
        this->eventDispatcher->dispatchFailedToLoadEvent(AdType::REWARDED_VIDEO, errorCode);
    };
    rewardedVideoAdListener->onAdOpened = [=]() {
        this->eventDispatcher->dispatchOpenedEvent(AdType::REWARDED_VIDEO);
    };
    rewardedVideoAdListener->onAdClosed = [=]() {
        this->eventDispatcher->dispatchClosedEvent(AdType::REWARDED_VIDEO);
    };
    rewardedVideoAdListener->onRewarded = [=](string type, int rewardAmount) {
        this->eventDispatcher->dispatchRewardedEvent(type, rewardAmount);
    };
    rewardedVideo->setListener(rewardedVideoAdListener);
#endif
}

/**
 * 광고 활성화 설정
 */
void AdsHelper::setBannerEnabled(bool isEnabled) {
    if( banner ) {
        banner->setEnabled(isEnabled);
    }
}

void AdsHelper::setInterstitialEnabled(bool isEnabled) {
    if( interstitial ) {
        interstitial->setEnabled(isEnabled);
    }
}

void AdsHelper::setRewardedVideoEnabled(bool isEnabled) {
    if( rewardedVideo ) {
        rewardedVideo->setEnabled(isEnabled);
    }
}

/**
 * 광고 자동 로드 스케줄러 실행
 */
void AdsHelper::setAutoLoad(float interval) {
    
#if SB_PLUGIN_USE_ADS
    if( autoLoadInterval == interval ) {
        return;
    }
    
    autoLoadInterval = interval;
    
    Director::getInstance()->getScheduler()->unschedule(AUTO_LOAD_SCHEDULER, this);
    
    if( interval == 0 ) {
        return;
    }
    
    Director::getInstance()->getScheduler()->schedule([=](float dt) {
        
        if( !isInitialized ) {
            return;
        }
        
        // 배너 광고 로드
        if( banner->isEnabled() && !banner->isLoaded() ) {
            this->loadBanner();
        }
        
        // 전면 광고 로드
        if( interstitial->isEnabled() &&
           !interstitial->isOpened() && !interstitial->isLoaded() ) {
            this->loadInterstitial();
        }
        
        // 보상형 비디오 광고 로드
        if( rewardedVideo->isEnabled() &&
           !rewardedVideo->isOpened() && !rewardedVideo->isLoaded() ) {
            this->loadRewardedVideo();
        }
        
    }, this, interval, false, AUTO_LOAD_SCHEDULER);
#endif
}

/**
 * 배너 광고 로드
 */
void AdsHelper::loadBanner() {
    
#if SB_PLUGIN_USE_ADS
    if( !banner ) {
        return;
    }
    
    Log::i("AdsHelper::loadBanner isEnabled: %d, isLoading: %d",
           banner->isEnabled(), banner->isLoading());
    
    if( banner->isEnabled() && !banner->isLoading() ) {
        if( loadBannerImpl() ) {
            banner->setLoading(true);
        }
    }
#endif
}

/**
 * 전면 광고 로드
 */
void AdsHelper::loadInterstitial() {
    
#if SB_PLUGIN_USE_ADS
    if( !interstitial ) {
        return;
    }
    
    Log::i("AdsHelper::loadInterstitial isEnabled: %d, isLoading: %d",
           interstitial->isEnabled(), interstitial->isLoading());
    
    if( interstitial->isEnabled() && !interstitial->isLoading() ) {
        if( loadInterstitialImpl() ) {
            interstitial->setLoading(true);
        }
    }
#endif
}

/**
 * 보상형 동영상 광고 로드
 */
void AdsHelper::loadRewardedVideo() {
    
#if SB_PLUGIN_USE_ADS
    if( !rewardedVideo ) {
        return;
    }
    
    Log::i("AdsHelper::loadRewardedVideo isEnabled: %d, isLoading: %d",
           rewardedVideo->isEnabled(), rewardedVideo->isLoading());
    
    if( rewardedVideo->isEnabled() && !rewardedVideo->isLoading() ) {
        if( loadRewardedVideoImpl() ) {
            rewardedVideo->setLoading(true);
        }
    }
#endif
}

void AdsHelper::showBanner(AdListener *listener) {
    
#if SB_PLUGIN_USE_ADS
    if( !instance->banner || !instance->banner->isEnabled() ) {
        return;
    }

    instance->banner->setVisible(true);
    instance->eventDispatcher->addListener(AdType::BANNER, listener);
    
    // 광고 로드됨
    if( instance->banner->isLoaded() ) {
        showBannerImpl();
    }
    // 광고 로드 필요
    else {
        instance->loadBanner();
    }
#endif
}

void AdsHelper::hideBanner() {
    
#if SB_PLUGIN_USE_ADS
    if( instance->banner ) {
        instance->banner->setVisible(false);
        hideBannerImpl();
    }
#endif
}

bool AdsHelper::isBannerVisible() {
#if SB_PLUGIN_USE_ADS
    return instance->banner ? instance->banner->isVisible() : false;
#else
    return false;
#endif
}

bool AdsHelper::isBannerLoaded() {
#if SB_PLUGIN_USE_ADS
    return instance->banner ? instance->banner->isLoaded() : false;
#else
    return false;
#endif
}

bool AdsHelper::isInterstitialLoaded() {
#if SB_PLUGIN_USE_ADS
    return instance->interstitial ? instance->interstitial->isLoaded() : false;
#else
    return false;
#endif
}

bool AdsHelper::isRewardedVideoLoaded() {
#if SB_PLUGIN_USE_ADS
    return instance->rewardedVideo ? instance->rewardedVideo->isLoaded() : false;
#else
    return false;
#endif
}

NS_SB_END;

