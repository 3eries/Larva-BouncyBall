//
//  FirebaseAnalytics.hpp
//
//  Created by seongmin hwang on 19/06/2019.
//

#ifndef FirebaseAnalytics_hpp
#define FirebaseAnalytics_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"
#include "../SBPluginConfig.h"

#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
#include "../analytics/SBAnalytics.hpp"

NS_SB_BEGIN;

class FirebaseAnalytics {
public:
    static void setCurrentScreen(const std::string &screen,
                                 const std::string &screenClass = "");
    
    typedef SBAnalytics::EventParams EventParams;
    static void logEvent(const std::string &event, const EventParams &params = EventParams());
};

NS_SB_END;

#endif // SB_PLUGIN_USE_FIREBASE_ANALYTICS

#endif /* FirebaseAnalytics_hpp */
