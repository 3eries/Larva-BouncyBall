//
//  ShopPopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#ifndef ShopPopup_hpp
#define ShopPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "PopupManager.hpp"

class CharacterCell;

class ShopPopup: public BasePopup {
public:
    CREATE_FUNC(ShopPopup);
    ~ShopPopup();
    
private:
    ShopPopup();
    
    bool init() override;
    void onEnter() override;
    bool onBackKeyReleased() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void runEnterAction(SBCallback onFinished = nullptr) override;
    void onEnterActionFinished() override;
};

#endif /* ShopPopup_hpp */
