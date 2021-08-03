//
//  SBPhysics.hpp
//
//  Created by seongmin hwang on 15/11/2018.
//

#ifndef SBPhysics_hpp
#define SBPhysics_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../base/SBMacros.h"
#include "DebugDrawView.hpp"

#define PTM_RATIO               32.0f

static inline float  PTM(float v)                 { return v / PTM_RATIO; }
static inline b2Vec2 PTM(float x, float y)        { return b2Vec2(PTM(x), PTM(y)); }
static inline b2Vec2 PTM(const cocos2d::Vec2 &p)  { return PTM(p.x, p.y); }

static inline float         MTP(float v)          { return v * PTM_RATIO; }
static inline cocos2d::Vec2 MTP(float x, float y) { return cocos2d::Vec2(MTP(x), MTP(y)); }
static inline cocos2d::Vec2 MTP(const b2Vec2 &p)  { return MTP(p.x, p.y); }

#pragma mark- SBPhysics

class SBPhysicsObject;

class SBPhysics {
public:
    static void loopBodies(b2World *world, std::function<void(b2Body*)> callback);
    static void loopUserDatas(b2World *world, std::function<void(void*)> callback);
    static void loopObjects(b2World *world, std::function<void(SBPhysicsObject*)> callback);
    
    static void removeBodies(b2World *world);
    
    static void syncNodeToBody(cocos2d::Node *node, b2Body *body);
    static void syncBodyToNode(b2Body *body, cocos2d::Node *node);
    
    static cocos2d::Vec2 getContactPoint(b2Contact *contact);
    static float getVelocityAngle(const b2Vec2 &velocity);
    
// findCollisionObjects
public:
    struct CollisionObjects {
        SBPhysicsObject *obj1;
        SBPhysicsObject *obj2;
        
        CollisionObjects() : obj1(nullptr), obj2(nullptr) {}
    };
    
    static CollisionObjects findCollisionObjects(uint16 categoryBits1, uint16 categoryBits2,
                                                 b2Fixture *fixtureA, b2Fixture *fixtureB);
    static CollisionObjects findCollisionObjects(uint16 categoryBits1, uint16 categoryBits2,
                                                 b2Contact *contact);
};

#pragma mark- SBPhysicsObject

class SBPhysicsObject {
public:
    SBPhysicsObject(cocos2d::Node *node);
    ~SBPhysicsObject();
    
public:
    virtual void initPhysics();
    virtual cocos2d::Size getPhysicsSize();
    
    virtual void removeBody();
    
    virtual bool beforeStep();
    virtual bool afterStep();
    
    virtual void beginContact(b2Fixture *other, b2Contact *contact) {};
    virtual void endContact(b2Fixture *other, b2Contact *contact) {};
    
    virtual void setNeedRemove(bool isNeedRemove);
        
    virtual void setBodyActive(bool isActive);
    virtual void setBodyAwake(bool isAwake);
    
    bool isBodyActive();
    bool isBodyAwake();
    
    virtual void setBodyPosition(const cocos2d::Vec2 &p);
    float        getBodyVelocityAngle();
    
    virtual void syncNodeToBody();
    virtual void syncBodyToNode();
    
protected:
    cocos2d::Node *node;
    CC_SYNTHESIZE(b2Body*, body, Body);
    
    SB_SYNTHESIZE_BOOL(collisionLocked, CollisionLocked);   // 충돌 잠금 여부
    SB_SYNTHESIZE_BOOL(syncLocked, SyncLocked);             // 동기화 잠금 여부
    SB_SYNTHESIZE_READONLY_BOOL(needRemove, NeedRemove);    // 제거 여부
};

#endif /* SBPhysics_hpp */
