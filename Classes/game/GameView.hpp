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
#include "GameDefine.h"
#include "../content/data/model/StageData.h"

class GameTile;
class Ball;
class StageProgressBar;

class GameView: public cocos2d::Node, public SBPhysicsObject {
private:
    enum Tag {
        DEBUG_DRAW_VIEW = 1000,
    };
    
    enum ZOrder {
        TILE_FLAG = 1,
        TILE_ITEM,
        TILE_BLOCK,
        BALL,
        
        MENU = 10,
    };
    
    static const cocos2d::CameraFlag CAMERA_FLAG_BG  = cocos2d::CameraFlag::USER1;
    static const cocos2d::CameraFlag CAMERA_FLAG_MAP = cocos2d::CameraFlag::USER2;
    static const int                 CAMERA_DEPTH_BG  = -2;
    static const int                 CAMERA_DEPTH_MAP = -1;
    
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
    void initCamera();
    void initBg();
    void initTiles();
    void initBall();
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

    void updateCamera(float dt);
    
    void addTile(GameTile *tile);
    void removeTile(GameTile *tile);
    GameTile*              getTile(const TilePosition &p);
    std::vector<GameTile*> getTiles(const TileId &tileId);
    
#pragma mark- Touch Event
private:
    void onTouchBegan(cocos2d::Touch *touch);
    void onTouchEnded(cocos2d::Touch *touch);
    
private:
    bool isTouchEnabled;
    
#pragma mark- Contact
private:
    void onContactFlag(Ball *ball, GameTile *tile);
    void onContactItem(Ball *ball, GameTile *tile);
    void onContactBlock(Ball *ball, GameTile *tile, cocos2d::Vec2 contactPoint,
                        PhysicsCategory category);
    void onContactFloor(Ball *ball);
    
private:
    CC_SYNTHESIZE_READONLY(b2World*, world, World);
    Ball *ball;
    std::vector<GameTile*> tiles;
    StageProgressBar *stageProgressBar;
    
    struct MapCameraInfo {
        cocos2d::Camera *camera;
        cocos2d::Node *camControlNode;
        cocos2d::Node *camNode;
    };
    
    MapCameraInfo mapCameraInfo;
    
    // for debug
    cocos2d::Label *infoLabel;
    
    cocos2d::Vec2 maxVelocity;
    cocos2d::Vec2 minVelocity;
};

#endif /* GameView_hpp */
