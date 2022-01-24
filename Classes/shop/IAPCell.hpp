//
//  IAPCell.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#ifndef IAPCell_hpp
#define IAPCell_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

class IAPCell: public cocos2d::ui::Widget {
public:
    static IAPCell* create();
    ~IAPCell();
    
private:
    IAPCell();
    
    bool init() override;
    
private:
    CC_SYNTHESIZE(SBCallback, onClickListener, OnClickListener);
};

#endif /* IAPCell_hpp */
