//
//  GetCharacterPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/25.
//

#include "GetCharacterPopup.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "SceneManager.h"

USING_NS_CC;
using namespace std;

#define SLIDE_DURATION   EffectDuration::POPUP_SLIDE_FAST

void GetCharacterPopup::show(const CharacterDataList &characters) {
    
    // 통계 이벤트
    for( auto chc : characters ) {
        SBAnalytics::EventParams params;
        params[ANALYTICS_EVENT_PARAM_CHAR_ID] = SBAnalytics::EventParam(chc.charId);

        SBAnalytics::logEvent(ANALYTICS_EVENT_CHARACTER_GET, params);
    }
    
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
    
    // powder
//    auto powder = SBSkeletonAnimation::create(DIR_IMG_COMMON + "get_powder.json");
//    powder->setAnchorPoint(Vec2::ZERO);
//    powder->setPosition(Vec2MC(0,0));
//    powder->setVisible(false);
//    SceneManager::getScene()->addChild(powder, POPUP_ZORDER);
    
    // 팝업 생성
    vector<GetCharacterPopup*> popups;
    
    for( auto chc : characters ) {
        auto popup = GetCharacterPopup::create(chc);
        popup->setVisible(false);
        popup->setIgnoreDismissAction(true); // 퇴장 연출 OFF
        popup->setOnDismissListener([=](Node*) {
    
//            // 노출된 팝업 제거
//            auto &getCharacterPopups = instance->getCharacterPopups;
//            getCharacterPopups.erase(getCharacterPopups.begin());
//
//            if( getCharacterPopups.size() == 0 ) {
//                // 마지막 팝업 퇴장 완료
//                return;
//            }
//
//            // 다음 팝업 노출
//            auto nextPopup = getCharacterPopups[0];
//            nextPopup->setVisible(true);
//            nextPopup->onEnterActionFinished();
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
        
        // powder
//        powder->setVisible(true);
//        powder->setAnimation(0, ANIM_NAME_RUN, true);
        
        // 효과음
        SBAudioEngine::playEffect(DIR_SOUND + "fanfare.mp3");
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
                
                // powder
                // powder->runAction(Sequence::create(FadeOut::create(0.06f), RemoveSelf::create(), nullptr));
            } break;
                
            case PopupEventType::EXIT_ACTION_FINISHED: {
                
            } break;
                
            default: break;
        }
    });
}

GetCharacterPopup* GetCharacterPopup::create(const CharacterData &data) {
    
    auto popup = new GetCharacterPopup(data);
    
    if( popup && popup->init() ) {
        popup->autorelease();
        return popup;
    }
    
    CC_SAFE_DELETE(popup);
    return nullptr;
}

GetCharacterPopup::GetCharacterPopup(const CharacterData &data): BasePopup(PopupType::GET_CHARACTER),
data(data),
onConfirmListener(nullptr) {
}

GetCharacterPopup::~GetCharacterPopup() {
    
}

bool GetCharacterPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void GetCharacterPopup::onEnter() {
    
    BasePopup::onEnter();
}

bool GetCharacterPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    // 반응 없음
    
    return true;
}

void GetCharacterPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
}

void GetCharacterPopup::initContentView() {
    
    BasePopup::initContentView();
    
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
    auto nameEffect = Label::createWithTTF(data.name, FONT_SUPER_STAR, 72, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    nameEffect->setTextColor(Color4B(197,0,0,255));
    nameEffect->enableOutline(Color4B(91,0,155,255), 8);
    nameEffect->setAnchorPoint(ANCHOR_M);
    nameEffect->setPosition(Vec2MC(0, 188) + Vec2(0, -2));
    addContentChild(nameEffect);
    
    // text size:72 color:255,255,0 Vec2MC(0, 192) , Size(374, 41)
    // 캐릭터 이름
    auto name = Label::createWithTTF(data.name, FONT_SUPER_STAR, 72, Size::ZERO,
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
    
        auto nameEffect2 = Label::createWithTTF(data.name, FONT_SUPER_STAR, 72, Size::ZERO,
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
    auto image = Sprite::create(ResourceHelper::getCharacterImage(data.charId));
    image->setAnchorPoint(ANCHOR_MB);
    image->setPosition(Vec2MC(0, -100) + Vec2(0, -216 * 0.5f));
    image->setScale(2.0f);
    addContentChild(image);
    
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
void GetCharacterPopup::runEnterAction(SBCallback onFinished) {
    
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
void GetCharacterPopup::runExitAction(SBCallback onFinished) {
    
    BasePopup::runExitAction(SLIDE_DURATION, onFinished);
    
    // 컨텐츠 slide out
    runSlideOutAction([=]() {
        
        this->onExitActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, SLIDE_DURATION);
}


