//
//  StageData.h
//
//  Created by seongmin hwang on 13/12/2018.
//

#ifndef StageData_h
#define StageData_h

#include "cocos2d.h"
#include "superbomb.h"

#include "TileData.h"

struct StageData {
    int                       stage;                   // 스테이지
    int                       world;                   // 월드
    cocos2d::Size             mapContentSize;          // 맵 크기
    int                       mapWidthTiles;           // 맵 가로 타일 수
    int                       mapHeightTiles;          // 맵 세로 타일 수
    cocos2d::Size             tileSize;                // 타일 크기
    TileDataList              tiles;                   // 타일 리스트
    int                       blockMoveWidth;          // 가로 무브 블럭 타일 수
    int                       blockMoveHeight;         // 세로 무브 블럭 타일 수

    StageData() : stage(0), world(0), blockMoveWidth(0), blockMoveHeight(0) {}
    
    bool isNull() const {
        return stage == 0;
    }
    
    TileData getTile(int x, int y) const {
        auto findTiles = SBCollection::find(tiles, [=](TileData tile) -> bool {
            return tile.x == x && tile.y == y;
        });
        
        if( findTiles.size() > 0 ) {
            return findTiles[0];
        }
        
        return TileData(TileId::INVALID);
    }
    
    TileData getTile(const TilePosition &p) const {
        return getTile((int)p.x, (int)p.y);
    }
    
    TileDataList getTiles(TileId tileId) const {
        return SBCollection::find(tiles, [=](TileData tile) -> bool {
            return tile.tileId == tileId;
        });
    }
    
    TileData getPairMoveEndBlock(const TileData &moveStartBlock) const {
        switch( moveStartBlock.tileId ) {
            case TileId::BLOCK_MOVE_HORIZONTAL_START: {
                for( int x = moveStartBlock.x+1; x < mapWidthTiles; ++x ) {
                    auto block = getTile(x, moveStartBlock.y);
                    
                    if( block.tileId == TileId::BLOCK_MOVE_HORIZONTAL_END ) {
                        return block;
                    }
                }
            } break;
                
            case TileId::BLOCK_MOVE_VERTICAL_START: {
                for( int y = moveStartBlock.y-1; y >= 0; --y ) {
                    auto block = getTile(moveStartBlock.x, y);
                    
                    if( block.tileId == TileId::BLOCK_MOVE_VERTICAL_END ) {
                        return block;
                    }
                }
            } break;
                
            default: break;
        }
        
        return TileData(TileId::INVALID);
    }
    
    std::string toString() {
        std::string str = "StageData {\n";
        str += STR_FORMAT("\t stage: %d\n", stage);
        str += STR_FORMAT("\t tileSize: %dx%d\n", (int)tileSize.width, (int)tileSize.height);
        str += STR_FORMAT("\t blockMoveWidth: %d, blockMoveHeight: %d\n", blockMoveWidth, blockMoveHeight);
        
//        str += "\tnumbers: ";
//        for( int n : numbers ) {
//            str += STR_FORMAT("%d,", n);
//        }
//        str += "\n";
//
//        str += STR_FORMAT("\tnumberWeightSum: %d, list: ", numberWeightSum);
//        for( int n : numberWeights ) {
//            str += STR_FORMAT("%d,", n);
//        }
//        str += "\n";
//        str += "}";
        
        return str;
    }
};

typedef std::vector<StageData> StageDataList;

static inline cocos2d::Vec2 convertTilePosition(const StageData &stage, int x, int y) {
    
    cocos2d::Vec2 pos;
    pos.y = (SB_WIN_SIZE.height - stage.mapContentSize.height) / 2;
    
    // 맵이 화면보다 작은 경우 가운데 정렬
    if( stage.mapContentSize.width < SB_WIN_SIZE.width ) {
        pos.x = (SB_WIN_SIZE.width - stage.mapContentSize.width) / 2;
    }
    
    // content size
    pos.x += x * stage.tileSize.width;
    pos.y += y * stage.tileSize.height;
    // padding
//    pos.x += x * TILE_PADDING_X;
//    pos.y += y * TILE_PADDING_Y;
    // anchor middle
    pos.x += stage.tileSize.width * 0.5f;
    pos.y += stage.tileSize.height * 0.5f;
    
    return pos;
}

static inline cocos2d::Vec2 convertTilePosition(const StageData &stage, const TilePosition &p) {
    return convertTilePosition(stage, (int)p.x, (int)p.y);
}

static inline cocos2d::Vec2 convertTilePosition(const StageData &stage, const TileData &tile) {
    return convertTilePosition(stage, tile.x, tile.y);
}

#endif /* StageData_h */
