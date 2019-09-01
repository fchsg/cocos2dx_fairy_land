//
//  Controller.cpp
//  XJ
//
//  Created by fuchenhao on 13-5-17.
//
//

#include "YYController.h"
#include "../animation/YYSprite.h"
#include "../animation/YYRect.h"
#include "../common/FileReadManager.h"
#include "../extensions/network/HttpClient.h"
#include "YYLogo.h"
#include "YYCover.h"
#include "YYLoginUI.h"
#include "YYScene.h"
#include "YYLoading.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYControlUILayer.h"
#include "YYPopUpUI.h"
#include "YYTaskTargetData.h"
#include "YYButton.h"
#include "YYNPC.h"
#include "YYGuide.h"
#include "YYShopData.h"
#include "YYProtossData.h"
#include "YYLoginAwardData.h"
#include "YYTaskData.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/stat.h>
#endif

USING_NS_CC;
//  using namespace CocosDenshion;

#define TOUCH_BEGIN 0
#define TOUCH_MOVE 1
#define TOUCH_END 2

Controller::Controller()
:m_eGameStatus(kStatusDefault)
,m_ePreGameStatus(kStatusDefault)
,m_fSceneCellTopY(0)
,m_u8LineIndex(0)
,m_u8SceneType(0)
,m_u8ProfessionIndex(0)
,m_nReconnectTime(0)
,m_nConnectCounter(0)
,m_nConnectDuration(200)
,m_nNetLoadingDuration(160)
,m_nNetLoadingFrameCounter(0)
,m_bRunning(false)
,m_bReturnLogin(false)
,m_bLoadingEnd(false)
,m_bJudgeLoadingEndByScript(false)
,m_bTutorialOnMainUI(false)
,m_bTutorial(false)
,m_pScheduler(NULL)
,m_pMessagePool(NULL)
,m_pOrderPool(NULL)
,m_pScene(NULL)
,m_pDungeonsUI(NULL)
,m_pSocket(NULL)
,m_pReconnectTips(NULL)
,m_pReconnectionLabel(NULL)
,m_pServerItem(NULL)
,m_pAssetsManager(NULL)
,m_bWaitingReceiveMessage(false)
,m_bNeedUpdate(true)
,m_eScriptStatus(kScriptStatusNull)
,m_nPauseCounter(0)
,m_nPauseDuration(0)
,m_nRoleIndex(0)
,m_bTrial(false)
,m_bRegServerFailed(false)
,m_eDungeonType(kDungeonDefault)
{
    m_pMessagePool = new CCArray(5);
    m_pOrderPool = new CCArray(50);
    m_pScriptPool = new CCArray(50);
    m_pCmdHandlerDict = new CCDictionary();
    m_pTargetDict = new CCDictionary();
    m_pScene = YYScene::create();
    m_pScene->retain();
}

static Controller * s_Controller = NULL;
static SocketStatus m_ControllerSocketStatus = kSocketServerClosed;
Controller * Controller::getInstance()
{
    if (s_Controller == NULL)
    {
        s_Controller = new Controller();
    }
    return s_Controller;
}

Controller::~Controller()
{
    CC_SAFE_DELETE(m_pAssetsManager);
    CC_SAFE_RELEASE(m_pScheduler);
    CC_SAFE_DELETE(m_pMessagePool);
    CC_SAFE_DELETE(m_pOrderPool);
    CC_SAFE_DELETE(m_pScriptPool);
    CC_SAFE_RELEASE(m_pScene);
    CC_SAFE_RELEASE(m_pServerItem);
}

bool Controller::init()
{
    m_pScheduler = CCDirector::sharedDirector()->getScheduler();
    m_pScheduler->retain();
    m_bRunning = true;
    this->schedule(schedule_selector(Controller::update),0.0f);
    //  添加触摸协议
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this, 1);
    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(this);
    //   initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    //   turn on display FPS
     pDirector->setDisplayStats(true);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    pDirector->setAnimationInterval(1.0 / 20);
#endif
    // android 设置帧率无效 需在 org.cocos2dx.lib Cocos2dxRenderer 文件修改
    ResolutionPolicy resolutionPolicy;
    if((pDirector->getWinSize().width / pDirector->getWinSize().height) < (float)STANDARD_SCENE_WIDTH / STANDARD_SCENE_HEIGHT)
    {
        resolutionPolicy = kResolutionFixedWidth;
    }
    else
    {
        resolutionPolicy = kResolutionFixedHeight;
    }
    //  屏幕适配
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(STANDARD_SCENE_WIDTH, STANDARD_SCENE_HEIGHT, resolutionPolicy);
    m_fSceneCellTopY = LOGIC_LAYER_HEIGHT;//  场景逻辑单元格最顶端到屏幕下边缘的高度
    clearRoleInfo();
    resetHeartBeatInfo();
    return true;
}

void Controller::run()
{
    if (init())
    {
        CCDirector::sharedDirector()->runWithScene(m_pScene);
        switchGameStatus(kStatusLogo);
//        createDownloadedDir();
        commondBinding();//绑定命令
        Config::getInstance()->loadConfig();
    }
}

// 重置心跳连接数据
void Controller::resetHeartBeatInfo()
{
    m_sHeartBeatInfo.registerConnectCnt = 0;
    m_sHeartBeatInfo.registerTimeIntervalCnt = 0;
    m_sHeartBeatInfo.gameConnectCnt = 0;
    m_sHeartBeatInfo.gameTimeIntervalCnt = 0;
}

/**
 * 处理接受消息
 */
void Controller::commondBinding()
{
    addCmdHandler(this, "RGV", commondHandle(Controller::verifyVersion));// 登录成功
    addCmdHandler(Config::getInstance(), "RSL", commondHandle(Config::setGameServer));//服务器列表
    addCmdHandler(this, "RLS", commondHandle(Controller::onCmdLogin));// 登录成功
    addCmdHandler(this, "RFT", commondHandle(Controller::onCmdErrorMessage));//错误信息提示
    addCmdHandler(this, "LO", commondHandle(Controller::logout));//登出 触动退出
    addCmdHandler(this, "LF", commondHandle(Controller::logout));//登出 登录错误
    addCmdHandler(this, "LR", commondHandle(Controller::loginError));//登录时的错误提示
    addCmdHandler(this, "RO", commondHandle(Controller::onCmdRoleInfo));//角色信息
    addCmdHandler(this, "LS", commondHandle(Controller::onCmdReconnect));// 断线重连
    addCmdHandler(this, "RID",commondHandle(Controller::onCmdRegHeartBeatConnect));// 注册服务器心跳连接
    addCmdHandler(this, "ID", commondHandle(Controller::onCmdGameHeartBeatConnect));// 游戏服务器心跳连接
    addCmdHandler(this, "RC", commondHandle(Controller::onCmdRandomRoleName));//随机角色姓名
    addCmdHandler(this, "RT", commondHandle(Controller::onCmdCreateRole));//创建角色成功
    addCmdHandler(this, "TB", commondHandle(Controller::onCmdAddTaskTargetData));
    addCmdHandler(this, "TC", commondHandle(Controller::onCmdRemoveTaskData));
    addCmdHandler(this, "BP", commondHandle(Controller::onCmdSetPlayerAttribute));//设置玩家属性
    addCmdHandler(this, "FT", commondHandle(Controller::onCmdErrorMessage));//错误信息提示
    addCmdHandler(this, "CO", commondHandle(Controller::receiveTreatureContainerData));// 处理返回的宝箱数据
    addCmdHandler(this, "RD", commondHandle(Controller::onCmdRemoveRole));//角色删除
    addCmdHandler(this, "LH", commondHandle(Controller::onCmdAddEquip));// 添加装备
    addCmdHandler(this, "LG", commondHandle(Controller::onCmdAddProp));// 添加道具
    addCmdHandler(GlobalData::getInstance(), "LA", commondHandle(GlobalData::sortProp));// 背包物品排序
    addCmdHandler(GlobalData::getInstance(), "LB", commondHandle(GlobalData::sortEquip));// 背包装备排序
    addCmdHandler(GlobalData::getInstance(), "LM", commondHandle(GlobalData::replaceEquip));// 用新属性替换旧的装备属性
    addCmdHandler(m_pScene, "AA", commondHandle(YYScene::onCmdLoadScene)); // 加载场景
    addCmdHandler(m_pScene, "AS", commondHandle(YYScene::setSceneData));    //设置场景大小及逻辑碰撞单元格大小
    addCmdHandler(m_pScene, "AG", commondHandle(YYScene::setBlockTile));// 设置有阻碍的单元格
    addCmdHandler(m_pScene, "IA", commondHandle(YYScene::receiveDungeonsData));//副本选择界面
    addCmdHandler(m_pScene, "IC", commondHandle(YYScene::receiveStageData));//关卡选择界面
    addCmdHandler(m_pScene, "IE", commondHandle(YYScene::receiveStageAwardData));//关卡奖励及难度确认界面
    addCmdHandler(m_pScene, "AO", commondHandle(YYScene::addSprite));//添加精灵
    addCmdHandler(m_pScene, "AP", commondHandle(YYScene::removeSprite));//删除精灵
    addCmdHandler(m_pScene, "AM", commondHandle(YYScene::processSpriteMove));//处理精灵移动
    addCmdHandler(m_pScene, "AQ", commondHandle(YYScene::setDropOdds));// 设置药品掉率
    addCmdHandler(m_pScene, "AZ", commondHandle(YYScene::setRoleControlled));// 设置可控角色
    addCmdHandler(m_pScene, "CL", commondHandle(YYScene::updateEquipComposeUI));//处理返回的装备合成数据
    addCmdHandler(m_pScene, "LL", commondHandle(YYScene::treasureContainerErrorTips));//
    addCmdHandler(m_pScene, "LI", commondHandle(YYScene::onCmdPutonEquip));// 换装
    addCmdHandler(m_pScene, "LJ", commondHandle(YYScene::reinforceFailedTips));//强化失败
    addCmdHandler(m_pScene, "LK", commondHandle(YYScene::forgeFailedTips));//锻造失败
    addCmdHandler(m_pScene, "NT",commondHandle(YYScene::receiveNPCTaskList));// 接收NPC任务列表
    addCmdHandler(m_pScene, "RE", commondHandle(YYScene::processRoleRelive));//处理角色复活
    addCmdHandler(m_pScene, "CT", commondHandle(YYScene::receiveChatMessage));//接收聊天信息
    addCmdHandler(m_pScene, "DL", commondHandle(YYScene::editItemNumber));// 修改物品数量
    addCmdHandler(m_pScene, "LP", commondHandle(YYScene::receivePropDetail));//道具信息
    addCmdHandler(m_pScene, "LN", commondHandle(YYScene::receiveEquipDetail));//装备信息
    addCmdHandler(m_pScene, "UM", commondHandle(YYScene::forgeByJewel));//花费宝石锻造
    addCmdHandler(m_pScene, "TA", commondHandle(YYScene::receiveTaskData));//添加任务
    addCmdHandler(m_pScene, "UL", commondHandle(YYScene::equipReinforce));//强化装备
    addCmdHandler(m_pScene, "TP", commondHandle(YYScene::getRankDataInfo));//排行榜
    addCmdHandler(m_pScene, "TL", commondHandle(YYScene::getTrainDataInfo));//试炼
    
    //技能
    addCmdHandler(this, "SQ", commondHandle(Controller::receiveSkillSQ));     // 玩家拥有技能数据
    addCmdHandler(m_pScene, "SP", commondHandle(YYScene::setRoleSkillIndex)); // 玩家正在使用技能数据
    addCmdHandler(this, "SV", commondHandle(Controller::receiveTalentSV));    // 天赋数据
    
    // 商城
    addCmdHandler(m_pScene, "SH" , commondHandle(YYScene::setShopData));//商城数据
    addCmdHandler(m_pScene, "SF" , commondHandle(YYScene::receiveAllShopDataFinished));//接受数据结束

    // 充值
    addCmdHandler(m_pScene, "PL" , commondHandle(YYScene::setChargeData)); // 充值数据
    addCmdHandler(m_pScene, "PS", commondHandle(YYScene::setChargeSerialNubmer)); // 订单号
    
    // 登陆奖励
    addCmdHandler(m_pScene, "SO", commondHandle(YYScene::setSignUpData)); // 登陆签到数据
    addCmdHandler(m_pScene, "SU", commondHandle(YYScene::setSignUpFinishData)); // 签到奖励数据
    addCmdHandler(m_pScene, "SR", commondHandle(YYScene::setSignUpBoxAwardData)); // 宝箱数据
    
    // 活跃度
    addCmdHandler(m_pScene, "AC" , commondHandle(YYScene::receiveActivityData)); // 接收活跃度数据
    
    // 成就
    addCmdHandler(m_pScene, "AL", commondHandle(YYScene::receiveAchievementData)); //  接收成就信息
    addCmdHandler(m_pScene, "AF", commondHandle(YYScene::setAchievementFinishedTipData)); // 弹出成就完成信息
    addCmdHandler(m_pScene, "AR", commondHandle(YYScene::setAchievementGetAwardData)); // 成就奖励
    
    // 占灵
    addCmdHandler(m_pScene, "PA", commondHandle(YYScene::setProtossWaitRecData)); // 拾取星灵信息
    addCmdHandler(m_pScene, "PB", commondHandle(YYScene::setProtossWaitRecOneData)); // 拾取单个星灵信息
    addCmdHandler(m_pScene, "PC", commondHandle(YYScene::setProtossBagData)); // 背包星灵信息
    addCmdHandler(m_pScene, "PD", commondHandle(YYScene::setProtossBagOneData)); // 背包单个星灵信息
    addCmdHandler(m_pScene, "PE", commondHandle(YYScene::setProtossEquipBagData)); // 装备星灵信息
    addCmdHandler(m_pScene, "PF", commondHandle(YYScene::setProtossEquipBagData)); // 装备单个星灵信息
    addCmdHandler(m_pScene, "PG", commondHandle(YYScene::setLightConstellationData)); // 点亮星座
    addCmdHandler(m_pScene, "PH", commondHandle(YYScene::setProtossPieceData)); // 碎片数量
    
    // 邮件
    addCmdHandler(m_pScene, "MS", commondHandle(YYScene::setMailNum));
    addCmdHandler(m_pScene, "ML", commondHandle(YYScene::setMailListData)); // 邮件列表
    addCmdHandler(m_pScene, "MR", commondHandle(YYScene::setMailDetailData)); // 读取邮件信息
    
    //公告
    addCmdHandler(m_pScene, "NL", commondHandle(YYScene::receiveNoticeInfo)); // 接收公告信息
    
    //用户中心
    addCmdHandler(m_pScene, "CS", commondHandle(YYScene::receiveUserCenterInfo)); // 接收用户中心信息
    
    //活动
    addCmdHandler(m_pScene, "AH", commondHandle(YYScene::receivePlayTableAH));// 活动_获取活动页面信息
    addCmdHandler(m_pScene, "AI", commondHandle(YYScene::receivePlayTableAI));// 活动_获取活动是否开启信息
    addCmdHandler(m_pScene, "TT", commondHandle(YYScene::receivePlayTableTT));// 活动_获取转盘页面信息
    addCmdHandler(m_pScene, "TR", commondHandle(YYScene::receivePlayTableTR));// 活动_返回转盘领取信息
    addCmdHandler(m_pScene, "TD", commondHandle(YYScene::receivePlayTableTD));// 活动_获取骰子页面信息
    addCmdHandler(m_pScene, "DR", commondHandle(YYScene::receivePlayTableDR));// 活动_返回骰子领取信息
    addCmdHandler(m_pScene, "DO", commondHandle(YYScene::receiveDungeonDO));// 活动_获取地下城页面信息
    addCmdHandler(m_pScene, "DA", commondHandle(YYScene::receiveDungeonDA));// 活动_返回地下城奖励信息
    addCmdHandler(m_pScene, "TO", commondHandle(YYScene::receiveProtossTO));// 活动_获取守卫星灵页面信息
    addCmdHandler(m_pScene, "TW", commondHandle(YYScene::receiveProtossTW));// 活动_返回守卫星灵奖励信息
    addCmdHandler(m_pScene, "BL", commondHandle(YYScene::receiveBossDungeonBL));// 活动_boss副本UI信息
    addCmdHandler(m_pScene, "BC", commondHandle(YYScene::receiveBossDungeonBC));// 活动_boss副本宝石信息
    addCmdHandler(m_pScene, "BA", commondHandle(YYScene::receiveBossDungeonBA));// 活动_boss副本奖励信息
    addCmdHandler(m_pScene, "WC", commondHandle(YYScene::receiveWorldBossWC));// 活动_世界bossUI信息
    addCmdHandler(m_pScene, "WT", commondHandle(YYScene::receiveWorldBossWT));// 活动_世界boss消除cd时间
    addCmdHandler(m_pScene, "WB", commondHandle(YYScene::receiveWorldBossWB));// 活动_世界boss增加祝福Buff
    addCmdHandler(m_pScene, "WK", commondHandle(YYScene::receiveWorldBossWK));// 活动_世界boss接收击杀boss消息
    addCmdHandler(m_pScene, "WD", commondHandle(YYScene::receiveWorldBossWD));// 活动_世界boss接收击是否可以打死boss的信息
    
}

void Controller::setGameStatus(GameStatus status)
{
    if (m_eGameStatus == status)
    {
        return;
    }
    m_ePreGameStatus = m_eGameStatus;
    m_eGameStatus = status;
}

void Controller::switchGameStatus(GameStatus status)
{
    setGameStatus(status);
    switch (m_eGameStatus)
    {
        case kStatusLogo:
            m_pScene->addLogo();
            break;
        case kStatusCover:
            m_pScene->addCover();
        {
            const std::string filePath = getFilePath(COVER_MUSIC ,kDirTypeSound ,kFileSuffixMp3);
            PLAY_BGMUSIC(filePath.c_str());
        }
            break;
        case kStatusLogin:
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            m_pScene->addLoginUI();
        {
            const std::string filePath = getFilePath(COVER_MUSIC ,kDirTypeSound ,kFileSuffixMp3);
            PLAY_BGMUSIC(filePath.c_str());
        }
            break;
        case kStatusLoading:
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            m_pScene->addLoadingLayer();
            break;
        case kStatusScene:
        {
            if (getSceneType() == DUNGEONS || getSceneType() == WAR_ZONE)
            {
                const std::string filePath = getFilePath(BATTLE_MUSIC1 ,kDirTypeSound ,kFileSuffixMp3);
                PLAY_BGMUSIC(filePath.c_str());
            }
            else
            {
                const std::string filePath = getFilePath(MAINCITY_MUSIC ,kDirTypeSound ,kFileSuffixMp3);
                PLAY_BGMUSIC(filePath.c_str());
                if (m_pScene->getControlUILayer())
                {
                    m_pScene->getControlUILayer()->updateTaskGuide();
                    m_pScene->getControlUILayer()->updateRoleWealth();
                }
            }
        }
            break;
        default:
            break;
    }

}

GameStatus Controller::getGameStatus()const
{
    return m_eGameStatus;
}

void Controller::schedule(SEL_SCHEDULE selector)
{
    this->schedule(selector, 0.0f, kCCRepeatForever, 0.0f);
}

void Controller::schedule(SEL_SCHEDULE selector, float interval)
{
    this->schedule(selector, interval, kCCRepeatForever, 0.0f);
}

void Controller::schedule(cocos2d::SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay)
{
    CCAssert( selector, "Argument must be non-nil");
    CCAssert( interval >=0, "Argument must be positive");
    
    m_pScheduler->scheduleSelector(selector, this, interval , repeat, delay, !m_bRunning);
}

void Controller::scheduleOnce(SEL_SCHEDULE selector, float delay)
{
    this->schedule(selector, 0.0f, 0, delay);
}

void Controller::unschedule(cocos2d::SEL_SCHEDULE selector)
{
    //   explicit nil handling
    if (selector == 0)
        return;
    
    m_pScheduler->unscheduleSelector(selector, this);
}

#pragma mark 游戏主循环

void Controller::update(float f)
{
    if (m_bReturnLogin && m_eGameStatus > kStatusLogin)
    {
        m_bReturnLogin = false;
        returnLoginUI(true);
    }
    processReceivedMessages();
    switch (m_eGameStatus)
    {
        case kStatusLogo:
        case kStatusCover:
        case kStatusLogin:
        case kStatusLoading:
        case kStatusUI:
        case kStatusScene:
        case kStatusDead:
            m_pScene->update(f);
            break;
        case kStatusDisconnect:
            reconnectionInfo();
            break;
        default:
            break;
    }
    if (m_eGameStatus != kStatusLogo && m_eGameStatus != kStatusCover)
    {
        connectCounter();
    }
    heartBeat();
}

#pragma mark 触控
void Controller::ccTouchesBegan(CCSet *touches,CCEvent * event)
{
    switch (m_eGameStatus)
    {
        case kStatusCover:
        case kStatusUI:
        case kStatusScene:
        case kStatusDead:
            m_pScene->touchBegan(touches);
            break;
        default:
            break;
    }
}

void Controller::ccTouchesMoved(CCSet * touches,CCEvent *event)
{
    switch (m_eGameStatus)
    {
        case kStatusUI:
        case kStatusScene:
        case kStatusDead:
            m_pScene->touchMoved(touches);
            break;
        default:
            break;
    }
}

void Controller::ccTouchesEnded(CCSet * touches,CCEvent *event)
{
    switch (m_eGameStatus)
    {
        case kStatusUI:
        case kStatusScene:
        case kStatusDead:
            m_pScene->touchEnded(touches);
            break;
        default:
            break;
    }
}

void Controller::keyBackClicked()
{
    m_pScene->showTips(g_ExitTip,kTipExit);
}

void Controller::keyMenuClicked()
{
    
}

void Controller::onExit()
{
     SimpleAudioEngine::sharedEngine()->end();
}

#pragma mark 联网

void Controller::verifyVersion(CCArray *array)
{
    m_pScene->setNetLoading(false);
    packageURL.clear();
    int index = 0;
    CCObject * obj = getNextElement(array, index);
    
    do{
        CC_BREAK_IF(!obj);
        std::string version = static_cast<CCString *>(obj)->getCString();
        if(version.compare(Config::getInstance()->getVersion()) == 0)
        {
            m_bNeedUpdate = false;
            break;
        }
        Config::getInstance()->setVersion(version);
        obj = getNextElement(array, index);
        CC_BREAK_IF(!obj);
        packageURL = static_cast<CCString *>(obj)->getCString();
    }while (0);
    
    Cover *cover = dynamic_cast<Cover *>(m_pScene->getTempLayer());
    if (cover)
    {
        if (m_bNeedUpdate)
        {
            cover->setCoverStatus(kCoverStatusDownload);
            setAssetsManager(cover);
        }
        else
        {
            cover->setCoverStatus(kCoverStatusNormal);
            cover->getTipSprite()->setVisible(true);
            GlobalData::getInstance()->readData();
        }
    }
}

void Controller::gotologinUI()
{
    Cover *cover = dynamic_cast<Cover *>(m_pScene->getTempLayer());
    if (cover)
    {
        cover->setCoverStatus(kCoverStatusNormal);
        cover->getTipSprite()->setVisible(true);
        GlobalData::getInstance()->readData();
    }
}

// 心跳连接
void Controller::heartBeat()
{
    m_pSocket = ODSocket::getInstance();
    if (m_pSocket && m_pSocket->getDelegate() && m_pSocket->getNetStatus() == kNetStatusConnected)
    {
        SocketStatus scoketType = m_pSocket->getSocketStatus();
        switch (scoketType) {
            case kSocketServerRegister:
            {
                m_sHeartBeatInfo.registerTimeIntervalCnt++;
                //CCLOG("registerTimeIntervalCnt %d",m_sHeartBeatInfo.registerTimeIntervalCnt);
                if (m_sHeartBeatInfo.registerTimeIntervalCnt >= HEARTBEAT_TIME_INTERVAL*FRAME_PER_SECOND)
                {
                    m_sHeartBeatInfo.registerConnectCnt++;
                    //CCLOG("registerConnectCnt++ %d", m_sHeartBeatInfo.registerConnectCnt);
                    m_sHeartBeatInfo.registerTimeIntervalCnt = 0;
                    sendMessage(ASK_HEARTBEAT, false);
                    if (m_sHeartBeatInfo.registerConnectCnt >= 2)
                    {
                        // 断线重连
                        
                    }
                }
            }
                break;
            case kSocketServerGame:
            {
                m_sHeartBeatInfo.gameTimeIntervalCnt++;
                //CCLOG("gameTimeIntervalCnt %d",m_sHeartBeatInfo.gameTimeIntervalCnt);
                if (m_sHeartBeatInfo.gameTimeIntervalCnt >= HEARTBEAT_TIME_INTERVAL*FRAME_PER_SECOND)
                {
                    m_sHeartBeatInfo.gameConnectCnt++;
                    //CCLOG("gameConnectCnt++ %d", m_sHeartBeatInfo.gameConnectCnt);
                    m_sHeartBeatInfo.gameTimeIntervalCnt = 0;
                    sendMessage(ASK_HEARTBEAT, false);
                    if (m_sHeartBeatInfo.gameConnectCnt >= 2)
                    {
                        // 断线重连
                        if(!m_bWaitingReceiveMessage)
                        {
                            m_sHeartBeatInfo.gameConnectCnt = 0;
                            switchGameStatus(kStatusDisconnect);
                            disConnect();
                        }
                        
                    }
                }
            }
                break;
            default:
                break;
        }
    }
    
    return;
}

// 链接游戏服务器
void Controller::connectGame()
{
	std::string loginInfo = Protocol::getLoginMessage(Config::getInstance()->getLoginInfo());
    setConnectMessage(loginInfo);
    switchGameStatus(kStatusLoading);
    m_sServerHost = m_pServerItem->serverHost;
    m_u16ServerPort = m_pServerItem->serverPort;
    pthread_t newThreadID;
    m_ControllerSocketStatus = kSocketServerGame;
    int threadStatus = startRequestThread(&newThreadID, Controller::connectServer, this);
    if (threadStatus)
    {//  0成功，失败返回其他值
        returnLoginUI(true);
    }
}

//链接注册服务器获取游戏服务器列表
bool Controller::connectRegister()
{
	ServerItem listServer(Config::getInstance()->getListServer().c_str());
    m_sServerHost = listServer.serverHost;
    m_u16ServerPort = listServer.serverPort;
    pthread_t newThreadID;
    m_ControllerSocketStatus = kSocketServerRegister;
    int threadStatus = startRequestThread(&newThreadID, Controller::connectServer,this);
    if (threadStatus)
    {//  0成功，失败返回其他值
        m_pScene->showTips(g_sNetworkError);
        return false;
    }
    return true;
}

// 链接服务器 线程回调函数
void * Controller::connectServer(void *arg)
{
	Controller *controller = (Controller*)arg;
    controller->m_pSocket = ODSocket::getInstance();
    controller->m_pSocket->setDelegate(controller);
    controller->m_pSocket->setSocketStatus(m_ControllerSocketStatus);
	controller->m_pSocket->Connect(controller->getServerHost().c_str(),controller->getServerPort());
    if (controller->m_pSocket->getNetStatus() == kNetStatusConnected)
    {
        if (!controller->getConnectMessage().empty())
        {
            controller->sendMessage(controller->getConnectMessage().c_str(),false);
            controller->getConnectMessage().clear();
        }
    }
    else
    {
        controller->m_bRegServerFailed = true; // 联网失败
    }
    return NULL;
}

// 关闭Socket网络连接
void Controller::closeConnect()
{
    m_bWaitingReceiveMessage = false;
    m_pSocket->Close();
}


// 请求登出
void Controller::askLogout(bool bReturnLogin)
{
    sendMessage(ASK_LOGOUT, false);
	if(bReturnLogin)
		returnLoginUI(false);
}


// 确认登出
void Controller::logout(cocos2d::CCArray *array)
{
    returnLoginUI(false);
    if (array->count() > 0)
    {
        const char *msg = static_cast<CCString *>(array->objectAtIndex(0))->getCString();
        if (msg)
        {
            m_pScene->showTips(msg);
        }
    }
}

// 登录页面错误
void Controller::loginError(cocos2d::CCArray *array)
{
    if (array->count() > 0)
    {
        const char *msg = static_cast<CCString *>(array->objectAtIndex(0))->getCString();
        if (msg)
        {
            m_pScene->showTips(msg);
        }
    }
}

// 创建角色
void Controller::createRole(int professionIndex, std::string &roleName)
{
	CCString * cmd = CCString::createWithFormat("%s_%d_%d_%s",ASK_ROLE_CREATE,m_nRoleIndex + 1,professionIndex,roleName.c_str());
    sendMessage(cmd->getCString(), false);
}


// 删除角色
void Controller::deleteRole()
{
	CCString * cmd = CCString::createWithFormat("%s_%d",ASK_ROLE_DELETE,m_nRoleIndex + 1);
    sendMessage(cmd->getCString(),false);
}

// 选择角色
void Controller::chooseRole()
{
    CCString * cmd = CCString::createWithFormat("%s_%d",ASK_ROLE_SELECT,m_nRoleIndex + 1);
    sendMessage(cmd->getCString(),false);
    switchGameStatus(kStatusLoading);
	return;
}

// 网络连接阻塞计数
void Controller:: connectCounter()
{
    if(m_pSocket && m_pSocket->getNetStatus() == kNetStatusConnected)
    {
        if(m_bWaitingReceiveMessage)
        {
            if(m_nConnectCounter < m_nConnectDuration)
            {
                m_nConnectCounter++;
            }
            else
            {
                m_nConnectCounter = 0;
                CCLOG("connectCounter disConnect");
                disConnect();
            }
        }
        if (m_pScene->isNetLoading())
        {
            if (m_nNetLoadingFrameCounter < m_nNetLoadingDuration)
            {
                m_nNetLoadingFrameCounter++;
            }
            else
            {
                m_nNetLoadingFrameCounter = 0;
                m_pScene->setNetLoading(false);
            }
        }
    }
    else if(m_pSocket && m_pSocket->getNetStatus() == kNetStatusDisConnect && m_bWaitingReceiveMessage)
    {
        CCLOG("force disConnect");
        m_nConnectCounter = 0;
        disConnect();
        m_nNetLoadingFrameCounter = 0;
        m_pScene->setNetLoading(false);
    }
}

// 断网重连UI界面
void Controller::disConnect()
{
    switch (m_eGameStatus)
    {
        case kStatusLoading:
        case kStatusLogin:
             returnLoginUI(true);
             break;
        case kStatusScene:
        case kStatusUI:
        case kStatusDead:
        {
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            m_pReconnectTips = CCLayer::create();
            std::string filePath = getFilePath("u0112", kDirTypePic, kFileSuffixPng);
            drawBackground(filePath.c_str(), winSize.width / 2 - 100,winSize.height/2 - 100 , 200, 200, m_pReconnectTips,3);
            m_nReconnectTime = 30;
            m_nCurrentTime = getCurrentTime();
            m_pReconnectionLabel = CCLabelTTF::create(CCString::createWithFormat(g_sWaitReconnectFmt, m_nReconnectTime)->getCString(), DEFAULT_FONT_NAME, 24);
            m_pReconnectionLabel->setColor(ccRED);
            m_pReconnectionLabel->setPosition(ccp(winSize.width / 2,winSize.height / 2));
            m_pReconnectTips->addChild(m_pReconnectionLabel);
            m_pScene->addChild(m_pReconnectTips,17);
            closeConnect();
            resetHeartBeatInfo();
            switchGameStatus(kStatusDisconnect);
        }
            break;
        default:
            returnLoginUI(true);
            break;
    }
}

// 断网重连
void Controller::reconnectionInfo()
{
    int temp = getCurrentSecond();
    if(temp != m_nCurrentTime)
    {
        m_nReconnectTime --;
        if(m_nReconnectTime % 5 == 0)
        {
            pthread_t connectThreadID;
            setConnectMessage(Protocol::getReconnectMessage(Config::getInstance()->getLoginInfo(),m_nRoleIndex + 1));
            startRequestThread(&connectThreadID,connectServer,this);
        }
        m_nCurrentTime = temp;
        m_pReconnectionLabel ->setString(CCString::createWithFormat(g_sWaitReconnectFmt, m_nReconnectTime)->getCString());
        if(m_nReconnectTime == 0)
        {
            if(m_pReconnectTips) // 删除显示面板
            {
                m_pReconnectTips->removeFromParentAndCleanup(true);
            }
            returnLoginUI(true);
        }
    }
}

#pragma mark 接收socket代理回调消息

void Controller::receivedMessage(const char * data, int dataLen)
{
	uint8_t *recvBuf = (uint8_t*)data;
    if (recvBuf[0] == 0xff && recvBuf[1] == 0xfd && recvBuf[8] == 0x22)	//skip FF FD 18 FF FD 1F FF FD 22
	{	// 过滤socket连接建立后服务器返回的9字节无用信
        recvBuf += 9;
		dataLen -= 9;
    }
	if(dataLen <= 0)
		return;
	m_orderString.append((char*)recvBuf);
	int index = m_orderString.rfind(0x0a);
	if(index <= 0)
		return;
	std::string ss = m_orderString.substr(0,index+1);
	m_orderString = m_orderString.substr(index+1);
    m_pMessagePool->addObject(CCString::create(ss.c_str()));
    m_nConnectCounter = 0;
}

#pragma mark 向服务器发送信息

void Controller::sendMessage(const char * message,bool isNetLoading)
{
    m_bWaitingReceiveMessage = true;
    if (ODSocket::getInstance()->getNetStatus() != kNetStatusConnected)
    {
        CCLOG("sendMessage disConnect");
        disConnect();
        return;
    }
    if (isNetLoading)
    {
        m_pScene->setNetLoading(true);
        addLoadingMessageToArray(message);
    }
    std::string buffer = message;
    buffer.append("\n");
    m_pSocket->Send(buffer.c_str(),buffer.length(),0);
}

#pragma mark 处理从服务器接收到的信息

void Controller::processReceivedMessages()
{
    if (m_pMessagePool)
    {
        for (int i = 0; i < m_pMessagePool->count();)
        {
            CCString * string = (CCString*)m_pMessagePool->objectAtIndex(i);
            if (string)
            {
                CCArray *array = split_string(string->getCString(), '\n');
                m_pOrderPool->addObjectsFromArray(array);
                m_pMessagePool->removeObjectAtIndex(i);
            }
            else
            {
                i++;
            }
        }
    }
    if (m_pOrderPool)
    {
        while (m_pOrderPool->count() > 0)
        {
            std::string message = static_cast<CCString *>(m_pOrderPool->objectAtIndex(0))->getCString();
            CCLOG("orderPool current process Message :%s",message.c_str());
            if(message.empty() || isReceiveMessageOver(message))
            {
                m_pOrderPool->removeObjectAtIndex(0);
                continue;
            }
            if (m_bWaitingReceiveMessage)
            {
                if (message.find(RECEIVE_ERR_MSG) != 0 || getGameStatus() != kStatusLoading)
                {// loading状态下接收到错误提示FT
                    m_bWaitingReceiveMessage = false;
                }
            }

            if(message.find(RECEIVE_LOADING_OVER) == 0)
            {
                if (!m_bJudgeLoadingEndByScript)
                {
                    m_bLoadingEnd = true;
                    if (m_pScene->getLoadingLayer()->isProgressCompleted())// 进度条100%
                    {
                        loadOver();
                    }
                }
                if (getGameStatus() == kStatusLoading)//loading状态下不处理FN后面的信息
                {
                    break;
                }
            }
            else if (message.find(RECEIVE_EXCLUDE_CARD) == 0)
            {// 排除奖励后信息
                m_pScene->getPopUpUI()->startShuffleCard();
            }
            else if (message.find(RECEIVE_TURN_UP_CARD) == 0)
            {// 抽奖信息

                CCArray *pArr = split_string(message.c_str(),'_');
                pArr->removeObjectAtIndex(0);
                m_pScene->getPopUpUI()->getAwardForChoiceCard(pArr);
            }
            else
            {
                ProtocolHandler *phandler;
                CCArray *array = split_string(message.c_str(),'_');
                if (array)
                {
                    std::string cmd = static_cast<CCString *>(array->objectAtIndex(0))->getCString();
                    phandler = static_cast<ProtocolHandler *>(m_pCmdHandlerDict->objectForKey(cmd));
                    if(phandler)
                    {
                        array->removeObjectAtIndex(0);
                        (m_pTargetDict->objectForKey(cmd)->*(phandler->_handler))(array);
                    }
                    else
                    {//脚本相关
                        if(message.compare(RECEIVE_RUN_SCRIPT) == 0)
                        {
                            m_bJudgeLoadingEndByScript = true;
                        }
                        m_pScriptPool->addObject(m_pOrderPool->objectAtIndex(0));
                    }
                }
            }
            m_pOrderPool->removeObjectAtIndex(0);
        }
    }

    //暂停执行脚本命令
    if (m_eScriptStatus == kScriptStatusPause)
    {
        if (m_nPauseDuration > 0)
        {
            if (m_nPauseDuration > m_nPauseCounter)
            {
                m_nPauseCounter ++;
            }
            else
            {
                m_nPauseDuration = 0;
                m_eScriptStatus = kScriptStatusRunning;
            }
        }
        return;
    }
    if (m_pScriptPool)
    {// 脚本指令池
        while(m_pScriptPool->count() > 0)
        {
            std::string script = static_cast<CCString *>(m_pScriptPool->objectAtIndex(0))->getCString();
            CCLOG("scriptPool current process Message :%s",script.c_str());
            callScriptMethod(script);
            m_pScriptPool->removeObjectAtIndex(0);
            if(getScriptStatus() == kScriptStatusPause)
            {
                break;
            }
        }
    }
}

void Controller::setLoadingOverStatus(kLoadingOverStatus status)
{
    m_eLoadingOverStatus = status;
    
}

void Controller::loadOver()
{
    if (getLoadingOverStatus() == kLOSDefault) //解决android 手机进副本UI界面 状态切换错误问题
    {
        return;
    }
    if(getLoadingOverStatus() == kLOSGameScene)
    {
        switchGameStatus(kStatusScene);
        m_pScene->showGameScene();
        CC_SAFE_RELEASE_NULL(m_pServerItem);
        if (m_pScene)
        {
            m_pScene->setCurrentTime(getCurrentSecond());
        }
    }
    else
    {
        switchGameStatus(kStatusLogin);
        LoginUI * loginUI = dynamic_cast<LoginUI *>(m_pScene->getTempLayer());
        if (loginUI)
        {
            if(getLoadingOverStatus() == kLOSLogin)
            {
                if (getSelectedServer())
                {
                    loginUI->showUILayer(kPanelTypeLogin);
                }
                else
                {
                    loginUI->showUILayer(kPanelTypeChooseServer);
                }
            }
            else if(getLoadingOverStatus() == kLOSCreateRole)
            {
                loginUI->showUILayer(kPanelTypeCreateRole);
            }
            else if(getLoadingOverStatus() == kLOSChooseRole)
            {
                loginUI->showUILayer(kPanelTypeChooseRole);
            }
        }
    }
    m_bLoadingEnd = false;
    m_bJudgeLoadingEndByScript = false;
}

#pragma mark 协议处理相关函数

void Controller::addCmdHandler(CCObject *target, const char *cmd, CMD_HANDLER cb)
{
	ProtocolHandler *handler = new ProtocolHandler(cb);
	if(handler == NULL)
	{
		CCLOG("addCmdHandler failed, %s", cmd);
		return;
	}
	handler->autorelease();
	m_pCmdHandlerDict->setObject(handler, cmd);
    m_pTargetDict->setObject(target, cmd);
}

/**
 * 登录成功
 */
void Controller::onCmdLogin(cocos2d::CCArray *arr)
{
    // 注册服务器验证登录成功
    std::string userID = "",password = "";
    int i = 0;
    do{
        CCString * str = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!str);
        userID = str->getCString();
        str = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!str);
        password = str->getCString();
        Config::getInstance()->setUserID(userID);
        Config::getInstance()->setPassword(password);
    }while (0);
    askLogout(false);
    closeConnect();
    if (!password.empty())
    {
        connectGame();
    }
}

/**
 * 断线重连
 */

void Controller::onCmdReconnect(cocos2d::CCArray *array)
{
    if (m_nReconnectTime > 0)
    {
        m_nReconnectTime = 0;
        int status = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
        if (status == 0)
        {//重连失败
            returnLoginUI(true);
        }
        else
        {
            m_pScene->removeChild(m_pReconnectTips);
            switchGameStatus(kStatusScene);
            if (m_pScene->isNetLoading())
            {
                m_pScene->setNetLoading(false);
                m_nNetLoadingFrameCounter = 0;
            }
        }
    }
}

void Controller::onCmdRegHeartBeatConnect(cocos2d::CCArray *arr)
{
    m_sHeartBeatInfo.registerConnectCnt--;
    CCLOG("registerConnectCnt-- %d",m_sHeartBeatInfo.registerConnectCnt);
}

void Controller::onCmdGameHeartBeatConnect(cocos2d::CCArray *arr)
{
    m_sHeartBeatInfo.gameConnectCnt--;
    CCLOG("gameConnectCnt-- %d",m_sHeartBeatInfo.gameConnectCnt);
}

void Controller::onCmdRoleInfo(cocos2d::CCArray *arr)
{
	if(arr)
	{
        clearRoleInfo();
        for(int i = 0;i < arr->count();i++)
        {
            CCString * roleInfo = (CCString *)arr->objectAtIndex(i);
            CCArray *args = split_string(roleInfo->getCString(),';');
            if(args)
            {
                int j = 0;
                do{
                    CCString * tempStr = static_cast<CCString *>(getNextElement(args, j));
                    CC_BREAK_IF(!tempStr);
                    m_RoleInfo[i].deleteTime = tempStr->intValue();
                    tempStr = static_cast<CCString *>(getNextElement(args, j));
                    CC_BREAK_IF(!tempStr);
                    m_RoleInfo[i].roleName = tempStr->getCString();
                    tempStr = static_cast<CCString *>(getNextElement(args, j));
                    CC_BREAK_IF(!tempStr);
                    m_RoleInfo[i].professionIndex = atoi(tempStr->getCString());
                    tempStr = static_cast<CCString *>(getNextElement(args, j));
                    CC_BREAK_IF(!tempStr);
                    m_RoleInfo[i].roleLevel = tempStr->intValue();
                    m_RoleInfo[i].posIndex = i+1;
                }while (0);
            }
            if (getGameStatus() == kStatusLogin)
            {
                LoginUI *loginUI = dynamic_cast<LoginUI *>(m_pScene->getTempLayer());
                if (loginUI && loginUI->getPanelType() == kPanelTypeChooseRole)
                {
                    loginUI->showRolePanel(i, getRoleInfo(i));
                    m_pScene->setNetLoading(false);
                    m_nNetLoadingFrameCounter = 0;
                }
            }
        }
        if (getGameStatus() == kStatusLoading)
        {
            m_bLoadingEnd = true;
            m_eLoadingOverStatus = kLOSChooseRole;
        }
	}
}

void Controller::onCmdRandomRoleName(cocos2d::CCArray *arr)
{
    if (getGameStatus() == kStatusLoading)
    {
        m_eLoadingOverStatus = kLOSCreateRole;
        m_bLoadingEnd = true;
    }
    const char *name = static_cast<CCString *>(arr->objectAtIndex(0))->getCString();
    setRandomName(name);
    if(getGameStatus() == kStatusLogin)
    {
        LoginUI *loginUI = dynamic_cast<LoginUI *>(m_pScene->getTempLayer());
        if (loginUI && loginUI->getPanelType() == kPanelTypeCreateRole)
        {
            loginUI->setRoleName(name);
            m_pScene->setNetLoading(false);
            m_nNetLoadingFrameCounter = 0;
        }
    }
}

void Controller::onCmdCreateRole(cocos2d::CCArray *arr)
{
    do{
        int i = 0;
        CCString *tempStr = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!tempStr);
        m_RoleInfo[m_nRoleIndex].posIndex = tempStr->intValue();
        tempStr = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!tempStr);
        m_RoleInfo[m_nRoleIndex].roleName = tempStr->getCString();
        tempStr = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!tempStr);
        m_RoleInfo[m_nRoleIndex].professionIndex = tempStr->intValue();
        tempStr = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!tempStr);
        m_RoleInfo[m_nRoleIndex].roleLevel = tempStr->intValue();
        switchGameStatus(kStatusLoading); // 角色创建成功 切换为loading
    }while (0);
}

/**
 *角色删除
 */
void Controller::onCmdRemoveRole(cocos2d::CCArray *arr)
{
    int i = 0;
    do{
        CCString *tempStr = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!tempStr);
        int index = tempStr->intValue() - 1;
        tempStr = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!tempStr);
        int delTime = tempStr->intValue();
        m_RoleInfo[index].deleteTime = delTime;
        if (delTime <= 0)
        {
            m_RoleInfo[index].professionIndex = 0;
        }
        LoginUI *loginUI = dynamic_cast<LoginUI *>(m_pScene->getTempLayer());
        if (loginUI && loginUI->getPanelType() == kPanelTypeChooseRole)
        {
            loginUI->setDeleteInfo(index);
            m_pScene->setNetLoading(false);
            m_nNetLoadingFrameCounter = 0;
        }
    }while (0);
}

void Controller::onCmdSetPlayerAttribute(cocos2d::CCArray *array)
{
    int i = 0;
    const char * spriteID = ((CCString *)array->objectAtIndex(i))->getCString();
    GlobalData::getInstance()->getPlayerAttributeDict()->setObject(array, spriteID);
    if (m_pScene)
    {
        if (m_pScene->getSpriteLayer())
        {
            m_pScene->getSpriteLayer()->setPlayerAttribute(spriteID, array);
        }
    }
    if(getGameStatus() == kStatusUI)
    {
        m_pScene->getPopUpUI()->updatePropertyInfo();
        if (m_pScene->getPopUpUI()->getUIType() == kUITalent) //更新天赋点数据
        {
            m_pScene->getPopUpUI()->updatePlayerTalentPointLabel();
        }
        else if(m_pScene->getPopUpUI()->getUIType() == kUIVip)
        {
            m_pScene->getPopUpUI()->updateVipUI();
        }
    }
    if(getSceneType() == MAIN_CITY || getSceneType() == NEWBIE_TASK_ZONE) // 更新主界面 金币宝石数
    {
        m_pScene->getControlUILayer()->updateRoleWealth();
    }
}

void Controller::onCmdAddProp(cocos2d::CCArray *array)
{
    GlobalData::getInstance()->addProperty(array);
    if (getGameStatus() == kStatusUI && m_pScene->getPopUpUI()->getUIType() == kUIBag)
    {
        m_pScene->getPopUpUI()->updateItemIconInBag(false);
    }
}

void Controller::onCmdAddEquip(cocos2d::CCArray *array)
{
    GlobalData::getInstance()->addEquipment(array);
    if (getGameStatus() == kStatusUI && m_pScene->getPopUpUI()->getUIType() == kUIBag)
    {
        m_pScene->getPopUpUI()->updateItemIconInBag(true);
    }
}

void Controller::receiveSkillSQ(cocos2d::CCArray *arr) // 接收角色技能数据
{
    const char * playerID = CCString::createWithFormat("%s.%d",Config::getInstance()->getUserID().c_str(),m_RoleInfo[m_nRoleIndex].posIndex)->getCString();
    GlobalData::getInstance()->receiveSkillSQ(arr,playerID);
    
    if (kStatusUI == Controller::getInstance()->getGameStatus() && m_pScene && kUISkill == m_pScene->getPopUpUI()->getUIType())   //UI 更新技能描述
    {
        m_pScene->getPopUpUI()->updateSkillDescription();
    }
}

void Controller::receiveTalentSV(cocos2d::CCArray *arr) // 接收角色天赋数据
{
    GlobalData::getInstance()->receiveTalentData(arr);
    if (kStatusUI == Controller::getInstance()->getGameStatus() && m_pScene && kUITalent == m_pScene->getPopUpUI()->getUIType())   //UI 更新天赋描述
    {
        m_pScene->getPopUpUI()->updateTalentUI();
    }
}

bool Controller::isReceiveMessageOver(std::string &message)
{
     if (m_pScene && m_pScene->isNetLoading())
     {
         for (int i = 0 ; i < GlobalData::getInstance()->getSetLoadingMessageArray()->count(); i++)
         {
             const char * loadingMessage = static_cast<CCString *>(GlobalData::getInstance()->getSetLoadingMessageArray()->objectAtIndex(i))->getCString();
             if (message.compare(loadingMessage) == 0)
             {
                 m_pScene->setNetLoading(false);
                 m_nNetLoadingFrameCounter = 0;
                 GlobalData::getInstance()->getSetLoadingMessageArray()->removeObjectAtIndex(i);
                 return true;
             }
         }
     }
    return false;
}

RoleInfo *Controller::getRoleInfo(int index)
{
	if(index < ROLE_MAX)
		return &m_RoleInfo[index];
	return NULL;
}

void Controller::clearRoleInfo()
{
	m_nRoleIndex = 0;
	for(int i = 0;i < ROLE_MAX;i++)
	{
		m_RoleInfo[i].posIndex = 0;
		m_RoleInfo[i].roleName = "";
		m_RoleInfo[i].professionIndex = 0;
		m_RoleInfo[i].roleLevel = 0;
        m_RoleInfo[i].deleteTime = 0;
	}
}

/**
 *  是否已经创建了人物角色
 */
bool Controller::isRoleCreated(int roleIndex)
{
	return m_RoleInfo[roleIndex].professionIndex != 0;
}

/**
 *  是否可以创建人物角色
 */
bool Controller::isRoleCreatable(int roleIndex)
{
	return m_RoleInfo[roleIndex].professionIndex == 0;
}

/*
 任务类型：1
 TB[任务ID]_[任务类型]_[按钮状态]_[寻路目标]_[目标icon]_[NPCID]
 任务类型：2
 TB[任务ID]_[任务类型]_[按钮状态]_[寻路目标]_[目标icon]_[[杀人需求,已杀数量];[杀人需求,已杀数量]]
 任务类型：3
 TB[任务ID]_[任务类型]_[按钮状态]_[寻路目标]_[目标icon]_[[收集需求,已收数量];[收集需求,已收数量]]
 任务类型：4
 TB[任务ID]_[任务类型]_[按钮状态]_[寻路目标]_[目标icon]_[[副本需求,已打数量];[副本需求,已打数量]]
 任务类型：5
 TB[任务ID]_[任务类型]_[按钮状态]_[寻路目标]_[目标icon]_[等级需求]
 */

void Controller::onCmdAddTaskTargetData(cocos2d::CCArray *array)
{
    int taskIndex = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
    TaskData * taskData = dynamic_cast<TaskData *>(GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex));
    if(taskData)
    {
        taskData->setTargetType(((CCString *)array->objectAtIndex(1))->intValue());//目标类型 1对话 2杀怪 3收集 4通关 5升级
        taskData->setStatus(((CCString *)array->objectAtIndex(2))->intValue());// 任务状态
        taskData->setTargetTileIndex(((CCString *)array->objectAtIndex(3))->intValue());
        taskData->setTargetIcon(((CCString *)array->objectAtIndex(4))->getCString());
        if (m_pScene->getPopUpUI())
        {
            if(m_pScene->getPopUpUI()->isWaitingForUpdateTask())
            {
                if (taskData->getTaskIndex() / 10000000 < 2)
                {// 主线任务编号第一位为1，支线任务编号第一位为2
                    if (m_pScene->getPopUpUI())
                    {
                        m_pScene->getPopUpUI()->setWaitingForUpdateTask(false);
                        m_pScene->getPopUpUI()->setAcceptedTaskIndex(taskIndex);
                    }
                    if (m_pScene->getSpriteLayer())
                    {
                        m_pScene->getSpriteLayer()->getRoleByControlled()->taskAutoFindingWay(taskData,true);
                    }
                    if (m_pScene->getControlUILayer())
                    {
                        m_pScene->getControlUILayer()->updateGuideButton(taskIndex);
                    }
                }
            }
            if(m_pScene->getPopUpUI()->isWaitingForFindTarget())
            {     // 一键操作需求 等到下一个任务继续连上一个操作
                m_pScene->getPopUpUI()->setWaitingForFindTarget(false);
                if (m_pScene->getSpriteLayer())
                {
                    m_pScene->getSpriteLayer()->getRoleByControlled()->taskAutoFindingWay(taskData,false);
                    m_pScene->getSpriteLayer()->openPortal(taskData->getStatus()); // 判断一键寻路开启 传送门
                }
                if (m_pScene->getControlUILayer())
                {
                    m_pScene->getControlUILayer()->updateGuideButton(taskIndex);
                }
            }
        }
        taskData->getTargetArray()->removeAllObjects();// 清除之前的目标
        switch (taskData->getTargetType())
        {
            case kTaskTalk:
            {
                TaskTarget * taskTarget = TaskTarget::create();
                taskTarget->setTargetID(((CCString *)array->objectAtIndex(5))->getCString());
                taskData->getTargetArray()->addObject(taskTarget);
                taskTarget->setDialogContent(((CCString *)array->objectAtIndex(6))->getCString());
                if(Controller::getInstance()->getGameStatus() == kStatusUI && m_pScene->getPopUpUI()->getUIType() == kUITask)
                {
                    switch (m_pScene->getPopUpUI()->getTaskUIType())
                    {
                        case kTaskList:
                            m_pScene->getPopUpUI()->updateTaskInfo(taskIndex);
                            break;
                        case kDialogInterface:
                        {
                            ((cocos2d::gui::UILabel*)m_pScene->getPopUpUI()->getTaskDialogueInfo())->setText(taskTarget->getDialogContent().c_str());
                            if(taskData->getStatus() == kTaskStatusAccept)
                            {
                                ((cocos2d::gui::UILabel*)m_pScene->getPopUpUI()->getTaskDialogueButton())->setText(g_sAccept);
                            }
                            if(taskData->getStatus() == kTaskStatusContinue)
                            {
                                ((cocos2d::gui::UILabel*)m_pScene->getPopUpUI()->getTaskDialogueButton())->setText(g_sContinue);
                            }
                            if(taskData->getStatus() == kTaskStatusOver)
                            {
                                ((cocos2d::gui::UILabel*)m_pScene->getPopUpUI()->getTaskDialogueButton())->setText(g_sDone);
                            }
                            if(taskData->getStatus() == kTaskStatusFinding)
                            {
                                ((cocos2d::gui::UILabel*)m_pScene->getPopUpUI()->getTaskDialogueButton())->setText(g_sSeek);
                            }
                        }
                            break;
                        default:
                            break;
                    }
                }
            }
                break;
            case kTaskKillMonster:
            case kTaskCollect:
            {
                CCArray* arr = split_string(((CCString *)array->objectAtIndex(5))->getCString(),';');
                for(int i = 0;i < arr->count(); ++i)
                {
                    TaskTarget *taskTarget = TaskTarget::create();
                    taskData->getTargetArray()->addObject(taskTarget);
                    CCArray* arr1 = split_string(((CCString*)arr->objectAtIndex(i))->getCString(),',');
                    taskTarget->setDungeonsName(((CCString *)arr1->objectAtIndex(0))->getCString());
                    taskTarget->setStageName(((CCString *)arr1->objectAtIndex(1))->getCString());
                    taskTarget->setStageLevel(((CCString*)arr1->objectAtIndex(2))->intValue());
                    taskTarget->setTargetID(((CCString*)arr1->objectAtIndex(3))->getCString());
                    taskTarget->setTargetName(((CCString*)arr1->objectAtIndex(4))->getCString());
                    if (taskData->getTargetType() == kTaskCollect)
                    {
                        taskTarget->setTargetItemAmount(((CCString*)arr1->objectAtIndex(5))->intValue());
                        taskTarget->setDropOdds(((CCString*)arr1->objectAtIndex(6))->floatValue());
                        taskTarget->setTargetItemCount(((CCString*)arr1->objectAtIndex(7))->intValue());
                    }
                    else
                    {
                        taskTarget->setTargetMonsterAmount(((CCString*)arr1->objectAtIndex(5))->intValue());
                        taskTarget->setTargetMonsterCount(((CCString*)arr1->objectAtIndex(6))->intValue());
                    }
                }
            }
                break;
            case kTaskPassStage:
            {
                const char * content = ((CCString *)array->objectAtIndex(5))->getCString();
                CCArray* arr = split_string(content,';');
                for(int i = 0; i < arr->count(); ++i)
                {
                    TaskTarget *taskTarget = TaskTarget::create();
                    taskData->getTargetArray()->addObject(taskTarget);
                    CCArray* arr2 = split_string(((CCString*)arr->objectAtIndex(i))->getCString(),',');
                    taskTarget->setDungeonsName(((CCString *)arr2->objectAtIndex(0))->getCString());
                    taskTarget->setStageName(((CCString *)arr2->objectAtIndex(1))->getCString());
                    taskTarget->setStageLevel(((CCString*)arr2->objectAtIndex(2))->intValue());
                    taskTarget->setClearStageAmount(((CCString*)arr2->objectAtIndex(3))->intValue());
                    taskTarget->setClearStageCount(((CCString*)arr2->objectAtIndex(4))->intValue());
                }
            }
                break;
            case kTaskLevelUp:
            {
                TaskTarget *taskTarget = TaskTarget::create();
                taskData->getTargetArray()->addObject(taskTarget);
                taskTarget->setPlayerLevel(((CCString *)array->objectAtIndex(3))->intValue());
            }
                break;
            default:
                break;
        }
        // 更新npc的任务列表及状态
        if (Controller::getInstance()->getScene()->getSpriteLayer())
        {
            for(int j = 0;j < getScene()->getSpriteLayer()->getNPCArray()->count(); j++)
            {
                YYNPC* npc = (YYNPC*)getScene()->getSpriteLayer()->getNPCArray()->objectAtIndex(j);
                if (npc)
                {
                    if (taskData->getTargetType() == kTaskTalk)
                    {// 如果任务目标是对话任务，向相关npc任务列表中添加该任务，删除无关NPC任务列表中的该任务数据
                        TaskTarget *taskTarget = static_cast<TaskTarget *>(taskData->getTargetArray()->objectAtIndex(0));
                        if (strcmp(taskTarget->getTargetID().c_str(),npc->getSpriteIDInServer()->getCString()) == 0)
                        {//相关npc
                            if (!npc->getTaskDataDict()->objectForKey(taskIndex))
                            {// 任务列表中不包含这个任务
                                npc->getTaskDataDict()->setObject(taskData, taskIndex);
                            }
                        }
                        else
                        {// 无关npc
                            if (npc->getTaskDataDict()->objectForKey(taskIndex))
                            {//在任务列表中包含这个任务
                                npc->getTaskDataDict()->removeObjectForKey(taskIndex);
                            }
                        }
                    }
                    else
                    {
                        if (npc->getTaskDataDict()->objectForKey(taskIndex))
                        {//在任务列表中包含这个任务
                            npc->getTaskDataDict()->removeObjectForKey(taskIndex);
                        }
                    }
                    npc->updateTaskStatus();
                }
            }
        }
    }
}

void Controller::onCmdRemoveTaskData(cocos2d::CCArray *arr)
{
    int taskIndex = ((CCString *)arr->objectAtIndex(0))->intValue();
    TaskData * task = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
    if (task)
    {
        GlobalData::getInstance()->getTaskDataDict()->removeObjectForKey(taskIndex);
        GlobalData::getInstance()->getTaskDataArray()->removeObject(task);
        if(m_pScene->getControlUILayer()->getTaskNumberLabel())
        { // 更新任务数
            ((CCLabelTTF*)m_pScene->getControlUILayer()->getTaskNumberLabel())->setString(CCString::createWithFormat("%d",GlobalData::getInstance()->getTaskDataDict()->count())->getCString());
        }
        // 更新npc的任务列表及状态
        if (m_pScene->getSpriteLayer())
        {
            for(int j = 0;j < getScene()->getSpriteLayer()->getNPCArray()->count(); j++)
            {
                YYNPC* npc = (YYNPC*)getScene()->getSpriteLayer()->getNPCArray()->objectAtIndex(j);
                if (npc)
                {
                    if (npc->getTaskDataDict()->objectForKey(taskIndex))
                    {//在任务列表中包含这个任务
                        npc->getTaskDataDict()->removeObjectForKey(taskIndex);
                        npc->updateTaskStatus();
                    }
                }
            }
        }
    }
}

void Controller::onCmdErrorMessage(cocos2d::CCArray *arr)
{
    CCString *str = static_cast<CCString *>(arr->objectAtIndex(0));
    if (getGameStatus() == kStatusLoading)
    {
        returnLoginUI(false);
    }
    if (m_pScene)
    {
        m_pScene->showTips(str->getCString());
        if (getGameStatus() == kStatusLogin)
        {
            dynamic_cast<LoginUI *>(m_pScene->getTempLayer())->setRegisterEditBoxEnable(false);
            dynamic_cast<LoginUI *>(m_pScene->getTempLayer())->setLoginEditBoxEnable(false);
        }
        else if (getGameStatus() == kStatusScene && (getSceneType() == DUNGEONS || getSceneType() == WAR_ZONE))
        {// 在战斗区域
            m_pScene->getControlUILayer()->clearKeyState();
        }
        if (m_pScene->isNetLoading())
        {
            m_pScene->setNetLoading(false);
            m_nNetLoadingFrameCounter = 0;
        }
    }
}

void Controller::receiveTreatureContainerData(cocos2d::CCArray *array)
{
    CCString *str = static_cast<CCString *>(array->objectAtIndex(0));
    
    if(strcmp(str->getCString(), "E") == 0)
    {
            array->removeObjectAtIndex(0);
            m_pScene->getPopUpUI()->setReadingTreasureData(false);
            m_pScene->getPopUpUI()->setTreasureOpened(true);
    }
    else
    {
        int value = str->intValue();
        if(value == 0)
        {
            GlobalData::getInstance()->getTreasureItemArray()->removeAllObjects();
            array->removeObjectAtIndex(0);
            m_pScene->getPopUpUI()->setTreasureData(array);
        }
        else if(value == 1)
        {
            array->removeObjectAtIndex(0);
            GlobalData::getInstance()->addEquipment(array,true);
        }
        else if(value == 2)
        {
            array->removeObjectAtIndex(0);
            GlobalData::getInstance()->addProperty(array,true);
        }
    }
}

#pragma mark 其他通用方法

/**
 * 从第charIndex个字符开始用符号sign分割字符串message
 */
cocos2d::CCArray * Controller::createArray(std::string message, uint16_t charIndex,char sign)
{
    if (charIndex >= message.length())
    {
        CCLOG("charIndex out of message length");
        return NULL;
    }
    CCArray * array = split_string(message.substr(charIndex).c_str(), sign);
    return array;
}


void Controller::addLoadingMessageToArray(const char * message)
{
    GlobalData::getInstance()->getSetLoadingMessageArray()->addObject(CCString::create(message));
}

#pragma mark按键是否被点中

bool Controller::isButtonTouched(cocos2d::CCNode * node,cocos2d::CCPoint point)
{
    if(dynamic_cast<YYSprite *>(node))
    {
        return dynamic_cast<YYSprite *>(node)->isTouched(point);
    }
    else
    {
        float x = node->getPosition().x - node->getContentSize().width * node->getAnchorPoint().x;
        float y = node->getPosition().y - node->getContentSize().height * node->getAnchorPoint().y;
        if(CCRectMake(x, y, node->getContentSize().width, node->getContentSize().height).containsPoint(point))
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    return false;
}

void Controller::returnLoginUI(bool showTips)
{
    switchGameStatus(kStatusLogin);
    LoginUI * loginUI = dynamic_cast<LoginUI *>(m_pScene->getTempLayer());
    if (loginUI)
    {
        loginUI->showUILayer(kPanelTypeLogin);
    }
    else
    {
        m_pScene->addLoginUI();
    }
    closeConnect();
    GlobalData::getInstance()->clean(); // 回到LoginUI 清空缓存数据
    if (showTips)
    {
        CCString * tipMessage = CCString::createWithFormat("%s%s",g_sNetworkError,g_sReconnectLater);
        m_pScene->showTips(tipMessage->getCString());
    }
}
#pragma mark 获取系统当前时间

struct tm * Controller::getSystemTime()
{
    struct cc_timeval now;
    struct tm *tm;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time((time_t*)&now);
#else
    CCTime::gettimeofdayCocos2d(&now, NULL);
#endif
    tm = localtime((time_t*)&now.tv_sec);
    return tm;
}

#pragma mark 获取系统当前时间(秒数)
int Controller::getCurrentSecond()
{
    struct tm * tm = getSystemTime();
	int ret = 0;
	if(tm)
    {
		ret = tm->tm_hour * 3600 + tm->tm_min * 60 + tm->tm_sec;
	}
    return ret;
}

#pragma mark 获取日历
const char * Controller::getCalendar()
{
    struct tm * tm = getSystemTime();
	if(tm)
    {
        if (tm->tm_min < 10)
        {
            return CCString::createWithFormat("%d.%d.%d  %d:%d%d",tm->tm_year + 1900,tm->tm_mon + 1,tm->tm_mday,tm->tm_hour,0,tm->tm_min)->getCString();
        }
        else
        {
            return CCString::createWithFormat("%d.%d.%d  %d:%d",tm->tm_year + 1900,tm->tm_mon + 1,tm->tm_mday,tm->tm_hour,tm->tm_min)->getCString();
        }
        
	}
    return NULL;
}

/**
 *
 *@num 数字
 *@texture 纹理图片
 *@position 位置
 *@alignType 对齐方式
 *@interval 间距
 *@parent 父节点
 *@zOrder 渲染层级
 */
CCSpriteBatchNode * Controller::drawNumber(int num,cocos2d::CCTexture2D * texture,cocos2d::CCPoint position, CCPoint anchor,int interval,CCLayer * parent,int zOrder){
    int bit = 0;
    int temp = num;
    while (temp >= 0)
    {
        bit ++;
        temp = temp / 10;
        if (temp == 0)
        {
            break;
        }
    }
    CCSpriteBatchNode * batchNode = CCSpriteBatchNode::createWithTexture(texture, bit);
    parent->addChild(batchNode,zOrder);
    int textureWidth = texture->getContentSize().width;//  0-9数字图片的总宽度
    int numWidth = textureWidth / 10;//  单个数字的宽度
    int numHeight = texture->getContentSize().height;//  单个数字的高度
    int numTotalWidth = numWidth * bit + interval * (bit - 1);//  要显示的数字总宽度
    int divisor;
    for (int i = 0; i < bit; i ++)
    {//  从个位数开始计算，然后是十位、百位、千位......
        if (i == 0)
        {
            divisor = 1;
        }
        else
        {
            divisor *= 10;
        }
        int number = num / divisor % 10;//
        CCSprite * sprite = CCSprite::createWithTexture(texture,CCRectMake(numWidth * number, 0, numWidth, numHeight));
        batchNode->addChild(sprite);
        sprite->setAnchorPoint(anchor);
        sprite->setPosition(ccpAdd(ccp((numTotalWidth - numWidth) * (1 - anchor.x) - (numWidth + interval) * i,numHeight * anchor.y),position));
    }
    return batchNode;
}

#pragma mark 参数校验
bool Controller::parameterVerify(cocos2d::CCArray* array, int parametersNum)
{
    if (array->count() < parametersNum)
    {// 缺少参数
        CCLOG("Missing Parameters");
        return false;//未通过校验
    }
    return true;
}
#pragma mark 请求下一组脚本指令
void Controller::askNextScriptCommond()
{
    sendMessage(ASK_NEXT_SCRIPT,false);
}

#pragma mark 脚本方法调用
void Controller::callScriptMethod(std::string& message)
{
    if (message.compare(SCRIPT_OVER) == 0)
    {
        m_eScriptStatus = kScriptStatusOver;
    }
    else if(message.compare(RECEIVE_RUN_SCRIPT) == 0)
    {// 开始执行场景加载后的脚本
        m_bLoadingEnd = true;
        m_eScriptStatus = kScriptStatusRunning;
    }
    
    CCArray * array = split_string(message.c_str(), '_');
    if (!m_pScene || !array)
    {
        return;
    }
    if(array->count() > 0)
    {
        std::string methodName = ((CCString *)array->objectAtIndex(0))->getCString();
        if (methodName.compare(SCRIPT_SET_UI_VISIBLE) == 0)
        {
            bool isVisible = false;
            if(array->count() > 1)
            {
                isVisible = ((CCString *)array->objectAtIndex(1))->intValue() > 0;
            }
            m_pScene->getControlUILayer()->setUIVisible(isVisible);
        }
        else if (methodName.compare(SCRIPT_LOCK_SPRITE) == 0)
        {// 锁定精灵，精灵只受脚本控制
            if(!parameterVerify(array,3))
            {
                return;
            }
            YYSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
                m_pScene->getControlUILayer()->clearKeyState();
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
            }
            if (sprite)
            {
                bool isLocked = false;
                if (array->count() > 1)
                {
                    isLocked = ((CCString *)array->objectAtIndex(2))->intValue() > 0;
                }
                sprite->setLocked(isLocked);
            }
        }
        else if (methodName.compare(SCRIPT_SET_SPRITE_VISIBLE) == 0)
        {//设置精灵是否可视
            if(!parameterVerify(array,2))
            {
                return;
            }
            YYSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
            }
            if (sprite)
            {
                bool isVisible = false;
                if (array->count() > 1)
                {
                    isVisible = ((CCString *)array->objectAtIndex(2))->intValue() > 0;
                }
                sprite->setVisible(isVisible);
                if (sprite->getNameLabel())
                {
                    sprite->getNameLabel()->setVisible(isVisible);
                }
            }
        }
        else if (methodName.compare(SCRIPT_REMOVE_SPRITE) == 0)
        {//移除精灵
            if(!parameterVerify(array,2))
            {
                return;
            }
            YYSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
            }
            if (sprite)
            {
                sprite->setDeleted(true);
            }
        }
        else if (methodName.compare(SCRIPT_LOCK_CAMERA) == 0)
        {// 锁定镜头，镜头只受脚本控制
            bool isCameraLocked = false;
            if (array->count() > 1)
            {
                isCameraLocked = ((CCString *)array->objectAtIndex(1))->intValue() > 0;
            }
            m_pScene->setCameraLocked(isCameraLocked);
        }
        else if (methodName.compare(SCRIPT_SET_CAMERA_POSITION) == 0)
        {// 设置镜头位置
            if(!parameterVerify(array,3))
            {
                return;
            }
            m_pScene->setScreenDatumPoint(ccp(((CCString *)array->objectAtIndex(1))->intValue(),((CCString *)array->objectAtIndex(2))->intValue()));
            m_pScene->updateCameraPosition();
        }
        else if (methodName.compare(SCRIPT_CAMERA_MOVE) == 0)
        {// 镜头移动
            if(!parameterVerify(array,5))
            {
                return;
            }
            m_pScene->setCameraMovedByScript(true);
            int displacementX = ((CCString *)array->objectAtIndex(2))->intValue();
            int displacementY = ((CCString *)array->objectAtIndex(3))->intValue();
            int offsetX = ((CCString *)array->objectAtIndex(4))->intValue();
            int offsetY = ((CCString *)array->objectAtIndex(5))->intValue();
            m_pScene->setCameraDestination(m_pScene->getScreenDatumPoint() + ccp(offsetX > 0 ? displacementX : -displacementX,offsetY > 0 ? displacementY : -displacementY));
            m_pScene->setCameraOffsetPerFrame(CCSizeMake(offsetX,offsetY));
            bool isRecordCameraPosition = false;
            if (array->count() > 5)
            {
                isRecordCameraPosition = ((CCString *)array->objectAtIndex(5))->intValue() > 0;
            }
            if (isRecordCameraPosition)
            {
                m_pScene->setCameraRecordPosition(m_pScene->getScreenDatumPoint());
            }
        }
        else if (methodName.compare(SCRIPT_CAMERA_MOVE_TO) == 0)
        {// 镜头移动至
            if(!parameterVerify(array,5))
            {
                return;
            }
            m_pScene->setCameraMovedByScript(true);
            m_pScene->setCameraDestination(ccp(((CCString *)array->objectAtIndex(1))->intValue(),((CCString *)array->objectAtIndex(2))->intValue()));
            m_pScene->setCameraOffsetPerFrame(CCSizeMake(((CCString *)array->objectAtIndex(3))->intValue(),((CCString *)array->objectAtIndex(4))->intValue()));
            bool isRecordCameraPosition = false;
            if (array->count() > 5)
            {
                isRecordCameraPosition = ((CCString *)array->objectAtIndex(5))->intValue() > 0;
            }
            if (isRecordCameraPosition)
            {
                m_pScene->setCameraRecordPosition(m_pScene->getScreenDatumPoint());
            }
        }
        else if (methodName.compare(SCRIPT_CAMERA_MOVE_BACK) == 0)
        {// 镜头移回
            if(!parameterVerify(array,3))
            {
                return;
            }
            m_pScene->setCameraMovedByScript(true);
            m_pScene->setCameraDestination(m_pScene->getCameraRecordPosition());
            m_pScene->setCameraOffsetPerFrame(CCSizeMake(((CCString *)array->objectAtIndex(1))->intValue(),((CCString *)array->objectAtIndex(2))->intValue()));
            bool isRecordCameraPosition = false;
            if (array->count() > 3)
            {
                isRecordCameraPosition = ((CCString *)array->objectAtIndex(3))->intValue() > 0;
            }
            if (isRecordCameraPosition)
            {
                m_pScene->setCameraRecordPosition(m_pScene->getScreenDatumPoint());
            }
        }
        else if (methodName.compare(SCRIPT_SAY) == 0)
        {// 对话
            if(!parameterVerify(array,3))
            {
                return;
            }
            const char *name = ((CCString *)array->objectAtIndex(1))->getCString();
            const char *content = ((CCString *)array->objectAtIndex(2))->getCString();
            const char *picFileName = NULL;
            if (array->count() > 3)
            {
                picFileName = ((CCString *)array->objectAtIndex(3))->getCString();
            }
            m_pScene->getControlUILayer()->say(name,content,picFileName);
            m_eScriptStatus = kScriptStatusPause;
        }
        else if (methodName.compare(SCRIPT_FACE) == 0)
        {// 表情 精灵ID 表情ID x,y偏移量
            if(!parameterVerify(array,4))
            {
                return;
            }
            YYSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
            }
            if (sprite)
            {
                const char *temp = ((CCString *)array->objectAtIndex(2))->getCString();
                int duration = ((CCString *)array->objectAtIndex(3))->intValue();
                int offX = 0,offY = 0;
                if (array->count() > 4)
                {
                    offX = ((CCString *)array->objectAtIndex(4))->intValue();
                }
                if (array->count() > 5)
                {
                    offY = ((CCString *)array->objectAtIndex(5))->intValue();
                }
                YYSprite *face = YYSprite::create();
                face->initWithActionSet(temp, kDirTypePic);
                face->setPosition(ccp(offX,sprite->getDefaultSize().height + offY));
                face->setDuration(duration);
                face->setDeleteCondition(DELETE_COUNTDOWN_TO_THE_END);
                face->setAction(0,true);
                face->setSpriteType(SPRITE_TYPE_FACE);
                //                sprite->getFaceArray()->addObject(face);
                sprite->getYYSpriteChildren()->addObject(face);
                sprite->addChild(face,1);
            }
        }
        else if (methodName.compare(SCRIPT_SKILLUSING) == 0)
        {// 使用技能 参数1 精灵ID，参数2 技能编号
            
            YYFightSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
                if (sprite)
                {
                    int shortcutIndex = ((CCString *)array->objectAtIndex(2))->intValue();// 快捷键索引
                    CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(dynamic_cast<YYPlayer *>(sprite)->getSpriteIDInServer()->getCString()));
                    int skillIndex  = ((CCString *)array->objectAtIndex(shortcutIndex))->intValue();
                    SkillData * skillData = (SkillData *)GlobalData::getInstance()->getPlayerSkillData(sprite->getSpriteIDInServer()->getCString(), skillIndex);
                    if (skillData)
                    {
                        sprite->createSkillWithData(skillData);
                        sprite->setSkillAction();
                        sprite->setActionStatus(kActionSkill);
                    }
                }
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYFightSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
                if (sprite)
                {
                    int skillIndex = ((CCString *)array->objectAtIndex(2))->intValue();
                    SkillData * skillData = (SkillData *)GlobalData::getInstance()->getCommonSkillData(skillIndex);
                    if (skillData)
                    {
                        sprite->createSkillWithData(skillData);
                        sprite->setSkillAction();
                        sprite->setActionStatus(kActionSkill);
                    }
                }
            }
        }
        else if (methodName.compare(SCRIPT_SET_ACTION) == 0)
        {// 设置人物动作 1 精灵ID 2 动作序号 3 持续帧数 4 是否停顿在最后一帧
            if(!parameterVerify(array,3))
            {
                return;
            }
            YYSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
            }
            if (sprite)
            {
                sprite->setMoving(false);
                int actionIndex = ((CCString *)array->objectAtIndex(2))->intValue();
                bool isKeepLastFrame = false;
                if (array->count() > 4)
                {
                    isKeepLastFrame = ((CCString *)array->objectAtIndex(4))->intValue() > 0;
                }
                sprite->setActionByScript(actionIndex, ((CCString *)array->objectAtIndex(3))->intValue(), isKeepLastFrame);
            }
        }
        else if (methodName.compare(SCRIPT_MOVE) == 0)
        {// 移动
            if(!parameterVerify(array,5))
            {
                return;
            }
            YYSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
            }
            if (sprite)
            {
                int displacementX = ((CCString *)array->objectAtIndex(2))->intValue();// 横方向位移
                int displacementY = ((CCString *)array->objectAtIndex(3))->intValue();// 纵方向位移
                int speed = ((CCString *)array->objectAtIndex(4))->intValue();// 每帧x偏移量
                sprite->setMoving(true);
                sprite->setMoveType(MOVE_BY_SCRIPT);
                sprite->setDestination(sprite->getPosition() + ccp(displacementX,displacementY));
                sprite->setScriptSpeed(speed);
                bool isCameraFollowSprite = false;
                if (array->count() > 5)
                {
                    isCameraFollowSprite = ((CCString *)array->objectAtIndex(5))->intValue() > 0;// 镜头是否跟随精灵
                }
                m_pScene->setCameraFollowSprite(isCameraFollowSprite);
                if (isCameraFollowSprite)
                {
                    m_pScene->setCameraFocusPosition(sprite->getPosition());
                }
                bool isMirror = false;
                if (array->count() > 6)
                {
                    isMirror = ((CCString *)array->objectAtIndex(6))->intValue() > 0;// 是不是倒退
                }
                sprite->setSpriteMirror(isMirror);
            }
        }
        else if (methodName.compare(SCRIPT_MOVE_TO) == 0)
        {// 移动到
            if(!parameterVerify(array,5))
            {
                return;
            }
            YYSprite * sprite = NULL;
            if (((CCString *)array->objectAtIndex(1))->intValue() == -1)
            {
                sprite = m_pScene->getSpriteLayer()->getRoleByControlled();
            }
            else
            {
                const char *spriteID = ((CCString *)array->objectAtIndex(1))->getCString();
                sprite = (YYSprite *)m_pScene->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(spriteID);
            }
            if (sprite)
            {
                sprite->setMoving(true);
                sprite->setMoveType(MOVE_BY_SCRIPT);
                sprite->setDestination(ccp(((CCString *)array->objectAtIndex(2))->intValue(),((CCString *)array->objectAtIndex(3))->intValue()));
                sprite->setScriptSpeed(((CCString *)array->objectAtIndex(4))->intValue());
                bool isCameraFollowSprite = false;
                if (array->count() > 5)
                {
                    isCameraFollowSprite = ((CCString *)array->objectAtIndex(5))->intValue() > 0;
                }
                m_pScene->setCameraFollowSprite(isCameraFollowSprite);
                if (isCameraFollowSprite)
                {
                    m_pScene->setCameraFocusPosition(sprite->getPosition());
                }
                bool isMirror = false;
                if (array->count() > 6)
                {
                    isMirror = ((CCString *)array->objectAtIndex(6))->intValue() > 0;
                }
                sprite->setSpriteMirror(isMirror);
            }
        }
        else if (methodName.compare(SCRIPT_PAUSE) == 0)
        {// 暂停脚本
            m_nPauseCounter = 0;
            m_nPauseDuration = ((CCString *)array->objectAtIndex(1))->intValue();
            if (m_nPauseDuration > 0)
            {
                m_eScriptStatus = kScriptStatusPause;
            }
        }
        else if (methodName.compare(SCRIPT_GUIDE) == 0)
        {//引导动画
            if(!parameterVerify(array,7))
            {
                return;
            }
            int index = 1;
            const char *anuFileName = ((CCString *)array->objectAtIndex(index ++))->getCString();
            const char *pngFileName = ((CCString *)array->objectAtIndex(index ++))->getCString();
            int x = ((CCString *)array->objectAtIndex(index ++))->intValue();
            int y = ((CCString *)array->objectAtIndex(index ++))->intValue();
            int type = ((CCString *)array->objectAtIndex(index ++))->intValue();

            const char * content = NULL;
            int contentW = 0;
            int contentH = 0;
            int contentX = 0;
            int contentY = 0;
            content = ((CCString *)array->objectAtIndex(index ++))->getCString();
            contentW = ((CCString *)array->objectAtIndex(index ++))->intValue();
            contentH = ((CCString *)array->objectAtIndex(index ++))->intValue();
            contentX = ((CCString *)array->objectAtIndex(index ++))->intValue();
            contentY = ((CCString *)array->objectAtIndex(index ++))->intValue();
            
            int tempValue1 = ((CCString *)array->objectAtIndex(index++))->intValue();
            int tempValue2 = -1,tempValue3 = -1,tempValue4 = -1;
            if (array->count() > index)
            {
                tempValue2 = ((CCString *)array->objectAtIndex(index++))->intValue();
            }
            if (array->count() > index)
            {
                tempValue3 = ((CCString *)array->objectAtIndex(index++))->intValue();
            }
            if (array->count() > index)
            {
                tempValue4 = ((CCString *)array->objectAtIndex(index))->intValue();
            }
            YYGuide * guide = YYGuide::create();
            guide->initWithActionSet(anuFileName, pngFileName, kDirTypeEffect);
            guide->setAction(type % 4,true);
            setTutorial(true);
            guide->setTag(TAG_GUIDE);
            m_bTutorialOnMainUI = type < 4;
            if (m_bTutorialOnMainUI)
            {
                if (m_pScene->getControlUILayer())
                {
                    m_pScene->getControlUILayer()->addChild(guide,2);
                }
            }
            else
            {
                if (m_pScene->getPopUpUI())
                {
                    m_pScene->getPopUpUI()->addChild(guide,2);
                }
            }
            CCLabelTTF * contentLabel = NULL;
            if (content)
            {
                contentLabel = CCLabelTTF::create(content, DEFAULT_FONT_NAME, 30,CCSizeMake(contentW, contentH),kCCTextAlignmentLeft,kCCVerticalTextAlignmentTop);
                contentLabel->setAnchorPoint(ccp(0,1));
                contentLabel->setDimensions(CCSizeMake(contentW, contentH));
                contentLabel->setPosition(ccp(contentX,contentY));
                guide->addChild(contentLabel);
            }
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            switch (type)
            {
                case 0:// 左下角
                    guide->setPosition(ccp(x,y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(tempValue1, tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
                case 1: // 右下角对齐
                    guide->setPosition(ccp(winSize.width + x,y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(winSize.width + tempValue1, tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
                case 2: // 右上角对齐
                    guide->setPosition(ccp(winSize.width + x,winSize.height + y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(winSize.width + tempValue1, winSize.height + tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
                case 3:// 左上角
                    guide->setPosition(ccp(x,winSize.height + y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(tempValue1, winSize.height + tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
                case 4:
                    guide->setPosition(ccp(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + x, m_pScene->getPopUpUI()->getAdjustScreenPosition().y + y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + tempValue1, m_pScene->getPopUpUI()->getAdjustScreenPosition().y + tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
                case 5:
                    guide->setPosition(ccp(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + STANDARD_SCENE_WIDTH + x,m_pScene->getPopUpUI()->getAdjustScreenPosition().y +  y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + STANDARD_SCENE_WIDTH + tempValue1, m_pScene->getPopUpUI()->getAdjustScreenPosition().y + tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
                case 6:
                    guide->setPosition(ccp(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + STANDARD_SCENE_WIDTH + x,m_pScene->getPopUpUI()->getAdjustScreenPosition().y + STANDARD_SCENE_HEIGHT + y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + STANDARD_SCENE_WIDTH + tempValue1, m_pScene->getPopUpUI()->getAdjustScreenPosition().y + STANDARD_SCENE_HEIGHT + tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
                case 7:
                    guide->setPosition(ccp(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + x,m_pScene->getPopUpUI()->getAdjustScreenPosition().y + STANDARD_SCENE_HEIGHT + y));
                    if (array->count() > index)
                    {
                        guide->setTouchRegion(CCRectMake(m_pScene->getPopUpUI()->getAdjustScreenPosition().x + tempValue1, m_pScene->getPopUpUI()->getAdjustScreenPosition().y + STANDARD_SCENE_HEIGHT + tempValue2, tempValue3, tempValue4));
                    }
                    else
                    {
                        guide->setTouchBeganValue(tempValue1);
                        guide->setTouchMoved(tempValue2 == 1);
                        guide->setTouchEndedValue(tempValue3);
                    }
                    break;
            }
            m_eScriptStatus = kScriptStatusPause;
        }
        else if (methodName.compare(SCRIPT_HELP) == 0)
        {//引导动画
            if(!parameterVerify(array,7))
            {
                return;
            }
            int index = 1;
            const char *anuFileName = ((CCString *)array->objectAtIndex(index ++))->getCString();
            const char *pngFileName = ((CCString *)array->objectAtIndex(index ++))->getCString();
            int x = ((CCString *)array->objectAtIndex(index ++))->intValue();
            int y = ((CCString *)array->objectAtIndex(index ++))->intValue();
            int type = ((CCString *)array->objectAtIndex(index ++))->intValue();
            int tempValue = ((CCString *)array->objectAtIndex(index ++))->intValue();
            
            YYGuide * guide = YYGuide::create();
            guide->initWithActionSet(anuFileName, pngFileName, kDirTypePic);
            CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            m_pScene->getControlUILayer()->setHelpDuration(tempValue);
            guide->setAction(0, true);
            guide->setScaleX(winSize.width / STANDARD_SCENE_WIDTH);
            guide->setScaleY(winSize.height / STANDARD_SCENE_HEIGHT);
            guide->setTag(TAG_HELP);
            m_pScene->getControlUILayer()->addChild(guide,1);
            switch (type)
            {
                case 0:// 左上角
                    guide->setPosition(ccp(x,winSize.height + y));
                    break;
                case 1:// 左中
                    guide->setPosition(ccp(x,winSize.height / 2 + y));
                    break;
                case 2:// 左下角
                    guide->setPosition(ccp(x,y));
                    break;
                case 3:// 下中
                    guide->setPosition(ccp(winSize.width / 2 + x,y));
                    break;
                case 4: // 右下角对齐
                    guide->setPosition(ccp(winSize.width + x,y));
                    break;
                case 5:// 右中
                    guide->setPosition(ccp(winSize.width + x,winSize.height / 2 + y));
                    break;
                case 6: // 右上角对齐
                    guide->setPosition(ccp(winSize.width + x,winSize.height + y));
                    break;
                case 7:// 上中
                    guide->setPosition(ccp(winSize.width / 2 + x,winSize.height + y));
                    break;
            }
        }
    }
}

void Controller::createDownloadedDir()
{
    pathToSave = CCFileUtils::sharedFileUtils()->getWritablePath();
    pathToSave += "tmpdir";
    
    // Create the folder if it doesn't exist
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    DIR *pDir = NULL;
    
    pDir = opendir (pathToSave.c_str());
    if (!pDir)
    {
        mkdir(pathToSave.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
    }
#else
	if ((GetFileAttributesA(pathToSave.c_str())) == INVALID_FILE_ATTRIBUTES)
	{
		CreateDirectoryA(pathToSave.c_str(), 0);
	}
#endif
    CC_SAFE_DELETE(pDir);
}

void Controller::setAssetsManager(AssetsManagerDelegateProtocol *delegate)
{

//    CC_SAFE_DELETE(m_pAssetsManager);
    m_pAssetsManager = new AssetsManager(packageURL.c_str(),
                                           "",
                                           pathToSave.c_str());
    if (delegate)
    {
        m_pAssetsManager->setDelegate(delegate);
    }
    m_pAssetsManager->setConnectionTimeout(3);
    
    m_pAssetsManager->update();
    const char* msg = CCString::createWithFormat(VERIFY_VERSION,Config::getInstance()->getVersion().c_str())->getCString();
    sendMessage(msg, false);
}

bool Controller::isTouchGuideRect(cocos2d::CCPoint touchPoint)
{
    YYGuide * guide = NULL;
    if (m_bTutorialOnMainUI)
    {
        guide = static_cast<YYGuide *>(m_pScene->getControlUILayer()->getChildByTag(TAG_GUIDE));
    }
    else
    {
        guide = static_cast<YYGuide *>(m_pScene->getPopUpUI()->getChildByTag(TAG_GUIDE));
    }
    if (guide)
    {// 设置了选中区域，未设置键值
        if (guide->getTouchRegion().containsPoint(touchPoint))
        {
            return true;
        }
    }
    return false;
}

USING_NS_CC_EXT;
//  网络请求图片下载
void Controller::onMenuClicked(cocos2d::CCObject *sender ,  char * setUrl_ ,  char * childpath , char * filename)
{
    CCHttpRequest* request = new CCHttpRequest();
    std::string url_ = "http://  112.25.14.24/dtxy/image/";
    //      url_ += setUrl_;
    //      url_+=childpath;
    url_+= filename;
    m_sFilename = filename;
    request->setUrl(url_.c_str());
//      std::vector<std::string> vec;
//      vec.push_back("Connection:Keep-Alive");
//      request->setHeaders(vec);
    request->setRequestType(cocos2d::extension::CCHttpRequest::kHttpGet);
    //
    CCHttpClient::getInstance()->setTimeoutForConnect(30);
    CCHttpClient::getInstance()->setTimeoutForRead(30);
    
    request->setResponseCallback(this, httpresponse_selector(Controller::onHttpRequestCompleted));
    //      request->setTag("GET PIC");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void Controller::onHttpRequestCompleted(CCNode* node, void* data){
    CCHttpResponse *response = (CCHttpResponse*)data;
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        CCLOG("response failed");
        CCLOG("error buffer: %s", response->getErrorBuffer());
        return;
    }
    //   You can get original request type from: response->request->reqType
    if (0 != strlen(response->getHttpRequest()->getTag()))
    {
        CCLOG("%s completed", response->getHttpRequest()->getTag());
    }
    int statusCode = response->getResponseCode();
    char statusString[64] = {};
    sprintf(statusString, "HTTP Status Code: %d, tag = %s", statusCode, response->getHttpRequest()->getTag());
    
    //      CCLOG("response code: %d", statusCode);
    //   dump data
    std::vector<char> *buffer = response->getResponseData();
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    std::string sBuffer(buffer->begin(),buffer->end());
    //  保存到本地文件
    path += m_sFilename;
    CCLOG("path: %s",path.c_str());
    FILE *fp = fopen(path.c_str(), "wb+");
    fwrite(sBuffer.c_str(), 1,buffer->size(),  fp);
    fclose(fp);
}

CCSprite* Controller::getSpriteFromWritablePath(const char* name){
    std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
    path+=name;
    FILE* fp = fopen(path.c_str(),"rb");
    if (!fp)
    {
        return NULL;
    }
    fseek(fp,0,SEEK_END);
    int len = ftell(fp);
    fseek(fp,0,SEEK_SET);
    char* buf = (char*)malloc(len);
    fread(buf,len,1,fp);
    fclose(fp);
    CCImage* img = new CCImage;
    img->initWithImageData(buf,len);
    free(buf);
    cocos2d::CCTexture2D* texture = new cocos2d::CCTexture2D();
    bool isImg = texture->initWithImage(img);
    img->release();
    if (!isImg)
    {
        delete texture;
        return CCSprite::create("50black.png");//  加载资源并非图片时返回的默认图
    }
    CCSprite* sprite = CCSprite::createWithTexture(texture);
    texture->release();
    return sprite;
}

/* get buffer as CCImage */
CCImage* Controller::newCCImage(const char * path)
{
    CCImage *image = new CCImage();
    image->initWithImageFile(path,CCImage::kFmtPng);
    unsigned char *  pData = image->getData();
    unsigned short height = image->getHeight();
    unsigned short width = image->getWidth();
    //      int bits = image->getBitsPerComponent();
    //      bool alpha = image->hasAlpha();
    //      bool premultipliedAlpha = image->isPremultipliedAlpha();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(pData[(j+i*width)*4] == 0
               && pData[(j+i*width)*4+1] == 0
               && pData[(j+i*width)*4+2] == 0)
            {
                pData[(j+i*width)*4+3] = 0;
            }
        }
    }
    return image;
}
//  / 检测2个矩形是否发生碰撞

//  / </summary>

//  / <returns></returns>

//  bool Controller::IsIntersect (CCPoint A[4], CCPoint B[4])
//
//  {
//
//      CCPoint AX, AY, BX, BY;
//
//      //      AX = new CCPoint();
//      //
//      //      AY = new CCPoint();
//      //
//      //      BX = new CCPoint();
//      //
//      //      BY = new CCPoint();
//
//
//
//      AX.x = A[0].x - A[1].x;
//
//      AX.y = A[0].y - A[1].y;
//
//      AY.x = A[0].x - A[3].x;
//
//      AY.y = A[0].y - A[3].y;
//
//      BX.x = B[0].x - B[1].x;
//
//      BX.y = B[0].y - B[1].y;
//
//      BY.x = B[0].x - B[3].x;
//
//      BY.y = B[0].y - B[3].y;
//
//      //  对于AX上：
//
//      if (Tmp(AX, A, B)) return false;
//
//      if (Tmp(AY, A, B)) return false;
//
//      if (Tmp(BX, A, B)) return false;
//
//      if (Tmp(BY, A, B)) return false;
//
//      return true;
//
//  }
//  bool Controller::Tmp(CCPoint IS,CCPoint A[4],CCPoint B[4])
//
//  {
//
//      float v[4];
//
//      for (int i = 0; i < 4; i++)
//
//      {
//
//          float tmp = (IS.x * A[i].x + IS.y * A[i].y) / (IS.x * IS.x + IS.y * IS.y);
//
//          v[i] = tmp * IS.x * IS.x + tmp * IS.y * IS.y;
//
//      }
//
//      float vv[4];
//
//      for (int i = 0; i < 4; i++)
//
//      {
//
//          float tmp = (IS.x * B[i].x + IS.y * B[i].y) / (IS.x * IS.x + IS.y * IS.y);
//
//          vv[i] = tmp * IS.x * IS.x + tmp * IS.y * IS.y;
//
//      }
//
//      if (fmaxf(fmaxf(v[0], v[1]),fmaxf(v[2],v[3])) >fminf(fminf(vv[0],vv[1]),fminf(vv[2],vv[3])) && fminf(fminf(v[0],v[1]),fminf(v[2],v[3])) < fmaxf(fmaxf(vv[0],vv[1]),fmaxf(vv[2],vv[3]))) {
//
//          return false;
//
//      }//  表示暂时不知道是否碰撞
//
//      else
//          return true;//  表示知道未碰撞
//
//  }
//  label 描边
CCRenderTexture* Controller::createStroke(cocos2d::CCLabelTTF *label, float size, cocos2d::ccColor3B color)
{
    float x = label->getTexture()->getContentSize().width+size*2;
    float y = label->getTexture()->getContentSize().height+size*2;
    CCRenderTexture *rt=CCRenderTexture::create(x, y);
    CCPoint originalPos=label->getPosition();
    ccColor3B originalColor=label->getColor();
    label->setColor(color);
    ccBlendFunc originalBlend=label->getBlendFunc();
	ccBlendFunc t0 = {GL_SRC_ALPHA,GL_ONE};
    label->setBlendFunc(t0);
    CCPoint center = ccp(x/2, y/2);
    rt->begin();
    for (int i = 0; i < 360; i += 15) {
        float _x=center.x + sin(CC_DEGREES_TO_RADIANS(i)) * size;
        float _y=center.y + cos(CC_DEGREES_TO_RADIANS(i)) * size;
        
        label->setPosition(ccp(_x, _y));
        label->visit();
        
    }
    rt->end();
    label->setPosition(originalPos);
    label->setColor(originalColor);
    label->setBlendFunc(originalBlend);
    float rtX=originalPos.x-size;
    float rtY=originalPos.y-size;
    
    rt->setPosition(ccp(rtX, rtY));
    
    return rt;
}
