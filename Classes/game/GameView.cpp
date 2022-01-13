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
    initGameListener();
    initTouchListener();
     
    // Debug Value
    infoLabel = Label::createWithTTF("", FONT_ROBOTO_BLACK, 60, Size::ZERO,
                                     TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
    infoLabel->setTextColor(Color4B::WHITE);
    infoLabel->setAnchorPoint(ANCHOR_BR);
    infoLabel->setPosition(Vec2BR(-50, 10));
    addChild(infoLabel);
    
    maxVelocity = Vec2(INT_MIN, INT_MIN);
    minVelocity = Vec2(INT_MAX, INT_MAX);
    
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
}

/**
 * 게임 일시정지
 */
void GameView::onGamePause() {
    
    SBNodeUtils::recursivePause(this);
}

/**
 * 게임 재개
 */
void GameView::onGameResume() {
    
    SBNodeUtils::recursiveResume(this);
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

#pragma mark- Initialize

/**
 * 물리 영역 초기화
 */
void GameView::initPhysics() {
    
    setSyncLocked(true);
    
    auto world = PHYSICS_MANAGER->initWorld();
    
    // Wall & Floor
    auto MAP_POSITION = Vec2MC(0,0);
    auto MAP_CONTENT_SIZE = SB_WIN_SIZE * 0.95f;
    
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
    
    // Ball
    ball = Ball::create(world);
    addChild(ball);
    
    // 임의의 블럭 생성 for prototype
    auto blockOrigin = Vec2(100, 100);
    
    Vec2 blockPos[] = {
        // 1층
        Vec2(BLOCK_SIZE.width * 0, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 0, BLOCK_SIZE.height * 1),
        Vec2(BLOCK_SIZE.width * 0, BLOCK_SIZE.height * 2),
        Vec2(BLOCK_SIZE.width * 0, BLOCK_SIZE.height * 3),
        
        Vec2(BLOCK_SIZE.width * 1, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 2, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 3, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 4, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 5, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 6, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 7, BLOCK_SIZE.height * 0),
        Vec2(BLOCK_SIZE.width * 8, BLOCK_SIZE.height * 0),
        
        // 2층
        Vec2(BLOCK_SIZE.width * 9, BLOCK_SIZE.height * 1),
        Vec2(BLOCK_SIZE.width * 10, BLOCK_SIZE.height * 2),
        Vec2(BLOCK_SIZE.width * 11, BLOCK_SIZE.height * 2),
        Vec2(BLOCK_SIZE.width * 12, BLOCK_SIZE.height * 2),
        
        // 3층
        Vec2(BLOCK_SIZE.width * 13, BLOCK_SIZE.height * 3),
        Vec2(BLOCK_SIZE.width * 14, BLOCK_SIZE.height * 4),
        Vec2(BLOCK_SIZE.width * 15, BLOCK_SIZE.height * 4),
        Vec2(BLOCK_SIZE.width * 16, BLOCK_SIZE.height * 4),
        Vec2(BLOCK_SIZE.width * 16, BLOCK_SIZE.height * 5),
        Vec2(BLOCK_SIZE.width * 16, BLOCK_SIZE.height * 6),
        Vec2(BLOCK_SIZE.width * 16, BLOCK_SIZE.height * 7),
    };
    
    for( int i = 0; i < sizeof(blockPos) / sizeof(Vec2); ++i ) {
        // auto block = Sprite::create(DIR_IMG_GAME + "block.png");
        auto block = Block::create();
        block->setAnchorPoint(ANCHOR_M);
        block->setPosition(blockOrigin + blockPos[i]);
        addChild(block);
        
        block->syncNodeToBody();
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
    
    auto toggleBtn = GameUIHelper::createFontButton("DebugDraw", Size(300,200));
    toggleBtn->setAnchorPoint(ANCHOR_TL);
    toggleBtn->setPosition(Vec2TL(10, 10));
    addChild(toggleBtn);
    
    toggleBtn->setOnClickListener([=](Node*) {
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
    addChild(stageProgressBar);
}

/**
 * 게임 이벤트 리스너 초기화
 */
void GameView::initGameListener() {
    
    StringList events({
        GAME_EVENT_RESET,
        GAME_EVENT_EXIT,
        GAME_EVENT_PAUSE,
        GAME_EVENT_RESUME,
        GAME_EVENT_STAGE_CHANGED,
        GAME_EVENT_STAGE_RESTART,
        GAME_EVENT_STAGE_CLEAR,
        GAME_EVENT_MOVE_NEXT_STAGE,
        GAME_EVENT_MOVE_NEXT_STAGE_FINISHED,
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
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
}
