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
    
    BLOCK_DROP          = 10101,       // 드랍 블럭
    
    BLOCK_DEATH         = 10201,       // 데스 블럭
    BLOCK_DEATH_4D      = 10251,       // 4D 데스 블럭
    
    BLOCK_JUMP          = 10301,       // 점프 블럭
    
    BLOCK_WAVE          = 10351,
    
    BLOCK_MOVE          = 10400,       // 무브 블럭, 코드로 생성됨 (타일셋에 정의되지 않음)
    BLOCK_MOVE_START    = 10401,       // 무브 블럭 (시작)
    BLOCK_MOVE_END      = 10402,       // 무브 블럭 (종료)
};

enum class TileType {
    NONE = 0,
    FLAG,
    ITEM,
    BLOCK,
};

enum class BlockType {
    NONE = 0,
    NORMAL,
    DROP,
    DEATH,
    JUMP,
    WAVE,
    MOVE,
};

typedef cocos2d::Vec2 TilePosition;
typedef std::vector<TilePosition> TilePositionList;

#define INVALID_TILE_NUMBER         0
#define INVALID_TILE_POSITION       TilePosition(-1,-1)

struct TileData {
    TileId tileId;
    TileType type;
    BlockType blockType;
    int stage;
    int x;
    int y;
    TilePosition p;
    
    TileData(TileId _tileId): tileId(_tileId), type(TileType::NONE), blockType(BlockType::NONE) {
        setTileId(_tileId);
    }
    
    void setTileId(TileId _tileId) {
        tileId = _tileId;
        
        // 타입 설정
        int i = (int)tileId;
        
        if( i > (int)TileId::BLOCK )           type = TileType::BLOCK;
        else if( i > (int)TileId::ITEM )       type = TileType::ITEM;
        else if( i > (int)TileId::FLAG )       type = TileType::FLAG;
        
        // 블럭 타입 설정
        if( isBlock() ) {
            if( i >= (int)TileId::BLOCK_MOVE )          blockType = BlockType::MOVE;
            else if( i >= (int)TileId::BLOCK_WAVE )     blockType = BlockType::WAVE;
            else if( i >= (int)TileId::BLOCK_JUMP )     blockType = BlockType::JUMP;
            else if( i >= (int)TileId::BLOCK_DEATH )    blockType = BlockType::DEATH;
            else if( i >= (int)TileId::BLOCK_DROP )     blockType = BlockType::DROP;
            else                                        blockType = BlockType::NORMAL;
        }
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
        return type == TileType::BLOCK;
    }
    
    std::string toString() {
        auto getTypeStr = [=]() -> std::string {
            switch( type ) {
                case TileType::NONE:              return "NONE";
                case TileType::FLAG:              return "FLAG";
                case TileType::ITEM:              return "ITEM";
                case TileType::BLOCK:             return "BLOCK";
                default:                          return "";
            }
        };
        
        auto getBlockTypeStr = [=]() -> std::string {
            switch( blockType ) {
                case BlockType::NORMAL:           return "NORMAL";
                case BlockType::DROP:             return "DROP";
                case BlockType::DEATH:            return "DEATH";
                case BlockType::JUMP:             return "JUMP";
                case BlockType::WAVE:             return "WAVE";
                case BlockType::MOVE:             return "MOVE";
                default:                          return "";
            }
        };
        
        return STR_FORMAT("TileData { %d, type=%s, blockType=%s, x,y=(%d,%d) }",
                          (int)tileId, getTypeStr().c_str(), getBlockTypeStr().c_str(),
                          x, y);
    }
};

typedef std::vector<TileData> TileDataList;

#endif /* TileData_h */
