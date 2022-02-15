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

#include "model/CharacterData.h"
#include "model/StageData.h"

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

private:
    StageDataList stages;
    
#pragma mark- UserData
public:
    static int              getStageStarCount(int stage);
    static int              getStageStarTotalCount();
    static bool             isStageCleared(int stage);
    static void             setStageStarCount(int stage, int star);
    
    static void             unlockStage(int stage);
    static bool             isStageLocked(int stage);
    
    static StageData        getLatestPlayStage();
    static void             setLatestPlayStage(int stage);
    
    static int              getTopUnlockedStage();
    static void             setTopUnlockedStage(int stage);
};

#endif /* StageManager_hpp */
