//
//  Block.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#ifndef Block_hpp
#define Block_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "GameTile.hpp"
#include "../../GameDefine.h"
    
class Ball;

/** @class Block
 * @brief 기본 블럭, 특수 블럭 등 맵에 표시되는 블럭은 Block 클래스를 상속합니다
 */
class Block: public GameTile {
public:
    static Block* create(const TileData &data);
    
    Block(const TileData &data);
    virtual ~Block();
    
protected:
    virtual bool init() override;
    virtual void initPhysics() override;
    
    virtual b2BodyDef createBodyDef();
    virtual b2Filter  createPhysicsFilter();
    
public:
    virtual void onContactBlock(Ball *ball, GameTile *block, cocos2d::Vec2 contactPoint,
                                PhysicsCategory category);
    virtual void onContactBlockAndBlock(GameTile *block1, GameTile *block2,
                                        cocos2d::Vec2 contactPoint,
                                        PhysicsCategory category1, PhysicsCategory category2);
};

#endif /* Block_hpp */
