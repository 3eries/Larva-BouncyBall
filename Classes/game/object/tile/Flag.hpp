//
//  Flag.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#ifndef Flag_hpp
#define Flag_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "GameTile.hpp"
#include "../../GameDefine.h"
    
class Ball;

/** @class Flag
 * @brief 설명
 */
class Flag: public GameTile {
public:
    static Flag* create(const TileData &data);
    
    Flag(const TileData &data);
    virtual ~Flag();
    
protected:
    virtual bool init() override;
    virtual void initPhysics() override;
    
public:
    virtual void onContactFlag(Ball *ball, GameTile *tile);
};

#endif /* Flag_hpp */
