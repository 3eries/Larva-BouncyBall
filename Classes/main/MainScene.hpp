//
//  MainScene.hpp
//
//  Created by seongmin hwang on 2018. 5. 14..
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BaseScene.hpp"

class MainScene : public BaseScene {
private:
    enum Tag {
        BTN_CREDIT           = 100,
        BTN_START,
        BTN_LEADER_BOARD,
        BTN_SETTING,
        BTN_TEST,
    };
    
public:
    CREATE_FUNC(MainScene);
    ~MainScene();
    
private:
    MainScene();
    
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
    
    void replaceGameScene();
    void showSettingPopup();
    
private:
    std::vector<cocos2d::Color3B> titleColors;
    int titleColorIndex;
};

#endif /* MainScene_hpp */
