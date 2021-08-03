//
//  LogoView.hpp
//
//  Created by seongmin hwang on 2018. 6. 14..
//

#ifndef LogoView_hpp
#define LogoView_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

class LogoView : public cocos2d::Node {
public:
    CREATE_FUNC(LogoView);
    ~LogoView();
    
private:
    LogoView();
    
    bool init() override;
    void logoFinished();
    
public:
    void run();
    
private:
    CC_SYNTHESIZE(SBCallback, onFinishedListener, OnFinishedListener);
    cocos2d::Node *anim;
};

#endif /* LogoView_hpp */
