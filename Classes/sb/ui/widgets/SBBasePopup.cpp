//
//  SBBasePopup.cpp
//
//  Created by seongmin hwang on 2018. 5. 17..
//

#include "SBBasePopup.hpp"

#include "../../base/SBMacros.h"
#include "../../base/SBDirector.hpp"
#include "../../utils/SBNodeUtils.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

SBBasePopup::SBBasePopup() :
backgroundView(nullptr),
contentView(nullptr),
onDismissListener(nullptr) {
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2::ZERO);
    setContentSize(SB_WIN_SIZE);
}

SBBasePopup::~SBBasePopup() {
    
}

bool SBBasePopup::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    initBackgroundView();
    initContentView();
    
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
    
    // 터치 방지
    {
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        listener->onTouchBegan = [=](Touch *touch, Event*) -> bool {
            
            if( !SBNodeUtils::hasVisibleParents(this) ) {
                return false;
            }
            
            this->onTouch(this, touch, SBTouchEventType::BEGAN);
            
            return true;
        };
        listener->onTouchEnded = [=](Touch *touch, Event*) {
            
            this->onTouch(this, touch, SBTouchEventType::ENDED);
        };
        
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    return true;
}

void SBBasePopup::initBackgroundView() {
 
    backgroundView = Layout::create();
    backgroundView->setAnchorPoint(Vec2::ZERO);
    backgroundView->setPosition(Vec2::ZERO);
    backgroundView->setContentSize(SB_WIN_SIZE);
    addChild(backgroundView, -1);
}

void SBBasePopup::initContentView() {

    contentView = SBNodeUtils::createWinSizeNode();
    addChild(contentView, 1);
}

void SBBasePopup::addBackgroundChild(Node *child, int localZOrder, int tag) {

    if( child->getTag() != Node::INVALID_TAG ) {
        tag = child->getTag();
    }
    
    backgroundView->addChild(child, localZOrder, tag);
}

void SBBasePopup::addContentChild(Node *child, int localZOrder, int tag) {
    
    if( child->getTag() != Node::INVALID_TAG ) {
        tag = child->getTag();
    }
    
    contentView->addChild(child, localZOrder, tag);
}

void SBBasePopup::dismiss() {
    
    retain();
    
    if( onDismissListener ) {
        onDismissListener(this);
    }
    
    removeFromParent();
    release();
}

/**
 * background fade
 */
void SBBasePopup::runBackgroundFadeAction(SBCallback onCompleted, float duration, float from, float to) {
    
    SBNodeUtils::recursiveCascadeOpacityEnabled(backgroundView, true);
    SBDirector::getInstance()->setScreenTouchLocked(true);
    
    backgroundView->setOpacity(from);
    
    auto fade = FadeTo::create(duration, to);
    auto callFunc = CallFunc::create([=]() {
        
        SBDirector::getInstance()->setScreenTouchLocked(false);
        SB_SAFE_PERFORM_LISTENER(this, onCompleted);
    });
    backgroundView->runAction(Sequence::create(fade, callFunc, nullptr));
}

/**
 * background fade in
 */
void SBBasePopup::runBackgroundFadeInAction(SBCallback onCompleted, float duration) {
    
    runBackgroundFadeAction(onCompleted, duration, 0, 255);
}

/**
 * background fade out
 */
void SBBasePopup::runBackgroundFadeOutAction(SBCallback onCompleted, float duration) {
    
    runBackgroundFadeAction(onCompleted, duration, 255, 0);
}

