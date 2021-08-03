//
//  LogoView.cpp
//
//  Created by seongmin hwang on 2018. 6. 14..
//

#include "LogoView.hpp"
#include <spine/spine-cocos2dx.h>

#include "Define.h"

USING_NS_CC;
using namespace std;

static const string FINISHED_SCHEDULER        = "FINISHED_SCHEDULER";

LogoView::LogoView() :
onFinishedListener(nullptr),
anim(nullptr) {
    
}

LogoView::~LogoView() {
    
}

bool LogoView::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2::ZERO);
    setContentSize(Size::ZERO);
    
    addChild(LayerColor::create(Color4B::WHITE));
    addChild(SBNodeUtils::createTouchNode());
    
    // 스파인 애니메이션 생성
    auto anim = SBSkeletonAnimation::create(ANIM_LOGO);
    anim->setAnchorPoint(Vec2::ZERO);
    anim->setPosition(Vec2(SB_WIN_SIZE*0.5f));
    addChild(anim);
    
    anim->setEndListener([=](spine::TrackEntry *entry) {
        
        CCLOG("logo anim end");
    });
    
    anim->setCompleteListener([=](spine::TrackEntry *entry) {
        
        CCLOG("logo anim completed");
    });
    
    this->anim = anim;
    
    return true;
}

void LogoView::logoFinished() {
 
    if( onFinishedListener ) {
        onFinishedListener();
    }
}

void LogoView::run() {
    
    auto skeletonAnim = dynamic_cast<SBSkeletonAnimation*>(anim);
    
    const float ANIM_DURATION = skeletonAnim->getAnimationDuration(ANIM_NAME_RUN);
    CCLOG("logo anim duration: %f", ANIM_DURATION);
    
    auto track = skeletonAnim->setAnimation(0, ANIM_NAME_RUN, false);
    skeletonAnim->update(0);

    skeletonAnim->setTrackEndListener(track, [=](spine::TrackEntry *entry) {
        
        CCLOG("track logo anim end");
    });
    
    skeletonAnim->setTrackCompleteListener(track, [=](spine::TrackEntry *entry) {
        
        CCLOG("track logo anim completed");
    });
    
    // scheduler
    scheduleOnce([=](float dt) {
        this->logoFinished();
    }, 3.5f/*ANIM_DURATION*/, FINISHED_SCHEDULER);
}
