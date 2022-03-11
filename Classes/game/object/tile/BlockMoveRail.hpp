//
//  BlockMoveRailRail.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#ifndef BlockMoveRailRail_hpp
#define BlockMoveRailRail_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "Block.hpp"

/** @class BlockMoveRail
 * @brief 무브 블럭 - 레일
 * 중간 레일은 START 블럭에 포함됩니다
 */
class BlockMoveRail: public Block {
public:
    static BlockMoveRail* create(const TileData &data);
    
    BlockMoveRail(const TileData &data);
    virtual ~BlockMoveRail();
    
protected:
    virtual bool init() override;
    virtual void initImage() override;
    virtual void initPhysics() override;
    
    static std::string getMoveBlockMiddleRailImage();
};

#endif /* BlockMoveRailRail_hpp */
