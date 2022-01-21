//
//  Item.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#ifndef Item_hpp
#define Item_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "GameTile.hpp"
#include "../../GameDefine.h"
    
class Ball;

/** @class Item
 * @brief 기본 블럭, 특수 블럭 등 맵에 표시되는 블럭은 Item 클래스를 상속합니다
 */
class Item: public GameTile {
public:
    static Item* create(const TileData &data);
    
    Item(const TileData &data);
    virtual ~Item();
    
protected:
    virtual bool init() override;
    virtual void initPhysics() override;
    
public:
    virtual void onContactItem(Ball *ball, GameTile *Item);
};

#endif /* Item_hpp */
