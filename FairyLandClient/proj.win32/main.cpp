#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include <signal.h>
#include <direct.h>
#include "YYProtocol.h"
USING_NS_CC;
extern void string_init();
extern void test_baseboard_id();

void signalHandler(int signalNum)
{
	char msg[128], *pDesc=NULL;
	switch(signalNum)
	{
	case SIGINT:pDesc = "interrupt";break;
	case SIGILL:pDesc = "illegal instruction";break;
	case SIGFPE:pDesc = "floating point exception";break;
	case SIGSEGV:pDesc = "segment violation";break;
	case SIGTERM:pDesc = "Software termination signal from kill";break;
	case SIGBREAK:pDesc = "Ctrl-Break sequence";break;
	case SIGABRT:pDesc = "abnormal termination triggered by abort call";break;
	}

	sprintf(msg, "Singal %d\n%s",signalNum,pDesc);
	CCMessageBox(msg,"Exception");
}
void InstallExceptionHandler()
{
	signal(SIGINT, signalHandler);
	signal(SIGILL, signalHandler);
	signal(SIGFPE, signalHandler);
	signal(SIGSEGV, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGBREAK, signalHandler);
	signal(SIGABRT, signalHandler);
}

int socket_init()
{
#ifdef WIN32
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);// win sock start up
	if ( ret ) {
		return -1;
	}
#endif
	return 0;
}
int random()
{
	return rand();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	_chdir("..\\Resources");

	InstallExceptionHandler();
	socket_init();
	string_init();
	srand(GetTickCount());

	AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("Fairland 2");
    eglView->setFrameSize(960, 640);
    return CCApplication::sharedApplication()->run();
}

