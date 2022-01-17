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

class GameTile;
class Ball;
class StageProgressBar;

class GameView: public cocos2d::Node, public SBPhysicsObject {
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
    
    void initPhysics() override;
    void initBg();
    void initGameListener();
    void initTouchListener();
    
#pragma mark- Game Event
public:
    void onGameReset();
    void onGameExit();
    void onGamePause();
    void onGameResume();
    
    void onStageChanged(const StageData &stage);
    void onStageRestart(const StageData &stage);
    void onStageClear(const StageData &stage);
    
    void onMoveNextStage();
    void onMoveNextStageFinished();

#pragma mark- Touch Event
private:
    void onTouchBegan(cocos2d::Touch *touch);
    void onTouchEnded(cocos2d::Touch *touch);
    
private:
    bool isTouchEnabled;
    
#pragma mark- Contact
private:
    bool onContactBlock(Ball *ball, GameTile *tile, cocos2d::Vec2 contactPoint);
    void onContactPortal(Ball *ball, GameTile *tile);
    
private:
    CC_SYNTHESIZE_READONLY(b2World*, world, World);
    Ball *ball;
    
    StageProgressBar *stageProgressBar;
    
    // for debug
    cocos2d::Label *infoLabel;
    
    cocos2d::Vec2 maxVelocity;
    cocos2d::Vec2 minVelocity;
};

#endif /* GameView_hpp */
