//
//  CharacterCell.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#include "CharacterCell.hpp"

#include "Define.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

#define CELL_SIZE       Size(400,700)

CharacterCell* CharacterCell::create() {
    
    auto cell = new CharacterCell();
    
    if( cell && cell->init() ) {
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return cell;
}

CharacterCell::CharacterCell():
onClickListener(nullptr) {
}

CharacterCell::~CharacterCell() {
}

bool CharacterCell::init() {
    
    if( !Widget::init() ) {
        return false;
    }
    
    setContentSize(CELL_SIZE);
    
    auto bg = LayerColor::create(Color4B(0,100,0,255*0.9f));
    bg->setIgnoreAnchorPointForPosition(false);
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(getContentSize(), 0, 0));
    bg->setContentSize(getContentSize());
    addChild(bg);
    
    // 이름
    auto name = Label::createWithTTF("NAME", FONT_ROBOTO_BLACK, 80, Size::ZERO,
                                     TextHAlignment::CENTER, TextVAlignment::CENTER);
    name->setTextColor(Color4B::WHITE);
    name->setAnchorPoint(ANCHOR_M);
    name->setPosition(Vec2MC(getContentSize(), 0, 250));
    addChild(name);
    
    // 획득 조건 - 이미지
    
    // 획득 조건 - 텍스트
    
    // 캐릭터 이미지
    // TODO: EffectSprite - Grayscale
    
    // 하단 버튼
    // SELECTED | SELECT | VIDEO |
    auto bottomBtn = SBButton::create(DIR_IMG_COMMON + "shop_btn_coin_purchase.png");
    bottomBtn->setZoomScale(ButtonZoomScale::NORMAL);
    bottomBtn->setAnchorPoint(ANCHOR_M);
    bottomBtn->setPosition(Vec2BC(getContentSize(), 0, 90));
    addChild(bottomBtn);
    
    bottomBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onClickListener);
    });
    
    return true;
}

