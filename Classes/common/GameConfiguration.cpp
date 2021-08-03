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
    firstCoin            = doc["first_coin"].GetInt();
    firstHint            = doc["first_hint"].GetInt();
    adRewardHint         = doc["ad_reward_hint"].GetInt();
    maxScore             = doc["max_score"].GetInt();
    
    CCLOG("\tstoreUrl: %s", storeUrl.c_str());
    CCLOG("\tmoreGamesUrl: %s", moreGamesUrl.c_str());
    CCLOG("\tfirstHint: %d", firstHint);
    CCLOG("========== PARSE END (game_config.json)  ==========");
}
