//
//  GameTile.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#include "GameTile.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "TileFactory.hpp"
#include "../../GameDefine.h"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

GameTile::GameTile(const TileData &data) : SBPhysicsObject(this),
data(data),
stage(StageManager::getStage(data.stage)),
physicsDef(TileFactory::createPhysicsDef(data)),
image(nullptr) {
}

GameTile::~GameTile() {
    
}

bool GameTile::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    auto stage = StageManager::getStage(data.stage);
    
    setAnchorPoint(ANCHOR_M);
    setContentSize(stage.tileSize);
    setPosition(convertTilePosition(stage, data));
    
    initImage();
    initPhysics();
    
    // sync
    syncNodeToBody();
    
    return true;
}

void GameTile::onEnter() {
    
    initGameListener();
    
    Node::onEnter();
}
    
void GameTile::cleanup() {
    
    removeListeners(this);
    
    Node::cleanup();
}
 
/**
 * 이미지 초기화
 */
void GameTile::initImage() {
    
    auto files = ResourceHelper::getTileImage(data.tileId);
    
    image = Sprite::create(files[0]);
    image->setScale(GAME_MANAGER->getMapScaleFactor());
    image->setAnchorPoint(ANCHOR_MB);
    image->setPosition(Vec2BC(getContentSize(), 0, 0));
    addChild(image);
    
    if( files.size() > 1 ) {
        float interval = 0;
        
        switch( data.tileId ) {
            case TileId::FLAG_START:            interval = 0.15f;        break;
            case TileId::ITEM_DOUBLE_JUMP:      interval = 0.5f;         break;
            case TileId::BLOCK_DEATH:           interval = 0.8f;         break;
            default:                            interval = 0.5f;         break;
        }
        
        auto animate = Animate::create(SBNodeUtils::createAnimation(files, interval));
        image->runAction(RepeatForever::create(animate));
    }
}

/**
 * 물리 객체 초기화
 */
void GameTile::initPhysics() {
    
}

/**
 * 게임 리스너 초기화
 */
void GameTile::initGameListener() {
    
//    auto listener = GameEventListener::create(this);
//    listener->onStageClear   = CC_CALLBACK_0(Tile::onStageClear, this);
//    listener->onFloorChanged = CC_CALLBACK_1(Tile::onFloorChanged, this);
//    GameManager::getEventDispatcher()->addListener(listener);
}

void GameTile::onStageClear() {
}
    
/**
 * 타일 제거 준비
 */
void GameTile::prepareRemove() {
    
    removeListeners(this);
    
    setBodyAwake(false);
    setCollisionLocked(true);
    
    b2Filter filter;
    filter.categoryBits = 0; // 0x0001;
    filter.maskBits = 0; // 0xFFFF;
    
    for( auto f = getBody()->GetFixtureList(); f; f = f->GetNext() ) {
        f->SetFilterData(filter);
    }
}

/**
 * 등장 액션
 */
void GameTile::enterWithAction() {
    
//    setScale(0);
//    runAction(ScaleTo::create(TILE_ENTER_DURATION, 1));
}

/**
 * 제거 액션
 */
void GameTile::removeWithAction() {
    
    prepareRemove();
}
 
    

