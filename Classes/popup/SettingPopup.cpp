//
//  SettingPopup.cpp
//
//  Created by seongmin hwang on 2018. 7. 6..
//

#include "SettingPopup.hpp"

#include "Define.h"
#include "User.hpp"
#include "UserDefaultKey.h"
#include "GameConfiguration.hpp"
#include "SceneManager.h"

USING_NS_CC;
USING_NS_SB;
using namespace cocos2d::ui;
using namespace std;

static const float FADE_DURATION    = 0.15f;
static const float SLIDE_DURATION   = EffectDuration::POPUP_SLIDE_FAST;

SettingPopup::SettingPopup() : BasePopup(PopupType::SETTING),
onHomeListener(nullptr) {
    
}

SettingPopup::~SettingPopup() {
}

bool SettingPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void SettingPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool SettingPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    dismissWithAction();
    
    return true;
}

void SettingPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void SettingPopup::initContentView() {
    
    BasePopup::initContentView();
    
    const bool isGameScene = SceneManager::isGameScene();
    const bool isRemovedAds = User::isRemovedAds();
    
    // common_bg_setting.png Vec2MC(0, 0) , Size(542, 380)
    // game_bg_setting.png Vec2MC(0, 1) , Size(542, 488)
    popupBg = Sprite::create(DIR_IMG_COMMON + "common_bg_setting.png");
    popupBg->setAnchorPoint(ANCHOR_M);
    popupBg->setPosition(Vec2MC(0, 0));
    addContentChild(popupBg);
    
    // 효과음
    // common_btn_effect_on.png Vec2MC(-166, 85) , Size(154, 154)
    auto effectBtn = SBToggleButton::create(DIR_IMG_COMMON + "common_btn_effect_off.png",
                                            DIR_IMG_COMMON + "common_btn_effect_on.png");
    effectBtn->setZoomScale(ButtonZoomScale::NORMAL);
    effectBtn->setAnchorPoint(ANCHOR_M);
    effectBtn->setPosition(Vec2MC(-166, 85));
    addContentChild(effectBtn);
    
    auto audioEngine = SBAudioEngine::getInstance();
    
    effectBtn->setSelected(!audioEngine->isEffectMute());
    effectBtn->setOnSelectedListener([=](bool isSelected) -> bool {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        audioEngine->setEffectMute(!isSelected);
        
        return false;
    });
    
    // 배경음
    // common_btn_bgm_on.png Vec2MC(0, 85) , Size(154, 154)
    auto bgmBtn = SBToggleButton::create(DIR_IMG_COMMON + "common_btn_bgm_off.png",
                                         DIR_IMG_COMMON + "common_btn_bgm_on.png");
    bgmBtn->setZoomScale(ButtonZoomScale::NORMAL);
    bgmBtn->setAnchorPoint(ANCHOR_M);
    bgmBtn->setPosition(Vec2MC(0, 85));
    addContentChild(bgmBtn);
    
    bgmBtn->setSelected(!audioEngine->isBGMMute());
    bgmBtn->setOnSelectedListener([=](bool isSelected) -> bool {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        audioEngine->setBGMMute(!isSelected);
        
        return false;
    });
    
    // how to play
    // common_btn_how_to_play.png Vec2MC(166, 85) , Size(154, 154)
    auto howToPlayBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_how_to_play.png");
    howToPlayBtn->setZoomScale(ButtonZoomScale::NORMAL);
    howToPlayBtn->setAnchorPoint(ANCHOR_M);
    howToPlayBtn->setPosition(Vec2MC(166, 85));
    addContentChild(howToPlayBtn);
    
    howToPlayBtn->setOnClickListener([=](Node*) {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        
        this->dismiss();
    });
    
    // more games
    // common_btn_3eries.png Vec2MC(0, -85) , Size(154, 154)
    // common_btn_3eries.png Vec2MC(-83, -85) , Size(154, 154)
    auto moreGames = SBButton::create(DIR_IMG_COMMON + "common_btn_more_games.png");
    moreGames->setZoomScale(0);
    moreGames->setAnchorPoint(ANCHOR_M);
    moreGames->setPosition(isRemovedAds ? Vec2MC(-83, -85) : Vec2MC(0, -85));
    addContentChild(moreGames);
    
    moreGames->setOnClickListener([=](Node*) {
        SBAnalytics::logEvent(ANALYTICS_EVENT_MORE_GAMES);
        Application::getInstance()->openURL(GAME_CONFIG->getMoreGamesUrl());
    });
    
    // remove ads
    SBButton *removeAdsBtn = nullptr;
    
    if( !isRemovedAds ) {
        removeAdsBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_remove_ads.png");
        removeAdsBtn->setZoomScale(ButtonZoomScale::NORMAL);
        removeAdsBtn->setAnchorPoint(ANCHOR_M);
        removeAdsBtn->setPosition(Vec2MC(-166, -85));
        addContentChild(removeAdsBtn);
        
        removeAdsBtn->setOnClickListener([=](Node*) {
//            SBDirector::getInstance()->setScreenTouchLocked(true);
//
//            auto listener = iap::PurchaseListener::create();
//            listener->setTarget(this);
//            listener->onPurchased = [=](const iap::Item &item) {
//                User::removeAds();
//                this->dismissWithAction();
//            };
//            
//            listener->onFinished = [=](bool result) {
//                SBDirector::postDelayed(this, [=]() {
//                    SBDirector::getInstance()->setScreenTouchLocked(false);
//                }, 0.2f);
//            };
//
//            iap::IAPHelper::purchaseRemoveAds(listener);
        });
    }
    
    // restore purchases
    // common_btn_restore.png Vec2MC(166, -85) , Size(154, 154)
    // common_btn_restore.png Vec2MC(83, -85) , Size(154, 154)
    auto restoreBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_restore.png");
    restoreBtn->setZoomScale(ButtonZoomScale::NORMAL);
    restoreBtn->setAnchorPoint(ANCHOR_M);
    restoreBtn->setPosition(isRemovedAds ? Vec2MC(83, -85) : Vec2MC(166, -85));
    addContentChild(restoreBtn);
    
    restoreBtn->setOnClickListener([=](Node*) {
        if( iap::IAPHelper::isReady() ) {
            iap::IAPHelper::restore(nullptr);
        }
        
        this->dismissWithAction();
    });
    
    // 게임씬인 경우 UI 변경
    if( isGameScene ) {
        popupBg->setTexture(DIR_IMG_GAME + "game_bg_setting.png");
        
        auto homeBtn = SBButton::create(DIR_IMG_GAME + "game_btn_home.png");
        homeBtn->setZoomScale(ButtonZoomScale::NORMAL);
        homeBtn->setAnchorPoint(ANCHOR_M);
        homeBtn->setPosition(Vec2MC(0, 171));
        addContentChild(homeBtn);
        
        homeBtn->setOnClickListener([=](Node*) {
            SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
            onHomeListener();
        });
        
        effectBtn->setPosition(Vec2MC(-166, 31));
        bgmBtn->setPosition(Vec2MC(0, 31));
        howToPlayBtn->setPosition(Vec2MC(166, 31));
        moreGames->setPosition(isRemovedAds ? Vec2MC(-83, -139) : Vec2MC(0, -139));
        restoreBtn->setPosition(isRemovedAds ? Vec2MC(83, -139) : Vec2MC(166, -139));
        
        if( removeAdsBtn ) {
            removeAdsBtn->setPosition(Vec2MC(-166, -139));
        }
    }
}

/**
 * 등장 연출
 */
void SettingPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(SLIDE_DURATION, onFinished);
    
    // 배경 fade in
    runBackgroundFadeInAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide in
    runSlideInAction([=]() {
        
        this->onEnterActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, SLIDE_DURATION);
}

/**
 * 퇴장 연출
 */
void SettingPopup::runExitAction(SBCallback onFinished) {
    
    BasePopup::runExitAction(SLIDE_DURATION, onFinished);
    
    // 배경 fade out
    runBackgroundFadeOutAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide out
    runSlideOutAction([=]() {
        
        this->onExitActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, SLIDE_DURATION);
}

/**
 * 등장 연출 완료
 */
void SettingPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
    
    // 배경 바깥 영역 터치 시 팝업 종료
    auto touchNode = SBNodeUtils::createTouchNode();
    addChild(touchNode);
    
    touchNode->addClickEventListener([=](Ref*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        this->dismissWithAction();
    });
    
    auto bgTouchNode = SBNodeUtils::createTouchNode();
    bgTouchNode->setAnchorPoint(popupBg->getAnchorPoint());
    bgTouchNode->setPosition(popupBg->getPosition());
    bgTouchNode->setContentSize(popupBg->getContentSize());
    addChild(bgTouchNode);
}
