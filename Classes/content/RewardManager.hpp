//
//  RewardManager.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/10/27.
//

#ifndef RewardManager_hpp
#define RewardManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"

#include "data/model/CharacterData.h"
#include "data/model/StageData.h"

enum class RewardItemType {
    INVALID = 0,
    WORLD,
    CHARACTER,
};

enum class RewardItemStatus {
    INVALID = 0,
    LOCKED,       // 조건 미충족
    UNLOCKED,     // 조건 달성
    REWARDED,     // 보상 획득함
};

struct RewardItemData {
    RewardItemType type;
    int world;
    int requireStar;
    std::string charId;
    
    RewardItemData() : type(RewardItemType::INVALID) {}
    
    bool isCharacter() const {
        return type == RewardItemType::CHARACTER;
    }
};

typedef std::function<void(RewardItemData)> OnRewardItemDataListener;
typedef std::vector<RewardItemData> RewardItemDataList;

/** @class RewardManager
 * @brief 이 클래스는 보상 데이터를 관리합니다
 */
class RewardManager {
public:
    static RewardManager* getInstance();
    static void destroyInstance();
    ~RewardManager();
    
    void init();
    
private:
    RewardManager();
    
public:
    static void                    checkUnlock();
    static void                    checkUnlock(int world);
    
    static RewardItemDataList      getItems();
    static RewardItemDataList      getItems(int world);
    static RewardItemData          getWorldItem(int world);
    static RewardItemDataList      getCharacterItems(int world);
    static RewardItemData          getCharacterItem(const std::string &charId);
    
    static RewardItemStatus        getStatus(const RewardItemData &data);
    static bool                    isRewarded(const RewardItemData &data);
    static bool                    isRewardedWorld(int world);
    static bool                    isRewardedCharacter(const std::string &charId);
    
    static RewardItemStatus        getWorldStatus(int world);
    static void                    setWorldStatus(int world, RewardItemStatus status);
    
    static RewardItemStatus        getCharacterStatus(const std::string &charId);
    static void                    setCharacterStatus(const std::string &charId,
                                                      RewardItemStatus status);
    
private:
    RewardItemDataList items;
};

#endif /* RewardManager_hpp */
