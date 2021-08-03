//
//  ReviewHelper.hpp
//
//  Created by seongmin hwang on 2018. 8. 13..
//

#ifndef ReviewHelper_hpp
#define ReviewHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../base/SBMacros.h"
#include "../base/SBTypes.hpp"

NS_SB_BEGIN;

/** @class ReviewHelper
 * @brief 스토어 리뷰 작성 알림 팝업을 담당합니다.
 */
class ReviewHelper {
public:
    static ReviewHelper* getInstance();
    static void destroyInstance();
    ~ReviewHelper();
    
private:
    ReviewHelper();
    
public:
    static void setReviewWrite(bool isWrite);
    static bool isReviewWrite();
    
    static bool isReviewAlertEnabled();
    static time_t getLastReviewAlertTime();
    
    static void showReviewPopup(SBCallback onCreateCustomPopup);
    
private:
    // 리뷰 알림 잠금 일수
    // ex) 1, 잠금일이 월요일이면 수요일에 오픈
    CC_SYNTHESIZE(int, firstLockedDay, FirstLockedDay); // 앱 최초 실행 후 잠금 일수
    CC_SYNTHESIZE(int, lockedDay, LockedDay);           // 재노출 잠금 일수
};

NS_SB_END;

#endif /* ReviewHelper_hpp */
