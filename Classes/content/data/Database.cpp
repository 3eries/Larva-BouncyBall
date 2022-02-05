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
    
    parseStageFile();
}

void Database::parseStageFile() {
    
    CCLOG("========== STAGE PARSE START ==========");
    
    for( int i = 0; i < 7; ++i ) {
        const auto STAGE = i+1;
        const auto STAGE_FILE = DIR_CONTENT_STAGE + STR_FORMAT("stage_%04d.tmx", STAGE);
        
        // 맵 정보
        auto mapInfo = TMXMapInfo::create(STAGE_FILE);
        auto layerInfo = SBCollection::find(mapInfo->getLayers(), [](TMXLayerInfo *info) -> bool {
            return info->_name == "BlockLayer";
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
                    int tileId = properties.at("tileType").asInt();

                    tile.tileId = (TileId)tileId;
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

StageData Database::getLastStage() {
    auto stages = getStages();
    return stages.size() > 0 ? stages[stages.size()-1] : StageData();
}

