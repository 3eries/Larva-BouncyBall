//
//  IAPListener.cpp
//
//  Created by seongmin hwang on 2018. 7. 12..
//

#include "IAPListener.hpp"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;
NS_IAP_BEGIN;

#pragma mark- Listener

Listener::Listener() :
target(nullptr), forever(false),
onPurchased(nullptr), onError(nullptr), onCanceled(nullptr), onFinished(nullptr), onRemoveAds(nullptr) {

}

Listener::~Listener() {
}

#pragma mark- PurchaseListener

PurchaseListener::PurchaseListener() {
    
}

PurchaseListener::~PurchaseListener() {
    
}

#pragma mark- RestoreListener

RestoreListener::RestoreListener() {
    
}

RestoreListener::~RestoreListener() {
    
}

NS_IAP_END;
NS_SB_END;
