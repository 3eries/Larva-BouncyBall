//
//  TutorialHelper.hpp
//
//  Created by seongmin hwang on 20/07/2019.
//

#ifndef TutorialHelper_hpp
#define TutorialHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

class TutorialHelper : public cocos2d::Ref {
public:
    static TutorialHelper* getInstance();
    static void destroyInstance();
    static bool isInitialized();
    
    ~TutorialHelper();
    
private:
    TutorialHelper();

public:
    void init(int step, int lastStep);
    
    bool isLastStep();
    
private:
    bool initialized;
    CC_SYNTHESIZE(int, step, Step);
    CC_SYNTHESIZE_READONLY(int, lastStep, LastStep);
};

#endif /* TutorialHelper_hpp */
