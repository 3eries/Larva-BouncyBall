//
//  CharacterManager.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/02/17.
//

#include "CharacterManager.hpp"

#include "Define.h"
#include "User.hpp"
#include "StageManager.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace std;

#define USER_DEFAULT_KEY_SELECTED_CHARACTER                "SELECTED_CHARACTER"

#define USER_DEFAULT_KEY_UNLOCKED_CHARACTER(__CHAR_ID__) \
STR_FORMAT("UNLOCKED_CHARACTER_%s", __CHAR_ID__.c_str()).c_str()

#define USER_DEFAULT_KEY_CHARACTER_VIEW_ADS_COUNT(__CHAR_ID__) \
STR_FORMAT("CHARACTER_VIEW_ADS_COUNT_%s", __CHAR_ID__.c_str()).c_str()

static CharacterManager *instance = nullptr;

CharacterManager* CharacterManager::getInstance() {
    
    if( !instance ) {
        instance = new CharacterManager();
    }
    
    return instance;
}

void CharacterManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

CharacterManager::CharacterManager() {
    
    initIAPListener();
}

CharacterManager::~CharacterManager() {
 
    removeListener(this);
    
    listeners.clear();
}

/**
 * 인앱 결제 리스너 초기화
 */
void CharacterManager::initIAPListener() {
    
    auto restoreListener = iap::RestoreListener::create();
    restoreListener->setForever(true);
    restoreListener->onRemoveAds = [=]() {
        this->unlockAll();
    };
    iap::IAPHelper::getInstance()->addListener(this, restoreListener);
}

void CharacterManager::init() {
    
    string json = SBStringUtils::readTextFile(CHARACTER_CONFIG_FILE);
    
#if ENCRYPT_CHARACTER_CONFIG_FILE
    json = SBSecurity::decryptAES256(json, AES256_KEY);
#endif
    
    CCLOG("CharacterManager::init:\n%s", json.c_str());
    CCLOG("========== PARSE START (character.json)  ==========");
    
    // 패키지 json 파싱
    rapidjson::Document doc = SBJSON::parse(json);
    
    // character order
    auto charOrder = doc["character_order"].GetArray();
    
    for( int i = 0; i < charOrder.Size(); ++i ) {
        const rapidjson::Value &v = charOrder[i];
        characterOrder.push_back(v.GetString());
    }
    
    // characters
    auto charList = doc["characters"].GetArray();
    
    for( int i = 0; i < charList.Size(); ++i ) {
        const rapidjson::Value &charValue = charList[i];
        
        CharacterData character;
        character.charId = charValue["char_id"].GetString();
        character.name = charValue["name"].GetString();
        character.unlockType = (CharacterUnlockType)charValue["unlock_type"].GetInt();
        character.unlockAmount = charValue["unlock_amount"].GetInt();
        
        characters.push_back(character);
        
        CCLOG("%s", character.toString().c_str());
    }
    
    CCLOG("========== PARSE END (character.json)  ==========");
    
    // 첫번째 캐릭터 잠금 해제
    unlock(characters[0].charId);
    
    // 기본 캐릭터 설정
    string charId = USER_DEFAULT->getStringForKey(USER_DEFAULT_KEY_SELECTED_CHARACTER, "");
    
    if( charId == "" ) {
        setSelected(characters[0].charId);
    }
}
/**
 * 캐릭터 반환
 */
CharacterData CharacterManager::getCharacter(const string &charId) {
    
    for( auto chc : characters ) {
        if( chc.charId == charId ) {
            return chc;
        }
    }
    
    return CharacterData();
}

/**
 * 선택된 캐릭터를 반환합니다
 */
CharacterData CharacterManager::getSelectedCharacter() {
    
    string charId = USER_DEFAULT->getStringForKey(USER_DEFAULT_KEY_SELECTED_CHARACTER, "");
    
    if( charId == "" ) {
        CCASSERT(false, "CharacterManager::getSelectedCharacter error.");
    }
    
    return getCharacter(charId);
}

bool CharacterManager::isSelectedCharacter(const string &charId) {
    return getSelectedCharacter().charId == charId;
}

/**
 * 캐릭터 잠금 해제 여부를 반환 합니다
 */
bool CharacterManager::isCharacterUnlocked(const string &charId) {
    
    return USER_DEFAULT->getBoolForKey(USER_DEFAULT_KEY_UNLOCKED_CHARACTER(charId), false);
}

/**
 * 캐릭터 선택
 */
void CharacterManager::setSelected(const string &charId) {
    
    CCASSERT(charId != "", "CharacterManager::setSelected error: invalid character id.");
    
    USER_DEFAULT->setStringForKey(USER_DEFAULT_KEY_SELECTED_CHARACTER, charId);
    USER_DEFAULT->flush();
    
    onCharacterSelected(getCharacter(charId));
}

/**
 * 잠금 해제된 캐릭터 리스트를 반환합니다
 */
CharacterDataList CharacterManager::getUnlockedCharacters() {
    return SBCollection::find(characters, [=](const CharacterData &chc) -> bool {
        return this->isCharacterUnlocked(chc.charId);
    });
}

void CharacterManager::unlockAll(OnCharacterListListener onUnlocked) {
 
    CharacterDataList newUnlockList;
    
    for( auto charId : characterOrder ) {
        if( isCharacterUnlocked(charId) ) {
            continue;
        }
        
        unlock(charId);
        newUnlockList.push_back(getCharacter(charId));
    }
    
    if( onUnlocked ) {
        onUnlocked(newUnlockList);
    }
}

/**
 * 캐릭터 잠금 해제
 */
void CharacterManager::unlock(const string &charId, bool isRestored) {
    
    if( isCharacterUnlocked(charId) ) {
        // 이미 잠금 해제됨
        return;
    }
    
    USER_DEFAULT->setBoolForKey(USER_DEFAULT_KEY_UNLOCKED_CHARACTER(charId), true);
    USER_DEFAULT->flush();
    
//    auto character = getCharacter(charId);
//
//    if( !isRestored ) {
//        onCharacterUnlocked(CharacterDataList({character}));
//    } else {
//        onCharacterRestored(CharacterDataList({character}));
//    }
}

/**
 * 캐릭터 잠금 해제 여부를 확인합니다
 */
void CharacterManager::checkUnlock(OnCharacterListListener onUnlocked) {
    
    const auto isRemovedAds = User::isRemovedAds();
    CharacterDataList unlockList;
    
    for( auto chc : characters ) {
        if( isCharacterUnlocked(chc.charId) ) {
            continue;
        }
        
        // IAP 아이템 보유
        if( isRemovedAds ) {
            unlockList.push_back(chc);
            continue;
        }
        
        // 잠금 해제 조건 체크
        switch( chc.unlockType ) {
            case CharacterUnlockType::SAUSAGE: {
                if( StageManager::getStageStarTotalCount() >= chc.unlockAmount ) {
                    unlockList.push_back(chc);
                }
            } break;
                
            case CharacterUnlockType::STAGE: {
                if( StageManager::isStageCleared(chc.unlockAmount) ) {
                    unlockList.push_back(chc);
                }
            } break;
                
            case CharacterUnlockType::VIEW_ADS: {
                if( getViewAdsCount(chc.charId) >= chc.unlockAmount ) {
                    unlockList.push_back(chc);
                }
            } break;
                
            default: break;
        }
    }
    
    if( unlockList.size() > 0 ) {
        for( auto chc : unlockList ) {
            unlock(chc.charId);
        }
        
        onUnlocked(unlockList);
    }
}

/**
 * 캐릭터 VIEW_ADS 카운트를 반환합니다
 */
int CharacterManager::getViewAdsCount(const string &charId) {
 
    return USER_DEFAULT->getIntegerForKey(USER_DEFAULT_KEY_CHARACTER_VIEW_ADS_COUNT(charId), 0);
}

/**
 * 캐릭터 VIEW_ADS 카운트를 1증가시킵니다
 */
void CharacterManager::increaseViewAdsCount(const string &charId) {
    
    USER_DEFAULT->setIntegerForKey(USER_DEFAULT_KEY_CHARACTER_VIEW_ADS_COUNT(charId), getViewAdsCount(charId)+1);
    USER_DEFAULT->flush();
}

#pragma mark- Character Event

/**
 * 캐릭터 선택됨
 */
void CharacterManager::onCharacterSelected(CharacterData character) {
    
    CCLOG("CharacterManager::onCharacterSelected: %s", character.charId.c_str());
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CHARACTER_EVENT_SELECTED, &character);
    
//    for( auto listener : listeners ) {
//        if( listener->onCharacterSelected ) {
//            listener->onCharacterSelected(character);
//        }
//    }
}

///**
// * 캐릭터 잠금 해제
// */
//void CharacterManager::onCharacterUnlocked(CharacterDataList characters) {
//
//    CCLOG("CharacterManager::onCharacterUnlocked: %s", SBStringUtils::insert(CharacterData::getCharacterIdList(characters), ",").c_str());
//
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CHARACTER_EVENT_UNLOCKED, &characters);
//
////    for( auto listener : listeners ) {
////        if( listener->onCharacterUnlocked ) {
////            listener->onCharacterUnlocked(characters);
////        }
////    }
//}
//
///**
// * 캐릭터 IAP 복원
// */
//void CharacterManager::onCharacterRestored(CharacterDataList characters) {
//
//    CCLOG("CharacterManager::onCharacterRestored: %s", SBStringUtils::insert(CharacterData::getCharacterIdList(characters), ",").c_str());
//
//    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CHARACTER_EVENT_RESTORED, &characters);
//
////    for( auto listener : listeners ) {
////        if( listener->onCharacterRestored ) {
////            listener->onCharacterRestored(characters);
////        }
////    }
//}

/**
 * 리스너 등록
 */
void CharacterManager::addListener(CharacterListener *listener) {
    
    CCASSERT(listener != nullptr, "CharacterManager::addListener error: listener must be not null");
    
    if( !listeners.contains(listener) ) {
        listeners.pushBack(listener);
    }
}

/**
 * 리스너 제거
 */
void CharacterManager::removeListener(CharacterListener *listener) {
    
    CCASSERT(listener != nullptr, "CharacterManager::addListener error: listener must be not null");
    
    if( listeners.contains(listener) ) {
        listeners.eraseObject(listener);
    }
}

void CharacterManager::removeListener(Ref *target) {
    
    CCASSERT(target != nullptr, "CharacterManager::removeListener error: target must be not null");
    
    auto removeListeners = SBCollection::find(listeners, [=](CharacterListener *listener) -> bool {
        return listener->getTarget() == target;
    });
    
    for( auto listener : removeListeners ) {
        listeners.eraseObject(listener);
    }
}
