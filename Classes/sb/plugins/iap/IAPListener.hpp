//
//  IAPListener.hpp
//
//  Created by seongmin hwang on 2018. 7. 12..
//

#ifndef IAPListener_hpp
#define IAPListener_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"

#include "IAPDefine.h"

NS_SB_BEGIN;
NS_IAP_BEGIN;

#pragma mark- Listener

class Listener : public cocos2d::Ref {
public:
    Listener();
    virtual ~Listener();
    
    std::function<void(const Item&)>             onPurchased;      // 요청 성공
    std::function<void(std::string/*errorMsg*/)> onError;          // 요청 실패
    std::function<void()>                        onCanceled;       // 요청 취소
    std::function<void(bool)>                    onFinished;       // 요청 종료
    
    // 광고 제거 아이템 구매 or 복원 성공
    std::function<void()> onRemoveAds;
    
protected:
    CC_SYNTHESIZE(cocos2d::Ref*, target, Target);
    SB_SYNTHESIZE_BOOL(forever, Forever);
};

#pragma mark- PurchaseListener

/** @class PurchaseListener
 * @brief 결제 리스너
 */
class PurchaseListener : public Listener {
public:
    SB_REF_CREATE_FUNC(PurchaseListener);
    virtual ~PurchaseListener();
    
private:
    PurchaseListener();
};

#pragma mark- RestoreListener

/** @class RestoreListener
 * @brief 아이템 복원 리스너
 */
class RestoreListener : public Listener {
public:
    SB_REF_CREATE_FUNC(RestoreListener);
    virtual ~RestoreListener();
    
private:
    RestoreListener();
};

NS_IAP_END;
NS_SB_END;

#endif /* IAPListener_hpp */
