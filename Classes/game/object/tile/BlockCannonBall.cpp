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
    
    setPosition(getCannonBallStartPos());
    syncNodeToBody();
    scheduleUpdate();
    
    // 1초 후 시작
    setVisible(false);
    scheduleOnce([=](float dt) {
        this->setVisible(true);
        this->moveStart();
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
//    physicsDef.bottom = 0;
    
    Block::initPhysics();
}

void BlockCannonBall::update(float dt) {
    
    syncNodeToBody();
}

/**
 * 이동 시작
 */
void BlockCannonBall::moveStart() {

    auto moveStartPos = getCannonBallStartPos();
    auto moveEndPos = getCannonBallEndPos();
    auto moveRange = moveStartPos.getDistance(moveEndPos) / stage.tileSize.height; // 움직이는 칸 수
        
    CCASSERT(moveRange > 0, "BlockCannonBall::moveStart error.");
    
    auto moveDuration = (moveRange * 0.3f) * GAME_MANAGER->getMapScaleFactor(); // 1B - 0.3초
    
    auto onStart = CallFunc::create([=] {
        
        this->setPosition(moveStartPos);
        this->setCollisionLocked(false); // 충돌 ON
        image->setOpacity(255);
        
        // 파티클
        auto particle = ParticleSystemQuad::create(DIR_CONTENT_TILE + "cannon_start.plist");
        particle->setScale(GAME_MANAGER->getMapScaleFactor());
        particle->setAnchorPoint(ANCHOR_M);
        particle->setAutoRemoveOnFinish(true);
        getParent()->addChild(particle, getLocalZOrder());
        
        auto margin = cannon->getContentSize().height / 2;
        particle->setPosition(cannon->getPosition() + (getCannonBallDirection() * margin));
    });
    auto onEnd = CallFunc::create([=] {
        
        this->setCollisionLocked(true); // 충돌 OFF
        image->runAction(FadeOut::create(0.5f));
    });
    
    auto action = Sequence::create(onStart,
                                   MoveTo::create(moveDuration, moveEndPos),
                                   onEnd,
                                   DelayTime::create(1.5f),
                                   nullptr);
    runAction(RepeatForever::create(action));
}

Vec2 BlockCannonBall::getCannonBallStartPos() {
    
    auto margin = (cannon->getContentSize().height/2); // + (getContentSize().height/2);
    return cannon->getPosition() + (getCannonBallDirection() * margin);
}

Vec2 BlockCannonBall::getCannonBallEndPos() {
    
    auto dir = getCannonBallDirection();
    auto p = cannon->getData().p + dir;
    
    // 같은 열/행에 위치한 블럭이 있는지 체크, 4D 데스는 예외
    for( ; ; p += dir ) {
        // 맵을 벗어나는지 체크
        if( p.x < 0 || p.x > stage.mapWidthTiles-1 ||
            p.y < 0 || p.y > stage.mapHeightTiles-1 ) {
            // p -= dir; // rollback
            break;
        }
        
        auto tile = stage.getTile(p);
        if( !tile.isValid() || !tile.isBlock() ) {
            continue;
        }
        
        if( tile.tileId == TileId::BLOCK_DEATH_4D ) {
            continue;
        }
        
        // Find!
        break;
    }
    
    p = convertTilePosition(stage, p);
    
    auto margin = (-cannon->getContentSize().height/2) + (-getContentSize().height/2);
    return p + (getCannonBallDirection() * margin);
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
