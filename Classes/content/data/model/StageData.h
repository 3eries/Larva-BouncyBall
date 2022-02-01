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

    StageData() : stage(0) {}
    
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
        
        return TileData(TileType::INVALID);
    }
    
    TileData getTile(const TilePosition &p) const {
        return getTile((int)p.x, (int)p.y);
    }
    
    TileDataList getTiles(TileType type) const {
        return SBCollection::find(tiles, [=](TileData tile) -> bool {
            return tile.type == type;
        });
    }
    
    std::string toString() {
        std::string str = "StageData {\n";
        str += STR_FORMAT("\t stage: %d\n", stage);
        str += STR_FORMAT("\t tileSize: %dx%d\n", (int)tileSize.width, (int)tileSize.height);
        
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
    
    std::string getTileMapString() {
        std::string str = STR_FORMAT("LEVEL %d\n", stage);
        
//        for( int y = tileRows-1; y >= 0; --y ) {
//            str += "[";
//
//            for( int x = 0; x < tileColumns; ++x ) {
//                auto tile = getTile(TilePosition(x,y));
//                str += STR_FORMAT("%d", !tile.isEmpty ? 1 : 0);
//            }
//
//            str += "]";
//
//            if( y > 0 ) {
//                str += "\n";
//            }
//        }
        
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
