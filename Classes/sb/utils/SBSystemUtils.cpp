//
//  SBSystemUtils.cpp
//  SBLib
//
//  Created by hwangseongmin on 2015. 6. 5..
//
//

#include "SBSystemUtils.h"

USING_NS_CC;
using namespace std;

/**
 * 앱 종료
 */
void SBSystemUtils::exitApp() {
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

string SBSystemUtils::timeToString(const struct tm *tm, const string &format) {
    
    const int LEN = 80;
    char buffer[LEN];
    
    strftime(buffer, LEN, format.c_str(), tm);
    
    return string(buffer);
}

string SBSystemUtils::timeToString(time_t time, const string &format) {
    
    return timeToString(localtime(&time), format);
}

/**
 * 현재 시스템 시간 정보를 반환한다.
 */
struct tm* SBSystemUtils::getCurrentTime() {
    
    time_t rawTime;
    time(&rawTime);
    
    return localtime(&rawTime);
}

/**
 * 현재 시스템 시간을 1/1000초 단위로 반환한다.
 */
double SBSystemUtils::getCurrentTimeMillis() {
    
    struct timeval now;
    
    if( gettimeofday(&now, nullptr) != 0 ) {
        CCLOG("SBSystemUtils error in gettimeofday");
        return 0;
    }
    
    return (now.tv_sec + (double)(now.tv_usec / 1000000.0f)) * 1000.0f;
}

/**
 * 현재 시스템 시간을 초 단위로 반환한다.
 */
double SBSystemUtils::getCurrentTimeSeconds() {
    
    return getCurrentTimeMillis() / 1000.0f;
}

string SBSystemUtils::getCurrentTimeString(const string &format) {
    
    return timeToString(getCurrentTime(), format);
}
