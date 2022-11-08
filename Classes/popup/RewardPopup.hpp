//
//  RewardPopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/25.
//

#ifndef RewardPopup_hpp
#define RewardPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "PopupManager.hpp"
#include "RewardManager.hpp"

class RewardPopup: public BasePopup {
public:
    static RewardPopup* show(const RewardItemDataList &characters);
    static RewardPopup* show(const RewardItemData &data) { return show(RewardItemDataList({data})); }
    
public:
    static RewardPopup* create(const RewardItemData &data);
    ~RewardPopup();
    
private:
    RewardPopup(const RewardItemData &data);
    
    bool init() override;
    void onEnter() override;
    bool onBackKeyReleased() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void runEnterAction(SBCallback onFinished = nullptr) override;
    void runExitAction(SBCallback onFinished = nullptr) override;
    
private:
    RewardItemData data;
    CC_SYNTHESIZE(SBCallback, onConfirmListener, OnConfirmListener);
};


#endif /* RewardPopup_hpp */
