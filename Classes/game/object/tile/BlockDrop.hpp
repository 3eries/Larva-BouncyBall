//
//  BlockDrop.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/15.
//

#ifndef BlockDrop_hpp
#define BlockDrop_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "Block.hpp"

/** @class BlockDrop
 * @brief 드랍 블럭
 */
class BlockDrop: public Block {
public:
    static BlockDrop* create(const TileData &data);
    
    BlockDrop(const TileData &data);
    virtual ~BlockDrop();
    
public:
    virtual void dropEffect();
};

#endif /* BlockDrop_hpp */
