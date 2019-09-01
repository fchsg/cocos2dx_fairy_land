//
//  ExceptionHandler.m
//  FairyLand
//
//  Created by shang on 14-2-14.
//
//

#import "ExceptionHandler.h"
#include <libkern/OSAtomic.h>
#include <execinfo.h>

NSString * const ExceptionSignalExceptionName    = @"ExceptionName";
NSString * const ExceptionSignalKey              = @"SignalKey";
NSString * const ExceptionAddressesKey           = @"AddressesKey";
volatile int32_t ExceptionCount = 0;
const int32_t   ExceptionMaximum = 10;
const NSInteger ExceptionSkipAddressCount = 4;
const NSInteger ExceptionReportAddressCount = 5;

@implementation ExceptionHandler

+ (NSArray *)backtrace
{
    void* callstack[128];
    int frames = backtrace(callstack, 128);
    char **strs = backtrace_symbols(callstack, frames);
    NSMutableArray *backtrace = [NSMutableArray arrayWithCapacity:frames];
    
    for (int i = ExceptionSkipAddressCount; i < ExceptionSkipAddressCount + ExceptionReportAddressCount; i++)
    {
	 	[backtrace addObject:[NSString stringWithUTF8String:strs[i]]];
    }
    free(strs);
    return backtrace;
}

- (void)alertView:(UIAlertView *)anAlertView clickedButtonAtIndex:(NSInteger)anIndex
{
	if (anIndex == 0)
	{
		dismissed = YES;
	}
}
- (void)handleException:(NSException *)exception
{
    NSString *msg = [NSString stringWithFormat:NSLocalizedString(@"%@",nil),
                        [exception reason],
                        [[exception userInfo] objectForKey:ExceptionAddressesKey]];
    
	UIAlertView *alert = [[[UIAlertView alloc]
                           initWithTitle:NSLocalizedString(@"程序异常", nil)
                           message:msg
                           delegate:self
                           cancelButtonTitle:NSLocalizedString(@"Quit", nil)
                           otherButtonTitles:NSLocalizedString(@"Continue", nil), nil]
                           autorelease];
	[alert show];

	CFRunLoopRef runLoop = CFRunLoopGetCurrent();
	CFArrayRef allModes = CFRunLoopCopyAllModes(runLoop);
	
    while (!dismissed)
	{
		for (NSString *mode in (NSArray *)allModes)
		{
			CFRunLoopRunInMode((CFStringRef)mode, 0.001, false);
		}
	}
	CFRelease(allModes);
	NSSetUncaughtExceptionHandler(NULL);
	signal(SIGABRT, SIG_DFL);
	signal(SIGILL, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGFPE, SIG_DFL);
	signal(SIGBUS, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	
    if ([[exception name] isEqual:ExceptionSignalExceptionName])
	{
		kill(getpid(), [[[exception userInfo] objectForKey:ExceptionSignalKey] intValue]);
	}
	else
	{
		[exception raise];
	}
}
- (void)showMessageHandler:(NSString *)msg
{
    
	UIAlertView *alert = [[[UIAlertView alloc]
                           initWithTitle:NSLocalizedString(@"提示", nil)
                           message:msg
                           delegate:self
                           cancelButtonTitle:NSLocalizedString(@"OK", nil)
                           otherButtonTitles: nil]
                          autorelease];
	[alert show];
    
	CFRunLoopRef runLoop = CFRunLoopGetCurrent();
	CFArrayRef allModes = CFRunLoopCopyAllModes(runLoop);
	
    while (!dismissed)
	{
		for (NSString *mode in (NSArray *)allModes)
		{
			CFRunLoopRunInMode((CFStringRef)mode, 0.001, false);
		}
	}
	CFRelease(allModes);
}
@end
NSString* getAppInfo()
{
    NSString *appInfo = [NSString stringWithFormat:@"App : %@ %@(%@)\nDevice : %@\nOS Version : %@ %@\n",
                         [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleDisplayName"],
                         [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleShortVersionString"],
                         [[NSBundle mainBundle] objectForInfoDictionaryKey:@"CFBundleVersion"],
                         [UIDevice currentDevice].model,
                         [UIDevice currentDevice].systemName,
                         [UIDevice currentDevice].systemVersion];
    NSLog(@"Crash!!!! %@", appInfo);
    return appInfo;
}

static void signalHandler(int signal)
{
	int32_t exceptionCount = OSAtomicIncrement32(&ExceptionCount);
	if (exceptionCount > ExceptionMaximum)
	{
		return;
	}
	
	NSMutableDictionary *userInfo = [NSMutableDictionary
                                     dictionaryWithObject:[NSNumber numberWithInt:signal]
                                     forKey:ExceptionSignalKey];
	NSArray *callStack = [ExceptionHandler backtrace];
	
    [userInfo
        setObject:callStack
        forKey:ExceptionAddressesKey];
	[[[[ExceptionHandler alloc] init] autorelease]
        performSelectorOnMainThread:@selector(handleException:)
        withObject: [NSException
                    exceptionWithName:ExceptionSignalExceptionName
                    reason:[NSString stringWithFormat:
                                NSLocalizedString(@"Signal %d was raised.\n"
                                                  @"%@", nil),
                                signal, getAppInfo()]
        userInfo: [NSDictionary
                   dictionaryWithObject:[NSNumber numberWithInt:signal]
                   forKey:ExceptionSignalKey]]
        waitUntilDone:YES];
}

void InstallExceptionHandler()
{
	signal(SIGABRT, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGSEGV, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGBUS, signalHandler);
	signal(SIGPIPE, signalHandler);
}
extern "C"
void showMessage(char *message)
{

    NSString *msg = [NSString stringWithCString: message encoding:NSUTF8StringEncoding];
        
        
    [[[[ExceptionHandler alloc] init] autorelease]
         performSelectorOnMainThread:@selector(showMessageHandler:)
         withObject: msg
         waitUntilDone:YES];
}
