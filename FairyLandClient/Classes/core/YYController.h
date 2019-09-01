// 
//  YYController.h
//  XJ
// 
//  Created by fuchenhao on 13-5-17. 
// 
// 

#ifndef __XJ__YYController__
#define __XJ__YYController__

#include <cocos2d.h>
#include "AssetsManager/AssetsManager.h"
#include "../common/ODSocket.h"
#include "YYGlobalData.h"
#include "YYConfig.h"

// ---------游戏状态----------

enum GameStatus
{
    kStatusDefault = -1,
    kStatusLogo = 0,		// 游戏Logo
    kStatusCover,           // 封面
    kStatusLogin,			// 登陆界面
    kStatusLoading,			// 游戏加载状态
    kStatusScene,			// 主城
    kStatusUI,				// 用户界面
    kStatusDead,			// 死亡确认
    kStatusDisconnect		// 断线重连
};

enum TipType
{
    kTipDefault = -1,
    kTipNormal = 0,// 普通提示信息(只需确认)
    kTipDeadInfo = 1,// 死亡提示信息(选择是否复活)
    kTipRoleDeleted = 2,// 角色删除提示
    kTipRoleDeletedCancel = 3,//角色删除取消提示
    kTipRemoveProtoss,//删除星灵
    kTipGoldCharge,//金币充值
    kTipJewCharge,//宝石充值
    kTipSwallowProtoss,//吞噬星灵
    kTipMergeProtoss,//合并星灵
    kTipGoldChargeForShopUI, // 商城界面金币充值
    kTipGoChargeForChargeUI, // 充值界面充值
    kTipHaseePayNomal,
    kTipGoChargeForHaseePayUI,
    kTipPlayTableDungeonReset, //活动_地下城 重置提示
    kTipPlayTableWorldBossKill, //活动_世界boss boss 击杀提示
    kTipCoverConnectFailed, // cover 联网失败 退出游戏
    kTipWipeDungeon, // 扫荡副本提示
    kTipWipeProtoss, // 扫荡星灵副本
    kTipExit//退出提示
};

enum ScriptStatus
{
    kScriptStatusNull = 0, // 无脚本运行
    kScriptStatusRunning = 1,// 脚本正在运行
    kScriptStatusPause = 2, // 暂停脚本
    kScriptStatusOver = 3 //脚本运行结束
};

enum DungeonType
{
    kDungeonDefault = -1,
    kDungeonNormal  = 0,   // 正常副本
    kDungeonDungeon = 1,   // 活动_地下城
    kDungeonProtoss = 2,    // 活动_守卫星灵
    kDungeonBoss = 3,       // 活动_BOSS 副本
    kWorldBoss = 4       // 活动_世界boss
};

#define MAIN_CITY 0 // 主城 
#define DUNGEONS 1 // 副本 
#define WAR_ZONE 2 // 新手战斗区域
#define NEWBIE_TASK_ZONE 3// 新手任务区

#define ROLE_MAX 3
typedef struct _RoleInfo_
{
	std::string roleName;//名称
    uint8_t     posIndex;//位置序号（1,2,3）
	uint16_t	professionIndex;//职业序号（1,2,3）
	int32_t		roleLevel;//等级
    int32_t		deleteTime;//删除角色需等待的时间
}RoleInfo;

typedef struct _HeartBeatInfo_
{
    int32_t registerConnectCnt; // 注册服务器连接计数器
    int32_t registerTimeIntervalCnt; // 注册服务器时间间隔计数器
    int32_t gameConnectCnt; // 游戏服务器务器连接计数器
    int32_t gameTimeIntervalCnt;   // 游戏服务器时间间隔计数器
    
}HeartBeatInfo;

#define HEARTBEAT_TIME_INTERVAL 30 // 心跳连接间隔 30s 模拟器测试暂时调整12s
#define TAG_GUIDE 25 // 新手指引教程

class Logo;
class Cover;
class LoginUI;
class Loading;
class YYScene;
class YYDungeonsUI;
class CreateRoleUI;
class YYTaskTargetData;
class GlobalData;

typedef void (cocos2d::CCObject::*CMD_HANDLER)(cocos2d::CCArray *);
#define commondHandle(_SELECTOR) (CMD_HANDLER)(&_SELECTOR)

enum kLoadingOverStatus// loading结束后状态
{
    kLOSDefault = -1,
    kLOSLogin = 0,
    kLOSCreateRole = 1,// loading后进入创建角色界面
    kLOSChooseRole = 2,// loading后进入选择角色界面
    kLOSGameScene// loading后进入游戏场景
};

class Controller : public cocos2d::CCObject,public cocos2d::CCStandardTouchDelegate,public cocos2d::CCKeypadDelegate,public SocketReciever
{
private:
    
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pMessagePool,MessagePool);// 接受服务器发送信息的池 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pOrderPool,OrderPool);// 指令池
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pScriptPool, ScriptPool);// 脚本池
    CC_SYNTHESIZE(uint8_t, m_u8ProfessionIndex, ProfessionIndex);// 职业序号 
    CC_SYNTHESIZE(GameStatus, m_ePreGameStatus, PreGameStatus);// 之前的游戏状态
    CC_SYNTHESIZE(uint8_t, m_u8SceneType, SceneType);// 场景类型 0不可战斗区域 1可战斗区域 
    CC_SYNTHESIZE(float, m_fSceneCellTopY, SceneCellTopY);// 场景逻辑单元格顶端Y坐标 
    CC_SYNTHESIZE(int32_t, m_nCurrentTime, CurrentTime); //
    CC_SYNTHESIZE(ScriptStatus, m_eScriptStatus, ScriptStatus);//脚本运行状态
    CC_SYNTHESIZE(std::string , m_sServerHost,ServerHost);
    CC_SYNTHESIZE(uint16_t , m_u16ServerPort,ServerPort);
    CC_SYNTHESIZE(YYScene *, m_pScene, Scene);
    CC_SYNTHESIZE(uint8_t, m_nRoleIndex, RoleIndex);
    CC_SYNTHESIZE(std::string, m_sRandomName, RandomName);
    CC_SYNTHESIZE(std::string, m_sConnectMessage, ConnectMessage);// 建立socke连接后发送的信息
    CC_SYNTHESIZE(uint8_t, m_u8LineIndex, LineIndex);// 服务器线路索引
    CC_SYNTHESIZE(bool, m_bRegServerFailed, RegServerFailed);// 注册服务器连接状态
    CC_SYNTHESIZE(HeartBeatInfo, m_sHeartBeatInfo, HeartBeatInfo); // 联网心跳连接信息
    
private:

    bool m_bNeedUpdate;//是否需要更新
    cocos2d::extension::AssetsManager * m_pAssetsManager;//资源管理器
    std::string packageURL;
    std::string pathToSave;

    ServerItem *m_pServerItem;
    GameStatus m_eGameStatus;// 游戏状态
    kLoadingOverStatus m_eLoadingOverStatus;// 加载结束后的游戏状态
    cocos2d::CCScheduler *m_pScheduler;
    bool m_bRunning;
    bool m_bReturnLogin;
    bool m_bJudgeLoadingEndByScript;//是否用脚本判断加载是否结束
    bool m_bLoadingEnd;//是否结束loading
    ODSocket *m_pSocket;
    YYDungeonsUI *m_pDungeonsUI;
    cocos2d::CCDictionary *m_pCmdHandlerDict;// 存储与指令相关的函数指针
    cocos2d::CCDictionary *m_pTargetDict;// 存储与指令相关的被调用的函数所属类的对象
    bool m_bWaitingReceiveMessage;// 等待接收消息
    int m_nConnectCounter;// 连线计数器 
    int m_nConnectDuration;// 连线持续时间 
    std::string m_sFilename;
    cocos2d::CCLabelTTF*  m_pReconnectionLabel; //重新连接标签
    int m_nReconnectTime; //重连等待时间 
    cocos2d::CCLayer * m_pReconnectTips;  //提示框层
    int m_nPauseCounter,m_nPauseDuration;
    std::string m_orderString;
	RoleInfo m_RoleInfo[3];
    bool m_bTutorial; // 新手引导教程
    bool m_bTutorialOnMainUI;//
    bool m_bTrial;   //是否进入试炼模式
    DungeonType m_eDungeonType; // 进入副本模式
    int m_nNetLoadingFrameCounter;//网络连接计时
    int m_nNetLoadingDuration;//网络通讯持续时长
    
public:
    
    Controller();
    ~Controller();
    
    static Controller * getInstance();
    void run();
    bool init();
    void switchGameStatus(GameStatus status);
    void setGameStatus(GameStatus status);
    GameStatus getGameStatus()const;
    
    void schedule(cocos2d::SEL_SCHEDULE selector);
    void schedule(cocos2d::SEL_SCHEDULE selector, float interval);
    void schedule(cocos2d::SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay);
    void scheduleOnce(cocos2d::SEL_SCHEDULE selector, float delay);
    void unschedule(cocos2d::SEL_SCHEDULE selector);
    
    void update(float f);
    
    //cocos2d::CCStandardTouchDelegate
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    
    //cocos2d::CCKeypadDelegate
    virtual void keyBackClicked();
    virtual void keyMenuClicked();
    
    virtual void onExit();

    /** 联网相关 */
    inline bool isNeedUpdate()
    {
        return m_bNeedUpdate;
    }
    
    inline void setNeedUpdate(bool isUpdate)
    {
        m_bNeedUpdate = isUpdate;
    }
    void setAssetsManager(cocos2d::extension::AssetsManagerDelegateProtocol *delegate);
    void verifyVersion(cocos2d::CCArray *array);//版本校验
    bool connectRegister();// 链接注册服务器
    void connectGame();// 登录
    void gotologinUI(); // 进入loginUI
	void askLogout(bool bReturnLogin = true);//登出
    static void * connectServer(void *arg);//链接指定的服务器
    void closeConnect();
    void connectCounter();
    void disConnect();
    void reconnectionInfo(); // 重新连接
    void sendMessage(const char * message,bool isNetLoading);
    void processReceivedMessages();
    void logout(cocos2d::CCArray *array);
    void loginError(cocos2d::CCArray *array);
    
    void heartBeat(); // 心跳连接
    // SocketReciever
    virtual void receivedMessage(const char * data, int dataLen);

    inline int getRoleIndex()
    {
        return m_nRoleIndex;
    }
    inline bool isJudgeLoadingEndByScript()
    {// 加载信息接收完毕
        return m_bJudgeLoadingEndByScript;
    }
    
    inline void setJudgeLoadingEndByScript(bool isByScript)
    {// 加载信息接收完毕
        m_bJudgeLoadingEndByScript = isByScript;
    }
    
    inline bool isLoadingEnd()
    {// 加载信息接收完毕 
        return m_bLoadingEnd;
    }
    
    inline void setLoadingEnd(bool isOver)
    {// 加载信息接收完毕
        m_bLoadingEnd = isOver;
    }
    
    inline bool isWaitingReceiveMessage()
    {
        return m_bWaitingReceiveMessage;
    }
    inline void setSelectedServer(ServerItem *serverItem)
    {
        CC_SAFE_RETAIN(serverItem);
        CC_SAFE_RELEASE(m_pServerItem);
        m_pServerItem = serverItem;
    }
    
    inline ServerItem *getSelectedServer()
    {
        return m_pServerItem;
    }
    
    inline void setTutorial(bool isGuide)
    {
        m_bTutorial = isGuide;
    }
    
    inline bool isTutorial()
    {
        return m_bTutorial;
    }
    
    void setLoadingOverStatus(kLoadingOverStatus status);
    
    inline kLoadingOverStatus getLoadingOverStatus()
    {
        return m_eLoadingOverStatus;
    }
    void loadOver();// 加载结束
    struct tm *getSystemTime();
    int getCurrentSecond(); // 获取系统时间（返回值以秒为单位）
    const char * getCalendar();//获取日期
    cocos2d::CCArray * createArray(std::string message, uint16_t charIndex,char sign);
    void onMenuClicked(cocos2d::CCObject *sender ,  char * setUrl_ ,  char * childpath , char * filename);
    void onHttpRequestCompleted(cocos2d::CCNode* node, void* data);
    cocos2d::CCSprite * getSpriteFromWritablePath(const char* name);

    void returnLoginUI(bool showTips);
    static bool isButtonTouched(cocos2d::CCNode * node,cocos2d::CCPoint point);
    bool parameterVerify(cocos2d::CCArray* array, int parametersNum);// 参数校验
    void callScriptMethod(std::string& message);//调用脚本方法
    void askNextScriptCommond();
    cocos2d::CCImage * newCCImage(const char * path);
    
    cocos2d::CCSpriteBatchNode * drawNumber(int num,cocos2d::CCTexture2D * texture,cocos2d::CCPoint position, cocos2d::CCPoint anchor,int interval,cocos2d::CCLayer * parent,int zOrder);
    
    cocos2d::CCRenderTexture * createStroke(cocos2d::CCLabelTTF *label, float size, cocos2d::ccColor3B color);
	bool isRoleCreated(int roleIndex);
	bool isRoleCreatable(int roleIndex);
    RoleInfo *getRoleInfo(int index);
	void clearRoleInfo();
	void createRole(int professionIndex, std::string &roleName);
	void deleteRole();
	void chooseRole();
    void addLoadingMessageToArray(const char * message);// 添加进入loading状态的信息
    bool isTouchGuideRect(cocos2d::CCPoint touchPoint);
    void addCmdHandler(CCObject *target, const char *cmd, CMD_HANDLER cb);
    
    // 试炼
    inline bool isTrial()
    {
        return m_bTrial;
    }
    
    inline void setTrial(bool _isTrial)
    {
        m_bTrial = _isTrial;
    }
    
    // 进入副本类型
    inline DungeonType getDungeonType()
    {
        return m_eDungeonType;
    }
    
    inline void setDungeonType(DungeonType type)
    {
        m_eDungeonType = type;
    }
    
    inline const char* getPlayerID()
    {
        const char* playerID = cocos2d::CCString::createWithFormat("%s.%d",Config::getInstance()->getUserID().c_str(),m_RoleInfo[m_nRoleIndex].posIndex)->getCString();
        return playerID;
    }
    // 重置心跳连接数据
    void resetHeartBeatInfo();
private:
    void createDownloadedDir();
    
    void commondBinding();
    void updatePropertyInfo();// 更新财产状况

	// 协议处理相关函数
    void onCmdRegHeartBeatConnect(cocos2d::CCArray *arr);
    void onCmdGameHeartBeatConnect(cocos2d::CCArray *arr);
    void onCmdLogin(cocos2d::CCArray *arr);
    void onCmdReconnect(cocos2d::CCArray *arr);
	void onCmdRoleInfo(cocos2d::CCArray *arr);
    void onCmdRandomRoleName(cocos2d::CCArray *arr);
    void onCmdCreateRole(cocos2d::CCArray *arr);
    void onCmdRemoveRole(cocos2d::CCArray *arr);
    void onCmdAddTaskTargetData(cocos2d::CCArray *arr);// 添加任务目标数据
    void onCmdRemoveTaskData(cocos2d::CCArray *arr);
    void onCmdErrorMessage(cocos2d::CCArray *arr);//错误信息提示
    void onCmdSetPlayerAttribute(cocos2d::CCArray *arr);//设置玩家属性
    void onCmdAddEquip(cocos2d::CCArray *arr);
    void onCmdAddProp(cocos2d::CCArray *arr);
    bool isReceiveMessageOver(std::string &message);// 接收信息结束
    void receiveTreatureContainerData(cocos2d::CCArray *arr);
    void receiveSkillSQ(cocos2d::CCArray *arr);// 接收技能数据
    void receiveTalentSV(cocos2d::CCArray *arr);// 接收天赋数据
};

class ProtocolHandler : public cocos2d::CCObject
{

public:
    
    CMD_HANDLER _handler;
    
	ProtocolHandler()
    {
    }
    
	ProtocolHandler(CMD_HANDLER cb)
	{
		_handler = cb;
	}
};
#endif /* defined(__XJ__YYController__) */
