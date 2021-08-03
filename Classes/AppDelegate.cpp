#include "AppDelegate.h"

#include "superbomb.h"

#include "Define.h"
#include "PatchManager.hpp"
#include "GameConfiguration.hpp"
#include "User.hpp"
#include "SceneManager.h"
#include "ContentManager.hpp"

#include "game/GameManager.hpp"

USING_NS_CC;
using namespace std;

static cocos2d::Size designResolutionSize       = cocos2d::Size(720, 1280);
static cocos2d::Size designResolutionSizeIPad   = cocos2d::Size(960, 1280);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
    GameManager::destroyInstance();
    ContentManager::destroyInstance();
    GameConfiguration::destroyInstance();
    User::destroyInstance();
    PatchManager::destroyInstance();
    SceneManager::destroyInstance();
    SBDirector::end();
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    
    if(!glview) {
        glview = GLViewImpl::create("LarvaBouncyBall");
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    if( SBDirector::isPadResolution() ) {
        glview->setDesignResolutionSize(designResolutionSizeIPad.width, designResolutionSizeIPad.height,
                                        ResolutionPolicy::EXACT_FIT);
    } else {
        glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height,
                                        ResolutionPolicy::FIXED_WIDTH);
    }

    register_all_packages();
    
    // 라이브러리 초기화
    SBDirector::init();
    {
        string json = SBStringUtils::readTextFile(PLUGIN_CONFIG_FILE);
#if ENCRYPT_PLUGIN_CONFIG_FILE
        json = SBSecurity::decryptAES256(json, AES256_KEY);
#endif
        superbomb::PluginManager::getInstance()->init(json);
    }
    superbomb::ReviewHelper::getInstance()->setFirstLockedDay(1);
    superbomb::ReviewHelper::getInstance()->setLockedDay(1);
    
    // 게임 모듈 초기화
    PatchManager::getInstance()->init();
    
    // run
    SceneManager::getInstance()->replace(SceneType::SPLASH);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    
    CCLOG("AppDelegate::applicationDidEnterBackground valid: %d (1이 정상)", Director::getInstance()->isValid());
    
    if( !Director::getInstance()->isValid() ) {
        return;
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(superbomb::EVENT_ENTER_BACKGROUND);
    Director::getInstance()->stopAnimation();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {

    CCLOG("AppDelegate::applicationWillEnterForeground valid: %d (0이 정상)", Director::getInstance()->isValid());
    
    if( Director::getInstance()->isValid() ) {
        return;
    }
    
#if SB_PLUGIN_USE_ADS
    auto adsHelper = superbomb::AdsHelper::getInstance();

    if( adsHelper->getInterstitial()->isOpened() ||
        adsHelper->getRewardedVideo()->isOpened() ) {
        CCLOG("AppDelegate::applicationWillEnterForeground 광고 오픈됨");
        return;
    }
#endif
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(superbomb::EVENT_ENTER_FOREGROUND);
    Director::getInstance()->startAnimation();
}
