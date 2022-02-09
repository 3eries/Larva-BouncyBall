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
#include "ContentManager.hpp"
#include "../GameDefine.h"

class GameTile;

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
    
    void            moveLeft();
    void            moveRight();
    void            stopMoveX(bool resetVelocity = true);
    
public:
    virtual void onContactBlock(Ball *ball, GameTile *tile, cocos2d::Vec2 contactPoint);
//    virtual void onContactItem(Ball *ball, Game::Tile *item);
//    virtual void onContactFloor(Ball *ball);
    
protected:
    StageData stage;
    
    cocos2d::Sprite *image;
    CC_SYNTHESIZE_READONLY(BallDirection, direction, Direction);
    
    double jumpEffectPlayedTime; // 점프 효과음 재생된 시간
    
    // 충돌 횟수
    // 발사 후 충돌을 카운팅합니다
    // 모든 충돌 횟수는 추락 시 리셋됩니다
    size_t contactCount;
    size_t brickContactCount; // 연속된 브릭 충돌 횟수
    size_t wallContactCount;  // 연속된 벽 충돌 횟수
};

#endif /* Ball_hpp */
