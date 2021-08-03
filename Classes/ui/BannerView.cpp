//
//  BannerView.cpp
//
//  Created by seongmin hwang on 27/03/2019.
//

#include "BannerView.hpp"

#include "Define.h"
#include "GameConfiguration.hpp"

USING_NS_CC;
USING_NS_SB;

BannerView::BannerView() {
}

BannerView::~BannerView() {
}

bool BannerView::init() {
    
    if( !Node::init() ) {
        return false;
    }
    
    setAnchorPoint(Vec2::ZERO);
    setPosition(Vec2::ZERO);
    setContentSize(Size::ZERO);
    
    // more games 버튼
    moreGames = SBButton::create(DIR_IMG_COMMON + "common_banner_more_games.png");
    moreGames->setZoomScale(0);
    moreGames->setAnchorPoint(ANCHOR_MB);
    moreGames->setPosition(Vec2BC(0,0));
    addChild(moreGames);
    
    moreGames->setOnClickListener([=](Node*) {
        
        SBAnalytics::logEvent(ANALYTICS_EVENT_MORE_GAMES);
        Application::getInstance()->openURL(GAME_CONFIG->getMoreGamesUrl());
    });
    
    // 배너 광고 로드 시 more games 버튼 숨김
    {
        auto listener = AdListener::create(AdType::BANNER);
        listener->setTarget(this);
        listener->setForever(true);
        listener->onAdLoaded = [=]() {
            moreGames->setVisible(false);
        };
        AdsHelper::getInstance()->getEventDispatcher()->addListener(listener);
    }
    
    // 광고 제거 이벤트
    {
        auto listener = EventListenerCustom::create(DIRECTOR_EVENT_REMOVE_ADS, [=](EventCustom *event) {
            this->setVisible(false);
        });
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    }
    
    return true;
}

void BannerView::onEnter() {
    
    Node::onEnter();
    
    updateNativeUI();
}

void BannerView::onExit() {
    
//#if SB_PLUGIN_USE_ADS
//    AdsHelper::hideBanner();
//#endif
    
    Node::onExit();
}

void BannerView::cleanup() {
    
    removeListeners(this);
    
    Node::cleanup();
}

void BannerView::setVisible(bool isVisible) {
    
    Node::setVisible(isVisible);
    
    updateNativeUI();
}

void BannerView::updateNativeUI() {
    
#if SB_PLUGIN_USE_ADS
    if( isVisible() ) {
        AdsHelper::showBanner();

        if( AdsHelper::isBannerLoaded() ) {
            moreGames->setVisible(false);
        }
    } else {
        AdsHelper::hideBanner();
    }
#endif
}
