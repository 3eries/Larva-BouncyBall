//
//  ClearPortal.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/01/23.
//

#include "ClearPortal.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

ClearPortal* ClearPortal::create(const TileData &data) {
    
    auto portal = new ClearPortal(data);
    
    if( portal && portal->init() ) {
        portal->autorelease();
        return portal;
    }
    
    CC_SAFE_DELETE(portal);
    return nullptr;
}

ClearPortal::ClearPortal(const TileData &data): Flag(data),
opened(false) {
}

ClearPortal::~ClearPortal() {
    
}

bool ClearPortal::init() {
    
    if( !Flag::init() ) {
        return false;
    }
    
    // 충돌 잠금
    // setCollisionLocked(true);
    
    return true;
}

void ClearPortal::initImage() {
    
    anim = SBSkeletonAnimation::create(ResourceHelper::getTileSkeletonJsonFile(data.tileId));
    anim->setScale(GAME_MANAGER->getMapScaleFactor());
    anim->setAnchorPoint(Vec2::ZERO);
    anim->setPosition(Vec2BC(getContentSize(), 0, 0));
    addChild(anim);
    
    // anim->runAnimation(ANIM_NAME_CLEAR);
    anim->setAnimation(0, "closed", true);
}

void ClearPortal::setStar(int star) {
    
    // setCollisionLocked(star >= 1);
    
    if( !opened && star >= 1 ) {
        opened = true;
        
        // 포털 오픈 연출
        anim->clearTracks();
        anim->runAnimation(ANIM_NAME_CLEAR);
        anim->runAnimation("opening", false, [=](spine::TrackEntry *entry) {
            anim->setAnimation(0, "open_idle", true);
        });
    }
}

/**
 * 볼 & Flag 충돌
 */
void ClearPortal::onContactFlag(Ball *ball, GameTile *tile) {
}
