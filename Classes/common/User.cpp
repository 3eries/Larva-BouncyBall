//
//  User.cpp
//
//  Created by seongmin hwang on 2018. 5. 23..
//

#include "User.hpp"

#include "Define.h"
#include "SceneManager.h"
#include "GameConfiguration.hpp"
#include "UserDefaultKey.h"

#include "ReviewPopup.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define USER_DEFAULT_KEY_REMOVE_ADS                     "USER_REMOVE_ADS"
#define USER_DEFAULT_KEY_TUTORIAL_COMPLETED             "USER_TUTORIAL_COMPLETED"

static User *instance = nullptr;
User* User::getInstance() {
    
    if( !instance ) {
        instance = new User();
    }
    
    return instance;
}

void User::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

User::User() {
}

User::~User() {
 
    removeListeners(this);
}

void User::init() {
    
    CCLOG("User {");
    CCLOG("\t isRemovedAds: %d", isRemovedAds());
    CCLOG("}");
    
    if( isRemovedAds() ) {
        removeAds();
    }
    
    // IAP 리스너 초기화
    auto onRemoveAds = [=]() {
        this->removeAds();
    };
    
    // purchase listener
    /*
    auto purchaseListener = iap::PurchaseListener::create();
    purchaseListener->setForever(true);
    purchaseListener->onRemoveAds = onRemoveAds;
    iap::IAPHelper::getInstance()->addListener(this, purchaseListener);
    */
     
    // restore listener
    auto restoreListener = iap::RestoreListener::create();
    restoreListener->setForever(true);
    restoreListener->onRemoveAds = onRemoveAds;
    iap::IAPHelper::getInstance()->addListener(this, restoreListener);
}

/**
 * 광고 제거 설정
 */
void User::setRemoveAds(bool isRemoveAds) {
    
    USER_DEFAULT->setBoolForKey(USER_DEFAULT_KEY_REMOVE_ADS, isRemoveAds);
    USER_DEFAULT->flush();
    
    AdsHelper::getInstance()->setBannerEnabled(!isRemoveAds);
    AdsHelper::getInstance()->setInterstitialEnabled(!isRemoveAds);
    AdsHelper::getInstance()->setRewardedVideoEnabled(!isRemoveAds);
    
    if( isRemoveAds ) {
        AdsHelper::hideBanner();
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DIRECTOR_EVENT_REMOVE_ADS);
    }
}

/**
 * 광고 제거
 */
void User::removeAds() {

    setRemoveAds(true);
}

/**
 * 광고 제거 여부
 */
bool User::isRemovedAds() {
    
    return USER_DEFAULT->getBoolForKey(USER_DEFAULT_KEY_REMOVE_ADS, false);
}

/**
 * 튜토리얼 완료 여부를 설정합니다
 */
void User::setTutorialCompleted(bool isCompleted) {
    
    USER_DEFAULT->setBoolForKey(USER_DEFAULT_KEY_TUTORIAL_COMPLETED, isCompleted);
    USER_DEFAULT->flush();
}

/**
 * 튜토리얼 완료 여부를 반환합니다
 */
bool User::isTutorialCompleted() {
    
    return USER_DEFAULT->getBoolForKey(USER_DEFAULT_KEY_TUTORIAL_COMPLETED, false);
}

