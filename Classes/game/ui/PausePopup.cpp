//
//  PausePopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/15.
//

#include "PausePopup.hpp"

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

PausePopup::PausePopup() : BasePopup(PopupType::PAUSE),
onHomeListener(nullptr),
onRetryListener(nullptr) {
    
}

PausePopup::~PausePopup() {
}

bool PausePopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void PausePopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool PausePopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    dismissWithAction();
    
    return true;
}

void PausePopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void PausePopup::initContentView() {
    
    BasePopup::initContentView();
    
    popupBg = Sprite::create(DIR_IMG_COMMON + "common_bg_pause.png");
    popupBg->setAnchorPoint(ANCHOR_M);
    popupBg->setPosition(Vec2MC(0, 2));
    addContentChild(popupBg);
    
    // 효과음
    auto effectBtn = SBToggleButton::create(DIR_IMG_COMMON + "common_btn_effect_off.png",
                                            DIR_IMG_COMMON + "common_btn_effect_on.png");
    effectBtn->setZoomScale(ButtonZoomScale::NORMAL);
    effectBtn->setAnchorPoint(ANCHOR_M);
    effectBtn->setPosition(Vec2MC(-192, 44));
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
    bgmBtn->setPosition(Vec2MC(0, 44));
    addContentChild(bgmBtn);
    
    bgmBtn->setSelected(!audioEngine->isBGMMute());
    bgmBtn->setOnSelectedListener([=](bool isSelected) -> bool {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        audioEngine->setBGMMute(!isSelected);
        
        return false;
    });
    
    auto homeBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_home.png");
    homeBtn->setZoomScale(ButtonZoomScale::NORMAL);
    homeBtn->setAnchorPoint(ANCHOR_M);
    homeBtn->setPosition(Vec2MC(192, 44));
    addContentChild(homeBtn);
    
    homeBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        onHomeListener();
    });
    
    auto retryBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_retry.png");
    retryBtn->setZoomScale(ButtonZoomScale::NORMAL);
    retryBtn->setAnchorPoint(ANCHOR_M);
    retryBtn->setPosition(Vec2MC(0, -114));
    addContentChild(retryBtn);
    
    retryBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        onRetryListener();
    });
}

/**
 * 등장 연출
 */
void PausePopup::runEnterAction(SBCallback onFinished) {
    
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
void PausePopup::runExitAction(SBCallback onFinished) {
    
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
void PausePopup::onEnterActionFinished() {
    
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
