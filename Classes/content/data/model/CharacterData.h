//
//  CharacterDatae.h
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/25.
//

#ifndef CharacterData_h
#define CharacterData_h

#include "cocos2d.h"
#include "superbomb.h"

/**
* 캐릭터 잠금해제 타입
*/
enum class CharacterUnlockType {
    NONE    = -1,
    DEFAULT = 0,    // 기본 지급
    SAUSAGE,        // 획득한 소시지 개수
    STAGE,          // 스테이지 도달
    VIEW_ADS,       // 동영상 광고 시청 횟수
    
//    GAME_PLAY,      // 플레이한 게임의 판 수
//    GAME_OVER,      // 게임 오버한 횟수
//    DAILY_LOGIN,    // 게임에 접속한 횟수
//    CREDIT,         // 크레딧 진입 횟수
};

/** @struct CharacterData
 */
struct CharacterData {
    std::string              charId;
    std::string              name;
    CharacterUnlockType      unlockType;
    int                      unlockAmount;
    int                      openWorld;       // 캐릭터가 오픈되는 월드
    int                      openRequireStar; // 오픈에 필요한 별 개수
    
    CharacterData(): charId("") {}
    
    bool isViewAdsType() {
        return unlockType == CharacterUnlockType::VIEW_ADS;
    }
    
    std::string toString() {
        return STR_FORMAT("CharacterData { %s, %s }", charId.c_str(), name.c_str());
    }
    
    static StringList getCharacterIdList(const std::vector<CharacterData> &characters) {
        
        StringList ids;
        
        for( auto character : characters ) {
            ids.push_back(character.charId);
        }
        
        return ids;
    }
};

typedef std::vector<CharacterData>              CharacterDataList;
typedef StringList                              CharacterIdList;

typedef std::function<void(CharacterData)>      OnCharacterListener;
typedef std::function<void(CharacterDataList)>  OnCharacterListListener;
typedef std::function<void(CharacterIdList)>    OnCharacterIdListListener;

#endif /* CharacterData_h */
