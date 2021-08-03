//
//  SceneManager.cpp
//
//  Created by seongmin hwang on 2018. 2. 22..
//

#include "SceneManager.h"

#include "Define.h"

#include "../splash/SplashScene.hpp"
#include "../main/MainScene.hpp"
#include "../game/GameScene.hpp"

USING_NS_CC;
using namespace std;

const float SceneManager::REPLACE_DURATION_SPLASH_TO_MAIN = 1.5f;
const float SceneManager::REPLACE_DURATION_MAIN = 0.3f;
const float SceneManager::REPLACE_DURATION_GAME = 0.4f;

static SceneManager *instance = nullptr;
SceneManager* SceneManager::getInstance() {
    
    if( !instance ) {
        instance = new SceneManager();
    }
    
    return instance;
}

void SceneManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

SceneManager::SceneManager() :
sceneType(SceneType::NONE),
prevSceneType(SceneType::NONE),
scene(nullptr),
isRunningReplaceScene(false),
isBackKeyLocked(false) {
}

SceneManager::~SceneManager() {
}

/**
 * Scene 생성
 */
Scene* SceneManager::createScene(SceneType type) {
    
    switch( type ) {
        case SceneType::SPLASH:          return SplashScene::create();
        case SceneType::MAIN:            return MainScene::create();
        case SceneType::GAME:            return GameScene::create();
        default:
            CCASSERT(false, "SceneManager::createScene error: invalid scene type.");
            break;
    }
    
    return nullptr;
}

/**
 * Scene 전환
 */
void SceneManager::replace(SceneType type, function<Scene*()> createSceneFunc) {
    
    CCASSERT(type != SceneType::NONE, "SceneManager::replaceScene error: invalid scene type.");
    CCASSERT(createSceneFunc != nullptr, "SceneManager::replaceScene error: create scene function is null.");
    
    if( isRunningReplaceScene ) {
        return;
    }
    
    prevSceneType = sceneType;
    auto prevScene = scene;
    
    isRunningReplaceScene = true;
    sceneType = type;
    
    // Scene 생성
    scene = createSceneFunc();
    auto trans = (Scene*)scene;
    
    CCASSERT(scene != nullptr, "SceneManager::replaceScene error: invalid scene.");
    
    // Scene 전환 효과
    switch( type ) {
        case SceneType::SPLASH:          break;
        case SceneType::MAIN: {
            // Splash -> Main
            if( prevSceneType == SceneType::SPLASH ) {
                /*
                 auto splashScene = dynamic_cast<SplashScene*>(prevScene);
                 
                 if( splashScene->isLogoMode() ) {
                 trans = TransitionCrossFade::create(REPLACE_DURATION_SPLASH_TO_MAIN, scene);
                 } else {
                 trans = TransitionFade::create(REPLACE_DURATION_MAIN, scene);
                 }
                 */
            }
            // XXX -> Main
            else {
                trans = TransitionFade::create(REPLACE_DURATION_MAIN, scene);
            }
        } break;
        case SceneType::GAME: {
            trans = TransitionSlideInR::create(REPLACE_DURATION_GAME, scene);
            // trans = TransitionFade::create(REPLACE_DURATION_GAME, scene);
        } break;
            
        default: break;
    }
    
    // Scene 전환
    Director::getInstance()->replaceScene(trans);
    
    // 터치 잠금 강제 해제
    SBDirector::getInstance()->setScreenTouchLocked(false);
    
    // Scene 전환 중복 방지
    Director::getInstance()->getScheduler()->schedule([=](float dt) {
        this->isRunningReplaceScene = false;
    }, this, 0, 0, 0.1f, false, "SCENE_ON_ENTER");
}

void SceneManager::replace(SceneType type) {
    
    return replace(type, [=]() -> Scene* {
        return this->createScene(type);
    });
}

/**
 * 백키 이벤트 핸들링
 * Scene 공통으로 처리할 로직 구현
 */
bool SceneManager::onBackKeyReleased() {
    
    if( getInstance()->isBackKeyLocked ) {
        return true;
    }
    
    if( !getScene() ) {
        return false;
    }
    
    auto children = getScene()->getChildren();
    
    for( auto child : children ) {
        if( false /*TODO*/) {
            return true;
        }
    }
    
    return false;
}

void SceneManager::setBackKeyLocked(bool isLocked) {
    
    getInstance()->isBackKeyLocked = isLocked;
}

SceneType SceneManager::getSceneType() {
    return instance->sceneType;
}

SceneType SceneManager::getPreviousSceneType() {
    return instance->prevSceneType;
}

Scene* SceneManager::getScene() {
    return instance->scene;
}

bool SceneManager::isGameScene() {
    return getSceneType() == SceneType::GAME;
}

