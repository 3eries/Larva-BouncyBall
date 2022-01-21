//
//  ClearPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#include "ClearPopup.hpp"

#include "Define.h"

USING_NS_CC;
using namespace std;

ClearPopup::ClearPopup(): BasePopup(PopupType::GAME_CLEAR),
onHomeListener(nullptr),
onRetryListener(nullptr),
onNextListener(nullptr) {
}

ClearPopup::~ClearPopup() {
    
}

bool ClearPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void ClearPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool ClearPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    // 반응 없음
    
    return true;
}

void ClearPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void ClearPopup::initContentView() {
    
    BasePopup::initContentView();
}

/**
 * 등장 연출
 */
void ClearPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(0/* ACTION DURATION */, onFinished);
}

/**
 * 등장 연출 완료
 */
void ClearPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
}


