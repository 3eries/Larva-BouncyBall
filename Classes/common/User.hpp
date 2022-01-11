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

class User : public cocos2d::Ref {
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
    
    // hint
    static void             setHintCount(int i);
    static int              getHintCount();
    static void             getHint(int i);
    static bool             useHint();
    
    // coin
    static void             setCoin(int i);
    static void             earnCoin(int i);
    static bool             spendCoin(int i);
    static int              getCoin();
    static bool             isEnoughCoin(int i = 0);
    
    // in game
    static void             setTutorialCompleted(bool isCompleted);
    static bool             isTutorialCompleted();
    
    static int              getStageStarCount(int stage);
    static void             setStageStarCount(int stage, int star);
    
    static void             unlockStage(int stage);
    static bool             isStageLocked(int stage);
    
    static int              getLatestPlayStage();
    static void             setLatestPlayStage(int stage);
    
    static int              getTopUnlockedStage();
    static void             setTopUnlockedStage(int stage);
};

#endif /* User_hpp */
