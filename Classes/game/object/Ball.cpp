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

#include "tile/Item.hpp"
#include "tile/Block.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define VELOCITY_BOUNCE_UP                     (22 * GAME_MANAGER->getMapScaleFactor())     // 1.3B
#define VELOCITY_JUMP_UP                       (37 * GAME_MANAGER->getMapScaleFactor())     // 3.25B

#define VELOCITY_MOVE_LEFT                     (-13 * GAME_MANAGER->getMapScaleFactor())    // 2.5B
#define VELOCITY_MOVE_RIGHT                    (13 * GAME_MANAGER->getMapScaleFactor())     // 2.5B

#define VELOCITY_DOUBLE_JUMP_LEFT              (-28 * GAME_MANAGER->getMapScaleFactor())    // 6B
#define VELOCITY_DOUBLE_JUMP_RIGHT             (28 * GAME_MANAGER->getMapScaleFactor())     // 6B
// #define VELOCITY_DOUBLE_JUMP_UP                (3 * GAME_MANAGER->getMapScaleFactor())
#define VELOCITY_DOUBLE_JUMP_UP                (12 * GAME_MANAGER->getMapScaleFactor())     // 0.B

#define VELOCITY_WAVE_MOVE_LEFT                (-30 * GAME_MANAGER->getMapScaleFactor())
#define VELOCITY_WAVE_MOVE_RIGHT               (30 * GAME_MANAGER->getMapScaleFactor())

#define SCHEDULER_HORIZONTAL_MOVE              "SCHEDULER_HORIZONTAL_MOVE"          // 수평 이동
#define SCHEDULER_HORIZONTAL_MOVE_LOCKED       "SCHEDULER_HORIZONTAL_MOVE_LOCKED"   // 수평 이동 잠금
#define SCHEDULER_DOUBLE_JUMP_END              "SCHEDULER_DOUBLE_JUMP_END"          // 더블 점프 종료
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
    
    // 더블 점프 효과에 필요한 아웃라인 이미지 생성
    const int OUTLINE_PIXEL = 4;
    
    auto createTexture = [=]() -> cocos2d::Texture2D* {
      
        auto img = Sprite::createWithTexture(image->getTexture());
        img->setAnchorPoint(ANCHOR_MB);
        img->setPosition(Vec2BC(img->getContentSize(), 0, OUTLINE_PIXEL));
        
        auto rt = RenderTexture::create(img->getContentSize().width,
                                        img->getContentSize().height + OUTLINE_PIXEL);
        rt->begin();
        img->visit();
        rt->end();
        
        return rt->getSprite()->getTexture();
    };
    
    outlineImage = superbomb::EffectSprite::create(createTexture());
    outlineImage->setVisible(false);
    outlineImage->setFlippedY(true); // for RenderTexture
    outlineImage->setAnchorPoint(ANCHOR_M);
    outlineImage->setPosition(Vec2MC(image->getContentSize(), 0, 0));
    image->addChild(outlineImage);

    Vec3 color(0, 255, 255);
    color = color / 255.0f;

    GLfloat radius = OUTLINE_PIXEL / 255.0f;
    GLfloat threshold = 1;

    outlineImage->setEffect(EffectOutline::create("shaders/example_Outline.fsh", color, radius, threshold));
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
    listener->onContactItem         = CC_CALLBACK_2(Ball::onContactItem, this);
    listener->onContactWall         = CC_CALLBACK_2(Ball::onContactWall, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.bullet = true;
    bodyDef.fixedRotation = true;
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = PHYSICS_MANAGER->getWorld()->CreateBody(&bodyDef);
    setBody(body);
    
//    b2CircleShape shape;
//    shape.m_radius = PTM(getContentSize().height*0.5f);
    b2PolygonShape shape;
    shape.SetAsBox(PTM(getContentSize().width*0.5f), PTM(getContentSize().height*0.5f));
    
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
    
    // 자연스로운 충돌을 위한 Shape
    {
//        auto boxSize = Size(stage.tileSize.width * 0.7f, getContentSize().height * 0.5f);
//
//        b2PolygonShape shape2;
////        shape2.SetAsBox(PTM(boxSize.width * 0.5f), PTM(boxSize.height * 0.5f),
////                        PTM(Vec2(0, (getContentSize().height * 0.5f) + (boxSize.height * 0.5f))), 0);
//        shape2.SetAsBox(PTM(boxSize.width * 0.5f), PTM(boxSize.height * 0.5f),
//                        PTM(Vec2(0, 0)), 0);
//
//        b2FixtureDef fixtureDef2 = fixtureDef;
//        fixtureDef2.shape = &shape2;
//        body->CreateFixture(&fixtureDef2);
    }
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
        outlineImage->setFlippedX(image->isFlippedX());
    }
}

/**
 * 방향을 설정합니다
 */
void Ball::setDirection(BallDirection direction) {
    
    this->direction = direction;
    
    CCLOG("방향 설정: %s", (isLeftDirection() ? "LEFT" : "RIGHT"));
    
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
    
    // 웨이브 상태 체크
    if( hasState(State::WAVE) ) {
        waveEnd(false);
        return;
    }
    
    // 방향에 따른 수평 이동
    setLinearVelocityX(getMoveVelocityX());
    
    // body->ApplyLinearImpulse(force, body->GetPosition(), false);
    // body->ApplyForceToCenter(b2Vec2(100, 0), false);
}

/**
 * 수평 이동 잠금
 * @param duration 잠금 시간
 */
void Ball::moveHorizontalLock(float duration, bool infinity) {
    
    moveHorizontalUnlock();
    
    horizontalMoveLocked = true;
    
    if( !infinity ) {
        schedule([=](float dt) {
            this->moveHorizontalUnlock();
        }, duration, SCHEDULER_HORIZONTAL_MOVE_LOCKED);
    }
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
 * 더블 점프 시작
 */
void Ball::doubleJumpStart() {
    
    CCLOG("더블 점프! %f", body->GetAngularVelocity());
    
    removeState(State::DOUBLE_JUMP_READY);
    addState(State::DOUBLE_JUMP);
    
    // 효과음
    SBAudioEngine::playEffect(SOUND_DOUBLE_JUMP);
    
    // 아웃 라인 제거
    outlineImage->setVisible(false);
    
    // 가속도 잠금
    velocityLocked = true;
    moveHorizontalLock(0, true);
    
    // 가속도 강제 변경
    float x = isLeftDirection() ? VELOCITY_DOUBLE_JUMP_LEFT : VELOCITY_DOUBLE_JUMP_RIGHT;
    // float y = fabsf(getLinearVelocityY()) + VELOCITY_DOUBLE_JUMP_UP;
    float y = VELOCITY_DOUBLE_JUMP_UP;
    setLinearVelocity(x, y, true);
    
    // 더블 점프 종료
    unschedule(SCHEDULER_DOUBLE_JUMP_END);
    
    schedule([=](float dt) {
        CCLOG("더블 점프 종료 시간~!");
        this->doubleJumpEnd(false);
    }, 0.7f, SCHEDULER_DOUBLE_JUMP_END);
}

/**
 * 더블 점프 종료
 */
void Ball::doubleJumpEnd(bool isContactBlock) {
    
    if( !hasState(State::DOUBLE_JUMP) ) {
        return;
    }
    
    CCLOG("더블 점프 종료");
    unschedule(SCHEDULER_DOUBLE_JUMP_END);
    
    removeState(State::DOUBLE_JUMP);
    
    velocityLocked = false;
    moveHorizontalUnlock();
    
    // 블럭과 충돌 시 가속도 제거
    if( isContactBlock ) {
        setLinearVelocityX(0);
    } else {
        if( direction != BallDirection::NONE ) {
            moveHorizontal(0);
        }
    }
}

/**
 * 웨이브 시작
 */
void Ball::waveStart(Block *block) {

    SBAudioEngine::playEffect(SOUND_WAVE);
    
    bool isLeft = ((int)block->getData().tileId % 2 == 0);
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
    const float horizontalLockDuration = 0.1f;
    
    moveHorizontalLock(horizontalLockDuration);
    
    SBDirector::postDelayed(this, [=] {
        this->updateImageDirection();
    }, horizontalLockDuration, false);
}

/**
 * 웨이브 종료
 */
void Ball::waveEnd(bool isContactBlock) {
    
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
 * 볼 <-> 아이템 충돌
 */
void Ball::onContactItem(Ball *ball, GameTile *tile) {
    
    auto item = dynamic_cast<Item*>(tile);
   
    switch( item->getData().tileId ) {
        // 더블 점프
        case TileId::ITEM_DOUBLE_JUMP: {
            addState(State::DOUBLE_JUMP_READY);
            
            // 아웃 라인 생성
            outlineImage->setVisible(true);
        } break;
            
        default: break;
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
    
    // 웨이브 종료
    if( hasState(State::WAVE) ) {
        waveEnd(true);
    }
    // 더블 점프 종료
    else if( hasState(State::DOUBLE_JUMP) ) {
        doubleJumpEnd(true);
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
    
    auto playEffectSound = [=]() {
        double now = SBSystemUtils::getCurrentTimeSeconds();
        
        if( now - jumpEffectPlayedTime > 0.1f ) {
            jumpEffectPlayedTime = now;
            SBAudioEngine::playEffect(SOUND_JUMP);
        }
    };
    
    switch( block->getData().blockType ) {
        // 점프 블럭
        case BlockType::JUMP: {
            playEffectSound();
            setLinearVelocity(getMoveVelocityX(), VELOCITY_JUMP_UP);
        } break;
            
        // 웨이브 블럭
        case BlockType::WAVE: {
            waveStart(block);
        } break;
            
        // 기본 점프
        default: {
            playEffectSound();
            setLinearVelocity(getMoveVelocityX(), VELOCITY_BOUNCE_UP);
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

/**
 * 볼 <-> 벽 충돌
 */
void Ball::onContactWall(Ball *ball, PhysicsCategory category) {
    
    if( category != PhysicsCategory::WALL_LEFT && category != PhysicsCategory::WALL_RIGHT ) {
        return;
    }
    
    // 웨이브 종료
    if( hasState(State::WAVE) ) {
        waveEnd(true);
    }
    // 더블 점프 종료
    else if( hasState(State::DOUBLE_JUMP) ) {
        doubleJumpEnd(true);
    }
}

#pragma mark- LinearVelocity

/**
 * Body 가속도를 설정합니다
 */
void Ball::setLinearVelocity(const b2Vec2 &v, bool force) {
    
    if( body && (force || !velocityLocked) ) {
        // CCLOG("setLinearVelocity: %f,%f", v.x, v.y);
        
        if( direction == BallDirection::LEFT ) {
            if( v.x > 0 ) {
                CCLOG("왼쪽 방향인데 오른쪽으로 힘 가해졌당");
            }
        }
        else if( direction == BallDirection::RIGHT ){
            if( v.x < 0 ) {
                CCLOG("오른쪽 방향인데 왼쪽으로 힘 가해졌당");
            }
        }
        
        body->SetLinearVelocity(v);
    }
}

void Ball::setLinearVelocity(float x, float y, bool force) {
    
    setLinearVelocity(b2Vec2(x,y), force);
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
    
    stopActionByTag(ActionTag::VELOCITY);
    
    velocityLocked = true;
    
    auto valueChanged = ActionFloat::create(duration, from, to, [=](float v) {
        if( isHorizontal ) {
            this->setLinearVelocityX(v, true);
        } else {
            this->setLinearVelocityY(v, true);
        }
    });
    auto callFunc = CallFunc::create([=]() {
        velocityLocked = false;
    });
    
    auto action = Sequence::create(valueChanged, callFunc, nullptr);
    action->setTag(ActionTag::VELOCITY);
    runAction(action);
}

float Ball::getLinearVelocityX() {
    return body ? body->GetLinearVelocity().x : 0;
}

float Ball::getLinearVelocityY() {
    return body ? body->GetLinearVelocity().y : 0;
}

float Ball::getMoveVelocityX() {
    switch( direction ) {
        case BallDirection::LEFT:       return VELOCITY_MOVE_LEFT;
        case BallDirection::RIGHT:      return VELOCITY_MOVE_RIGHT;
        default:                        return 0;
    }
}
