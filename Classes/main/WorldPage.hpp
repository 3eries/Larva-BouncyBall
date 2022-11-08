//
//  WorldPage.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/10.
//

#ifndef WorldPage_hpp
#define WorldPage_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "RewardManager.hpp"

class StageCell;

#pragma mark- WorldPage

class WorldPage: public cocos2d::ui::Widget {
public:
    static WorldPage* create(int world);
    ~WorldPage();
    
private:
    WorldPage(int world);
    
    bool init() override;
    void initLockedUI();
    void initStageUI();
    
public:
    void unlock();
    
private:
    enum Tag {
        LOCKED_LAYER = 100,
        LOCKED_DESCRIPTION,
        LOCKED_NEED_KEY,
        LOCKED,             // 자물쇠
        
        PROGRESS_BAR,
    };
    
    CC_SYNTHESIZE_READONLY(int, world, World);
    CC_SYNTHESIZE(std::function<void(StageCell*)>,
                  onClickListener, OnClickListener);
    CC_SYNTHESIZE(OnRewardItemDataListener,
                  onClickRewardItemListener, OnClickRewardItemListener);
};

#pragma mark- StageCell

class StageCell: public cocos2d::ui::Widget {
public:
    static StageCell* create(int stage);
    ~StageCell();
    
private:
    StageCell(int stage);
    
    bool init() override;
    
private:
    CC_SYNTHESIZE_READONLY(int, stage, Stage);
    CC_SYNTHESIZE(std::function<void(StageCell*)>,
                  onClickListener, OnClickListener);
};

#endif /* WorldPage_hpp */
