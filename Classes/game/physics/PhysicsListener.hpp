//
//  PhysicsListener.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#ifndef PhysicsListener_hpp
#define PhysicsListener_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "../GameDefine.h"

class GameTile;
class Ball;

class PhysicsListener: public cocos2d::Ref {
public:
    SB_REF_CREATE_FUNC(PhysicsListener);
    ~PhysicsListener() {}
    
public:
    // 물리 세계 업데이트
    std::function<void()>                                    onUpdate;
    
    std::function<void(b2Contact*)>                          onBeginContact;
    std::function<void(b2Contact*)>                          onEndContact;
    std::function<void(b2Contact*, const b2Manifold*)>       onPreSolve;
    std::function<void(b2Contact*, const b2ContactImpulse*)> onPostSolve;
    
    std::function<void(Ball*,GameTile*)>                     onContactFlag;
    std::function<void(Ball*,GameTile*)>                     onContactItem;
    std::function<void(Ball*,GameTile*,
                       cocos2d::Vec2,PhysicsCategory)>       onContactBlock;
    
    std::function<void(Ball*,PhysicsCategory)>               onContactWall;
    std::function<void(Ball*)>                               onContactFloor;
    
private:
    PhysicsListener() : target(nullptr), contactTarget(nullptr),
    onUpdate(nullptr),
    onBeginContact(nullptr), onEndContact(nullptr), onPreSolve(nullptr), onPostSolve(nullptr),
    onContactFlag(nullptr), onContactItem(nullptr), onContactBlock(nullptr),
    onContactWall(nullptr), onContactFloor(nullptr) {}
    
    CC_SYNTHESIZE(cocos2d::Ref*, target, Target);
    
    // 충돌 타겟
    // 타겟이 속한 충돌일 경우만 충돌 리스너 실행
    // null인 경우, 검사 없이 리스너 실행
    CC_SYNTHESIZE(SBPhysicsObject*, contactTarget, ContactTarget);
};

#endif /* PhysicsListener_hpp */
