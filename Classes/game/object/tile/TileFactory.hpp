//
//  TileFactory.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/15.
//

#ifndef TileFactory_hpp
#define TileFactory_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "StageManager.hpp"
#include "../../GameDefine.h"

class TileFactory {
public:
    static PhysicsDef createPhysicsDef(const cocos2d::Size &tileSize);
    static PhysicsDef createPhysicsDef(const TileData &data);
};

#endif /* TileFactory_hpp */
