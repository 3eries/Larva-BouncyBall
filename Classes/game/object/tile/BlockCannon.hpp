//
//  BlockCannon.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#ifndef BlockCannon_hpp
#define BlockCannon_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "Block.hpp"

/** @class BlockCannon
 * @brief 발사대 블럭
 */
class BlockCannon: public Block {
public:
    static BlockCannon* create(const TileData &data);
    
    BlockCannon(const TileData &data);
    virtual ~BlockCannon();
    
protected:
    virtual bool init() override;
    virtual void initImage() override;
    virtual void initPhysics() override;
};

#endif /* BlockCannon_hpp */
