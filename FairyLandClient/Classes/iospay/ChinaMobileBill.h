//
//  ChinaMobileBill.h
//  Fairyland
//
//  Created by 付辰昊 on 8/27/14.
//
//

#ifndef __Fairyland__ChinaMobileBill__
#define __Fairyland__ChinaMobileBill__


#import <UIKit/UIKit.h>
#import "GameBilling.h"

@interface ChinaMobileBill : NSObject 

+ (GameBilling *)sdkInstance;
- (void) initChinaMobileBillUI;
@end

#endif /* defined(__Fairyland__ChinaMobileBill__) */
