//
//  StageSkipPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/04/13.
//

#include "StageSkipPopup.hpp"

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

StageSkipPopup::StageSkipPopup() : BasePopup(PopupType::STAGE_SKIP),
onSkipListener(nullptr) {
}

StageSkipPopup::~StageSkipPopup() {
}

bool StageSkipPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void StageSkipPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool StageSkipPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    dismissWithAction();
    
    return true;
}

void StageSkipPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void StageSkipPopup::initContentView() {
    
    BasePopup::initContentView();
    
    // game_skip_bg.png Vec2MC(0, 8) , Size(784, 552)
    popupBg = Sprite::create(DIR_IMG_GAME + "game_skip_bg.png");
    popupBg->setAnchorPoint(ANCHOR_M);
    popupBg->setPosition(Vec2MC(0, 8));
    addContentChild(popupBg);
    
    // SKIP 버튼
    // game_btn_skip.png Vec2MC(0, -236) , Size(456, 144)
    auto skipBtn = SBButton::create(DIR_IMG_GAME + "game_btn_skip.png");
    skipBtn->setZoomScale(ButtonZoomScale::NORMAL);
    skipBtn->setAnchorPoint(ANCHOR_M);
    skipBtn->setPosition(Vec2MC(0, -236));
    addContentChild(skipBtn);
    
    skipBtn->setOnClickListener([=](Node*) {
        // 광고 노출
        SBDirector::getInstance()->setScreenTouchLocked(true);
        
        auto listener = superbomb::RewardedVideoAdListener::create();
        listener->setTarget(this);
        listener->onAdOpened = [=]() {
            SBDirector::getInstance()->setScreenTouchLocked(false);
        };
        listener->onAdClosed = [=]() {
            this->retain();
            onSkipListener(listener->isRewarded());
            this->release();
        };
        superbomb::AdsHelper::getInstance()->showRewardedVideo(listener);
    });
}

/**
 * 등장 연출
 */
void StageSkipPopup::runEnterAction(SBCallback onFinished) {
    
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
void StageSkipPopup::runExitAction(SBCallback onFinished) {
    
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
void StageSkipPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
    
    // 배경 바깥 영역 터치 시 팝업 종료
    auto touchNode = SBNodeUtils::createTouchNode();
    addChild(touchNode);
    
    touchNode->addClickEventListener([=](Ref*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        
        this->retain();
        onSkipListener(false);
        this->release();
    });
    
    auto bgTouchNode = SBNodeUtils::createTouchNode();
    bgTouchNode->setAnchorPoint(popupBg->getAnchorPoint());
    bgTouchNode->setPosition(popupBg->getPosition());
    bgTouchNode->setContentSize(popupBg->getContentSize());
    addChild(bgTouchNode);
}
