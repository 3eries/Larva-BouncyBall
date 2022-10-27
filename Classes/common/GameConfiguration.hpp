//
//  GameConfiguration.hpp
//
//  Created by seongmin hwang on 2018. 6. 3..
//

#ifndef GameConfiguration_hpp
#define GameConfiguration_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#define INVALID_WORLD       -1

/**
 * 월드 오픈 조건
 */
struct WorldOpenRequirements {
    int world;
    int requireStar;
    
    WorldOpenRequirements(): world(INVALID_WORLD) {}
};

#define GAME_CONFIG              GameConfiguration::getInstance()
#define MAX_STAR_COUNT_PER_WORLD (GAME_CONFIG->getStagePerWorld()*3)

class GameConfiguration : public cocos2d::Ref {
public:
    static GameConfiguration* getInstance();
    static void destroyInstance();
    ~GameConfiguration();
    
private:
    GameConfiguration();
    
public:
    void init();
    void parse(const std::string &json);
    
    WorldOpenRequirements getWorldOpenRequirements(int world);
    
private:
    CC_SYNTHESIZE_READONLY(std::string, storeUrl, StoreUrl);
    CC_SYNTHESIZE_READONLY(std::string, moreGamesUrl, MoreGamesUrl);
    
    CC_SYNTHESIZE_READONLY(int, worldCount, WorldCount);                // 월드 개수
    CC_SYNTHESIZE_READONLY(int, stagePerWorld, StagePerWorld);          // 월드 당 스테이지 개수
    CC_SYNTHESIZE_READONLY(std::vector<WorldOpenRequirements>, worldOpenRequirementsList, WorldOpenRequirementsList);
    
    CC_SYNTHESIZE_READONLY(int, firstCoin, FirstCoin);                  // 최초 코인
    CC_SYNTHESIZE_READONLY(int, firstHint, FirstHint);                  // 최초 힌트
    CC_SYNTHESIZE_READONLY(int, adRewardHint, AdRewardHint);            // 광고 보상 힌트
    CC_SYNTHESIZE_READONLY(int, maxScore, MaxScore);                    // 최대 스코어
};

#endif /* GameConfiguration_hpp */
