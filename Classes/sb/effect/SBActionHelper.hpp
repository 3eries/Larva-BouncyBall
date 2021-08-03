//
//  SBActionHelper.hpp
//
//  Created by seongmin hwang on 2018. 7. 16..
//

#ifndef SBActionHelper_hpp
#define SBActionHelper_hpp

#include <stdio.h>

#include "cocos2d.h"

class SBActionHelper {
public:
    static void runMoveAction(cocos2d::Node *n,
                              cocos2d::Vec2 from, cocos2d::Vec2 to,
                              float duration, bool isClear = true);
};

#endif /* SBActionHelper_hpp */
