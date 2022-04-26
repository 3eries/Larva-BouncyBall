//
//  SalePopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/04/25.
//

#ifndef SalePopup_hpp
#define SalePopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BasePopup.hpp"

class SalePopup : public BasePopup {
public:
    CREATE_FUNC(SalePopup);
    ~SalePopup();
    
private:
    SalePopup();
    
    bool init() override;
    void onEnter() override;
    bool onBackKeyReleased() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void onClickBuy();
    
    void runEnterAction(SBCallback onFinished = nullptr) override;
    void runExitAction(SBCallback onFinished = nullptr) override;
    
    void onEnterActionFinished() override;
    
private:
    cocos2d::Sprite *popupBg;
};

#endif /* SalePopup_hpp */
