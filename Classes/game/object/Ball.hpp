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
#include "../GameDefine.h"

class GameTile;

class Ball: public cocos2d::Node, public SBPhysicsObject {
public:
    static Ball* create(b2World *world);
    virtual ~Ball();
    
protected:
    Ball(b2World *world);
    
    virtual bool init() override;
    virtual void cleanup() override;
    
    virtual void initImage();
    virtual void initPhysics() override;
    
public:
    static b2Body*  createBody(b2World *world, SBPhysicsObject *userData = nullptr);
    
    virtual bool    beforeStep() override;
    virtual bool    afterStep() override;
    
    void            setFirstPosition(const cocos2d::Vec2 &p);
    void            setDirection(BallDirection direction);
    
    void            moveLeft();
    void            moveRight();
    void            stopMoveX(bool resetVelocity = true);
    
    virtual void    shoot(b2Vec2 velocity);
    
    static  b2Vec2  getShootingVelocity(const cocos2d::Vec2 &start, const cocos2d::Vec2 &end,
                                        float maxVelocity);
    virtual void    fallToFloor();
    
    virtual void    checkMovement(float dt);
    
    virtual void    sleepWithAction();
    
public:
    virtual bool onContactBlock(Ball *ball, GameTile *tile, cocos2d::Vec2 contactPoint);
//    virtual void onContactItem(Ball *ball, Game::Tile *item);
//    virtual void onContactWall(Ball *ball);
//    virtual void onContactFloor(Ball *ball);
    
protected:
    CC_SYNTHESIZE_READONLY(b2World*, world, World);
    cocos2d::Sprite *image;
    CC_SYNTHESIZE_READONLY(BallDirection, direction, Direction);
    
    SB_SYNTHESIZE_BOOL(fall, Fall);   // 추락 여부
    
    // 충돌 횟수
    // 발사 후 충돌을 카운팅합니다
    // 모든 충돌 횟수는 추락 시 리셋됩니다
    size_t contactCount;
    size_t brickContactCount; // 연속된 브릭 충돌 횟수
    size_t wallContactCount;  // 연속된 벽 충돌 횟수
};

#endif /* Ball_hpp */
