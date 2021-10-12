//
//  PhysicsManager.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#include "PhysicsManager.hpp"

#include "../object/Ball.hpp"
#include "../object/tile/GameTile.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

static const string SCHEDULER_UPDATE                  = "PHYSICS_SCHEDULER_UPDATE";

#define DEBUG_LOG_ENABLED               0

PhysicsManager::PhysicsManager():
gameView(nullptr), world(nullptr), debugDrawView(nullptr) {
}

PhysicsManager::~PhysicsManager() {
    
    CC_SAFE_DELETE(world);
    listeners.clear();
}

b2World* PhysicsManager::initWorld() {
    
    world = new b2World(PHYSICS_GRAVITY);
    world->SetContactFilter(this);
    world->SetContactListener(this);
    
    return world;
}

/**
 * 모든 바디 제거
 */
void PhysicsManager::removeBodies() {
    
    SBPhysics::loopObjects(world, [=](SBPhysicsObject *obj) {
        obj->removeBody();
    });
    
    SBPhysics::removeBodies(world);
}

/**
 * 스케줄러 시작
 */
void PhysicsManager::startScheduler() {
    // SBScheduleHelper::schedule(CC_CALLBACK_1(PhysicsManager::update, this), this, PHYSICS_FPS, SCHEDULER_UPDATE);
    SBScheduleHelper::schedule(CC_CALLBACK_1(PhysicsManager::update, this), this, SCHEDULER_UPDATE);
}

/**
 * 스케줄러 정지
 */
void PhysicsManager::stopScheduler() {
    SBScheduleHelper::unschedule(this, SCHEDULER_UPDATE);
}

/**
 * 스케줄러 일시정지
 */
void PhysicsManager::pauseScheduler() {
    SBScheduleHelper::pause(this);
}

/**
 * 스케줄러 재게
 */
void PhysicsManager::resumeScheduler() {
    SBScheduleHelper::resume(this);
}

/**
 * 물리 세계 업데이트
 */
void PhysicsManager::update(float dt) {
    
    SBPhysics::loopObjects(world, [](SBPhysicsObject *obj) { obj->beforeStep(); });
    world->Step(PHYSICS_FPS, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    SBPhysics::loopObjects(world, [](SBPhysicsObject *obj) { obj->afterStep(); });
    
    dispatchOnUpdate();
}
 
/**
 * 충돌 여부 설정
 */
bool PhysicsManager::ShouldCollide(b2Fixture *fixtureA, b2Fixture *fixtureB) {
    
    if( !b2ContactFilter::ShouldCollide(fixtureA, fixtureB) ) {
        return false;
    }
    
    return true;
    
    /////////////////////////// 살려내
    /*
    auto userDataA = fixtureA->GetBody()->GetUserData();
    auto userDataB = fixtureB->GetBody()->GetUserData();
    
    if( !userDataA || !userDataB ) {
        return false;
    }
    
    auto objA = (SBPhysicsObject*)userDataA;
    auto objB = (SBPhysicsObject*)userDataB;
    
    if( objA->isCollisionLocked() || objB->isCollisionLocked() ) {
        return false;
    }
     */
    ///////// 살려내
    
    // 벽돌 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::BRICK, fixtureA, fixtureB);
        
        if( objs.obj1 && objs.obj2 ) {
            auto ball = (Ball*)objs.obj1;
            auto brick = (Brick*)objs.obj2;
            
            // 벽돌이 깨짐
            if( brick->isBroken() ) {
                return false;
            }
        }
    }
     */
    
    // 바닥 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::FLOOR, fixtureA, fixtureB);
        
        if( objs.obj1 && objs.obj2 ) {
        }
    }
    */
    
    return true;
}

/**
 * 충돌 시작
 * 물리 시뮬레이션의 스텝에서 두 형태가 처음으로 접촉을 시작했을 때 호출됩니다.
 * 해당 b2Contact 객체를 비활성화 하면 뒤이어 발생하는 이벤트를 무시할 수 있습니다.
 * 그렇게 하면, preSolve, postSolve 콜백 함수는 호출되지 않으며 발생한 힘(충돌)을 계산하지 않게 됩니다.
 * 그렇지만 end 콜백 함수는 여전히 호출됩니다.
 */
void PhysicsManager::BeginContact(b2Contact *contact) {
    
    dispatchOnBeginContact(contact);
    
    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();
    
    // 벽돌 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::BRICK, fixtureA, fixtureB);
        
        if( objs.obj1 && objs.obj2 ) {
            auto ball = (Ball*)objs.obj1;
            auto brick = (Brick*)objs.obj2;

            return;
        }
    }
    */
    
    // 아이템 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::ITEM, fixtureA, fixtureB);
        
        if( objs.obj1 && objs.obj2 ) {
            return;
        }
    }
     */
    
    // 벽 체크
    /*
    {
        PhysicsCategory wallCategorys[] = {
            PhysicsCategory::WALL_LEFT,
            PhysicsCategory::WALL_RIGHT,
            PhysicsCategory::WALL_TOP,
        };
        
        for( auto wallCategory : wallCategorys ) {
            auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, wallCategory, fixtureA, fixtureB);
            
            if( objs.obj1 && objs.obj2 ) {
                dispatchOnContactWall((Ball*)objs.obj1);
                return;
            }
        }
    }
     */
}

/**
 * 충돌 종료
 * 충돌 상황이 이번 스텝에서 해제됐을 때, 즉 두 바디의 접촉이 떨어졌을 때 호출됩니다.
 */
void PhysicsManager::EndContact(b2Contact *contact) {
    
#if DEBUG_LOG_ENABLED
    CCLOG("PhysicsManager::EndContact");
#endif
    
    dispatchOnEndContact(contact);
}

/**
 * 두 형태가 접촉을 계속하는 동안 호출됩니다.
 * 충돌 후 물리 계산이 되기 전입니다.
 */
void PhysicsManager::PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
    
#if DEBUG_LOG_ENABLED
    CCLOG("PhysicsManager::PreSolve contact enabled: %d", contact->IsEnabled());
#endif
    
    dispatchOnPreSolve(contact, oldManifold);
    
    // 벽돌 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::BRICK, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            auto ball = (Ball*)objs.obj1;
            auto brick = (Brick*)objs.obj2;
            
            // 벽돌이 이미 깨진 경우, 충돌 비활성화
            if( brick->isBroken() ) {
                contact->SetEnabled(false);
            }

            return;
        }
    }
    */
    
    // 아이템 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::ITEM, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            contact->SetEnabled(false);
            dispatchOnContactItem((Ball*)objs.obj1, (Game::Tile*)objs.obj2);
            
            return;
        }
    }
     */
    
    // 바닥 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::FLOOR, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            return;
        }
    }
     */
}

/**
 * 두 형태가 접촉 후, 충돌을 통한 물리 계산을 이미 처리했을 때 호출됩니다.
 * 이 콜백 함수로 충돌력을 계산할 수 있는데 Impulse에 충돌의 충격량이 전달되어 옵니다.
 */
void PhysicsManager::PostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
    
#if DEBUG_LOG_ENABLED
    CCLOG("PhysicsManager::PostSolve");
#endif
    
    dispatchOnPostSolve(contact, impulse);
    
    // 벽돌 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::BRICK, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            auto ball = (Ball*)objs.obj1;
            auto brick = (Brick*)objs.obj2;
            
            if( !brick->isBroken() ) {
                dispatchOnContactBrick(ball, brick, SBPhysics::getContactPoint(contact));
            }

            return;
        }
    }
     */
    
    // 아이템 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::ITEM, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            return;
        }
    }
     */
    
    // 바닥 체크
    /*
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::FLOOR, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            return;
        }
    }
     */
}

/**
 * 이벤트 리스너 추가
 */
void PhysicsManager::addListener(PhysicsListener *listener) {
    
    if( listener ) {
        if( listeners.contains(listener) ) {
            CCASSERT(false, "PhysicsManager::addListener error: already exists.");
            return;
        }
        
        this->listeners.pushBack(listener);
    }
}

/**
 * 이벤트 리스너 삭제
 */
void PhysicsManager::removeListener(PhysicsListener *listener) {
    
    if( listener && listeners.contains(listener) ) {
        this->listeners.eraseObject(listener);
    }
}

void PhysicsManager::removeListener(Ref *target) {
    
    if( !target ) {
        return;
    }
    
    SBCollection::remove(this->listeners, [=](PhysicsListener *listener) {
        return listener->getTarget() == target;
    });
}

void PhysicsManager::dispatchOnUpdate() {
    
    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onUpdate ) {
            listener->onUpdate();
        }
    }
}

void PhysicsManager::dispatchOnBeginContact(b2Contact *contact) {
    
    auto objA = (SBPhysicsObject*)contact->GetFixtureA()->GetBody()->GetUserData();
    auto objB = (SBPhysicsObject*)contact->GetFixtureB()->GetBody()->GetUserData();
    
    if( objA && objA->isCollisionLocked() ) return;
    if( objB && objB->isCollisionLocked() ) return;
    
    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onBeginContact ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == objA || contactTarget == objB ) {
                    listener->onBeginContact(contact);
                }
            } else {
                listener->onBeginContact(contact);
            }
        }
    }
}

void PhysicsManager::dispatchOnEndContact(b2Contact *contact) {
    
    auto objA = (SBPhysicsObject*)contact->GetFixtureA()->GetBody()->GetUserData();
    auto objB = (SBPhysicsObject*)contact->GetFixtureB()->GetBody()->GetUserData();
    
    if( objA && objA->isCollisionLocked() ) return;
    if( objB && objB->isCollisionLocked() ) return;
    
    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onEndContact ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == objA || contactTarget == objB ) {
                    listener->onEndContact(contact);
                }
            } else {
                listener->onEndContact(contact);
            }
        }
    }
}

void PhysicsManager::dispatchOnPreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
    
    auto objA = (SBPhysicsObject*)contact->GetFixtureA()->GetBody()->GetUserData();
    auto objB = (SBPhysicsObject*)contact->GetFixtureB()->GetBody()->GetUserData();
    
    if( objA && objA->isCollisionLocked() ) return;
    if( objB && objB->isCollisionLocked() ) return;
    
    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onPreSolve ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == objA || contactTarget == objB ) {
                    listener->onPreSolve(contact, oldManifold);
                }
            } else {
                listener->onPreSolve(contact, oldManifold);
            }
        }
    }
}

void PhysicsManager::dispatchOnPostSolve(b2Contact *contact, const b2ContactImpulse *impulse) {
    
    auto objA = (SBPhysicsObject*)contact->GetFixtureA()->GetBody()->GetUserData();
    auto objB = (SBPhysicsObject*)contact->GetFixtureB()->GetBody()->GetUserData();
    
    if( objA && objA->isCollisionLocked() ) return;
    if( objB && objB->isCollisionLocked() ) return;
    
    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onPostSolve ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == objA || contactTarget == objB ) {
                    listener->onPostSolve(contact, impulse);
                }
            } else {
                listener->onPostSolve(contact, impulse);
            }
        }
    }
}

void PhysicsManager::dispatchOnContactBlock(Ball *ball, GameTile *block, Vec2 contactPoint) {

    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onContactBrick ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == ball || contactTarget == block ) {
                    listener->onContactBrick(ball, block, contactPoint);
                }
            } else {
                listener->onContactBrick(ball, block, contactPoint);
            }
        }
    }
}

void PhysicsManager::dispatchOnContactItem(Ball *ball, GameTile *item) {

    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onContactItem ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == ball || contactTarget == item ) {
                    listener->onContactItem(ball, item);
                }
            } else {
                listener->onContactItem(ball, item);
            }
        }
    }
}

void PhysicsManager::dispatchOnContactWall(Ball *ball) {
    
    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onContactWall ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == ball ) {
                    listener->onContactWall(ball);
                }
            } else {
                listener->onContactWall(ball);
            }
        }
    }
}

void PhysicsManager::dispatchOnContactFloor(Ball *ball) {

    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onContactFloor ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == ball ) {
                    listener->onContactFloor(ball);
                }
            } else {
                listener->onContactFloor(ball);
            }
        }
    }
}


