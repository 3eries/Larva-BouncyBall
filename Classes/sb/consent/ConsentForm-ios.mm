//
//  ConsentForm-ios.m
//
//  Created by seongmin hwang on 2018. 9. 26..
//

#include "ConsentForm.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import <UIKit/UIAlert.h>

#if SB_PLUGIN_USE_IAP
#include <UserMessagingPlatform/UserMessagingPlatform.h>
#endif // SB_PLUGIN_USE_IAP

#import "AppController.h"
#import "RootViewController.h"

#include "../platform/SBPlatformMacros.h"
#include "ConsentManager.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

void ConsentForm::show() {
    
#if SB_PLUGIN_USE_IAP
//    [UMPConsentInformation.sharedInstance reset];
//    return;
    
    // Create a UMPRequestParameters object.
    UMPRequestParameters *parameters = [[UMPRequestParameters alloc] init];
    // Set tag for under age of consent. Here NO means users are not under age.
    parameters.tagForUnderAgeOfConsent = NO;

    // Force a geography for GDPR
    /*
    UMPDebugSettings *debugSettings = [[UMPDebugSettings alloc] init];
    debugSettings.testDeviceIdentifiers = @[ @"6A2A3F2D-4E16-4886-9F92-491567BAB9D8" ];
    debugSettings.geography = UMPDebugGeographyEEA;
    parameters.debugSettings = debugSettings;
    */
    
    // Request an update to the consent information.
    [UMPConsentInformation.sharedInstance
     requestConsentInfoUpdateWithParameters:parameters
     completionHandler:^(NSError *_Nullable error) {
        
        if( error ) {
            // Handle the error.
        } else {
            // The consent information state was updated.
            // You are now ready to check if a form is
            // available.
            UMPFormStatus formStatus = UMPConsentInformation.sharedInstance.formStatus;
            
            // load form
            if( formStatus == UMPFormStatusAvailable ) {
                [UMPConsentForm loadWithCompletionHandler:^(UMPConsentForm *form, NSError *loadError) {
                    if (loadError) {
                        // Handle the error.
                    } else {
                        // Present the form. You can also hold on to the reference to present
                        // later.
                        if (UMPConsentInformation.sharedInstance.consentStatus == UMPConsentStatusRequired) {
                            [form presentFromViewController:ROOT_VIEW_CONTROLLER
                                          completionHandler:^(NSError *_Nullable dismissError) {
                                
                                if (UMPConsentInformation.sharedInstance.consentStatus == UMPConsentStatusObtained) {
                                      // App can start requesting ads.
                                    ConsentManager::setPrivacyPolicyChecked(true);
                                }
                            }];
                        } else {
                            // Keep the form available for changes to user consent.
                        }
                    }
                }];
            }
        }
    }];
#endif // SB_PLUGIN_USE_IAP
}

NS_SB_END;

#endif // CC_PLATFORM_IOS
