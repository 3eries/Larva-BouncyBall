//
//  BlockMove.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#include "BlockMove.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "TileFactory.hpp"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

BlockMove* BlockMove::create(const TileData &startBlock, const TileData &endBlock) {
    
    TileData data(TileId::BLOCK_MOVE);
    data.stage = startBlock.stage;
    data.setPosition(startBlock.x, startBlock.y); // START 블럭과 동일한 타일 좌표
    
    auto block = new BlockMove(data, startBlock, endBlock);
    
    if( block && block->init() ) {
        block->autorelease();
        return block;
    }
    
    CC_SAFE_DELETE(block);
    return nullptr;
}

BlockMove::BlockMove(const TileData &data,
                     const TileData &startBlock, const TileData &endBlock): Block(data),
startBlock(startBlock),
endBlock(endBlock) {
}

BlockMove::~BlockMove() {
}

bool BlockMove::init() {
    
    if( !Block::init() ) {
        return false;
    }
    
    syncNodeToBody();
    scheduleUpdate();
    
    moveStart();
    
    return true;
}

/**
 * 이미지 초기화
 */
void BlockMove::initImage() {
    
    auto width = stage.blockMoveWidth * stage.tileSize.width;
    
    setPosition(Vec2((getPositionX() - getContentSize().width/2) + width/2,
                     getPositionY()));
    setContentSize(Size(width, getContentSize().height));
    
    // head
    auto head = Sprite::create(getMoveBlockHeadImage(stage.world));
    head->setScale(GAME_MANAGER->getMapScaleFactor());
    head->setAnchorPoint(ANCHOR_ML);
    head->setPosition(Vec2ML(getContentSize(), 0, 0));
    addChild(head);
    
    // body
    for( int i = 0; i < stage.blockMoveWidth-1; ++i ) {
        auto body = Sprite::create(getMoveBlockBodyImage(stage.world));
        body->setScale(GAME_MANAGER->getMapScaleFactor());
        body->setAnchorPoint(ANCHOR_ML);
        body->setPosition(Vec2ML(getContentSize(), (i+1) * stage.tileSize.width, 0));
        addChild(body);
    }
}

/**
 * 물리 객체 초기화
 */
void BlockMove::initPhysics() {
    
    physicsDef = TileFactory::createPhysicsDef(getContentSize());
    physicsDef.bottom = 0;
    
    Block::initPhysics();
}

void BlockMove::update(float dt) {
    
    syncNodeToBody();
}

/**
 * 이동 시작
 */
void BlockMove::moveStart() {

    auto endBlockPos = convertTilePosition(stage, endBlock).x;
    
    auto moveStartPos = getPositionX();
    auto moveEndPos = (endBlockPos + stage.tileSize.width/2) - getContentSize().width/2;
    auto moveRange = (endBlock.x - startBlock.x + 1) - stage.blockMoveWidth; // 움직이는 칸 수
    
    auto duration = (moveRange * 0.6f) * GAME_MANAGER->getMapScaleFactor();
    auto move = Sequence::create(MoveTo::create(duration, Vec2(moveEndPos, getPositionY())),
                                 MoveTo::create(duration, Vec2(moveStartPos, getPositionY())), nullptr);
    runAction(RepeatForever::create(move));
}

/**
 * 무브 블럭 헤드 이미지를 반환합니다
 */
string BlockMove::getMoveBlockHeadImage(int world) {
    return DIR_CONTENT_TILE + STR_FORMAT("tile_%05d_head_%02d.png", (int)TileId::BLOCK_MOVE, world);
}

/**
 * 무브 블럭 헤드 이미지를 반환합니다
 */
string BlockMove::getMoveBlockBodyImage(int world) {
    return DIR_CONTENT_TILE + STR_FORMAT("tile_%05d_body_%02d.png", (int)TileId::BLOCK_MOVE, world);
}
