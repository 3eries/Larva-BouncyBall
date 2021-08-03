//
//  SBSkeletonAnimation.cpp
//
//  Created by seongmin hwang on 29/07/2019.
//

#include "SBSkeletonAnimation.hpp"

#include "../../base/SBMacros.h"
#include "../../utils/SBStringUtils.h"

USING_NS_CC;
using namespace std;

SBSkeletonAnimation* SBSkeletonAnimation::create(const string &skeletonJsonFile, float scale) {
    
    string atlasFile = SBStringUtils::replaceAll(skeletonJsonFile, ".json", ".atlas");
    
    auto anim = new SBSkeletonAnimation();
    anim->initWithJsonFile(skeletonJsonFile, atlasFile, scale);
    anim->autorelease();
    return anim;
}

SBSkeletonAnimation::SBSkeletonAnimation() {
}

SBSkeletonAnimation::~SBSkeletonAnimation() {
}

void SBSkeletonAnimation::initialize() {
    
    spine::SkeletonAnimation::initialize();
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2MC(0,0));
}

void SBSkeletonAnimation::update(float dt) {
    
    retain();
    spine::SkeletonAnimation::update(dt);
    release();
}

spine::TrackEntry* SBSkeletonAnimation::runAnimation(const string &name,
                                                     bool isAutoRemoveOnFinish,
                                                     spine::CompleteListener onFinished) {
    
    auto track = setAnimation(0, name, false);
    
    if( isAutoRemoveOnFinish || onFinished ) {
        setTrackCompleteListener(track, [=](spine::TrackEntry *entry) {
            this->retain();
            
            if( onFinished ) {
                onFinished(entry);
            }
            
            if( isAutoRemoveOnFinish ) {
                this->removeFromParent();
            }
            
            this->release();
        });
    }
    
    return track;
}

/**
 *  애니메이션 duration을 반환한다.
 */
float SBSkeletonAnimation::getAnimationDuration(const string &name) {
    
    auto animation = findAnimation(name);
    
    if( !animation ) {
        CCLOG("SBSkeletonAnimation::getAnimationDuration error: animation not found.");
        return 0;
    }
    
    return animation->getDuration();
}
