//
//  GameKitHelper.m
//  EndlessLove
//
//  Created by 陈苏超 on 15/11/17.
//
//

#import <CommonCrypto/CommonCryptor.h>
#import "GameKitHelper.h"

#include "../CommonDefinition.h"

#define IS_MIN_IOS6 ([[[UIDevice currentDevice] systemVersion] floatValue] >= 6.0f)

@interface GameKitHelper () <GKLeaderboardViewControllerDelegate, GKAchievementViewControllerDelegate>
@end

@implementation GameKitHelper

@synthesize gameCenterAvailable;

#pragma mark - Singleton
+ (id)sharedHelper {
    static GameKitHelper *sharedHelper = nil;
    static dispatch_once_t once = 0;
    dispatch_once(&once, ^{sharedHelper = [[self alloc] init];});
    return sharedHelper;
}

#pragma mark - Initializer
- (id)init{
    self = [super init];
    if (self) {
        gameCenterAvailable = [self isGameCenterAvailable];
        if (gameCenterAvailable) {
            NSNotificationCenter *nc = [NSNotificationCenter defaultCenter];
            [nc addObserver:self
                   selector:@selector(authenticationChanged)
                       name:GKPlayerAuthenticationDidChangeNotificationName
                     object:nil];
        }
    }
    return self;
}

#pragma mark - isAvailable
- (BOOL)isGameCenterAvailable {
    // check for presence of GKLocalPlayer API
    Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
    
    // check if the device is running iOS 4.1 or later
    NSString *reqSysVer = @"4.1";
    NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
    BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
    
    return (gcClass && osVersionSupported);
}

#pragma mark - Authenticate
- (void)authenticationChanged{
    GKLocalPlayer *player = [GKLocalPlayer localPlayer];
    
    if (player.isAuthenticated && self.authenticated == NO) {
        if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Authentication changed - player authenticated.");
        self.authenticated = YES;
    }
    else if (!player.isAuthenticated && self.authenticated == YES) {
        if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Authentication changed - player not authenticated.");
        self.authenticated = NO;
    }
}

- (void)authenticateLocalUser {
    if (!gameCenterAvailable) return;
    
    NSLog(@"GameKitHelper: Authenticating local user...");

    GKLocalPlayer *localPlayer = [GKLocalPlayer localPlayer];
    if ([localPlayer isAuthenticated] == YES) {
        if (GAMEKITHELPER_LOGGING) NSLog(@"The local player has already authenticated.");
        return;
    }
    
    void (^authBlock)(UIViewController *, NSError *) = ^(UIViewController *viewController, NSError *error) {
        
        if (viewController) {
            [[self topViewController] presentViewController:viewController animated:YES completion:nil];
        }
        
        if (localPlayer.isAuthenticated) {
            self.authenticated = YES;
            if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Player successfully authenticated.");
            // Report possible cached scores / achievements
            [self reportCachedAchievements];
            [self reportCachedScores];
            // get achievements from game center
            [self loadAllAchievements];
            // get score from game center & notify player data to update
            [self retirieveLocalPlayerScore:@"Highscore"];
            [self retirieveLocalPlayerScore:@"GoldNumber"];
            [self retirieveLocalPlayerScore:@"GoldNumberAll"];
            [self retirieveLocalPlayerScore:@"ReviveNumber"];
            [self retirieveLocalPlayerScore:@"DoubleNumber"];
            // notify mainmenu to reset purchase status
            cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_GAMECENTER_AUTHENTICATED);
        }

        if (error) {
            self.authenticated = NO;
            if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Player didn't authenticate");
        }
    };
    
    //iOS 6.x +
    if (IS_MIN_IOS6) {
        [localPlayer setAuthenticateHandler:authBlock];
    }
    //iOS 5.0
    else {
        [localPlayer authenticateWithCompletionHandler:^(NSError *error) {
            authBlock(nil, error);
         }];
    }
}

#pragma mark - Leaderboard
- (void)retirieveLocalPlayerScore:(NSString*)leaderboardId {
    if (!gameCenterAvailable) return;
    if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    
    GKLeaderboard* leaderboard = [[GKLeaderboard alloc] init];
    
    if (leaderboard) {
        leaderboard.identifier = leaderboardId;
        [leaderboard loadScoresWithCompletionHandler:^(NSArray<GKScore *> * _Nullable scores, NSError * _Nullable error) {
            if (error) {
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Retrive score from leaderboard failed.");
            }
            if (scores) {
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Retrive score from leaderboard successfully.");
                // notify gamemeidator to reload the score
                string eventId = EVENT_GAMECENTER_SCORERETRIVED + [leaderboardId UTF8String];
                char* buf = new char[21];
                sprintf(buf, "%lld", leaderboard.localPlayerScore.value);
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(eventId, buf);
            }
        }];
    }
}

- (void)reportScore:(long long)aScore forLeaderboard:(NSString*)leaderboardId {
    if (!gameCenterAvailable) return;
    if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    
    GKScore *score = [[GKScore alloc] initWithLeaderboardIdentifier:leaderboardId];
    score.value = aScore;
    
    [score reportScoreWithCompletionHandler:^(NSError *error) {
        if (!error) {
            if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Reported score (%lli) to %@ successfully.", score.value, leaderboardId);
        }
        else {
            [self cacheScore:score];
            if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Reporting score (%lli) to %@ failed, caching...", score.value, leaderboardId);
        }
    }];
}

- (void)showLeaderboard:(NSString*)leaderboardId {
    if (!gameCenterAvailable) return;
    if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    GKLeaderboardViewController *viewController = [[GKLeaderboardViewController alloc] init];
    viewController.leaderboardDelegate = self;
    if (leaderboardId) {
        viewController.category = leaderboardId;
    }
    
    [[self topViewController] presentViewController:viewController animated:YES completion:nil];
}

#pragma mark - Achievements
- (void)checkAndUnlockAchievement:(NSString*)achievementId {
	if (!gameCenterAvailable) return;
	if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    if (![self isAchievementUnlocked:achievementId])
    {
        [self reportAchievement:achievementId percentComplete:100];
    }
}

- (void)unlockAchievementPercent:(NSString*)achievementId percentComplete:(double)percent {
    if (!gameCenterAvailable || percent > 100.0f) return;
    if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    if ([self getAchievementPercent:achievementId] < percent)
    {
        [self reportAchievement:achievementId percentComplete:percent];
    }
}

- (BOOL)isAchievementUnlocked:(NSString*)achievementId {
    if (!gameCenterAvailable) return FALSE;

    return [self getAchievementForID:achievementId].completed;
}

- (double)getAchievementPercent:(NSString*)achievementId {
    if (!gameCenterAvailable) return FALSE;
    if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    return [self getAchievementForID:achievementId].percentComplete;
}

- (GKAchievement*)getAchievementForID:(NSString*)achievementId {
    if (self.achievementDictionary == nil) {
        self.achievementDictionary = [[NSMutableDictionary alloc] init];
    }
    
    GKAchievement* achievement = [self.achievementDictionary objectForKey:achievementId];
    if (achievement == nil) {
        achievement = [[GKAchievement alloc] initWithIdentifier:achievementId];
        [self.achievementDictionary setObject:achievement
                                       forKey:achievement.identifier];
    }
    [achievement retain];
    return achievement;
}

- (void)reportAchievement:(NSString*)achievementId percentComplete:(double)percent {
    if (!gameCenterAvailable) return;

    if (percent > 100.0f) percent = 100.0f;
    
    GKAchievement* achievementLocal = [self getAchievementForID:achievementId];
    if (achievementLocal) {
        achievementLocal.percentComplete = percent;
    }
    GKAchievement* achievement = [[GKAchievement alloc] initWithIdentifier:achievementId];
    if (achievement) {
        achievement.percentComplete = percent;
        achievement.showsCompletionBanner = YES;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error) {
            if (!error) {
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Achievement (%@) with %f%% progress reported", achievement.identifier, achievement.percentComplete);
            }
            else {
                [self cacheAchievement:achievement];
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Reporting achievement (%@) with %f%% progress failed, caching...", achievement.identifier, achievement.percentComplete);
            }
        }];
    }
}

- (void)showAchievements {
    if (!gameCenterAvailable) return;
    if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    GKAchievementViewController *viewController = [[GKAchievementViewController alloc] init];
    if (viewController) {
        viewController.achievementDelegate = self;
        
        [[self topViewController] presentViewController:viewController animated:YES completion:nil];
    }
}

- (void)resetAchievements {
    if (!gameCenterAvailable) return;
	if (!self.authenticated) {
        [self authenticateLocalUser];
    }
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError *error) {
        if (!error) {
            if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Achievements reset successfully.");
        }
        else {
            if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Failed to reset achievements.");
        }
    }];
}

- (void)loadAllAchievements {
    if (self.achievementDictionary == nil) {
        self.achievementDictionary = [[NSMutableDictionary alloc] init];
    }
    [GKAchievement loadAchievementsWithCompletionHandler:^(NSArray<GKAchievement *> * _Nullable achievements, NSError * _Nullable error) {
        if (error == nil && achievements) {
            NSArray* tempArray = [NSArray arrayWithArray:achievements];
            for (GKAchievement* tempAchievement in tempArray) {
                [self.achievementDictionary setObject:tempAchievement
                                               forKey:tempAchievement.identifier];
                if (tempAchievement) {
                    if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: %d - Achievement (%@) with %f%% progress was reported at %@", tempAchievement.completed, tempAchievement.identifier, tempAchievement.percentComplete, tempAchievement.lastReportedDate);
                }
            }
        }
    }];
}

#pragma mark - Notifications
- (void)showNotification:(NSString*)title message:(NSString*)message identifier:(NSString*)achievementId {
    if (!gameCenterAvailable) return;
    //Show notification only if it hasn't been achieved yet
    if (![self boolForKey:achievementId]) {
        [GKNotificationBanner showBannerWithTitle:title message:message completionHandler:nil];
    }
}

#pragma mark - Caching
#pragma mark - Caching Scores
- (void)cacheScore:(GKScore*)aScore {
    //Retrieve cached scores
    NSMutableArray *scores = [self objectForKey:@"cachedScores"];
    
    //Add new score to array
    [scores addObject:aScore];
    
    //Save scores to persistant storage
    [self saveObject:scores key:@"cachedScores"];
}

- (void)reportCachedScores {
    //Retrieve cached scores
    NSMutableArray *scores = [self objectForKey:@"cachedScores"];
    
    if (scores.count == 0) {
        return;
    }
    
    if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Attempting to report %lu cached scores...", (unsigned long)scores.count);
    
    //iOS 6.x+
    if (IS_MIN_IOS6) {
        [GKScore reportScores:scores withCompletionHandler:^(NSError *error) {
            if (!error) {
                [self removeAllCachedScores];
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Reported %lu cached score(s) successfully.", (unsigned long)scores.count);
            }
            else {
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Failed to report %lu cached score(s).", (unsigned long)scores.count);
            }
        }];
    }
    //iOS 5.1
    else {
        for (GKScore *score in scores) {
            [score reportScoreWithCompletionHandler:^(NSError *error) {
                if (!error) {
                    [self removeCachedScore:score];
                    if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Reported cached score (%@ : %lli) successfully.", score.category, score.value);
                }
                else {
                    if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Failed to report cached score (%@ : %lli).", score.category, score.value);
                }
            }];
        }
    }
}

- (void)removeCachedScore:(GKScore*)score {
    NSMutableArray *scores = [self objectForKey:@"cachedScores"];
    [scores removeObject:score];
    [self saveObject:scores key:@"cachedScores"];
}

- (void)removeAllCachedScores {
    [self saveObject:[NSMutableArray new] key:@"cachedScores"];
}

#pragma mark - Caching Achievements
- (void)cacheAchievement:(GKAchievement*)achievement {
    //Retrieve cached achievements
    NSMutableArray *achievements = [self objectForKey:@"cachedAchievements"];
    
    //Add new achievment to array
    [achievements addObject:achievement];
    
    //Save achievement to persistant storage
    [self saveObject:achievements key:@"cachedAchievements"];
}

- (void)reportCachedAchievements {
    //Retrieve cached achievements
    NSMutableArray *achievements = [self objectForKey:@"cachedAchievements"];
    
    if (achievements.count == 0) {
        return;
    }
    
    if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Attempting to report %lu cached achievements...", (unsigned long)achievements.count);
    
    //iOS 6.x +
    if (IS_MIN_IOS6) {
        [GKAchievement reportAchievements:achievements withCompletionHandler:^(NSError *error) {
            if (!error) {
                [self removeAllCachedAchievements];
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Reported %lu cached achievement(s) successfully.", (unsigned long)achievements.count);
            }
            else {
                if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Failed to report %lu cached achievement(s).", (unsigned long)achievements.count);
            }
        }];
    }
    //iOS 5.1
    else {
        for (GKAchievement *achievement in achievements) {
            [achievement reportAchievementWithCompletionHandler:^(NSError *error) {
                if (!error) {
                    [self removeCachedAchievement:achievement];
                    if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: Reported cached achievement (%@) successfully.", achievement.identifier);
                }
                else {
                    if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Failed to report cached achievement (%@).", achievement.identifier);
                }
            }];
        }
    }
}

- (void)removeCachedAchievement:(GKAchievement*)achievement {
    NSMutableArray *achievements = [self objectForKey:@"cachedAchievements"];
    [achievements removeObject:achievement];
    [self saveObject:achievements key:@"cachedAchievements"];
}

- (void)removeAllCachedAchievements {
    [self saveObject:[NSMutableArray new] key:@"cachedAchievements"];
}

#pragma mark - Data Persistance
- (NSString*)filePath {
    NSString *fileExt = @".abgk";
    NSString *fileName = [NSString stringWithFormat:@"%@%@", [[self appName] lowercaseString], fileExt];
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    NSString *path = [documentsDirectory stringByAppendingPathComponent:fileName];
    return path;
}

- (NSMutableDictionary*)dataDictionary {
    NSData *binaryFile = [NSData dataWithContentsOfFile:[self filePath]];
    NSMutableDictionary *dictionary = nil;
    
    if (binaryFile == nil) {
        dictionary = [NSMutableDictionary dictionary];
    }
    else {
        NSData *decryptedData = [self decryptData:binaryFile withKey:SECRET_KEY];
        dictionary = [NSKeyedUnarchiver unarchiveObjectWithData:decryptedData];
    }
    
    return dictionary;
}

- (void)saveData:(NSData*)data key:(NSString*)key {
    BOOL fileExists = [[NSFileManager defaultManager] fileExistsAtPath:[self filePath]];
    NSMutableDictionary *tempDic = nil;
    if (fileExists == NO) {
        tempDic = [NSMutableDictionary new];
    }
    else {
        tempDic = [self dataDictionary];
    }
    
    [tempDic setObject:data forKey:key];
    
    NSData *dicData = [NSKeyedArchiver archivedDataWithRootObject:tempDic];
    NSData *encryptedData = [self encryptData:dicData withKey:SECRET_KEY];
    [encryptedData writeToFile:[self filePath] atomically:YES];
}

- (NSData*)dataForKey:(NSString*)key {
    NSMutableDictionary *tempDic = [self dataDictionary];
    NSData *loadedData = [tempDic objectForKey:key];
    
    if (loadedData) {
        return loadedData;
    }
    
    return nil;
}

- (void)saveObject:(id<NSCoding>)object key:(NSString*)key {
    NSData *data = [NSKeyedArchiver archivedDataWithRootObject:object];
    [self saveData:data key:key];
}

- (id)objectForKey:(NSString*)key {
    NSData *data = [self dataForKey:key];
    if (data) {
        return [NSKeyedUnarchiver unarchiveObjectWithData:data];
    }
    return nil;
}

- (void)saveBool:(BOOL)boolean key:(NSString*)key {
    NSNumber *number = [NSNumber numberWithBool:boolean];
    NSData *data = [NSKeyedArchiver archivedDataWithRootObject:number];
    [self saveData:data key:key];
}

- (BOOL)boolForKey:(NSString*)key {
    NSData *data = [self dataForKey:key];
    if (data) {
        return [[NSKeyedUnarchiver unarchiveObjectWithData:data] boolValue];
    }
    return NO;
}

#pragma mark - Helper
- (NSString*)appName {
    NSString *bundlePath = [[[NSBundle mainBundle] bundleURL] lastPathComponent];
    return [[bundlePath stringByDeletingPathExtension] lowercaseString];
}

- (UIViewController*)topViewController {
    UIViewController *topController = [UIApplication sharedApplication].keyWindow.rootViewController;
    while (topController.presentedViewController) {
        topController = topController.presentedViewController;
    }
    return topController;
}

- (NSData*)makeCryptedVersionOfData:(NSData*)data withKeyData:(const void*)keyData ofLength:(NSUInteger) keyLength decrypt:(bool)decrypt {
    int keySize = kCCKeySizeAES256;
    char key[kCCKeySizeAES256];
    bzero(key, sizeof(key));
    memcpy(key, keyData, keyLength > keySize ? keySize : keyLength);
    
    size_t bufferSize = [data length] + kCCBlockSizeAES128;
    void* buffer = malloc(bufferSize);
    
    size_t dataUsed;
    
    CCCryptorStatus status = CCCrypt(decrypt ? kCCDecrypt : kCCEncrypt,
                                     kCCAlgorithmAES128,
                                     kCCOptionPKCS7Padding | kCCOptionECBMode,
                                     key, keySize,
                                     NULL,
                                     [data bytes], [data length],
                                     buffer, bufferSize,
                                     &dataUsed);
    
    switch(status) {
        case kCCSuccess:
            return [NSData dataWithBytesNoCopy:buffer length:dataUsed];
        default:
            if (GAMEKITHELPER_LOGGING) NSLog(@"GameKitHelper: ERROR -> Failed to encrypt!");
    }
    
    free(buffer);
    return nil;
}

- (NSData*)encryptData:(NSData*)data withKey:(NSString*)key {
    NSData *keyData = [key dataUsingEncoding:NSUTF8StringEncoding];
    return [self makeCryptedVersionOfData:data withKeyData:[keyData bytes] ofLength:[keyData length] decrypt:false];
}

- (NSData*)decryptData:(NSData*)data withKey:(NSString*)key {
    NSData *keyData = [key dataUsingEncoding:NSUTF8StringEncoding];
    return [self makeCryptedVersionOfData:data withKeyData:[keyData bytes] ofLength:[keyData length] decrypt:true];
}

#pragma mark - GKLeaderboardViewControllerDelegate
- (void)leaderboardViewControllerDidFinish:(GKLeaderboardViewController*)viewController {
    [viewController dismissViewControllerAnimated:YES completion:nil];
}

#pragma mark - GKAchievementViewControllerDelegate
- (void)achievementViewControllerDidFinish:(GKAchievementViewController *)viewController {
    [viewController dismissViewControllerAnimated:YES completion:nil];
}

@end