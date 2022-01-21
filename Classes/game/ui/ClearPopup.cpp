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

ClearPopup::ClearPopup(): BasePopup(PopupType::GAME_CLEAR) {
    
}

ClearPopup::~ClearPopup() {
    
}

bool ClearPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

