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

static const float FADE_DURATION    = 0.1f;
static const float SLIDE_DURATION   = EffectDuration::POPUP_SLIDE_FAST;

SettingPopup::SettingPopup() : BasePopup(PopupType::SETTING) {
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
    
    popupBg = Sprite::create(DIR_IMG_COMMON + "common_bg_settings.png");
    popupBg->setAnchorPoint(ANCHOR_M);
    popupBg->setPosition(Vec2MC(0, 17));
    addContentChild(popupBg);
    
    // 효과음
    auto effectBtn = SBToggleButton::create(DIR_IMG_COMMON + "common_btn_effect_off.png",
                                            DIR_IMG_COMMON + "common_btn_effect_on.png");
    effectBtn->setZoomScale(ButtonZoomScale::NORMAL);
    effectBtn->setAnchorPoint(ANCHOR_M);
    effectBtn->setPosition(Vec2MC(-192, 8));
    addContentChild(effectBtn);
    
    auto audioEngine = SBAudioEngine::getInstance();
    
    effectBtn->setSelected(!audioEngine->isEffectMute());
    effectBtn->setOnSelectedListener([=](bool isSelected) -> bool {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        audioEngine->setEffectMute(!isSelected);
        
        return false;
    });
    
    // 배경음
    auto bgmBtn = SBToggleButton::create(DIR_IMG_COMMON + "common_btn_bgm_off.png",
                                         DIR_IMG_COMMON + "common_btn_bgm_on.png");
    bgmBtn->setZoomScale(ButtonZoomScale::NORMAL);
    bgmBtn->setAnchorPoint(ANCHOR_M);
    bgmBtn->setPosition(Vec2MC(0, 8));
    addContentChild(bgmBtn);
    
    bgmBtn->setSelected(!audioEngine->isBGMMute());
    bgmBtn->setOnSelectedListener([=](bool isSelected) -> bool {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        audioEngine->setBGMMute(!isSelected);
        
        return false;
    });

    // restore purchases
    auto restoreBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_restore.png");
    restoreBtn->setZoomScale(ButtonZoomScale::NORMAL);
    restoreBtn->setAnchorPoint(ANCHOR_M);
    restoreBtn->setPosition(Vec2MC(192, 8));
    addContentChild(restoreBtn);

    restoreBtn->setOnClickListener([=](Node*) {
        if( iap::IAPHelper::isReady() ) {
            iap::IAPHelper::restore(nullptr);
        }

        this->dismissWithAction();
    });
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
