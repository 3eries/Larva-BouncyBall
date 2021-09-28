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
#include "object/StageProgressBar.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define DEBUG_DRAW_PHYSICS      1

GameView::GameView():
world(nullptr) {
}

GameView::~GameView() {
    
    CC_SAFE_DELETE(world);
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
    
    scheduleUpdate();
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

#pragma mark- Initialize

/**
 * 물리 영역 초기화
 */
void GameView::initPhysics() {
    
    world = PHYSICS_MANAGER->initWorld();
    
    // 물리 객체 초기화
    auto MAP_POSITION = Vec2MC(0,0);
    auto MAP_CONTENT_SIZE = SB_WIN_SIZE * 0.9f;
    
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
        fixtureDef.restitution = 1;     // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
        fixtureDef.friction = 0;        // 마찰력
        fixtureDef.filter = filter;
        body->CreateFixture(&fixtureDef);
    }
    
    // Ball
    auto ball = Ball::create(world);
    addChild(ball);
    
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
