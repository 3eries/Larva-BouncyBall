//
//  ClearPortal.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/01/23.
//

#include "ClearPortal.hpp"

#include "Define.h"
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
    setCollisionLocked(true);
    
    return true;
}

void ClearPortal::setStar(int star) {
    
    // setCollisionLocked(star >= 1);
    
    if( !opened && star >= 1 ) {
        opened = true;
        
        // TODO: 포털 오픈 연출
        auto blink = Blink::create(2.0f, 2);
        image->runAction(RepeatForever::create(blink));
    }
}

/**
 * 볼 & Flag 충돌
 */
void ClearPortal::onContactFlag(Ball *ball, GameTile *tile) {
}
