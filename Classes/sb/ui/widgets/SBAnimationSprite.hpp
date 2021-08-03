//
//  SBAnimationSprite.hpp
//
//  Created by seongmin hwang on 2018. 3. 7..
//

#ifndef SBAnimationSprite_hpp
#define SBAnimationSprite_hpp

#include <stdio.h>

#include "cocos2d.h"

#include "../../base/SBTypes.hpp"

class SBAnimationSprite : public cocos2d::Sprite {
public:
    static const int LOOP_FOREVER = -1;
    
public:
    CREATE_FUNC(SBAnimationSprite);
    static SBAnimationSprite* create(cocos2d::Animation *anim, int loops = LOOP_FOREVER);
    static SBAnimationSprite* create(std::vector<std::string> animFiles,
                                     float delayPerUnit, int loops = LOOP_FOREVER);
    virtual ~SBAnimationSprite();
    
protected:
    SBAnimationSprite();
    
    virtual bool init() override;
    virtual bool init(cocos2d::Animation *anim, int loops);
    virtual bool init(cocos2d::Animation *anim);
    
public:
    virtual void setAnimation(cocos2d::Animation *anim, int loops = LOOP_FOREVER);
    virtual void setDelayPerUnit(float delayPerUnit);
    
    virtual void setSpriteFrame(size_t i);
    
    virtual void runAnimation(SBCallbackNode onCompletedListener = nullptr);
    virtual void stopAnimation();
    
protected:
    CC_SYNTHESIZE_READONLY(cocos2d::Animation*, anim, Animation);
    int loops;
    
    SB_SYNTHESIZE_BOOL(runningAnimation, RunningAnimation);
    CC_SYNTHESIZE_READONLY(cocos2d::Animate*, animate, Animate);
    cocos2d::Action *action;
};

#endif /* SBAnimationSprite_hpp */
