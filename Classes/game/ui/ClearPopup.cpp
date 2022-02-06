//
//  ClearPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#include "ClearPopup.hpp"

#include "Define.h"
#include "GameUIHelper.hpp"

USING_NS_CC;
using namespace std;

ClearPopup::ClearPopup(): BasePopup(PopupType::GAME_CLEAR),
onHomeListener(nullptr),
onRetryListener(nullptr),
onNextListener(nullptr) {
}

ClearPopup::~ClearPopup() {
    
}

bool ClearPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void ClearPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool ClearPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    // 반응 없음
    
    return true;
}

void ClearPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color4B(0,0,0,255*0.9f));
}

void ClearPopup::initContentView() {
    
    BasePopup::initContentView();
    
    // 타이틀
    auto title = Label::createWithTTF("GAME CLEAR", FONT_ROBOTO_BLACK, 100, Size::ZERO,
                                      TextHAlignment::CENTER, TextVAlignment::TOP);
    title->setTextColor(Color4B::WHITE);
    title->setAnchorPoint(ANCHOR_MT);
    title->setPosition(Vec2TC(0, -100));
    addContentChild(title);
    
    // 별
    const int STAR = GAME_MANAGER->getStar();
    
    Vec2 pos[] = {
        Vec2MC(-360, 20),
        Vec2MC(0, 20),
        Vec2MC(360, 20),
    };
    
    for( int i = 0; i < STAR; ++i ) {
        auto star = Sprite::create(DIR_IMG_GAME + "game_ui_sausage.png");
        star->setScale(2);
        star->setAnchorPoint(ANCHOR_M);
        star->setPosition(pos[i]);
        addContentChild(star);
    }
    
    // Home
    auto homeBtn = GameUIHelper::createFontButton("HOME", Size(400,300));
    homeBtn->setAnchorPoint(ANCHOR_MB);
    homeBtn->setPosition(Vec2BC(-500, 40));
    addChild(homeBtn);
    
    homeBtn->setOnClickListener([=](Node*) {
        SB_SAFE_PERFORM_LISTENER(this, onHomeListener);
    });
    
    // Retry
    auto retryBtn = GameUIHelper::createFontButton("RETRY", Size(400,300));
    retryBtn->setAnchorPoint(ANCHOR_MB);
    retryBtn->setPosition(Vec2BC(0, 40));
    addChild(retryBtn);
    
    retryBtn->setOnClickListener([=](Node*) {
        SB_SAFE_PERFORM_LISTENER(this, onRetryListener);
    });
    
    // Next Stage
    auto nextBtn = GameUIHelper::createFontButton("NEXT", Size(400,300));
    nextBtn->setAnchorPoint(ANCHOR_MB);
    nextBtn->setPosition(Vec2BC(500, 40));
    addChild(nextBtn);
    
    nextBtn->setOnClickListener([=](Node*) {
        SB_SAFE_PERFORM_LISTENER(this, onNextListener);
    });
    
    // Shop
}

/**
 * 등장 연출
 */
void ClearPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(0/* ACTION DURATION */, onFinished);
}

/**
 * 등장 연출 완료
 */
void ClearPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
}


