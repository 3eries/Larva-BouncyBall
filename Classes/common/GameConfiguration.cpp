//
//  GameConfiguration.cpp
//
//  Created by seongmin hwang on 2018. 6. 3..
//

#include "GameConfiguration.hpp"

#include "Define.h"

USING_NS_CC;
using namespace std;

#define JSON_FILE       "config/game_config.json"

static GameConfiguration *instance = nullptr;
GameConfiguration* GameConfiguration::getInstance() {
    
    if( !instance ) {
        instance = new GameConfiguration();
    }
    
    return instance;
}

void GameConfiguration::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

GameConfiguration::GameConfiguration() {
}

GameConfiguration::~GameConfiguration() {
}

void GameConfiguration::init() {

//    string jsonStr = SBStringUtils::readTextFile(JSON_FILE);
//    CCLOG("jsonStr: %s", jsonStr.c_str());
}

void GameConfiguration::parse(const string &json) {
    
    CCLOG("========== PARSE START (game_config.json)  ==========");
    
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    auto platformObj = doc["ios"].GetObject();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto platformObj = doc["android"].GetObject();
#endif
    
    // store
    storeUrl             = platformObj["store"].GetString();
    moreGamesUrl         = platformObj["more_games"].GetString();
    
    // game value
    worldCount           = doc["world_count"].GetInt();
    stagePerWorld        = doc["stage_per_world"].GetInt();
    firstCoin            = doc["first_coin"].GetInt();
    firstHint            = doc["first_hint"].GetInt();
    adRewardHint         = doc["ad_reward_hint"].GetInt();
    maxScore             = doc["max_score"].GetInt();
    
    // world_open_requirements
    {
        auto list = doc["world_open_requirements"].GetArray();

        for( auto it = list.begin(); it != list.end(); it++ ) {
            auto obj = (*it).GetObject();
            
            WorldOpenRequirements requirements;
            requirements.world = obj["world"].GetInt();
            requirements.requireStar = obj["open_require_star"].GetInt();
            
            worldOpenRequirementsList.push_back(requirements);
        }
    }
    
    CCLOG("\tstoreUrl: %s", storeUrl.c_str());
    CCLOG("\tmoreGamesUrl: %s", moreGamesUrl.c_str());
    CCLOG("\tworldCount: %d", worldCount);
    CCLOG("\tfirstHint: %d", firstHint);
    CCLOG("========== PARSE END (game_config.json)  ==========");
}

/**
 * 해당 월드의 오픈 조건을 반환합니다
 */
WorldOpenRequirements GameConfiguration::getWorldOpenRequirements(int world) {
    
    if( world <= worldOpenRequirementsList.size() ) {
        return worldOpenRequirementsList[world-1];
    }
    return WorldOpenRequirements();
}
