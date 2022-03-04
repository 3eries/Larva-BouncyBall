//
//  BlockDrop.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/15.
//

#include "BlockDrop.hpp"

#include "Define.h"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

BlockDrop* BlockDrop::create(const TileData &data) {
    
    auto block = new BlockDrop(data);
    
    if( block && block->init() ) {
        block->autorelease();
        return block;
    }
    
    CC_SAFE_DELETE(block);
    return nullptr;
}

BlockDrop::BlockDrop(const TileData &data): Block(data) {
}

BlockDrop::~BlockDrop() {
}

/**
 * 드랍 연출
 */
void BlockDrop::dropEffect() {
    
    auto tileSize = StageManager::getStage(data.stage).tileSize;
    auto imgBox = SB_BOUNDING_BOX_IN_WORLD(image);
    
    auto pos = Vec2(imgBox.getMidX(), imgBox.getMidY());
    pos = getParent()->convertToNodeSpace(pos);
    
    auto effectImg = Sprite::createWithTexture(image->getTexture());
    effectImg->setScale(image->getScale());
    effectImg->setAnchorPoint(ANCHOR_M);
    effectImg->setPosition(pos);
    getParent()->addChild(effectImg, getLocalZOrder());
    
    auto move = MoveTo::create(0.3f, pos + Vec2(0, -tileSize.height));
    auto fade = FadeOut::create(0.3f);
    auto seq = Sequence::create(Spawn::create(move, fade, nullptr),
                                RemoveSelf::create(), nullptr);
    effectImg->runAction(seq);
}
