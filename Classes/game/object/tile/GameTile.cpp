//
//  GameTile.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#include "GameTile.hpp"

#include "Define.h"
#include "../../GameDefine.h"
#include "../../GameManager.hpp"

USING_NS_CC;
using namespace std;

GameTile::GameTile(int rows, int columns) : SBPhysicsObject(this),
rows(rows),
columns(columns),
available(true) {
}

GameTile::~GameTile() {
    
}

bool GameTile::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(ANCHOR_M);
    // setContentSize(MEASURE_TILE_SIZE(rows, columns));
    
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
    
    available = false;
    
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

/**
 * 타일 좌표 설정
 */
/*
void Tile::setTilePosition(const TilePosition &tilePos, bool action, SBCallback onActionFinished) {
    
    CCASSERT(tilePos != INVALID_TILE_POSITION, "Tile::setTilePosition error.");
    
    this->tilePos = tilePos;
    
    Vec2 p = convertToTilePosition(tilePos, rows, columns);
    
    if( action ) {
        auto move = MoveTo::create(TILE_MOVE_DURATION, p);
        auto callFunc = CallFunc::create([=]() {
            this->syncNodeToBody();
            
            if( onActionFinished ) {
                onActionFinished();
            }
        });
        runAction(Sequence::create(move, callFunc, nullptr));
        
    } else {
        setPosition(p);
        syncNodeToBody();
    }
}
*/

void GameTile::setTilePosition(const TilePosition &tilePos) {
    
    CCASSERT(tilePos != INVALID_TILE_POSITION, "Tile::setTilePosition error.");
    
    this->tilePos = tilePos;
    
    setPosition(convertToTilePosition(tilePos, rows, columns));
    syncNodeToBody();
}
 
void GameTile::moveWithAction(const TilePosition &tilePos, float duration, SBCallback onActionFinished) {
    
    CCASSERT(tilePos != INVALID_TILE_POSITION, "Tile::moveWithAction error.");
    
    this->tilePos = tilePos;
    
    auto move = MoveTo::create(duration, convertToTilePosition(tilePos, rows, columns));
    auto callFunc = CallFunc::create([=]() {
        this->syncNodeToBody();
        
        if( onActionFinished ) {
            onActionFinished();
        }
    });
    runAction(Sequence::create(move, callFunc, nullptr));
}
    
/**
 * 좌표가 타일에 포함됐는지 반환합니다
 */
bool GameTile::isContainsPosition(const TilePosition &p) {
    
    return p.x >= tilePos.x && p.x <= tilePos.x + (rows-1) &&
           p.y >= tilePos.y && p.y <= tilePos.y + (columns-1);
}
    

