//
//  SettingPopup.hpp
//
//  Created by seongmin hwang on 2018. 7. 6..
//

#ifndef SettingPopup_hpp
#define SettingPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BasePopup.hpp"

class SettingPopup : public BasePopup {
public:
    CREATE_FUNC(SettingPopup);
    ~SettingPopup();
    
private:
    SettingPopup();
    
    bool init() override;
    void onEnter() override;
    bool onBackKeyReleased() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void runEnterAction(SBCallback onFinished = nullptr) override;
    void runExitAction(SBCallback onFinished = nullptr) override;
    
    void onEnterActionFinished() override;
    
private:
    cocos2d::Sprite *popupBg;
    CC_SYNTHESIZE(SBCallback, onHomeListener, OnHomeListener);
};

#endif /* SettingPopup_hpp */
