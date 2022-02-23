//
//  CharacterCell.hpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#ifndef CharacterCell_hpp
#define CharacterCell_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "superbomb.h"
#include "CharacterManager.hpp"

class CharacterCell: public cocos2d::ui::Widget {
public:
    static CharacterCell* create(const CharacterData &data);
    ~CharacterCell();
    
private:
    CharacterCell(const CharacterData &data);
    
    bool init() override;
    
    void initLockLayer();
    void initUnlockLayer();
    
public:
    void unlock();
    void select();
    void unselect();
    
    void updateUnlockAmount();
    void updateViewAdsButton();
    
private:
    CC_SYNTHESIZE_READONLY(CharacterData, data, Data);
    CC_SYNTHESIZE(std::function<void(CharacterCell*)>, onSelectListener, OnSelectListener);
    CC_SYNTHESIZE(std::function<void(CharacterCell*)>, onViewAdsListener, OnViewAdsListener);
    
    cocos2d::Label *name;
    superbomb::EffectSprite *image;
    cocos2d::ui::Widget *unlockAmountLayer;
    
    cocos2d::ui::Widget *lockLayer;
    cocos2d::ui::Widget *unlockLayer;
    
    cocos2d::ui::Widget *selectedLayer;
    cocos2d::ui::Widget *unselectedLayer;
    
    SBButton*                viewAdsButton;
    superbomb::EffectSprite* viewAdsDisabledImage;
};

#endif /* CharacterCell_hpp */
