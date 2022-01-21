//
//  ClearPopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#ifndef ClearPopup_hpp
#define ClearPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "PopupManager.hpp"
#include "GameManager.hpp"

class ClearPopup: public BasePopup {
public:
    CREATE_FUNC(ClearPopup);
    ~ClearPopup();
    
private:
    ClearPopup();
    
    bool init() override;
};

#endif /* ClearPopup_hpp */
