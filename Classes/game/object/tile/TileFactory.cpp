//
//  TileFactory.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/15.
//

#include "TileFactory.hpp"

USING_NS_CC;
using namespace std;

PhysicsDef TileFactory::createPhysicsDef(const Size &tileSize) {
    
    PhysicsDef def;
    def.size   = PTM(tileSize);
    def.left   = PTM(-tileSize.width*0.5f);
    def.right  = PTM( tileSize.width*0.5f);
    def.bottom = PTM(-tileSize.height*0.5f);
    def.top    = PTM( tileSize.height*0.5f);
    
    return def;
}
    
PhysicsDef TileFactory::createPhysicsDef(const TileData &data) {
    
    auto stage = StageManager::getStage(data.stage);
    auto tileSize = stage.tileSize;
    // tileSize = tileSize + Size(2,2); // 블럭끼리 겹치게 보정
    
    switch( data.tileId ) {
        case TileId::ITEM_SAUSAGE: {
            tileSize.width *= 0.7f;
        } break;
        
        case TileId::ITEM_DOUBLE_JUMP: {
            tileSize.width *= 0.7f;
            tileSize.height *= 0.7f;
        } break;
            
        case TileId::BLOCK_DEATH: {
            tileSize.width *= 0.5f;
        } break;
            
        case TileId::BLOCK_DEATH_4D: {
            tileSize.width *= 0.5f;
            tileSize.height *= 0.5f;
        } break;
        
        default: break;
    }
    
    PhysicsDef def = createPhysicsDef(tileSize);
    
    if( data.tileId == TileId::BLOCK_DROP_1 ||
        data.tileId == TileId::BLOCK_DROP_2 ) {
        def.bottom = 0;
    }
    
    return def;
}
