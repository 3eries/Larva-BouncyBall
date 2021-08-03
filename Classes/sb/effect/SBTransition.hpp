//
//  SBTransition.hpp
//
//  Created by seongmin hwang on 06/03/2019.
//

#ifndef SBTransition_hpp
#define SBTransition_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../base/SBMacros.h"

NS_SB_BEGIN

/** @class TransitionFadeOut
 * @brief Fade out the outgoing scene.
 * Reference: cocos2d::TransitionFade
 */
class CC_DLL TransitionFadeOut : public cocos2d::TransitionScene {
public:
    static TransitionFadeOut* create(float duration, float inDelay,
                                     cocos2d::Scene* scene,
                                     const cocos2d::Color3B &color = cocos2d::Color3B::BLACK);
    static TransitionFadeOut* create(float duration, cocos2d::Scene* scene,
                                     const cocos2d::Color3B &color = cocos2d::Color3B::BLACK);

    virtual void onEnter() override;
    virtual void onExit() override;
    
CC_CONSTRUCTOR_ACCESS:
    TransitionFadeOut();
    virtual ~TransitionFadeOut();
    
    bool initWithDuration(float t, float inDelay,
                          cocos2d::Scene *scene, const cocos2d::Color3B &color);
    
protected:
    float inDelay;
    cocos2d::Color4B color;
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(TransitionFadeOut);
};

NS_SB_END

#endif /* SBTransition_hpp */

