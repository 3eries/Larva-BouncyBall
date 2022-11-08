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
    virtual bool init(const std::string &fragmentFilename);
    
protected:
    CC_SYNTHESIZE_READONLY(cocos2d::GLProgramState*, glprogramstate, GLProgramState);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    std::string _fragSource;
    cocos2d::EventListenerCustom* _backgroundListener;
#endif
};

class EffectOutline: public Effect {
public:
    static EffectOutline* create(const std::string &fragmentFilename,
                                 const cocos2d::Vec3 &color, GLfloat radius, GLfloat threshold);
    virtual ~EffectOutline() {};

protected:
    EffectOutline();
    virtual bool init(const std::string &fragmentFilename,
                      const cocos2d::Vec3 &color, GLfloat radius, GLfloat threshold);
};

class EffectOutline2: public Effect {
public:
    static EffectOutline2* create(const std::string &fragmentFilename,
                                  const cocos2d::Vec3 &color, GLint thickness, cocos2d::Size size);
    virtual ~EffectOutline2() {};

protected:
    EffectOutline2();
    virtual bool init(const std::string &fragmentFilename,
                      const cocos2d::Vec3 &color, GLint thickness, cocos2d::Size size);
};

#pragma mark- EffectSprite

class EffectSprite : public cocos2d::Sprite {
public:
    static EffectSprite* createOutline(cocos2d::Sprite *src,
                                       int pixel, cocos2d::Vec3 color);
    
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
