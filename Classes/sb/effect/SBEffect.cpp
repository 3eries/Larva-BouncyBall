//
//  SBEffect.cpp
//  SBLib
//
//  Created by hwangseongmin on 2015. 6. 1..
//
//

#include "SBEffect.h"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN

#pragma mark- Effect

Effect* Effect::create(const string &fragmentFilename) {
    
    auto effect = new (nothrow)Effect();
    
    if( effect && effect->init(fragmentFilename) ) {
        effect->autorelease();
        return effect;
    }
    
    delete effect;
    return nullptr;
}

Effect::Effect() :
glprogramstate(nullptr) {
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}

Effect::~Effect() {
    
    CC_SAFE_RELEASE_NULL(glprogramstate);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

bool Effect::init(const string &fragmentFilename)
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _fragSource = fragSource;
#endif
    
    glprogramstate = (glprogram == nullptr ? nullptr : GLProgramState::getOrCreateWithGLProgram(glprogram));
    CC_SAFE_RETAIN(glprogramstate);
    
    return glprogramstate != nullptr;
}

#pragma mark- EffectSprite

EffectSprite* EffectSprite::create(const string &filename) {
    
    auto spr = new (nothrow)EffectSprite();
    
    if( spr && spr->initWithFile(filename) ) {
        spr->autorelease();
        return spr;
    }
    
    delete spr;
    return nullptr;
}

EffectSprite* EffectSprite::create(Texture2D *tex) {
    
    auto spr = new (nothrow)EffectSprite();
    
    if( spr && spr->initWithTexture(tex) ) {
        spr->autorelease();
        return spr;
    }
    
    delete spr;
    return nullptr;
}

EffectSprite::EffectSprite() :
defaultEffect(nullptr) {
    
    effects.reserve(2);
}

EffectSprite::~EffectSprite() {
    
    for( auto &tuple : effects ) {
        std::get<1>(tuple)->release();
    }
    
    CC_SAFE_RELEASE(defaultEffect);
}

void EffectSprite::setEffect(Effect* effect) {
    
    if( defaultEffect == effect ) {
        return;
    }
    
    CC_SAFE_RELEASE(defaultEffect);
    defaultEffect = effect;
    CC_SAFE_RETAIN(defaultEffect);
    
    if( defaultEffect ) {
        defaultEffect->setTarget(this);
        setGLProgramState(defaultEffect->getGLProgramState());
    } else {
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP));
    }
}

static int tuple_sort(const std::tuple<ssize_t,Effect*,QuadCommand> &tuple1,
                      const std::tuple<ssize_t,Effect*,QuadCommand> &tuple2) {
    return std::get<0>(tuple1) < std::get<0>(tuple2);
}

void EffectSprite::addEffect(Effect *effect, ssize_t order) {
    
    effect->retain();
    effect->setTarget(this);
    
    effects.push_back(std::make_tuple(order,effect,QuadCommand()));
    
    std::sort(std::begin(effects), std::end(effects), tuple_sort);
}

void EffectSprite::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    
#if CC_USE_CULLING
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
    
    if(_insideBounds)
#endif
    {
        // negative effects: order < 0
        int idx=0;
        for(auto &effect : effects) {
            
            if(std::get<0>(effect) >=0)
                break;
            auto glProgramState = std::get<1>(effect)->getGLProgramState();
            if (glProgramState)
            {
                QuadCommand &q = std::get<2>(effect);
                q.init(_globalZOrder, _texture->getName(), glProgramState, _blendFunc, &_quad, 1, transform, flags);
                renderer->addCommand(&q);
            }
            idx++;
        }
        
        // normal effect: order == 0
        _trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
        renderer->addCommand(&_trianglesCommand);
        
        // positive effects: order >= 0
        for(auto it = std::begin(effects)+idx; it != std::end(effects); ++it) {
            QuadCommand &q = std::get<2>(*it);
            q.init(_globalZOrder, _texture->getName(), std::get<1>(*it)->getGLProgramState(), _blendFunc, &_quad, 1, transform, flags);
            renderer->addCommand(&q);
            idx++;
        }
    }
}

NS_SB_END
