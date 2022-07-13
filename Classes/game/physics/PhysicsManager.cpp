//
//  PhysicsManager.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#include "PhysicsManager.hpp"

#include "../GameManager.hpp"

#include "../object/Ball.hpp"
#include "../object/tile/GameTile.hpp"
#include "../object/tile/Block.hpp"

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
    if( !GAME_MANAGER->hasState(GameState::TUTORIAL_PAUSE) ) {
        SBScheduleHelper::resume(this);
    }
}

/**
 * 물리 세계 업데이트
 */
void PhysicsManager::update(float dt) {
    
    SBPhysics::loopObjects(world, [](SBPhysicsObject *obj) { obj->beforeStep(); });
    world->Step(PHYSICS_FPS, VELOCITY_ITERATIONS, POSITION_ITERATIONS);
    // world->Step(MIN(dt, PHYSICS_FPS), VELOCITY_ITERATIONS, POSITION_ITERATIONS);
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
    
#if DEBUG_LOG_ENABLED
    CCLOG("PhysicsManager::BeginContact");
#endif
    
    dispatchOnBeginContact(contact);
    
    auto fixtureA = contact->GetFixtureA();
    auto fixtureB = contact->GetFixtureB();
    
    // 블럭 상단 체크
//    {
//        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::BLOCK_TOP, fixtureA, fixtureB);
//
//        if( objs.obj1 && objs.obj2 ) {
//            return;
//        }
//    }
//
//    // 아이템 체크
//    {
//        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::ITEM, fixtureA, fixtureB);
//
//        if( objs.obj1 && objs.obj2 ) {
//            return;
//        }
//    }
    
    // 벽 체크
    {
        PhysicsCategory wallCategorys[] = {
            PhysicsCategory::WALL_LEFT,
            PhysicsCategory::WALL_RIGHT,
            PhysicsCategory::WALL_TOP,
        };
        
        for( auto wallCategory : wallCategorys ) {
            auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, wallCategory, fixtureA, fixtureB);
            
            if( objs.obj1 && objs.obj2 ) {
                dispatchOnContactWall((Ball*)objs.obj1, wallCategory);
                return;
            }
        }
    }
    
    // 바닥 체크
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::FLOOR, fixtureA, fixtureB);
        
        if( objs.obj1 && objs.obj2 ) {
            contact->SetEnabled(false);
            dispatchOnContactFloor((Ball*)objs.obj1);
            return;
        }
    }
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
    
    // 깃발 체크
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::FLAG, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            contact->SetEnabled(false);
            dispatchOnContactFlag((Ball*)objs.obj1, (GameTile*)objs.obj2);
            
            return;
        }
    }
    
    // 아이템 체크
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::ITEM, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            contact->SetEnabled(false);
            dispatchOnContactItem((Ball*)objs.obj1, (GameTile*)objs.obj2);
            
            return;
        }
    }
    
    // 블럭 체크
//    {
//        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::BLOCK_TOP, contact);
//
//        if( objs.obj1 && objs.obj2 ) {
//            auto ball = (Ball*)objs.obj1;
//            auto block = (Block*)objs.obj2;
//
//            // 벽돌이 이미 깨진 경우, 충돌 비활성화
//            /*
//            if( brick->isBroken() ) {
//                contact->SetEnabled(false);
//            }
//             */
//
//            return;
//        }
//    }
//
//    // 바닥 체크
//    {
//        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::FLOOR, contact);
//
//        if( objs.obj1 && objs.obj2 ) {
//            return;
//        }
//    }
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
    
    // 블럭 & 볼 체크
    {
        for( auto category : PHYSICS_BLOCK_CATEGORYS ) {
            auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, category, contact);
            
            if( objs.obj1 && objs.obj2 ) {
                auto ball = (Ball*)objs.obj1;
                auto block = (Block*)objs.obj2;
                
                dispatchOnContactBlock(ball, block, SBPhysics::getContactPoint(contact), category);

                return;
            }
        }
    }
    
    // 블럭 & 블럭 체크
    {
        for( int i = 0; i < PHYSICS_BLOCK_CATEGORYS.size(); ++i ) {
            auto category1 = PHYSICS_BLOCK_CATEGORYS[i];
            
            for( int j = 0; j < PHYSICS_BLOCK_CATEGORYS.size(); ++j ) {
                auto category2 = PHYSICS_BLOCK_CATEGORYS[j];
                auto objs = SBPhysics::findCollisionObjects(category1, category2, contact);
                
                if( objs.obj1 && objs.obj2 ) {
                    auto block1 = (Block*)objs.obj1;
                    auto block2 = (Block*)objs.obj2;
                    
                    dispatchOnContactBlockAndBlock(block1, block2,
                                                   SBPhysics::getContactPoint(contact),
                                                   category1, category2);
                    return;
                }
            }
        }
    }
    
    // 아이템 체크
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::ITEM, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            return;
        }
    }
    
    // 바닥 체크
    {
        auto objs = SBPhysics::findCollisionObjects(PhysicsCategory::BALL, PhysicsCategory::FLOOR, contact);
        
        if( objs.obj1 && objs.obj2 ) {
            return;
        }
    }
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

void PhysicsManager::dispatchOnContactFlag(Ball *ball, GameTile *flag) {

    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onContactFlag ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == ball || contactTarget == flag ) {
                    listener->onContactFlag(ball, flag);
                }
            } else {
                listener->onContactFlag(ball, flag);
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

void PhysicsManager::dispatchOnContactBlock(Ball *ball, GameTile *block, Vec2 contactPoint, PhysicsCategory category) {

    auto copyListeners = listeners;
    
//    auto ballBox = SB_BOUNDING_BOX_IN_WORLD(ball);
//    auto blockBox = SB_BOUNDING_BOX_IN_WORLD(block);
//
//    Rect physicsBox;
//    physicsBox.size = block->getPhysicsSize();
//    physicsBox.origin = Vec2(blockBox.getMidX(), blockBox.getMidY()) - Vec2(physicsBox.size*0.5f);
//
//    auto contactDirection = [=]() -> ContactDirection {
//
//        auto ballCenter = Vec2(ballBox.getMidX(), ballBox.getMidY());
//
//        auto t1 = Vec2(physicsBox.getMinX(), physicsBox.getMaxY()).getDistance(contactPoint);
////        auto t2 = Vec2(physicsBox.getMidX(), physicsBox.getMaxY()).getDistance(contactPoint);
////        auto t3 = Vec2(physicsBox.getMaxX(), physicsBox.getMaxY()).getDistance(contactPoint);
//
//        auto l1 = Vec2(physicsBox.getMinX(), physicsBox.getMidY()).getDistance(contactPoint);
//        auto l2 = Vec2(physicsBox.getMinX(), physicsBox.getMinY()).getDistance(contactPoint);
//
//        auto r1 = Vec2(physicsBox.getMaxX(), physicsBox.getMidY()).getDistance(contactPoint);
//        auto r2 = Vec2(physicsBox.getMaxX(), physicsBox.getMinY()).getDistance(contactPoint);
//
//        auto b = Vec2(physicsBox.getMidX(), physicsBox.getMinY()).getDistance(contactPoint);
//
//        float min = t1;
//        // min = MIN(t2, MIN(t3, min));
//        min = MIN(l1, MIN(l2, min));
//        min = MIN(r1, MIN(r2, min));
//        min = MIN(b, min);
//
//        // 상단부터 체크
//        if( min == t1 /*|| min == t2 || min == t3*/ ) return ContactDirection::TOP;
//        if( min == l1 || min == l2 )              return ContactDirection::LEFT;
//        if( min == r1 || min == r2 )              return ContactDirection::LEFT;
//        if( min == b )                            return ContactDirection::BOTTOM;
//        return ContactDirection::TOP;
//    }();
    
    for( auto listener : copyListeners ) {
        if( listener->onContactBlock ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == ball || contactTarget == block ) {
                    listener->onContactBlock(ball, block, contactPoint, category);
                }
            } else {
                listener->onContactBlock(ball, block, contactPoint, category);
            }
        }
    }
}

/**
 * 블록 간 충돌을 전달합니다
 * @param block1 블록1
 * @param block2 블록2
 * @param contactPoint 충돌 좌표
 * @param category1 블록1의 카테고리
 * @param category2 블록2의 카테고리
 */
void PhysicsManager::dispatchOnContactBlockAndBlock(GameTile *block1, GameTile *block2,
                                                    Vec2 contactPoint,
                                                    PhysicsCategory category1, PhysicsCategory category2) {

    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onContactBlockAndBlock ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == block1 || contactTarget == block2 ) {
                    listener->onContactBlockAndBlock(block1, block2, contactPoint, category1, category2);
                }
            } else {
                listener->onContactBlockAndBlock(block1, block2, contactPoint, category1, category2);
            }
        }
    }
}

void PhysicsManager::dispatchOnContactWall(Ball *ball, PhysicsCategory category) {
    
    auto copyListeners = listeners;
    
    for( auto listener : copyListeners ) {
        if( listener->onContactWall ) {
            auto contactTarget = listener->getContactTarget();
            
            if( contactTarget ) {
                if( contactTarget == ball ) {
                    listener->onContactWall(ball, category);
                }
            } else {
                listener->onContactWall(ball, category);
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


