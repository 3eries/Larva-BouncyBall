//
//  TutorialHelper.cpp
//
//  Created by seongmin hwang on 20/07/2019.
//

#include "TutorialHelper.hpp"

#include "Define.h"

USING_NS_CC;
USING_NS_SB;

static TutorialHelper *instance = nullptr;
TutorialHelper* TutorialHelper::getInstance() {
    
    if( !instance ) {
        instance = new TutorialHelper();
    }
    
    return instance;
}

void TutorialHelper::destroyInstance() {
    
    removeListeners(instance);
    CC_SAFE_DELETE(instance);
}

bool TutorialHelper::isInitialized() {
    
    return instance && instance->initialized;
}

TutorialHelper::TutorialHelper() :
initialized(false) {
}

TutorialHelper::~TutorialHelper() {
    
    removeListeners(this);
}

void TutorialHelper::init(int step, int lastStep) {
    
    initialized = true;
    
    this->step = step;
    this->lastStep = lastStep;
}

bool TutorialHelper::isLastStep() {
    return step == lastStep;
}
