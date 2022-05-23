//
//  SBPlatformHelper-ios.m
//
//  Created by seongmin hwang
//

#include "SBPlatformHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>

USING_NS_CC;
using namespace std;

bool SBPlatformHelper::isNetworkOnline() {

    struct sockaddr_in zeroAddress;
    bzero(&zeroAddress, sizeof(zeroAddress));
    zeroAddress.sin_len = sizeof(zeroAddress);
    zeroAddress.sin_family = AF_INET;
       
    SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(NULL, (struct sockaddr *)&zeroAddress);
    SCNetworkReachabilityFlags flags;
   
    BOOL didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
    CFRelease(defaultRouteReachability);
   
    if( !didRetrieveFlags ) {
        CCLOG("SBPlatformHelper::isNetworkOnline error.");
    }
   
    BOOL isReachable = flags & kSCNetworkFlagsReachable;
    BOOL needsConnection = flags & kSCNetworkFlagsConnectionRequired;
    
    if( isReachable && !needsConnection ) {
        return true;
    }
    
    return false;
}

#endif // CC_PLATFORM_IOS
