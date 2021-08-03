//
//  SBTransition.cpp
//
//  Created by seongmin hwang on 06/03/2019.
//

#include "SBTransition.hpp"

USING_NS_CC;

NS_SB_BEGIN

const unsigned int kSceneFade = 0xFADEFADE;

//
// FadeOut Transition
//
TransitionFadeOut::TransitionFadeOut() {
}

TransitionFadeOut::~TransitionFadeOut() {
}

TransitionFadeOut* TransitionFadeOut::create(float duration, float inDelay, Scene *scene, const Color3B &color) {
    
    auto transition = new (std::nothrow) TransitionFadeOut();
    transition->initWithDuration(duration, inDelay, scene, color);
    transition->autorelease();
    return transition;
}

TransitionFadeOut* TransitionFadeOut::create(float duration, Scene *scene, const Color3B &color) {
    
    return create(duration, 0, scene, color);
}

bool TransitionFadeOut::initWithDuration(float duration, float inDelay, Scene *scene, const Color3B &color) {
    
    if( TransitionScene::initWithDuration(duration, scene) ) {
        this->inDelay = inDelay;
        this->color.r = color.r;
        this->color.g = color.g;
        this->color.b = color.b;
        this->color.a = 0;
    }
    
    return true;
}

void TransitionFadeOut::onEnter() {
    
    TransitionScene::onEnter();
    
    _inScene->setVisible(false);
    
    auto fadeLayer = LayerColor::create(color);
    addChild(fadeLayer, 2);
    
    auto a = Sequence::create(FadeIn::create(_duration),
                              DelayTime::create(inDelay),
                              CallFunc::create(CC_CALLBACK_0(TransitionScene::hideOutShowIn, this)),
                              CallFunc::create(CC_CALLBACK_0(TransitionScene::finish, this)),
                              RemoveSelf::create(), nullptr);
    fadeLayer->runAction(a);
}

void TransitionFadeOut::onExit() {
    
    TransitionScene::onExit();
}

NS_SB_END
