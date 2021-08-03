//
//  AdListener.hpp
//
//  Created by seongmin hwang on 2018. 7. 26..
//

#ifndef AdListener_hpp
#define AdListener_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"

#include "AdsDefine.h"

NS_SB_BEGIN;

#pragma mark- AdListener

class AdListener : public cocos2d::Ref {
public:
    static AdListener* create(AdType type);
    virtual ~AdListener();
    
public:
    std::function<void()>                    onAdLoaded;
    std::function<void(int/*errorCode*/)>    onAdFailedToLoad;
    std::function<void()>                    onAdOpened;
    std::function<void()>                    onAdClosed;
    
protected:
    AdListener(AdType type);
    
    CC_SYNTHESIZE(AdType, type, Type);
    CC_SYNTHESIZE(cocos2d::Ref*, target, Target);
    SB_SYNTHESIZE_BOOL(forever, Forever);
};

#pragma mark- RewardedVideoAdListener

class RewardedVideoAdListener : public AdListener {
public:
    SB_REF_CREATE_FUNC(RewardedVideoAdListener);
    ~RewardedVideoAdListener();

public:
    typedef std::function<void(std::string/*type*/, int/*amount*/)> OnRewarded;
    OnRewarded onRewarded;
    
private:
    RewardedVideoAdListener();
    
    SB_SYNTHESIZE_BOOL(rewarded, Rewarded);
};

NS_SB_END;

#endif /* AdListener_hpp */
