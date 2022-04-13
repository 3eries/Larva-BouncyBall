//
//  StageSkipPopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/04/13.
//

#ifndef StageSkipPopup_hpp
#define StageSkipPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BasePopup.hpp"

class StageSkipPopup : public BasePopup {
public:
    CREATE_FUNC(StageSkipPopup);
    ~StageSkipPopup();
    
private:
    StageSkipPopup();
    
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
    CC_SYNTHESIZE(std::function<void(bool)>, onSkipListener, OnSkipListener);
};

#endif /* StageSkipPopup_hpp */
