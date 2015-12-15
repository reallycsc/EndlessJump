//
//  IAPShare.h
//  ;
//
//  Created by Htain Lin Shwe on 10/7/12.
//  Copyright (c) 2012 Edenpod. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "IAPHelper.h"

#include "CommonHeader.h"

@interface IAPShare : NSObject
@property (nonatomic,strong) IAPHelper *iap;

+ (IAPShare *) sharedHelper;

- (void)requestProductWithID:(NSString *)productID isProduction:(BOOL)production;

- (void)buyProductWithID:(NSString *)productID;

+ (id)toJSON:(NSString*)json;

@end
