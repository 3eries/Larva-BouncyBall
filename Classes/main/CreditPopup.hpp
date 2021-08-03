//
//  CreditPopup.hpp
//
//  Created by seongmin hwang on 2018. 6. 28..
//

#ifndef CreditPopup_hpp
#define CreditPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "superbomb.h"

#include "BasePopup.hpp"

class CreditPopup : public BasePopup {
public:
    CREATE_FUNC(CreditPopup);
    ~CreditPopup();
    
private:
    CreditPopup();
    
    bool init() override;
    void onEnter() override;
    
    void initBackgroundView() override;
    void initCredit();
    
    void runEnterAction(SBCallback onFinished = nullptr) override;
    void onEnterActionFinished() override;
};

#endif /* CreditPopup_hpp */
