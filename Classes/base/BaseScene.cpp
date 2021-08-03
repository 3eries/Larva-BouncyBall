//
//  BaseScene.cpp
//
//  Created by seongmin hwang on 2018. 7. 19..
//

#include "BaseScene.hpp"

#include "Define.h"

USING_NS_CC;
USING_NS_SB;
using namespace std;

BaseScene::BaseScene() {
}

BaseScene::~BaseScene() {
    removeListeners(this);
}

bool BaseScene::init() {
    
    if( !Scene::init() ) {
        return false;
    }
    
    // 백그라운드 전환 이벤트 리스너 등록
    {
        auto listener = EventListenerCustom::create(superbomb::EVENT_ENTER_BACKGROUND, [=](EventCustom *event) {
            this->onApplicationEnterBackground();
        });
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    // 포어그라운드 전환 이벤트 리스너 등록
    {
        auto listener = EventListenerCustom::create(superbomb::EVENT_ENTER_FOREGROUND, [=](EventCustom *event) {
            this->onApplicationEnterForeground();
        });
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    // Key 이벤트 리스너 등록
    {
        auto listener = EventListenerKeyboard::create();
        listener->onKeyReleased = [=] (EventKeyboard::KeyCode keyCode, Event *event) {
            
            if( keyCode == EventKeyboard::KeyCode::KEY_BACK ) {
                this->onBackKeyReleased();
            }
        };
        
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    // 터치 방지 노드 초기화
    {
        touchLockNode = Node::create();
        touchLockNode->setVisible(false);
        addChild(touchLockNode, ZOrder::SCENE_TOUCH_LOCKED);
        
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch*, Event*) -> bool {
            return touchLockNode->isVisible();
        };
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, touchLockNode);
    }
    
    return true;
}

void BaseScene::cleanup() {
    
    removeListeners(this);
    
    Scene::cleanup();
}

bool BaseScene::onBackKeyReleased() {
    
    if( SceneManager::onBackKeyReleased() ) {
        return false;
    }
    
    return true;
}

void BaseScene::setSceneTouchLocked(bool isLocekd) {
    
    touchLockNode->setVisible(isLocekd);
}

void BaseScene::replaceScene(SceneType type) {
    
    SceneManager::getInstance()->replace(type);
}
