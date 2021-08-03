//
//  AdListener.cpp
//
//  Created by seongmin hwang on 2018. 7. 26..
//

#include "AdListener.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

#pragma mark- AdListener

AdListener* AdListener::create(AdType type) {
    
    auto listener = new AdListener(type);
    
    if( listener ) {
        listener->autorelease();
        return listener;
    }
    
    CC_SAFE_DELETE(listener);
    return nullptr;
}

AdListener::AdListener(AdType type) :
type(type), target(nullptr), forever(false),
onAdLoaded(nullptr), onAdFailedToLoad(nullptr),
onAdOpened(nullptr), onAdClosed(nullptr) {
}

AdListener::~AdListener() {
}

#pragma mark- RewardedVideoAdListener

RewardedVideoAdListener::RewardedVideoAdListener() : AdListener(AdType::REWARDED_VIDEO),
rewarded(false),
onRewarded(nullptr) {
}

RewardedVideoAdListener::~RewardedVideoAdListener() {
}

NS_SB_END;
