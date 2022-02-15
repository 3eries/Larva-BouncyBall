//
//  PausePopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/15.
//

#ifndef PausePopup_hpp
#define PausePopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BasePopup.hpp"

class PausePopup : public BasePopup {
public:
    CREATE_FUNC(PausePopup);
    ~PausePopup();
    
private:
    PausePopup();
    
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
    CC_SYNTHESIZE(SBCallback, onRetryListener, OnRetryListener);
};


#endif /* PausePopup_hpp */
