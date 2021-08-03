//
//  AdsDefine.h
//
//  Created by seongmin hwang on 2018. 7. 26..
//

#ifndef AdsDefine_h
#define AdsDefine_h

#include "../../base/SBMacros.h"

NS_SB_BEGIN;

enum class AdPlatform {
    NONE = 0,
    ADMOB,
    FACEBOOK,
    INMOBI,
    UNITY,
    VUNGLE,
};

static const std::map<AdPlatform, std::string> AD_PLATFORM_STRING_MAP = {
    { AdPlatform::NONE,        "none", },
    { AdPlatform::ADMOB,       "admob", },
    { AdPlatform::FACEBOOK,    "facebook", },
    { AdPlatform::INMOBI,      "inmobi", },
    { AdPlatform::UNITY,       "unity", },
    { AdPlatform::VUNGLE,      "vungle", },
};

enum class AdType {
    NONE = 0,
    BANNER,
    INTERSTITIAL,
    REWARDED_VIDEO,
};

static inline std::string adTypeToString(const AdType &type) {
 
    switch( type ) {
        case AdType::BANNER:            return "Banner";
        case AdType::INTERSTITIAL:      return "Interstitial";
        case AdType::REWARDED_VIDEO:    return "RewardedVideo";
        default:                        return "none";
    }
}

struct AdsConfig {
    // 테스트 모드
    bool testMode;

    float autoLoadInterval;             // 광고 자동 로드 시간 간격 (단위 : 초)
    
    std::string appId;                  // 애드몹 앱 아이디
    std::string bannerUnitId;
    std::string interstitialUnitId;
    std::string rewardedVideoUnitId;
    
    AdsConfig() : testMode(false), autoLoadInterval(0), appId("") {}
};

NS_SB_END;

#endif /* AdsDefine_h */
