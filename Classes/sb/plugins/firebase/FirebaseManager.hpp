//
//  FirebaseManager.hpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#ifndef FirebaseManager_hpp
#define FirebaseManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"

NS_SB_BEGIN;

class FirebaseManager {
public:
    static FirebaseManager* getInstance();
    static void destroyInstance();
    ~FirebaseManager();
    
private:
    FirebaseManager();
    
public:
    void init();
};

NS_SB_END;

#endif /* FirebaseManager_hpp */
