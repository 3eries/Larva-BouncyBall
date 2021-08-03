//
//  TestHelper.hpp
//
//  Created by seongmin hwang on 2018. 6. 1..
//

#ifndef TestHelper_hpp
#define TestHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#define ENABLE_TEST_MENU        0
#define TEST_HELPER             TestHelper::getInstance()

class TestHelper : public cocos2d::Ref {
public:
    static TestHelper* getInstance();
    static void destroyInstance();
    
    ~TestHelper();
    
private:
    TestHelper();
    
private:
    SB_SYNTHESIZE_BOOL(cheatMode, CheatMode);               // 치트 모드 on/off
};

#endif /* TestHelper_hpp */
