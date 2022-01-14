//
//  MainScene.cpp
//
//  Created by seongmin hwang on 2018. 5. 14..
//

#include "MainScene.hpp"

#include "Define.h"
#include "User.hpp"
#include "SceneManager.h"
#include "PopupManager.hpp"
#include "GameUIHelper.hpp"

#include "../test/TestHelper.hpp"
#include "../game/GameManager.hpp"
#include "../game/GameDefine.h"

#include "WorldPage.hpp"
#include "CommonLoadingBar.hpp"
#include "ExitAlertPopup.hpp"
#include "SettingPopup.hpp"
#include "BannerView.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace cocos2d::ui;
using namespace spine;
using namespace std;

MainScene::MainScene() {
    
}

MainScene::~MainScene() {
    
}

bool MainScene::init() {
    
    if( !BaseScene::init() ) {
        return false;
    }
    
    SBAnalytics::setCurrentScreen(ANALYTICS_SCREEN_MAIN);
    
    initBg();
    initMenu();
    initWorlds();
    
    return true;
}

void MainScene::onEnter() {
    
    BaseScene::onEnter();
    
    // bgm
    scheduleOnce([=](float) {
        SBAudioEngine::playBGM(SOUND_BGM_MAIN);
    }, SceneManager::REPLACE_DURATION_MAIN, "MAIN_SCENE_BGM_DELAY");
}

void MainScene::onEnterTransitionDidFinish() {
    
    BaseScene::onEnterTransitionDidFinish();
    
    // 자동 씬 전환
    scheduleOnce([=](float dt) {
        // replaceGameScene(1);
    }, 0.1f, "AUTO_REPLACE");
}

void MainScene::onExit() {
    
    BaseScene::onExit();
}

bool MainScene::onBackKeyReleased() {
    
    if( !BaseScene::onBackKeyReleased() ) {
        return false;
    }
    
    // 앱 종료 알림 팝업 생성
    if( PopupManager::getInstance()->getPopupCount() == 0 ) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        
        auto popup = (ExitAlertPopup*)PopupManager::show(PopupType::EXIT_APP);
        popup->setOnExitAppListener([=]() {
            SBSystemUtils::exitApp();
        });
        
        return true;
    }
    
    return false;
}

/**
 * 버튼 클릭
 */
void MainScene::onClick(Node *sender) {
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    
    switch( sender->getTag() ) {
        // 상점
        case Tag::BTN_SHOP: {
        } break;
            
        // 설정
        case Tag::BTN_SETTING: {
            showSettingPopup();
        } break;
            
        default:
            break;
    }
}

/**
 * 게임씬으로 전환
 */
void MainScene::replaceGameScene(int stage) {
    
    SB_SAFE_HIDE(getChildByTag(Tag::BTN_SHOP));
    SB_SAFE_HIDE(getChildByTag(Tag::BTN_SETTING));
    
    auto onAdClosed = [=]() {
        GAME_MANAGER->init();
        GAME_MANAGER->setStage(stage);
        
        replaceScene(SceneType::GAME);
    };
    
    // 최초 실행이 아닐때 전면 광고 노출
    if( !SBDirector::isFirstRun() &&
        !User::isRemovedAds() && AdsHelper::isInterstitialLoaded() ) {
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
 * 설정 팝업 노출
 */
void MainScene::showSettingPopup() {
    
    auto popup = SettingPopup::create();
    SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
}

void MainScene::initBg() {
    
    addChild(LayerColor::create(Color4B(GAME_BG_COLOR)));
}

/**
 * 메뉴 초기화
 */
void MainScene::initMenu() {
    
    // 메인 화면 전용 메뉴
//    auto settingBtn = Button::create(DIR_IMG_COMMON + "common_btn_more.png");
//    settingBtn->setTag(Tag::BTN_SETTING);
//    settingBtn->setZoomScale(ButtonZoomScale::NORMAL);
//    settingBtn->setAnchorPoint(ANCHOR_M);
//    settingBtn->setPosition(Vec2TR(-56, -54));
//    addChild(settingBtn);
//
//    settingBtn->setOnClickListener(CC_CALLBACK_1(GameScene::onClick, this));
    
    SBUIInfo infos[] = {
        SBUIInfo(Tag::BTN_SHOP, ANCHOR_M, Vec2TL(56, -54),
                 DIR_IMG_MAIN + "main_btn_leaderboard.png"),
        SBUIInfo(Tag::BTN_SETTING, ANCHOR_M, Vec2TR(-56, -54),
                 DIR_IMG_COMMON + "common_btn_more.png"),
    };
    
    for( int i = 0; i < sizeof(infos)/sizeof(SBUIInfo); ++i ) {
        auto info = infos[i];
        
        auto btn = SBButton::create(info.file);
        btn->setZoomScale(ButtonZoomScale::NORMAL);
        info.apply(btn);
        addChild(btn, ZOrder::COMMON_MENU_BOTTOM);
        
        btn->setOnClickListener(CC_CALLBACK_1(MainScene::onClick, this));
    }
}

/**
 * 월드 페이지 초기화
 */
void MainScene::initWorlds() {
    
    auto pageView = PageView::create();
    pageView->setDirection(PageView::Direction::HORIZONTAL);
    pageView->setIndicatorEnabled(true);
    pageView->setBounceEnabled(true);
    pageView->setAnchorPoint(ANCHOR_M);
    pageView->setPosition(Vec2MC(0, 0));
    pageView->setContentSize(SB_WIN_SIZE);
    addChild(pageView);
    
    // 월드 페이지 생성
    for( int i = 0; i < GAME_CONFIG->getWorldCount(); ++i ) {
        auto page = WorldPage::create(i+1);
        page->setOnClickListener([=](StageCell *cell) {
            this->replaceGameScene(cell->getStage());
        });
        pageView->insertCustomItem(page, i);
    }
    
    // 마지막으로 플레이한 월드로 포커스
    int latestPlayWorld = GAME_CONFIG->getWorldAtStage(User::getLatestPlayStage());
    pageView->setCurrentPageIndex(latestPlayWorld-1);
}
