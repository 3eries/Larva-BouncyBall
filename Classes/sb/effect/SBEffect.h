//
//  SBEffect.h
//  SBLib
//
//  Created by hwangseongmin on 2015. 6. 1..
//
//

#ifndef __SBLib__SBEffect__
#define __SBLib__SBEffect__

#include <stdio.h>

#include "cocos2d.h"

#include "../base/SBMacros.h"

NS_SB_BEGIN

#pragma mark- Effect

class EffectSprite;

class Effect : public cocos2d::Ref {
public:
    static Effect* create(const std::string &fragmentFilename);
    virtual ~Effect();
    
    virtual void setTarget(EffectSprite *sprite){}
    
protected:
    Effect();
    bool init(const std::string &fragmentFilename);
    
protected:
    CC_SYNTHESIZE_READONLY(cocos2d::GLProgramState*, glprogramstate, GLProgramState);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    std::string _fragSource;
    cocos2d::EventListenerCustom* _backgroundListener;
#endif
};

#pragma mark- EffectSprite

class EffectSprite : public cocos2d::Sprite {
public:
    CREATE_FUNC(EffectSprite);
    static EffectSprite* create(const std::string &filename);
    static EffectSprite* create(cocos2d::Texture2D *tex);
    virtual ~EffectSprite();
    
protected:
    EffectSprite();
    
public:
    void setEffect(Effect* effect);
    void addEffect(Effect *effect, ssize_t order);
    
    void draw(cocos2d::Renderer *renderer,
              const cocos2d::Mat4 &transform, uint32_t flags) override;
    
protected:
    std::vector<std::tuple<ssize_t,Effect*,cocos2d::QuadCommand>> effects;
    Effect *defaultEffect;
};

NS_SB_END

#endif /* defined(__SBLib__SBEffect__) */
