//
//  RewardManager.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/10/27.
//

#include "RewardManager.hpp"

#include "Define.h"
#include "GameConfiguration.hpp"
#include "CharacterManager.hpp"
#include "User.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define USER_DEFAULT_KEY_REWARD_WORLD_STATUS(__WORLD__) \
STR_FORMAT("USER_REWARD_WORLD_%d_STATUS", __WORLD__)

#define USER_DEFAULT_KEY_REWARD_CHARACTER_STATUS(__CHAR_ID__) \
STR_FORMAT("USER_REWARD_CHARACTER_%s_STATUS", __CHAR_ID__.c_str())

static RewardManager *instance = nullptr;
RewardManager* RewardManager::getInstance() {
    
    if( !instance ) {
        instance = new RewardManager();
    }
    
    return instance;
}

void RewardManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

RewardManager::RewardManager() {
    
}

RewardManager::~RewardManager() {
    
}

void RewardManager::init() {
    
    CCLOG("========== PARSE START (reward.json)  ==========");
    
    string json = SBStringUtils::readTextFile(REWARD_CONFIG_FILE);
    
#if ENCRYPT_REWARD_CONFIG_FILE
    json = SBSecurity::decryptAES256(json, AES256_KEY);
#endif
    
    rapidjson::Document doc;
    doc.Parse(json.c_str());
    
    auto list = doc.GetArray();

    for( auto it = list.begin(); it != list.end(); it++ ) {
        auto obj = (*it).GetObject();
        
        RewardItemData item;
        item.type = (RewardItemType)obj["type"].GetInt();
        item.world = obj["world"].GetInt();
        item.requireStar = obj["require_star"].GetInt();
        
        if( item.type == RewardItemType::CHARACTER ) {
            item.charId = obj["char_id"].GetString();
        }
        
        items.push_back(item);
    }
    
    CCLOG("========== PARSE END (reward.json)  ==========");
    
    auto printRewardStatus = [=]() {
#if COCOS2D_DEBUG
        CCLOG("========== REWARD STATUS  ==========");
        auto worldCount = GAME_CONFIG->getWorldCount();
        auto getStatusStr = [](RewardItemStatus status) -> string {
            switch( status ) {
                case RewardItemStatus::LOCKED:       return "LOCKED";
                case RewardItemStatus::UNLOCKED:     return "UNLOCKED";
                case RewardItemStatus::REWARDED:     return "REWARDED";
                default:                             return "";
            }
        };
        
        // 월드
        for( int world = 1; world <= worldCount; ++world ) {
            auto key = USER_DEFAULT_KEY_REWARD_WORLD_STATUS(world);
            auto status = getWorldStatus(world);
            CCLOG("%s - %s", key.c_str(), getStatusStr(status).c_str());
        }
        
        // 캐릭터
        auto characters = CHARACTER_MANAGER->getCharacters();
        
        for( auto chc : characters ) {
            auto key = USER_DEFAULT_KEY_REWARD_CHARACTER_STATUS(chc.charId);
            auto status = getCharacterStatus(chc.charId);
            CCLOG("%s - %s", key.c_str(), getStatusStr(status).c_str());
        }
        
        CCLOG("====================================");
#endif
    };
    
    printRewardStatus();
    
    // v1에서 이미 획득한 캐릭터 예외처리
    auto characters = CHARACTER_MANAGER->getUnlockedCharacters_deprecated();
    
    for( auto chc : characters ) {
        setCharacterStatus(chc.charId, RewardItemStatus::REWARDED);
    }
    
    // 조건 달성 체크
    checkUnlock();
    
    printRewardStatus();
}

/**
 * 조건 달성된 아이템을 체크합니다
 */
void RewardManager::checkUnlock() {
    
    CCLOG("CHECK UNLOCK");
    
    // 기본 지급
    auto items = getItems();
    
    for( auto item : items ) {
        if( item.requireStar == 0 ) {
            if( item.isCharacter() ) {
                setCharacterStatus(item.charId, RewardItemStatus::REWARDED);
            } else {
                setWorldStatus(item.world, RewardItemStatus::REWARDED);
            }
        }
    }
    
    // 월드 별 체크
    auto worldCount = GAME_CONFIG->getWorldCount();
    
    for( int world = 1; world <= worldCount; ++world ) {
        checkUnlock(world);
    }
}

void RewardManager::checkUnlock(int world) {
    
    // 월드
    if( world > 1 ) {
        auto item = getWorldItem(world);
        
        if( getStatus(item) == RewardItemStatus::LOCKED ) {
            auto star = StageManager::getWorldStarCount(world-1); // 이전 월드의 별 개수
            
            if( star >= item.requireStar ) {
                setWorldStatus(world, RewardItemStatus::UNLOCKED);
            }
        }
    }
    
    // 캐릭터
    auto items = getCharacterItems(world);
    auto star = StageManager::getWorldStarCount(world);
    
    for( auto item : items ) {
        if( getStatus(item) == RewardItemStatus::LOCKED ) {
            if( star >= item.requireStar ) {
                setCharacterStatus(item.charId, RewardItemStatus::UNLOCKED);
            }
        }
    }
}

RewardItemDataList RewardManager::getItems() {
    return instance->items;
}

RewardItemDataList RewardManager::getItems(int world) {
    return SBCollection::find(getItems(), [=](const RewardItemData &item) -> bool {
        return item.world == world;
    });
}

/**
 * 해당 월드의 월드 아이템을 반환합니다
 */
RewardItemData RewardManager::getWorldItem(int world) {
    
    auto items = SBCollection::find(getItems(), [=](const RewardItemData &item) -> bool {
        return item.type == RewardItemType::WORLD && item.world == world;
    });
    
    if( items.size() > 0 ) {
        return items[0];
    } else {
        return RewardItemData();
    }
}

/**
 * 해당 월드의 캐릭터 아이템을 반환합니다
 */
RewardItemDataList RewardManager::getCharacterItems(int world) {
    
    auto items = SBCollection::find(getItems(), [=](const RewardItemData &item) -> bool {
        return item.type == RewardItemType::CHARACTER && item.world == world;
    });
    
    return items;
}

/**
 * 캐릭터 아이템을 반환합니다
 */
RewardItemData RewardManager::getCharacterItem(const string &charId) {
    
    auto items = SBCollection::find(getItems(), [=](const RewardItemData &item) -> bool {
        return item.charId == charId;
    });
    
    CCASSERT(items.size() > 0, "RewardManager::getCharacterItem error: invalid charId.");
    return items[0];
}

/**
 * 해당 보상 아이템의 상태를 반환합니다
 */
RewardItemStatus RewardManager::getStatus(const RewardItemData &data) {
    if( data.isCharacter() ) {
        return getCharacterStatus(data.charId);
    } else {
        return getWorldStatus(data.world);
    }
}

bool RewardManager::isRewarded(const RewardItemData &data) {
    return getStatus(data) == RewardItemStatus::REWARDED;
}

bool RewardManager::isRewardedWorld(int world) {
    return isRewarded(getWorldItem(world));
}

bool RewardManager::isRewardedCharacter(const string &charId) {
    return isRewarded(getCharacterItem(charId));
}

/**
 * 해당 월드의 상태를 반환합니다
 */
RewardItemStatus RewardManager::getWorldStatus(int world) {
    auto key = USER_DEFAULT_KEY_REWARD_WORLD_STATUS(world);
    return (RewardItemStatus)USER_DEFAULT->getIntegerForKey(key.c_str(),
                                                            (int)RewardItemStatus::LOCKED);
}

void RewardManager::setWorldStatus(int world, RewardItemStatus status) {
    auto key = USER_DEFAULT_KEY_REWARD_WORLD_STATUS(world);
    USER_DEFAULT->setIntegerForKey(key.c_str(), (int)status);
    USER_DEFAULT->flush();
}

/**
 * 해당 캐릭터의 상태를 반환합니다
 */
RewardItemStatus RewardManager::getCharacterStatus(const string &charId) {
    auto key = USER_DEFAULT_KEY_REWARD_CHARACTER_STATUS(charId);
    return (RewardItemStatus)USER_DEFAULT->getIntegerForKey(key.c_str(),
                                                            (int)RewardItemStatus::LOCKED);
}

void RewardManager::setCharacterStatus(const string &charId, RewardItemStatus status) {
    auto key = USER_DEFAULT_KEY_REWARD_CHARACTER_STATUS(charId);
    USER_DEFAULT->setIntegerForKey(key.c_str(), (int)status);
    USER_DEFAULT->flush();
}
