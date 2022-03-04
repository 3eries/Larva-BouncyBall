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

#define VELOCITY_WAVE_MOVE_LEFT                (-30 * GAME_MANAGER->getMapScaleFactor())
#define VELOCITY_WAVE_MOVE_RIGHT               (30 * GAME_MANAGER->getMapScaleFactor())

#define BOUNCE_DOWN_DELAY                      0.5f

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
state(State::NONE),
stage(stage),
direction(BallDirection::NONE),
velocityLocked(false),
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
    
    image = Sprite::create(ResourceHelper::getCharacterImage(CHARACTER_MANAGER->getSelectedCharacter().charId));
    image->setScale(GAME_MANAGER->getMapScaleFactor());
    image->setAnchorPoint(ANCHOR_MB);
    image->setPosition(Vec2BC(getContentSize(), 0, 0));
    addChild(image);
    
    /*
    auto outline = superbomb::EffectSprite::create(image->getTexture());
    outline->setScale(image->getScale());
    outline->setAnchorPoint(image->getAnchorPoint());
    outline->setPosition(image->getPosition());
    addChild(outline);
    
    Vec3 color(0 / 255.0f, 255 / 255.0f, 255 / 255.0f);
    GLfloat radius = 0.01f;
    GLfloat threshold = 1.75;
    
    outline->setEffect(EffectOutline::create("shaders/example_Outline.fsh", color, radius, threshold));
    */
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

/**
 * 게임 상태를 변경합니다
 */
void Ball::setState(State state) {
    this->state = state;
}

void Ball::addState(State state) {
    this->state = (State)(this->state | state);
}

void Ball::removeState(State state) {
    this->state = (State)(this->state & ~state);
}

bool Ball::hasState(State state) {
    return (this->state & state) == state;
}

bool Ball::beforeStep() {
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
}

/**
 * 이미지 방향을 설정합니다
 */
void Ball::setImageDirection(BallDirection direction) {
    
    if( direction != BallDirection::NONE ) {
        image->setFlippedX(direction == BallDirection::LEFT);
    }
}

/**
 * 방향을 설정합니다
 */
void Ball::setDirection(BallDirection direction) {
    
    this->direction = direction;
    
    setImageDirection(direction);
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
    
    CCLOG("Ball::moveHorizontal: %d", hasState(State::WAVE));
    
    // 웨이브 상태 체크
    if( hasState(State::WAVE) ) {
        onWaveEnd(false);
        return;
    }
    
    // 방향에 따른 수평 이동
    float x = (direction == BallDirection::LEFT) ? VELOCITY_MOVE_LEFT : VELOCITY_MOVE_RIGHT;
    setLinearVelocityX(x);
    
    // body->ApplyLinearImpulse(force, body->GetPosition(), false);
    // body->ApplyForceToCenter(b2Vec2(100, 0), false);
}

/**
 * 수평 이동 잠금
 * @param duration 잠금 시간
 */
void Ball::moveHorizontalLock(float duration) {
    
    moveHorizontalUnlock();
    
    horizontalMoveLocked = true;
    
    schedule([=](float dt) {
        this->moveHorizontalUnlock();
    }, duration, SCHEDULER_HORIZONTAL_MOVE_LOCKED);
}

/**
 * 수평 이동 잠금 해제
 * @param duration 잠금 시간
 */
void Ball::moveHorizontalUnlock() {
    
    if( !horizontalMoveLocked ) {
        return;
    }
    
    horizontalMoveLocked = false;
    unschedule(SCHEDULER_HORIZONTAL_MOVE_LOCKED);
}

/**
 * 수평 이동을 멈춥니다
 */
void Ball::stopHorizontal() {
    
    setDirection(BallDirection::NONE);
    
    setLinearVelocityX(0);
}

/**
 * 웨이브 시작
 */
void Ball::onWaveStart(Block *block) {

    bool isLeft = (block->getData().tileId == TileId::BLOCK_WAVE_LEFT);
    setImageDirection(isLeft ? BallDirection::LEFT : BallDirection::RIGHT);
    
    // 웨이브 상태 추가
    addState(State::WAVE);
    
    // 중력 제거
    PHYSICS_MANAGER->getWorld()->SetGravity(b2Vec2(0,0));
    
    // Body 잠금
    setCollisionLocked(true);
    setLinearVelocity(b2Vec2(0, 0));
    
    // Step 이후에 Body 잠금 해제
    auto SCHEDULER = "AFTER_STEP";
    
    schedule([=](float dt) {
        this->unschedule(SCHEDULER);
        this->setCollisionLocked(false);
        
        // 캐릭터를 블럭 옆으로 이동
        auto blockBox = SB_BOUNDING_BOX_IN_WORLD(block);
        setBodyPosition(Vec2(blockBox.getMidX(), blockBox.getMinY() + getContentSize().height/2) +
                        Vec2(isLeft ? -stage.tileSize.width : stage.tileSize.width, 1));
        
        // 캐릭터 발사
        float velocityX = isLeft ? VELOCITY_WAVE_MOVE_LEFT : VELOCITY_WAVE_MOVE_RIGHT;
        setLinearVelocity(b2Vec2(velocityX, 0), true);
    }, SCHEDULER);
    
    // 수평 이동 잠금
    moveHorizontalLock(0.1f);
    
    SBDirector::postDelayed(this, [=] {
        this->updateImageDirection();
    }, 0.1f, false);
}

/**
 * 웨이브 종료
 */
void Ball::onWaveEnd(bool isContactBlock) {
    
    CCLOG("Ball::onWaveEnd: %s", isContactBlock ? "contact block" : "touch");
    
    removeState(State::WAVE);
    
    // 중력 생성
    PHYSICS_MANAGER->getWorld()->SetGravity(PHYSICS_GRAVITY);
    
    // 블럭과 충돌 시 가속도 제거
    if( isContactBlock ) {
        setLinearVelocity(b2Vec2(0,0));
    }
    // 방향 체크
    else {
        bool isLeft = (body->GetLinearVelocity().x < 0);
        
        if( isLeft ) {
            // Wave Left && Touch Left
            if( direction == BallDirection::LEFT ) {
                setLinearVelocityX(VELOCITY_MOVE_LEFT);
            }
            // Wave Left && Touch Right
            else {
                float duration = 0.3f * GAME_MANAGER->getMapScaleFactor();
                
                setLinearVelocityWithAction(duration, getLinearVelocityX(), 0, true);
                moveHorizontalLock(duration);
            }
        } else {
            // Wave Right && Touch Left
            if( direction == BallDirection::LEFT ) {
                float duration = 0.3f * GAME_MANAGER->getMapScaleFactor();
                
                setLinearVelocityWithAction(duration, getLinearVelocityX(), 0, true);
                moveHorizontalLock(duration);
            }
            // Wave Right && Touch Right
            else {
                setLinearVelocityX(VELOCITY_MOVE_RIGHT);
            }
        }
    }
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
    
    // 웨이브 진행중에 블럭과 충돌 시 웨이브 상태 제거
    if( hasState(State::WAVE) ) {
        onWaveEnd(true);
    }
    
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
    
    // 캐릭터 작용
    switch( block->getData().tileId ) {
        // 점프 블럭
        case TileId::BLOCK_JUMP: {
            setLinearVelocityY(VELOCITY_JUMP_UP);
        } break;
            
        // 웨이브 블럭
        case TileId::BLOCK_WAVE_RIGHT:
        case TileId::BLOCK_WAVE_LEFT: {
            onWaveStart(block);
        } break;
            
        // 기본 점프
        default: {
            setLinearVelocityY(VELOCITY_BOUNCE_UP);
        } break;
    }
    
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
    
    // 바디에 힘을 가하는 다른 방법
//    body->ApplyLinearImpulseToCenter(b2Vec2(0, VELOCITY_BOUNCE_UP / 0.1f), true);
//    body->ApplyForceToCenter(b2Vec2(0, VELOCITY_BOUNCE_UP*3), false);
}

/**
 * 볼 <-> 블럭 측면 충돌
 */
void Ball::onContactBlockSide(Block *block) {
    
    // 벽에 튕기는 거리 조절을 위해 n초간 수평 이동 잠금
    if( horizontalMoveLocked ) {
        return;
    }
    
    moveHorizontalLock(0.1f * GAME_MANAGER->getMapScaleFactor());
}

#pragma mark- LinearVelocity

/**
 * Body 가속도를 설정합니다
 */
void Ball::setLinearVelocity(const b2Vec2 &v, bool force) {
    
    if( body && (force || !velocityLocked) ) {
        body->SetLinearVelocity(v);
    }
}

void Ball::setLinearVelocityX(float x, bool force) {
    
    if( body ) {
        setLinearVelocity(b2Vec2(x, body->GetLinearVelocity().y), force);
    }
}

void Ball::setLinearVelocityY(float y, bool force) {
    
    if( body ) {
        setLinearVelocity(b2Vec2(body->GetLinearVelocity().x, y), force);
    }
}

void Ball::setLinearVelocityWithAction(float duration, float from, float to,
                                       bool isHorizontal) {
    
    velocityLocked = true;
    
    auto action = ActionFloat::create(duration, from, to, [=](float v) {
        if( isHorizontal ) {
            this->setLinearVelocityX(v, true);
        } else {
            this->setLinearVelocityY(v, true);
        }
    });
    auto callFunc = CallFunc::create([=]() {
        velocityLocked = false;
    });
    runAction(Sequence::create(action, callFunc, nullptr));
}

float Ball::getLinearVelocityX() {
    return body ? body->GetLinearVelocity().x : 0;
}

float Ball::getLinearVelocityY() {
    return body ? body->GetLinearVelocity().y : 0;
}
