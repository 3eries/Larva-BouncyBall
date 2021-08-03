//
//  FirebaseAnalytics-ios.mm
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "FirebaseAnalytics.hpp"

#if SB_PLUGIN_USE_FIREBASE_ANALYTICS
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <Firebase/Firebase.h>

#include "../../platform/SBPlatformMacros.h"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;
    
void FirebaseAnalytics::setCurrentScreen(const string &screen, const string &screenClass) {
    
    if( screenClass != "" ) {
        [FIRAnalytics logEventWithName:kFIREventScreenView
                            parameters:@{kFIRParameterScreenClass: NS_STRING(screenClass.c_str()),
                                         kFIRParameterScreenName: NS_STRING(screen.c_str())}];
    } else {
        [FIRAnalytics logEventWithName:kFIREventScreenView
                            parameters:@{kFIRParameterScreenName: NS_STRING(screen.c_str())}];
    }
}

void FirebaseAnalytics::logEvent(const string &event, const EventParams &params) {
    
    NSMutableDictionary *nsParams = [NSMutableDictionary dictionary];
    
    for( auto it = params.begin(); it != params.end(); ++it ) {
        NSString *nsKey = NS_STRING(it->first.c_str());
        NSObject *nsValue = nil;
        
        Value value = it->second;
        
        switch( value.getType() ) {
            case Value::Type::INTEGER:      nsValue = [NSNumber numberWithInt:value.asInt()];                   break;
            case Value::Type::UNSIGNED:     nsValue = [NSNumber numberWithUnsignedInt:value.asUnsignedInt()];   break;
            case Value::Type::FLOAT:
            case Value::Type::DOUBLE:       nsValue = [NSNumber numberWithDouble:value.asDouble()];             break;
            case Value::Type::STRING:       nsValue = NS_STRING(value.asString().c_str());                      break;
            default:
                CCASSERT(false, "Analytics::logEvent invalid event parameter type.");
                break;
        }
        
        if( nsKey && nsValue ) {
            [nsParams setValue:nsValue forKey:nsKey];
        }
    }
    
    [FIRAnalytics logEventWithName:NS_STRING(event.c_str())
                        parameters:nsParams];
}

NS_SB_END;

#endif // CC_PLATFORM_IOS
#endif // SB_PLUGIN_USE_FIREBASE_ANALYTICS
