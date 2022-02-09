//
//  WelcomeScene.cpp
//
//  Created by seongmin hwang on 2018. 5. 14..
//

#include "WelcomeScene.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "User.hpp"
#include "SceneManager.h"
#include "PopupManager.hpp"
#include "GameConfiguration.hpp"

#include "../test/TestHelper.hpp"
#include "../game/GameDefine.h"

#include "CommonLoadingBar.hpp"
#include "ExitAlertPopup.hpp"
#include "SettingPopup.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace cocos2d::ui;
using namespace std;

WelcomeScene::WelcomeScene() {
    
}

WelcomeScene::~WelcomeScene() {
    
}

bool WelcomeScene::init() {
    
    if( !BaseScene::init() ) {
        return false;
    }
    
    SBAnalytics::setCurrentScreen(ANALYTICS_SCREEN_WELCOME);
    
    initBg();
    initTitle();
    initMenu();
    
    return true;
}

void WelcomeScene::onEnter() {
    
    BaseScene::onEnter();
    
    // bgm
    scheduleOnce([=](float) {
        SBAudioEngine::playBGM(SOUND_BGM_MAIN);
    }, SceneManager::REPLACE_DURATION_MAIN, "MAIN_SCENE_BGM_DELAY");
}

void WelcomeScene::onEnterTransitionDidFinish() {
    
    BaseScene::onEnterTransitionDidFinish();
    
    // 자동 씬 전환
    scheduleOnce([=](float dt) {
        //onClick(getChildByTag(Tag::BTN_START));
    }, 0.1f, "AUTO_REPLACE");
}

void WelcomeScene::onExit() {
    
    BaseScene::onExit();
}

bool WelcomeScene::onBackKeyReleased() {
    
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
void WelcomeScene::onClick(Node *sender) {
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    
    switch( sender->getTag() ) {
        // 크레딧
        case Tag::BTN_CREDIT: {
            SBAnalytics::logEvent(ANALYTICS_EVENT_CREDIT);
            
//            auto popup = PopupManager::show(PopupType::CREDIT);
//            popup->setLocalZOrder(ZOrder::POPUP_TOP);
//            popup->setOnDismissListener([=](Node*) {
//            });
        } break;
            
        // 게임 시작
        case Tag::BTN_START: {
            replaceMainScene();
        } break;
            
        // test
        case Tag::BTN_TEST: {
        } break;
            
        default:
            break;
    }
}

/**
 * 메인씬으로 전환
 */
void WelcomeScene::replaceMainScene() {
    
    auto onAdClosed = [=]() {
        replaceScene(SceneType::MAIN);
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
void WelcomeScene::showSettingPopup() {
    
    auto popup = SettingPopup::create();
    SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
}

void WelcomeScene::initBg() {
    
    int latestPlayWorld = User::getLatestPlayStage().world;
    
    auto bg = Sprite::create(ResourceHelper::getWorldBackgroundImage(latestPlayWorld));
    bg->setScale(SB_WIN_SIZE.width / bg->getContentSize().width);
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(0,0));
    addChild(bg);
    
    // 타이틀
    auto title = Sprite::create(DIR_IMG_MAIN + "lobby_game_title.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2MC(0, 62));
    addChild(title);
    
    // 크레딧
    auto creditBtn = SBNodeUtils::createTouchNode();
    creditBtn->setTag(Tag::BTN_CREDIT);
    creditBtn->setAnchorPoint(ANCHOR_M);
    creditBtn->setPosition(Vec2MC(0, 50));
    creditBtn->setContentSize(Size(title->getContentSize().width * 0.7f, 250));
    addChild(creditBtn, SBZOrder::MIDDLE);
    
    creditBtn->addClickEventListener([=](Ref*) {
        this->onClick(creditBtn);
    });
    
    // 크레딧 영역 확인용
    // creditBtn->addChild(SBNodeUtils::createBackgroundNode(creditBtn, Color4B(255,0,0,255*0.5f)));
    //
}

void WelcomeScene::initTitle() {
}

/**
 * 메뉴 초기화
 */
void WelcomeScene::initMenu() {
    
    // 탭하여 시작
    auto tapToStart = Sprite::create(DIR_IMG_MAIN + "lobby_touch_to_start.png");
    tapToStart->setAnchorPoint(ANCHOR_M);
    tapToStart->setPosition(Vec2MC(0, -328));
    addChild(tapToStart);
    
    auto btn = SBNodeUtils::createTouchNode();
    btn->setTag(Tag::BTN_START);
    btn->setContentSize(Size(SB_WIN_SIZE.width*0.9f, SB_WIN_SIZE.height*0.85f));
    addChild(btn);
    
    btn->addClickEventListener([=](Ref*) {
        this->onClick(btn);
    });
    
    // blink
    {
        auto delay = DelayTime::create(0.5f);
        auto callFunc = CallFunc::create([=]() {
            auto blink = RepeatForever::create(Blink::create(1.0f, 1));
            tapToStart->runAction(blink);
        });
        tapToStart->runAction(Sequence::create(delay, callFunc, nullptr));
    }
}
