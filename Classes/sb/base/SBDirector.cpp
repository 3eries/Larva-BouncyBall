//
//  SBDirector.cpp
//
//  Created by seongmin hwang on 2018. 5. 17..
//

#include "SBDirector.hpp"

#include "SBUserDefaultKey.h"

#include "../utils/SBSystemUtils.h"
#include "../utils/SBStringUtils.h"
#include "../audio/SBAudioEngine.hpp"

#include "event/SBEvent.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

static SBDirector *instance = nullptr;
SBDirector* SBDirector::getInstance() {
    
    if( !instance ) {
        instance = new SBDirector();
    }
    
    return instance;
}

void SBDirector::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

SBDirector::SBDirector() :
todayFirstRun(false),
enterBackgroundTime(0),
backgroundStayTime(0),
screenTouchLocked(false) {
}

SBDirector::~SBDirector() {
    
    for( auto listener : eventListeners ) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(listener);
    }
    
    eventListeners.clear();
}

void SBDirector::init() {
    
    getInstance()->initRunCount();
    
    // log
    {
        string log;
        log += "\t" + STR_FORMAT("run count: %d", getRunCount()) + "\n";
        log += "\t" + STR_FORMAT("first run time: %s", SBSystemUtils::timeToString(getFirstRunTime(),"%Y-%m-%d %l:%M:%S").c_str()) + "\n";
        log += "\t" + STR_FORMAT("last run time: %s", SBSystemUtils::timeToString(getLastRunTime(), "%Y-%m-%d %l:%M:%S").c_str()) + "\n";
        log += "\t" + STR_FORMAT("today first run: %d", getInstance()->todayFirstRun) + "\n";
        
        CCLOG("SBDirector::init\n{\n%s}", log.c_str());
    }
    
    // 라이브러리 초기화
    SBAudioEngine::getInstance()->init();
    
    // 이벤트 리스너 초기화
    getInstance()->initCustomListener();
    getInstance()->initTouchListener();
}

void SBDirector::end() {

    SBAudioEngine::getInstance()->destroyInstance();
    
    destroyInstance();
}

void SBDirector::initRunCount() {
    
    // 실행 횟수 설정
    auto userDefault = UserDefault::getInstance();
    
    const int runCount = getRunCount() + 1;
    userDefault->setIntegerForKey(SBUserDefaultKey::RUN_COUNT, runCount);
    
    // 실행 시간 설정
    // 시간은 숫자지만, 자료형 범위를 벗어날 수 있어 저장은 문자열로 한다.
    const string nowTime = TO_STRING(mktime(SBSystemUtils::getCurrentTime()));
    
    if( runCount == 1 ) {
        // 최초 실행
        userDefault->setStringForKey(SBUserDefaultKey::FIRST_RUN_TIME, nowTime);
    }
    
    userDefault->setStringForKey(SBUserDefaultKey::LAST_RUN_TIME, nowTime);
    
    // 오늘 첫번째 실행인지 설정
    {
        auto getDate = [](time_t time) -> string {
            return SBSystemUtils::timeToString(time, "%Y-%m-%d");
        };
        
        string todayDate = getDate(SBStringUtils::toNumber<time_t>(nowTime));
        string savedDate = getTodayDate();
        
        if( savedDate == "" ) {
            getInstance()->todayFirstRun = true;
        } else {
            getInstance()->todayFirstRun = (todayDate != savedDate);
        }
        
        if( getInstance()->todayFirstRun ) {
            userDefault->setStringForKey(SBUserDefaultKey::TODAY_DATE, todayDate);
        }
    }
    
    userDefault->flush();
}

/**
 * 커스텀 리스너 초기화
 */
void SBDirector::initCustomListener() {
    
    auto eventDispatcher = Director::getInstance()->getEventDispatcher();
    
    string eventNames[] = {
        EVENT_ENTER_BACKGROUND,
        EVENT_ENTER_FOREGROUND,
    };
    
    for( string eventName : eventNames ) {
        /*
        auto listener = eventDispatcher->addCustomEventListener(eventName, [=](EventCustom *event) {
            this->onCustomEvent(HASH_STR(event->getEventName().c_str()));
        });
         */
        auto listener = EventListenerCustom::create(eventName, [=](EventCustom *event) {
            this->onCustomEvent(event->getEventName());
        });
        eventDispatcher->addEventListenerWithFixedPriority(listener, -1);
        
        eventListeners.pushBack(listener);
    }
}

/**
 * 터치 리스너 초기화
 */
void SBDirector::initTouchListener() {
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [=](Touch*, Event*) -> bool {
        return this->screenTouchLocked;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, -1);
    
    eventListeners.pushBack(listener);
}

/**
 * 커스텀 이벤트 처리
 */
void SBDirector::onCustomEvent(const string &eventName) {
    
    // EVENT_ENTER_BACKGROUND
    if( eventName == EVENT_ENTER_BACKGROUND ) {
        // update time
        enterBackgroundTime = SBSystemUtils::getCurrentTimeSeconds();
        backgroundStayTime = 0;
        
        // update engine
        SBAudioEngine::pauseAll();
    }
    // EVENT_ENTER_FOREGROUND
    else if( eventName == EVENT_ENTER_FOREGROUND ) {
        // update time
        backgroundStayTime = SBSystemUtils::getCurrentTimeSeconds() - enterBackgroundTime;
        CCLOG("SBDirector EVENT_ENTER_FOREGROUND background stay time: %f", backgroundStayTime);
        
        // update engine
        SBAudioEngine::resumeAll();
    }
}
    
/**
 * 실행 횟수를 반환합니다
 */
int SBDirector::getRunCount() {
    
    return UserDefault::getInstance()->getIntegerForKey(SBUserDefaultKey::RUN_COUNT, 0);
}

/**
 * 앱 설치 후 첫번째 실행인지를 반환합니다
 */
bool SBDirector::isFirstRun() {
    
    return (getRunCount() == 1);
}

/**
 * 오늘 첫번째 실행인지를 반환합니다
 */
bool SBDirector::isTodayFirstRun() {
    
    return getInstance()->todayFirstRun;
}

/**
 * 앱 최초 실행 시간을 반환합니다
 */
time_t SBDirector::getFirstRunTime() {
    
    string time = UserDefault::getInstance()->getStringForKey(SBUserDefaultKey::FIRST_RUN_TIME, "");
    
    if( time != "" ) {
        return SBStringUtils::toNumber<time_t>(time);
    }
    
    return 0;
}

/**
 * 앱 마지막 실행 시간을 반환합니다
 */
time_t SBDirector::getLastRunTime() {
    
    string time = UserDefault::getInstance()->getStringForKey(SBUserDefaultKey::LAST_RUN_TIME, "");
    
    if( time != "" ) {
        return SBStringUtils::toNumber<time_t>(time);
    }
    
    return 0;
}

/**
 * 저장된 오늘 날짜를 반환합니다
 */
string SBDirector::getTodayDate() {
    
    return UserDefault::getInstance()->getStringForKey(SBUserDefaultKey::TODAY_DATE, "");
}

/**
 * 백그라운드에서 머문 시간을 반환합니다
 * @return 단위: sec
 */
double SBDirector::getBackgroundStayTime() {
    
    return getInstance()->backgroundStayTime;
}

/**
 * handler 노드에 액션을 적용하여 동작을 지연 시킵니다
 */
void SBDirector::postDelayed(Node *handler, SBCallback runnable,
                             float delay, bool isScreenTouchLocked) {
    
    CCASSERT(handler != nullptr, "SBDirector::postDelayed error: handler must be not null");
    CCASSERT(runnable != nullptr, "SBDirector::postDelayed error: runnable must be not null");
    
    if( isScreenTouchLocked ) {
        getInstance()->setScreenTouchLocked(true);
    }
    
    auto delayAction = DelayTime::create(delay);
    auto callFunc = CallFunc::create([=]() {
        
        if( isScreenTouchLocked ) {
            getInstance()->setScreenTouchLocked(false);
        }
        
        runnable();
    });
    handler->runAction(Sequence::create(delayAction, callFunc, nullptr));
}

#define PHONE_SCREEN_SIZE         Size(1280, 720)
#define PAD_SCREEN_SIZE           Size(1280, 960)

#define PHONE_SCREEN_RATIO        (PHONE_SCREEN_SIZE.width / PHONE_SCREEN_SIZE.height)
#define PAD_SCREEN_RATIO          (PAD_SCREEN_SIZE.width / PAD_SCREEN_SIZE.height)

bool SBDirector::isPhoneResolution() {
    
    const float ratio = getScreenRatio();
    return fabsf(ratio - PHONE_SCREEN_RATIO) < fabsf(ratio - PAD_SCREEN_RATIO);
}

bool SBDirector::isPadResolution() {
    
    const float ratio = getScreenRatio();
    return fabsf(ratio - PAD_SCREEN_RATIO) < fabsf(ratio - PHONE_SCREEN_RATIO);
}

bool SBDirector::isIPad() {
    
    return Application::getInstance()->getTargetPlatform() == Application::Platform::OS_IPAD;
}

float SBDirector::getScreenRatio() {
    
    auto frameSize = Director::getInstance()->getOpenGLView()->getFrameSize();
    
    float screenWidth  = MAX(frameSize.width, frameSize.height);
    float screenHeight = MIN(frameSize.width, frameSize.height);
    
    return screenWidth / screenHeight;
}
