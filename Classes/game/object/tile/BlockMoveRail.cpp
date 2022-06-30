//
//  BlockMoveRail.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#include "BlockMoveRail.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "TileFactory.hpp"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

BlockMoveRail* BlockMoveRail::create(const TileData &data) {
    
    auto block = new BlockMoveRail(data);
    
    if( block && block->init() ) {
        block->autorelease();
        return block;
    }
    
    CC_SAFE_DELETE(block);
    return nullptr;
}

BlockMoveRail::BlockMoveRail(const TileData &data): Block(data) {
}

BlockMoveRail::~BlockMoveRail() {
}

bool BlockMoveRail::init() {
    
    if( !Block::init() ) {
        return false;
    }
    
    return true;
}

/**
 * 이미지 초기화
 */
void BlockMoveRail::initImage() {
    
    Block::initImage();
    
    image->setOpacity(255*0.5f);
    
    // START 블럭일 때 중간 레일 생성
    switch( data.tileId ) {
        case TileId::BLOCK_MOVE_HORIZONTAL_START: {
            auto endBlock = stage.getPairMoveEndBlock(data);
            
            for( int i = 1; i < endBlock.x - data.x; ++i ) {
                auto rail = Sprite::create(getMoveBlockMiddleRailImage(data));
                rail->setScale(GAME_MANAGER->getMapScaleFactor());
                rail->setOpacity(image->getOpacity());
                rail->setAnchorPoint(ANCHOR_ML);
                rail->setPosition(Vec2ML(getContentSize(), i * stage.tileSize.width, 0));
                addChild(rail);
            }
        } break;
            
        case TileId::BLOCK_MOVE_VERTICAL_START: {
            auto endBlock = stage.getPairMoveEndBlock(data);
            
            for( int i = 1; i < data.y - endBlock.y; ++i ) {
                auto rail = Sprite::create(getMoveBlockMiddleRailImage(data));
                rail->setScale(GAME_MANAGER->getMapScaleFactor());
                rail->setOpacity(image->getOpacity());
                rail->setAnchorPoint(ANCHOR_MB);
                rail->setPosition(Vec2BC(getContentSize(), 0, i * -stage.tileSize.height));
                addChild(rail);
            }
        } break;
            
        default: break;
    }
}

/**
 * 물리 객체 초기화
 */
void BlockMoveRail::initPhysics() {
    
    Block::initPhysics();
    
    // 충돌 잠금
    setCollisionLocked(true);
}

/**
 * 무브 블럭 중간 레일 이미지를 반환합니다
 */
string BlockMoveRail::getMoveBlockMiddleRailImage(const TileData &data) {
    
    switch( data.tileId ) {
        case TileId::BLOCK_MOVE_HORIZONTAL_START: {
            return DIR_CONTENT_TILE + STR_FORMAT("tile_%05d_rail_horizontal.png", (int)TileId::BLOCK_MOVE);
        } break;
            
        case TileId::BLOCK_MOVE_VERTICAL_START: {
            return DIR_CONTENT_TILE + STR_FORMAT("tile_%05d_rail_vertical.png", (int)TileId::BLOCK_MOVE);
        } break;
            
        default: {
            return "";
        } break;
    }
}
