//
//  AdsEventDispatcher.hpp
//
//  Created by seongmin hwang on 2018. 4. 9..
//

#ifndef AdsEventDispatcher_hpp
#define AdsEventDispatcher_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"

#include "AdsDefine.h"
#include "AdListener.hpp"

NS_SB_BEGIN;

/** @class AdsEventDispatcher
 * @brief 이 클래스는 구글 모바일 광고 SDK의 이벤트 리스너를 관리합니다
 */
class AdsEventDispatcher {
public:
    AdsEventDispatcher();
    ~AdsEventDispatcher();
    
public:
    typedef cocos2d::Vector<AdListener*> AdListeners;
    
public:
    void addListener(AdListener *listener);
    void addListener(AdType type, AdListener *listener);
    void removeListener(AdListener *listener);
    void removeListener(cocos2d::Ref *target);
    void removeListeners(AdListeners listeners, bool remainForever = true);
    
public:
    void dispatchLoadedEvent(AdType type);
    void dispatchFailedToLoadEvent(AdType type, int errorCode);
    void dispatchOpenedEvent(AdType type);
    void dispatchClosedEvent(AdType type);
    void dispatchRewardedEvent(std::string type, int amount);
    
private:
    template <class F>
    AdListeners findListeners(F func) {
        
        AdListeners result;
        
        for( auto listener : this->listeners ) {
            if( func(listener) ) {
                result.pushBack(listener);
            }
        }
        
        return result;
    }
    
    AdListeners listeners;
};

NS_SB_END;

#endif /* AdsEventDispatcher_hpp */
