//
//  Block.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#include "Block.hpp"

#include "Define.h"
#include "../../GameDefine.h"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

Block* Block::create() {
    
    auto block = new Block(1,1);
    
    if( block && block->init() ) {
        block->autorelease();
        return block;
    }
    
    CC_SAFE_DELETE(block);
    return nullptr;
}

Block::Block(int rows, int columns): GameTile(rows, columns),
image(nullptr) {
}

Block::~Block() {
    
}

bool Block::init() {
    
    if( !GameTile::init() ) {
        return false;
    }
    
    setAnchorPoint(ANCHOR_M);
    setContentSize(BLOCK_SIZE);
    
    initImage();
    initPhysics();
    
    // sync
    syncNodeToBody();
    
    return true;
}

void Block::onEnter() {
    
    GameTile::onEnter();
}
    
void Block::cleanup() {
    
    GameTile::cleanup();
}

/**
 * 이미지 초기화
 */
void Block::initImage() {
    
    image = Sprite::create(DIR_IMG_GAME + "block.png");
    image->setAnchorPoint(ANCHOR_M);
    image->setPosition(Vec2MC(BLOCK_SIZE, 0, 0));
    addChild(image);
}
    
/**
 * 물리 객체 초기화
 */
void Block::initPhysics() {
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->setContactTarget(this);
    listener->onPreSolve     = CC_CALLBACK_2(Block::onPreSolve, this);
    listener->onContactBlock = CC_CALLBACK_3(Block::onContactBlock, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    auto size = BLOCK_SIZE;
    size = size + Size(2,2); // 블럭끼리 겹치게 보정
    
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
        { b2Vec2(left, bottom), b2Vec2(left, top) },          // left
        { b2Vec2(right, bottom), b2Vec2(right, top) },        // right
        { b2Vec2(left, bottom), b2Vec2(right, bottom) },      // bottom
        { b2Vec2(left + PTM(1), top), b2Vec2(right - PTM(1), top) },            // top
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

void Block::onStageClear() {
}

void Block::onPreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
}

/**
 * 볼 & 벽돌 충돌
 */
bool Block::onContactBlock(Ball *ball, GameTile *block, Vec2 contactPoint) {
    
    // image->setVisible(!image->isVisible());
    
//    if( isBroken() ) {
//        Log::w("이미 깨진 브릭에 충돌 이벤트 발생!!").showMessageBox();
//        return false;
//    }
//
//    runBallHitAction(ball, contactPoint);
//    sufferDamage(ball, contactPoint);
    
    return true;
}

