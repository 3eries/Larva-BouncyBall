//
//  PluginPlay.hpp
//
//  Created by seongmin hwang on 2018. 7. 21..
//

#ifndef PluginPlay_hpp
#define PluginPlay_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "../SBPluginConfig.h"
#include "../../base/SBMacros.h"
#include "../../base/SBTypes.hpp"

NS_SB_BEGIN;

class PluginPlay {
public:
    static PluginPlay* getInstance();
    static void destroyInstance();
    ~PluginPlay();
    
private:
    PluginPlay();
    
public:
    void init();
    
    /**
     * Request connection to the platform-specific services backend.
     * This method will invoke plugin's listener <code>onConnectionStatusChanged</code> method.
     */
    static void signIn(bool showLoginUI = true);
    
    /**
     * Request disconnection from the GooglePlay/Game Center backend.
     * This method will invoke plugin's listener <code>onConnectionStatusChanged</code> method.
     */
    static void signOut();
    
    static bool isSignedIn();
    
    /**
     * Get the currently logged in player's id.
     */
    static std::string getPlayerId();
    
    /**
     * Request to show all leaderboards.
     */
    static void showAllLeaderboards();
    
    /**
     * Request to show the default Leaderboard view.
     * In this view you'll be able to interactively select between daily, weekly or all-time leaderboard time frames and the scope
     * to global or you google play's friends results.
     *
     * Android only:
     *  if empty string or __ALL__ is used as leaderboard_name, sdkbox play will invoke an activity
     *  with all game-defined leader boards.
     */
    static void showLeaderboard(const std::string &leaderboardName = "");
    
    /**
     * Request submission of an score value to a leaderboard name defined in sdkbox_config.json file.
     * If the leaderboard name does not exists, or the id associated is not defined in the Developer Console for the application,
     * the call will silently fail.
     * If everything's right, it will notify the method <code>onScoreSubmitted</code>.
     */
    static void submitScore(const std::string &leaderboardName, long score);
    
    /**
     * Request to show the default Achievements view.
     * In this view, you'll only see public achievements.
     * It will show wether or not achievements are unlocked, and the steps towards unlocking it for incremental achievements.
     * Total experience count is measured as well.
     */
    static void showAchievements();
    
    /**
     * Load achievements metadata.
     * A forece reload will force a cloud-side requery of the achievements information.
     * See `onAchievementsLoaded` for a description on the returned information.
     */
    static void loadAchievements(bool forceReload);
    
    /**
     * Request to unlock an achievement defined by its name.
     * This method assumes the achievement is non incremental.
     * If the achievement type is incorrectly defined in the configuration file, or the play services determines it is of the wrong type,
     * this method will fail silently.
     * Otherwise, if everything is right, the method <code>onAchievementUnlocked</code> will be invoked on the plugin listener.
     */
    static void unlockAchievement(const std::string &achievementName);
    
    /**
     * Request to increment the step count of an incremental achievement by the specified number of steps.
     * This method assumes the achievement is incremental.
     * If the achievement type is incorrectly defined in the configuration file, or the play services determines it is of the wrong type,
     * this method will fail silently.
     * If the call is successful, this method may invoke two different methods:
     *   + <code>onIncrementalAchievementStep</code> if the achievement is not unlocked.
     *   + <code>onIncrementalAchievementUnlocked</code> the first time it's been newly unlocked.
     * On Android, the achievement is set to a fixed number of incremental steps. On iOS, the achievment is set as
     * a percentage value (0..100). In either case, the `increment` value will be added to the current achievement's
     * value.
     */
    static void incrementAchievement(const std::string &achievementName, int increment);
    
    /**
     * Reveal a hidden achievement.
     * This method will notify on plugin's listener `onReveal` or `onRevelError` methods.
     */
    static void reveal(const std::string &achievementName);
    
    /**
     * Set an incremental achievement to the given amount of steps.
     * If achievement's current steps are already equal or bigger the specified steps, nothing will happen.
     * This method will  notify on plugin's listener `onSetSteps` or `onSetStepsError` methods.
     */
    static void setSteps(const std::string &achievementName, double steps);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    /**
     * Calling this class method deletes all progress towards achievements
     * previously reported for the local player. Hidden achievements that
     * were previously visible are now hidden again.
     *
     * iOS Only
     */
    static void resetAchievements();
#endif
};
    
NS_SB_END;

#endif /* PluginPlay_hpp */
