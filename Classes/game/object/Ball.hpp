//
//  Ball.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/27.
//

#ifndef Ball_hpp
#define Ball_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "StageManager.hpp"
#include "../GameDefine.h"

class GameTile;
class Block;

class Ball: public cocos2d::Node, public SBPhysicsObject {
public:
    static Ball* create(const StageData &stage);
    virtual ~Ball();
    
protected:
    Ball(const StageData &stage);
    
    virtual bool init() override;
    virtual void cleanup() override;
    
    virtual void initImage();
    virtual void initPhysics() override;
    
public:
    virtual bool    beforeStep() override;
    virtual bool    afterStep() override;
    
    void            setFirstPosition(const cocos2d::Vec2 &p);
    void            setDirection(BallDirection direction);
    void            setDirection(cocos2d::Touch *touch);
    
    void            moveHorizontal(float dt);
    void            stopHorizontal();
    
public:
    virtual void onContactBlock(Ball *ball, GameTile *tile, cocos2d::Vec2 contactPoint,
                                PhysicsCategory category);
    virtual void onContactBlockTop(Block *block);
    virtual void onContactBlockSide(Block *block);
    
protected:
    StageData stage;
    
    cocos2d::Sprite *image;
    
    CC_SYNTHESIZE_READONLY(BallDirection, direction, Direction);
    bool horizontalMoveLocked;      // 강제 수평 이동 잠금 Flag
    
    double jumpEffectPlayedTime;    // 점프 효과음 재생된 시간
};

#endif /* Ball_hpp */
