//
//  GetCharacterPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/25.
//

#include "GetCharacterPopup.hpp"

#include "Define.h"

USING_NS_CC;
using namespace std;

GetCharacterPopup* GetCharacterPopup::create(const CharacterData &data) {
    
    auto popup = new GetCharacterPopup(data);
    
    if( popup && popup->init() ) {
        popup->autorelease();
        return popup;
    }
    
    CC_SAFE_DELETE(popup);
    return nullptr;
}

GetCharacterPopup::GetCharacterPopup(const CharacterData &data): BasePopup(PopupType::GET_CHARACTER),
data(data) {
}

GetCharacterPopup::~GetCharacterPopup() {
    
}

bool GetCharacterPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void GetCharacterPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool GetCharacterPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    // 반응 없음
    
    return true;
}

void GetCharacterPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void GetCharacterPopup::initContentView() {
    
    BasePopup::initContentView();
    
    // 타이틀
    auto title = Label::createWithTTF("CHARACTER", FONT_ROBOTO_BLACK, 100, Size::ZERO,
                                      TextHAlignment::CENTER, TextVAlignment::TOP);
    title->setTextColor(Color4B::WHITE);
    title->setAnchorPoint(ANCHOR_MT);
    title->setPosition(Vec2TC(0, -100));
    addContentChild(title);
}

/**
 * 등장 연출
 */
void GetCharacterPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(0/* ACTION DURATION */, onFinished);
}

/**
 * 등장 연출 완료
 */
void GetCharacterPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
}

