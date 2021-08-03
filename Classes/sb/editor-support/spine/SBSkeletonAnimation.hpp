//
//  SBSkeletonAnimation.hpp
//
//  Created by seongmin hwang on 29/07/2019.
//

#ifndef SBSkeletonAnimation_hpp
#define SBSkeletonAnimation_hpp

#include <stdio.h>

#include <spine/spine-cocos2dx.h>

#include "../../base/SBTypes.hpp"

class SBSkeletonAnimation : public spine::SkeletonAnimation {
public:
    static SBSkeletonAnimation* create(const std::string &skeletonJsonFile, float scale = 1);
    virtual ~SBSkeletonAnimation();
    
protected:
    SBSkeletonAnimation();
    virtual void initialize() override;
    
public:
    virtual void update(float dt) override;
    
    virtual spine::TrackEntry* runAnimation(const std::string &name,
                                            bool isAutoRemoveOnFinish = false,
                                            spine::CompleteListener onFinished = nullptr);
    
    float getAnimationDuration(const std::string &name);
};

#endif /* SBSkeletonAnimation_hpp */
