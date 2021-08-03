//
//  Database.cpp
//
//  Created by seongmin hwang on 13/12/2018.
//

#include "Database.hpp"

#include "Define.h"
#include "GameConfiguration.hpp"
#include "User.hpp"
#include "../ContentResourceHelper.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define STAGE_FILE               (DIR_CONTENT_DATA + "stage.json")

static Database *instance = nullptr;
Database* Database::getInstance() {
    
    if( !instance ) {
        instance = new Database();
    }
    
    return instance;
}

void Database::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

Database::Database() {
    
}

Database::~Database() {
    
}

void Database::init() {
    
    parseStageJson();
}

void Database::parseStageJson() {
    
    CCLOG("========== PARSE START (stage.json)  ==========");
    string json = SBStringUtils::readTextFile(STAGE_FILE);
    
#if ENCRYPT_STAGE_FILE
    json = SBSecurity::decryptAES256(json, AES256_KEY);
#endif
    
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    
    rapidjson::Document::AllocatorType &allocator = doc.GetAllocator();

    auto list = doc.GetArray();
    
    for( int i = 0; i < list.Size(); ++i ) {
        const rapidjson::Value &v = list[i];
        
        StageData stage;
        stage.parse(v, allocator);
        
        stages.push_back(stage);
    }
    
    CCLOG("========== PARSE END (stage.json)  ==========");
    
    // order by floor asc
    sort(stages.begin(), stages.end(), [](const StageData &s1, const StageData &s2) {
        return s1.stage < s2.stage;
    });
    
    originStages = stages;
    
    // log
    for( auto stage : stages ) {
        CCLOG("%s", stage.toString().c_str());
        CCLOG("%s", stage.getTileMapString().c_str());
    }
}

/**
 * 스테이지 데이터를 반환합니다
 */
StageDataList Database::getOriginalStages() {
    
    return instance->originStages;
}

StageDataList Database::getStages() {
    
    return instance->stages;
}

StageData Database::getStage(int stage) {
    
    auto stages = getStages();
    
    for( auto stageData : stages ) {
        if( stageData.stage == stage ) {
            return stageData;
        }
    }
    
    return StageData();
}

StageData Database::getFirstStage() {
    
    auto stages = getStages();
    return stages[0];
}

StageData Database::getLastStage() {
    
    auto stages = getStages();
    return stages.size() > 0 ? stages[stages.size()-1] : StageData();
}
