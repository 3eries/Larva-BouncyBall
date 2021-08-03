//
//  CommonLoadingBar.cpp
//
//  Created by seongmin hwang on 2018. 3. 29..
//

#include "CommonLoadingBar.hpp"

#include "Define.h"
#include "SceneManager.h"
#include "GameUIHelper.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

#define             DEFAULT_MESSAGE          "CONNECT"

static const string SCHEDULER_SHOW_DELAY     = "SCHEDULER_SHOW_DELAY";
static const string SCHEDULER_DISMISS_DELAY  = "SCHEDULER_DISMISS_DELAY";

CommonLoadingBar::CommonLoadingBar() : BasePopup(PopupType::LOADING_BAR),
isShowing(false),
uiDelay(0),
dotCount(0),
message(DEFAULT_MESSAGE) {
}

CommonLoadingBar::~CommonLoadingBar() {
    
}

bool CommonLoadingBar::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    getBackgroundView()->setVisible(false);
    getContentView()->setVisible(false);
    
    return true;
}

void CommonLoadingBar::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

void CommonLoadingBar::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void CommonLoadingBar::initContentView() {
    
    BasePopup::initContentView();
    
    auto loadingLabel = Label::createWithTTF(message, FONT_RETRO, 60);
    loadingLabel->setAnchorPoint(ANCHOR_M);
    loadingLabel->setPosition(Vec2MC(0, 0));
    addContentChild(loadingLabel);
    
    schedule([=](float dt) {
        
        string str = message;
        
        for( int i = 0; i < dotCount; ++i ) {
            str += ".";
        }
        
        loadingLabel->setString(str);
        
        ++dotCount;
        
        if( dotCount > 3 ) {
            dotCount = 0;
        }
        
    }, 0.3f, "CommonLoadingBar Dot");
}

/**
 * 등장 연출
 */
void CommonLoadingBar::runEnterAction(float duratoin, SBCallback onFinished) {
    
    BasePopup::runEnterAction(duratoin, onFinished);
    
    // 배경 fade in
//    runBackgroundFadeInAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide in
//    runSlideAction(onActionFinished,
//                   SLIDE_IN_DURATION,
//                   SLIDE_OUT_POSITION, SLIDE_IN_POSITION);
    
    onEnterActionFinished();
    SB_SAFE_PERFORM_LISTENER(this, onFinished);
}

/**
 * 퇴장 연출
 */
void CommonLoadingBar::runExitAction(float duratoin, SBCallback onFinished) {
    
    BasePopup::runExitAction(duratoin, onFinished);
    
    onExitActionFinished();
    SB_SAFE_PERFORM_LISTENER(this, onFinished);
}

CommonLoadingBar* CommonLoadingBar::show(Node *parent, int zOrder) {
    
    CCASSERT(!isShowing, "CommonLoadingBar::show error: already show.");
    
    isShowing = true;
    
    if( parent == nullptr ) {
        parent = SceneManager::getScene();
    }
    
    CCASSERT(parent != nullptr, "CommonLoadingBar::show error: parent is null.");
    parent->addChild(this, zOrder);
    
    if( uiDelay == 0 ) {
        getBackgroundView()->setVisible(true);
        getContentView()->setVisible(true);
    } else {
        scheduleOnce([=](float dt) {
            getBackgroundView()->setVisible(true);
            getContentView()->setVisible(true);
        }, uiDelay, SCHEDULER_SHOW_DELAY);
    }
    
    return this;
}

CommonLoadingBar* CommonLoadingBar::show(int zOrder) {
    
    return show(nullptr, zOrder);
}

CommonLoadingBar* CommonLoadingBar::dismissWithDelay(float delay) {
    
    if( !isShowing ) {
        return this;
    }
    
    isShowing = false;
    
    // 지연 시간 없음, 노드 제거
    if( delay == 0 ) {
        BasePopup::dismiss();
    }
    // 지연 시간 있음, hide -> 지연 -> 노드 제거
    else {
        getBackgroundView()->setVisible(false);
        getContentView()->setVisible(false);
        
        scheduleOnce([=](float dt) {
            BasePopup::dismiss();
        }, delay, SCHEDULER_DISMISS_DELAY);
    }
    
    return this;
}

CommonLoadingBar* CommonLoadingBar::setMessage(const string &message) {
    
    this->message = message;
    
    return this;
}
