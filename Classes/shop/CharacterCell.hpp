//
//  CharacterCell.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#ifndef CharacterCell_hpp
#define CharacterCell_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

class CharacterCell: public cocos2d::ui::Widget {
public:
    static CharacterCell* create();
    ~CharacterCell();
    
private:
    CharacterCell();
    
    bool init() override;
    
private:
    CC_SYNTHESIZE(std::function<void()>, onClickListener, OnClickListener);
};

#endif /* CharacterCell_hpp */
