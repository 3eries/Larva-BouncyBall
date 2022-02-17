//
//  StageManager.cpp
//
//  Created by seongmin hwang on 13/12/2018.
//

#include "StageManager.hpp"

#include "Define.h"
#include "GameConfiguration.hpp"
#include "User.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define USER_DEFAULT_KEY_LATEST_PLAY_STAGE              "USER_LATEST_PLAY_STAGE"
#define USER_DEFAULT_KEY_TOP_UNLOCKED_STAGE             "USER_TOP_UNLOCKED_STAGE"

// #define USER_DEFAULT_KEY_STAGE_STAR                     "USER_STAGE_STAR"
#define USER_DEFAULT_KEY_STAGE_STAR(__STAGE__) \
STR_FORMAT("USER_STAGE_%d_STAR", __STAGE__).c_str()

static StageManager *instance = nullptr;
StageManager* StageManager::getInstance() {
    
    if( !instance ) {
        instance = new StageManager();
    }
    
    return instance;
}

void StageManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

StageManager::StageManager() {
    
}

StageManager::~StageManager() {
    
}

void StageManager::init() {
    
    parseStageFile();
    
    // 1스테이지 잠금 해제
    unlockStage(1);
    
    CCLOG("StageManager {");
    CCLOG("\t latest play stage: %d", getLatestPlayStage().stage);
    CCLOG("\t top unlocked stage: %d", getTopUnlockedStage());
    CCLOG("}");
}

void StageManager::parseStageFile() {
    
    CCLOG("========== STAGE PARSE START ==========");
    
    for( int i = 0; i < 18; ++i ) {
        const auto STAGE = i+1;
        const auto STAGE_FILE = DIR_CONTENT_STAGE + STR_FORMAT("stage_%04d.tmx", STAGE);
        
        // 맵 정보
        auto mapInfo = TMXMapInfo::create(STAGE_FILE);
        auto layerInfo = SBCollection::find(mapInfo->getLayers(), [](TMXLayerInfo *info) -> bool {
            return info->_name == "TileLayer";
        }).at(0);
        auto tileSize = mapInfo->getTileSize();
        
        StageData stage;
        stage.stage = STAGE;
        stage.world = ((STAGE-1) / GAME_CONFIG->getStagePerWorld()) + 1;
        stage.mapWidthTiles = (int)mapInfo->getMapSize().width;
        stage.mapHeightTiles = (int)mapInfo->getMapSize().height;
        stage.mapContentSize = Size(stage.mapWidthTiles * tileSize.width,
                                    stage.mapHeightTiles * tileSize.height);
        stage.tileSize = tileSize;
        
        // 타일 정보
//        {
//            vector<vector<int>> tiles;
//
//            for( int x = 0; x < stage.mapWidthTiles; x++ ) {
//                vector<int> heightTiles;
//
//                for( int y = 0; y < stage.mapHeightTiles; y++ ) {
//                    heightTiles.push_back(0);
//                }
//
//                tiles.push_back(heightTiles);
//            }
//        }
        
        {
            CCLOG("=== PARSED TILES HEAD ===");
            
            for( int y = 0; y < stage.mapHeightTiles; y++ ) {
                string str = "";
                
                for( int x = 0; x < stage.mapWidthTiles; x++ ) {
                    int pos = static_cast<int>(x + stage.mapWidthTiles * y);

                    int gid = layerInfo->_tiles[pos];
                    str += STR_FORMAT("[%d]", gid);
                }
                
                CCLOG("%s", str.c_str());
            }
            
            CCLOG("=== PARSED TILES TAIL ===");
        }
        
        for( int x = 0; x < stage.mapWidthTiles; x++ ) {
            TileDataList heightTiles;
            
            for( int y = 0; y < stage.mapHeightTiles; y++ ) {
                int pos = static_cast<int>(x + stage.mapWidthTiles * y);
                int gid = layerInfo->_tiles[pos];
                int convertToGLY = stage.mapHeightTiles - y - 1;
                
                TileData tile(TileId::NONE);
                tile.stage = STAGE;
                tile.setPosition(x, convertToGLY);
                
                if( gid != 0 ) {
                    auto properties = mapInfo->getTileProperties().at(gid).asValueMap();
                    int tileId = properties.at("tile_id").asInt();

                    tile.setTileId((TileId)tileId);
                }
                
                stage.tiles.push_back(tile);
            }
        }

        {
            CCLOG("");
            CCLOG("=== MY TILES HEAD ===");
            
            for( int y = stage.mapHeightTiles-1; y >= 0; y-- ) {
                string str = "";
                
                for( int x = 0; x < stage.mapWidthTiles; x++ ) {
                    auto tile = stage.getTile(x,y);
                    str += STR_FORMAT("[%d]", ((int)tile.tileId > (int)TileId::NONE) ? 1 : 0);
                }
                
                CCLOG("%s", str.c_str());
            }
            
            CCLOG("=== MY TILES TAIL ===");
        }
        
        stages.push_back(stage);
    }

    
    CCLOG("========== STAGE PARSE END ==========");
    
    // order by stage asc
    sort(stages.begin(), stages.end(), [](const StageData &s1, const StageData &s2) {
        return s1.stage < s2.stage;
    });
    
    // log
    for( auto stage : stages ) {
        CCLOG("%s", stage.toString().c_str());
        CCLOG("%s", stage.getTileMapString().c_str());
    }
}

/**
 * 스테이지 데이터를 반환합니다
 */
StageDataList StageManager::getStages() {
    return instance->stages;
}

StageData StageManager::getStage(int stage) {
    auto stages = getStages();
    
    for( auto stageData : stages ) {
        if( stageData.stage == stage ) {
            return stageData;
        }
    }
    
    return StageData();
}

/**
 * 마지막 스테이지 데이터를 반환합니다
 */
StageData StageManager::getLastStage() {
    auto stages = getStages();
    return stages.size() > 0 ? stages[stages.size()-1] : StageData();
}

#pragma mark- UserData

/**
 * 스테이지 별 개수를 반환합니다
 */
#define INVALID_STAR_COUNT      -1

int StageManager::getStageStarCount(int stage) {
 
    // use json
    /*
    auto json = USER_DEFAULT->getStringForKey(USER_DEFAULT_KEY_STAGE_STAR, "");
    
    if( json == "" ) {
        return INVALID_STAR_COUNT;
    }
    
    auto doc = SBJSON::parse(json);
    auto stageKey = TO_STRING(stage);
    
    if( doc.FindMember(stageKey.c_str()) == doc.MemberEnd() ) {
        return INVALID_STAR_COUNT;
    }
    
    return doc[stageKey.c_str()].GetInt();
     */
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_STAGE_STAR(stage), INVALID_STAR_COUNT);
}

/**
 * 합산된 별 개수를 반환합니다
 */
int StageManager::getStageStarTotalCount() {
    
    int topStage = getTopUnlockedStage();
    int total = 0;
    
    for( int stage = 1; stage <= topStage; ++stage ) {
        int star = getStageStarCount(stage);
        
        if( star != INVALID_STAR_COUNT ) {
            total += star;
        }
    }
        
    return total;
}

/**
 * 스테이지 클리어 여부를 반환합니다
 */
bool StageManager::isStageCleared(int stage) {
    return getStageStarCount(stage) > 0;
}

/**
 * 스테이지 별 개수를 설정합니다
 */
void StageManager::setStageStarCount(int stage, int star) {
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_STAGE_STAR(stage), star);
    USER_DEFAULT->flush();
}

/**
 * 스테이지를 잠금 해제합니다
 */
void StageManager::unlockStage(int stage) {
    setStageStarCount(stage, MAX(0, getStageStarCount(stage)));
    setTopUnlockedStage(stage);
}

bool StageManager::isStageLocked(int stage) {
    return getStageStarCount(stage) == INVALID_STAR_COUNT;
}

/**
 * 마지막으로 플레이한 스테이지를 반환합니다.
 */
StageData StageManager::getLatestPlayStage() {
    int stage = USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_LATEST_PLAY_STAGE, getTopUnlockedStage());
    return StageManager::getStage(stage);
}

void StageManager::setLatestPlayStage(int stage) {
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_LATEST_PLAY_STAGE, stage);
    USER_DEFAULT->flush();
}

/**
 * 잠금 해제된 최고 스테이지를 반환합니다.
 */
int StageManager::getTopUnlockedStage() {
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_TOP_UNLOCKED_STAGE, 1);
}

void StageManager::setTopUnlockedStage(int stage) {
    stage = MAX(getTopUnlockedStage(), stage);
    
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_TOP_UNLOCKED_STAGE, stage);
    USER_DEFAULT->flush();
}
