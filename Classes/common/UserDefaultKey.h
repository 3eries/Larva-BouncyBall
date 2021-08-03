//
//  UserDefaultKey.h
//  Snow Bros-mobile
//
//  Created by seongmin hwang on 2018. 3. 22..
//

#ifndef UserDefaultKey_h
#define UserDefaultKey_h

#include "cocos2d.h"
#include "superbomb.h"

namespace UserDefaultKey {
    static const char* EXTERNAL_GAME_CONFIG_FILE_VERSION     = "EXTERNAL_GAME_CONFIG_FILE_VERSION"; // 게임 설정 파일 버전
    
    static const char* FIRST_RUN                             = "FIRST_RUN";                         // 첫 실행 여부
    static const char* SPLASH_LOGO_VIEW_TIME                 = "SPLASH_LOGO_VIEW_TIME";             // 스플래쉬 시리즈 로고를 본 시각
    static const char* PLAY_COUNT                            = "PLAY_COUNT";                        // 인게임 플레이 횟수
    static const char* BEST_SCORE                            = "BEST_SCORE";                        // 베스트 스코어
    static const char* RANKING                               = "RANKING";                           // 랭킹 JSON
}

#endif /* UserDefaultKey_h */
