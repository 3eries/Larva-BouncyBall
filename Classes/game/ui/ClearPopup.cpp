//
//  ClearPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#include "ClearPopup.hpp"

#include "Define.h"
#include "GameUIHelper.hpp"

USING_NS_CC;
using namespace std;

ClearPopup::ClearPopup(): BasePopup(PopupType::GAME_CLEAR),
onStarEffectFinishedListener(nullptr),
onShopListener(nullptr),
onHomeListener(nullptr),
onRetryListener(nullptr),
onNextListener(nullptr) {
}

ClearPopup::~ClearPopup() {
    
}

bool ClearPopup::init() {
    
    if( !BasePopup::init() ) {
        return false;
    }
    
    return true;
}

void ClearPopup::onEnter() {
    
    BasePopup::onEnter();
    
    runEnterAction();
}

bool ClearPopup::onBackKeyReleased() {
    
    if( !BasePopup::onBackKeyReleased() ) {
        return false;
    }
    
    // 반응 없음
    
    return true;
}

void ClearPopup::initBackgroundView() {
    
    BasePopup::initBackgroundView();
    
    setBackgroundColor(Color4B(0,0,0,255*0.9f));
}

void ClearPopup::initContentView() {
    
    BasePopup::initContentView();
    
    // 타이틀
    auto title = Sprite::create(DIR_IMG_RESULT + "result_text_title.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(Vec2TC(-2, -248));
    addContentChild(title);
    
    // 별
    Vec2 pos[] = {
        Vec2MC(-360, -22),
        Vec2MC(0, 14),
        Vec2MC(360, -22),
    };
    
    for( int i = 0; i < 3; ++i ) {
        auto starBg = Sprite::create(DIR_IMG_RESULT + "result_star_grey.png");
        starBg->setAnchorPoint(ANCHOR_M);
        starBg->setPosition(pos[i]);
        addContentChild(starBg);
        
        starBgs.push_back(starBg);
    }
    
    runStarAnimation(0);
    
    // Shop
    auto shopBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_shop.png");
    shopBtn->setZoomScale(ButtonZoomScale::NORMAL);
    shopBtn->setAnchorPoint(ANCHOR_M);
    shopBtn->setPosition(Vec2TL(130, -102));
    addContentChild(shopBtn);
    
    shopBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onShopListener);
    });
    
    // Home
    auto homeBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_home.png");
    homeBtn->setZoomScale(ButtonZoomScale::NORMAL);
    homeBtn->setAnchorPoint(ANCHOR_M);
    homeBtn->setPosition(Vec2BC(-444, 192));
    addChild(homeBtn);
    
    homeBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onHomeListener);
    });
    
    // Retry
    auto retryBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_retry.png");
    retryBtn->setZoomScale(ButtonZoomScale::NORMAL);
    retryBtn->setAnchorPoint(ANCHOR_M);
    retryBtn->setPosition(Vec2BC(-92, 192));
    addChild(retryBtn);
    
    retryBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onRetryListener);
    });
    
    // Next Stage
    auto nextBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_next_stage.png");
    nextBtn->setZoomScale(ButtonZoomScale::NORMAL);
    nextBtn->setAnchorPoint(ANCHOR_M);
    nextBtn->setPosition(Vec2BC(352, 192));
    addChild(nextBtn);
    
    nextBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onNextListener);
    });
}

void ClearPopup::runStarAnimation(int i) {
    
    SBAudioEngine::playEffect(SOUND_GAME_CLEAR_STAR);
    
    // 연출이 진행되는 동안 터치 방지
    SBDirector::getInstance()->setScreenTouchLocked(true);
    
    auto starBg = starBgs[i];
    
    auto star = SBSkeletonAnimation::create(DIR_IMG_RESULT + "result_star.json");
    star->setPosition(Vec2MC(starBg->getContentSize(), 0, 0));
    starBg->addChild(star);
    
    star->runAnimation("start", false, [=](spine::TrackEntry*) {
        star->setAnimation(0, "idle", true);
    });
    
    auto particle = ParticleSystemQuad::create(DIR_IMG_RESULT + "result_star_particle.plist");
    particle->setAnchorPoint(ANCHOR_M);
    particle->setPosition(Vec2MC(starBg->getContentSize(), 0, 0));
    starBg->addChild(particle);
    
    // 마지막 별이었는지 체크
    if( i == GAME_MANAGER->getStar()-1 ) {
        // 터치 재개
        SBDirector::getInstance()->setScreenTouchLocked(false);
        SB_SAFE_PERFORM_LISTENER(this, onStarEffectFinishedListener);
    }
    // 다음 별 애니메이션
    else {
        auto delay = DelayTime::create(0.3f);
        auto callFunc = CallFunc::create([=]() {
            this->runStarAnimation(i+1);
        });
        runAction(Sequence::create(delay, callFunc, nullptr));
    }
}

/**
 * 등장 연출
 */
void ClearPopup::runEnterAction(SBCallback onFinished) {
    
    BasePopup::runEnterAction(0/* ACTION DURATION */, onFinished);
}

/**
 * 등장 연출 완료
 */
void ClearPopup::onEnterActionFinished() {
    
    BasePopup::onEnterActionFinished();
}


