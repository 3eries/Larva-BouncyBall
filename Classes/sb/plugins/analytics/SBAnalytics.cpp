//
//  SBAnalytics.cpp
//
//  Created by seongmin hwang on 20/06/2019.
//

#include "SBAnalytics.hpp"

#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
#include "../firebase/FirebaseAnalytics.hpp"
#endif

USING_NS_SB;
using namespace std;

const string SBAnalytics::EVENT_DEBUG                      = "series_debug";
const string SBAnalytics::EVENT_ERROR                      = "series_error";
const string SBAnalytics::EVENT_PARAM_MESSAGE              = "message";

void SBAnalytics::init() {
}

void SBAnalytics::setCurrentScreen(const string &screen, const string &screenClass) {
    
#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
    
#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
    FirebaseAnalytics::setCurrentScreen(screen, screenClass);
#endif // SB_PLUGIN_USE_FIREBASE_ANALYTICS
    
#endif // COCOS2D_DEBUG == 0
}

void SBAnalytics::logEvent(const string &event, const EventParams &params) {

#if !defined(COCOS2D_DEBUG) || COCOS2D_DEBUG == 0
    
#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
    FirebaseAnalytics::logEvent(event, params);
#endif // SB_PLUGIN_USE_FIREBASE_ANALYTICS
    
#endif // COCOS2D_DEBUG == 0
}

void SBAnalytics::logDebug(const string &msg) {
    
    EventParams params;
    params[EVENT_PARAM_MESSAGE] = msg;
    
#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
    FirebaseAnalytics::logEvent(EVENT_DEBUG, params);
#endif // SB_PLUGIN_USE_FIREBASE_ANALYTICS
}

void SBAnalytics::logError(const string &msg) {
    
    EventParams params;
    params[EVENT_PARAM_MESSAGE] = msg;
    
#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
    FirebaseAnalytics::logEvent(EVENT_ERROR, params);
#endif // SB_PLUGIN_USE_FIREBASE_ANALYTICS
}

string SBAnalytics::getNumberRange(int n, int begin, int end, int range) {
    
    while( true ) {
        if( n >= begin && n <= end ) {
            break;
        }
        
        begin = (end + 1);
        end += range;
    }
    
    return STR_FORMAT("%d-%d", begin, end);
}
