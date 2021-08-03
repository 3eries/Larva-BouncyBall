//
//  PopupManager.cpp
//
//  Created by seongmin hwang on 2018. 7. 6..
//

#include "PopupManager.hpp"

#include <spine/spine-cocos2dx.h>

#include "Define.h"
#include "SceneManager.h"

#include "ExitAlertPopup.hpp"
#include "../main/CreditPopup.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace spine;
using namespace std;

static PopupManager *instance = nullptr;
PopupManager* PopupManager::getInstance() {
    
    if( !instance ) {
        instance = new PopupManager();
    }
    
    return instance;
}

void PopupManager::destroyInstance() {
    
    CC_SAFE_DELETE(instance);
}

PopupManager::PopupManager() {
}

PopupManager::~PopupManager() {
    
    popups.clear();
    listeners.clear();
}

string PopupManager::getPopupInfo() {
    
    auto getPopupName = [](PopupType type) -> string {
        switch( type ) {
            case PopupType::NONE:              return "NONE";
            case PopupType::LOADING_BAR:       return "LOADING_BAR";
            case PopupType::EXIT_APP:          return "EXIT_APP";
            case PopupType::REVIEW:            return "REVIEW";
            case PopupType::CREDIT:            return "CREDIT";
            case PopupType::SHOP:              return "SHOP";
            case PopupType::SETTING:           return "SETTING";
            case PopupType::PAUSE:             return "PAUSE";
            case PopupType::GET_HINT:          return "GET_HINT";
            case PopupType::GAME_CLEAR:        return "GAME_CLEAR";
            case PopupType::GAME_RESULT:       return "GAME_RESULT";
            case PopupType::CONTINUE:          return "CONTINUE";
            default: break;
        };
        
        return "INVALID";
    };
    
    auto popups = getInstance()->popups;
    string str = "PopupInfo\n[\n";
    
    for( int i = popups.size()-1; i >= 0; --i ) {
        auto popup = popups.at(i);
        str += STR_FORMAT("\t%s isVisible: %d", getPopupName(popup->getType()).c_str(), popup->isVisible()) + "\n";
    }
    
    str += "]";
    
    return str;
}

/**
 * 팝업 추가
 */
void PopupManager::addPopup(BasePopup *popup) {
    
    CCASSERT(popup != nullptr, "PopupManager::addPopup error: popup must be not null");
    CCASSERT(popup->getType() != PopupType::NONE, "PopupManager::addPopup error: type must be not none");
    CCASSERT(!popups.contains(popup), "PopupManager::addPopup error: popup already added");
    
    popups.pushBack(popup);
    
    CCLOG("PopupManager::addPopup type: %d count: %d", (int)popup->getType(), (int)getPopupCount());
}

/**
 * 팝업 제거
 */
void PopupManager::removePopup(BasePopup *popup) {
    
    CCASSERT(popup != nullptr, "PopupManager::removePopup error: popup must be not null");
    
    if( popups.contains(popup) ) {
        popups.eraseObject(popup);
        
        CCLOG("PopupManager::removePopup type: %d count: %d", (int)popup->getType(), (int)getPopupCount());
    }
}

/**
 * 해당 타입의 팝업 반환
 */
BasePopup* PopupManager::getPopup(PopupType type) {
    
    auto popups = getInstance()->popups;
    
    for( auto popup : popups ) {
        if( popup->getType() == type ) {
            return popup;
        }
    }
    
    return nullptr;
}

/**
 * 가장 앞에 나와 있는 팝업 반환
 */
BasePopup* PopupManager::getFrontPopup() {
    
    auto popups = getInstance()->popups;
    
    for( int i = popups.size()-1; i >= 0; --i ) {
        auto popup = popups.at(i);
        
        if( popup->isVisible() ) {
            return popup;
        }
    }
    
    return nullptr;
}

/**
 * 모든 팝업 갯수 반환
 */
size_t PopupManager::getPopupCount() {
    return getInstance()->popups.size();
}

/**
 * 해당 타입의 팝업 갯수를 반환합니다
 */
size_t PopupManager::getPopupCount(PopupType type) {
    
    auto popups = getInstance()->popups;
    size_t cnt = 0;
    
    for( auto popup : popups ) {
        if( popup->getType() == type ) {
            cnt++;
        }
    }
    
    return cnt;
}

/**
 * 해당 타입의 팝업 존재 여부 반환
 */
bool PopupManager::exists(PopupType type) {
    
    return getPopup(type) != nullptr;
}

/**
 * 팝업 노출
 */
BasePopup* PopupManager::show(OnPopupEvent onEventListener, PopupType type, int zOrder) {
    
    if( zOrder == -1 ) {
        zOrder = ZOrder::POPUP_BOTTOM;
    }
    
    auto popup = createPopup(type);
    popup->setOnPopupEventListener([=](Node *sender, PopupEventType eventType) {
        
        if( onEventListener ) {
            onEventListener(sender, eventType);
        }
    });
    SceneManager::getInstance()->getScene()->addChild(popup, zOrder);
    
    popup->runEnterAction();
    
    return popup;
}

BasePopup* PopupManager::show(PopupType type, int zOrder) {

    return show(nullptr, type, zOrder);
}

/**
 * 팝업 크로스
 * 1번 퇴장 -> 2번 등장 -> 2번 퇴장 -> 3번 등장
 */
void PopupManager::cross(OnPopupEvent onEventListener,
                         BasePopup *popup1, BasePopup *popup2, BasePopup *popup3) {
    
    CCASSERT(popup1 != nullptr, "PopupManager::cross error: invalid popup1.");
    CCASSERT(popup2 != nullptr, "PopupManager::cross error: invalid popup2.");
    
    vector<BasePopup*> crossPopups({ popup1, popup2, });
    
    if( popup3 ) {
        crossPopups.push_back(popup3);
    }
    
    const float POPUP_EFFECT_TIME_SCALE = 0.7f;
    
    // 팝업 등/퇴장 시간 스케일
    for( auto popup : crossPopups ) {
        popup->setEnterTimeScale(POPUP_EFFECT_TIME_SCALE);
        popup->setExitTimeScale(POPUP_EFFECT_TIME_SCALE);
    }
    
    // 팝업 리스너 등록
    auto listener = PopupListener::create();
    listener->retain();
    listener->onEvent = [=](Node *sender, PopupEventType eventType) {
        
        bool found = false;
        
        for( auto popup : crossPopups ) {
            if( popup == sender ) {
                found = true;
                break;
            }
        }
        
        if( !found ) {
            return;
        }
        
        if( onEventListener ) {
            onEventListener(sender, eventType);
        }
        
        auto popup = dynamic_cast<BasePopup*>(sender);
        
        switch( eventType ) {
            case PopupEventType::ENTER: {
            } break;
                
            case PopupEventType::ENTER_ACTION_FINISHED: {
                popup->setEnterTimeScale(1);
                
                // 3번 등장 후 리스너 제거
                if( popup3 && popup == popup3 ) {
                    getInstance()->removeListener(listener);
                }
                
            } break;
                
            case PopupEventType::EXIT_ACTION: {
            } break;
                
            case PopupEventType::EXIT_ACTION_FINISHED: {
                popup->setExitTimeScale(1);
                
                // Step 3. 2번 퇴장 -> 3번 등장
                if( popup == popup2 ) {
                    if( popup3 ) {
                        popup3->runEnterAction();
                    }
                    // 3번 없음, 리스너 제거
                    else {
                        getInstance()->removeListener(listener);
                    }
                }
                
            } break;
                
            default: break;
        }
    };
    
    getInstance()->addListener(listener);
    
    // Step 1. 1번 팝업 퇴장
    popup1->runExitAction([=]() {
        
        // 3번 팝업이 있는 경우 1번 팝업 제거
//        if( popup1 != popup3 ) {
//            popup1->dismiss();
//        }
        
        // Step 2. 2번 팝업 등장
        SceneManager::getScene()->addChild(popup2, ZOrder::POPUP_BOTTOM);
        popup2->runEnterAction();
    });
}

void PopupManager::cross(BasePopup *popup1, BasePopup *popup2, BasePopup *popup3) {
    
    cross(nullptr, popup1, popup2, popup3);
}

BasePopup* PopupManager::createPopup(PopupType type) {
    
    switch( type ) {
        case PopupType::EXIT_APP:              return ExitAlertPopup::create();
        case PopupType::CREDIT:                return CreditPopup::create();
        default:
            return nullptr;
    }
}

/**
 * 리스너 등록
 */
void PopupManager::addListener(PopupListener *listener) {
    
    CCASSERT(listener != nullptr, "PopupManager::addListener error: listener must be not null");
    
    if( !listeners.contains(listener) ) {
        listeners.pushBack(listener);
    }
    
    CCLOG("PopupManager::addListener count: %d", (int)listeners.size());
}

void PopupManager::addListener(Node *target, PopupListener *listener) {
    
    CCASSERT(target != nullptr, "PopupManager::addListener error: target must be not null");
    CCASSERT(listener != nullptr, "PopupManager::addListener error: listener must be not null");
    
    listener->setTarget(target);
    addListener(listener);
}

/**
 * 리스너 제거
 */
void PopupManager::removeListener(PopupListener *listener) {
    
    CCASSERT(listener != nullptr, "PopupManager::removeListener error: listener must be not null");
    
    if( listeners.contains(listener) ) {
        listeners.eraseObject(listener);
    }
    
    // Log::i("PopupManager::removeListener count: %d", (int)listeners.size());
}

void PopupManager::removeListener(Ref *target) {
    
    CCASSERT(target != nullptr, "PopupManager::removeListener error: target must be not null");
    
    auto removeListeners = SBCollection::find(listeners, [=](PopupListener *listener) -> bool {
        return listener->getTarget() == target;
    });
    
    for( auto listener : removeListeners ) {
        listeners.eraseObject(listener);
    }
    
    // Log::i("PopupManager::removeListener count: %d", (int)listeners.size());
}

/**
 * 팝업 이벤트 전달
 */
void PopupManager::dispatchEvent(BasePopup *popup, PopupEventType eventType) {
    
    for( auto listener : listeners ) {
        if( listener->onEvent ) {
            listener->onEvent(popup, eventType);
        }
    }
}


