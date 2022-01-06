//
//  WelcomeScene.hpp
//
//  Created by seongmin hwang on 2018. 5. 14..
//

#ifndef WelcomeScene_hpp
#define WelcomeScene_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BaseScene.hpp"

class WelcomeScene : public BaseScene {
private:
    enum Tag {
        BTN_CREDIT           = 100,
        BTN_START,
        BTN_LEADER_BOARD,
        BTN_SETTING,
        BTN_TEST,
    };
    
public:
    CREATE_FUNC(WelcomeScene);
    ~WelcomeScene();
    
private:
    WelcomeScene();
    
    bool init() override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExit() override;
    bool onBackKeyReleased() override;
    
    void initBg();
    void initTitle();
    void initMenu();
    
private:
    void onClick(cocos2d::Node *sender) override;
    
    void replaceMainScene();
    void showSettingPopup();
};

#endif /* WelcomeScene_hpp */
