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
    static Block* create();
    
    Block(int rows, int columns);
    virtual ~Block();
    
protected:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void cleanup() override;
    
    virtual void initImage();
    virtual void initPhysics() override;
    
    virtual void onStageClear() override;
    
public:
    virtual void    onPreSolve(b2Contact *contact, const b2Manifold *oldManifold);
    virtual bool    onContactBlock(Ball *ball, GameTile *block, cocos2d::Vec2 contactPoint);
    
protected:
    cocos2d::Sprite *image;
};

#endif /* Block_hpp */
