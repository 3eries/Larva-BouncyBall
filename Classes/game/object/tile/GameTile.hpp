//
//  GameTile.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2021/09/28.
//

#ifndef GameTile_hpp
#define GameTile_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "../../GameDefine.h"
    
/** @class GameTile
 * @brief 타일은 맵의 기본 구성 단위입니다
 * 블럭, 아이템 등 맵에 표시되는 오브젝트는 Tile 클래스를 상속합니다
 */
class GameTile: public cocos2d::Node, public SBPhysicsObject {
public:
    GameTile(int rows, int columns);
    virtual ~GameTile();
    
protected:
    virtual bool init() override;
    virtual void onEnter() override;
    virtual void cleanup() override;
    virtual void initPhysics() override;
    virtual void initGameListener();
    
    virtual void onStageClear();
    
public:
    virtual void prepareRemove();
    
    virtual void enterWithAction();
    virtual void removeWithAction();
    
public:
//    virtual void setTilePosition(const TilePosition &tilePos, bool action = true,
//                                 SBCallback onActionFinished = nullptr);
    virtual void setTilePosition(const TilePosition &tilePos);
    virtual void moveWithAction(const TilePosition &tilePos, float duration,
                                SBCallback onActionFinished = nullptr);
    virtual bool isContainsPosition(const TilePosition &p);
    
protected:
    CC_SYNTHESIZE_READONLY(int, rows, Rows);        // 가로칸
    CC_SYNTHESIZE_READONLY(int, columns, Columns);  // 세로칸
    CC_SYNTHESIZE_READONLY(TilePosition, tilePos, TilePosition);
    
    // 타일 유효 여부
    SB_SYNTHESIZE_READONLY_BOOL(available, Available);
};

#endif /* GameTile_hpp */
