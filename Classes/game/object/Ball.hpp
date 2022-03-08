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
        WAVE                  = (1 << 3),     // 웨이브 진행중
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
    virtual bool    beforeStep() override;
    virtual bool    afterStep() override;
    
    void            setFirstPosition(const cocos2d::Vec2 &p);
    
    void            setImageDirection(BallDirection direction);
    void            updateImageDirection() { setImageDirection(direction); }
    
    void            setDirection(BallDirection direction);
    void            setDirection(cocos2d::Touch *touch);
    bool            isLeftDirection() { return direction == BallDirection::LEFT; }
    
    void            moveHorizontal(float dt);
    void            moveHorizontalLock(float duration, bool infinity = false);
    void            moveHorizontalUnlock();
    void            stopHorizontal();
    
public:
    void            doubleJumpStart();
    void            doubleJumpEnd();
    
    void            waveStart(Block *block);
    void            waveEnd(bool isContactBlock);
    
public:
    virtual void onContactItem(Ball *ball, GameTile *tile);
    virtual void onContactBlock(Ball *ball, GameTile *tile, cocos2d::Vec2 contactPoint,
                                PhysicsCategory category);
    virtual void onContactBlockTop(Block *block);
    virtual void onContactBlockSide(Block *block);
    
protected:
    CC_SYNTHESIZE_READONLY(State, state, State);
    StageData stage;
    
    cocos2d::Sprite *image;
    superbomb::EffectSprite *outlineImage;
    
    CC_SYNTHESIZE_READONLY(BallDirection, direction, Direction);

    bool horizontalMoveLocked;      // 수평 이동 잠금 Flag
    
    double jumpEffectPlayedTime;    // 점프 효과음 재생된 시간
    
#pragma mark- LinearVelocity
private:
    void            setLinearVelocity(const b2Vec2 &v, bool force = false);
    void            setLinearVelocityX(float x, bool force = false);
    void            setLinearVelocityY(float y, bool force = false);
    void            setLinearVelocityWithAction(float duration, float from, float to,
                                                bool isHorizontal);
    float           getLinearVelocityX();
    float           getLinearVelocityY();
    
    bool velocityLocked; // 가속도 잠금 Flag
};

#endif /* Ball_hpp */
