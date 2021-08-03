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
    initTitle();
    initMenu();
    
    // 개발 버전 표기
    /*
    auto versionLabel = Label::createWithTTF(DEV_VERSION, FONT_ROBOTO_BLACK, 30, Size::ZERO,
                                             TextHAlignment::RIGHT, TextVAlignment::BOTTOM);
    versionLabel->setTextColor(Color4B::WHITE);
    versionLabel->setAnchorPoint(ANCHOR_BR);
    versionLabel->setPosition(Vec2BR(0,120));
    addChild(versionLabel, INT_MAX);
    */
    
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
        // 크레딧
        case Tag::BTN_CREDIT: {
            SBAnalytics::logEvent(ANALYTICS_EVENT_CREDIT);
            
            auto popup = PopupManager::show(PopupType::CREDIT);
            popup->setLocalZOrder(ZOrder::POPUP_TOP);
            popup->setOnDismissListener([=](Node*) {
            });
        } break;
            
        // 게임 시작
        case Tag::BTN_START: {
            replaceGameScene();
        } break;

        // 리더 보드
        case Tag::BTN_LEADER_BOARD: {
            if( superbomb::PluginPlay::isSignedIn() ) {
                superbomb::PluginPlay::showAllLeaderboards();
            } else {
                superbomb::PluginPlay::signIn();
            }
        } break;
            
        // 설정
        case Tag::BTN_SETTING: {
            showSettingPopup();
        } break;
            
        // test
        case Tag::BTN_TEST: {
        } break;
            
        default:
            break;
    }
}

/**
 * 게임씬으로 전환
 */
void MainScene::replaceGameScene() {
    
    SB_SAFE_HIDE(getChildByTag(Tag::BTN_SETTING));
    
    auto onAdClosed = [=]() {
        GAME_MANAGER->init();
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
    
    // 배너
    if( !User::isRemovedAds() ) {
        auto bannerView = BannerView::create();
        addChild(bannerView, SBZOrder::TOP);
    }
    
    // 크레딧
    auto creditBtn = SBNodeUtils::createTouchNode();
    creditBtn->setTag(Tag::BTN_CREDIT);
    creditBtn->setAnchorPoint(ANCHOR_M);
    creditBtn->setPosition(Vec2MC(0, 200));
    creditBtn->setContentSize(Size(SB_WIN_SIZE.width*0.6f, 200));
    addChild(creditBtn, SBZOrder::MIDDLE);
    
    creditBtn->addClickEventListener([=](Ref*) {
        this->onClick(creditBtn);
    });
}

void MainScene::initTitle() {
 
    titleColors = TILE_COLORS;
    titleColorIndex = 0;
    
    // random_shuffle(titleColors.begin(), titleColors.end());

//    auto stencil = Sprite::create(DIR_IMG_MAIN + "main_title_mask.png");
//    stencil->setAnchorPoint(Vec2::ZERO);
//    stencil->setPosition(Vec2::ZERO);
//
//    auto clippingNode = ClippingNode::create(stencil);
//    clippingNode->setAlphaThreshold(0);
////    clippingNode->setInverted(true);
//    clippingNode->setAnchorPoint(ANCHOR_M);
//    clippingNode->setPosition(Vec2MC(0, 250));
//    clippingNode->setContentSize(stencil->getContentSize());
//    addChild(clippingNode);
//
//    /*
//    auto title = Sprite::create(DIR_IMG_MAIN + "main_title.png");
//    title->setAnchorPoint(Vec2::ZERO);
//    title->setPosition(Vec2::ZERO);
//    title->setColor(titleColors[titleColors.size()-1]);
//    clippingNode->addChild(title);
//    */
//
//    auto cover = Sprite::create(DIR_IMG_MAIN + "main_title_cover.png");
//    cover->setAnchorPoint(ANCHOR_MB);
//    cover->setPosition(Vec2BC(clippingNode->getContentSize(), 0, -cover->getContentSize().height));
//    cover->setColor(titleColors[0]);
//    clippingNode->addChild(cover);

    int firstColorIdx = arc4random() % (titleColors.size()-1);
    firstColorIdx += 1;
    
    auto title = Sprite::create(DIR_IMG_MAIN + "main_title.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2MC(-2, 101));
    title->setColor(titleColors[firstColorIdx]);
    addChild(title);
    
    Vec2 COVER_POSITION = Vec2MC(0, -80);
    
    auto cover = Sprite::create(DIR_IMG_MAIN + "main_title_cover.png");
    cover->setAnchorPoint(ANCHOR_MT);
    cover->setPosition(COVER_POSITION);
    cover->setColor(titleColors[titleColorIndex++]);
    addChild(cover);
    
    auto mask = Sprite::create(DIR_IMG_MAIN + "main_title_mask.png");
    mask->setAnchorPoint(ANCHOR_M);
    mask->setPosition(Vec2MC(0,0));
    mask->setColor(GAME_BG_COLOR);
    addChild(mask);
    
    // 연출
    auto move = MoveBy::create(3.0f, Vec2(0, cover->getContentSize().height));
    auto callFunc = CallFunc::create([=]() {
       
        title->setColor(cover->getColor());
        
        cover->setPositionY(COVER_POSITION.y);
        cover->setColor(titleColors[titleColorIndex]);
        
        titleColorIndex++;
        
        if( titleColorIndex == titleColors.size() ) {
            titleColorIndex = 0;
        }
    });
    auto delay = DelayTime::create(0.0f);
    auto seq = Sequence::create(move, callFunc, delay, nullptr);
    cover->runAction(RepeatForever::create(seq));
}

/**
 * 메뉴 초기화
 */
void MainScene::initMenu() {
    
    // 탭하여 시작
    auto tapToStart = Label::createWithTTF("TAP TO START", FONT_ROBOTO_BLACK, 40, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    tapToStart->setTextColor(Color4B(239, 255, 233, 255));
    tapToStart->setAnchorPoint(ANCHOR_M);
    tapToStart->setPosition(Vec2BC(0, 250));
    addChild(tapToStart);
    
    auto btn = SBNodeUtils::createTouchNode();
    btn->setTag(Tag::BTN_START);
    btn->setAnchorPoint(ANCHOR_MB);
    btn->setPosition(Vec2BC(0, 80));
    btn->setContentSize(Size(SB_WIN_SIZE.width*0.85f, SB_WIN_SIZE.height*0.40f));
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
        SBUIInfo(Tag::BTN_LEADER_BOARD, ANCHOR_M, Vec2TL(56, -54),
                 DIR_IMG_MAIN + "main_btn_leaderboard.png"),
        SBUIInfo(Tag::BTN_SETTING, ANCHOR_M, Vec2TR(-56, -54),
                 DIR_IMG_COMMON + "common_btn_more.png"),
    };
    
    for( int i = 0; i < sizeof(infos)/sizeof(SBUIInfo); ++i ) {
        auto info = infos[i];
        
        auto btn = SBButton::create(info.file);
        btn->setZoomScale(ButtonZoomScale::NORMAL);
        info.apply(btn);
        addChild(btn);
        
        btn->setOnClickListener(CC_CALLBACK_1(MainScene::onClick, this));
    }
}
