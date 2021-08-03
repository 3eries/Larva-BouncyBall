//
//  ReviewHelper.cpp
//
//  Created by seongmin hwang on 2018. 8. 13..
//

#include "ReviewHelper.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#if SB_PLUGIN_USE_IAP
#import <StoreKit/StoreKit.h>
#endif // SB_PLUGIN_USE_IAP
#endif

#include "../base/SBUserDefaultKey.h"
#include "../base/SBDirector.hpp"

#include "SBSystemUtils.h"
#include "SBStringUtils.h"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

static ReviewHelper *instance = nullptr;
ReviewHelper* ReviewHelper::getInstance() {
    
    if( !instance ) {
        instance = new ReviewHelper();
    }
    
    return instance;
}

void ReviewHelper::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

ReviewHelper::ReviewHelper() :
firstLockedDay(0),
lockedDay(0) {
}

ReviewHelper::~ReviewHelper() {
}

/**
 * 리뷰 작성 여부
 */
void ReviewHelper::setReviewWrite(bool isWrite) {
    
    UserDefault::getInstance()->setBoolForKey(SBUserDefaultKey::REVIEW_WRITE, isWrite);
    UserDefault::getInstance()->flush();
}

bool ReviewHelper::isReviewWrite() {
    
    return UserDefault::getInstance()->getBoolForKey(SBUserDefaultKey::REVIEW_WRITE, false);
}

/**
 * 리뷰 알림 활성화 여부
 */
bool ReviewHelper::isReviewAlertEnabled() {
    
    // 작성 여부 체크
    if( isReviewWrite() ) {
        return false;
    }
    
    const string DATE_FORMAT = "%Y-%m-%d";
    const int DAY_SEC = 24 * 60 * 60;
    
    bool isEnabled = true;
    
    time_t now;
    time(&now);
    
    string nowDate = SBSystemUtils::timeToString(now, DATE_FORMAT);
    
    // 앱 최초 실행 후 잠금 체크
    const int firstLockedDay = getInstance()->getFirstLockedDay();
    
    if( firstLockedDay > 0 ) {
        time_t lockedTime = SBDirector::getFirstRunTime() + (firstLockedDay * DAY_SEC);
        string lockedDate = SBSystemUtils::timeToString(lockedTime, DATE_FORMAT);
        
        if( nowDate <= lockedDate ) {
            isEnabled = false;
        }
    }
    
    // 재노출 잠금 체크
    const int lockedDay = getInstance()->getLockedDay();
    time_t lastReviewAlertTime = getLastReviewAlertTime();
    
    if( isEnabled && lockedDay > 0 && lastReviewAlertTime > 0 ) {
        time_t lockedTime = lastReviewAlertTime + (lockedDay * DAY_SEC);
        string lockedDate = SBSystemUtils::timeToString(lockedTime, DATE_FORMAT);
        
        if( nowDate <= lockedDate ) {
            isEnabled = false;
        }
    }
    
    return isEnabled;
}

/**
 * 리뷰 알림이 노출된 마지막 시간을 반환 합니다
 */
time_t ReviewHelper::getLastReviewAlertTime() {
    
    string time = UserDefault::getInstance()->getStringForKey(SBUserDefaultKey::REVIEW_ALERT_TIME, "");
    
    if( time != "" ) {
        return SBStringUtils::toNumber<time_t>(time);
    }
    
    return 0;
}

/**
 * 리뷰 알림 팝업 노출
 */
void ReviewHelper::showReviewPopup(SBCallback onCreateCustomPopup) {
    
    const string nowTime = TO_STRING(mktime(SBSystemUtils::getCurrentTime()));
    
    UserDefault::getInstance()->setStringForKey(SBUserDefaultKey::REVIEW_ALERT_TIME, nowTime);
    UserDefault::getInstance()->flush();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if( @available(iOS 10.3, *) ) {
        // do storekit review here
#if SB_PLUGIN_USE_IAP
        [SKStoreReviewController requestReview];
#endif // SB_PLUGIN_USE_IAP
        // TODO: 콜백으로 write flag 갱신
    } else {
        if( onCreateCustomPopup ) {
            onCreateCustomPopup();
        }
    }
#else
    if( onCreateCustomPopup ) {
        onCreateCustomPopup();
    }
#endif
}

NS_SB_END;
