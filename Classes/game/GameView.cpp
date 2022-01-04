//
//  GameView.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "GameView.hpp"

#include "Define.h"
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

GameView::GameView():
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
        CCLOG("velocity: %f, %f", body->GetLinearVelocity().x, body->GetLinearVelocity().y);
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

#define     SCHEDULER_BALL_DIRECTION                "SCHEDULER_BALL_DIRECTION"
#define     SCHEDULER_END_VELOCITY                  "SCHEDULER_END_VELOCITY"

void GameView::onTouchBegan(Touch *touch) {
    
    unschedule(SCHEDULER_BALL_DIRECTION);
    unschedule(SCHEDULER_END_VELOCITY);
    
    bool isLeft = (touch->getLocation().x < SB_WIN_SIZE.width*0.5f);
    ball->setDirection(isLeft ? BallDirection::LEFT : BallDirection::RIGHT);
    
    b2Vec2 force = b2Vec2(5, 0);
    
    auto body = ball->getBody();
    // body->ApplyLinearImpulse(force, body->GetPosition(), false);
    // body->ApplyForceToCenter(b2Vec2(100, 0), false);
    body->SetLinearVelocity(b2Vec2(isLeft ? -10 : 10, body->GetLinearVelocity().y));
    
    // 방향에 따른 가속도 스케줄러 실행
    schedule([=](float dt) {
        auto body = ball->getBody();
        body->SetLinearVelocity(b2Vec2(isLeft ? -10 : 10, body->GetLinearVelocity().y));
    }, 0.01f, SCHEDULER_BALL_DIRECTION);
}

void GameView::onTouchEnded(Touch *touch) {
    
    unschedule(SCHEDULER_BALL_DIRECTION);
    
    scheduleOnce([=](float dt) {
        auto body = ball->getBody();
        body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
    }, 0.1f, SCHEDULER_END_VELOCITY);
}

#pragma mark- Initialize

/**
 * 물리 영역 초기화
 */
void GameView::initPhysics() {
    
    auto world = PHYSICS_MANAGER->initWorld();
    
    // 물리 객체 초기화
    auto MAP_POSITION = Vec2MC(0,0);
    auto MAP_CONTENT_SIZE = SB_WIN_SIZE * 0.95f;
    
    b2BodyDef bodyDef;
    bodyDef.position = PTM(MAP_POSITION);
    // bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = world->CreateBody(&bodyDef);
    // setBody(body);
    
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
    auto blockOrigin = Vec2(400, 300);
    
    Vec2 blockPos[] = {
        blockOrigin + Vec2(BLOCK_SIZE.width * -2, 0),
        blockOrigin + Vec2(BLOCK_SIZE.width * -1, 0),
        blockOrigin + Vec2(0, 0),
        blockOrigin + Vec2(BLOCK_SIZE.width * 1, 0),
        blockOrigin + Vec2(BLOCK_SIZE.width * 2, BLOCK_SIZE.height * 1),
        blockOrigin + Vec2(BLOCK_SIZE.width * 3, BLOCK_SIZE.height * 1),
        blockOrigin + Vec2(BLOCK_SIZE.width * 4, 0),
        blockOrigin + Vec2(BLOCK_SIZE.width * 5, 0),
        /* empty */
        blockOrigin + Vec2(BLOCK_SIZE.width * 7, BLOCK_SIZE.height * 1),
        blockOrigin + Vec2(BLOCK_SIZE.width * 8, BLOCK_SIZE.height * 2),
        blockOrigin + Vec2(BLOCK_SIZE.width * 9, BLOCK_SIZE.height * 3),
        blockOrigin + Vec2(BLOCK_SIZE.width * 10, BLOCK_SIZE.height * 3),
        blockOrigin + Vec2(BLOCK_SIZE.width * 11, BLOCK_SIZE.height * 3),
        blockOrigin + Vec2(BLOCK_SIZE.width * 12, BLOCK_SIZE.height * 2),
    };
    
    for( int i = 0; i < sizeof(blockPos) / sizeof(Vec2); ++i ) {
        // auto block = Sprite::create(DIR_IMG_GAME + "block.png");
        auto block = Block::create();
        block->setAnchorPoint(ANCHOR_M);
        block->setPosition(blockPos[i]);
        addChild(block);
        
        block->syncNodeToBody();
    }
    
    /*
    auto blockPos = Vec2MC(-200, -250);
    
    for( int i = 0; i < 1; ++i ) {
        auto p1 = blockPos;
        auto p2 = p1 + Vec2(BLOCK_SIZE.width, 0);
        
        b2BodyDef bodyDef;
        bodyDef.position = PTM(p1);
        // bodyDef.userData = (SBPhysicsObject*)this;
        
        auto body = world->CreateBody(&bodyDef);
        
        b2PolygonShape shape;
        // shape.Set(PTM(p1), PTM(p2));
        shape.SetAsBox(PTM(BLOCK_SIZE.width*0.5f), PTM(BLOCK_SIZE.height*0.5f));
        
        b2Filter filter;
        filter.categoryBits = PhysicsCategory::WALL_TOP;
        filter.maskBits = PHYSICS_MASK_BITS_WALL;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 0.1f;      // 밀도
        fixtureDef.restitution = 1;     // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
        fixtureDef.friction = 0;        // 마찰력
        fixtureDef.filter = filter;
        body->CreateFixture(&fixtureDef);
        
        blockPos.x += BLOCK_SIZE.width;
        blockPos.y += BLOCK_SIZE.height + 10;
    }
     */
    
#if DEBUG_DRAW_PHYSICS
    // DebugDrawView
    auto view = DebugDrawView::create(world);
    view->setTag(Tag::DEBUG_DRAW_VIEW);
    addChild(view, SBZOrder::MIDDLE);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
//        flags += b2Draw::e_jointBit;
//        flags += b2Draw::e_aabbBit;
//        flags += b2Draw::e_pairBit;
//        flags += b2Draw::e_centerOfMassBit;
    view->getDebugDraw()->SetFlags(flags);
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
