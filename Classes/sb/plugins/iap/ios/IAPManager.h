//
//  IAPManager.h
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#ifndef IAPManager_h
#define IAPManager_h

#if SB_PLUGIN_USE_IAP

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <StoreKit/StoreKit.h>

#include "cocos2d.h"
#include "IAPHelper.hpp"

@interface IAPManager : NSObject<SKProductsRequestDelegate, SKPaymentTransactionObserver> {
}

+ (IAPManager*) getInstance;

- (void) initIAP:(const superbomb::iap::Config&)config;
- (void) purchase:(const std::string&)itemId;
- (void) restore;

#pragma mark- SKProductsRequestDelegate
- (void) productsRequest:(SKProductsRequest *)request
      didReceiveResponse:(SKProductsResponse *)response;

#pragma mark- SKPaymentTransactionObserver
- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;
- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue;

@end

#endif // SB_PLUGIN_USE_IAP
#endif /* IAPManager_h */
