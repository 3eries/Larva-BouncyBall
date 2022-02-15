//
//  User.hpp
//
//  Created by seongmin hwang on 2018. 5. 23..
//

#ifndef User_hpp
#define User_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "ContentManager.hpp"

class User: public cocos2d::Ref {
public:
    static User* getInstance();
    static void destroyInstance();
    
    ~User();
    
private:
    User();
    
public:
    void init();
    
    // remove ads
    static void             setRemoveAds(bool isRemoveAds);
    static void             removeAds();
    static bool             isRemovedAds();
    
    // in game
    static void             setTutorialCompleted(bool isCompleted);
    static bool             isTutorialCompleted();
};

#endif /* User_hpp */
