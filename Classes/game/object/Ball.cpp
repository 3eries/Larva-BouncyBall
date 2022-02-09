//
//  Ball.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/27.
//

#include "Ball.hpp"

#include "Define.h"
#include "../GameManager.hpp"
#include "../GameView.hpp"

#include "tile/Block.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define VELOCITY_BOUNCE_UP                     (22 * GAME_MANAGER->getMapScaleFactor())
#define VELOCITY_JUMP_UP                       (37 * GAME_MANAGER->getMapScaleFactor())
#define VELOCITY_BOUNCE_DOWN                   (-15 * GAME_MANAGER->getMapScaleFactor())
#define VELOCITY_MOVE_LEFT                     (-13 * GAME_MANAGER->getMapScaleFactor())
#define VELOCITY_MOVE_RIGHT                    (13 * GAME_MANAGER->getMapScaleFactor())

#define BOUNCE_DOWN_DELAY                      0.5f
#define CONTINUOUS_X_INTERVAL                  0.1f
#define RESET_X_DELAY                          0.05f

#define SCHEDULER_BOUNCE_DOWN                  "SCHEDULER_BOUNCE_DOWN"
#define SCHEDULER_CONTINUOUS_X                 "SCHEDULER_CONTINUOUS_X"
#define SCHEDULER_RESET_VELOCITY_X             "SCHEDULER_RESET_VELOCITY_X"

Ball* Ball::create(const StageData &stage) {
    
    auto ball = new Ball(stage);
    
    if( ball && ball->init() ) {
        ball->autorelease();
        return ball;
    }
    
    CC_SAFE_DELETE(ball);
    return nullptr;
}

Ball::Ball(const StageData &stage) : SBPhysicsObject(this),
stage(stage),
direction(BallDirection::NONE),
jumpEffectPlayedTime(0) {
}

Ball::~Ball() {
    
}

bool Ball::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    auto size = Size(stage.tileSize.width*0.7f, stage.tileSize.height*0.5f);
    
    setAnchorPoint(ANCHOR_M);
    setContentSize(size);
    setCascadeOpacityEnabled(true);
    
    initImage();
    initPhysics();
    
    return true;
}

void Ball::cleanup() {
    
    removeListeners(this);
    
    Node::cleanup();
}

/**
 * 이미지 초기화
 */
void Ball::initImage() {
    
    image = Sprite::create();
    image->setScale(GAME_MANAGER->getMapScaleFactor());
    image->setAnchorPoint(ANCHOR_MB);
    image->setPosition(Vec2BC(getContentSize(), 0, 0));
    addChild(image);
    
    auto updateImage = [=]() {
        image->setTexture(DIR_CONTENT_CHARACTER + "char_001.png");
    };
    
    updateImage();
    
    // 볼 스킨 변경 리스너
//    auto listener = EventListenerCustom::create(DIRECTOR_EVENT_UPDATE_BALL_SKIN, [=](EventCustom *event) {
//        updateImage();
//    });
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

/**
 * 물리 초기화
 */
void Ball::initPhysics() {
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->setContactTarget(this);
    listener->onContactBlock        = CC_CALLBACK_3(Ball::onContactBlock, this);
//    listener->onContactItem         = CC_CALLBACK_2(Ball::onContactItem, this);
//    listener->onContactWall         = CC_CALLBACK_1(Ball::onContactWall, this);
//    listener->onContactFloor        = CC_CALLBACK_1(Ball::onContactFloor, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    bodyDef.fixedRotation = true;
    bodyDef.userData = (SBPhysicsObject*)this;
    
//    b2CircleShape shape;
//    shape.m_radius = PTM(getContentSize().height*0.5f);
    b2PolygonShape shape;
    shape.SetAsBox(PTM(getContentSize().width*0.5f), PTM(getContentSize().height*0.5f));
    
    auto body = PHYSICS_MANAGER->getWorld()->CreateBody(&bodyDef);
    setBody(body);
    
    b2Filter filter;
    filter.categoryBits = PhysicsCategory::BALL;
    filter.maskBits = PHYSICS_MASK_BITS_BALL;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.1f;      // 밀도
    fixtureDef.restitution = 1.0f;  // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
    fixtureDef.friction = 0;        // 마찰력
    fixtureDef.filter = filter;
    body->CreateFixture(&fixtureDef);
}

bool Ball::beforeStep() {
    
//    if( isShoot ) {
//        isShoot = false;
//        getBody()->SetLinearVelocity(shootVelocity);
//    }
    
    return true;
}

bool Ball::afterStep() {
    
    if( !SBPhysicsObject::afterStep() ) {
        return false;
    }
    
    if( !body->IsActive() || !body->IsAwake() ) {
        return true;
    }
    
    syncBodyToNode();
    
    return true;
}

/**
 * 최초 좌표를 설정합니다
 */
void Ball::setFirstPosition(const Vec2 &p) {
    
    setPosition(p);
    
    syncNodeToBody();
    // getBody()->SetLinearVelocity(b2Vec2(0, VELOCITY_BOUNCE_DOWN));
}

void Ball::setDirection(BallDirection direction) {
    
    this->direction = direction;
    
    if( direction != BallDirection::NONE ) {
        image->setFlippedX(direction == BallDirection::LEFT);
    }
}

/**
 * 왼쪽으로 이동합니다
 */
void Ball::moveLeft() {
    
    stopMoveX(false);
    
    setDirection(BallDirection::LEFT);
    
    auto body = getBody();
    // body->ApplyLinearImpulse(force, body->GetPosition(), false);
    // body->ApplyForceToCenter(b2Vec2(100, 0), false);
    body->SetLinearVelocity(b2Vec2(VELOCITY_MOVE_LEFT, body->GetLinearVelocity().y));
    
    schedule([=](float dt) {
        body->SetLinearVelocity(b2Vec2(VELOCITY_MOVE_LEFT, body->GetLinearVelocity().y));
    }, CONTINUOUS_X_INTERVAL, SCHEDULER_CONTINUOUS_X);
}

/**
 * 오른쪽으로 이동합니다
 */
void Ball::moveRight() {
    
    stopMoveX(false);
    
    setDirection(BallDirection::RIGHT);
    
    auto body = getBody();
    body->SetLinearVelocity(b2Vec2(VELOCITY_MOVE_RIGHT, body->GetLinearVelocity().y));
    
    schedule([=](float dt) {
        body->SetLinearVelocity(b2Vec2(VELOCITY_MOVE_RIGHT, body->GetLinearVelocity().y));
    }, CONTINUOUS_X_INTERVAL, SCHEDULER_CONTINUOUS_X);
}

/**
 * X축 움직임을 멈춥니다
 */
void Ball::stopMoveX(bool resetVelocity) {
    
    setDirection(BallDirection::NONE);
    
    unschedule(SCHEDULER_CONTINUOUS_X);
    unschedule(SCHEDULER_RESET_VELOCITY_X);
    
    if( resetVelocity ) {
        auto body = getBody();
        body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
        
        // 딜레이 후 리셋
//        scheduleOnce([=](float dt) {
//            auto body = getBody();
//            body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
//        }, RESET_X_DELAY, SCHEDULER_RESET_VELOCITY_X);
    }
}

/**
 * 볼 & 브릭 충돌
 */
void Ball::onContactBlock(Ball *ball, GameTile *tile, Vec2 contactPoint) {

    // 충돌 잠금 상태일 경우 충돌 무시
    if( isCollisionLocked() ) {
        return;
    }
    
    auto block = (Block*)tile;
    
    // 충돌 횟수 업데이트
    contactCount++;
    
    // 효과음
    double now = SBSystemUtils::getCurrentTimeSeconds();
    
    if( now - jumpEffectPlayedTime > 0.1f ) {
        jumpEffectPlayedTime = now;
        // SBAudioEngine::playEffect(SOUND_JUMP);
    }
    
    // Bounce Up
    // CCLOG("Bounce Up");
    auto body = getBody();
//    body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, 0));
//    body->ApplyLinearImpulseToCenter(b2Vec2(0, VELOCITY_BOUNCE_UP / 0.1f), true);
//    body->ApplyForceToCenter(b2Vec2(0, VELOCITY_BOUNCE_UP*3), false);
    float velocityY = (block->getData().tileId == TileId::BLOCK_JUMP) ? VELOCITY_JUMP_UP : VELOCITY_BOUNCE_UP;
    body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, velocityY));
    
    // 중력 없이 바운스 하는 방법
    // Bounce Down
    unschedule(SCHEDULER_BOUNCE_DOWN);
    
    scheduleOnce([=](float dt) {
//        CCLOG("Bounce Down");
//        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x,
//                                       VELOCITY_BOUNCE_DOWN));
    }, BOUNCE_DOWN_DELAY, SCHEDULER_BOUNCE_DOWN);
}

///**
// * 볼 & 아이템 충돌
// */
//void Ball::onContactItem(Ball *ball, Game::Tile *item) {
//}
//
///**
// * 볼 & 바닥 충돌
// */
//void Ball::onContactFloor(Ball *ball) {
//
//    fallToFloor();
//}

