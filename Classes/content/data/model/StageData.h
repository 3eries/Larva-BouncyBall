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
    int             stage;                   // 스테이지

    cocos2d::Size   mapContentSize;          // 맵 크기
    cocos2d::Size   mapWidthTiles;           // 맵 가로 타일 수
    cocos2d::Size   mapHeightTiles;          // 맵 세로 타일 수
    cocos2d::Size   tileSize;                // 타일 크기
    
    TileDataList    tiles;                   // 타일 리스트
    int             tileRows;                // 타일 가로줄 수
    int             tileColumns;             // 타일 세로줄 수
    
    
    StageData() : stage(0) {}
    
    void parse(const rapidjson::Value &v, rapidjson::Document::AllocatorType &allocator) {
        
        // int values
        {
            StringList keys({
                "level",
            });
            
            std::vector<int*> ptrs({
                &stage,
            });
            
            SBJSON::parse(v, allocator, keys, ptrs);
        }
        
        // tile
        auto tileList = v["tile"].GetArray();
        tileRows = tileList.Size();
        tileColumns = tileList[0].GetArray().Size();
        
        for( int i = 0; i < tileRows; ++i ) {
            auto row = tileList[i].GetArray();
            int y = tileRows - i - 1;
            
            for( int x = 0; x < row.Size(); ++x ) {
                TileData tile;
                tile.p.x = x;
                tile.p.y = y;
                
                tiles.push_back(tile);
            }
        }
    }
    
    bool isNull() const {
        return stage == 0;
    }
    
    TileData getTile(const TilePosition &p) const {
        for( auto tile : tiles ) {
            if( tile.p.equals(p) ) {
                return tile;
            }
        }
        
        TileData tile;
        tile.p = p;
        return tile;
    }
    
    TileDataList getRowTiles(int y) const {
    
        TileDataList rowTiles;
        
        for( int x = 0; x < tileColumns; ++x ) {
            rowTiles.push_back(getTile(TilePosition(x,y)));
        }
        
        return rowTiles;
    }
    
    TileDataList getColumnTiles(int x) const {
        
        TileDataList columnTiles;
        
        for( int y = 0; y < tileRows; ++y ) {
            columnTiles.push_back(getTile(TilePosition(x,y)));
        }
        
        return columnTiles;
    }
    
//    bool isTileEmpty(const TilePosition &p) const {
//        return getTile(p).isEmpty;
//    }
    
    int getTileId(const TilePosition &p) const {
        return ((int)p.x + 1) + ((int)p.y * tileColumns);
    }
    
    std::string toString() {
        std::string str = "StageData {\n";
        str += STR_FORMAT("\t stage: %d\n", stage);
        str += STR_FORMAT("\t tileSize: %dx%d\n", tileColumns, tileRows);
        
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

typedef std::vector<StageData>           StageDataList;

#endif /* StageData_h */
