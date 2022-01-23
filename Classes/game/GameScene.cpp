//
//  GameScene.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "GameScene.hpp"
#include "Define.h"
#include "User.hpp"
#include "SceneManager.h"
#include "PopupManager.hpp"
#include "BannerView.hpp"

#include "GameManager.hpp"
#include "GameView.hpp"

#include "CommonLoadingBar.hpp"
#include "SettingPopup.hpp"
#include "ui/ClearPopup.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace cocos2d::ui;
using namespace std;

GameScene* GameScene::create() {
    
    auto scene = new GameScene();
    
    if( scene && scene->init() ) {
        scene->autorelease();
        return scene;
    }
    
    delete scene;
    return nullptr;
}

GameScene::GameScene() :
gameView(nullptr) {
}

GameScene::~GameScene() {
}

bool GameScene::init() {
    
    if( !BaseScene::init() ) {
        return false;
    }
    
    SBAnalytics::setCurrentScreen(ANALYTICS_SCREEN_GAME);
    
    initBg();
    initGameView();
    initMenu();
    initGameListener();
    
    // 멀티 터치 방지
    addChild(SBNodeUtils::createSwallowMultiTouchNode(), INT_MAX);
    
    return true;
}

void GameScene::onEnter() {
    
    BaseScene::onEnter();
}

void GameScene::onEnterTransitionDidFinish() {
    
    BaseScene::onEnterTransitionDidFinish();
    
    SBAudioEngine::playBGM(SOUND_BGM_GAME);
    
    GameManager::onGameEnter();
    GameManager::onGameStart();
}

void GameScene::onExit() {
    
    BaseScene::onExit();
}

void GameScene::cleanup() {
    
    BaseScene::cleanup();
}

bool GameScene::onApplicationEnterBackground() {
    
    if( !BaseScene::onApplicationEnterBackground() ) {
        return false;
    }
    
    GameManager::onGamePause();
    
    return true;
}

bool GameScene::onApplicationEnterForeground() {
    
    if( !BaseScene::onApplicationEnterForeground() ) {
        return false;
    }
    
    // GameManager::onGameResume();
    
    return true;
}

bool GameScene::onBackKeyReleased() {
    
    if( !BaseScene::onBackKeyReleased() ) {
        return false;
    }
    
    // 설정 팝업 생성
    if( PopupManager::getInstance()->getPopupCount() == 0 ) {
        onClick(getChildByTag(Tag::BTN_SETTING));
        return true;
    }
    
    return false;
}

/**
 * 게임 일시정지
 */
void GameScene::onGamePause() {
    
    // 설정 팝업 생성
    auto popup = SettingPopup::create();
    popup->setOnHomeListener([=]() {
        this->replaceMainScene();
    });
    popup->setOnDismissListener([=](Node*) {
        GameManager::onGameResume();
    });
    SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
}

/**
 * 게임 재개
 */
void GameScene::onGameResume() {
}

/**
 * 게임 오버
 */
void GameScene::onGameOver() {
    
    MessageBox("게임 오버됨 -> 재시작", "");
    replaceGameScene(GAME_MANAGER->getStage().stage);
}

/**
 * 스테이지 변경
 */
void GameScene::onStageChanged(const StageData &stage) {
}

/**
 * 스테이지 클리어
 */
void GameScene::onStageClear(const StageData &stage) {
    
    // 클리어 팝업
    auto popup = ClearPopup::create();
    popup->setOnHomeListener([=]() {
        this->replaceMainScene();
    });
    popup->setOnRetryListener([=]() {
        this->replaceGameScene(stage.stage);
    });
    popup->setOnNextListener([=]() {
        this->replaceGameScene(stage.stage+1);
    });
    SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
}

/**
 * Scene 전환
 */
void GameScene::replaceMainScene() {
    
    GameManager::onGameExit();
    GameManager::destroyInstance();
    removeListeners(this);
    
    replaceScene(SceneType::MAIN);
}

void GameScene::replaceGameScene(int stage) {
    
    // 스테이지 유무 체크
    if( Database::getStage(stage).isNull() ) {
        MessageBox("스테이지 데이터가 없습니다", "");
        return;
    }
    
    GameManager::onGameExit();
    GameManager::destroyInstance();
    removeListeners(this);
    
    GAME_MANAGER->init();
    GAME_MANAGER->setStage(stage);
    
    replaceScene(SceneType::GAME);
}

/**
 * 버튼 클릭
 */
void GameScene::onClick(Node *sender) {
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    
    switch( sender->getTag() ) {
        case Tag::BTN_SETTING: {
            GameManager::onGamePause();
        } break;
    }
}

/**
 * 배경 초기화
 */
void GameScene::initBg() {
    
    addChild(LayerColor::create(Color4B(GAME_BG_COLOR)));
    
    // 배너
    /*
    if( !User::isRemovedAds() ) {
        auto bannerView = BannerView::create();
        bannerView->setTag(Tag::BANNER);
        addChild(bannerView, SBZOrder::TOP);
    }
     */
}

/**
 * 게임뷰 초기화
 */
void GameScene::initGameView() {
    
    gameView = GameView::create();
    addChild(gameView);
    
    // GAME_MANAGER->setView(gameView);
}

/**
 * 메뉴 초기화
 */
void GameScene::initMenu() {
    
    // 설정 버튼
    // common_btn_more.png Vec2TR(-56, -54) , Size(74, 74)
    auto settingBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_more.png");
    settingBtn->setTag(Tag::BTN_SETTING);
    settingBtn->setZoomScale(ButtonZoomScale::NORMAL);
    settingBtn->setAnchorPoint(ANCHOR_M);
    settingBtn->setPosition(Vec2TR(-56, -54));
    addChild(settingBtn);
    
    settingBtn->setOnClickListener(CC_CALLBACK_1(GameScene::onClick, this));
}

/**
 * 게임 이벤트 리스너 초기화
 */
void GameScene::initGameListener() {

    GameEventList events({
        GameEvent::PAUSE,
        GameEvent::RESUME,
        GameEvent::OVER,
        GameEvent::STAGE_CHANGED,
        GameEvent::STAGE_CLEAR,
    });
    
    GameManager::addEventListener(events, [=](GameEvent event, void *userData) {
        
        switch( event ) {
            case GameEvent::PAUSE:      this->onGamePause();     break;
            case GameEvent::RESUME:     this->onGameResume();    break;
            case GameEvent::OVER:       this->onGameOver();      break;
                
            case GameEvent::STAGE_CHANGED: {
                auto stage = (StageData*)userData;
                this->onStageChanged(*stage);
            } break;
                
            case GameEvent::STAGE_CLEAR: {
                auto stage = (StageData*)userData;
                this->onStageClear(*stage);
            } break;
                
            default: break;
        }
    }, this);
}
