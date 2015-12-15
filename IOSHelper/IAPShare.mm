//
//  IAPShare.m
//  inappPurchasesTest
//
//  Created by Htain Lin Shwe on 10/7/12.
//  Copyright (c) 2012 Edenpod. All rights reserved.
//

#import "IAPShare.h"
#import "ProgressHUD.h"

#if ! __has_feature(objc_arc)
#error You need to either convert your project to ARC or add the -fobjc-arc compiler flag to IAPShare.m.
#endif

@implementation IAPShare
@synthesize iap= _iap;
static IAPShare * _sharedHelper;

+ (IAPShare *) sharedHelper {
    
    if (_sharedHelper != nil) {
        return _sharedHelper;
    }
    _sharedHelper = [[IAPShare alloc] init];
    _sharedHelper.iap = nil;
    return _sharedHelper;
}

- (void)requestProductWithID:(NSString *)productID isProduction:(BOOL)production {
    if (_iap.products == nil) {
        _iap.production = production; // No for test, YES for release
        
        [_iap requestProductsWithCompletion : ^ (SKProductsRequest* request, SKProductsResponse* response) {
            if (response > 0) {
                // check is purcahsed
                if ([_iap isPurchasedProductsIdentifier : productID] == YES)
                {
                    // notify others to reset removead statues
                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_PURCHASED_REMOVEAD + [productID UTF8String]);
                }
            }
        }];
    }
    else {
        // check is purcahsed
        if ([_iap isPurchasedProductsIdentifier : productID] == YES)
        {
            // notify others to reset removead statues
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_PURCHASED_REMOVEAD + [productID UTF8String]);
        }
    }
}

- (void)buyProductWithID:(NSString *)productID {
    for(SKProduct* skProduct in _iap.products) {
        if ([skProduct.productIdentifier compare:productID] == NSOrderedSame) {
            [ProgressHUD show : @"Buying from Apple.inc, please wait…"
                  Interaction : FALSE];
            [_iap buyProductOnCompletion:skProduct
                            onCompletion:^(SKPaymentTransaction *transcation) {
                                [ProgressHUD dismiss];
                                if (transcation.error) {
                                    NSLog(@"Fail %@",[transcation.error localizedDescription]);
                                }
                                else if (transcation.transactionState == SKPaymentTransactionStatePurchased) {
                                    [ProgressHUD show : @"Checking receipt from Apple.inc, please wait…"
                                          Interaction : FALSE];
                                    [_iap checkReceipt : transcation.transactionReceipt
                                       AndSharedSecret : @"your sharesecret"
                                           onCompletion: ^ (NSString *response, NSError *error) {
                                               [ProgressHUD dismiss];
                                               if (response) {
                                                   //Convert JSON String to NSDictionary
                                                   NSDictionary* rec = [IAPShare toJSON : response];
                                                   if ([rec[@"status"] integerValue] == 0) {
                                                       [_iap provideContentWithTransaction : transcation];
                                                       NSLog(@"SUCCESS %@",response);
                                                       NSLog(@"Pruchases %@",_iap.purchasedProducts);
                                                       // buy success, proceed next step
                                                       Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_RESTORED_REMOVEAD+[productID UTF8String]);
                                                   }
                                                   else {
                                                       [ProgressHUD showError : @"Receipt status is wrong."];
                                                       NSLog(@"Fail in response status is 0");
                                                   }
                                               }
                                               else {
                                                   [ProgressHUD showError : @"No response from Apple.inc."];
                                               }
                                           }];
                                }
                                else if (transcation.transactionState == SKPaymentTransactionStateFailed) {
                                    [ProgressHUD showError : @"Checking receipt failed."];
                                    NSLog(@"Fail in SKPaymentTransactionStateFailed");
                                }
                            }];//end of buy product
        }
    }
}

+ (id)toJSON:(NSString *)json {
    NSError* e = nil;
    id jsonObject = [NSJSONSerialization JSONObjectWithData: [json dataUsingEncoding:NSUTF8StringEncoding]
                                    options: NSJSONReadingMutableContainers
                                      error: &e];
    
    if(e==nil) {
        return jsonObject;
    }
    else {
        NSLog(@"%@",[e localizedDescription]);
        return nil;
    }
    
}

@end
