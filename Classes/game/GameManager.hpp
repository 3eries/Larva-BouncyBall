//
//  GameManager.hpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "Define.h"
#include "GameConfiguration.hpp"
#include "StageManager.hpp"
#include "CharacterManager.hpp"
#include "GameDefine.h"
#include "physics/PhysicsManager.hpp"

#define GAME_MANAGER        GameManager::getInstance()
#define PHYSICS_MANAGER     GameManager::getPhysicsManager()

/** @class GameManager
 * @brief 이 클래스는 게임 공통 데이터를 관리하고 상태 전환을 담당합니다
 * 별도의 초기화는 필요 없습니다.
 * 싱글톤이지만 GameScene을 벗어날 경우 destroyInstance 호출로 인스턴스가 제거되도록 설계되었습니다
 */
class GameManager {
public:
    static GameManager* getInstance();
    static void destroyInstance();
    static bool isNullInstance();
    ~GameManager();
    
private:
    GameManager();
    
    void reset();
    
public:
    void init();
    
    // Game State
    void            setState(GameState state);
    void            addState(GameState state);
    void            removeState(GameState state);
    bool            hasState(GameState state);
    bool            isPlaying();
    bool            isPaused();
    
    // Data
    void            setStage(int stage);
    void            setStar(int star);
    
    // Getter
    static int      getPlayCount();
    bool            isContinuable();
    
private:
    CC_SYNTHESIZE_READONLY(GameState, state, State);               // 게임 상태
    
    CC_SYNTHESIZE_READONLY(StageData, stage, Stage);               // 스테이지
    CC_SYNTHESIZE_READONLY(float, mapScaleFactor, MapScaleFactor); //
    
    CC_SYNTHESIZE_READONLY(int, star, Star);                       // 획득한 별 개수
    CC_SYNTHESIZE_READONLY(int, continueCount, ContinueCount);     // 이어하기한 횟수
    
// Physics
public:
    static PhysicsManager* getPhysicsManager();
private:
    PhysicsManager *physicsManager;
    
// Game Event
public:
    typedef std::function<void(GameEvent/*event*/, void*/*userData*/)> GameEventListener;
    
    static void addEventListener(GameEventList events, GameEventListener gameEventListener,
                                 cocos2d::Node *target);
    static void dispatchCustomEvent(GameEvent event, void *optionalUserData = nullptr);
    static cocos2d::EventDispatcher* getEventDispatcher();
    
    static void onGameEnter();
    static void onGameExit();
    static void onGameReset();
    static void onGameStart();
    static void onGameRestart();
    static void onGamePause();
    static void onGameResume();
    static void onGameOver(GameOverType type,
                           const TilePosition &tilePos = INVALID_TILE_POSITION);
    static void onGameContinue();
    static void onGameResult();
    
    static void onStageChanged();
    static void onStageRestart();
    static void onStageClear();
    
    static void onMoveNextStage();
    static void onMoveNextStageFinished();
};

#endif /* GameManager_hpp */
