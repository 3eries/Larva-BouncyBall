//
//  ConsentManager.hpp
//
//  Created by seongmin hwang on 2018. 9. 27..
//

#ifndef ConsentManager_hpp
#define ConsentManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../base/SBMacros.h"

NS_SB_BEGIN;

class ConsentManager {
public:
    static ConsentManager* getInstance();
    static void destroyInstance();
    ~ConsentManager();
    
private:
    ConsentManager();
    
public:
    static void setPrivacyPolicyChecked(bool isChecked);
    static bool isPrivacyPolicyChecked();
};

NS_SB_END;

#endif /* ConsentManager_hpp */
