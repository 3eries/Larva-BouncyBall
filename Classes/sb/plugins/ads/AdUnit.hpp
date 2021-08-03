//
//  AdUnit.hpp
//
//  Created by seongmin hwang on 09/09/2019.
//

#ifndef AdUnit_hpp
#define AdUnit_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"
#include "../SBPluginConfig.h"

#include "AdsDefine.h"
#include "AdListener.hpp"

NS_SB_BEGIN;

class AdUnit : public cocos2d::Ref {
public:
    AdUnit(const AdType &type, const std::string &unitId);
    ~AdUnit();
    
public:
    void onAdLoaded();
    void onAdFailedToLoad(int errorCode);
    void onAdOpened();
    void onAdClosed();
    void onRewarded(const std::string &type, int amount);
    
protected:
    CC_SYNTHESIZE_READONLY(AdType, type, Type);
    CC_SYNTHESIZE_READONLY(std::string, unitId, UnitId);
    CC_SYNTHESIZE_RETAIN(AdListener*, listener, Listener);
    
    SB_SYNTHESIZE_BOOL(enabled, Enabled);
    SB_SYNTHESIZE_BOOL(visible, Visible);
    SB_SYNTHESIZE_BOOL(loading, Loading);
    SB_SYNTHESIZE_BOOL(loaded, Loaded);
    SB_SYNTHESIZE_READONLY_BOOL(opened, Opened);
};

NS_SB_END;

#endif /* AdUnit_hpp */
