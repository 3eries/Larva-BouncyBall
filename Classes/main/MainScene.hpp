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
#include "RewardManager.hpp"

class MainScene : public BaseScene {
private:
    enum Tag {
        BG               = 10,
        WORLD_TITLE,
        WORLD_PAGE,
        
        BTN_SHOP         = 100,
        BTN_SETTING,
        
        BANNER           = 1000,
    };
    
public:
    static MainScene* create(int selectedWorld = 0);
    ~MainScene();
    
private:
    MainScene();
    
    bool init(int selectedWorld);
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExit() override;
    bool onBackKeyReleased() override;
    
    void initBg(int selectedWorld);
    void initMenu();
    void initWorlds(int selectedWorld);
    
private:
    void onPageChanged(size_t page);
    void onClickRewardItem(const RewardItemData &data);
    void onClick(cocos2d::Node *sender) override;
    
    void replaceGameScene(int stage);
    
    void showShopPopup();
    void showSettingPopup();
    
private:
    size_t pageIndex;
    int    pageDiffCount;
};

#endif /* MainScene_hpp */
