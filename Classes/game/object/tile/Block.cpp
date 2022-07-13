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
    listener->onContactBlock = CC_CALLBACK_4(Block::onContactBlock, this);
    listener->onContactBlockAndBlock = CC_CALLBACK_5(Block::onContactBlockAndBlock, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    b2BodyDef bodyDef = createBodyDef();
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = PHYSICS_MANAGER->getWorld()->CreateBody(&bodyDef);
    setBody(body);
    
    const float left   = physicsDef.left;
    const float right  = physicsDef.right;
    const float bottom = physicsDef.bottom;
    const float top    = physicsDef.top;
    
    b2Vec2 vectors[4][2] = {
        { b2Vec2(left, bottom), b2Vec2(left, top) },                 // left
        { b2Vec2(right, bottom), b2Vec2(right, top) },               // right
        { b2Vec2(left, bottom), b2Vec2(right, bottom) },             // bottom
        { b2Vec2(left + PTM(1), top), b2Vec2(right - PTM(1), top) }, // top
    };
    
    PhysicsCategory categorys[] = {
        PhysicsCategory::BLOCK_SIDE,
        PhysicsCategory::BLOCK_SIDE,
        PhysicsCategory::BLOCK_BOTTOM,
        PhysicsCategory::BLOCK_TOP,
    };
    
    for( int i = 0; i < 4; ++i ) {
        b2Vec2 v1 = vectors[i][0];
        b2Vec2 v2 = vectors[i][1];
        
        b2EdgeShape shape;
        shape.Set(v1, v2);
        
        b2Filter filter = createPhysicsFilter();
        filter.categoryBits = categorys[i];
        
        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 0.1f;      // 밀도
        fixtureDef.restitution = 1.0;   // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
        fixtureDef.friction = 0;        // 마찰력
        fixtureDef.filter = filter;
        body->CreateFixture(&fixtureDef);
    }
}

b2BodyDef Block::createBodyDef() {
    
    b2BodyDef def;
    return def;
}

b2Filter Block::createPhysicsFilter() {
    
    b2Filter filter;
    filter.maskBits = PHYSICS_MASK_BITS_BLOCK;
    return filter;
}

/**
 * 볼 & 블록 충돌
 */
void Block::onContactBlock(Ball *ball, GameTile *block, Vec2 contactPoint, PhysicsCategory category) {
}


/**
 * 블록 & 블록 충돌
 */
void Block::onContactBlockAndBlock(GameTile *block1, GameTile *block2,
                                   Vec2 contactPoint,
                                   PhysicsCategory category1, PhysicsCategory category2) {
}
