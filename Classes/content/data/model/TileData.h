//
//  TileData.h
//  LarvaBouncyBall
//
//  Created by seongmin hwang on 2022/01/17.
//

#ifndef TileData_h
#define TileData_h

#include "cocos2d.h"
#include "superbomb.h"

enum class TileId {
    INVALID             = -1,          // 유효하지 않음
    NONE                = 0,           // 빈 칸
    
    // Flag
    FLAG_START          = 11,          // 깃발
    FLAG_CLEAR_PORTAL,                 // 클리어 포털
    
    // Item
    ITEM_SAUSAGE        = 1001,        // 소시지
    ITEM_DOUBLE_JUMP,                  // 더블 점프
    
    // Block
    BLOCK_NORMAL        = 10001,       // 기본 블럭
    BLOCK_BREKING_1,                   // 깨지는 블럭
    BLOCK_BREKING_2,                   // 깨지는 블럭
    BLOCK_GAME_OVER,                   // 게임 오버 블럭
    BLOCK_JUMP,                        // 점프 블럭
};

#define INVALID_TILE_NUMBER         0

typedef cocos2d::Vec2 TilePosition;
typedef std::vector<TilePosition> TilePositionList;

struct TileData {
    TileId tileId;
    int stage;
    int x;
    int y;
    TilePosition p;
    
    TileData(TileId _tileId): tileId(_tileId) {}
    
    void setPosition(int _x, int _y) {
        x = _x;
        y = _y;
        p = TilePosition(x,y);
    }
    
    bool isValid() const {
        return tileId != TileId::INVALID;
    }
    
    bool isBlockType() const {
        return (int)tileId >= (int)TileId::BLOCK_NORMAL;
    }
    
    std::string toString() {
        auto getIdStr = [](TileId tileId) -> std::string {
            switch( tileId ) {
                case TileId::INVALID:             return "INVALID";
                case TileId::NONE:                return "NONE";
                case TileId::FLAG_START:          return "FLAG_START";
                case TileId::FLAG_CLEAR_PORTAL:   return "FLAG_CLEAR_PORTAL";
                case TileId::ITEM_SAUSAGE:        return "ITEM_SAUSAGE";
                case TileId::ITEM_DOUBLE_JUMP:    return "ITEM_DOUBLE_JUMP";
                case TileId::BLOCK_NORMAL:        return "BLOCK_NORMAL";
                case TileId::BLOCK_BREKING_1:
                case TileId::BLOCK_BREKING_2:     return "BLOCK_BREKING";
                case TileId::BLOCK_JUMP:          return "BLOCK_JUMP";
                default:                            return "";
            }
        };
        
        return STR_FORMAT("TileData { %s, x,y=(%d,%d) }", getIdStr(tileId).c_str(), x, y);
    }
};

typedef std::vector<TileData> TileDataList;

#endif /* TileData_h */
