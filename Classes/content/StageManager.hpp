//
//  StageManager.hpp
//
//  Created by seongmin hwang on 13/12/2018.
//

#ifndef StageManager_hpp
#define StageManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "data/model/CharacterData.h"
#include "data/model/StageData.h"

/** @class StageManager
 * @brief 이 클래스는 스테이지 데이터를 관리합니다
 */
class StageManager {
public:
    static StageManager* getInstance();
    static void destroyInstance();
    ~StageManager();
    
    void init();
    
private:
    StageManager();
    
    void parseStageFile();
    
public:
    static StageDataList       getStages();
    static StageData           getStage(int stage);
    static StageData           getLastStage();
    static int                 getWorld(int stage);

private:
    StageDataList stages;
    
#pragma mark- UserData
public:
    static int              getWorldFirstStage(int world);
    static int              getWorldLastStage(int world);
    static int              getWorldStarCount(int world);
    
    static int              getStageStarCount(int stage);
    static void             setStageStarCount(int stage, int star);
    static int              getStageStarTotalCount();
    static bool             isStageCleared(int stage);
    static int              getClearedStageCount();
    
    static void             unlockStage(int stage);
    static bool             isStageLocked(int stage);
    
    static int              getLatestPlayWorld();
    static int              getLatestPlayStage();
    static void             setLatestPlayStage(int stage);
    
    static int              getTopUnlockedStage(int world);
    static void             setTopUnlockedStage(int world, int stage);
    
    static int              getLatestMainWorldStarCount(int world);
    static void             setLatestMainWorldStarCount(int world, int star);
};

#endif /* StageManager_hpp */
