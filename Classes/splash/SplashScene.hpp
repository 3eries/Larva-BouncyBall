//
//  SplashScene.hpp
//
//  Created by seongmin hwang on 2018. 2. 22..
//

#ifndef SplashScene_hpp
#define SplashScene_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "BaseScene.hpp"

class SplashScene : public BaseScene {
public:
    CREATE_FUNC(SplashScene);
    ~SplashScene();
    
private:
    SplashScene();
    
    bool init() override;
    void onEnter() override;
    void onExit() override;
    
private:
    void replaceMainScene();
    
    void initLaunchImage();
    void launchImageFinished();
    
    void login();
    void loginCompleted();
    
private:
    bool isReplacedMainScene;
    bool isLaunchImageFinished;
    bool isLoginCompleted;
};

#endif /* SplashScene_hpp */
