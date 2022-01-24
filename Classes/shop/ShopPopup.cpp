//
//  ShopPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#include "ShopPopup.hpp"

#include "Define.h"
#include "GameUIHelper.hpp"

#include "IAPCell.hpp"
#include "CharacterCell.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

#define CHARACTER_LIST_SIZE_1               Size(1300,700)   // Cell 3개
#define CHARACTER_LIST_SIZE_2               Size(1750,700)   // Cell 4개
#define CHARACTER_LIST_CELL_SIZE            Size(400,700)
#define CHARACTER_LIST_CELL_PADDING         50

ShopPopup::ShopPopup(): BasePopup(PopupType::SHOP) {
}

ShopPopup::~ShopPopup() {
    
}

bool ShopPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void ShopPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool ShopPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    dismissWithAction();
    
    return true;
}

void ShopPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color4B(0,0,0,255*0.9f));
}

void ShopPopup::initContentView() {
    
    BasePopup::initContentView();
    
    // 타이틀
    auto title = Label::createWithTTF("SHOP", FONT_ROBOTO_BLACK, 120, Size::ZERO,
                                      TextHAlignment::CENTER, TextVAlignment::TOP);
    title->setTextColor(Color4B::WHITE);
    title->setAnchorPoint(ANCHOR_MT);
    title->setPosition(Vec2TC(0, -30));
    addContentChild(title);
    
    // 닫기 버튼
    auto closeBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_close.png");
    closeBtn->setAnchorPoint(ANCHOR_M);
    closeBtn->setPosition(Vec2TR(-80, -100));
    addContentChild(closeBtn);
    
    closeBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        this->dismissWithAction();
    });
    
    // IAP Cell
    // 가운데 정렬을 위한 좌표 계산
    float firstCellX = (SB_WIN_SIZE.width - CHARACTER_LIST_SIZE_2.width) / 2;
    
    auto iapCell = IAPCell::create();
    iapCell->setAnchorPoint(ANCHOR_ML);
    iapCell->setPosition(Vec2ML(firstCellX, -70));
    addContentChild(iapCell);

    iapCell->setOnClickListener([=]() {
        // TODO: IAP
        CCLOG("상품 구매");
    });
    
    // ScrollView - CharacterList
    Vec2 pos;
    pos.x = SBNodeUtils::getBoundingBoxInWorld(iapCell).getMaxX() + CHARACTER_LIST_CELL_PADDING;
    pos.y = SB_WIN_SIZE.height*0.5f - 70;
    
    auto characterListView = ListView::create();
    characterListView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    // characterListView->setPadding(50, 0, 0, 0);
    characterListView->setGravity(ListView::Gravity::CENTER_VERTICAL);
    characterListView->setItemsMargin(CHARACTER_LIST_CELL_PADDING);
    // characterListView->setBounceEnabled(true);
    // characterListView->setScrollBarEnabled(false);
    // characterListView->setAnchorPoint(ANCHOR_M);
    // characterListView->setPosition(Vec2MC(0, -70));
    characterListView->setAnchorPoint(ANCHOR_ML);
    characterListView->setPosition(pos);
    characterListView->setContentSize(CHARACTER_LIST_SIZE_1);
    characterListView->addEventListener([=](Ref*, ScrollView::EventType eventType) {
      
        if( eventType == ScrollView::EventType::AUTOSCROLL_ENDED ) {
            // CCLOG("AUTOSCROLL_ENDED");
            // characterListView->setBounceEnabled(true);
        }
    });
    
    addContentChild(characterListView);
    
    for( int i = 0; i < 8; ++i ) {
        auto cell = CharacterCell::create();
        cell->setAnchorPoint(ANCHOR_M);
        characterListView->pushBackCustomItem(cell);
        
        cell->setOnClickListener([=]() {
            CCLOG("Cell: %d", i);
        });
    }
    
    // 리스트 영역 확인용
//    auto n = LayerColor::create(Color4B(255,0,0,255*0.3f));
//    n->setIgnoreAnchorPointForPosition(false);
//    n->setAnchorPoint(characterListView->getAnchorPoint());
//    n->setPosition(characterListView->getPosition());
//    n->setContentSize(characterListView->getContentSize());
//    addContentChild(n);
}

/**
 * 등장 연출
 */
void ShopPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(0/* ACTION DURATION */, onFinished);
}

/**
 * 등장 연출 완료
 */
void ShopPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
}
