//
//  GameScene.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "GameScene.hpp"
#include "Define.h"
#include "ResourceHelper.hpp"
#include "User.hpp"
#include "SceneManager.h"
#include "PopupManager.hpp"
#include "BannerView.hpp"
#include "../main/MainScene.hpp"

#include "GameManager.hpp"
#include "GameView.hpp"

#include "CommonLoadingBar.hpp"
#include "GetCharacterPopup.hpp"
#include "ShopPopup.hpp"
#include "SalePopup.hpp"
#include "ui/PausePopup.hpp"
#include "ui/ClearPopup.hpp"
#include "ui/StageSkipPopup.hpp"

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
    // addChild(SBNodeUtils::createSwallowMultiTouchNode(), INT_MAX);
    
    return true;
}

void GameScene::onEnter() {
    
    BaseScene::onEnter();
}

void GameScene::onEnterTransitionDidFinish() {
    
    BaseScene::onEnterTransitionDidFinish();
    
    SBAudioEngine::playBGM(ResourceHelper::getWorldBackgroundMusic(GAME_MANAGER->getStage().world));
    
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
    
    if( GAME_MANAGER->isPlaying() ) {
        GameManager::onGamePause();
    }
    
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
    
    // 일시정지 팝업 생성
    if( PopupManager::getInstance()->getPopupCount() == 0 ) {
        onClick(getChildByTag(Tag::BTN_PAUSE));
        return true;
    }
    
    return false;
}

/**
 * 게임 일시정지
 */
void GameScene::onGamePause() {
    
    // 일시정지 팝업 생성
    auto popup = PausePopup::create();
    popup->setOnHomeListener([=]() {
        this->showInterstitial([=]() {
            this->replaceMainScene();
        });
    });
    popup->setOnRetryListener([=]() {
        this->replaceGameScene(GAME_MANAGER->getStage().stage);
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
    
    auto stage = GAME_MANAGER->getStage();
    
    // 스테이지 스킵 팝업
    if( User::getGameOverCountForSkipStage() >= 3 && AdsHelper::isRewardedVideoLoaded() ) {
        // 게임 오버 횟수 리셋
        User::setGameOverCountForSkipStage(0);
        
        auto popup = StageSkipPopup::create();
        popup->setOnSkipListener([=](bool isSkip) {
            popup->dismiss();
            
            // 스테이지 클리어
            if( isSkip ) {
                // 통계 이벤트
                {
                    SBAnalytics::EventParams params;
                    params[ANALYTICS_EVENT_PARAM_STAGE] = SBAnalytics::EventParam(TO_STRING(stage.stage));
                    params[ANALYTICS_EVENT_PARAM_STAGE_RANGE] = SBAnalytics::EventParam(SBAnalytics::getNumberRange(stage.stage, 1, 5, 5));
                    
                    SBAnalytics::logEvent(ANALYTICS_EVENT_STAGE_SKIP, params);
                }
                
                GAME_MANAGER->setStar(3);
                GameManager::onStageClear();
            }
            // 스테이지 재시작
            else {
                replaceGameScene(stage.stage);
            }
        });
        SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
    }
    // 스테이지 재시작
    else {
        replaceGameScene(stage.stage);
    }
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
    popup->setOnStarEffectFinishedListener([=]() {
        
        // 세일 팝업, 월드의 마지막 스테이지 클리어
        if( !User::isRemovedAds() && stage.stage % GAME_CONFIG->getStagePerWorld() == 0 ) {
            SBDirector::getInstance()->postDelayed(this, [=]() {
                auto popup = SalePopup::create();
                SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
            }, 0.5f, true);
        }
    });
    popup->setOnHomeListener([=]() {
        this->replaceMainScene();
    });
    popup->setOnRetryListener([=]() {
        this->showInterstitial([=]() {
            this->replaceGameScene(stage.stage);
        });
    });
    popup->setOnNextListener([=]() {
        auto onAdClosed = [=]() {
            // 마지막 스테이지는 메인 화면으로 이동
            if( stage.stage == StageManager::getLastStage().stage ) {
                // 커밍순 월드로 포커스
                this->replaceMainScene(stage.world+1);
            }
            // 다음 스테이지 진행
            else {
                this->replaceGameScene(stage.stage+1);
            }
        };
        
        if( stage.stage >= 10 ) {
            // 10 스테이지부터 전면 광고 노출
            if( SBPlatformHelper::isNetworkOnline() ) {
                this->showInterstitial(onAdClosed);
            }
            // 네크워크 오프라인 에러
            else {
                SBAnalytics::logEvent(ANALYTICS_EVENT_SB_NETWORK_ERROR_POPUP);
                SBPlatformHelper::showNetworkErrorPopup();
            }
        } else {
            onAdClosed();
        }
    });
    popup->setOnShopListener([=]() {
        auto shopPopup = ShopPopup::create();
        SceneManager::getScene()->addChild(shopPopup, ZOrder::POPUP_MIDDLE);
    });
    SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
}

/**
 * Scene 전환
 */
void GameScene::replaceMainScene(int selectedWorld) {
    
    GameManager::onGameExit();
    GameManager::destroyInstance();
    removeListeners(this);
    
    SceneManager::getInstance()->replace(SceneType::MAIN, [=]() -> MainScene* {
        return MainScene::create(selectedWorld);
    });
}

void GameScene::replaceGameScene(int stage) {
    
    // 스테이지 유무 체크
    if( StageManager::getStage(stage).isNull() ) {
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
 * 전면 광고 노출
 */
void GameScene::showInterstitial(SBCallback onAdClosed) {
    
    if( !User::isRemovedAds() && AdsHelper::isInterstitialLoaded() ) {
        SBDirector::getInstance()->setScreenTouchLocked(true);
        
        auto listener = AdListener::create(AdType::INTERSTITIAL);
        listener->setTarget(this);
        listener->onAdClosed = onAdClosed;
        AdsHelper::showInterstitial(listener);
        
    } else {
        onAdClosed();
    }
}

/**
 * 버튼 클릭
 */
void GameScene::onClick(Node *sender) {
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    
    switch( sender->getTag() ) {
        case Tag::BTN_PAUSE: {
            GameManager::onGamePause();
        } break;
    }
}

/**
 * 배경 초기화
 */
void GameScene::initBg() {
    
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
    auto pauseBtn = SBButton::create(DIR_IMG_GAME + "game_btn_pause.png");
    pauseBtn->setTag(Tag::BTN_PAUSE);
    pauseBtn->setZoomScale(ButtonZoomScale::NORMAL);
    pauseBtn->setAnchorPoint(ANCHOR_M);
    pauseBtn->setPosition(Vec2TR(-78, -80));
    addChild(pauseBtn);
    
    pauseBtn->setOnClickListener(CC_CALLBACK_1(GameScene::onClick, this));
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
