//
//  SBAction.cpp
//
//  Created by seongmin hwang on 2018. 7. 4..
//

#include "SBAction.hpp"

#include "../base/SBMacros.h"
#include "../utils/SBStringUtils.h"

USING_NS_CC;
using namespace std;

#pragma mark- SBRollingNumber

const string SBRollingNumber::SCHEDULER_ROLLING      = "SB_SCHEDULER_ROLLING";
const string SBRollingNumber::SCHEDULER_ROLLING_STOP = "SB_SCHEDULER_ROLLING_STOP";

SBRollingNumber* SBRollingNumber::create(OnRollingListener onRollingListener, int number,
                                         float timePerDigit, float rollSpeed) {
    
    auto action = new SBRollingNumber();
    
    if( action && action->init(onRollingListener, number, timePerDigit, rollSpeed) ) {
        action->autorelease();
        return action;
    }
    
    delete action;
    return nullptr;
}

SBRollingNumber::SBRollingNumber() {
    
}

SBRollingNumber::~SBRollingNumber() {
    
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

bool SBRollingNumber::init(OnRollingListener onRollingListener,
                           int number, float timePerDigit, float rollSpeed) {
    
    auto chars = SBStringUtils::toArray(TO_STRING(number));
    float duration = chars.size() * timePerDigit;
    
    if( !ActionInterval::initWithDuration(duration) ) {
        return false;
    }
    
    CCASSERT(onRollingListener != nullptr, "SBRollingNumber::init error: invalid onRollingListener");
    
    this->onRollingListener = onRollingListener;
    this->number = number;
    this->timePerDigit = timePerDigit;
    this->rollSpeed = rollSpeed;
    this->rollIndex = 0;
    
    // 숫자 목록 초기화
    for( int i = (int)chars.size()-1; i >= 0; --i ) {
        numberList.push_back(chars[i]);
    }
    
    rollNumberList = numberList;
    
    return true;
}

void SBRollingNumber::startWithTarget(Node *target) {
    
    ActionInterval::startWithTarget(target);
    
    // 롤링
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) {
        
        this->onRolling();
        
    }, this, rollSpeed, false, SCHEDULER_ROLLING);
    
    // 롤링중인 숫자 차례로 정지
    scheduler->schedule([=](float dt) {
        
        rollNumberList[rollIndex] = numberList[rollIndex];
        // CCLOG("stop : %d", rollIndex);
        ++rollIndex;
        
        performListener();
        
        // 롤링 정지
        if( this->isRollingFinished() ) {
            this->onRollingFinished();
        }
        
    }, this, timePerDigit, false, SCHEDULER_ROLLING_STOP);
}

void SBRollingNumber::stop() {
    
    onRollingFinished();
    
    ActionInterval::stop();
}

void SBRollingNumber::update(float t) {
    
}

/**
 * onRolling
 */
void SBRollingNumber::onRolling() {

    if( isRollingFinished() ) {
        return;
    }
    
    // 롤링 숫자 생성
    for( int i = rollIndex; i < numberList.size(); ++i ) {
        int ran = arc4random() % 10;
        rollNumberList[i] = TO_STRING(ran);
    }
    
    performListener();
}

/**
 * onRollingFinished
 */
void SBRollingNumber::onRollingFinished() {

    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);

    rollNumberList = numberList;
    performListener();
}

void SBRollingNumber::performListener() {
    
    string str = "";
    
    for( int i = 0; i < rollNumberList.size(); ++i ) {
        str = rollNumberList[i] + str;
    }
    
//    int i = SBStringUtils::toNumber<int>(str);
    onRollingListener(str);
}

bool SBRollingNumber::isRollingFinished() {
    
    return rollIndex > numberList.size()-1;
}

#pragma mark- SBIncreaseNumber

const string SBIncreaseNumber::SCHEDULER_INCREASE      = "SB_SCHEDULER_INCREASE";

#define INCREASE_SCHEDULE_INTERVAL 0.04f

SBIncreaseNumber* SBIncreaseNumber::create(OnIncreaseListener onIncreaseListener, float duration,
                                           int startNumber, int targetNumber) {
    
    auto action = new SBIncreaseNumber();
    
    if( action && action->init(onIncreaseListener, duration, startNumber, targetNumber) ) {
        action->autorelease();
        return action;
    }
    
    delete action;
    return nullptr;
}

SBIncreaseNumber::SBIncreaseNumber() {
    
}

SBIncreaseNumber::~SBIncreaseNumber() {
    
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

bool SBIncreaseNumber::init(OnIncreaseListener onIncreaseListener, float duration,
                            int startNumber, int targetNumber) {
    
    if( !ActionInterval::initWithDuration(duration) ) {
        return false;
    }
    
    CCASSERT(onIncreaseListener != nullptr, "SBIncreaseNumber::init error: invalid onIncreaseListener");
    
    this->onIncreaseListener = onIncreaseListener;
    this->startNumber = startNumber;
    this->targetNumber = targetNumber;
    this->number = startNumber;
    this->increasePerStep = 0;
    
    return true;
}

// target : 200
// start  : 0
// diff   : 200
// duration : 1.0
// increase per step : 1
// interval : duration / diff
//// fps 미만 :
// 예시)
// fps : 0.04
// (1 / 200) = 0.005
// (1 / 200) = 0.005
// diff / (duration / fps)
// 200 / (1 / 0.04)
void SBIncreaseNumber::startWithTarget(Node *target) {
    
    ActionInterval::startWithTarget(target);
    
    const int diff = targetNumber - startNumber;
    
    float interval = getDuration() / diff;
    interval = MAX(INCREASE_SCHEDULE_INTERVAL, interval);
    
    const int totalStep = (int)(getDuration() / interval);
    increasePerStep = diff / totalStep;
    
    auto scheduler = Director::getInstance()->getScheduler();
    scheduler->schedule([=](float dt) {
        
        this->onIncrease();
        
        // 완료 체크
        if( this->isIncreaseFinished() ) {
            this->onIncreaseFinished();
        }
        
    }, this, interval, false, SCHEDULER_INCREASE);
}

void SBIncreaseNumber::stop() {
    
    onIncreaseFinished();
    
    ActionInterval::stop();
}

void SBIncreaseNumber::update(float t) {
    
}

/**
 * onIncrease
 */
void SBIncreaseNumber::onIncrease() {
    
    if( isIncreaseFinished() ) {
        return;
    }
    
    // 숫자 증가
    // sd
    
    onIncreaseListener(TO_STRING(number));
}

/**
 * onIncreaseFinished
 */
void SBIncreaseNumber::onIncreaseFinished() {
    
    Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
    
    onIncreaseListener(TO_STRING(targetNumber));
}

bool SBIncreaseNumber::isIncreaseFinished() {
    
    return number == targetNumber;
}

