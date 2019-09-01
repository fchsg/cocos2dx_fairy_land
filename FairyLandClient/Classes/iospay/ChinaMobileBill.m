//
//  ChinaMobileBill.cpp
//  Fairyland
//
//  Created by 付辰昊 on 8/27/14.
//
//

#include "ChinaMobileBill.h"

static GameBilling * ChinaMobile_sdk = nil;

void alert(NSString* msg)
{
    UIAlertView* alertView = [[UIAlertView alloc] initWithTitle:nil
                                                        message:msg
                                                       delegate:nil
                                              cancelButtonTitle:@"确定"
                                              otherButtonTitles:nil, nil];
    [alertView show];
    [alertView release];
}

@interface ChinaMobileBill () <GameBillingDelegate>

@end
@implementation ChinaMobileBill

+ (GameBilling *)sdkInstance
{
    if (ChinaMobile_sdk == nil)
    {
        ChinaMobile_sdk = [GameBilling initializeGameBilling];
    }
    return ChinaMobile_sdk;
}

- (id)init
{
	self = [super init];
	if (self)
    {
        [self initChinaMobileBillUI];
	}
	return self;
}

- (void)initChinaMobileBillUI
{
    GameBilling* sdk = [ChinaMobileBill sdkInstance];
    if (sdk)
    {
        sdk.delegate = self;
        [sdk setDialogOrientationMask:UIInterfaceOrientationMaskLandscape];
        [sdk doBillingWithUIAndBillingIndex:@"001" isRepeated:YES cpParam:nil];
    }
}

- (void)onBillingResult:(BillingResultType)resultCode billingIndex:(NSString *)index message:(NSString *)message
{
    NSString* tip = nil;
    if (resultCode == BillingResultType_PaySuccess || resultCode == BillingResultType_PaySuccess_Activated)
    {
        tip = [NSString stringWithFormat:@"-----billingIndex[%@]支付成功", index];
        
    }else if (resultCode == BillingResultType_PayFail) {
        tip = [NSString stringWithFormat:@"-----billingIndex[%@]支付失败,错误信息：%@", index,message];
    }else if (resultCode == BillingResultType_Cancel) {
        tip = [NSString stringWithFormat:@"-----billingIndex[%@]用户取消操作", index];
    }else if (resultCode == BillingResultType_GetVericode_Success) {
        tip = [NSString stringWithFormat:@"-----billingIndex[%@]获取验证码成功", index];
    }else if (resultCode == BillingResultType_GetVericode_Fail) {
        tip = [NSString stringWithFormat:@"-----billingIndex[%@]获取验证码失败,错误信息：%@", index,message];
    }else if (resultCode == BillingResultType_No_NetWork) {
        tip = @"当前网络不可用";
    }else{
        tip = message;
    }
	NSLog(@"回调结果：%@",tip);
    
    alert([NSString stringWithFormat:@"仅供测试用，sdk回调结果： \r\n %@", tip]);
}

@end