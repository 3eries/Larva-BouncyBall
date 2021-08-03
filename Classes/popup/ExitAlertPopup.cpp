//
//  ExitAlertPopup.cpp
//
//  Created by seongmin hwang on 2018. 7. 12..
//

#include "ExitAlertPopup.hpp"

#include "Define.h"
#include "SceneManager.h"
#include "ResourceHelper.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

static const float FADE_DURATION    = 0.15f;
static const float SLIDE_DURATION   = EffectDuration::POPUP_SLIDE_FAST;

ExitAlertPopup::ExitAlertPopup() : BasePopup(PopupType::EXIT_APP),
onExitAppListener(nullptr) {
    
}

ExitAlertPopup::~ExitAlertPopup() {
}

bool ExitAlertPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void ExitAlertPopup::onEnter() {
    
    BasePopup::onEnter();
}

bool ExitAlertPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    dismissWithAction();
    
    return true;
}

void ExitAlertPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void ExitAlertPopup::initContentView() {
    
    BasePopup::initContentView();
    
    // bg
    popupBg = Sprite::create(DIR_IMG_COMMON + "common_bg_exit.png");
    popupBg->setAnchorPoint(ANCHOR_M);
    popupBg->setPosition(Vec2MC(0, 0));
    addContentChild(popupBg);
    
    // YES
    // common_btn_yes.png Vec2MC(-88, -76) , Size(164, 98)
    auto yesBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_yes.png");
    yesBtn->setZoomScale(ButtonZoomScale::NORMAL);
    yesBtn->setAnchorPoint(ANCHOR_M);
    yesBtn->setPosition(Vec2MC(-88, -76));
    addContentChild(yesBtn);
    
    yesBtn->setOnClickListener([=](Node*) {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onExitAppListener);
    });
    
    // NO
    // common_btn_nope.png Vec2MC(88, -76) , Size(164, 98)
    auto noBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_nope.png");
    noBtn->setZoomScale(ButtonZoomScale::NORMAL);
    noBtn->setAnchorPoint(ANCHOR_M);
    noBtn->setPosition(Vec2MC(88, -76));
    addContentChild(noBtn);
    
    noBtn->setOnClickListener([=](Node*) {
        
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        this->dismissWithAction();
    });
}

/**
 * 등장 연출
 */
void ExitAlertPopup::runEnterAction(float duration, SBCallback onFinished) {
    
    BasePopup::runEnterAction(duration, onFinished);
    
    // 배경 fade in
    runBackgroundFadeInAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide in
    runSlideInAction([=]() {
        
        this->onEnterActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, duration);
}

void ExitAlertPopup::runEnterAction(SBCallback onFinished) {
    
    runEnterAction(SLIDE_DURATION, onFinished);
}

/**
 * 퇴장 연출
 */
void ExitAlertPopup::runExitAction(float duration, SBCallback onFinished) {
    
    BasePopup::runExitAction(duration, onFinished);
    
    // 배경 fade out
    runBackgroundFadeOutAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide out
    runSlideOutAction([=]() {
        
        this->onExitActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, duration);
}

void ExitAlertPopup::runExitAction(SBCallback onFinished) {
    
    runExitAction(SLIDE_DURATION, onFinished);
}

/**
 * 등장 연출 완료
 */
void ExitAlertPopup::onEnterActionFinished() {
    
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

