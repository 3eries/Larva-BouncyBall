//
//  SBScheduleHelper.hpp
//
//  Created by seongmin hwang on 09/11/2018.
//

#ifndef SBScheduleHelper_hpp
#define SBScheduleHelper_hpp

#include <stdio.h>

#include "cocos2d.h"

class SBScheduleHelper {
public:
    typedef std::function<void(float)> OnUpdateListener;
    
    static cocos2d::Scheduler* getScheduler();
    
public:
    static void schedule(const OnUpdateListener &callback, void *target,
                         float interval, const std::string &key);
    static void schedule(const OnUpdateListener &callback, void *target,
                         const std::string &key);
    static void schedule(const OnUpdateListener &callback, void *target,
                         float interval, unsigned int repeat, float delay,
                         const std::string &key);
    static void scheduleOnce(const OnUpdateListener &callback, void *target,
                             float delay, const std::string &key);
    
    static void unschedule(void *target);
    static void unschedule(void *target, const std::string &key);
    
    static void pause(void *target);
    static void resume(void *target);
};

#endif /* SBScheduleHelper_hpp */
