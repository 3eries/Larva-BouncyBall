//
//  GameManager.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "GameManager.hpp"

#include "UserDefaultKey.h"
#include "User.hpp"
#include "TestHelper.hpp"
#include "RewardManager.hpp"
#include "CharacterManager.hpp"

#include "GameView.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

static GameManager *instance = nullptr;
GameManager* GameManager::getInstance() {
    
    if( !instance ) {
        instance = new GameManager();
    }
    
    return instance;
}

void GameManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

bool GameManager::isNullInstance() {
    return instance == nullptr;
}

GameManager::GameManager():
physicsManager(new PhysicsManager()) {
}

GameManager::~GameManager() {
    
    CC_SAFE_DELETE(physicsManager);
}


void GameManager::init() {
    
    reset();
}

void GameManager::reset() {
    
    CCLOG("GameManager::reset");
    
    state = GameState::NONE;
    mapScaleFactor = 1;
    star = 0;
    continueCount = 0;
    
    // setStage(1);
}

/**
 * 게임 상태를 변경합니다
 */
void GameManager::setState(GameState state) {
    this->state = state;
}

void GameManager::addState(GameState state) {
    this->state = (GameState)(this->state | state);
}

void GameManager::removeState(GameState state) {
    this->state = (GameState)(this->state & ~state);
}

bool GameManager::hasState(GameState state) {
    return (this->state & state) == state;
}

/**
 * 게임 플레이중인지 반환합니다
 */
bool GameManager::isPlaying() {
    
    return hasState(GameState::STARTED) &&
    !hasState(GameState::PAUSED) &&
    !hasState(GameState::GAME_OVER) &&
    !hasState(GameState::RESULT);
}

/**
 * 게임이 일시정지 되었는지 반환합니다
 */
bool GameManager::isPaused() {
    
    return hasState(GameState::PAUSED);
}

/**
 * 스테이지를 설정합니다
 */
void GameManager::setStage(int stage) {
    
    this->stage = StageManager::getStage(stage);
    StageManager::setLatestPlayStage(stage);
    
    // Scale factor 설정
    mapScaleFactor = this->stage.tileSize.height / TILE_DEFAULT_SIZE.height;
}

/**
 * 획득한 별 개수를 설정합니다
 */
void GameManager::setStar(int star) {
    
    this->star = star;
}

int GameManager::getPlayCount() {
    return UserDefault::getInstance()->getIntegerForKey(UserDefaultKey::PLAY_COUNT, 0);
}

bool GameManager::isContinuable() {
    return instance->getContinueCount() == 0;
}

PhysicsManager* GameManager::getPhysicsManager() {
    return instance->physicsManager;
}

#pragma mark- Game Event

void GameManager::addEventListener(GameEventList events, GameEventListener gameEventListener,
                                   Node *target) {
    
    for( auto eventKey : events ) {
        auto listener = EventListenerCustom::create(GAME_EVENT_STRINGS[eventKey], [=](EventCustom *event) {
            gameEventListener(eventKey, event->getUserData());
        });
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, target);
    }
}

void GameManager::dispatchCustomEvent(GameEvent event, void *optionalUserData) {
 
    getEventDispatcher()->dispatchCustomEvent(GAME_EVENT_STRINGS[event], optionalUserData);
}

EventDispatcher* GameManager::getEventDispatcher() {
    return Director::getInstance()->getEventDispatcher();
}

/**
 * 게임 진입
 */
void GameManager::onGameEnter() {
    
    // log
    {
        string log;
        log += "\t" + STR_FORMAT("play count: %d", getPlayCount()) + "\n";
        
        Log::i("GameManager::onGameEnter\n{\n%s}", log.c_str());
    }
    
    instance->setState(GameState::ENTERED);
    dispatchCustomEvent(GameEvent::ENTER);
}

/**
 * 게임 퇴장
 */
void GameManager::onGameExit() {
    
    Log::i("GameManager::onGameExit");
    
    // onGamePause();
    
    getPhysicsManager()->stopScheduler();
    getPhysicsManager()->removeBodies();
    
    instance->reset();
    instance->setState(GameState::EXITED);
    dispatchCustomEvent(GameEvent::EXIT);
}

/**
 * 게임 리셋
 * onGameStart(게임 시작), onGameContinue(이어하기)에서 호출됨
 */
void GameManager::onGameReset() {
    
    Log::i("GameManager::onGameReset");
    
    getPhysicsManager()->startScheduler();
    
    dispatchCustomEvent(GameEvent::RESET);
    onStageChanged();
}

/**
 * 게임 시작
 */
void GameManager::onGameStart() {
    
    Log::i("GameManager::onGameStart start");
    
    auto stage = instance->stage;
    
    // 통계 이벤트
    {
        SBAnalytics::EventParams params;
        params[ANALYTICS_EVENT_PARAM_STAGE] = SBAnalytics::EventParam(TO_STRING(stage.stage));
        params[ANALYTICS_EVENT_PARAM_STAGE_RANGE] = SBAnalytics::EventParam(SBAnalytics::getNumberRange(stage.stage, 1, 5, 5));

        if( StageManager::isStageCleared(stage.stage) ) {
            SBAnalytics::logEvent(ANALYTICS_EVENT_STAGE_PLAY_RE, params);
        } else {
            SBAnalytics::logEvent(ANALYTICS_EVENT_STAGE_PLAY, params);
        }
    }
    
    onGameReset();
    
    UserDefault::getInstance()->setIntegerForKey(UserDefaultKey::PLAY_COUNT, getPlayCount()+1);
    UserDefault::getInstance()->flush();
    
    instance->setState(GameState::STARTED);
    dispatchCustomEvent(GameEvent::START);
    
    Log::i("GameManager::onGameStart end");
}

/**
 * 게임 재시작
 */
void GameManager::onGameRestart() {
    
    Log::i("GameManager::onGameRestart start");
    
    instance->reset();
    dispatchCustomEvent(GameEvent::RESTART);
    
    onGameStart();
    
    Log::i("GameManager::onGameRestart end");
}

/**
 * 게임 일시정지
 */
void GameManager::onGamePause() {
    
    Log::i("GameManager::onGamePause state is paused: %d", instance->hasState(GameState::PAUSED));
    
    if( instance->hasState(GameState::PAUSED) ) {
        return;
    }
    
    getPhysicsManager()->pauseScheduler();
    
    instance->addState(GameState::PAUSED);
    dispatchCustomEvent(GameEvent::PAUSE);
}

/**
 * 게임 재개
 */
void GameManager::onGameResume() {
    
    Log::i("GameManager::onGameResume state is paused: %d", instance->hasState(GameState::PAUSED));
    
    if( !instance->hasState(GameState::PAUSED) ) {
        return;
    }
    
    getPhysicsManager()->resumeScheduler();
    
    instance->removeState(GameState::PAUSED);
    dispatchCustomEvent(GameEvent::RESUME);
}

/**
 * 게임 오버
 */
void GameManager::onGameOver(GameOverType type, const TilePosition &tilePos) {
    
    Log::i("GameManager::onGameOver type: %d", (int)type);
    
    CCASSERT(instance->hasState(GameState::STARTED), "GameManager::onGameOver invalid called.");
    
    auto stage = instance->stage;
    
    // 통계 이벤트
    {
        auto gameOverType = [type]() -> string {
            switch( type ) {
                case GameOverType::FALL:        return "fall";
                case GameOverType::DEATH_BLOCK: return "death_block";
                case GameOverType::CANNON_BALL: return "cannon_ball";
                default:                        return "none";
            }
        }();
        
        SBAnalytics::EventParams params;
        params[ANALYTICS_EVENT_PARAM_STAGE] = SBAnalytics::EventParam(TO_STRING(stage.stage));
        params[ANALYTICS_EVENT_PARAM_STAGE_RANGE] = SBAnalytics::EventParam(SBAnalytics::getNumberRange(stage.stage, 1, 5, 5));
        params[ANALYTICS_EVENT_PARAM_TYPE] = SBAnalytics::EventParam(gameOverType);
        params[ANALYTICS_EVENT_PARAM_SUMMARY] = SBAnalytics::EventParam(STR_FORMAT("%d-%s", stage.stage, gameOverType.c_str()));

        if( tilePos == INVALID_TILE_POSITION ) {
            params[ANALYTICS_EVENT_PARAM_TILE_POSITION] = SBAnalytics::EventParam(STR_FORMAT("%d-%s", stage.stage, gameOverType.c_str()));
        } else {
            auto str = STR_FORMAT("%d-[%d,%d]", stage.stage,
                                  (int)tilePos.x, stage.mapHeightTiles - (int)tilePos.y - 1);
            params[ANALYTICS_EVENT_PARAM_TILE_POSITION] = SBAnalytics::EventParam(str);
        }
        
        SBAnalytics::logEvent(ANALYTICS_EVENT_GAME_OVER, params);
    }
    
    // 게임 오버 횟수 업데이트
    User::setGameOverCountForSkipStage(User::getGameOverCountForSkipStage()+1);
    
    getPhysicsManager()->stopScheduler();
    
    instance->addState(GameState::GAME_OVER);
    dispatchCustomEvent(GameEvent::OVER);
}

/**
 * 게임 이어하기
 */
void GameManager::onGameContinue() {
    
    Log::i("GameManager::onGameContinue start");
    CCASSERT(instance->hasState(GameState::GAME_OVER), "GameManager::onGameContinue invalid called.");
    
    onGameReset();
    
    instance->continueCount++;
    
    instance->removeState(GameState::GAME_OVER);
    instance->addState(GameState::CONTINUE);
    
    dispatchCustomEvent(GameEvent::CONTINUE);
    
    Log::i("GameManager::onGameContinue end");
}

/**
 * 게임 결과
 */
void GameManager::onGameResult() {
    
    CCLOG("GameManager::onGameResult");
    
    CCASSERT(instance->hasState(GameState::GAME_OVER), "GameManager::onGameResult invalid called.");
    
    instance->removeState(GameState::GAME_OVER);
    instance->addState(GameState::RESULT);
    dispatchCustomEvent(GameEvent::RESULT);
}

/**
 * 스테이지 변경
 */
void GameManager::onStageChanged() {
    
    auto stage = instance->stage;
    Log::i("GameManager::onStageChanged stage: %d", stage.stage);
    
    dispatchCustomEvent(GameEvent::STAGE_CHANGED, &stage);
}

/**
 * 스테이지 재시작
 */
void GameManager::onStageRestart() {
    
    auto stage = instance->stage;
    Log::i("GameManager::onStageRestart stage: %d", stage.stage);
    
    dispatchCustomEvent(GameEvent::STAGE_RESTART, &stage);
}

/**
 * 스테이지 클리어
 */
void GameManager::onStageClear() {
    
    auto stage = instance->stage;
    Log::i("GameManager::onStageClear stage: %d", stage.stage);
    
    superbomb::PluginPlay::submitScore(LEADER_BOARD_TOP_LEVEL, stage.stage);
    
    // 통계 이벤트
    {
        SBAnalytics::EventParams params;
        params[ANALYTICS_EVENT_PARAM_STAGE] = SBAnalytics::EventParam(TO_STRING(stage.stage));
        params[ANALYTICS_EVENT_PARAM_STAGE_RANGE] = SBAnalytics::EventParam(SBAnalytics::getNumberRange(stage.stage, 1, 5, 5));
        params[ANALYTICS_EVENT_PARAM_STAR] = SBAnalytics::EventParam(TO_STRING(instance->star));
        params[ANALYTICS_EVENT_PARAM_SUMMARY] = SBAnalytics::EventParam(STR_FORMAT("%d-%d", stage.stage, instance->star));
        
        if( StageManager::isStageCleared(stage.stage) ) {
            SBAnalytics::logEvent(ANALYTICS_EVENT_STAGE_CLEAR_RE, params);
        } else {
            SBAnalytics::logEvent(ANALYTICS_EVENT_STAGE_CLEAR, params);
        }
    }
    
    // 스테이지 별 개수 저장, 이전 별 개수보다 커야 함
    int star = MAX(instance->star, StageManager::getStageStarCount(stage.stage));
    StageManager::setStageStarCount(stage.stage, star);
    
    // 다음 스테이지 해제
    if( stage.stage < StageManager::getWorldLastStage(stage.world) ) {
        StageManager::unlockStage(stage.stage+1);
    }
    
    // 보상 체크
    RewardManager::checkUnlock();
    
    // 게임 오버 횟수 리셋
    User::setGameOverCountForSkipStage(0);
    
    instance->addState(GameState::RESULT);
    dispatchCustomEvent(GameEvent::STAGE_CLEAR, &stage);
}

/**
 * 다음 스테이지로 이동
 */
void GameManager::onMoveNextStage() {
    
    // 마지막 레벨
    if( instance->stage.stage == StageManager::getLastStage().stage ) {
    }
    
    instance->setStage(instance->stage.stage+1);
    dispatchCustomEvent(GameEvent::MOVE_NEXT_STAGE, &instance->stage);
}

/**
 * 다음 스테이지로 이동 완료
 */
void GameManager::onMoveNextStageFinished() {
    
    dispatchCustomEvent(GameEvent::MOVE_NEXT_STAGE_FINISHED, &instance->stage);
    onStageChanged();
}
