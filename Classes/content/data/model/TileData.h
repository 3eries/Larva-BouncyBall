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
    FLAG                = 0,
    FLAG_START          = 1,           // 깃발
    FLAG_CLEAR_PORTAL   = 11,          // 클리어 포털
    
    // Item
    ITEM                = 1000,
    ITEM_SAUSAGE        = 1001,        // 소시지
    ITEM_DOUBLE_JUMP    = 1011,        // 더블 점프
    
    // Block
    BLOCK               = 10000,
    BLOCK_NORMAL        = 10001,       // 기본 블럭
    BLOCK_DROP_1        = 10101,       // 드랍 블럭
    BLOCK_DROP_2        = 10102,
    BLOCK_DEATH         = 10201,       // 데스 블럭
    BLOCK_DEATH_4D      = 10251,       // 4D 데스 블럭
    BLOCK_JUMP          = 10301,       // 점프 블럭
    BLOCK_WAVE_RIGHT    = 10351,       // 웨이브 블럭 (오른쪽)
    BLOCK_WAVE_LEFT     = 10352,       // 웨이브 블럭 (왼쪽)
    
    BLOCK_MOVE          = 10400,       // 무브 블럭
    BLOCK_MOVE_START    = 10401,       // 무브 블럭 (시작)
    BLOCK_MOVE_END      = 10402,       // 무브 블럭 (종료)
};

enum class TileType {
    NONE = 0,
    FLAG,
    ITEM,
    BLOCK,
};

#define INVALID_TILE_NUMBER         0

typedef cocos2d::Vec2 TilePosition;
typedef std::vector<TilePosition> TilePositionList;

struct TileData {
    TileId tileId;
    TileType type;
    int stage;
    int x;
    int y;
    TilePosition p;
    
    TileData(TileId _tileId): tileId(_tileId), type(TileType::NONE) {
        setTileId(_tileId);
    }
    
    void setTileId(TileId _tileId) {
        tileId = _tileId;
        
        // 타입 설정
        int i = (int)tileId;
        
        if( i > (int)TileId::BLOCK )           type = TileType::BLOCK;
        else if( i > (int)TileId::ITEM )       type = TileType::ITEM;
        else if( i > (int)TileId::FLAG )       type = TileType::FLAG;
    }
    
    void setPosition(int _x, int _y) {
        x = _x;
        y = _y;
        p = TilePosition(x,y);
    }
    
    bool isValid() const {
        return tileId != TileId::INVALID;
    }
    
    bool isBlock() const {
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
                case TileId::BLOCK_DROP_1:
                case TileId::BLOCK_DROP_2:        return "BLOCK_DROP";
                case TileId::BLOCK_DEATH:         return "BLOCK_DEATH";
                case TileId::BLOCK_DEATH_4D:      return "BLOCK_DEATH_4D";
                case TileId::BLOCK_JUMP:          return "BLOCK_JUMP";
                case TileId::BLOCK_WAVE_RIGHT:    return "BLOCK_WAVE_RIGHT";
                case TileId::BLOCK_WAVE_LEFT:     return "BLOCK_WAVE_LEFT";
                case TileId::BLOCK_MOVE_START:    return "BLOCK_MOVE_START";
                case TileId::BLOCK_MOVE_END:      return "BLOCK_MOVE_END";
                default:                          return "";
            }
        };
        
        auto getTypeStr = [](TileType type) -> std::string {
            switch( type ) {
                case TileType::NONE:              return "NONE";
                case TileType::FLAG:              return "FLAG";
                case TileType::ITEM:              return "ITEM";
                case TileType::BLOCK:             return "BLOCK";
                default:                          return "";
            }
        };
        
        return STR_FORMAT("TileData { %s, type=%s, x,y=(%d,%d) }",
                          getIdStr(tileId).c_str(), getTypeStr(type).c_str(), x, y);
    }
};

typedef std::vector<TileData> TileDataList;

#endif /* TileData_h */
