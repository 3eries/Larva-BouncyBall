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
#include "MainStageProgressBar.hpp"

USING_NS_CC;
using namespace std;

#define CELL_SIZE       Size(184, 156)

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
onClickListener(nullptr),
onClickRewardItemListener(nullptr) {
    
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
    
    // 커밍순
    if( world > GAME_CONFIG->getWorldCount() ) {
        auto comingsoon = Sprite::create(DIR_IMG_MAIN + "main_coming_soon.png");
        comingsoon->setAnchorPoint(ANCHOR_M);
        comingsoon->setPosition(Vec2MC(0, -8));
        addChild(comingsoon);
        return true;
    }
    
    if( !RewardManager::isRewardedWorld(world) ) {
        initLockedUI();
    } else {
        initStageUI();
    }
    
    return true;
}

/**
 * 월드 잠김 UI 초기화
 */
void WorldPage::initLockedUI() {
    
    auto lockedLayer = Widget::create();
    lockedLayer->setTag(Tag::LOCKED_LAYER);
    lockedLayer->setAnchorPoint(Vec2::ZERO);
    lockedLayer->setPosition(Vec2::ZERO);
    lockedLayer->setContentSize(SB_WIN_SIZE);
    addChild(lockedLayer, SBZOrder::BOTTOM);
    
    // main_bg_lock.png Vec2MC(0, -9) , Size(1320, 612)
    auto bg = Sprite::create(DIR_IMG_MAIN + "main_bg_lock.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(0, -9));
    lockedLayer->addChild(bg);
    
    // 문구
    // 빨간색 텍스트 컬러 + 8px 흰색 스트로크 + 검정색 드랍쉐도우
    // Text Sabo size:80 color:197,0,0 + stroke 8px + dropshadow Vec2MC(0, -84) , Size(1089, 178)
    {
        string descStr = STR_FORMAT("PLEASE GET %d STARS\nON WORLD %d",
                                    RewardManager::getWorldItem(world).requireStar, world-1);
        
        auto desc = Label::createWithTTF(descStr, FONT_SABO, 80, Size::ZERO,
                                         TextHAlignment::CENTER, TextVAlignment::CENTER);
        desc->setTag(Tag::LOCKED_DESCRIPTION);
        desc->setTextColor(Color4B(197,0,0,255));
        desc->enableOutline(Color4B::WHITE, 8);
        desc->enableShadow(Color4B::BLACK, Size(0, -8));
        desc->setAnchorPoint(ANCHOR_M);
        desc->setPosition(Vec2MC(0, -84));
        lockedLayer->addChild(desc);
    }
    
    // main_icon_lock_world04.png Vec2MC(0, 114) , Size(136, 152)
    auto lockedAnim = SBSkeletonAnimation::create(DIR_IMG_MAIN + "unlock_world.json");
    lockedAnim->setTag(Tag::LOCKED);
    lockedAnim->setPosition(Vec2MC(0, 114));
    lockedLayer->addChild(lockedAnim);

    lockedAnim->setAnimation(0, STR_FORMAT("lock_world%02d", world), true);
    
    // main_need_key_balloon.png Vec2MC(190, 146) , Size(196, 128)
    // idle, disappear, nope
    auto needKeyAnim = SBSkeletonAnimation::create(DIR_IMG_MAIN + "need_key_balloon.json");
    needKeyAnim->setTag(Tag::LOCKED_NEED_KEY);
    needKeyAnim->setPosition(Vec2MC(190, 146));
    lockedLayer->addChild(needKeyAnim);

    needKeyAnim->setAnimation(0, "idle", true);
    
    // 터치
    auto touchNode = Widget::create();
    touchNode->setAnchorPoint(bg->getAnchorPoint());
    touchNode->setPosition(bg->getPosition());
    touchNode->setContentSize(bg->getContentSize());
    touchNode->setTouchEnabled(true);
    bg->getParent()->addChild(touchNode);
    
    touchNode->addClickEventListener([=](Ref*) {
        touchNode->setVisible(false);
        
        needKeyAnim->clearTracks();
        needKeyAnim->runAnimation("nope", false, [=](spine::TrackEntry *entry) {
            touchNode->setVisible(true);
            needKeyAnim->setAnimation(0, "idle", true);
        });
    });
}

/**
 * 스테이지 UI 초기화
 */
void WorldPage::initStageUI() {
    
    // Cell 초기화
    const int STAGE_COUNT = GAME_CONFIG->getStagePerWorld();
    const int STAGE_WIDTH_COUNT = 6;
    const Vec2 PADDING(40, 24);
    
    auto getStageWidth = [=]() -> float {
        float w = STAGE_WIDTH_COUNT * CELL_SIZE.width;
        w += ((STAGE_WIDTH_COUNT-1) * PADDING.x);
        return w;
    };
    
    // main_box_stage.png Vec2MC(-560, 110) , Size(184, 156)
    Vec2 origin;
    origin.x = ((SB_WIN_SIZE.width - getStageWidth()) / 2) + CELL_SIZE.width*0.5f;
    origin.y = SB_WIN_SIZE.height*0.5f + 110;
    
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
    
    // 진행도
    auto progressBar = MainStageProgressBar::create();
    progressBar->setTag(Tag::PROGRESS_BAR);
    progressBar->setWorld(world);
    progressBar->setOnItemClickListener([=](RewardItemData data) {
        this->retain();
        onClickRewardItemListener(data);
        this->release();
    });
    addChild(progressBar, SBZOrder::BOTTOM);
}

/**
 * 월드 잠금 해제
 */
void WorldPage::unlock() {
    
    SBDirector::getInstance()->setScreenTouchLocked(true);
    auto lockedLayer = getChildByTag(Tag::LOCKED_LAYER);
    
    // 1. need key 제거
    auto needKeyAnim = lockedLayer->getChildByTag<SBSkeletonAnimation*>(Tag::LOCKED_NEED_KEY);
    needKeyAnim->runAnimation("disappear", true, [=](spine::TrackEntry*) {
       
        // 2. 자물쇠 오픈
        auto lockedAnim = lockedLayer->getChildByTag<SBSkeletonAnimation*>(Tag::LOCKED);
        lockedAnim->runAnimation(STR_FORMAT("unlocking_world%02d", world), false, [=](spine::TrackEntry*) {
            
            SBDirector::postDelayed(this, [=]() {
                SBDirector::getInstance()->setScreenTouchLocked(false);
                
                // 3. 잠금 UI 제거 및 스테이지 UI 초기화
                lockedLayer->removeFromParent();
                this->initStageUI();
            }, 0.3f, false);
        });

        // 문구 변경
        SBDirector::postDelayed(this, [=]() {
            string str = STR_FORMAT("YOU HAVE UNLOCKED\nWORLD %d NOW!", world);
            lockedLayer->getChildByTag<Label*>(Tag::LOCKED_DESCRIPTION)->setString(str);
        }, 0.7f, false);
    });
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
    
    if( StageManager::isStageLocked(stage) ) {
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
    
    auto stageLabel = Label::createWithTTF(TO_STRING(stage), FONT_SUPER_STAR, 80, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    stageLabel->setTextColor(Color4B::WHITE);
    stageLabel->enableOutline(Color4B::BLACK, 4);
    stageLabel->enableShadow(Color4B::BLACK, Size(0, -4));
    stageLabel->setAnchorPoint(ANCHOR_M);
    stageLabel->setPosition(Vec2MC(CELL_SIZE, 0, 25));
    addChild(stageLabel);

    int stars = StageManager::getStageStarCount(stage);
    
    Vec2 pos[] = {
        Vec2BC(CELL_SIZE, -36, 44),
        Vec2BC(CELL_SIZE, 0, 44),
        Vec2BC(CELL_SIZE, 36, 44),
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

