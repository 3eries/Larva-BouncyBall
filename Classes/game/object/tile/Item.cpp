//
//  Item.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#include "Item.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

Item* Item::create(const TileData &data) {
    
    auto Item = new class Item(data);
    
    if( Item && Item->init() ) {
        Item->autorelease();
        return Item;
    }
    
    CC_SAFE_DELETE(Item);
    return nullptr;
}

Item::Item(const TileData &data): GameTile(data) {
}

Item::~Item() {
    
}

bool Item::init() {
    
    if( !GameTile::init() ) {
        return false;
    }
    
    return true;
}

void Item::initImage() {
    
    auto skeletonJsonFile = ResourceHelper::getTileSkeletonJsonFile(data.tileId);
    
    // 스파인 애니메이션
    if( skeletonJsonFile != "" ) {
        anim = SBSkeletonAnimation::create(skeletonJsonFile);
        anim->setScale(GAME_MANAGER->getMapScaleFactor());
        anim->setAnchorPoint(Vec2::ZERO);
        anim->setPosition(Vec2BC(getContentSize(), 0, 0));
        addChild(anim);
        
        anim->setAnimation(0, ANIM_NAME_RUN, true);
    }
    // 이미지
    else {
        GameTile::initImage();
    }
    
    // 소시지 파티클
    if( data.tileId == TileId::ITEM_SAUSAGE ) {
        auto particle = ParticleSystemQuad::create(DIR_IMG_GAME + "steam_particle_texture.plist");
        particle->setAnchorPoint(ANCHOR_M);
        particle->setPosition(Vec2MC(getContentSize(), 0, 25));
        addChild(particle);
    }
}
    
/**
 * 물리 객체 초기화
 */
void Item::initPhysics() {
    
    // Listener
    auto listener = PhysicsListener::create();
    listener->setTarget(this);
    listener->setContactTarget(this);
    listener->onContactItem = CC_CALLBACK_2(Item::onContactItem, this);
    PHYSICS_MANAGER->addListener(listener);
    
    // Body
    auto size = getContentSize();
    // size = size + Size(2,2); // 블럭끼리 겹치게 보정
    
    if( data.tileId == TileId::ITEM_SAUSAGE ) {
        size.width *= 0.7f;
    }
    
    physicsSize = size;
    
    b2BodyDef bodyDef;
    bodyDef.userData = (SBPhysicsObject*)this;
    
    auto body = PHYSICS_MANAGER->getWorld()->CreateBody(&bodyDef);
    setBody(body);
    
    b2PolygonShape shape;
    shape.SetAsBox(PTM(size.width*0.5f), PTM(size.height*0.5f));
     
    b2Filter filter;
    filter.categoryBits = PhysicsCategory::ITEM;
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
 * 볼 & 아이템 충돌
 */
void Item::onContactItem(Ball *ball, GameTile *Item) {
}
