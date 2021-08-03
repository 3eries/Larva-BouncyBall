//
//  SBScheduleHelper.cpp
//
//  Created by seongmin hwang on 09/11/2018.
//

#include "SBScheduleHelper.hpp"

USING_NS_CC;
using namespace std;

Scheduler* SBScheduleHelper::getScheduler() {
    
    return Director::getInstance()->getScheduler();
}

void SBScheduleHelper::schedule(const OnUpdateListener &callback, void *target,
                                float interval, const string &key) {
    
    getScheduler()->schedule(callback, target, interval, false, key);
}

void SBScheduleHelper::schedule(const OnUpdateListener &callback, void *target, const string &key) {
    
    schedule(callback, target, 0, key);
}

void SBScheduleHelper::schedule(const OnUpdateListener &callback, void *target,
                                float interval, unsigned int repeat, float delay, const string &key) {
    
    getScheduler()->schedule(callback, target, interval, repeat, delay, false, key);
}

void SBScheduleHelper::scheduleOnce(const OnUpdateListener &callback, void *target,
                                    float delay, const string &key) {
    
    schedule(callback, target, 0, 0, delay, key);
}

void SBScheduleHelper::unschedule(void *target) {
    
    getScheduler()->unscheduleAllForTarget(target);
}

void SBScheduleHelper::unschedule(void *target, const string &key) {
    
    getScheduler()->unschedule(key, target);
}

void SBScheduleHelper::pause(void *target) {
    
    getScheduler()->pauseTarget(target);
}

void SBScheduleHelper::resume(void *target) {
    
    getScheduler()->resumeTarget(target);
}


