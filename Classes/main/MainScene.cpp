//
//  MainScene.cpp
//
//  Created by seongmin hwang on 2018. 5. 14..
//

#include "MainScene.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
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
#include "GetCharacterPopup.hpp"
#include "ShopPopup.hpp"
#include "SettingPopup.hpp"
#include "SalePopup.hpp"
#include "BannerView.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace cocos2d::ui;
using namespace std;

MainScene* MainScene::create(int selectedWorld) {
    
    auto scene = new MainScene();
    
    if( scene && scene->init(selectedWorld) ) {
        scene->autorelease();
        return scene;
    }
    
    CC_SAFE_DELETE(scene);
    return nullptr;
}

MainScene::MainScene():
pageIndex(0),
pageDiffCount(0) {
    
}

MainScene::~MainScene() {
    
}

bool MainScene::init(int selectedWorld) {
    
    if( !BaseScene::init() ) {
        return false;
    }
    
    SBAnalytics::setCurrentScreen(ANALYTICS_SCREEN_MAIN);
    
    if( selectedWorld <= 0 ) {
        // 기본값 - 마지막으로 플레이한 월드
        selectedWorld = StageManager::getLatestPlayWorld();
    }
    
    initBg(selectedWorld);
    initMenu();
    initWorlds(selectedWorld);
    
    // 게임 오버 횟수 리셋
    User::setGameOverCountForSkipStage(0);
    
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
    
    // 캐릭터 잠금 해제 체크
    CHARACTER_MANAGER->checkUnlock([=](CharacterDataList unlockCharacters) {
        // 캐릭터 획득 팝업
        GetCharacterPopup::show(unlockCharacters);
    });
    
    // 세일 팝업
    if( !User::isRemovedAds() /* 아이템 없음 */ &&
        !SalePopup::isTodayOpened() /* 하루 1회 */ &&
        SceneManager::getPreviousSceneType() == SceneType::GAME /* 이전 씬이 게임 */ &&
        StageManager::getTopUnlockedStage() >= 11 /* 10스테이지까지 클리어됨 */ ) {
        
        auto popup = SalePopup::create();
        addChild(popup, ZOrder::POPUP_MIDDLE);
    }
    
    // 네트워크 연결 상태 체크
    bool isNetworkOnline = SBPlatformHelper::isNetworkOnline();
    
    SBAnalytics::EventParams params;
    params[ANALYTICS_EVENT_PARAM_STATUS] = SBAnalytics::EventParam(isNetworkOnline ? "online" : "offline");
    
    SBAnalytics::logEvent(ANALYTICS_EVENT_SB_NETWORK_STATUS, params);
    
    if( !isNetworkOnline ) {
        SBAnalytics::logEvent(ANALYTICS_EVENT_SB_NETWORK_STATUS_OFFLINE);
    }
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
            showShopPopup();
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
    
    // 스테이지 유무 체크
    if( StageManager::getStage(stage).isNull() ) {
        MessageBox("스테이지 데이터가 없습니다", "");
        return;
    }
    
    SB_SAFE_HIDE(getChildByTag(Tag::BTN_SHOP));
    SB_SAFE_HIDE(getChildByTag(Tag::BTN_SETTING));
    
    GAME_MANAGER->init();
    GAME_MANAGER->setStage(stage);
    
    replaceScene(SceneType::GAME);
}

/**
 * 상점 팝업을 띄웁니다
 */
void MainScene::showShopPopup() {
    
    auto popup = ShopPopup::create();
    SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
}

/**
 * 설정 팝업을 띄웁니다
 */
void MainScene::showSettingPopup() {
    
    auto popup = SettingPopup::create();
    SceneManager::getScene()->addChild(popup, ZOrder::POPUP_MIDDLE);
}

void MainScene::initBg(int selectedWorld) {
    
    auto bg = Sprite::create(ResourceHelper::getWorldBackgroundImage(selectedWorld));
    bg->setTag(Tag::BG);
    // bg->setScale(SB_WIN_SIZE.width / bg->getContentSize().width);
    bg->setScaleX(SB_WIN_SIZE.width / bg->getContentSize().width);
    bg->setScaleY(SB_WIN_SIZE.height / bg->getContentSize().height);
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(0,0));
    addChild(bg);
    
    auto cover = Sprite::create(DIR_IMG_MAIN + "main_bg_cover.png");
    // cover->setScale(SB_WIN_SIZE.width / cover->getContentSize().width);
    cover->setScaleX(SB_WIN_SIZE.width / cover->getContentSize().width);
    cover->setScaleY(SB_WIN_SIZE.height / cover->getContentSize().height);
    cover->setAnchorPoint(ANCHOR_M);
    cover->setPosition(Vec2MC(0,0));
    addChild(cover);
}

/**
 * 메뉴 초기화
 */
void MainScene::initMenu() {
    
    SBUIInfo infos[] = {
        SBUIInfo(Tag::BTN_SHOP,    ANCHOR_M, Vec2TL(210, -104),  DIR_IMG_MAIN + "main_btn_shop.png"),
        SBUIInfo(Tag::BTN_SETTING, ANCHOR_M, Vec2TR(-104, -104), DIR_IMG_MAIN + "main_btn_menu.png"),
    };
    
    for( int i = 0; i < sizeof(infos)/sizeof(SBUIInfo); ++i ) {
        auto info = infos[i];
        
        auto btn = SBButton::create(info.file);
        btn->setZoomScale(ButtonZoomScale::NORMAL);
        info.apply(btn);
        addChild(btn, ZOrder::COMMON_MENU_BOTTOM);
        
        btn->setOnClickListener(CC_CALLBACK_1(MainScene::onClick, this));
    }
    
    // 상점 버튼 - 캐릭터 로테이션 연출
    auto shopBtn = getChildByTag<SBButton*>(Tag::BTN_SHOP);

    auto chcImage = Sprite::create();
    chcImage->setAnchorPoint(ANCHOR_M);
    chcImage->setPosition(shopBtn->getContentsLayer()->convertToNodeSpace(Vec2TL(111, -46)));
    shopBtn->getContentsLayer()->addChild(chcImage);

    StringList charImageList;

    for( int i = 0; i < 8; ++i ) {
        charImageList.push_back(ResourceHelper::getCharacterImage(CHARACTER_MANAGER->getCharacters()[i].charId));
    }

    random_shuffle(charImageList.begin(), charImageList.end());

    auto anims = SBNodeUtils::createAnimation(charImageList, 0.4f);
    chcImage->runAction(RepeatForever::create(Animate::create(anims)));
}

/**
 * 월드 페이지 초기화
 */
void MainScene::initWorlds(int selectedWorld) {
    
    auto worldTitle = Sprite::create(DIR_IMG_MAIN + STR_FORMAT("main_title_world_%02d.png",
                                                               selectedWorld));
    worldTitle->setTag(Tag::WORLD_TITLE);
    worldTitle->setAnchorPoint(ANCHOR_M);
    worldTitle->setPosition(Vec2TC(0, -124));
    addChild(worldTitle);
    
    auto pageView = PageView::create();
    pageView->setDirection(PageView::Direction::HORIZONTAL);
    pageView->setBounceEnabled(true);
    pageView->setAnchorPoint(ANCHOR_M);
    pageView->setPosition(Vec2MC(0, 0));
    pageView->setContentSize(SB_WIN_SIZE);
    addChild(pageView);
    
    // 인디게이터
    pageView->setIndicatorEnabled(false);
    pageView->setIndicatorEnabled(true);
    pageView->setIndicatorIndexNodesTexture(DIR_IMG_MAIN + "main_indicator_dot.png");
    pageView->setIndicatorPosition(Vec2TC(0, -248) + Vec2(0, -32 / 2));
    // pageView->setIndicatorIndexNodesScale(0.55f);
    pageView->setIndicatorIndexNodesColor(Color3B(0, 0, 0));
    pageView->setIndicatorIndexNodesOpacity(255*0.5f);
    pageView->setIndicatorSelectedIndexColor(Color3B(29, 226, 255));
    pageView->setIndicatorSelectedIndexOpacity(255);
    pageView->setIndicatorSpaceBetweenIndexNodes(28);
    
    // SBNodeUtils::recursiveCascadeOpacityEnabled(pageView, true);
    
    // 월드 페이지 생성
    for( int i = 0; i < GAME_CONFIG->getWorldCount()+1; ++i ) {
        auto page = WorldPage::create(i+1);
        page->setOnClickListener([=](StageCell *cell) {
            this->replaceGameScene(cell->getStage());
        });
        pageView->insertCustomItem(page, i);
    }
    
    // 마지막으로 플레이한 월드로 포커스
    pageView->setCurrentPageIndex(selectedWorld-1);
    
    // 페이지 전환 시 타이틀 업데이트
    auto onPageChanged = [=](size_t page) {
        
        this->pageIndex = page;
        int world = (int)page + 1;
        
        worldTitle->setTexture(DIR_IMG_MAIN + STR_FORMAT("main_title_world_%02d.png", world));
        
        auto bg = getChildByTag<Sprite*>(Tag::BG);
        bg->setTexture(ResourceHelper::getWorldBackgroundImage(world));
    };
    
//    pageView->addEventListener([=](Ref*, PageView::EventType eventType) {
//        if( eventType == PageView::EventType::TURNING ) {
//            onPageChanged(pageView->getCurrentPageIndex());
//        }
//    });
    
    pageView->addEventListener([=](Ref*, ScrollView::EventType eventType) {
       
        switch( eventType ) {
            // 페이지 스크롤 진행중
            case ScrollView::EventType::CONTAINER_MOVED: {
                ssize_t i = pageView->getCurrentPageIndex();
                if( i < 0 ) {
                    return;
                }
                
                if( pageIndex == i ) {
                    return;
                }
                
                ++pageDiffCount;

                // 페이지 전환
                if( pageDiffCount == 3 ) {
                    pageDiffCount = 0;
                    onPageChanged(i);
                }
            } break;
                
            // 스크롤 완료
            case ScrollView::EventType::AUTOSCROLL_ENDED: {
                pageDiffCount = 0;
                
                if( pageIndex != pageView->getCurrentPageIndex() ) {
                    onPageChanged(pageView->getCurrentPageIndex());
                }
            } break;
                
            default: break;
        }
    });
}
