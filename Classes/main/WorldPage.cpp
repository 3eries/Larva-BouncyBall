//
//  WorldPage.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/10.
//

#include "WorldPage.hpp"

#include "Define.h"
#include "GameConfiguration.hpp"
#include "User.hpp"

USING_NS_CC;
using namespace std;

#define CELL_SIZE       Size(184, 172)

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
    
    // 스테이지 Cell 생성
    // main_box_stage.png Vec2MC(-560, 74) , Size(184, 172)
    // main_box_stage.png Vec2MC(-336, 74) , Size(184, 172)
    // main_box_stage.png Vec2MC(-560, -138) , Size(184, 172)
    const int STAGE_COUNT = GAME_CONFIG->getStagePerWorld();
    const int STAGE_WIDTH_COUNT = 6;
    const Vec2 PADDING(40, 40);
    
    auto getStageWidth = [=]() -> float {
        float w = STAGE_WIDTH_COUNT * CELL_SIZE.width;
        w += ((STAGE_WIDTH_COUNT-1) * PADDING.x);
        return w;
    };
    
    Vec2 origin;
    origin.x = ((SB_WIN_SIZE.width - getStageWidth()) / 2) + CELL_SIZE.width*0.5f;
    origin.y = SB_WIN_SIZE.height*0.5f + 74;
    
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
        pos.x += PADDING.x;
        
        if( stage % STAGE_WIDTH_COUNT == 0 ) {
            pos.x = origin.x;
            pos.y -= CELL_SIZE.height;
            pos.y -= PADDING.y;
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
    
    if( User::isStageLocked(stage) ) {
        // 잠김
        auto locked = Sprite::create(DIR_IMG_MAIN + "main_box_stage_disable.png");
        locked->setAnchorPoint(ANCHOR_M);
        locked->setPosition(Vec2MC(CELL_SIZE, 0, 0));
        addChild(locked);
        
        return true;
    }
    
    auto bg = Sprite::create(DIR_IMG_MAIN + "main_box_stage.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(CELL_SIZE, 0, 0));
    addChild(bg);
    
    // 888 Superstar size:80 255,255,255 stroke:4px shadow:4px Vec2MC(0, 25) , Size(128, 61)
    auto stageLabel = Label::createWithTTF(TO_STRING(stage), FONT_SUPER_STAR, 80, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    stageLabel->setTextColor(Color4B::WHITE);
    stageLabel->enableOutline(Color4B::BLACK, 4);
    stageLabel->enableShadow(Color4B::BLACK, Size(0, -4));
    stageLabel->setAnchorPoint(ANCHOR_M);
    stageLabel->setPosition(Vec2MC(CELL_SIZE, 0, 25));
    addChild(stageLabel);

    int stars = User::getStageStarCount(stage);
    
    Vec2 pos[] = {
        Vec2BC(CELL_SIZE, -36, 48),
        Vec2BC(CELL_SIZE, 0, 48),
        Vec2BC(CELL_SIZE, 36, 48),
    };
    
    for( int i = 0; i < stars; ++i ) {
        auto starSpr = Sprite::create(DIR_IMG_MAIN + "main_icon_stage_star.png");
        starSpr->setAnchorPoint(ANCHOR_M);
        starSpr->setPosition(pos[i]);
        addChild(starSpr);
    }
    
    // onClick
    setTouchEnabled(true);
    addClickEventListener([=](Ref*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER_N(this, onClickListener);
    });
    
    return true;
}

