//
//  CharacterCell.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/24.
//

#include "CharacterCell.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "User.hpp"
#include "StageManager.hpp"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

#define CELL_SIZE       Size(384, 736)

CharacterCell* CharacterCell::create(const CharacterData &data) {
    
    auto cell = new CharacterCell(data);
    
    if( cell && cell->init() ) {
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return cell;
}

CharacterCell::CharacterCell(const CharacterData &data):
data(data),
onSelectListener(nullptr),
onViewAdsListener(nullptr),
unlockAmountLayer(nullptr),
lockLayer(nullptr), unlockLayer(nullptr) {
}

CharacterCell::~CharacterCell() {
}

bool CharacterCell::init() {
    
    if( !Widget::init() ) {
        return false;
    }
    
    setContentSize(CELL_SIZE);
    
    const bool isUnlocked = CHARACTER_MANAGER->isCharacterUnlocked(data.charId);
    const bool isSelected = CHARACTER_MANAGER->isSelectedCharacter(data.charId);
    
    auto bg = Sprite::create(DIR_IMG_SHOP + "shop_column_bg.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(getContentSize(), 0, 0));
    addChild(bg);
    
    name = Label::createWithTTF(data.name, FONT_SUPER_STAR, 65, Size::ZERO,
                                TextHAlignment::CENTER, TextVAlignment::CENTER);
    name->setTextColor(Color4B(27,27,27,255));
    name->setAnchorPoint(ANCHOR_M);
    name->setPosition(Vec2TC(getContentSize(), -1, -53) + Vec2(0, -2));
    addChild(name);
    
    if( name->getContentSize().width > 350 ) {
        name->setScale(350 / name->getContentSize().width);
    }
    
    // 캐릭터 이미지
    image = superbomb::EffectSprite::create(ResourceHelper::getCharacterImage(data.charId));
    image->setAnchorPoint(ANCHOR_M);
    image->setPosition(Vec2MC(getContentSize(), 0, 2));
    image->setScale(1.5f);
    addChild(image);
    
    // 잠금 해제 조건
    updateUnlockAmount();
    
    // 기타 UI
    initUnlockLayer();
    
    if( isUnlocked ) {
        selectedLayer->setVisible(isSelected);
        unselectedLayer->setVisible(!isSelected);
    } else {
        initLockLayer();
        unlockLayer->setVisible(false);
    }
    
    return true;
}

void CharacterCell::initLockLayer() {
    
    // 이름
    // YELLOW color:197,0,0 size:80 Vec2TC(-2, -53) , Size(230, 45)
    name->setTextColor(Color4B(197,0,0,255));
    image->setEffect(superbomb::Effect::create("shaders/example_GreyScale.fsh"));
    
    lockLayer = Widget::create();
    lockLayer->setAnchorPoint(Vec2::ZERO);
    lockLayer->setPosition(Vec2::ZERO);
    lockLayer->setContentSize(getContentSize());
    addChild(lockLayer);
    
    // VIEW ADS 버튼
    if( data.isViewAdsType() ) {
        viewAdsButton = SBButton::create(DIR_IMG_SHOP + "shop_btn_ad.png");
        viewAdsButton->setZoomScale(0);
        viewAdsButton->setAnchorPoint(ANCHOR_M);
        viewAdsButton->setPosition(Vec2BC(getContentSize(), 0, 100));
        lockLayer->addChild(viewAdsButton);
        
        viewAdsButton->setOnClickListener([=](Node*) {
            this->retain();
            onViewAdsListener(this);
            this->release();
        });
        
        viewAdsDisabledImage = superbomb::EffectSprite::create(DIR_IMG_SHOP + "shop_btn_ad.png");
        viewAdsDisabledImage->setEffect(superbomb::Effect::create("shaders/example_GreyScale.fsh"));
        viewAdsDisabledImage->setAnchorPoint(viewAdsButton->getAnchorPoint());
        viewAdsDisabledImage->setPosition(viewAdsButton->getPosition());
        viewAdsButton->getParent()->addChild(viewAdsDisabledImage);
        
        updateViewAdsButton();
        
        schedule([=](float dt) {
            this->updateViewAdsButton();
        }, 1.0f, "SCHEDULER_CHECK_AD_LOADED");
    }
    // 자물쇠
    else {
        auto locked = Sprite::create(DIR_IMG_SHOP + "shop_btn_locked.png");
        locked->setAnchorPoint(ANCHOR_M);
        locked->setPosition(Vec2BC(getContentSize(), 0, 100));
        lockLayer->addChild(locked);
    }
}

void CharacterCell::initUnlockLayer() {
    
    // 이름
    // RED color:27,27,27 size:80 Vec2TC(-1, -53) , Size(115, 45)
    unlockLayer = Widget::create();
    unlockLayer->setAnchorPoint(Vec2::ZERO);
    unlockLayer->setPosition(Vec2::ZERO);
    unlockLayer->setContentSize(getContentSize());
    addChild(unlockLayer);
    
    // SELECTED
    {
        selectedLayer = Widget::create();
        selectedLayer->setAnchorPoint(Vec2::ZERO);
        selectedLayer->setPosition(Vec2::ZERO);
        selectedLayer->setContentSize(getContentSize());
        unlockLayer->addChild(selectedLayer);
        
        // shop_btn_selected.png Vec2BC(0, 100) , Size(320, 144)
        auto selected = Sprite::create(DIR_IMG_SHOP + "shop_btn_selected.png");
        selected->setAnchorPoint(ANCHOR_M);
        selected->setPosition(Vec2BC(getContentSize(), 0, 100));
        selectedLayer->addChild(selected);
        
        // shop_icon_check.png Vec2BC(116, 180) , Size(104, 80)
        auto check = Sprite::create(DIR_IMG_SHOP + "shop_icon_check.png");
        check->setAnchorPoint(ANCHOR_M);
        check->setPosition(Vec2BC(getContentSize(), 116, 180));
        selectedLayer->addChild(check);
    }

    // UNSELECTED
    {
        unselectedLayer = Widget::create();
        unselectedLayer->setAnchorPoint(Vec2::ZERO);
        unselectedLayer->setPosition(Vec2::ZERO);
        unselectedLayer->setContentSize(getContentSize());
        unlockLayer->addChild(unselectedLayer);
        
        auto selectBtn = SBButton::create(DIR_IMG_SHOP + "shop_btn_select.png");
        selectBtn->setZoomScale(ButtonZoomScale::NORMAL);
        selectBtn->setAnchorPoint(ANCHOR_M);
        selectBtn->setPosition(Vec2BC(getContentSize(), 0, 100));
        unselectedLayer->addChild(selectBtn);
        
        selectBtn->setOnClickListener([=](Node*) {
            this->retain();
            SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
            onSelectListener(this);
            this->release();
        });
    }
}

void CharacterCell::unlock() {
    
    SB_SAFE_HIDE(lockLayer);
    SB_SAFE_SHOW(unlockLayer);
    
    name->setTextColor(Color4B(27,27,27,255));
    image->setEffect(nullptr);
    updateUnlockAmount();
    
    unselect();
}

void CharacterCell::select() {
 
    selectedLayer->setVisible(true);
    unselectedLayer->setVisible(false);
}

void CharacterCell::unselect() {
 
    selectedLayer->setVisible(false);
    unselectedLayer->setVisible(true);
}

/**
 * 잠금 해제 수량 업데이트
 */
void CharacterCell::updateUnlockAmount() {

    const bool isUnlocked = CHARACTER_MANAGER->isCharacterUnlocked(data.charId);
    
    if( unlockAmountLayer ) {
        unlockAmountLayer->removeFromParent();
        unlockAmountLayer = nullptr;
    }
    
    unlockAmountLayer = Widget::create();
    unlockAmountLayer->setAnchorPoint(Vec2::ZERO);
    unlockAmountLayer->setPosition(Vec2::ZERO);
    unlockAmountLayer->setContentSize(getContentSize());
    addChild(unlockAmountLayer);
    
    // 디폴트 캐릭터 체크
    if( data.unlockType == CharacterUnlockType::DEFAULT ) {
        // - Vec2TC(0, -137) , Size(20, 6)
        auto none = Label::createWithTTF("-", FONT_SUPER_STAR, 52, Size::ZERO,
                                         TextHAlignment::CENTER, TextVAlignment::CENTER);
        none->setTextColor(Color4B(255,255,255,255));
        none->setAnchorPoint(ANCHOR_M);
        none->setPosition(Vec2TC(getContentSize(), 0, -137) + Vec2(0, -2));
        unlockAmountLayer->addChild(none);
        
        return;
    }
    
    string unlockIconFile;
    string unlockAmountStr;
    
    auto getAmount = [=](int current) -> int {
        return isUnlocked ? data.unlockAmount : current;
    };
    
    switch( data.unlockType ) {
        case CharacterUnlockType::SAUSAGE: {
            unlockIconFile = DIR_IMG_SHOP + "shop_icon_sausage.png";
            unlockAmountStr = STR_FORMAT("%d/%d",
                                         getAmount(StageManager::getStageStarTotalCount()),
                                         data.unlockAmount);
        } break;

        case CharacterUnlockType::STAGE: {
            // shop_icon_stage.png Vec2TC(-122, -136) , Size(60, 52)
            // World 4-18 color:197,0,0 size:52 Vec2TC(36, -135) , Size(232, 29)
            unlockIconFile = DIR_IMG_SHOP + "shop_icon_stage.png";
            unlockAmountStr = STR_FORMAT("WORLD %d-%d",
                                         getAmount(StageManager::getWorld(data.unlockAmount)),
                                         data.unlockAmount);
        } break;

        case CharacterUnlockType::VIEW_ADS: {
            // shop_icon_ad.png Vec2TC(-55, -136) , Size(64, 44)
            // 3/10 color:197,0,0 size:52 Vec2TC(39, -135) , Size(96, 29)
            unlockIconFile = DIR_IMG_SHOP + "shop_icon_ad.png";
            unlockAmountStr = STR_FORMAT("%d/%d",
                                         getAmount(CHARACTER_MANAGER->getViewAdsCount(data.charId)),
                                         data.unlockAmount);
        } break;
            
        default: break;
    }
    
    // shop_icon_sausage.png Vec2TC(-88, -136) , Size(64, 52)
    auto unlockIcon = Sprite::create(unlockIconFile);
    unlockIcon->setAnchorPoint(ANCHOR_ML);
    unlockIcon->setPosition(Vec2TL(getContentSize(), 0, -136));
    unlockAmountLayer->addChild(unlockIcon);
    
    // 88/688 color:255,255,255 size:52 Vec2TC(39, -135) , Size(161, 29)
    auto unlockAmount = Label::createWithTTF(unlockAmountStr, FONT_SUPER_STAR, 52, Size::ZERO,
                                             TextHAlignment::LEFT, TextVAlignment::CENTER);
    unlockAmount->setTextColor(isUnlocked ? Color4B(255,255,255,255) : Color4B(197,0,0,255));
    unlockAmount->setAnchorPoint(ANCHOR_ML);
    unlockAmount->setPosition(Vec2(unlockIcon->getPositionX() + unlockIcon->getContentSize().width + 14,
                                   unlockIcon->getPositionY()) +
                              Vec2(0, -2));
    unlockAmountLayer->addChild(unlockAmount);
    
    // 가운데 정렬
    float w = SB_BOUNDING_BOX_IN_WORLD(unlockAmount).getMaxX() - SB_BOUNDING_BOX_IN_WORLD(unlockIcon).getMinX();
    float diff = (getContentSize().width - w) / 2;
    
    unlockIcon->setPositionX(unlockIcon->getPositionX() + diff);
    unlockAmount->setPositionX(unlockAmount->getPositionX() + diff);
}

/**
 * 광고 보기 버튼 업데이트
 */
void CharacterCell::updateViewAdsButton() {

    if( !data.isViewAdsType() ) {
        return;
    }
    
    if( CHARACTER_MANAGER->isCharacterUnlocked(data.charId) ) {
        return;
    }
    
    const bool isAdLoaded = superbomb::AdsHelper::isRewardedVideoLoaded();
    
    viewAdsButton->setVisible(isAdLoaded);
    viewAdsDisabledImage->setVisible(!isAdLoaded);
}
