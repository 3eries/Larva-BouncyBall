//
//  StageData.h
//
//  Created by seongmin hwang on 13/12/2018.
//

#ifndef StageData_h
#define StageData_h

#include "cocos2d.h"
#include "superbomb.h"

#define INVALID_TILE_NUMBER         0

typedef cocos2d::Vec2 TilePosition;
typedef std::vector<TilePosition> TilePositionList;

struct TileData {
    TilePosition p;
    bool isEmpty;
    
    TileData() : isEmpty(true) {}
};

typedef std::vector<TileData> TileDataList;

struct StageData {
    int             stage;                   // 스테이지
    int             clearCondition;          // 클리어 조건
    
    IntList         numbers;                 // 숫자 리스트
    IntList         numberWeights;           // 숫자 가중치 리스트
    int             numberWeightSum;         // 숫자 가중치 합계
    
    TileDataList    tiles;                   // 타일 리스트
    int             tileRows;                // 타일 가로줄 수
    int             tileColumns;             // 타일 세로줄 수
    
    // 가상 레벨에서 사용하는 변수
    bool            isVirtual;
    
    StageData() : stage(0), numberWeightSum(0), isVirtual(false) {}
    
    void parse(const rapidjson::Value &v, rapidjson::Document::AllocatorType &allocator) {
        
        // int values
        {
            StringList keys({
                "level",
                "clear_condition",
            });
            
            std::vector<int*> ptrs({
                &stage,
                &clearCondition,
            });
            
            SBJSON::parse(v, allocator, keys, ptrs);
        }
        
        // number_list
        auto numberList = v["number_list"].GetArray();
        
        for( int i = 0; i < numberList.Size(); ++i ) {
            // const rapidjson::Value &v = list[i];
            numbers.push_back(numberList[i].GetInt());
        }
        
        // number_weight_list
        if( v.HasMember("number_weight_list") ) {
            auto numberWeightList = v["number_weight_list"].GetArray();
            
            for( int i = 0; i < numberWeightList.Size(); ++i ) {
                numberWeights.push_back(numberWeightList[i].GetInt());
            }
        }
        
        if( numberWeights.size() == 0 ) {
            for( int i = 0; i < numberList.Size(); ++i ) {
                numberWeights.push_back(100 / numberList.Size());
            }
        }
        
        for( auto n : numberWeights ) {
            numberWeightSum += n;
        }
        
        CCASSERT(numbers.size() == numberWeights.size(), "StageData number error: 숫자 리스트와 가중치 리스트의 크기가 다릅니다.");
        
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
                tile.isEmpty = (row[x].GetInt() == 0 );
                
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
    
    bool isTileEmpty(const TilePosition &p) const {
        return getTile(p).isEmpty;
    }
    
    int getTileId(const TilePosition &p) const {
        return ((int)p.x + 1) + ((int)p.y * tileColumns);
    }
    
    std::string toString() {
        std::string str = "LevelData {\n";
        str += STR_FORMAT("\tlevel: %d, clearCondition: %d\n", stage, clearCondition);
        str += STR_FORMAT("\ttileSize: %dx%d\n", tileColumns, tileRows);
        
        str += "\tnumbers: ";
        for( int n : numbers ) {
            str += STR_FORMAT("%d,", n);
        }
        str += "\n";

        str += STR_FORMAT("\tnumberWeightSum: %d, list: ", numberWeightSum);
        for( int n : numberWeights ) {
            str += STR_FORMAT("%d,", n);
        }
        str += "\n";
        str += "}";
        
        return str;
    }
    
    std::string getTileMapString() {
        std::string str = STR_FORMAT("LEVEL %d\n", stage);
        
        for( int y = tileRows-1; y >= 0; --y ) {
            str += "[";
            
            for( int x = 0; x < tileColumns; ++x ) {
                auto tile = getTile(TilePosition(x,y));
                str += STR_FORMAT("%d", !tile.isEmpty ? 1 : 0);
            }
            
            str += "]";
            
            if( y > 0 ) {
                str += "\n";
            }
        }
        
        return str;
    }
};

typedef std::vector<StageData>           StageDataList;

#endif /* StageData_h */
