//
//  MainStageProgressBar.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/10/13.
//

#ifndef MainStageProgressBar_hpp
#define MainStageProgressBar_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"
#include "RewardManager.hpp"

class RewardItem;

class MainStageProgressBar: public cocos2d::ui::Widget {
public:
    CREATE_FUNC(MainStageProgressBar);
    ~MainStageProgressBar();
    
private:
    MainStageProgressBar();
    
    bool init() override;
    void updateStarUI(int star);
    
public:
    void setWorld(int world);
    
private:
    CC_SYNTHESIZE(OnRewardItemDataListener,
                  onItemClickListener, OnItemClickListener);
    
    cocos2d::Label *starLabel;            // 현재 별 개수
    cocos2d::ProgressTimer *progressBar;  // 진행도
    cocos2d::Node *rewardLayer;           // 진행도에 표시되는 보상 레이어
    std::vector<RewardItem*> rewardItems;
};

#pragma mark- RewardItem

class RewardItem: public cocos2d::ui::Widget {
public:
    static RewardItem* create(const RewardItemData &data);
    ~RewardItem();
    
private:
    RewardItem(const RewardItemData &data);
    bool init() override;
    
public:
    void updateUI(int currentStar);
    
private:
    CC_SYNTHESIZE_READONLY(RewardItemData, data, Data);
    CC_SYNTHESIZE(OnRewardItemDataListener, onClickListener, OnClickListener);
    int userStar;
    
    superbomb::EffectSprite *icon;
    superbomb::EffectSprite *iconOutline;
};

#endif /* MainStageProgressBar_hpp */
