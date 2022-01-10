//
//  WorldPage.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/10.
//

#include "WorldPage.hpp"

#include "Define.h"

USING_NS_CC;
using namespace std;

#define STAGE_COUNT     18
#define CELL_SIZE       Size(200, 180)

#pragma mark- WorldPage

WorldPage* WorldPage::create(int world) {
    
    auto page = new WorldPage(world);
    
    if( page && page->init() ) {
        page->autorelease();
        return page;
    }
    
    CC_SAFE_DELETE(page);
    return nullptr;
}

WorldPage::WorldPage(int world):
world(world),
onClickListener(nullptr) {
    
}

WorldPage::~WorldPage() {
    
}

bool WorldPage::init() {
    
    if( !Widget::init() ) {
        return false;
    }
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2::ZERO);
    setContentSize(SB_WIN_SIZE);
    
    auto worldLabel = Label::createWithTTF(STR_FORMAT("WORLD %d", world), FONT_ROBOTO_BLACK, 60,
                                           Size::ZERO, TextHAlignment::CENTER, TextVAlignment::CENTER);
    worldLabel->setTextColor(Color4B::WHITE);
    worldLabel->setAnchorPoint(ANCHOR_M);
    worldLabel->setPosition(Vec2TC(0, -70));
    addChild(worldLabel);
    
    // 스테이지 Cell 생성
    Vec2 origin(300, 650);
    Vec2 padding(50, 50);
    Vec2 pos = origin;
    
    for( int i = 0; i < STAGE_COUNT; ++i ) {
        int stage = ((world-1) * STAGE_COUNT) + (i + 1);
        
        auto cell = StageCell::create(stage);
        cell->setAnchorPoint(ANCHOR_M);
        cell->setPosition(pos);
        cell->setOnClickListener([=](StageCell *cell) {
            if( onClickListener ) {
                this->retain();
                onClickListener(cell);
                this->release();
            }
        });
        addChild(cell);
        
        pos.x += CELL_SIZE.width;
        pos.x += padding.x;
        
        if( stage % 6 == 0 ) {
            pos.x = origin.x;
            pos.y -= CELL_SIZE.height;
            pos.y -= padding.y;
        }
    }
    
    return true;
}

#pragma mark- StageCell

StageCell* StageCell::create(int stage) {
    
    auto cell = new StageCell(stage);
    
    if( cell && cell->init() ) {
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return nullptr;
}

StageCell::StageCell(int stage):
stage(stage),
onClickListener(nullptr) {
    
}

StageCell::~StageCell() {
}

bool StageCell::init() {
    
    if( !Widget::init() ) {
        return false;
    }
    
    setContentSize(CELL_SIZE);
    
    addChild(SBNodeUtils::createBackgroundNode(this, Color4B(50, 50, 50, 255)));
    
    auto stageLabel = Label::createWithTTF(TO_STRING(stage), FONT_ROBOTO_BLACK, 40, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    stageLabel->setTextColor(Color4B::WHITE);
    stageLabel->setAnchorPoint(ANCHOR_M);
    stageLabel->setPosition(Vec2MC(CELL_SIZE, 0, 0));
    addChild(stageLabel);
    
    // Stars
    
    // onClick
    setTouchEnabled(true);
    addClickEventListener([=](Ref*) {
        SB_SAFE_PERFORM_LISTENER_N(this, onClickListener);
    });
    
    return true;
}

