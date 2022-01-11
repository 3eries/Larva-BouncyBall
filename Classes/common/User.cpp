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
#define USER_DEFAULT_KEY_LATEST_PLAY_STAGE              "USER_LATEST_PLAY_STAGE"
#define USER_DEFAULT_KEY_TOP_UNLOCKED_STAGE             "USER_TOP_UNLOCKED_STAGE"

// #define USER_DEFAULT_KEY_STAGE_STAR                     "USER_STAGE_STAR"
#define USER_DEFAULT_KEY_STAGE_STAR(__STAGE__) \
STR_FORMAT("USER_STAGE_%d_STAR", __STAGE__).c_str()


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
    
    // 1스테이지 잠금 해제
    unlockStage(1);
    
    CCLOG("User {");
    CCLOG("\t coin: %d", getCoin());
    CCLOG("\t latest play stage: %d", getLatestPlayStage());
    CCLOG("\t top unlocked stage: %d", getTopUnlockedStage());
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
    
    USER_DEFAULT->setBoolForKey(USER_DEFAULT_KEY_REMOVE_ADS, isRemoveAds);
    USER_DEFAULT->flush();
    
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
    
    return USER_DEFAULT->getBoolForKey(USER_DEFAULT_KEY_REMOVE_ADS, false);
}

/**
 * 힌트 개수를 설정합니다
 */
void User::setHintCount(int i) {
    
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_HINT, i);
    USER_DEFAULT->flush();
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(DIRECTOR_EVENT_UPDATE_HINT_COUNT);
}

/**
 * 힌트 개수를 반환합니다
 */
int User::getHintCount() {
    
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_HINT, GAME_CONFIG->getFirstHint());
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
    
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_COIN, i);
    USER_DEFAULT->flush();
    
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
    
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_COIN, GAME_CONFIG->getFirstCoin());
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
    
    USER_DEFAULT->setBoolForKey(USER_DEFAULT_KEY_TUTORIAL_COMPLETED, isCompleted);
    USER_DEFAULT->flush();
}

/**
 * 튜토리얼 완료 여부를 반환합니다
 */
bool User::isTutorialCompleted() {
    
    return USER_DEFAULT->getBoolForKey(USER_DEFAULT_KEY_TUTORIAL_COMPLETED, false);
}

/**
 * 스테이지 별 개수를 반환합니다
 */
#define INVALID_STAR_COUNT      -1

int User::getStageStarCount(int stage) {
 
    // use json
    /*
    auto json = USER_DEFAULT->getStringForKey(USER_DEFAULT_KEY_STAGE_STAR, "");
    
    if( json == "" ) {
        return INVALID_STAR_COUNT;
    }
    
    auto doc = SBJSON::parse(json);
    auto stageKey = TO_STRING(stage);
    
    if( doc.FindMember(stageKey.c_str()) == doc.MemberEnd() ) {
        return INVALID_STAR_COUNT;
    }
    
    return doc[stageKey.c_str()].GetInt();
     */
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_STAGE_STAR(stage), INVALID_STAR_COUNT);
}

/**
 * 스테이지 별 개수를 설정합니다
 */
void User::setStageStarCount(int stage, int star) {
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_STAGE_STAR(stage), star);
    USER_DEFAULT->flush();
}

/**
 * 스테이지를 잠금 해제합니다
 */
void User::unlockStage(int stage) {
    setStageStarCount(stage, MAX(0, getStageStarCount(stage)));
    setTopUnlockedStage(stage);
}

bool User::isStageLocked(int stage) {
    return getStageStarCount(stage) == INVALID_STAR_COUNT;
}

/**
 * 마지막으로 플레이한 스테이지를 반환합니다.
 */
int User::getLatestPlayStage() {
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_LATEST_PLAY_STAGE, getTopUnlockedStage());
}

void User::setLatestPlayStage(int stage) {
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_LATEST_PLAY_STAGE, stage);
    USER_DEFAULT->flush();
}

/**
 * 잠금 해제된 최고 스테이지를 반환합니다.
 */
int User::getTopUnlockedStage() {
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_TOP_UNLOCKED_STAGE, 1);
}

void User::setTopUnlockedStage(int stage) {
    stage = MAX(getTopUnlockedStage(), stage);
    
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_TOP_UNLOCKED_STAGE, stage);
    USER_DEFAULT->flush();
}
