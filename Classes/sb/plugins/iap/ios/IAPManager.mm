//
//  IAPManager.m
//
//  Created by seongmin hwang on 2018. 4. 10..
//

#import "IAPManager.h"

#if SB_PLUGIN_USE_IAP

#import "AppController.h"
#import "RootViewController.h"

#include "../../../platform/SBPlatformMacros.h"

USING_NS_CC;
USING_NS_SB;
using namespace std;

@interface IAPManager (Private)

- (id) init;

- (void) onTransactionStatePurchased:(SKPaymentTransaction *)transaction;
- (void) onTransactionStateFailed:(SKPaymentTransaction *)transaction;
- (void) onTransactionStateRestored:(SKPaymentTransaction *)transaction;
- (void) finishTransaction:(SKPaymentTransaction *)transaction;

+ (string) getItemId:(SKPaymentTransaction *)transaction;

@end

@implementation IAPManager

+ (IAPManager*) getInstance {
    
    static IAPManager *instance = nil;
    static dispatch_once_t oncePredicate;
    
    dispatch_once(&oncePredicate, ^{
        instance = [[IAPManager alloc] init];
    });
    
    return instance;
}

- (id) init {
    
    if( self = [super init] ) {
    }
    
    return self;
}

/**
 * IAP 모듈 초기화
 */
- (void) initIAP:(const iap::Config&)config {
    
#if SB_PLUGIN_USE_IAP
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
#endif
}

/**
 * 구매 요청
 */
- (void) purchase:(const string&)itemId {
    
#if SB_PLUGIN_USE_IAP
    // 아이템 조회
    SKProductsRequest *productRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:
                                         [NSSet setWithObject:NS_STRING(itemId.c_str())]];
    productRequest.delegate = self;
    [productRequest start];
#endif
}

/**
 * 복원 요청
 */
- (void) restore {

#if SB_PLUGIN_USE_IAP
    [[SKPaymentQueue defaultQueue] restoreCompletedTransactions];
#endif
}

#pragma mark- SKProductsRequestDelegate

/**
 * 아이템 조회 완료
 */
- (void) productsRequest:(SKProductsRequest *)request
      didReceiveResponse:(SKProductsResponse *)response {
    
    // 유효한 아이템인지 체크
    if( [response.products count] == 0 ) {
        iap::IAPHelper::getInstance()->onPurchaseError("invalid item");
        return;
    }
    /*
     if( [response.invalidProductIdentifiers count] > 0 ) {
     // invaild products.
     NSString *invalidString = [response.invalidProductIdentifiers objectAtIndex:0];
     return;
     }
     */
    
    // 구매 요청
    SKProduct *product = [response.products objectAtIndex:0];
    NSLog(@"Title: %@", product.localizedTitle);
    NSLog(@"Description: %@", product.localizedDescription);
    NSLog(@"Price: %@", product.price);
    
    SKPayment *payment = [SKPayment paymentWithProduct:product];
    [[SKPaymentQueue defaultQueue] addPayment:payment];
}

/**
 *  구매 진행 상태
 */
- (void) paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    
    NSLog(@"paymentQueue updatedTransactions start");
    
    for( SKPaymentTransaction *transaction in transactions ) {
        NSLog(@"transactionState: %ld", transaction.transactionState);
        
        switch( transaction.transactionState ) {
            // 구매 완료
            case SKPaymentTransactionStatePurchased: {
                [self onTransactionStatePurchased:transaction];
            } break;
                
            // 구매 실패
            case SKPaymentTransactionStateFailed: {
                [self onTransactionStateFailed:transaction];
            } break;
                
            // 복원 완료
            case SKPaymentTransactionStateRestored: {
                [self onTransactionStateRestored:transaction];
            } break;
                
            // 구매중
            case SKPaymentTransactionStatePurchasing: {
                NSLog(@"SKPaymentTransactionStatePurchasing...");
            } break;
                
            case SKPaymentTransactionStateDeferred:
                NSLog(@"SKPaymentTransactionStateDeferred");
                break;
                
            default:
                break;
        }
    }
    
    NSLog(@"paymentQueue updatedTransactions end");
}

/**
 *  구매 성공 처리
 */
- (void) onTransactionStatePurchased:(SKPaymentTransaction *)transaction {
    
    NSLog(@"onTransactionStatePurchased");

    iap::IAPHelper::getInstance()->onPurchased([IAPManager getItemId:transaction]);
    
    [self finishTransaction:transaction];
}

/**
 *  구매/복구 실패 처리
 */
- (void) onTransactionStateFailed:(SKPaymentTransaction *)transaction {
    
    NSLog(@"onTransactionStateFailed: %@", transaction.error);

    NSString *errorMsg = [transaction.error.userInfo objectForKey:NSLocalizedDescriptionKey];
    
    if( transaction.error.code == SKErrorPaymentCancelled ) {
        iap::IAPHelper::getInstance()->onPurchaseCanceled();
    } else {
        iap::IAPHelper::getInstance()->onPurchaseError([errorMsg UTF8String]);
    }
    
    [self finishTransaction:transaction];
}

/**
 *  복구 완료 처리(?)
 */
- (void) onTransactionStateRestored: (SKPaymentTransaction *)transaction {
    
    // FIXME: 테스트필요!!!!!!!!!!!!!!
    NSLog(@"onTransactionStateRestored");
    
    [self finishTransaction:transaction];
}

/**
 *  트랜젝션 삭제
 */
- (void) finishTransaction:(SKPaymentTransaction *)transaction {
    
    [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
}

/**
 *  상품 복구 완료
 */
- (void) paymentQueueRestoreCompletedTransactionsFinished:(SKPaymentQueue *)queue {
    
    NSLog(@"paymentQueueRestoreCompletedTransactionsFinished: %d", queue.transactions.count);
    
    for( SKPaymentTransaction *transaction in queue.transactions ) {
        iap::IAPHelper::getInstance()->onRestored([IAPManager getItemId:transaction]);
    }
    
    iap::IAPHelper::getInstance()->onRestoreFinished(true);
}

+ (string) getItemId:(SKPaymentTransaction *)transaction {
    
    return [transaction.payment.productIdentifier UTF8String];
}

@end

#endif // SB_PLUGIN_USE_IAP
