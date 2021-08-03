//
//  SBMacros.h
//
//  Created by hwangseongmin on 2016. 11. 24..
//
//

#ifndef SBMacros_h
#define SBMacros_h

#include "cocos2d.h"

#ifdef __cplusplus
#define NS_SB_BEGIN                     namespace superbomb {
#define NS_SB_END                       }
#define USING_NS_SB                     using namespace superbomb
#define NS_SB                           ::superbomb
#else
#define NS_SB_BEGIN
#define NS_SB_END
#define USING_NS_SB
#define NS_SB
#endif

#define SB_FORCE_LOG(format, ...)               cocos2d::log(format, ##__VA_ARGS__)

#define DIR_ADD(__base__, __dir__)              std::string(__base__).append(__dir__).append("/")
#define HASH_STR(__str__)                       SBStringUtils::hashStr(__str__)
#define STR_FORMAT(__format__, ...)             cocos2d::StringUtils::format(__format__, __VA_ARGS__)
#define TO_STRING(__var__)                      SBStringUtils::toString(__var__)
#define TO_INTEGER(__var__)                     SBStringUtils::toNumber<int>(__var__)

#define SB_WIN_SIZE                             cocos2d::Director::getInstance()->getWinSize()
#define SB_RUNNING_SCENE                        cocos2d::Director::getInstance()->getRunningScene()
#define SB_BOUNDING_BOX_IN_WORLD(__node__)      SBNodeUtils::getBoundingBoxInWorld(__node__)

#define SB_REF_CREATE_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
    __TYPE__ *pRet = new(std::nothrow) __TYPE__(); \
    if (pRet ) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = nullptr; \
    return nullptr; \
    } \
}

#define SB_SYNTHESIZE_BUILDER(builder, varType, varName, funName)\
protected: varType varName;\
public: virtual inline varType get##funName(void) const { return varName; }\
virtual inline builder set##funName(varType var){ varName = var; return this; }

#define SB_SYNTHESIZE_READONLY_BOOL(varName, funName)\
protected: bool varName;\
public: virtual bool is##funName(void) const { return varName; }

#define SB_SYNTHESIZE_BOOL(varName, funName)\
protected: bool varName;\
public: virtual bool is##funName(void) const { return varName; }\
public: virtual void set##funName(bool var){ varName = var; }

#define SB_SAFE_PERFORM_LISTENER(__NODE__, __LISTENER__) \
if( __NODE__ && __LISTENER__ ) { \
__NODE__->retain(); \
__LISTENER__(); \
__NODE__->release(); \
}

#define SB_SAFE_PERFORM_LISTENER_N(__NODE__, __LISTENER__) \
if( __NODE__ && __LISTENER__ ) { \
__NODE__->retain(); \
__LISTENER__(__NODE__); \
__NODE__->release(); \
}

#define SB_SAFE_REMOVE(n)                   do { if(n) { (n)->removeFromParent(); } } while(0)
#define SB_SAFE_REMOVE_NULL(n)              do { if(n) { (n)->removeFromParent(); (n) = nullptr; } } while(0)

#define SB_SAFE_SHOW(n)                     do { if(n) { (n)->setVisible(true); } } while(0)
#define SB_SAFE_HIDE(n)                     do { if(n) { (n)->setVisible(false); } } while(0)

// 좌표
static inline cocos2d::Vec2 Vec2TL(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(0, size.height) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2TC(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(size.width/2, size.height) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2TR(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(size.width, size.height) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2ML(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(0, size.height/2) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2MC(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(size.width/2, size.height/2) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2MR(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(size.width, size.height/2) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2BL(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2BC(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(size.width/2, 0) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2BR(cocos2d::Size size, float x, float y) {
    return cocos2d::Vec2(size.width, 0) + cocos2d::Vec2(x, y);
}

static inline cocos2d::Vec2 Vec2TL(float x, float y) { return Vec2TL(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2TC(float x, float y) { return Vec2TC(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2TR(float x, float y) { return Vec2TR(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2ML(float x, float y) { return Vec2ML(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2MC(float x, float y) { return Vec2MC(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2MR(float x, float y) { return Vec2MR(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2BL(float x, float y) { return Vec2BL(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2BC(float x, float y) { return Vec2BC(SB_WIN_SIZE, x, y); }
static inline cocos2d::Vec2 Vec2BR(float x, float y) { return Vec2BR(SB_WIN_SIZE, x, y); }

#define ANCHOR_M    cocos2d::Vec2(0.5f, 0.5f)
#define ANCHOR_BL   cocos2d::Vec2(0.0f, 0.0f)
#define ANCHOR_TL   cocos2d::Vec2(0.0f, 1.0f)
#define ANCHOR_BR   cocos2d::Vec2(1.0f, 0.0f)
#define ANCHOR_TR   cocos2d::Vec2(1.0f, 1.0f)
#define ANCHOR_MR   cocos2d::Vec2(1.0f, 0.5f)
#define ANCHOR_ML   cocos2d::Vec2(0.0f, 0.5f)
#define ANCHOR_MT   cocos2d::Vec2(0.5f, 1.0f)
#define ANCHOR_MB   cocos2d::Vec2(0.5f, 0.0f)

NS_SB_BEGIN

static inline bool isMultiTouch(cocos2d::Touch *touch) {
    return touch->getID() != 0;
};

static inline float convertDistanceFromPointToInch(const cocos2d::Vec2 &dis) {
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    int dpi = cocos2d::Device::getDPI();
    float distance = cocos2d::Vec2(dis.x * glview->getScaleX() / dpi, dis.y * glview->getScaleY() / dpi).getLength();
    return distance;
}

NS_SB_END

#endif /* SBMacros_h */
