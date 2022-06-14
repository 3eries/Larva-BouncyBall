//
//  SBPlatformHelper.h
//
//  Created by seongmin hwang on 2018. 7. 23..
//

#ifndef SBPlatformHelper_h
#define SBPlatformHelper_h

#include "cocos2d.h"

#include "../base/SBMacros.h"
#include "../base/SBTypes.hpp"
#include "../utils/SBSystemUtils.h"
#include "SBPlatformMacros.h"

class SBPlatformHelper {
public:
    static bool isNetworkOnline();
    static void showNetworkErrorPopup();
};

#endif /* SBPlatformHelper_h */
