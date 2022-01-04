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

USING_NS_CC;
USING_NS_SB;
using namespace std;

static const string SCHEDULER_CHECK_MOVEMENT = "SCHEDULER_CHECK_MOVEMENT";

Ball* Ball::create(b2World *world) {
    
    auto ball = new Ball(world);
    
    if( ball && ball->init() ) {
        ball->autorelease();
        return ball;
    }
    
    CC_SAFE_DELETE(ball);
    return nullptr;
}

Ball::Ball(b2World *world) : SBPhysicsObject(this),
world(world),
direction(BallDirection::NONE),
fall(false) {
}

Ball::~Ball() {
    
}

bool Ball::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(ANCHOR_M);
    setPosition(BALL_POSITION);
    setContentSize(BALL_SIZE);
    setCascadeOpacityEnabled(true);
    
    initImage();
    initPhysicsListener();
    
    setBody(createBody(world, (SBPhysicsObject*)this));
    syncNodeToBody();
    setBodyAwake(true);
    
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
    image->setAnchorPoint(ANCHOR_MB);
    image->setPosition(Vec2BC(BALL_SIZE, 0, -10));
    addChild(image);
    
    auto updateImage = [=]() {
        image->setTexture(DIR_IMG_GAME + "hero.png");
    };
    
    updateImage();
    
    // 볼 스킨 변경 리스너
//    auto listener = EventListenerCustom::create(DIRECTOR_EVENT_UPDATE_BALL_SKIN, [=](EventCustom *event) {
//        updateImage();
//    });
//    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

/**
 * 물리 리스너 초기화
 */
void Ball::initPhysicsListener() {
    
//    auto listener = GamePhysicsListener::create();
//    listener->setTarget(this);
//    listener->setContactTarget(this);
//    listener->onBeginContact        = CC_CALLBACK_1(Ball::onBeginContact, this);
//    listener->onEndContact          = CC_CALLBACK_1(Ball::onEndContact, this);
//    listener->onPreSolve            = CC_CALLBACK_2(Ball::onPreSolve, this);
//    listener->onPostSolve           = CC_CALLBACK_2(Ball::onPostSolve, this);
//    listener->onContactBrick        = CC_CALLBACK_3(Ball::onContactBrick, this);
//    listener->onContactItem         = CC_CALLBACK_2(Ball::onContactItem, this);
//    listener->onContactWall         = CC_CALLBACK_1(Ball::onContactWall, this);
//    listener->onContactFloor        = CC_CALLBACK_1(Ball::onContactFloor, this);
//    GameManager::getPhysicsManager()->addListener(listener);
}

b2Body* Ball::createBody(b2World *world, SBPhysicsObject *userData) {
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    bodyDef.fixedRotation = true;
    // bodyDef.awake = false;
    bodyDef.userData = userData;
    
    b2CircleShape circle;
    circle.m_radius = PTM(BALL_RADIUS);
    
    auto body = world->CreateBody(&bodyDef);
    
    b2Filter filter;
    filter.categoryBits = PhysicsCategory::BALL;
    filter.maskBits = PHYSICS_MASK_BITS_BALL;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 0.1f;      // 밀도
    fixtureDef.restitution = 1.0f;  // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
    fixtureDef.friction = 0;        // 마찰력
    fixtureDef.filter = filter;
    body->CreateFixture(&fixtureDef);
    
    return body;
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
    
    // 바디 값 동기화
    auto beforePos = getPositionX();
    syncBodyToNode();
    
    // 이미지 좌우반전
    if( getPositionX() != beforePos ) {
        bool isLeft = (getPositionX() < beforePos);
        image->setFlippedX(isLeft);
    }
    
    if( !isSyncLocked() ) {
        // setRotation(getBodyVelocityAngle());
    }
    
    return true;
}

void Ball::setDirection(BallDirection direction) {
    
    this->direction = direction;
    
    image->setFlippedX(direction == BallDirection::LEFT);
    
    // TODO: Physics
}

/**
 * 발사
 */
void Ball::shoot(b2Vec2 velocity) {
    
    contactCount = 0;
    brickContactCount = 0;
    wallContactCount = 0;
    
    // 활성화
    setFall(false);
    setBodyAwake(true);
    setCollisionLocked(false);
    setOpacity(255);
    setRotation(0);
    
    // 발사
    getBody()->SetLinearVelocity(velocity);
    
    // 움직임 체크
    schedule(CC_CALLBACK_1(Ball::checkMovement, this), PHYSICS_FPS*2, SCHEDULER_CHECK_MOVEMENT);
}

/**
 * 볼의 발사 속도를 반환합니다
 */
b2Vec2 Ball::getShootingVelocity(const Vec2 &start, const Vec2 &end, float maxVelocity) {
    
    Vec2 diff = end - start;
    
    b2Vec2 v = PTM(diff);
    v.Normalize();
    v.x *= maxVelocity;
    v.y *= maxVelocity;
    
    return v;
}

/**
 * 추락
 */
void Ball::fallToFloor() {
    
    contactCount = 0;
    brickContactCount = 0;
    wallContactCount = 0;
    unschedule(SCHEDULER_CHECK_MOVEMENT);
    
    setFall(true);
    setBodyAwake(false);
    setCollisionLocked(true);
    setSyncLocked(true);
    
    runAction(RotateTo::create(0.05f, 0));
}

void Ball::checkMovement(float dt) {
 
    auto velocity = getBody()->GetLinearVelocity();
    float angle = getBodyVelocityAngle();
    
    /*
    auto tuningVelocity = velocity;
    tuningVelocity.Normalize();
    tuningVelocity.x *= BALL_MAX_VELOCITY;
    tuningVelocity.y *= BALL_MAX_VELOCITY;
    
    CCLOG("before(%f,%f, angle:%d) -> after(%f,%f, angle:%d)",
          velocity.x, velocity.y, (int)SBPhysics::getVelocityAngle(velocity),
          tuningVelocity.x, tuningVelocity.y, (int)SBPhysics::getVelocityAngle(tuningVelocity));
    
    if( fabsf(tuningVelocity.x - velocity.x) > 0.5f ||
        fabsf(tuningVelocity.y - velocity.y) > 0.5f ) {
        CCLOG("---------> 차이 발생함!!!!!!!");
    }
    */
    
    // 수평으로 무한 반복되지 않게 조정
    // 진행 각도에 따라 강제로 힘을 가한다
    // Test Code
    /*
    if( fabsf(angle) > 85 && wallContactCount >= 2 ) {
        Log::i("force!!");
        getBody()->ApplyForceToCenter(b2Vec2(0,1), false);
    }
     */
    
    const int RANGE = 5;
    angle = fabsf(angle);
    
    if( angle >= 90-RANGE && angle <= 90+RANGE ) {
        if( brickContactCount >= 5 || wallContactCount >= 2 ) {
//            Log::i("velocity: %f,%f, angle: %f, contactCount: %d, brickContactCount: %d, wallContactCount: %d",
//                   velocity.x, velocity.y, angle,
//                   contactCount, brickContactCount, wallContactCount);
//            Log::i("force!!");
            
            getBody()->ApplyForceToCenter(b2Vec2(0, angle < 90 ? 0.5f : -0.5f), false);
        }
    }
}

/**
 * 물리 세계에서 rest 상태로 진입합니다
 */
void Ball::sleepWithAction() {
    
    setBodyAwake(false);
    
    auto fadeOut = FadeOut::create(0.1f);
    auto hide = Hide::create();
    runAction(Sequence::create(fadeOut, hide, nullptr));
}

//void Ball::onBeginContact(b2Contact *contact) {
//}
//
//void Ball::onEndContact(b2Contact *contact) {
//}
//
//void Ball::onPreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
//}
//
//void Ball::onPostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
//}
//
///**
// * 볼 & 브릭 충돌
// */
//bool Ball::onContactBrick(Ball *ball, Game::Tile *tile, Vec2 contactPoint) {
//
//    // 충돌 잠금 상태일 경우 충돌 무시
//    if( isCollisionLocked() ) {
//        return false;
//    }
//
//    // 충돌 횟수 업데이트
//    contactCount++;
//
//    // 중립 브릭은 벽 충돌로 분리
//    auto brick = (Brick*)tile;
//
//    if( brick->getData().type != BrickType::NEUTRAL ) {
//        brickContactCount++;
//        wallContactCount = 0;
//    } else {
//        brickContactCount = 0;
//        wallContactCount++;
//    }
//
//    return true;
//}
//
///**
// * 볼 & 아이템 충돌
// */
//void Ball::onContactItem(Ball *ball, Game::Tile *item) {
//}
//
///**
// * 볼 & 벽 충돌
// */
//void Ball::onContactWall(Ball *ball) {
//
//    contactCount++;
//    brickContactCount = 0;
//    wallContactCount++;
//}
//
///**
// * 볼 & 바닥 충돌
// */
//void Ball::onContactFloor(Ball *ball) {
//
//    fallToFloor();
//}

