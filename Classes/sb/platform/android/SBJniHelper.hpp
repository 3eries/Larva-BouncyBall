//
//  SBJniHelper.hpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#ifndef SBJniHelper_hpp
#define SBJniHelper_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

NS_SB_BEGIN;

class JniHelper {
public:
    static jobject createMapObject(JNIEnv *env, std::map<std::string, std::string> params);
};

NS_SB_END;

#endif // CC_PLATFORM_ANDROID

#endif /* SBJniHelper_hpp */
