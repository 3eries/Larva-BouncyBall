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
#include "CharacterManager.hpp"

class GetCharacterPopup: public BasePopup {
public:
    static void show(const CharacterDataList &characters);
    
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
    void runExitAction(SBCallback onFinished = nullptr) override;
    
private:
    CharacterData data;
    CC_SYNTHESIZE(SBCallback, onConfirmListener, OnConfirmListener);
};


#endif /* GetCharacterPopup_hpp */
