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
    enum State {
        NONE                  = (1 << 0),
        DOUBLE_JUMP_READY     = (1 << 1),     // 더블 점프 대기
        DOUBLE_JUMP           = (1 << 2),     // 더블 점프 발동됨
        
        WAVE                  = (1 << 3),     // 웨이브
        WAVE_LEFT             = (1 << 4),     // 왼쪽 웨이브
        WAVE_RIGHT            = (1 << 5),     // 오른쪽 웨이브
        WAVE_HORIZONTAL_LOCK  = (1 << 6),     // 웨이브 중 수평 이동 잠금
    };
    
    enum ActionTag {
        VELOCITY = 10,
    };
    
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
    void            setState(State state);
    void            addState(State state);
    void            removeState(State state);
    bool            hasState(State state);
    
public:
    virtual void    resume() override;
    virtual bool    beforeStep() override;
    virtual bool    afterStep() override;
    
    void            setFirstPosition(const cocos2d::Vec2 &p);
    
    void            prepareTouch();
    
    void            setImageDirection(BallDirection direction);
    void            setDirection(BallDirection direction);
    bool            isLeftDirection() { return direction == BallDirection::LEFT; }
    
    void            scheduleMoveHorizontal();
    void            unscheduleMoveHorizontal();
    void            moveHorizontal(float dt);
    void            stopHorizontal(float delay);
    
public:
    void            doubleJumpStart();
    void            doubleJumpEnd(bool isContactBlock);
    
    void            waveStart(Block *block);
    void            waveEnd(bool isContactBlock);
    
public:
    void            onContactItem(Ball *ball, GameTile *tile);
    void            onContactBlock(Ball *ball, GameTile *tile, cocos2d::Vec2 contactPoint,
                                PhysicsCategory category);
    void            onContactBlockTop(Block *block);
    void            onContactBlockSide(Block *block);
    void            onContactWall(Ball *ball, PhysicsCategory category);
    
protected:
    CC_SYNTHESIZE_READONLY(State, state, State);
    StageData stage;
    
    cocos2d::Sprite *image;
    superbomb::EffectSprite *outlineImage;
    
    CC_SYNTHESIZE_READONLY(BallDirection, direction, Direction);

    double contactBlockSideTime;      // 블럭 측면 충돌 시간
    double jumpEffectPlayedTime;      // 점프 효과음 재생된 시간
    
#pragma mark- LinearVelocity
private:
    void            setLinearVelocity(const b2Vec2 &v);
    void            setLinearVelocity(float x, float y);
    void            setLinearVelocityX(float x);
    void            setLinearVelocityY(float y);
    float           getLinearVelocityX();
    float           getLinearVelocityY();
    float           getMoveVelocityX();
    
    void            setLinearVelocityWithAction(float duration, float from, float to,
                                                std::function<void(float)> onValueChanged,
                                                SBCallback onFinished = nullptr);
    void            setLinearVelocityXWithAction(float duration, float from, float to,
                                                 SBCallback onFinished = nullptr);
    void            setLinearVelocityYWithAction(float duration, float from, float to,
                                                 SBCallback onFinished = nullptr);
};

#endif /* Ball_hpp */
