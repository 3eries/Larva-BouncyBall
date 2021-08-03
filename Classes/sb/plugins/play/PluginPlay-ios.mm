//
//  PluginPlay-ios.m
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#include "PluginPlay.hpp"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#import <Foundation/Foundation.h>
#import "ios/GameCenterHelper.h"

#include "../../platform/SBPlatformMacros.h"

USING_NS_CC;
using namespace std;

NS_SB_BEGIN;

void PluginPlay::init() {
    
#if SB_PLUGIN_USE_PLAY
#endif
}

void PluginPlay::signIn(bool showLoginUI) {
    
#if SB_PLUGIN_USE_PLAY
    [GameCenterHelper signIn];
#endif
}

void PluginPlay::signOut() {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper signOut];
#endif
}

bool PluginPlay::isSignedIn() {
    
#if SB_PLUGIN_USE_PLAY
    return [GameCenterHelper isSignedIn];
#else
    return false;
#endif
}

string PluginPlay::getPlayerId() {

#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return "";
    }
    
    return [[GameCenterHelper getPlayerId] UTF8String];
#else
    return "";
#endif
}

void PluginPlay::showAllLeaderboards() {

#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper showAllLeaderboards];
#endif
}

void PluginPlay::showLeaderboard(const string &leaderboardName) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper showLeaderboard:NS_STRING(leaderboardName.c_str())];
#endif
}

void PluginPlay::submitScore(const string &leaderboardName, long score) {

#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper submitScore:NS_STRING(leaderboardName.c_str()) :score];
#endif
}

void PluginPlay::showAchievements() {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper showAchievements];
#endif
}

void PluginPlay::loadAchievements(bool forceReload) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper loadAchievements:forceReload];
#endif
}

void PluginPlay::unlockAchievement(const string &achievementName) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper unlockAchievement:NS_STRING(achievementName.c_str())];
#endif
}

void PluginPlay::incrementAchievement(const string &achievementName, int increment) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper incrementAchievement:NS_STRING(achievementName.c_str()) :increment];
#endif
}

void PluginPlay::reveal(const string &achievementName) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper reveal:NS_STRING(achievementName.c_str())];
#endif
}

void PluginPlay::setSteps(const string &achievementName, double steps) {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper setSteps:NS_STRING(achievementName.c_str()) :steps];
#endif
}

void PluginPlay::resetAchievements() {
    
#if SB_PLUGIN_USE_PLAY
    if( !isSignedIn() ) {
        return;
    }
    
    [GameCenterHelper resetAchievements];
#endif
}

NS_SB_END;

#endif // CC_PLATFORM_IOS
