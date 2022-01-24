//
//  IAPCell.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#include "IAPCell.hpp"

#include "Define.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

#define CELL_SIZE       Size(400,700)

IAPCell* IAPCell::create() {
    
    auto cell = new IAPCell();
    
    if( cell && cell->init() ) {
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return cell;
}

IAPCell::IAPCell():
onClickListener(nullptr) {
}

IAPCell::~IAPCell() {
}

bool IAPCell::init() {
    
    if( !Widget::init() ) {
        return false;
    }
    
    setContentSize(CELL_SIZE);
    
    auto bg = LayerColor::create(Color4B(100,0,0,255*0.9f));
    bg->setIgnoreAnchorPointForPosition(false);
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(getContentSize(), 0, 0));
    bg->setContentSize(getContentSize());
    addChild(bg);
    
    // BUY 버튼
    auto buyBtn = SBButton::create(DIR_IMG_COMMON + "shop_btn_coin_purchase.png");
    buyBtn->setZoomScale(0);
    buyBtn->setAnchorPoint(ANCHOR_M);
    buyBtn->setPosition(Vec2BC(getContentSize(), 0, 90));
    addChild(buyBtn);
    
    buyBtn->setOnClickListener([=](Node*) {
        SB_SAFE_PERFORM_LISTENER(this, onClickListener);
    });
    
    return true;
}
