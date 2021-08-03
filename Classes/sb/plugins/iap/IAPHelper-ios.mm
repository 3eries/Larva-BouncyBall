//
//  IAPHelper-ios.m
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "IAPHelper.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import "ios/IAPManager.h"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;
NS_IAP_BEGIN;

void IAPHelper::initImpl(const Config &config, const string &originalJson) {
    
#if SB_PLUGIN_USE_IAP
    [[IAPManager getInstance] initIAP:config];
#endif
}

void IAPHelper::purchaseImpl(const Item &item) {
    
#if SB_PLUGIN_USE_IAP
    [[IAPManager getInstance] purchase:item.itemId];
#endif
}

void IAPHelper::restoreImpl() {
    
#if SB_PLUGIN_USE_IAP
    [[IAPManager getInstance] restore];
#endif
}

void IAPHelper::consumeAll() {
}

bool IAPHelper::isReady() {
    
#if SB_PLUGIN_USE_IAP
    return true;
#else
    return false;
#endif
}

NS_IAP_END;
NS_SB_END;

#endif // CC_PLATFORM_IOS
