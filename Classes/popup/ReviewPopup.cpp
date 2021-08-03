//
//  ReviewPopup.cpp
//
//  Created by seongmin hwang on 2018. 8. 7..
//

#include "ReviewPopup.hpp"

#include "Define.h"
#include "SceneManager.h"
#include "GameUIHelper.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

static const float FADE_DURATION    = 0.15f;
static const float SLIDE_DURATION   = EffectDuration::POPUP_SLIDE_FAST;

ReviewPopup::ReviewPopup() : BasePopup(PopupType::REVIEW),
onGoListener(nullptr) {
    
}

ReviewPopup::~ReviewPopup() {
}

bool ReviewPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void ReviewPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

void ReviewPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void ReviewPopup::initContentView() {
    
    BasePopup::initContentView();
    
    // RSP_popup_bg_rate_us.png Vec2MC(0, 32) , Size(488, 472)
    stoneBg = Sprite::create(DIR_IMG_GAME + "RSP_popup_bg_rate_us.png");
    stoneBg->setAnchorPoint(ANCHOR_M);
    stoneBg->setPosition(Vec2MC(0, 32));
    addContentChild(stoneBg);
    
    const Size SIZE(stoneBg->getContentSize());
    
    // title
    auto title = Sprite::create(DIR_IMG_GAME + "RSP_popup_title_rate_us.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2MC(SIZE, 0, 166));
    stoneBg->addChild(title);
    
    // desc
    auto desc = Label::createWithTTF("YOUR REVIEW\nWILL BE A\nPAGE IN HISTORY", FONT_RETRO, 36 - 3,
                                     Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    desc->setTextColor(Color4B::WHITE);
    desc->enableShadow(Color4B(0,0,0,255*0.6f), Size(0, -4));
    desc->setAnchorPoint(ANCHOR_M);
    desc->setPosition(Vec2MC(SIZE, 0, 24 + 6));
    stoneBg->addChild(desc);
    
    // go
    // btn_go.png Vec2MC(0, -144) , Size(240, 120)
    auto goBtn = SBButton::create(DIR_IMG_GAME + "btn_go.png");
    goBtn->setAnchorPoint(ANCHOR_M);
    goBtn->setPosition(Vec2MC(SIZE, 0, -144));
    stoneBg->addChild(goBtn);
    
    goBtn->setOnClickListener([=](Node*) {
        this->retain();
        onGoListener();
        this->release();
    });
}

/**
 * 등장 연출
 */
void ReviewPopup::runEnterAction(float duration, SBCallback onFinished) {
    
    BasePopup::runEnterAction(duration, onFinished);
    
    // 배경 fade in
    runBackgroundFadeInAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide in
    runSlideInAction([=]() {
        
        this->onEnterActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, duration);
}

void ReviewPopup::runEnterAction(SBCallback onFinished) {
    
    runEnterAction(SLIDE_DURATION, onFinished);
}

/**
 * 퇴장 연출
 */
void ReviewPopup::runExitAction(float duration, SBCallback onFinished) {
    
    BasePopup::runExitAction(duration, onFinished);
    
    // 배경 fade out
    runBackgroundFadeOutAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide out
    runSlideOutAction([=]() {
        
        this->onExitActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, duration);
}

void ReviewPopup::runExitAction(SBCallback onFinished) {
    
    runExitAction(SLIDE_DURATION, onFinished);
}

/**
 * 등장 연출 완료
 */
void ReviewPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
    
    // 비석 바깥 영역 터치 시 팝업 종료
    /*
    auto touchNode = SBNodeUtils::createTouchNode();
    addChild(touchNode);
    
    touchNode->addClickEventListener([=](Ref*) {
        this->dismissWithAction();
    });
    
    auto stoneTouchNode = SBNodeUtils::createTouchNode();
    stoneTouchNode->setAnchorPoint(stoneBg->getAnchorPoint());
    stoneTouchNode->setPosition(stoneBg->getPosition());
    stoneTouchNode->setContentSize(stoneBg->getContentSize());
    addChild(stoneTouchNode);
     */
}

