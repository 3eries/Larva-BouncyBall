//
//  SplashScene.cpp
//
//  Created by seongmin hwang on 2018. 2. 22..
//

#include "SplashScene.hpp"

#include <spine/spine-cocos2dx.h>

#include "Define.h"
#include "User.hpp"
#include "UserDefaultKey.h"
#include "PatchManager.hpp"
#include "ResourceHelper.hpp"
#include "SceneManager.h"
#include "GameUIHelper.hpp"
#include "FunnyHelper.hpp"
#include "StageManager.hpp"
#include "CharacterManager.hpp"
#include "RewardManager.hpp"

#include "LogoView.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace spine;
using namespace std;

static const float  CHECK_REPLACE_MAIN_SCENE_INTERVAL   = 0.05f;
static const string CHECK_REPLACE_MAIN_SCENE_SCHEDULER  = "CHECK_REPLACE_MAIN_SCENE_SCHEDULER";

#define USER_DEFAULT_KEY_PROLOGUE                         "PROLOGUE"

SplashScene::SplashScene() :
isReplacedMainScene(false),
isLaunchImageFinished(false),
isPrologueFinished(false),
isLoginCompleted(false),
logoView(nullptr) {
    
}

SplashScene::~SplashScene() {
    
}

bool SplashScene::init() {
    
    if( !BaseScene::init() ) {
        return false;
    }
    
    ResourceHelper::preload();
    
    initLaunchImage();
    login();
    
    // 메인 화면 전환 체크 스케줄러 실행
    schedule([=](float dt) {
        if( isLaunchImageFinished && isPrologueFinished && isLoginCompleted ) {
            this->replaceMainScene();
        }
    }, CHECK_REPLACE_MAIN_SCENE_INTERVAL, CHECK_REPLACE_MAIN_SCENE_SCHEDULER);
    
    // Firebase 스크린 설정, 모듈이 초기화 되기 전에 실행될 수 있기 때문에 스케줄러 구현
    scheduleOnce([=](float dt) {
        SBAnalytics::setCurrentScreen(ANALYTICS_SCREEN_SPLASH);
    }, 0.1f, "SPLASH_SCREEN_NAME");
    
    return true;
}

void SplashScene::onEnter() {
    
    BaseScene::onEnter();
    
    ConsentForm::show();
}

void SplashScene::onExit() {
    
    BaseScene::onExit();
}

/**
 * 메인화면으로 전환
 */
void SplashScene::replaceMainScene() {
    
    if( isReplacedMainScene ) {
        return;
    }
    
    isReplacedMainScene = true;
    replaceScene(SceneType::WELCOME);
}

/**
 * UI 초기화
 */
void SplashScene::initLaunchImage() {

    logoView = LogoView::create();
    addChild(logoView);
    
    logoView->setOnFinishedListener([=]() {
        this->launchImageFinished();
    });
    logoView->run();
}

/**
 * 런치 이미지 종료
 */
void SplashScene::launchImageFinished() {
    
    CCLOG("SplashScene::launchImageFinished");
    
    if( !isLaunchImageFinished ) {
        isLaunchImageFinished = true;
    }
    
    runPrologue();
}

/**
 * 프롤로그 재생
 */
void SplashScene::runPrologue() {
    
    if( USER_DEFAULT->getBoolForKey(USER_DEFAULT_KEY_PROLOGUE, false) ) {
        isPrologueFinished = true;
        return;
    }
    
    USER_DEFAULT->setBoolForKey(USER_DEFAULT_KEY_PROLOGUE, true);
    USER_DEFAULT->flush();

    logoView->setVisible(false);
    
    auto anim = SBSkeletonAnimation::create(DIR_IMG_SPLASH + "prolog.json");
    anim->setAnchorPoint(Vec2::ZERO);
    anim->setPosition(Vec2MC(0,0));
    addChild(anim, SBZOrder::BOTTOM);
    
    anim->runAnimation(ANIM_NAME_RUN, false, [=](spine::TrackEntry*) {
        CCLOG("프롤로그 끝");
        isPrologueFinished = true;
    });
    
    anim->setEventListener([=](spine::TrackEntry *entry, spine::Event *event) {

        string eventName = event->getData().getName().buffer();
        CCLOG("프롤로그 이벤트 - %s", eventName.c_str());
        
        // cut_in / page
        // 스코어 증가 시작
        if( eventName == "score_start" ) {
        }
    });
}

/**
 * 로그인
 */
void SplashScene::login() {
    
    auto patchMgr = PatchManager::getInstance();
    patchMgr->setOnAppVersion([=](AppVersion version) {
    });
    
    patchMgr->setOnGameConfig([=](string version, string json) {
    });
    
    patchMgr->patch();
    
    loginCompleted();
}

/**
 * 로그인 완료
 */
void SplashScene::loginCompleted() {
    
    if( isLoginCompleted ) {
        return;
    }
    
    isLoginCompleted = true;
    
    StageManager::getInstance()->init();
    CharacterManager::getInstance()->init();
    RewardManager::getInstance()->init();
    User::getInstance()->init();
}


