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

#define CELL_SIZE       Size(384, 736)

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
    
    auto bg = Sprite::create(DIR_IMG_SHOP + "shop_column_deal.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(getContentSize(), 0, 0));
    addChild(bg);
    
    auto tag = Sprite::create(DIR_IMG_SHOP + "shop_column_deal_tag.png");
    tag->setAnchorPoint(ANCHOR_M);
    tag->setPosition(Vec2TC(getContentSize(), 0, 20));
    addChild(tag);
    
    // BUY 버튼
    auto buyBtn = SBButton::create(DIR_IMG_SHOP + "shop_btn_buy.png");
    buyBtn->setZoomScale(0);
    buyBtn->setAnchorPoint(ANCHOR_M);
    buyBtn->setPosition(Vec2BC(getContentSize(), 0, 100));
    addChild(buyBtn);
    
    buyBtn->setOnClickListener([=](Node*) {
        SB_SAFE_PERFORM_LISTENER(this, onClickListener);
    });
    
    return true;
}
