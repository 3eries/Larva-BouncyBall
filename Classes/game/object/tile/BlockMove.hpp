//
//  BlockMove.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#ifndef BlockMove_hpp
#define BlockMove_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "Block.hpp"

/** @class BlockMove
 * @brief 무브 블럭
 * 맵에 정의된 START, END 블럭의 데이터로 무브 블럭을 생성합니다
 */
class BlockMove: public Block {
public:
    static BlockMove* create(const TileData &startBlock, const TileData &endBlock);
    
    BlockMove(const TileData &data,
              const TileData &startBlock, const TileData &endBlock);
    virtual ~BlockMove();
    
protected:
    virtual bool init() override;
    virtual void initImage() override;
    virtual void initPhysics() override;
    virtual void update(float dt) override;
    
    virtual void moveStart();
    
    static std::string getMoveBlockHeadImage(int world);
    static std::string getMoveBlockBodyImage(int world);
    
protected:
    TileData startBlock;
    TileData endBlock;
};

#endif /* BlockMove_hpp */
