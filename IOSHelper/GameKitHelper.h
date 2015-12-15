//
//  GameKitHelper.h
//  EndlessLove
//
//  Created by 陈苏超 on 15/11/17.
//
//

#ifndef GameKitHelper_h
#define GameKitHelper_h

#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

#include "CommonHeader.h"

#ifndef GAMEKITHELPER_LOGGING
#define GAMEKITHELPER_LOGGING 1
#endif

// Set SECRET_KEY for proper encryption
#define SECRET_KEY @"reallycsc"

@interface GameKitHelper : NSObject

// Always access class through this singleton
// Call it once on application start to authenticate local player
+ (id)sharedHelper;

// is available
- (BOOL)isGameCenterAvailable;

// Authentication
- (void)authenticateLocalUser;

// Leaderboards
- (void)retirieveLocalPlayerScore:(NSString*)leaderboardId;
- (void)reportScore:(long long)aScore forLeaderboard:(NSString*)leaderboardId;
- (void)showLeaderboard:(NSString*)leaderboardId;

// Achievements
- (void)checkAndUnlockAchievement:(NSString*)achievementId;
- (void)unlockAchievementPercent:(NSString*)achievementId percentComplete:(double)percent;
- (BOOL)isAchievementUnlocked:(NSString*)achievementId;
- (double)getAchievementPercent:(NSString*)achievementId;
- (void)reportAchievement:(NSString*)achievementId percentComplete:(double)percent;
- (void)showAchievements;
- (void)resetAchievements;

// Notifications
- (void)showNotification:(NSString*)title message:(NSString*)message identifier:(NSString*)achievementId;

@property (nonatomic, assign, readonly) BOOL gameCenterAvailable;
@property (nonatomic, assign, getter = isAuthenticated) BOOL authenticated;
// local achievement dictionary
@property (nonatomic, retain) NSMutableDictionary* achievementDictionary;

@end

#endif /* GameKitHelper_h */
