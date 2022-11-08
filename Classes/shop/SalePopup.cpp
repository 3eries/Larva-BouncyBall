//
//  SalePopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/04/25.
//

#include "SalePopup.hpp"

#include "Define.h"
#include "User.hpp"
#include "UserDefaultKey.h"
#include "GameConfiguration.hpp"
#include "SceneManager.h"
#include "CharacterManager.hpp"

USING_NS_CC;
USING_NS_SB;
using namespace cocos2d::ui;
using namespace std;

static const float FADE_DURATION    = 0.1f;
static const float SLIDE_DURATION   = EffectDuration::POPUP_SLIDE_FAST;

#define USER_DEFAULT_KEY_SALE_POPUP_OPEN_DATE        "SALE_POPUP_OPEN_DATE"

/**
 * 오늘 팝업이 오픈됐는지 반환합니다
 */
bool SalePopup::isTodayOpened() {
    
    auto openDate = USER_DEFAULT->getStringForKey(USER_DEFAULT_KEY_SALE_POPUP_OPEN_DATE, "");
    return openDate == SBDirector::getTodayDate();
}

SalePopup::SalePopup(): BasePopup(PopupType::SALE) {
}

SalePopup::~SalePopup() {
}

bool SalePopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    // 오픈 시간 저장
    USER_DEFAULT->setStringForKey(USER_DEFAULT_KEY_SALE_POPUP_OPEN_DATE, SBDirector::getTodayDate());
    USER_DEFAULT->flush();
    
    return true;
}

void SalePopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool SalePopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
    dismissWithAction();
    
    return true;
}

void SalePopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color::POPUP_BG);
}

void SalePopup::initContentView() {
    
    BasePopup::initContentView();
    
    // common_bg_super_sale.png Vec2MC(-24, 24) , Size(1168, 640)
    popupBg = Sprite::create(DIR_IMG_COMMON + "common_bg_super_sale.png");
    popupBg->setAnchorPoint(ANCHOR_M);
    popupBg->setPosition(Vec2MC(-24, 24));
    addContentChild(popupBg);
    
    auto buyBtn = SBButton::create(DIR_IMG_COMMON + "common_btn_super_sale.png");
    buyBtn->setZoomScale(ButtonZoomScale::NORMAL);
    buyBtn->setAnchorPoint(ANCHOR_M);
    buyBtn->setPosition(Vec2MC(0, -272));
    addContentChild(buyBtn);
    
    buyBtn->setOnClickListener([=](Node*) {
        this->onClickBuy();
    });
}

/**
 * 구매 버튼 클릭
 */
void SalePopup::onClickBuy() {
    
    SBDirector::getInstance()->setScreenTouchLocked(true);

    auto listener = iap::PurchaseListener::create();
    listener->setTarget(this);
    listener->onPurchased = [=](const iap::Item &item) {
        
        // 통계 이벤트
        {
            SBAnalytics::EventParams params;
            params[ANALYTICS_EVENT_PARAM_CLEARED_STAGE_COUNT] = SBAnalytics::EventParam(TO_STRING(StageManager::getClearedStageCount()));
            params[ANALYTICS_EVENT_PARAM_POPUP] = SBAnalytics::EventParam("sale");

            SBAnalytics::logEvent(ANALYTICS_EVENT_IAP_REMOVE_ADS, params);
        }
        
        // 광고 제거
        User::removeAds();
        
        this->dismissWithAction();
    };

    listener->onFinished = [=](bool result) {
        SBDirector::postDelayed(this, [=]() {
            SBDirector::getInstance()->setScreenTouchLocked(false);
        }, 0.2f);
    };

    iap::IAPHelper::purchaseRemoveAds(listener);
}

/**
 * 등장 연출
 */
void SalePopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(SLIDE_DURATION, onFinished);
    
    // 배경 fade in
    runBackgroundFadeInAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide in
    runSlideInAction([=]() {
        
        this->onEnterActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, SLIDE_DURATION);
}

/**
 * 퇴장 연출
 */
void SalePopup::runExitAction(SBCallback onFinished) {
    
    BasePopup::runExitAction(SLIDE_DURATION, onFinished);
    
    // 배경 fade out
    runBackgroundFadeOutAction(nullptr, FADE_DURATION);
    
    // 컨텐츠 slide out
    runSlideOutAction([=]() {
        
        this->onExitActionFinished();
        SB_SAFE_PERFORM_LISTENER(this, onFinished);
        
    }, SLIDE_DURATION);
}

/**
 * 등장 연출 완료
 */
void SalePopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
    
    // 배경 바깥 영역 터치 시 팝업 종료
    auto touchNode = SBNodeUtils::createTouchNode();
    addChild(touchNode);
    
    touchNode->addClickEventListener([=](Ref*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        this->dismissWithAction();
    });
    
    auto bgTouchNode = SBNodeUtils::createTouchNode();
    bgTouchNode->setAnchorPoint(popupBg->getAnchorPoint());
    bgTouchNode->setPosition(popupBg->getPosition());
    bgTouchNode->setContentSize(popupBg->getContentSize());
    addChild(bgTouchNode);
}
