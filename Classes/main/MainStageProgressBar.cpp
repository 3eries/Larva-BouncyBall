//
//  MainStageProgressBar.cpp
//  LarvaBouncyBall-mobile
//
//  Created by ace on 2022/10/13.
//

#include "MainStageProgressBar.hpp"

#include "Define.h"
#include "ResourceHelper.hpp"
#include "GameConfiguration.hpp"
#include "CharacterManager.hpp"
#include "StageManager.hpp"
#include "User.hpp"

USING_NS_CC;
using namespace std;

MainStageProgressBar::MainStageProgressBar():
onItemClickListener(nullptr) {
}

MainStageProgressBar::~MainStageProgressBar() {
    
}

bool MainStageProgressBar::init() {
    
    if( !Widget::init() ) {
        return false;
    }
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2::ZERO);
    setContentSize(SB_WIN_SIZE);
    
    // 배경
    // main_bg_progress_bar.png Vec2TC(0, -280) , Size(960, 56)
    auto bg = Sprite::create(DIR_IMG_MAIN + "main_bg_progress_bar.png");
    bg->setAnchorPoint(ANCHOR_M);
    bg->setPosition(WITH_BANNER_SIZE(Vec2TC(0, -280)));
    addChild(bg);
    
    // 진행도
    // main_green_progress_bar.png Vec2TC(108, -280) , Size(728, 40)
    progressBar = ProgressTimer::create(Sprite::create(DIR_IMG_MAIN + "main_green_progress_bar.png"));
    progressBar->setType(ProgressTimer::Type::BAR);
    progressBar->setMidpoint(ANCHOR_ML);
    progressBar->setBarChangeRate(Vec2(1, 0));
    progressBar->setAnchorPoint(ANCHOR_M);
    progressBar->setPosition(WITH_BANNER_SIZE(Vec2TC(108, -280)));
    addChild(progressBar);
    
    progressBar->setPercentage(0);
    
    // 별 개수
    // 46/54 size:56 Vec2TC(-346, -281) , Size(147, 32)
    starLabel = Label::createWithTTF("0", FONT_SUPER_STAR, 56, Size::ZERO,
                                     TextHAlignment::CENTER, TextVAlignment::CENTER);
    starLabel->setTextColor(Color4B::WHITE);
    starLabel->setAnchorPoint(ANCHOR_M);
    starLabel->setPosition(WITH_BANNER_SIZE(Vec2TC(-346, -281) + Vec2(-45, 0)));
    addChild(starLabel);
    
    auto divider = Label::createWithTTF("/", FONT_SUPER_STAR, 56, Size::ZERO,
                                        TextHAlignment::CENTER, TextVAlignment::CENTER);
    divider->setTextColor(Color4B::WHITE);
    divider->setAnchorPoint(ANCHOR_M);
    divider->setPosition(WITH_BANNER_SIZE(Vec2TC(-346, -281)));
    addChild(divider);
    
    auto maxStarLabel = Label::createWithTTF(TO_STRING(MAX_STAR_COUNT_PER_WORLD),
                                             FONT_SUPER_STAR, 56, Size::ZERO,
                                             TextHAlignment::CENTER, TextVAlignment::CENTER);
    maxStarLabel->setTextColor(Color4B::WHITE);
    maxStarLabel->setAnchorPoint(ANCHOR_M);
    maxStarLabel->setPosition(WITH_BANNER_SIZE(Vec2TC(-346, -281) + Vec2(45, 0)));
    addChild(maxStarLabel);
    
    // 보상 레이어
    rewardLayer = Node::create();
    rewardLayer->setAnchorPoint(progressBar->getAnchorPoint());
    rewardLayer->setPosition(progressBar->getPosition());
    rewardLayer->setContentSize(progressBar->getContentSize());
    progressBar->getParent()->addChild(rewardLayer);
    
    return true;
}

/**
 * 별 개수에 영향을 받는 UI를 업데이트합니다
 */
void MainStageProgressBar::updateStarUI(int star) {
 
    starLabel->setString(TO_STRING(star));
    starLabel->setTextColor([star]() -> Color4B {
        if( star <= 15 )        return Color4B(255, 0, 0, 255);
        else if( star <= 31 )   return Color4B(255, 84, 0, 255);
        else if( star <= 41 )   return Color4B(255, 216, 0, 255);
        else if( star <= 53 )   return Color4B(216, 255, 0, 255);
        else                    return Color4B(0, 255, 0, 255);
    }());
    
    // 진행도 업데이트
    float ratio = (float)star / MAX_STAR_COUNT_PER_WORLD;
    progressBar->setPercentage(ratio * 100);
    
    // 보상 아이템 업데이트
    for( auto item : rewardItems ) {
        item->updateUI(star);
    }
}

/**
 * 월드를 설정합니다
 */
void MainStageProgressBar::setWorld(int world) {
    
    const int oldStar = StageManager::getLatestMainWorldStarCount(world);
    const int star = StageManager::getWorldStarCount(world);
    
    StageManager::setLatestMainWorldStarCount(world, star);
    
    // 보상 레이어 업데이트
    rewardLayer->removeAllChildren();
    rewardItems.clear();
    
    auto addRewardItem = [=](const RewardItemData &data) -> RewardItem* {
        
        auto item = RewardItem::create(data);
        item->setAnchorPoint(ANCHOR_M);
        
        auto ratio = (float)data.requireStar / MAX_STAR_COUNT_PER_WORLD;
        auto x = MAX(progressBar->getContentSize().width * ratio, item->getContentSize().width/2);
        item->setPosition(Vec2(x, progressBar->getContentSize().height/2 + 30));
        
        rewardLayer->addChild(item);
        rewardItems.push_back(item);
        
        if( RewardManager::getStatus(data) == RewardItemStatus::UNLOCKED ) {
            item->setOnClickListener([=](RewardItemData data) {
                this->retain();
                onItemClickListener(data);
                this->release();
            });
        }
        
        return item;
    };
    
    // 월드
    if( world < GAME_CONFIG->getWorldCount() ) {
        addRewardItem(RewardManager::getWorldItem(world+1));
    }
    
    // 캐릭터
    auto characters = RewardManager::getCharacterItems(world);
    
    for( auto chc : characters ) {
        if( chc.requireStar > 0 ) {
            addRewardItem(chc);
        }
    }
    
    // 별 개수 업데이트
    const int STAR_ACTION_TAG = 200;
    stopActionByTag(STAR_ACTION_TAG);

    // 상승 연출
    if( oldStar != star ) {
        updateStarUI(oldStar);
        
        auto ff = CallFunc::create([=]() {
            CCLOG("상승 연출 시작");
        });
        auto starUp = ActionFloat::create(0.3f, oldStar, star, [=](float v) {
            this->updateStarUI((int)v);
        });
        auto seq = Sequence::create(DelayTime::create(0.3f), ff,
                                    starUp, nullptr);
        seq->setTag(STAR_ACTION_TAG);
        runAction(seq);
        
    } else {
        updateStarUI(star);
    }
}

#pragma mark- RewardItem

RewardItem* RewardItem::create(const RewardItemData &data) {
    
    auto item = new RewardItem(data);
    
    if( item && item->init() ) {
        item->autorelease();
        return item;
    }
    
    CC_SAFE_DELETE(item);
    return nullptr;
}

RewardItem::RewardItem(const RewardItemData &data):
data(data),
onClickListener(nullptr),
userStar(0),
iconOutline(nullptr) {
}

RewardItem::~RewardItem() {
}

bool RewardItem::init() {
    
    if( !Widget::init() ) {
        return false;
    }
    
    auto ITEM_SIZE = Size(148, 150);
    setContentSize(ITEM_SIZE);
    
    // 터치 이벤트
    setTouchEnabled(true);
    addClickEventListener([=](Ref*) {
        if( onClickListener ) {
            this->retain();
            onClickListener(data);
            this->updateUI(this->userStar);
            this->release();
        }
    });
    
    // 아이콘
    // main_icon_key.png Vec2MC(-2, 1) , Size(64, 92)
    // character 70% Vec2MC(-2, 1) , Size(94, 102)
    auto iconImage = [=]() -> string {
        if( data.isCharacter() ) {
            return ResourceHelper::getCharacterImage(data.charId);
        } else {
            return DIR_IMG_MAIN + "main_icon_key.png";
        }
    }();
    
    icon = superbomb::EffectSprite::create(iconImage);
    icon->setAnchorPoint(ANCHOR_M);
    icon->setPosition(Vec2MC(ITEM_SIZE, -2, 1));
    addChild(icon);
    
    int outlinePixel = 12;
    
    if( data.isCharacter() ) {
        icon->setAnchorPoint(ANCHOR_MB);
        icon->setPosition(Vec2MC(ITEM_SIZE, -2, 1) + Vec2(0, -102/2) + Vec2(0, 0));
        
        float scale = 0.8f;
        icon->setScale(scale);
        outlinePixel *= scale;
    }
    
    iconOutline = superbomb::EffectSprite::createOutline(icon, outlinePixel, Vec3(0,255,0));
    iconOutline->setAnchorPoint(ANCHOR_M);
    iconOutline->setPosition(Vec2MC(icon->getContentSize(), 0, 0));
    icon->addChild(iconOutline);
    
    // 필요 별 개수
    // main_bg_progress_small.png Vec2MC(0, -51) , Size(96, 48)
    auto starBg = Sprite::create(DIR_IMG_MAIN + "main_bg_progress_small.png");
    starBg->setAnchorPoint(ANCHOR_M);
    starBg->setPosition(Vec2MC(ITEM_SIZE, 0, -51));
    addChild(starBg);
    
    // 32 size: 40 Vec2MC(16, -50) , Size(38, 23)
    auto starLabel = Label::createWithTTF(TO_STRING(data.requireStar),
                                          FONT_SUPER_STAR, 40, Size::ZERO,
                                          TextHAlignment::CENTER, TextVAlignment::CENTER);
    starLabel->setTextColor(Color4B::WHITE);
    starLabel->setAnchorPoint(ANCHOR_M);
    starLabel->setPosition(Vec2MC(ITEM_SIZE, 16, -50) + Vec2(0, -5));
    addChild(starLabel);
    
    return true;
}

/**
 * 유저의 현재 별 개수에 따른 UI를 업데이트합니다
 */
void RewardItem::updateUI(int userStar) {
    
    this->userStar = userStar;
    
    // 초기화
    icon->setEffect(nullptr);
    iconOutline->setVisible(false);
    
    if( !RewardManager::isRewarded(data) ) {
        // 조건 미충족
        if( userStar < data.requireStar ) {
            icon->setEffect(superbomb::Effect::create("shaders/example_GreyScale.fsh"));
        }
        // 조건 충족
        else {
            iconOutline->setVisible(true);
        }
    }
}
