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

void StageProgressBar::initUI() {
    
    auto starBg = LayerColor::create(Color4B(0,0,0,255*0.9f));
    starBg->setIgnoreAnchorPointForPosition(false);
    starBg->setAnchorPoint(ANCHOR_MT);
    starBg->setPosition(Vec2TC(0, 0));
    starBg->setContentSize(Size(400, 120));
    addChild(starBg);
    
    Vec2 pos[] = {
        Vec2TC(-150, -6),
        Vec2TC(0, -6),
        Vec2TC(150, -6),
    };
    
    for( int i = 0; i < 3; ++i ) {
        auto star = Sprite::create(DIR_IMG_GAME + "game_top_sausage.png");
        star->setVisible(false);
        star->setAnchorPoint(ANCHOR_MT);
        star->setPosition(pos[i]);
        addChild(star);
        
        stars.push_back(star);
    }
}

void StageProgressBar::initGameListener() {
    
//    GameEventList events({
//        GameEvent::STAGE_CHANGED,
//        GameEvent::STAGE_RESTART,
//        GameEvent::STAGE_CLEAR,
//        GameEvent::MOVE_NEXT_STAGE,
//        GameEvent::MOVE_NEXT_STAGE_FINISHED,
//    });
//
//    GameManager::addEventListener(events, [=](GameEvent event, void *userData) {
//
//        switch( event ) {
//            case GameEvent::STAGE_CHANGED: {
//                auto stage = (StageData*)userData;
//                this->onStageChanged(*stage);
//            } break;
//
//            case GameEvent::STAGE_RESTART: {
//                auto stage = (StageData*)userData;
//                this->onStageRestart(*stage);
//            } break;
//
//            case GameEvent::STAGE_CLEAR: {
//                auto stage = (StageData*)userData;
//                this->onStageClear(*stage);
//            } break;
//
//            case GameEvent::MOVE_NEXT_STAGE:            this->onMoveNextStage();          break;
//            case GameEvent::MOVE_NEXT_STAGE_FINISHED:   this->onMoveNextStageFinished();  break;
//
//            default: break;
//        }
//    }, this);
}

void StageProgressBar::setStar(int star) {
    
    for( int i = 0; i < star; ++i ) {
        stars[i]->setVisible(true);
    }
}

/*
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
*/

///**
// * 스테이지 변경
// */
//void StageProgressBar::onStageChanged(const StageData &stage) {
//
//    setPercentage(0);
//
//    levelLabel->setString(STR_FORMAT("STAGE %d", stage.stage));
//    startToggleGageTextScheduler();
//}
//
///**
// * 스테이지 재시작
// */
//void StageProgressBar::onStageRestart(const StageData &stage) {
//
//    setPercentage(0);
//}
//
///**
// * 스테이지 클리어
// */
//void StageProgressBar::onStageClear(const StageData &stage) {
//
//    stopToggleGageTextScheduler();
//}
//
///**
// * 다음 스테이지로 이동
// */
//void StageProgressBar::onMoveNextStage() {
//
//    auto updatePercentage = [=](float f) {
//        gage->setPercentage(f);
//    };
//    auto numberAction = ActionFloat::create(MOVE_NEXT_LEVEL_DURATION, 100, 0, updatePercentage);
//    gage->runAction(numberAction);
//}
//
///**
// * 다음 스테이지로 이동 완료
// */
//void StageProgressBar::onMoveNextStageFinished() {
//}
//
