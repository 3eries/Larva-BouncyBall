//
//  RewardPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/25.
//

#include "RewardPopup.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "SceneManager.h"
#include "CharacterManager.hpp"
#include "User.hpp"

USING_NS_CC;
using namespace std;

#define SLIDE_DURATION   EffectDuration::POPUP_SLIDE_FAST

RewardPopup* RewardPopup::show(const RewardItemDataList &characters) {
    
    // bg
    const int POPUP_ZORDER = ZOrder::POPUP_TOP;
    
    auto popupBg = LayerColor::create(Color::POPUP_BG);
    SceneManager::getScene()->addChild(popupBg, POPUP_ZORDER);
    
    // get_it_effect.png Vec2MC(0, -20) , Size(864, 864)
    // shine
    auto shine = Sprite::create(DIR_IMG_GET_IT + "get_it_effect.png");
    shine->setVisible(false);
    shine->setAnchorPoint(ANCHOR_M);
    shine->setPosition(Vec2MC(0, -20));
    SceneManager::getScene()->addChild(shine, POPUP_ZORDER);
    
    if( superbomb::AdsHelper::isBannerVisible() ) {
        shine->setPositionY(shine->getPositionY() + BANNER_HALF_HEIGHT);
    }
    
    // 팝업 생성
    vector<RewardPopup*> popups;
    
    for( auto chc : characters ) {
        auto popup = RewardPopup::create(chc);
        popup->setVisible(false);
        popup->setIgnoreDismissAction(true); // 퇴장 연출 OFF
        popup->setOnDismissListener([=](Node*) {
        });
        SceneManager::getScene()->addChild(popup, POPUP_ZORDER);
        
        popups.push_back(popup);
    }
    
    // 첫번째 팝업 노출
    auto firstPopup = popups[popups.size()-1];
    auto lastPopup = popups[0];
    
    firstPopup->setVisible(true);
    firstPopup->runEnterAction([=]() {
        
        for( auto popup : popups ) {
            popup->setVisible(true);
        }
        
        // shine effect - scale
        shine->setVisible(true);
        shine->setScale(0);
        
        auto scale = ScaleTo::create(0.1f, 4);
        shine->runAction(scale);
        
        // shine effect - rotate
        shine->runAction(RepeatForever::create(RotateBy::create(4.0f, 360)));
        
        // 효과음
        SBAudioEngine::playEffect(SOUND_GET_CHARACTER);
    });
    
    // 배경 등장 연출
    popupBg->setOpacity(0);
    popupBg->runAction(FadeTo::create(0.1f, Color::POPUP_BG.a));
    
    // 마지막 팝업 퇴장 연출 ON
    lastPopup->setIgnoreDismissAction(false);
    lastPopup->setOnPopupEventListener([=](Node *sender, PopupEventType eventType) {
        
        switch( eventType ) {
            case PopupEventType::EXIT_ACTION: {
                // bg
                popupBg->stopAllActions();
                popupBg->runAction(Sequence::create(FadeOut::create(0.1f), RemoveSelf::create(), nullptr));
                
                // shine
                shine->stopAllActions();
                shine->runAction(Sequence::create(FadeOut::create(0.06f), RemoveSelf::create(), nullptr));
            } break;
                
            case PopupEventType::EXIT_ACTION_FINISHED: {
                
            } break;
                
            default: break;
        }
    });
    
    return firstPopup;
}

RewardPopup* RewardPopup::create(const RewardItemData &data) {
    
    auto popup = new RewardPopup(data);
    
    if( popup && popup->init() ) {
        popup->autorelease();
        return popup;
    }
    
    CC_SAFE_DELETE(popup);
    return nullptr;
}

RewardPopup::RewardPopup(const RewardItemData &data): BasePopup(PopupType::REWARD),
data(data),
onConfirmListener(nullptr) {
}

RewardPopup::~RewardPopup() {
    
}

bool RewardPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    // 배너를 고려한 컨텐츠뷰 좌표 수정
    if( superbomb::AdsHelper::isBannerVisible() ) {
        setPositionY(getPositionY() + BANNER_HALF_HEIGHT);
    }
    
    return true;
}

void RewardPopup::onEnter() {
    
    BasePopup::onEnter();
}

bool RewardPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    // 반응 없음
    
    return true;
}

void RewardPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
}

void RewardPopup::initContentView() {
    
    BasePopup::initContentView();
    
    const bool isCharacter = data.isCharacter();
    
    // get_it_box_bg.png Vec2MC(0, -76) , Size(440, 752)
    auto bg = Sprite::create(DIR_IMG_GET_IT + "get_it_box_bg.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(Vec2MC(0, -76));
    addContentChild(bg);
    
    // 타이틀
    // get_it_title.png Vec2TC(0, -176) , Size(528, 112)
    auto title = Sprite::create(DIR_IMG_GET_IT + "get_it_title.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2TC(0, -176));
    addContentChild(title);
    
    // 캐릭터 이름 효과
    // text size:72 color:197,0,0 stroke size:8px stroke color:91,0,155 Vec2MC(0, 188) , Size(394, 61)
    string rewardName = [=]() -> string {
        if( isCharacter ) {
            return CHARACTER_MANAGER->getCharacter(data.charId).name;
        } else {
            return STR_FORMAT("WORLD %d KEY", data.world);
        }
    }();
    
    auto nameEffect = Label::createWithTTF(rewardName, FONT_SUPER_STAR, 72, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    nameEffect->setTextColor(Color4B(197,0,0,255));
    nameEffect->enableOutline(Color4B(91,0,155,255), 8);
    nameEffect->setAnchorPoint(ANCHOR_M);
    nameEffect->setPosition(Vec2MC(0, 188) + Vec2(0, -2));
    addContentChild(nameEffect);
    
    // text size:72 color:255,255,0 Vec2MC(0, 192) , Size(374, 41)
    // 캐릭터 이름
    auto name = Label::createWithTTF(rewardName, FONT_SUPER_STAR, 72, Size::ZERO,
                                     TextHAlignment::CENTER, TextVAlignment::CENTER);
    name->setTextColor(Color4B(255,255,0,255));
    name->setAnchorPoint(ANCHOR_M);
    name->setPosition(Vec2MC(0, 192) + Vec2(0, -2));
    addContentChild(name, 1);
    
    if( name->getContentSize().width > 358 ) {
        name->setScale(358 / name->getContentSize().width);
    }
    
    // FIXME: 테두리 효과가 렌더링이 안되면 좌표 오차가 생겨서 스케줄러로 우회 해결
    schedule([=](float dt) {
        this->unschedule("NAME_EFFECT");
    
        auto nameEffect2 = Label::createWithTTF(rewardName, FONT_SUPER_STAR, 72, Size::ZERO,
                                                TextHAlignment::CENTER, TextVAlignment::CENTER);
        nameEffect2->setScale(name->getScale());
        nameEffect2->setTextColor(nameEffect->getTextColor());
        nameEffect2->enableOutline(Color4B(91,0,155,255), 8);
        nameEffect2->setAnchorPoint(nameEffect->getAnchorPoint());
        nameEffect2->setPosition(nameEffect->getPosition());
        addContentChild(nameEffect2);
        
        nameEffect->removeFromParent();
        
    }, "NAME_EFFECT");
    
    // red Vec2MC(0, -100) , Size(184, 216)
    // main_icon_key.png 200% size Vec2MC(0, -76) , Size(104, 160)
    auto imageFile = [=]() -> string {
        if( isCharacter ) {
            return ResourceHelper::getCharacterImage(data.charId);
        } else {
            return DIR_IMG_MAIN + "main_icon_key.png";
        }
    }();
    
    auto image = Sprite::create(imageFile);
    image->setScale(2.0f);
    addContentChild(image);
    
    if( isCharacter ) {
        image->setAnchorPoint(ANCHOR_MB);
        image->setPosition(Vec2MC(0, -100) + Vec2(0, -216 * 0.5f));
    } else {
        image->setAnchorPoint(ANCHOR_M);
        image->setPosition(Vec2MC(0, -76));
    }
    
    // badge
    // get_it_badge_new.png Vec2MC(184, 54) , Size(208, 204)
    auto newBadge = Sprite::create(DIR_IMG_GET_IT + "get_it_badge_new.png");
    newBadge->setAnchorPoint(ANCHOR_M);
    newBadge->setPosition(Vec2MC(184, 54));
    addContentChild(newBadge);
    
    // confirm button
    // get_it_btn_thumb.png Vec2BC(0, 192) , Size(360, 144)
    auto confirmBtn = SBButton::create(DIR_IMG_GET_IT + "get_it_btn_thumb.png");
    confirmBtn->setZoomScale(ButtonZoomScale::NORMAL);
    confirmBtn->setAnchorPoint(ANCHOR_M);
    confirmBtn->setPosition(Vec2BC(getContentSize(), 0, 192));
    addContentChild(confirmBtn);
    
    confirmBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onConfirmListener);
        
        this->dismissWithAction();
    });
}

/**
 * 등장 연출
 */
void RewardPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(SLIDE_DURATION, onFinished);
    
    // 컨텐츠 slide in
    runSlideInAction([=]() {
        
        this->onEnterActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, SLIDE_DURATION);
}

/**
 * 퇴장 연출
 */
void RewardPopup::runExitAction(SBCallback onFinished) {
    
    BasePopup::runExitAction(SLIDE_DURATION, onFinished);
    
    // 컨텐츠 slide out
    runSlideOutAction([=]() {
        
        this->onExitActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, SLIDE_DURATION);
}


