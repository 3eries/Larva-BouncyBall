//
//  CharacterManager.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/17.
//

#ifndef CharacterManager_hpp
#define CharacterManager_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "CharacterListener.hpp"
#include "data/model/CharacterData.h"

#define CHARACTER_MANAGER CharacterManager::getInstance()

#define CHARACTER_EVENT_SELECTED        "CHARACTER_EVENT_SELECTED" // 캐릭터 선택
#define CHARACTER_EVENT_UNLOCKED        "CHARACTER_EVENT_UNLOCKED" // 캐릭터 잠금 해제
#define CHARACTER_EVENT_RESTORED        "CHARACTER_EVENT_RESTORED" // 캐릭터 복원

/** @class CharacterManager
 * @brief 이 클래스는 캐릭터 데이터를 관리합니다
 */
class CharacterManager : public cocos2d::Ref {
public:
    static CharacterManager* getInstance();
    static void destroyInstance();
    ~CharacterManager();
    
private:
    CharacterManager();
    
    void initIAPListener();
    
public:
    void init();
    
    CharacterData       getCharacter(const std::string &charId);
    CharacterData       getSelectedCharacter();
    bool                isSelectedCharacter(const std::string &charId);
    bool                isCharacterUnlocked(const std::string &charId);
    
    void                setSelected(const std::string &charId);
    
    void                unlockAll(OnCharacterListListener onUnlocked = nullptr);
    void                unlock(const std::string &charId, bool isRestored = false);
    
    void                checkUnlock(OnCharacterListListener onUnlocked);
    
    int                 getViewAdsCount(const std::string &charId);
    void                increaseViewAdsCount(const std::string &charId);
    
private:
    CC_SYNTHESIZE_READONLY(StringList, characterOrder, CharacterOrder);
    CC_SYNTHESIZE_READONLY(CharacterDataList, characters, Characters);
    
#pragma mark- Character Event
public:
    void onCharacterSelected(CharacterData character);
//    void onCharacterUnlocked(CharacterDataList characters);
//    void onCharacterRestored(CharacterDataList characters);
    
public:
    void addListener(CharacterListener *listener);
    void removeListener(CharacterListener *listener);
    void removeListener(cocos2d::Ref *target);
    
private:
    cocos2d::Vector<CharacterListener*> listeners;
};


#endif /* CharacterManager_hpp */
