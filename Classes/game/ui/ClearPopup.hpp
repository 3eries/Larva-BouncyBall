//
//  ClearPopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#ifndef ClearPopup_hpp
#define ClearPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "PopupManager.hpp"
#include "../GameManager.hpp"

class ClearPopup: public BasePopup {
public:
    CREATE_FUNC(ClearPopup);
    ~ClearPopup();
    
private:
    ClearPopup();
    
    bool init() override;
    void onEnter() override;
    bool onBackKeyReleased() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void runStarAnimation(int i);
    
    void runEnterAction(SBCallback onFinished = nullptr) override;
    void onEnterActionFinished() override;
    
private:
    std::vector<cocos2d::Sprite*> starBgs;
    
    CC_SYNTHESIZE(SBCallback, onStarEffectFinishedListener, OnStarEffectFinishedListener);
    CC_SYNTHESIZE(SBCallback, onShopListener, OnShopListener);
    CC_SYNTHESIZE(SBCallback, onHomeListener, OnHomeListener);
    CC_SYNTHESIZE(SBCallback, onRetryListener, OnRetryListener);
    CC_SYNTHESIZE(SBCallback, onNextListener, OnNextListener);
    CC_SYNTHESIZE(SBCallback, onStarRewardedListener, OnStarRewardedListener);
};

#endif /* ClearPopup_hpp */
