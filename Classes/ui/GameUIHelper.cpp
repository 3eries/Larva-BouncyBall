//
//  GameUIHelper.cpp
//
//  Created by seongmin hwang on 2018. 5. 14..
//

#include "GameUIHelper.hpp"

#include "Define.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

// 버튼 정의
const Size ButtonSize::SMALL              = Size(200, 80);
const Size ButtonSize::MEDIUM             = Size(250, 80);
const Size ButtonSize::LARGE              = Size(300, 80);

/**
 * 기본 버튼 생성
 */
SBButton* GameUIHelper::createButton(SBButton::Config config) {
    
    auto btn = SBButton::create(config);
    btn->setZoomScale(ButtonZoomScale::NORMAL);
    return btn;
}

/**
 * 폰트 버튼 생성
 */
SBButton* GameUIHelper::createFontButton(const string &title, const Size &size) {
    
    auto btn = createButton(SBButton::Config("", size, title, FONT_RETRO, 50));
    
    auto titleLabel = btn->getTitle();
    titleLabel->setColor(Color3B::WHITE);
    titleLabel->enableOutline(Color4B::BLACK, 3);
    
    return btn;
}

/**
 * VIP 마크 생성
 */
Sprite* GameUIHelper::createVIPMark(Node *removeAdsBtn) {
    
    auto vip = Sprite::create(DIR_IMG_GAME + "RSP_btn_vip_version.png");
    vip->setAnchorPoint(removeAdsBtn->getAnchorPoint());
    vip->setPosition(removeAdsBtn->getPosition());
    removeAdsBtn->getParent()->addChild(vip, removeAdsBtn->getLocalZOrder()+1);
    
    return vip;
}

/**
 * 썸네일 테두리를 생성합니다
 */
Node* GameUIHelper::createThumbnailBorder(const Size &originSize,
                                          int whiteBorder, int blackBorder) {
    
    auto border = Node::create();
    border->setCascadeOpacityEnabled(true);
    border->setAnchorPoint(ANCHOR_M);
    border->setPosition(Vec2MC(originSize, 0, 0));
    border->setContentSize(originSize);
    
    if( whiteBorder > 0 ) {
        auto whiteFrame = LayerColor::create(Color4B::WHITE);
        whiteFrame->setIgnoreAnchorPointForPosition(false);
        whiteFrame->setAnchorPoint(ANCHOR_M);
        whiteFrame->setPosition(Vec2MC(originSize, 0, 0));
        whiteFrame->setContentSize(Size(originSize.width + (whiteBorder*2),
                                        originSize.height + (whiteBorder*2)));
        border->addChild(whiteFrame, whiteBorder > blackBorder ? -1 : 1);
    }
    
    if( blackBorder > 0 ) {
        auto blackFrame = LayerColor::create(Color4B::BLACK);
        blackFrame->setIgnoreAnchorPointForPosition(false);
        blackFrame->setAnchorPoint(ANCHOR_M);
        blackFrame->setPosition(Vec2MC(originSize, 0, 0));
        blackFrame->setContentSize(Size(originSize.width + (blackBorder*2),
                                        originSize.height + (blackBorder*2)));
        border->addChild(blackFrame);
    }
    
    return border;
}
