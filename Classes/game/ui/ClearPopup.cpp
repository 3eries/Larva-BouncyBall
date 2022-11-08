//
//  ClearPopup.cpp
//  LarvaBouncyBall-mobile
//
//  Created by seongmin hwang on 2022/01/21.
//

#include "ClearPopup.hpp"

#include "Define.h"
#include "GameUIHelper.hpp"
#include "ResourceHelper.hpp"
#include "User.hpp"

USING_NS_CC;
using namespace std;

ClearPopup::ClearPopup(): BasePopup(PopupType::GAME_CLEAR),
onStarEffectFinishedListener(nullptr),
onShopListener(nullptr),
onHomeListener(nullptr),
onRetryListener(nullptr),
onNextListener(nullptr),
onStarRewardedListener(nullptr) {
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
    runStarAnimation(0);
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
    
    auto stage = GAME_MANAGER->getStage();
    
    // 현재 스테이지
    // world/stage info size:70 Vec2TC(-2, -102) , Size(450, 39)
    auto stageLabel = Label::createWithTTF(STR_FORMAT("WORLD %d - %d", stage.world, stage.stage),
                                           FONT_SUPER_STAR, 70, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    stageLabel->setTextColor(Color4B::WHITE);
    stageLabel->setAnchorPoint(ANCHOR_M);
    stageLabel->setPosition(WITH_BANNER_SIZE(Vec2TC(0, -102)));
    addContentChild(stageLabel);
    
    // 타이틀
    auto title = Sprite::create(DIR_IMG_RESULT + "result_text_title.png");
    title->setAnchorPoint(ANCHOR_M);
    title->setPosition(WITH_BANNER_SIZE(Vec2TC(-2, -248)));
    addContentChild(title);
    
    // 별
    Vec2 pos[] = {
        Vec2MC(-360, -6),
        Vec2MC(0, 30),
        Vec2MC(360, -6),
    };
    
    for( int i = 0; i < 3; ++i ) {
        auto starBg = Sprite::create(DIR_IMG_RESULT + "result_star_grey.png");
        starBg->setAnchorPoint(ANCHOR_M);
        starBg->setPosition(WITH_BANNER_SIZE(pos[i]));
        addContentChild(starBg);
        
        starBgs.push_back(starBg);
    }
    
    // Shop
    auto shopBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_shop.png");
    shopBtn->setZoomScale(ButtonZoomScale::NORMAL);
    shopBtn->setAnchorPoint(ANCHOR_M);
    shopBtn->setPosition(Vec2TL(130, -112));
    addContentChild(shopBtn);
    
    shopBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onShopListener);
    });
    
    // 상점 버튼 - 캐릭터 로테이션 연출
    auto chcImage = Sprite::create();
    chcImage->setAnchorPoint(ANCHOR_M);
    chcImage->setPosition(shopBtn->getContentsLayer()->convertToNodeSpace(Vec2TL(130, -39)));
    shopBtn->getContentsLayer()->addChild(chcImage);

    StringList charImageList;

    for( int i = 0; i < 8; ++i ) {
        charImageList.push_back(ResourceHelper::getCharacterImage(CHARACTER_MANAGER->getCharacters()[i].charId));
    }

    random_shuffle(charImageList.begin(), charImageList.end());

    auto anims = SBNodeUtils::createAnimation(charImageList, 0.4f);
    chcImage->runAction(RepeatForever::create(Animate::create(anims)));
    
    // 상점 버튼 - 타이틀
    auto shopTitle = Sprite::create(DIR_IMG_RESULT + "result_btn_shop_text.png");
    shopTitle->setAnchorPoint(ANCHOR_M);
    shopTitle->setPosition(shopBtn->getContentsLayer()->convertToNodeSpace(Vec2TL(128, -142)));
    shopBtn->getContentsLayer()->addChild(shopTitle);
    
    // Home
    auto homeBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_home.png");
    homeBtn->setZoomScale(ButtonZoomScale::NORMAL);
    homeBtn->setAnchorPoint(ANCHOR_M);
    homeBtn->setPosition(WITH_BANNER_SIZE(Vec2BC(-444, 232)));
    addContentChild(homeBtn);
    
    homeBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onHomeListener);
    });
    
    // Retry
    auto retryBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_retry.png");
    retryBtn->setZoomScale(ButtonZoomScale::NORMAL);
    retryBtn->setAnchorPoint(ANCHOR_M);
    retryBtn->setPosition(WITH_BANNER_SIZE(Vec2BC(-92, 232)));
    addContentChild(retryBtn);
    
    retryBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onRetryListener);
    });
    
    // Next Stage
    auto nextBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_next_stage.png");
    nextBtn->setZoomScale(ButtonZoomScale::NORMAL);
    nextBtn->setAnchorPoint(ANCHOR_M);
    nextBtn->setPosition(WITH_BANNER_SIZE(Vec2BC(352, 232)));
    addContentChild(nextBtn);
    
    nextBtn->setOnClickListener([=](Node*) {
        SBAudioEngine::playEffect(SOUND_BUTTON_CLICK);
        SB_SAFE_PERFORM_LISTENER(this, onNextListener);
    });
    
    // 광고로 별 3개 얻기
    if( StageManager::getStageStarCount(stage.stage) < 3 &&
        superbomb::AdsHelper::isRewardedVideoLoaded() ) {
        auto bg = Sprite::create(DIR_IMG_RESULT + "result_bg_all_star.png");
        bg->setAnchorPoint(ANCHOR_M);
        bg->setPosition(WITH_BANNER_SIZE(Vec2TR(-204, -382)));
        addContentChild(bg);
        
        auto adBtn = SBButton::create(DIR_IMG_RESULT + "result_btn_all_star_ad.png");
        adBtn->setZoomScale(0);
        adBtn->setAnchorPoint(ANCHOR_M);
        adBtn->setPosition(WITH_BANNER_SIZE(Vec2TR(-204, -512)));
        addContentChild(adBtn);
        
        adBtn->setOnClickListener([=](Node*) {
            // 광고 노출
            SBDirector::getInstance()->setScreenTouchLocked(true);
            
            auto listener = superbomb::RewardedVideoAdListener::create();
            listener->setTarget(this);
            listener->onAdOpened = [=]() {
                SBDirector::getInstance()->setScreenTouchLocked(false);
            };
            listener->onAdClosed = [=]() {
                if( !listener->isRewarded() ) {
                    return;
                }
                
                // 통계 이벤트
                {
                    // sb_ad_reward
                    SBAnalytics::EventParams params;
                    params[ANALYTICS_EVENT_PARAM_TYPE] = SBAnalytics::EventParam("game_result");
                    
                    SBAnalytics::logEvent(ANALYTICS_EVENT_SB_AD_REWARD, params);
                    
                    // game_result_get_star
                    params.clear();
                    
                    params[ANALYTICS_EVENT_PARAM_STAGE] = SBAnalytics::EventParam(TO_STRING(stage.stage));
                    params[ANALYTICS_EVENT_PARAM_STAGE_RANGE] = SBAnalytics::EventParam(SBAnalytics::getNumberRange(stage.stage, 1, 5, 5));
                    
                    SBAnalytics::logEvent(ANALYTICS_EVENT_GAME_RESULT_GET_STAR, params);
                }
                
                SB_SAFE_PERFORM_LISTENER(this, onStarRewardedListener);
            };
            superbomb::AdsHelper::getInstance()->showRewardedVideo(listener);
        });
    }
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


