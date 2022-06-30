//
//  BlockCannon.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#include "BlockCannon.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "TileFactory.hpp"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

BlockCannon* BlockCannon::create(const TileData &data) {
    
    auto block = new BlockCannon(data);
    
    if( block && block->init() ) {
        block->autorelease();
        return block;
    }
    
    CC_SAFE_DELETE(block);
    return nullptr;
}

BlockCannon::BlockCannon(const TileData &data): Block(data) {
}

BlockCannon::~BlockCannon() {
}

bool BlockCannon::init() {
    
    if( !Block::init() ) {
        return false;
    }
    
    return true;
}

/**
 * 이미지 초기화
 */
void BlockCannon::initImage() {
    
    Block::initImage();
}

/**
 * 물리 객체 초기화
 */
void BlockCannon::initPhysics() {
    
    Block::initPhysics();
}

