//
//  FunnyHelper.hpp
//
//  Created by seongmin hwang on 2018. 7. 6..
//

#ifndef FunnyHelper_hpp
#define FunnyHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

struct FamousSaying {
    std::string saying;
    std::string person;
    
    FamousSaying(std::string _saying, std::string _person) :
    saying(_saying), person(_person) {}
};

class FunnyHelper {
public:
    static FamousSaying getRandomFamousSaying();
};

#endif /* FunnyHelper_hpp */
