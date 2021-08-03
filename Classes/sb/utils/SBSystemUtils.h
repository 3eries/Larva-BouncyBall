//
//  SBSystemUtils.h
//  SBLib
//
//  Created by hwangseongmin on 2015. 6. 5..
//
//

#ifndef __SBLib__SBSystemUtils__
#define __SBLib__SBSystemUtils__

#include <stdio.h>

#include "cocos2d.h"

/** @class SBSystemUtils
 * @brief 여러 가지 유용한 클래스 필드와 메서드가 포함되어 있습니다. 인스턴스화 할 수 없습니다.
 *
 */
class SBSystemUtils {
public:
    static void exitApp();
    
    static std::string timeToString(const struct tm *tm, const std::string &format);
    static std::string timeToString(time_t time, const std::string &format);
    
    static struct tm* getCurrentTime();
    static double getCurrentTimeMillis();
    static double getCurrentTimeSeconds();
    static std::string getCurrentTimeString(const std::string &format);
};

#endif /* defined(__SBLib__SBSystemUtils__) */
