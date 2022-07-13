//
//  BlockCannonBall.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/03/11.
//

#ifndef BlockCannonBall_hpp
#define BlockCannonBall_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "Block.hpp"

class BlockCannon;

/** @class BlockCannonBall
 * @brief 발사대 블록 - 발사체
 * 맵에 정의된 START, END 블럭의 데이터로 무브 블럭을 생성합니다
 */
class BlockCannonBall: public Block {
public:
    static BlockCannonBall* create(BlockCannon *cannon);
    
    BlockCannonBall(const TileData &data, BlockCannon *cannon);
    virtual ~BlockCannonBall();
    
protected:
    virtual bool init() override;
    virtual void initImage() override;
    virtual void initPhysics() override;
    
    virtual b2BodyDef createBodyDef() override;
    virtual b2Filter  createPhysicsFilter() override;
    
    virtual void update(float dt) override;
    virtual void move();
    
    virtual void onMoveStart();
    virtual void onMoveEnd(bool nextMove);
    
    virtual void onPreSolve(b2Contact *contact, const b2Manifold *oldManifold);
    virtual void onContactBlockAndBlock(GameTile *block1, GameTile *block2,
                                        cocos2d::Vec2 contactPoint,
                                        PhysicsCategory category1, PhysicsCategory category2) override;
    
    cocos2d::Vec2 getCannonBallStartPos();
    cocos2d::Vec2 getCannonBallEndPos();
    cocos2d::Vec2 getCannonBallDirection();
    static std::string getCannonBallImage(const TileData &cannonData);
    
protected:
    BlockCannon *cannon;
};

#endif /* BlockCannonBall_hpp */
