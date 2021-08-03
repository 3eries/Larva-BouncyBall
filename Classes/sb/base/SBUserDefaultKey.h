//
//  SBUserDefaultKey.h
//
//  Created by seongmin hwang on 2018. 5. 17..
//

#ifndef SBUserDefaultKey_h
#define SBUserDefaultKey_h

#include "cocos2d.h"

namespace SBUserDefaultKey {
    static const char* RUN_COUNT                     = "SUPERBOMB_RUN_COUNT";                        // 앱 실행 횟수
    static const char* FIRST_RUN_TIME                = "SUPERBOMB_FIRST_RUN_TIME";                   // 최초 실행 시간
    static const char* LAST_RUN_TIME                 = "SUPERBOMB_LAST_RUN_TIME";                    // 마지막 실행 시간
    static const char* TODAY_DATE                    = "SUPERBOMB_TODAY_DATE";                       // 오늘 날짜
    
    static const char* EFFECT_MUTE                   = "SUPERBOMB_EFFECT_MUTE";                      // 효과음 음소거 여부
    static const char* BGM_MUTE                      = "SUPERBOMB_BGM_MUTE";                         // 배경음 음소거 여부
    
    static const char* REVIEW_WRITE                  = "SUPERBOMB_REVIEW_WRITE";                     // 리뷰 작성 여부
    static const char* REVIEW_ALERT_ENABLED          = "SUPERBOMB_REVIEW_ALERT_ENABLED";             // 리뷰 알림 활성화 여부
    static const char* REVIEW_ALERT_TIME             = "SUPERBOMB_REVIEW_ALERT_TIME";                // 리뷰 알림 노출 시간
}

#endif /* SBUserDefaultKey_h */
