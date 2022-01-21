//
//  Block.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#include "Block.hpp"

#include "Define.h"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

Block* Block::create(const TileData &data) {
    
    auto block = new Block(data);
    
    if( block && block->init() ) {
        block->autorelease();
        return block;
    }
    
    CC_SAFE_DELETE(block);
    return nullptr;
}

Block::Block(const TileData &data): GameTile(data) {
}

Block::~Block() {
    
}

bool Block::init() {
    
    if( !GameTile::init() ) {
        return false;
    }
    
    return true;
}
    
/**
 * 물리 객체 초기화
 */
void Block::initPhysics() {
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->setContactTarget(this);
    listener->onContactBlock = CC_CALLBACK_3(Block::onContactBlock, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    auto size = getContentSize();
    // size = size + Size(2,2); // 블럭끼리 겹치게 보정
    
    b2BodyDef bodyDef;
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = PHYSICS_MANAGER->getWorld()->CreateBody(&bodyDef);
    setBody(body);
    
    /*
    b2PolygonShape box;
    box.SetAsBox(PTM(size.width*0.5f), PTM(size.height*0.5f));
     
    b2Filter filter;
    filter.categoryBits = PhysicsCategory::BLOCK;
    filter.maskBits = PhysicsCategory::BALL;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = 0.1f;
    //
    fixtureDef.restitution = 1.0f;  // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
//    fixtureDef.restitution = 0;
    //
    fixtureDef.friction = 0;
    fixtureDef.filter = filter;
    body->CreateFixture(&fixtureDef);
     */
    
    float left   = PTM(-size.width*0.5f);
    float right  = PTM( size.width*0.5f);
    float bottom = PTM(-size.height*0.5f);
    float top    = PTM( size.height*0.5f);
    
    b2Vec2 vectors[4][2] = {
        { b2Vec2(left, bottom), b2Vec2(left, top) },                 // left
        { b2Vec2(right, bottom), b2Vec2(right, top) },               // right
        { b2Vec2(left, bottom), b2Vec2(right, bottom) },             // bottom
        { b2Vec2(left + PTM(1), top), b2Vec2(right - PTM(1), top) }, // top
    };
    
    PhysicsCategory categorys[] = {
        PhysicsCategory::BLOCK_SIDE,
        PhysicsCategory::BLOCK_SIDE,
        PhysicsCategory::BLOCK_SIDE,
        PhysicsCategory::BLOCK,
    };
    
    for( int i = 0; i < 4; ++i ) {
        b2Vec2 v1 = vectors[i][0];
        b2Vec2 v2 = vectors[i][1];
        
        b2EdgeShape shape;
        shape.Set(v1, v2);
        
        b2Filter filter;
        filter.categoryBits = categorys[i];
        filter.maskBits = PhysicsCategory::BALL;
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 0.1f;      // 밀도
        fixtureDef.restitution = 1.0;   // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
        fixtureDef.friction = 0;        // 마찰력
        fixtureDef.filter = filter;
        body->CreateFixture(&fixtureDef);
    }
}

/**
 * 볼 & 벽돌 충돌
 */
void Block::onContactBlock(Ball *ball, GameTile *block, Vec2 contactPoint) {
    
    // image->setVisible(!image->isVisible());
    
//    if( isBroken() ) {
//        Log::w("이미 깨진 브릭에 충돌 이벤트 발생!!").showMessageBox();
//        return false;
//    }
//
//    runBallHitAction(ball, contactPoint);
//    sufferDamage(ball, contactPoint);
}

