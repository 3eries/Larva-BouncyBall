//
//  GetCharacterPopup.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/25.
//

#ifndef GetCharacterPopup_hpp
#define GetCharacterPopup_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "PopupManager.hpp"
#include "ContentManager.hpp"

class GetCharacterPopup: public BasePopup {
public:
    static GetCharacterPopup* create(const CharacterData &data);
    ~GetCharacterPopup();
    
private:
    GetCharacterPopup(const CharacterData &data);
    
    bool init() override;
    void onEnter() override;
    bool onBackKeyReleased() override;
    
    void initBackgroundView() override;
    void initContentView() override;
    
    void runEnterAction(SBCallback onFinished = nullptr) override;
    void onEnterActionFinished() override;
    
private:
    CharacterData data;
};


#endif /* GetCharacterPopup_hpp */
