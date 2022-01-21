//
//  GameView.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "GameView.hpp"

#include "Define.h"
#include "GameUIHelper.hpp"
#include "GameDefine.h"
#include "ContentManager.hpp"
#include "SceneManager.h"

#include "object/Ball.hpp"
#include "object/tile/Flag.hpp"
#include "object/tile/Item.hpp"
#include "object/tile/Block.hpp"

#include "object/StageProgressBar.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define DEBUG_DRAW_PHYSICS      1

GameView::GameView(): SBPhysicsObject(this),
isTouchEnabled(false) {
}

GameView::~GameView() {
}

bool GameView::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(ANCHOR_M);
    setPosition(Vec2MC(0,0));
    setContentSize(SB_WIN_SIZE);
    
    initPhysics();
    initBg();
    initTiles();
    initBall();
    initGameListener();
    initTouchListener();
    
    return true;
}

void GameView::onEnter() {
    
    Log::i("GameView::onEnter");
    
    Node::onEnter();
    
    onStageChanged(GAME_MANAGER->getStage());
}

void GameView::onEnterTransitionDidFinish() {
    
    Node::onEnterTransitionDidFinish();
    
    // 튜토리얼
    if( GAME_MANAGER->getStage().stage == 1 ) {
    }
    
    //
    schedule([=](float dt) {
        auto body = ball->getBody();
        auto velocity = body->GetLinearVelocity();
        
        // CCLOG("velocity: %f, %f", velocity.x, velocity.y);
        this->maxVelocity.x = MAX(velocity.x, this->maxVelocity.x);
        this->maxVelocity.y = MAX(velocity.y, this->maxVelocity.y);
        this->minVelocity.x = MIN(velocity.x, this->minVelocity.x);
        this->minVelocity.y = MIN(velocity.y, this->minVelocity.y);
        
        string msg = "";
        msg += STR_FORMAT("x: %d", (int)velocity.x) + "\n";
        msg += STR_FORMAT("y: %d", (int)velocity.y) + "\n";
        msg += STR_FORMAT("maxX: %d", (int)maxVelocity.x) + "\n";
        msg += STR_FORMAT("maxY: %d", (int)maxVelocity.y) + "\n";
        msg += STR_FORMAT("minX: %d", (int)minVelocity.x) + "\n";
        msg += STR_FORMAT("minY: %d", (int)minVelocity.y) + "\n";
        
        infoLabel->setString(msg);
        
    }, 0.1f, "TEST");
}

void GameView::cleanup() {
    
    removeListeners(this);
    
    Node::cleanup();
}

#pragma mark- Game Event

/**
 * 게임 리셋
 */
void GameView::onGameReset() {
}

/**
 * 게임 종료
 */
void GameView::onGameExit() {
    
#if DEBUG_DRAW_PHYSICS
    removeChildByTag(Tag::DEBUG_DRAW_VIEW);
#endif
    
    SBNodeUtils::recursivePauseSchedulerAndActions(this);
}

/**
 * 게임 일시정지
 */
void GameView::onGamePause() {
    
    // SBNodeUtils::recursivePause(this);
}

/**
 * 게임 재개
 */
void GameView::onGameResume() {
    
    // SBNodeUtils::recursiveResume(this);
}

/**
 * 스테이지 변경
 */
void GameView::onStageChanged(const StageData &stage) {
    
    isTouchEnabled = true;
}

/**
 * 스테이지 재시작
 */
void GameView::onStageRestart(const StageData &stage) {
}

/**
 * 스테이지 클리어
 */
void GameView::onStageClear(const StageData &stage) {
}

/**
 * 다음 스테이지로 이동
 */
void GameView::onMoveNextStage() {
    
    // 연출 완료
    SBDirector::postDelayed(this, [=]() {
        GameManager::onMoveNextStageFinished();
    }, MOVE_NEXT_LEVEL_DURATION, true);
}

/**
 * 다음 스테이지로 이동 완료
 */
void GameView::onMoveNextStageFinished() {
}

#pragma mark- Touch Event

void GameView::onTouchBegan(Touch *touch) {
    
    bool isLeft = (touch->getLocation().x < SB_WIN_SIZE.width*0.5f);

    if( isLeft ) {
        ball->moveLeft();
    } else {
        ball->moveRight();
    }
}

void GameView::onTouchEnded(Touch *touch) {
    
    ball->stopMoveX();
}

#pragma mark- Physics Event

/**
 * 볼 & 깃발 충돌
 */
void GameView::onContactFlag(Ball *ball, GameTile *tile) {
    
    auto flag = dynamic_cast<Flag*>(tile);
    
    // 충돌 처리
    switch( flag->getData().type ) {
        case TileType::FLAG_CLEAR_PORTAL: {
        } break;
            
        default: break;
    }
}

/**
 * 볼 & 아이템 충돌
 */
void GameView::onContactItem(Ball *ball, GameTile *tile) {
    
    auto item = dynamic_cast<Item*>(tile);
    
    // 아이템 삭제
    item->prepareRemove();
    item->setLazyRemove(true);
    item->setVisible(false);
    
    // 충돌 처리
    switch( item->getData().type ) {
        // 소시지
        case TileType::ITEM_SAUSAGE: {
            int star = GAME_MANAGER->getStar() + 1;
            star = MIN(3, star);
            
            GAME_MANAGER->setStar(star);
            stageProgressBar->setStar(star);
        } break;
            
        // 더블 점프
        case TileType::ITEM_DOUBLE_JUMP: {
            
        } break;
            
        default: break;
    }
}

/**
 * 볼 & 브릭 충돌
 */
void GameView::onContactBlock(Ball *ball, GameTile *tile, Vec2 contactPoint) {
    
    auto block = dynamic_cast<Block*>(tile);
    
    // 충돌 처리
    switch( block->getData().type ) {
        // Breaking
        case TileType::BLOCK_BREKING: {
        } break;
        
        // Game Over
        case TileType::BLOCK_GAME_OVER: {
        } break;
            
        // Jump
        case TileType::BLOCK_JUMP: {
        } break;
            
        default: break;
    }
}

#pragma mark- Initialize

/**
 * 물리 영역 초기화
 */
void GameView::initPhysics() {
    
    setSyncLocked(true);
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->onContactFlag         = CC_CALLBACK_2(GameView::onContactFlag, this);
    listener->onContactItem         = CC_CALLBACK_2(GameView::onContactItem, this);
    listener->onContactBlock        = CC_CALLBACK_3(GameView::onContactBlock, this);
//    listener->onContactWall         = CC_CALLBACK_1(Ball::onContactWall, this);
//    listener->onContactFloor        = CC_CALLBACK_1(Ball::onContactFloor, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    world = PHYSICS_MANAGER->initWorld();
    
    // Wall & Floor
    auto MAP_POSITION = Vec2MC(0,0);
    auto MAP_CONTENT_SIZE = GAME_MANAGER->getStage().mapContentSize;
    
    b2BodyDef bodyDef;
    bodyDef.position = PTM(MAP_POSITION);
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = world->CreateBody(&bodyDef);
    setBody(body);
    
    float left   = PTM(-MAP_CONTENT_SIZE.width*0.5f);
    float right  = PTM( MAP_CONTENT_SIZE.width*0.5f);
    float bottom = PTM(-MAP_CONTENT_SIZE.height*0.5f);
    float top    = PTM( MAP_CONTENT_SIZE.height*0.5f);
    
    b2Vec2 vectors[4][2] = {
        { b2Vec2(left, bottom), b2Vec2(left, top) },          // left
        { b2Vec2(right, bottom), b2Vec2(right, top) },        // right
        { b2Vec2(left, bottom), b2Vec2(right, bottom) },      // bottom
        { b2Vec2(left, top), b2Vec2(right, top) },            // top
    };
    
    PhysicsCategory categorys[] = {
        PhysicsCategory::WALL_LEFT,
        PhysicsCategory::WALL_RIGHT,
        PhysicsCategory::FLOOR,
        PhysicsCategory::WALL_TOP,
    };
    
    for( int i = 0; i < 4; ++i ) {
        b2Vec2 v1 = vectors[i][0];
        b2Vec2 v2 = vectors[i][1];
        
        b2EdgeShape shape;
        shape.Set(v1, v2);
        
        b2Filter filter;
        filter.categoryBits = categorys[i];
        filter.maskBits = PHYSICS_MASK_BITS_WALL;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 0.1f;      // 밀도
        fixtureDef.restitution = 0;     // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
        fixtureDef.friction = 0;        // 마찰력
        fixtureDef.filter = filter;
        body->CreateFixture(&fixtureDef);
    }
    
#if DEBUG_DRAW_PHYSICS
    // DebugDrawView
    auto view = DebugDrawView::create(world);
    view->setVisible(false);
    view->setTag(Tag::DEBUG_DRAW_VIEW);
    addChild(view, SBZOrder::MIDDLE);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
//        flags += b2Draw::e_jointBit;
//        flags += b2Draw::e_aabbBit;
//        flags += b2Draw::e_pairBit;
//        flags += b2Draw::e_centerOfMassBit;
    view->getDebugDraw()->SetFlags(flags);
    
    // display label
    infoLabel = Label::createWithTTF("", FONT_ROBOTO_BLACK, 60, Size::ZERO,
                                     TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
    infoLabel->setVisible(false);
    infoLabel->setTextColor(Color4B::WHITE);
    infoLabel->setAnchorPoint(ANCHOR_BR);
    infoLabel->setPosition(Vec2BR(-50, -20));
    addChild(infoLabel, SBZOrder::MIDDLE);
    
    maxVelocity = Vec2(INT_MIN, INT_MIN);
    minVelocity = Vec2(INT_MAX, INT_MAX);
    
    // toggle button
    auto toggleBtn = GameUIHelper::createFontButton("DebugDraw", Size(300,200));
    toggleBtn->setAnchorPoint(ANCHOR_TL);
    toggleBtn->setPosition(Vec2TL(10, 10));
    addChild(toggleBtn, SBZOrder::MIDDLE);
    
    toggleBtn->setOnClickListener([=](Node*) {
        infoLabel->setVisible(!infoLabel->isVisible());
        view->setVisible(!view->isVisible());
    });
#endif
}

/**
 * 배경 초기화
 */
void GameView::initBg() {
    
    auto bg = Sprite::create(DIR_IMG_GAME + "bg.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(0,0));
    bg->setScaleX(SB_WIN_SIZE.width / bg->getContentSize().width);
    bg->setScaleY(SB_WIN_SIZE.height / bg->getContentSize().height);
    addChild(bg, -1);
    
    // 스테이지 진행도
    stageProgressBar = StageProgressBar::create();
    addChild(stageProgressBar, ZOrder::MENU);
}

/**
 * 타일 초기화
 */
void GameView::initTiles() {
    
    auto stage = GAME_MANAGER->getStage();
    
    for( auto tileData : stage.tiles ) {
        if( tileData.type == TileType::NONE ) {
            continue;
        }
        
        GameTile *tile = nullptr;
        
        switch( tileData.type ) {
            case TileType::FLAG_START:
            case TileType::FLAG_CLEAR_PORTAL: {
                tile = Flag::create(tileData);
            } break;
                
            case TileType::ITEM_SAUSAGE:
            case TileType::ITEM_DOUBLE_JUMP: {
                tile = Item::create(tileData);
            } break;
                
            case TileType::BLOCK_NORMAL:
            case TileType::BLOCK_BREKING:
            case TileType::BLOCK_GAME_OVER:
            case TileType::BLOCK_JUMP: {
                tile = Block::create(tileData);
            } break;
                
            default: break;
        }
        
        if( tile ) {
            addChild(tile, tileData.isBlockType() ? ZOrder::BLOCK : ZOrder::ITEM);
        }
    }
}

/**
 * 볼(캐릭터) 초기화
 */
void GameView::initBall() {
 
    auto stage = GAME_MANAGER->getStage();
    auto flag = stage.getTiles(TileType::FLAG_START)[0];
    
    ball = Ball::create(stage);
    addChild(ball, ZOrder::BALL);
    
    auto pos = convertTilePosition(stage, flag.x, flag.y+1);
//    pos.y -= (stage.tileSize.height - ball->getContentSize().height) / 2;
//    pos.y += 1;
    ball->setFirstPosition(pos);
}

/**
 * 게임 이벤트 리스너 초기화
 */
void GameView::initGameListener() {
    
    GameEventList events({
        GameEvent::RESET,
        GameEvent::EXIT,
        GameEvent::PAUSE,
        GameEvent::RESUME,
        GameEvent::STAGE_CHANGED,
        GameEvent::STAGE_RESTART,
        GameEvent::STAGE_CLEAR,
        GameEvent::MOVE_NEXT_STAGE,
        GameEvent::MOVE_NEXT_STAGE_FINISHED,
    });
    
    GameManager::addEventListener(events, [=](GameEvent event, void *userData) {
        
        switch( event ) {
            case GameEvent::RESET:     this->onGameReset();         break;
            case GameEvent::EXIT:      this->onGameExit();         break;
            case GameEvent::PAUSE:     this->onGamePause();         break;
            case GameEvent::RESUME:    this->onGameResume();        break;
                
            case GameEvent::STAGE_CHANGED: {
                auto stage = (StageData*)userData;
                this->onStageChanged(*stage);
            } break;
                
            case GameEvent::STAGE_RESTART: {
                auto stage = (StageData*)userData;
                this->onStageRestart(*stage);
            } break;
                
            case GameEvent::STAGE_CLEAR: {
                auto stage = (StageData*)userData;
                this->onStageClear(*stage);
            } break;
                
            case GameEvent::MOVE_NEXT_STAGE:            this->onMoveNextStage();          break;
            case GameEvent::MOVE_NEXT_STAGE_FINISHED:   this->onMoveNextStageFinished();  break;
                
            default: break;
        }
    }, this);
}

void GameView::initTouchListener() {
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [=](Touch *touch, Event *unusedEvent) -> bool {
        
        if( !isTouchEnabled ) {
            return false;
        }
        
        this->onTouchBegan(touch);
        return true;
    };
    touchListener->onTouchMoved = [=](Touch *touch, Event *unusedEvent) {
    };
    touchListener->onTouchEnded = [=](Touch *touch, Event *unusedEvent) {
        this->onTouchEnded(touch);
    };
    touchListener->onTouchCancelled = [=](Touch *touch, Event *unusedEvent) {
        this->onTouchEnded(touch);
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}
