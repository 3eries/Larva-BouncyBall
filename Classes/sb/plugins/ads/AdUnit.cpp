//
//  AdUnit.cpp
//
//  Created by seongmin hwang on 09/09/2019.
//

#include "AdUnit.hpp"

#include "../../base/SBDebug.hpp"
#include "AdsHelper.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

AdUnit::AdUnit(const AdType &type, const string &unitId) :
type(type),
unitId(unitId),
listener(nullptr),
enabled(true), visible(false), loading(false), loaded(false), opened(false) {
}

AdUnit::~AdUnit() {
    
    CC_SAFE_RELEASE(listener);
}

void AdUnit::onAdLoaded() {

    Log::i("%s onAdLoaded", adTypeToString(type).c_str());
    
    loaded = true;
    loading = false;
    
    if( listener ) {
        listener->onAdLoaded();
    }
}

void AdUnit::onAdFailedToLoad(int errorCode) {
    
    Log::i("%s onAdFailedToLoad errorCode: %d", adTypeToString(type).c_str(), errorCode);
    
    loaded = false;
    loading = false;
    
    if( listener ) {
        listener->onAdFailedToLoad(errorCode);
    }
}

void AdUnit::onAdOpened() {
    
    Log::i("%s onAdOpened", adTypeToString(type).c_str());
    
    opened = true;
    
    if( listener ) {
        listener->onAdOpened();
    }
    
    Application::getInstance()->applicationDidEnterBackground();
}

void AdUnit::onAdClosed() {

    Log::i("%s onAdClosed", adTypeToString(type).c_str());
    
    opened = false;
    loaded = false;
    
    Application::getInstance()->applicationWillEnterForeground();
    
    if( listener ) {
        listener->onAdClosed();
    }
}

void AdUnit::onRewarded(const string &rewardType, int amount) {
    
    Log::i("%s onRewarded rewardType: %s, amount: %d", adTypeToString(type).c_str(),
           rewardType.c_str(), amount);
    
    auto rewardedVideoAdListener = dynamic_cast<RewardedVideoAdListener*>(listener);
    
    if( rewardedVideoAdListener ) {
        rewardedVideoAdListener->onRewarded(rewardType, amount);
    }
}

NS_SB_END;
