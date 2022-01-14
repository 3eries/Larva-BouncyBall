//
//  StageProgressBar.cpp
//
//  Created by seongmin hwang on 19/08/2019.
//

#include "StageProgressBar.hpp"

#include "Define.h"

USING_NS_CC;
using namespace std;

static const string SCHEDULER_TOGGLE_GAGE_TEXT = "SCHEDULER_TOGGLE_GAGE_TEXT";

StageProgressBar::StageProgressBar() {
}

StageProgressBar::~StageProgressBar() {
}

bool StageProgressBar::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2::ZERO);
    setContentSize(SB_WIN_SIZE);
    
    initUI();
    initGameListener();
    
    return true;
}

void StageProgressBar::onEnter() {
    
    Node::onEnter();
    
    onStageChanged(GAME_MANAGER->getStage());
}

void StageProgressBar::initUI() {
    
    // game_gage_level.png Vec2TC(0, -57) , Size(440, 44)
    auto gageBg = Sprite::create(DIR_IMG_GAME + "game_gage_level.png");
    gageBg->setAnchorPoint(ANCHOR_M);
    gageBg->setPosition(Vec2TC(0, -57));
    addChild(gageBg);
    
    gage = ProgressTimer::create(Sprite::create(DIR_IMG_GAME + "game_gage_level.png"));
    gage->setType(ProgressTimer::Type::BAR);
    gage->setColor(Color3B(46, 196, 182));
    gage->setMidpoint(Vec2(0,0));
    gage->setBarChangeRate(Vec2(1, 0));
    gage->setAnchorPoint(ANCHOR_M);
    gage->setPosition(Vec2MC(gageBg->getContentSize(), 0, 0));
    gage->setPercentage(0);
    gageBg->addChild(gage);
    
    // LEVEL 13 size:39 Vec2TC(0, -58) , Size(164, 28)
    levelLabel = Label::createWithTTF("LEVEL 1", FONT_ROBOTO_BLACK, 39, Size::ZERO,
                                      TextHAlignment::CENTER, TextVAlignment::CENTER);
    levelLabel->setTextColor(Color4B::BLACK);
    levelLabel->setAnchorPoint(ANCHOR_M);
    levelLabel->setPosition(Vec2MC(gageBg->getContentSize(), 0, 0));
    gageBg->addChild(levelLabel);
    
    clearCountLabel = Label::createWithTTF("0 / 0", FONT_ROBOTO_BLACK, 39, Size::ZERO,
                                           TextHAlignment::CENTER, TextVAlignment::CENTER);
    clearCountLabel->setVisible(false);
    clearCountLabel->setTextColor(Color4B::BLACK);
    clearCountLabel->setAnchorPoint(ANCHOR_M);
    clearCountLabel->setPosition(Vec2MC(gageBg->getContentSize(), 0, 0));
    gageBg->addChild(clearCountLabel);
}

void StageProgressBar::initGameListener() {
    
    GameEventList events({
        GameEvent::STAGE_CHANGED,
        GameEvent::STAGE_RESTART,
        GameEvent::STAGE_CLEAR,
        GameEvent::MOVE_NEXT_STAGE,
        GameEvent::MOVE_NEXT_STAGE_FINISHED,
    });
    
    GameManager::addEventListener(events, [=](GameEvent event, void *userData) {
        
        switch( event ) {
            case GameEvent::STAGE_CHANGED: {
                auto stage = (StageData*)userData;
                this->onStageChanged(*stage);
            } break;
                
            case GameEvent::STAGE_RESTART: {
                auto stage = (StageData*)userData;
                this->onStageRestart(*stage);
            } break;
                
            case GameEvent::STAGE_CLEAR: {
                auto stage = (StageData*)userData;
                this->onStageClear(*stage);
            } break;
                
            case GameEvent::MOVE_NEXT_STAGE:            this->onMoveNextStage();          break;
            case GameEvent::MOVE_NEXT_STAGE_FINISHED:   this->onMoveNextStageFinished();  break;
                
            default: break;
        }
    }, this);
}

void StageProgressBar::startToggleGageTextScheduler() {
    
    stopToggleGageTextScheduler();
    
    schedule([=](float dt) {
        
        levelLabel->setVisible(!levelLabel->isVisible());
        clearCountLabel->setVisible(!levelLabel->isVisible());
        
    }, 3, SCHEDULER_TOGGLE_GAGE_TEXT);
}

void StageProgressBar::stopToggleGageTextScheduler() {
    
    unschedule(SCHEDULER_TOGGLE_GAGE_TEXT);
    
    levelLabel->setVisible(true);
    clearCountLabel->setVisible(false);
}

void StageProgressBar::setPercentage(float percentage, bool withAction) {
    
    gage->stopAllActions();
    
    if( withAction ) {
        auto updatePercentage = [=](float f) {
            gage->setPercentage(f);
        };
        auto numberAction = ActionFloat::create(0.2f, gage->getPercentage(), percentage,
                                                updatePercentage);
        gage->runAction(numberAction);
        
    } else {
        gage->setPercentage(percentage);
    }
}

void StageProgressBar::setClearCount(int clearCount, int clearCondition) {
 
    clearCountLabel->setString(STR_FORMAT("%d / %d", clearCount, clearCondition));
}

/**
 * 스테이지 변경
 */
void StageProgressBar::onStageChanged(const StageData &stage) {
    
    setPercentage(0);
    setClearCount(0, stage.clearCondition);
    
    levelLabel->setString(STR_FORMAT("STAGE %d", stage.stage));
    startToggleGageTextScheduler();
}

/**
 * 스테이지 재시작
 */
void StageProgressBar::onStageRestart(const StageData &stage) {
    
    setPercentage(0);
}

/**
 * 스테이지 클리어
 */
void StageProgressBar::onStageClear(const StageData &stage) {
    
    stopToggleGageTextScheduler();
}

/**
 * 다음 스테이지로 이동
 */
void StageProgressBar::onMoveNextStage() {
    
    auto updatePercentage = [=](float f) {
        gage->setPercentage(f);
    };
    auto numberAction = ActionFloat::create(MOVE_NEXT_LEVEL_DURATION, 100, 0, updatePercentage);
    gage->runAction(numberAction);
}

/**
 * 다음 스테이지로 이동 완료
 */
void StageProgressBar::onMoveNextStageFinished() {
}

