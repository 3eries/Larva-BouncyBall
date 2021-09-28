//
//  GameView.hpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#ifndef GameView_hpp
#define GameView_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "../content/data/model/StageData.h"

class StageProgressBar;

class GameView: public cocos2d::Node {
private:
    enum Tag {
        DEBUG_DRAW_VIEW = 1000,
    };
    
public:
    CREATE_FUNC(GameView);
    ~GameView();
    
private:
    GameView();
    
    bool init() override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void cleanup() override;
    
    void initPhysics();
    void initBg();
    void initGameListener();
    
private:
    CC_SYNTHESIZE_READONLY(b2World*, world, World);
    
    StageProgressBar *stageProgressBar;
    
#pragma mark- Game Event
public:
    void onGameReset();
    void onGamePause();
    void onGameResume();
    
    void onStageChanged(const StageData &stage);
    void onStageRestart(const StageData &stage);
    void onStageClear(const StageData &stage);
    
    void onMoveNextStage();
    void onMoveNextStageFinished();
};

#endif /* GameView_hpp */
