#pragma once
#include "CommonDefinition.h"

NS_CSC_BEGIN
class CSC_IOSHelper : public Ref
{
public:
	CSC_IOSHelper();
	// singlar
	static CSC_IOSHelper* getInstance();
	bool init();

public:
	// Game Center
	static void GameCenter_authenticateLocalUser(); // login
	static bool GameCenter_isAuthenticated();

	static void GameCenter_showLeaderboard(const char* id);
	static void GameCenter_retriveScoreFromLeaderboard(const char* id);
	static void GameCenter_reportScoreForLeaderboard(const char* id, const int score);

	static void GameCenter_showAchievements();
	static void GameCenter_checkAndUnlockAchievement(const char* id);
	static void GameCenter_unlockAchievementPercent(const char* id, double percent);
	static void GameCenter_resetAchievements();

	// IAP
	static void IAP_initWithProductSet(vector<string>* products); // init

	static void IAP_requestAllPurchasedProducts(bool isTest);
    void IAP_requestAllPurchasedProductsWithCallback(bool isTest, const function<void()> &func);

	static unsigned long IAP_getProductsCount();
	static const char* IAP_getProductID(int index);
	static const char* IAP_getProductTitle(int index);
	static const char* IAP_getProductDescription(int index);
	static const char* IAP_getProductPrice(int index);

	static bool IAP_isPurchased(const char* id);
    void IAP_purchaseProduct(bool isTest, const char* id);
    void IAP_restorePurchase(); // restore 

private:
    void waitingTimeOut(float dt);
};
NS_CSC_END