//
//  GameCenterHelper.h
//
//  Created by seongmin hwang on 2018. 7. 22..
//

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

#include "cocos2d.h"

@interface GameCenterHelper : NSObject<GKGameCenterControllerDelegate> {
    
}

+ (GameCenterHelper*) getInstance;

+ (void) signIn;
+ (void) signOut;

+ (BOOL) isSignedIn;

+ (NSString*) getPlayerId;

+ (void) showAllLeaderboards;
+ (void) showLeaderboard:(NSString *)leaderboardName;

+ (void) submitScore:(NSString *)leaderboardName :(long)score;

+ (void) showAchievements;
+ (void) loadAchievements:(BOOL)forceReload;

+ (void) reportAchievement:(NSString *)achievementName :(float)percent;
+ (void) unlockAchievement:(NSString *)achievementName;

+ (void) incrementAchievement:(NSString *)achievementName :(int)increment;
+ (void) reveal:(NSString *)achievementName;
+ (void) setSteps:(NSString *)achievementName :(double)steps;

+ (void) resetAchievements;

///////////////////////////////////////////////////
//////////// GKGameCenterControllerDelegate
///////////////////////////////////////////////////
- (void) gameCenterViewControllerDidFinish:(GKGameCenterViewController *)controller;

@end

