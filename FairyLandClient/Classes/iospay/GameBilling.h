//
//  Resource.h
//  GameBillingSDK
//
//  Created by 吴 晓明 on 13-6-6.
//  Copyright (c) 2013年 吴晓明. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#define SYNTHESIZE_SINGLETON_FOR_CLASS(classname) \
\
static classname *initialize##classname = nil; \
\
+ (classname *)initialize##classname \
{ \
@synchronized(self) \
{ \
if (initialize##classname == nil) \
{ \
initialize##classname = [[self alloc] init]; \
} \
} \
\
return initialize##classname; \
} \
\
+ (id)allocWithZone:(NSZone *)zone \
{ \
@synchronized(self) \
{ \
if (initialize##classname == nil) \
{ \
initialize##classname = [super allocWithZone:zone]; \
return initialize##classname; \
} \
} \
\
return nil; \
} \
\
- (id)copyWithZone:(NSZone *)zone \
{ \
return self; \
} \
\

typedef enum{
    //支付成功，有正常的网络交互
    BillingResultType_PaySuccess = 0x1000,
    //非重复计费点，已成功付过一次费用的情况，不会产生网络交互。此参数意味着计费点已激活。
    //如果有合法的cp透传参数，在这种情况下不需要去服务器验证付费结果
    BillingResultType_PaySuccess_Activated,
    //支付失败
    BillingResultType_PayFail ,
    //用户取消计费行为
    BillingResultType_Cancel ,
    //获取验证码成功
    BillingResultType_GetVericode_Success,
    //获取验证码失败
    BillingResultType_GetVericode_Fail,
    //当前网络不可用
    BillingResultType_No_NetWork,
    //其他错误信息
    BillingResultType_Other,
    
} BillingResultType;


@protocol GameBillingDelegate;

@interface GameBilling : NSObject
{}

@property (nonatomic,assign) id <GameBillingDelegate> delegate;

/**
 初始化SDK
 */
+ (GameBilling *)initializeGameBilling;
/**
 初始化SDK
 可自定义UI界面显示的游戏名称、游戏提供商、客服电话，一般来说计费文件已集成此三个信息，非必要情况下不建议使用该方法。
 @param gameName 游戏名称
 @param provider 游戏提供商
 @param serviceTel 客服电话
 */
+ (GameBilling *)initializeGameBillingWithGameName:(NSString *)gameName provider:(NSString *)provider serviceTel:(NSString *)serviceTel;

/**
 *  购买结果本地保存功能及接口
 *  @param billingIndex 《三位》计费点Id
 *  @param state 已计费:YES,未计费:NO
 
 注：该接口仅适用于强制计费点。
 
 */
- (void)setPayState:(NSString *)billingIndex withState:(BOOL)state;

/**
 *  购买结果本地获取功能及接口
 *  @param billingIndex 《三位》计费点Id
 *  @returns 已计费:YES,未计费:NO
 
 注：该接口仅适用于强制计费点。
 
 */
- (BOOL)getPayState:(NSString *)billingIndex;

/**
 *联网方式计费 获取短信验证码(无UI)
 *@param    billingIndex 三位计费点Id
 *@param    phoneNum    付费中国移动手机号
 */
- (void)getVeriCodeWithBillingIndex:(NSString *)billingIndex phoneNum:(NSString *)phoneNum;
/**
 *联网方式计费(无UI)
 *@param    billingIndex 三位计费点Id
 *@param    isRepeated  是否重复计费点
 *@param    phoneNum    付费中国移动手机号
 *@param    veriCode    付费手机短信验证码
 *@param    cpParam     CP透传参数（**可选参数**），只允许数字和大小写字母，长度必须是16位。如果不需要该参数请传nil
 */
-(void)doBillingByNetWithBillingIndex:(NSString *)billingIndex isRepeated:(BOOL)isRepeated phoneNum:(NSString *)phoneNum veriCode:(NSString *)veriCode cpParam:(NSString*)cpParam;

/**
 *  联网计费方法(带UI)
 *  @param billingIndex 三位计费点Id
 *  @param isRepeated   是否重复计费点
 *  @param cpParam      CP透传参数（**可选参数**），只允许数字和大小写字母，长度必须是16位。如果不需要该参数请传nil
 */
- (void)doBillingWithUIAndBillingIndex:(NSString *)billingIndex isRepeated:(BOOL)isRepeated cpParam:(NSString*)cpParam;

/**
 *  设置UI计费对话框支持的方向，默认UIInterfaceOrientationMaskAll。
 如果游戏只支持横屏或者只支持竖屏，请一定要设置相应的UI计费对话框的方向
 
 例：
 
 横屏
 [_sdk setDialogOrientationMask:UIInterfaceOrientationMaskLandscape];
 竖屏
 [_sdk setDialogOrientationMask:UIInterfaceOrientationMaskPortrait];
 */
-(void)setDialogOrientationMask:(UIInterfaceOrientationMask)orientationMask;




@end


/**
 计费委托
 */
@protocol GameBillingDelegate<NSObject>
@required
- (void)onBillingResult:(BillingResultType)resultCode billingIndex:(NSString *)index message:(NSString *)message;
@end



/**
 用户行为日志上报
 */

typedef void(^GameBillingUploadLogCallBack)(BOOL success,NSDictionary* userInfo);

@interface CMGAMELogManager : NSObject

/**
 *  设置是否打印日志，用于开发调试场景
 *  @param isLogEnabled 是否打开控制台日志输出功能,默认NO
 */
+ (void) setIsLogEnabled:(BOOL)isLogEnabled;

/**
 
 *  触发某个事件
 
 *  @param eventId
 自定义监控事件,比如记录用户点击购买道具按钮的使用行为
 注：该值最大长度为10
 *  @param pageId
 需要收集信息的某个页面标识
 注：该值最大长度为10
 *  @param callBack
 事件处理完毕后的回调，不需要回调可传nil
 参数success，上传成功返回YES,上传失败返回NO
 参数userInfo，sdk返回的其他信息，供扩展，暂时固定返回nil
 
 */

+ (void) onEvent:(NSString*)eventId pageId:(NSString*)pageId callBack:(GameBillingUploadLogCallBack)callBack;
/**
 *  启动游戏事件，必须集成。
 注：SDK初始化的时候会自动调用一次onStartGame
 *  @param callBack
 事件处理完毕后的回调，不需要回调可传nil
 参数success，上传成功返回YES,上传失败返回NO
 参数userInfo，sdk返回的其他信息，供扩展，暂时固定返回nil
 
 */
+ (void) onStartGame:(GameBillingUploadLogCallBack)callBack;


/**
 *  退出游戏事件，必须集成。下面两个方法功能一样，二选一或组合使用。
 *  @param exitDate
 退出游戏时的时间
 *  @param callBack
 事件处理完毕后的回调，不需要回调可传nil
 参数success，上传成功返回YES,上传失败返回NO
 参数userInfo，sdk返回的其他信息，供扩展，暂时固定返回nil
 
 */
+ (void) onExitGame:(GameBillingUploadLogCallBack)callBack;
+ (void) onExitGameWithExitDate:(NSDate* )exitDate callBack:(GameBillingUploadLogCallBack)callBack;

/**
 *  进入某个页面
 *  @param pageId  页面标识id。
 注：该值最大长度为10
 
 */
+ (void) enterinPage:(NSString*)pageId;

/**
 *  退出某个页面
 *  @param pageId   页面标识id。
 注：该值最大长度为10
 */
+ (void) exitPage:(NSString*)pageId;
@end

