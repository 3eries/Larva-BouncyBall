//
//  PhysicsManager.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#ifndef PhysicsManager_hpp
#define PhysicsManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "../GameDefine.h"
#include "PhysicsListener.hpp"

class GameTile;
class Ball;

/** @class PhysicsManager
 * @brief 이 클래스는 물리 세계의 오브젝트 동기화, 충돌 검사를 담당합니다.
 */
class PhysicsManager : public cocos2d::Ref, public b2ContactFilter, public b2ContactListener {
public:
    PhysicsManager();
    ~PhysicsManager();
    
    b2World* initWorld();
    
    void removeBodies();
    
public:
    void startScheduler();
    void stopScheduler();
    void pauseScheduler();
    void resumeScheduler();
    
    void update(float dt);
    
private:
    // b2ContactFilter
    bool ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB) override;
    
    // b2ContactListener
    void BeginContact(b2Contact *contact) override;
    void EndContact(b2Contact *contact) override;
    void PreSolve(b2Contact *contact, const b2Manifold *oldManifold) override;
    void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) override;
    
public:
    void addListener(PhysicsListener *listener);
    void removeListener(PhysicsListener *listener);
    void removeListener(cocos2d::Ref *target);

private:
    void dispatchOnUpdate();
    void dispatchOnBeginContact(b2Contact *contact);
    void dispatchOnEndContact(b2Contact *contact);
    void dispatchOnPreSolve(b2Contact *contact, const b2Manifold *oldManifold);
    void dispatchOnPostSolve(b2Contact *contact, const b2ContactImpulse *impulse);
    
    void dispatchOnContactFlag(Ball *ball, GameTile *item);
    void dispatchOnContactItem(Ball *ball, GameTile *item);
    void dispatchOnContactBlock(Ball *ball, GameTile *block, cocos2d::Vec2 contactPoint);
    
    void dispatchOnContactWall(Ball *ball);
    void dispatchOnContactFloor(Ball *ball);
    
private:
    CC_SYNTHESIZE(cocos2d::Node*, gameView, GameView);
    CC_SYNTHESIZE_READONLY(b2World*, world, World);
    CC_SYNTHESIZE(DebugDrawView*, debugDrawView, DebugDrawView);
    
    cocos2d::Vector<PhysicsListener*> listeners;
};

#endif /* PhysicsManager_hpp */
