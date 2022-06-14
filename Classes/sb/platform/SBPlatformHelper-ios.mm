//
//  SBPlatformHelper-ios.m
//
//  Created by seongmin hwang
//

#include "SBPlatformHelper.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <SystemConfiguration/SystemConfiguration.h>
#import <netinet/in.h>
#import "AppController.h"
#import "RootViewController.h"

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

void SBPlatformHelper::showNetworkErrorPopup() {

    UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"No internet Connetion"
                                                                   message:@"Please play to online"
                                                            preferredStyle:UIAlertControllerStyleAlert];
    [alert addAction:[UIAlertAction actionWithTitle:@"OK"
                                              style:UIAlertActionStyleDefault
                                            handler:^(UIAlertAction *action) {
        SBSystemUtils::exitApp();
    }]];

    [ROOT_VIEW_CONTROLLER presentViewController:alert animated:YES completion:^{}];
}

#endif // CC_PLATFORM_IOS
