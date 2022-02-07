//
//  Flag.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#include "Flag.hpp"

#include "Define.h"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

Flag* Flag::create(const TileData &data) {
    
    auto Flag = new class Flag(data);
    
    if( Flag && Flag->init() ) {
        Flag->autorelease();
        return Flag;
    }
    
    CC_SAFE_DELETE(Flag);
    return nullptr;
}

Flag::Flag(const TileData &data): GameTile(data) {
}

Flag::~Flag() {
    
}

bool Flag::init() {
    
    if( !GameTile::init() ) {
        return false;
    }
    
    return true;
}
    
/**
 * 물리 객체 초기화
 */
void Flag::initPhysics() {
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->setContactTarget(this);
    listener->onContactFlag = CC_CALLBACK_2(Flag::onContactFlag, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    auto size = getContentSize();
    
    b2BodyDef bodyDef;
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = PHYSICS_MANAGER->getWorld()->CreateBody(&bodyDef);
    setBody(body);
    
    b2PolygonShape shape;
    shape.SetAsBox(PTM(size.width*0.5f), PTM(size.height*0.5f));
     
    b2Filter filter;
    filter.categoryBits = PhysicsCategory::FLAG;
    filter.maskBits = PhysicsCategory::BALL;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 0.1f;      // 밀도
    fixtureDef.restitution = 1.0;   // 반발력 - 물체가 다른 물체에 닿았을때 팅기는 값
    fixtureDef.friction = 0;        // 마찰력
    fixtureDef.filter = filter;
    body->CreateFixture(&fixtureDef);
}

/**
 * 볼 & Flag 충돌
 */
void Flag::onContactFlag(Ball *ball, GameTile *tile) {
}
