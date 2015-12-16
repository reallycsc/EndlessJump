#include "CSC_IOSHelper.h"

static CSC_IOSHelper _sharedContext;

CSC_IOSHelper* CSC_IOSHelper::getInstance()
{
	static bool s_bFirstUse = true;
	if (s_bFirstUse)
	{
		_sharedContext.init();
		s_bFirstUse = false;
	}
	return &_sharedContext;
}

CSC_IOSHelper::CSC_IOSHelper()
{
}

bool CSC_IOSHelper::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(!Node::init());
		bRet = true;
	} while (false);
	return bRet;
}

/////////////////////////// GameCenter ///////////////////////////
void CSC_IOSHelper::GameCenter_authenticateLocalUser()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] authenticateLocalUser];
#endif
}

bool CSC_IOSHelper::GameCenter_isAuthenticated()
{
	bool ret = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if ([GameKitHelper sharedHelper].isAuthenticated == YES)
	{
		ret = true;
	}
#endif
	return ret;
}

void CSC_IOSHelper::GameCenter_checkAndUnlockAchievement(const char* id)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] checkAndUnlockAchievement:id];
#endif
}

void CSC_IOSHelper::GameCenter_unlockAchievementPercent(const char* id, double percent)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] unlockAchievementPercent :id 
		percentComplete: percent];
#endif
}

void CSC_IOSHelper::GameCenter_showLeaderboard(const char* id)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] showLeaderboard :id];
#endif
}

void CSC_IOSHelper::GameCenter_retriveScoreFromLeaderboard(const char* id)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] retirieveLocalPlayerScore:id];
#endif
}

void CSC_IOSHelper::GameCenter_showAchievements()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] showAchievements];
#endif
}

void CSC_IOSHelper::GameCenter_reportScoreForLeaderboard(const char* id, const int score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] reportScore :score forLeaderboard : id];
#endif
}

void CSC_IOSHelper::GameCenter_resetAchievements()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[[GameKitHelper sharedHelper] resetAchievements];
#endif
}

/////////////////////////// IAP ///////////////////////////
void CSC_IOSHelper::IAP_initWithProductSet(vector<string>* products)
{
	int count = products->size();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (![IAPShare sharedHelper].iap) {
		NSMutableSet  *productIdentifiers = [NSMutableSet setWithCapacity : count];
		for (size_t i = 0; i < count; i++)
		{
			[productIdentifiers addObject : products->at(i).c_str()];
		}
		[IAPShare sharedHelper].iap = [[IAPHelper alloc] initWithProductIdentifiers:productIdentifiers];
	}
#endif
}

void CSC_IOSHelper::IAP_requestAllPurchasedProducts(bool isTest)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	do
	{
		if ([[Reachability reachabilityForInternetConnection] currentReachabilityStatus] == NotReachable)
		{
			[ProgressHUD showError : @"No internet connection!"];
				NSLog(@"No internet connection!");
					break;
		}

		IAPHelper* helper = [IAPShare sharedHelper].iap;
		for (NSString* identifier in helper.productIdentifiers) {
			if (helper.products == nil) {
				if (isTest)
					helper.production = NO; // No for test, YES for release
				else
					helper.production = YES;

				[helper requestProductsWithCompletion : ^ (SKProductsRequest* request, SKProductsResponse* response) {
					if (response > 0) {
						// check is purcahsed
						if ([helper isPurchasedProductsIdentifier : identifier] == YES)
						{
							// notify others to reset removead statues
							Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_PURCHASED + [identifier UTF8String]);
						}
					}
				}];
			}
			else {
				// check is purcahsed
				if ([helper isPurchasedProductsIdentifier : identifier] == YES)
				{
					// notify others to reset removead statues
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_PURCHASED + [identifier UTF8String]);
				}
			}
		}
	} while (false);
#endif
}

void CSC_IOSHelper::IAP_requestAllPurchasedProductsWithCallback(bool isTest, const std::function<void()> &func)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	do
	{
		if ([[Reachability reachabilityForInternetConnection] currentReachabilityStatus] == NotReachable)
		{
			[ProgressHUD showError : @"No internet connection!"];
				NSLog(@"No internet connection!");
					break;
		}

		IAPHelper* helper = [IAPShare sharedHelper].iap;
		for (NSString* identifier in helper.productIdentifiers) {
			if (helper.products == nil) {
				if (isTest)
					helper.production = NO; // No for test, YES for release
				else
					helper.production = YES;

				[helper requestProductsWithCompletion : ^ (SKProductsRequest* request, SKProductsResponse* response) {
					if (response > 0) {
						[ProgressHUD dismiss];
						this->unschedule(schedule_selector(CSC_IOSHelper::waitingTimeOut));
						func();
					}
				}];
				[ProgressHUD show : @"Downloading information, please wait¡­"
					Interaction : FALSE];
				this->scheduleOnce(schedule_selector(CSC_IOSHelper::waitingTimeOut), 10.0f);
			}
			else {
				func();
			}
		}
	} while (false);
#endif
}

void CSC_IOSHelper::IAP_purchaseProduct(bool isTest, const char* id)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if ([[IAPShare sharedHelper].iap isPurchasedProductsIdentifier : id] == YES)
	{
		// notify others to reset removead statues
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_PURCHASED + id);
	}
	else
	{
		this->IAP_requestAllPurchasedProductsWithCallback(isTest,
			[=](Ref* pSender)->void
		{
			[[IAPShare sharedHelper] buyProductWithID:id];
		})
	}
#endif
}

int CSC_IOSHelper::IAP_getProductsCount()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return [[IAPShare sharedHelper].iap.products count];
#else
	return 0;
#endif
}

const char* CSC_IOSHelper::IAP_getProductID(int index)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return [[[IAPShare sharedHelper].iap.products objectAtIndex : index].productIdentifier UTF8String];
#else
	return "#None";
#endif
}

const char* CSC_IOSHelper::IAP_getProductTitle(int index)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return [[[IAPShare sharedHelper].iap.products objectAtIndex : index].localizedTitle UTF8String];
#else
	return "#None";
#endif
}

const char* CSC_IOSHelper::IAP_getProductDescription(int index)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return [[[IAPShare sharedHelper].iap.products objectAtIndex : index].localizedDescription UTF8String];
#else
	return "#None";
#endif
}

const char* CSC_IOSHelper::IAP_getProductPrice(int index)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	SKProduct *product = [[IAPShare sharedHelper].iap.products objectAtIndex : index];
	NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
	[numberFormatter setFormatterBehavior : NSNumberFormatterBehavior10_4];
	[numberFormatter setNumberStyle : NSNumberFormatterCurrencyStyle];
	[numberFormatter setLocale : product.priceLocale];
	return [[numberFormatter stringFromNumber : product.price] UTF8String];
#else
	return "#None";
#endif
}

bool CSC_IOSHelper::IAP_isPurchased(const char* id)
{
	bool ret = false;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if ([[IAPShare sharedHelper].iap isPurchasedProductsIdentifier : id] == YES)
		ret = true;
#endif
	return ret;
}

void CSC_IOSHelper::IAP_restorePurchase()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	do
	{
		if ([[Reachability reachabilityForInternetConnection] currentReachabilityStatus] == NotReachable)
		{
			[ProgressHUD showError : @"No internet connection!"];
				NSLog(@"No internet connection!");
					break;
		}

		[[IAPShare sharedHelper].iap restoreProductsWithCompletion : ^ (SKPaymentQueue *payment, NSError *error) {
			if (payment.transactions.count == 0) {
				[ProgressHUD showError : @"Restored nothing!"];
					NSLog(@"Restored nothing!");
			}
			else {
				for (SKPaymentTransaction *transaction in payment.transactions) {
					NSString* identifier = transaction.payment.productIdentifier;
					NSLog(@"Restored %@", identifier);
					Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_RESTORED+[identifier UTF8String]);
				}
				//[ProgressHUD dismiss];
				this->unschedule(schedule_selector(CSC_IOSHelper::waitingTimeOut));
				[ProgressHUD showSuccess : @"Restore success."];
			}
		}];
		[ProgressHUD show : @"Downloading information, please wait¡­"
			Interaction : FALSE];
		this->scheduleOnce(schedule_selector(CSC_IOSHelper::waitingTimeOut), 10.0f);
	} while (false);
#endif
}

/////////////////////////// Private functions ///////////////////////////
void CSC_IOSHelper::waitingTimeOut(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	[ProgressHUD showError : @"Response time out, please try again."];
#endif
}
