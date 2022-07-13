//
//  BlockCannonBall.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#include "BlockCannonBall.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "TileFactory.hpp"
#include "BlockCannon.hpp"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

#define ACTION_TAG_MOVE     100

BlockCannonBall* BlockCannonBall::create(BlockCannon *cannon) {
    
    auto cannonData = cannon->getData();
    
    TileData data(TileId::BLOCK_CANNON_BALL);
    data.stage = cannonData.stage;
    data.setPosition(cannonData.x, cannonData.y); // 캐논과 동일한 타일 좌표
    
    auto block = new BlockCannonBall(data, cannon);
    
    if( block && block->init() ) {
        block->autorelease();
        return block;
    }
    
    CC_SAFE_DELETE(block);
    return nullptr;
}

BlockCannonBall::BlockCannonBall(const TileData &data, BlockCannon *cannon): Block(data),
cannon(cannon) {
}

BlockCannonBall::~BlockCannonBall() {
}

bool BlockCannonBall::init() {
    
    if( !Block::init() ) {
        return false;
    }
    
    setCollisionLocked(true);
    setPosition(getCannonBallStartPos());
    syncNodeToBody();
    scheduleUpdate();
    
    // 1초 후 이동 시작
    setVisible(false);
    scheduleOnce([=](float dt) {
        this->setVisible(true);
        this->move();
    }, 1.0f, "FIRST_DELAY");
    
    return true;
}

/**
 * 이미지 초기화
 */
void BlockCannonBall::initImage() {
    
    auto contentSize = stage.tileSize * 0.5f;
    setContentSize(contentSize);
    
    image = Sprite::create(getCannonBallImage(cannon->getData()));
    image->setScale(GAME_MANAGER->getMapScaleFactor());
    image->setAnchorPoint(ANCHOR_M);
    image->setPosition(Vec2MC(getContentSize(), 0, 0));
    addChild(image);
}

/**
 * 물리 객체 초기화
 */
void BlockCannonBall::initPhysics() {
    
    physicsDef = TileFactory::createPhysicsDef(getContentSize());
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->setContactTarget(this);
    // listener->onContactBlock = CC_CALLBACK_4(Block::onContactBlock, this);
    listener->onPreSolve = CC_CALLBACK_2(BlockCannonBall::onPreSolve, this);
    listener->onContactBlockAndBlock = CC_CALLBACK_5(BlockCannonBall::onContactBlockAndBlock, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    b2BodyDef bodyDef = createBodyDef();
    bodyDef.gravityScale = 0;
    bodyDef.bullet = true;
    bodyDef.fixedRotation = true;
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = PHYSICS_MANAGER->getWorld()->CreateBody(&bodyDef);
    setBody(body);

    b2PolygonShape shape;
    shape.SetAsBox(PTM(getContentSize().width*0.5f), PTM(getContentSize().height*0.5f));
            
    b2Filter filter = createPhysicsFilter();
    filter.categoryBits = PhysicsCategory::BLOCK_DYNAMIC;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.1f;      // 밀도
    fixtureDef.restitution = 1.0;   // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
    fixtureDef.friction = 0;        // 마찰력
    fixtureDef.filter = filter;
    body->CreateFixture(&fixtureDef);
}

b2BodyDef BlockCannonBall::createBodyDef() {
    
    b2BodyDef def = Block::createBodyDef();
    def.type = b2_dynamicBody;
    return def;
}

b2Filter BlockCannonBall::createPhysicsFilter() {
    
    b2Filter filter = Block::createPhysicsFilter();
    filter.maskBits = PHYSICS_MASK_BITS_BLOCK_DYNAMIC;
    return filter;
}

void BlockCannonBall::update(float dt) {
    
    syncNodeToBody();
}

/**
 * 이동
 */
void BlockCannonBall::move() {

    auto moveStartPos = getCannonBallStartPos();
    auto moveEndPos = getCannonBallEndPos();
    auto moveRange = moveStartPos.getDistance(moveEndPos) / stage.tileSize.height; // 움직이는 칸 수
        
    CCASSERT(moveRange > 0, "BlockCannonBall::moveStart error.");
    
    auto moveDuration = (moveRange * 0.25f) * GAME_MANAGER->getMapScaleFactor(); // 1B - 0.25초
    
    auto onStart = CallFunc::create([=] {
        this->onMoveStart();
    });
    auto onEnd = CallFunc::create([=] {
        this->onMoveEnd(true);
    });
    
    auto action = Sequence::create(onStart,
                                   MoveTo::create(moveDuration, moveEndPos),
                                   onEnd,
                                   nullptr);
    action->setTag(ACTION_TAG_MOVE);
    runAction(action);
}

/**
 * 이동 시작
 */
void BlockCannonBall::onMoveStart() {
    
    setPosition(getCannonBallStartPos());
    image->setOpacity(255);
    
    // 0.1초 후 충돌 ON
    setCollisionLocked(true);
    
    SBDirector::postDelayed(this, [=]() {
        this->setCollisionLocked(false);
    }, 0.1f);
    
    // 파티클
    auto particle = ParticleSystemQuad::create(DIR_CONTENT_TILE + "cannon_start.plist");
    particle->setScale(GAME_MANAGER->getMapScaleFactor());
    particle->setAnchorPoint(ANCHOR_M);
    particle->setAutoRemoveOnFinish(true);
    getParent()->addChild(particle, getLocalZOrder());
    
    auto margin = cannon->getContentSize().height / 2;
    particle->setPosition(cannon->getPosition() + (getCannonBallDirection() * margin));
}

/**
 * 이동 종료
 */
void BlockCannonBall::onMoveEnd(bool nextMove) {
    
    stopActionByTag(ACTION_TAG_MOVE);
    
    setCollisionLocked(true); // 충돌 OFF
    image->runAction(FadeOut::create(0.2f));
    
    if( nextMove ) {
        SBDirector::postDelayed(this, [=]() {
            this->move();
        }, 1.5f);
    }
    
    // 파티클
    auto particle = ParticleSystemQuad::create(DIR_CONTENT_TILE + "cannon_end.plist");
    particle->setScale(GAME_MANAGER->getMapScaleFactor());
    particle->setAnchorPoint(ANCHOR_M);
    particle->setPosition(getPosition());
    particle->setAutoRemoveOnFinish(true);
    getParent()->addChild(particle, getLocalZOrder());
}

/**
 * 블록과 충돌 후 물리 계산 전
 */
void BlockCannonBall::onPreSolve(b2Contact *contact, const b2Manifold *oldManifold){
    
    for( int i = 0; i < PHYSICS_BLOCK_CATEGORYS.size(); ++i ) {
        auto category = PHYSICS_BLOCK_CATEGORYS[i];
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BLOCK_DYNAMIC, category, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            auto block1 = (Block*)objs.obj1;
            auto block2 = (Block*)objs.obj2;
            auto self = (block1 == this) ? block1 : block2;
            auto other = (block1 == this) ? block2 : block1;

            // 4D 데스블록은 충돌 무시
            if( other->getData().tileId == TileId::BLOCK_DEATH_4D ) {
                contact->SetEnabled(false);
            }
            
            break;
        }
    }
}

/**
 * 블록과 충돌
 */
void BlockCannonBall::onContactBlockAndBlock(GameTile *block1, GameTile *block2,
                                             Vec2 contactPoint,
                                             PhysicsCategory category1, PhysicsCategory category2) {
    
    CCLOG("BlockCannonBall::onContactBlockAndBlock");
    onMoveEnd(true);
}

Vec2 BlockCannonBall::getCannonBallStartPos() {
    
    auto margin = (cannon->getContentSize().height/2); // + (getContentSize().height/2);
    return cannon->getPosition() + (getCannonBallDirection() * margin);
}

Vec2 BlockCannonBall::getCannonBallEndPos() {
    
    auto p = cannon->getData().p;
    
    switch( cannon->getData().tileId ) {
        case TileId::BLOCK_CANNON_UP:       p.y = stage.mapHeightTiles;     break;
        case TileId::BLOCK_CANNON_DOWN:     p.y = -1;                       break;
        case TileId::BLOCK_CANNON_LEFT:     p.x = -1;                       break;
        case TileId::BLOCK_CANNON_RIGHT:    p.x = stage.mapWidthTiles;      break;
        default: break;
    }
    
    p = convertTilePosition(stage, p);

    auto margin = (-stage.tileSize.height/2) + (-getContentSize().height/2);
    return p + (getCannonBallDirection() * margin);
    
//    auto dir = getCannonBallDirection();
//    auto p = cannon->getData().p + dir;
//
//    // 같은 열/행에 위치한 블럭이 있는지 체크
//    for( ; ; p += dir ) {
//        // 맵을 벗어나는지 체크
//        if( p.x < 0 || p.x > stage.mapWidthTiles-1 ||
//            p.y < 0 || p.y > stage.mapHeightTiles-1 ) {
//            // p -= dir; // rollback
//            break;
//        }
//
//        auto tile = stage.getTile(p);
//        if( !tile.isValid() || !tile.isBlock() ) {
//            continue;
//        }
//
//        // 4D 데스는 예외
//        if( tile.tileId == TileId::BLOCK_DEATH_4D ) {
//            continue;
//        }
//
//        // Find!
//        break;
//    }
//
//    p = convertTilePosition(stage, p);
//
//    auto margin = (-cannon->getContentSize().height/2) + (-getContentSize().height/2);
//    return p + (getCannonBallDirection() * margin);
}

Vec2 BlockCannonBall::getCannonBallDirection() {
    switch( cannon->getData().tileId ) {
        case TileId::BLOCK_CANNON_UP:       return Vec2( 0,  1);
        case TileId::BLOCK_CANNON_DOWN:     return Vec2( 0, -1);
        case TileId::BLOCK_CANNON_LEFT:     return Vec2(-1,  0);
        case TileId::BLOCK_CANNON_RIGHT:    return Vec2( 1,  0);
        default:                            return Vec2::ZERO;
    }
}

/**
 * 캐논볼 이미지를 반환합니다
 */
string BlockCannonBall::getCannonBallImage(const TileData &cannonData) {
    int idx = 1;
    return DIR_CONTENT_TILE + STR_FORMAT("tile_%05d_cannon_ball_%02d.png",
                                         (int)TileId::BLOCK_CANNON, idx);
}
