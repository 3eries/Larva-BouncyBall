//
//  Ball.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/27.
//

#include "Ball.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "CharacterManager.hpp"
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
#define HORIZONTAL_MOVE_LOCKED_DURATION        (0.1f * GAME_MANAGER->getMapScaleFactor())

#define SCHEDULER_HORIZONTAL_MOVE              "SCHEDULER_HORIZONTAL_MOVE"          // 수평 이동
#define SCHEDULER_HORIZONTAL_MOVE_LOCKED       "SCHEDULER_HORIZONTAL_MOVE_LOCKED"   // 수평 이동 잠금
#define SCHEDULER_BOUNCE_DOWN                  "SCHEDULER_BOUNCE_DOWN"

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
horizontalMoveLocked(false),
jumpEffectPlayedTime(0) {
}

Ball::~Ball() {
    
}

bool Ball::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    auto size = Size(stage.tileSize.width*0.5f, stage.tileSize.height*0.5f);
    
    setAnchorPoint(ANCHOR_M);
    setContentSize(size);
    setCascadeOpacityEnabled(true);
    
    initImage();
    initPhysics();
    
    schedule(CC_CALLBACK_1(Ball::moveHorizontal, this), SCHEDULER_HORIZONTAL_MOVE);
    
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
        image->setTexture(ResourceHelper::getCharacterImage(CHARACTER_MANAGER->getSelectedCharacter().charId));
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
    listener->onContactBlock        = CC_CALLBACK_4(Ball::onContactBlock, this);
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

/**
 * 방향을 설정합니다
 */
void Ball::setDirection(BallDirection direction) {
    
    this->direction = direction;
    
    if( direction != BallDirection::NONE ) {
        image->setFlippedX(direction == BallDirection::LEFT);
    }
}

void Ball::setDirection(Touch *touch) {
 
    bool isLeft = (touch->getLocation().x < SB_WIN_SIZE.width*0.5f);

    if( isLeft ) {
        setDirection(BallDirection::LEFT);
    } else {
        setDirection(BallDirection::RIGHT);
    }
}

/**
 * 수평 이동
 */
void Ball::moveHorizontal(float dt) {
    
    if( direction == BallDirection::NONE ) {
        return;
    }
    
    if( horizontalMoveLocked ) {
        return;
    }
    
    // 방향에 따른 수평 이동
    float x = (direction == BallDirection::LEFT) ? VELOCITY_MOVE_LEFT : VELOCITY_MOVE_RIGHT;
    
    auto body = getBody();
    body->SetLinearVelocity(b2Vec2(x, body->GetLinearVelocity().y));
    
    // body->ApplyLinearImpulse(force, body->GetPosition(), false);
    // body->ApplyForceToCenter(b2Vec2(100, 0), false);
}

/**
 * 수평 이동을 멈춥니다
 */
void Ball::stopHorizontal() {
    
    setDirection(BallDirection::NONE);
    
    auto body = getBody();
    body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
}

/**
 * 볼 <-> 블럭 충돌
 */
void Ball::onContactBlock(Ball *ball, GameTile *tile, Vec2 contactPoint, PhysicsCategory category) {

    // 충돌 잠금 상태일 경우 충돌 무시
    if( isCollisionLocked() ) {
        return;
    }
    
    auto block = (Block*)tile;
    
    switch( category ) {
        case PhysicsCategory::BLOCK_TOP:    onContactBlockTop(block);    break;
        case PhysicsCategory::BLOCK_SIDE:   onContactBlockSide(block);   break;
        default: break;
    }
}

/**
 * 볼 <-> 블럭 상단 충돌
 */
void Ball::onContactBlockTop(Block *block) {
    
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
    /*
    unschedule(SCHEDULER_BOUNCE_DOWN);
    
    scheduleOnce([=](float dt) {
        CCLOG("Bounce Down");
        body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x,
                                       VELOCITY_BOUNCE_DOWN));
    }, BOUNCE_DOWN_DELAY, SCHEDULER_BOUNCE_DOWN);
    */
}

/**
 * 볼 <-> 블럭 측면 충돌
 */
void Ball::onContactBlockSide(Block *block) {
    
    if( horizontalMoveLocked ) {
        return;
    }
    
    horizontalMoveLocked = true;
    
    scheduleOnce([=](float dt) {
        horizontalMoveLocked = false;
    }, HORIZONTAL_MOVE_LOCKED_DURATION, SCHEDULER_HORIZONTAL_MOVE_LOCKED);
}
