//
//  ReviewPopup.hpp
//
//  Created by seongmin hwang on 2018. 8. 7..
//

#ifndef ReviewPopup_hpp
#define ReviewPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BasePopup.hpp"

/** @class ReviewPopup
 * @brief '리뷰 남겨주세요' 팝업
 */
class ReviewPopup : public BasePopup {
public:
    CREATE_FUNC(ReviewPopup);
    ~ReviewPopup();
    
private:
    ReviewPopup();
    
    bool init() override;
    
    void onEnter() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void runEnterAction(float duration, SBCallback onFinished) override;
    void runEnterAction(SBCallback onFinished = nullptr) override;
    
    void runExitAction(float duration, SBCallback onFinished) override;
    void runExitAction(SBCallback onFinished = nullptr) override;
    
    void onEnterActionFinished() override;
    
private:
    CC_SYNTHESIZE(SBCallback, onGoListener, OnGoListener);
    cocos2d::Node *stoneBg;
};

#endif /* ReviewPopup_hpp */
