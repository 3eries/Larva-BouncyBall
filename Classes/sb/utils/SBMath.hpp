//
//  SBMath.hpp
//
//  Created by seongmin hwang on 2018. 9. 18..
//

#ifndef SBMath_hpp
#define SBMath_hpp

#include <stdio.h>

#include "cocos2d.h"

class SBMath {
public:
    static int getDigit(int n);
    
    static float getDegree(const cocos2d::Vec2 &from, const cocos2d::Vec2 &to);
    static cocos2d::Vec2 getEndPosition(const cocos2d::Vec2 &startPos,
                                        float angle, float dist);
    
    static int random(int min, int max);
};

#endif /* SBMath_hpp */
