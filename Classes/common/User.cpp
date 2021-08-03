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
#define USER_DEFAULT_KEY_HINT                           "USER_HINT"
#define USER_DEFAULT_KEY_COIN                           "USER_COIN"
#define USER_DEFAULT_KEY_TUTORIAL_COMPLETED             "USER_TUTORIAL_COMPLETED"
#define USER_DEFAULT_KEY_CLEAR_STAGE                    "USER_CLEAR_STAGE"

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
    CCLOG("\tcoin: %d, clear stage: %d", getCoin(), getClearStage());
    CCLOG("}");
    
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
    
    if( isRemovedAds() ) {
        removeAds();
    }
}

/**
 * 광고 제거 설정
 */
void User::setRemoveAds(bool isRemoveAds) {
    
    UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_KEY_REMOVE_ADS, isRemoveAds);
    UserDefault::getInstance()->flush();
    
    AdsHelper::getInstance()->setBannerEnabled(!isRemoveAds);
    AdsHelper::getInstance()->setInterstitialEnabled(!isRemoveAds);
    
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
    
    return UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_KEY_REMOVE_ADS, false);
}

/**
 * 힌트 개수를 설정합니다
 */
void User::setHintCount(int i) {
    
    UserDefault::getInstance()->setIntegerForKey(USER_DEFAULT_KEY_HINT, i);
    UserDefault::getInstance()->flush();
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DIRECTOR_EVENT_UPDATE_HINT_COUNT);
}

/**
 * 힌트 개수를 반환합니다
 */
int User::getHintCount() {
    
    return UserDefault::getInstance()->getIntegerForKey(USER_DEFAULT_KEY_HINT, GAME_CONFIG->getFirstHint());
}

/**
 * 힌트를 획득합니다
 */
void User::getHint(int i) {
    
    setHintCount(getHintCount() + i);
}

/**
 * 힌트를 사용합니다
 */
bool User::useHint() {
    
    int count = getHintCount();
    if( count == 0 ) {
        return false;
    }
    
    setHintCount(count-1);
    return true;
}

/**
 * 코인을 설정합니다
 */
void User::setCoin(int i) {
    
    UserDefault::getInstance()->setIntegerForKey(USER_DEFAULT_KEY_COIN, i);
    UserDefault::getInstance()->flush();
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DIRECTOR_EVENT_UPDATE_USER_COIN);
}

/**
 * 코인을 획득합니다
 */
void User::earnCoin(int i) {
    
    int coin = getCoin() + i;
    setCoin(coin);
}

/**
 * 코인을 소모합니다
 * @return 코인이 충분한지 않으면 false를 반환합니다
 */
bool User::spendCoin(int i) {
    
    int coin = getCoin() - i;
    if( coin < 0 ) {
        return false;
    }
    
    setCoin(coin);
    return true;
}

/**
 * 코인을 반환합니다
 */
int User::getCoin() {
    
    return UserDefault::getInstance()->getIntegerForKey(USER_DEFAULT_KEY_COIN, GAME_CONFIG->getFirstCoin());
}

/**
 * 코인이 충분한지 반환합니다
 */
bool User::isEnoughCoin(int i) {
    
    return getCoin() >= i;
}

/**
 * 튜토리얼 완료 여부를 설정합니다
 */
void User::setTutorialCompleted(bool isCompleted) {
    
    UserDefault::getInstance()->setBoolForKey(USER_DEFAULT_KEY_TUTORIAL_COMPLETED, isCompleted);
    UserDefault::getInstance()->flush();
}

/**
 * 튜토리얼 완료 여부를 반환합니다
 */
bool User::isTutorialCompleted() {
    
    return UserDefault::getInstance()->getBoolForKey(USER_DEFAULT_KEY_TUTORIAL_COMPLETED, false);
}

/**
 * 클리어한 스테이지를 설정합니다
 */
void User::setClearStage(int stage) {
    
    if( stage <= getClearStage() ) {
        return;
    }
    
    UserDefault::getInstance()->setIntegerForKey(USER_DEFAULT_KEY_CLEAR_STAGE, stage);
    UserDefault::getInstance()->flush();
}

/**
 * 클리어한 스테이지를 반환합니다
 */
int User::getClearStage() {
    
    return UserDefault::getInstance()->getIntegerForKey(USER_DEFAULT_KEY_CLEAR_STAGE, 0);
}

void User::resetClearStage() {
    
    UserDefault::getInstance()->setIntegerForKey(USER_DEFAULT_KEY_CLEAR_STAGE, 0);
    UserDefault::getInstance()->flush();
}

/**
 * 리뷰 체크
 */
bool User::checkReview(float popupDelay) {
    
    if( !ReviewHelper::isReviewAlertEnabled() ) {
        return false;
    }
    
    auto showPopup = [=]() {
        SBAnalytics::logEvent(ANALYTICS_EVENT_REVIEW_POPUP);
        
        ReviewHelper::showReviewPopup([=]() {
            
            // 커스텀 팝업
            auto popup = ReviewPopup::create();
            popup->setOnGoListener([=]() {
                
                ReviewHelper::setReviewWrite(true);
                Application::getInstance()->openURL(GameConfiguration::getInstance()->getStoreUrl());
                
                popup->dismissWithAction();
            });
            SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
        });
    };
    
    if( popupDelay == 0 ) {
        showPopup();
    } else {
        SBDirector::postDelayed(SceneManager::getScene(), showPopup, popupDelay, true);
    }
    
    return true;
}

