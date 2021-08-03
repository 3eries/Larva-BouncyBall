//
//  GameCenterHelper.m
//
//  Created by seongmin hwang on 2018. 7. 22..
//

#import "GameCenterHelper.h"

#import "AppController.h"
#import "RootViewController.h"

#include "../../SBPluginConfig.h"
#include "../../../platform/SBPlatformMacros.h"

USING_NS_CC;
using namespace std;

@interface GameCenterHelper (Private)

- (id) init;
- (void) authenticateLocalPlayer;

@end

@implementation GameCenterHelper

+ (GameCenterHelper*) getInstance {
    
    static GameCenterHelper *instance = nil;
    static dispatch_once_t oncePredicate;
    
    dispatch_once(&oncePredicate, ^{
        instance = [[GameCenterHelper alloc] init];
    });
    
    return instance;
}

- (id) init {
    
    if( self = [super init] ) {
    }
    
    return self;
}

/**
 * 플레이어 인증
 */
- (void) authenticateLocalPlayer {
    
#if SB_PLUGIN_USE_PLAY
    NSLog(@"authenticateLocalPlayer isAuthenticated: %d", [GameCenterHelper isSignedIn]);
    
    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    localPlayer.authenticateHandler = ^(UIViewController *loginViewController, NSError *error){
    
        NSLog(@"authenticateLocalPlayer completion viewController: %d isAuthenticated: %d error: %@",
              loginViewController != nullptr,
              [GameCenterHelper isSignedIn],
              (error != nil) ? error : @"");
        
        // show game center
        if( loginViewController != nil ) {
            [ROOT_VIEW_CONTROLLER presentViewController:loginViewController animated:YES completion:^{
                NSLog(@"Finished Game Center.");
            }];
        }
        // player was authenticated
        else if( localPlayer.isAuthenticated ) {
        }
        // Game Center should be disabled
        else {
            
        }
    };
#endif
}

+ (void) signIn {
    
#if SB_PLUGIN_USE_PLAY
    if( [GameCenterHelper isSignedIn] ) {
        return;
    }
    
    [[GameCenterHelper getInstance] authenticateLocalPlayer];
#endif
}

+ (void) signOut {
    
#if SB_PLUGIN_USE_PLAY
    if( ![GameCenterHelper isSignedIn] ) {
        return;
    }
#endif
}

+ (BOOL) isSignedIn {
#if SB_PLUGIN_USE_PLAY
    return [GKLocalPlayer localPlayer].isAuthenticated;
#else
    return NO;
#endif
}

+ (NSString*) getPlayerId {
#if SB_PLUGIN_USE_PLAY
    return [GKLocalPlayer localPlayer].playerID;
#else
    return @"";
#endif
}

+ (void) showAllLeaderboards {
    
#if SB_PLUGIN_USE_PLAY
    GKGameCenterViewController *controller = [[GKGameCenterViewController alloc] init];
    controller.gameCenterDelegate = [GameCenterHelper getInstance];
    controller.viewState = GKGameCenterViewControllerStateLeaderboards;
    
    [ROOT_VIEW_CONTROLLER presentViewController:controller animated:YES completion:^{
        NSLog(@"showAllLeaderboards completion");
    }];
#endif
}

+ (void) showLeaderboard:(NSString *)leaderboardName {

#if SB_PLUGIN_USE_PLAY
    GKGameCenterViewController *controller = [[GKGameCenterViewController alloc] init];
    controller.gameCenterDelegate = [GameCenterHelper getInstance];
    controller.viewState = GKGameCenterViewControllerStateLeaderboards;
    controller.leaderboardIdentifier = leaderboardName;
    
    [ROOT_VIEW_CONTROLLER presentViewController:controller animated:YES completion:^{
        NSLog(@"showLeaderboard completion");
    }];
#endif
}

+ (void) submitScore:(NSString *)leaderboardName :(long)score {
 
#if SB_PLUGIN_USE_PLAY
    // int64_t score = (int64_t)highScore;
    GKScore *scoreReporter = [[GKScore alloc] initWithLeaderboardIdentifier:leaderboardName];
    scoreReporter.value = score;
    scoreReporter.context = 0;
    
    NSArray *scores = @[scoreReporter];
    [GKScore reportScores:scores withCompletionHandler:^(NSError *error) {
        //Do something interesting here.
    }];
#endif
}

+ (void) showAchievements {
    
#if SB_PLUGIN_USE_PLAY
    GKGameCenterViewController *controller = [[GKGameCenterViewController alloc] init];
    controller.gameCenterDelegate = [GameCenterHelper getInstance];
    controller.viewState = GKGameCenterViewControllerStateAchievements;
    
    [ROOT_VIEW_CONTROLLER presentViewController:controller animated:YES completion:^{
        NSLog(@"showAchievements completion");
    }];
#endif
}

+ (void) loadAchievements:(BOOL)forceReload {
    
    // TODO:
}

+ (void) reportAchievement:(NSString *)achievementName :(float)percent {

#if SB_PLUGIN_USE_PLAY
    GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier:achievementName];
    achievement.showsCompletionBanner = YES;
    achievement.percentComplete = percent;
    
    [GKAchievement reportAchievements:@[achievement] withCompletionHandler:^(NSError *error) {
        if( error != nil ) {
            NSLog(@"Error in reporting achievements: %@", error);
        }
    }];
#endif
}

+ (void) unlockAchievement:(NSString *)achievementName {
    
#if SB_PLUGIN_USE_PLAY
    [GameCenterHelper reportAchievement:achievementName :100];
#endif
}

+ (void) incrementAchievement:(NSString *)achievementName :(int)increment {
 
    // TODO:
}

+ (void) reveal:(NSString *)achievementName {
    
    // TODO:
}

+ (void) setSteps:(NSString *)achievementName :(double)steps {
    
    // TODO:
}

+ (void) resetAchievements {
    
    // TODO:
}

///////////////////////////////////////////////////
//////////// GKGameCenterControllerDelegate
///////////////////////////////////////////////////

- (void) gameCenterViewControllerDidFinish:(GKGameCenterViewController *)controller {
    
    [controller dismissViewControllerAnimated:YES completion:nil];
}

@end


