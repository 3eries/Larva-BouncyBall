//
//  LogoView.cpp
//
//  Created by seongmin hwang on 2018. 6. 14..
//

#include "LogoView.hpp"
#include <spine/spine-cocos2dx.h>

#include "Define.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace std;

static const string FINISHED_SCHEDULER        = "FINISHED_SCHEDULER";

LogoView::LogoView() :
onFinishedListener(nullptr),
videoPlayer(nullptr),
videoPlayTime(0) {
}

LogoView::~LogoView() {
    
}

bool LogoView::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2::ZERO);
    setContentSize(Size::ZERO);
    
    // addChild(LayerColor::create(Color4B::WHITE));
    addChild(SBNodeUtils::createTouchNode());
    
    showVideo();

    // 백그라운드 전환 이벤트 리스너 등록
    {
        auto listener = EventListenerCustom::create(superbomb::EVENT_ENTER_BACKGROUND, [=](EventCustom *event) {
            this->removeVideo();
        });
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    // 포어그라운드 전환 이벤트 리스너 등록
    {
        auto listener = EventListenerCustom::create(superbomb::EVENT_ENTER_FOREGROUND, [=](EventCustom *event) {
            this->showVideo();
        });
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    return true;
}

#define SCHEDULER_VIDEO_PLAY            "SCHEDULER_VIDEO_PLAY"

void LogoView::showVideo() {
    
    CCLOG("showVideo");
    
    removeVideo();
    
    videoPlayer = VideoPlayer::create();
    videoPlayer->setFileName(DIR_IMG_SPLASH + "intro.mp4");
    videoPlayer->setAnchorPoint(ANCHOR_M);
    videoPlayer->setPosition(Vec2MC(0,0));
    videoPlayer->setContentSize(SB_WIN_SIZE);
    videoPlayer->setKeepAspectRatioEnabled(true);
    videoPlayer->setFullScreenEnabled(true);
    videoPlayer->setUserInputEnabled(false);
    videoPlayer->setStyle(VideoPlayer::StyleType::NONE);
    addChild(videoPlayer);

    videoPlayer->addEventListener([=](Ref* sender, VideoPlayer::EventType eventType) {
        if( eventType == VideoPlayer::EventType::COMPLETED ) {
            this->logoFinished();
        }
    });
    
    if( videoPlayTime > 0 ) {
        videoPlayer->seekTo(videoPlayTime);
        videoPlayer->play();
    }
    
    // 플레이 시간 업데이트
    schedule([=](float dt) {
        if( videoPlayer ) {
            videoPlayTime += dt;
        }
    }, SCHEDULER_VIDEO_PLAY);
}

void LogoView::removeVideo() {
    
    CCLOG("removeVideo");
    
    unschedule(SCHEDULER_VIDEO_PLAY);
    
    if( videoPlayer ) {
        videoPlayer->removeFromParent();
        videoPlayer = nullptr;
    }
}

void LogoView::logoFinished() {
 
    if( onFinishedListener ) {
        onFinishedListener();
    }
}

void LogoView::run() {
    
    videoPlayer->play();
}
