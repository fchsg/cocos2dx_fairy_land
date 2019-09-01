//
//  YYScene.h
//  XJ
//
//  Created by fuchenhao on 13-4-19. 
//
//

#ifndef __XJ__YYScene__
#define __XJ__YYScene__

#include <cocos2d.h>
#include "CCScale9Sprite.h"
#include "../common/ODSocket.h"
#include "YYController.h"

#define TAG_POPUP_WINDOW 2000 //UI 弹出窗口

class Logo;
class Cover;
class LoginUI;
class Loading;
class YYControlUILayer;
class YYSpriteLayer;
class YYSprite;
class YYLayer;
class YYDragRegion;
class YYCellLayer;
class PopUpUI;
class YYButton;

class YYScene : public cocos2d::CCScene,public SocketReciever{
    CC_SYNTHESIZE(cocos2d::CCSize, m_SceneSize, SceneSize);// 场景大小 
    CC_SYNTHESIZE(uint16_t,m_u16TileColumnAmount,TileColumnAmount);// 行走层元素格列数 
    CC_SYNTHESIZE(uint16_t,m_u16TileRowAmount,TileRowAmount);// 行走层元素格行数 
    CC_SYNTHESIZE(uint16_t,m_u16TileWidth,TileWidth);// 行走层元素格宽度 
    CC_SYNTHESIZE(uint16_t,m_u16TileHeight,TileHeight);// 行走层元素格高度 
    CC_SYNTHESIZE(cocos2d::CCPoint, m_ScreenDatumPoint, ScreenDatumPoint);// 屏幕基准点（左下点）相对游戏场景的坐标 
    CC_SYNTHESIZE(std::string, m_pComposePic, ComposePic);// 合成后的装备图片
    CC_SYNTHESIZE_READONLY(YYCellLayer *, m_pCellLayer, CellLayer);// 格子 
    CC_SYNTHESIZE_READONLY(YYSpriteLayer *, m_pSpriteLayer, SpriteLayer);// 战斗精灵层 
    CC_SYNTHESIZE_READONLY(YYControlUILayer *, m_pControlUILayer, ControlUILayer);// 触控按键UI层 
    CC_SYNTHESIZE_READONLY(PopUpUI *, m_pPopUpUI, PopUpUI);// 触控UI弹出层
    CC_SYNTHESIZE_READONLY(cocos2d::CCLayer *,m_pTempLayer, TempLayer);//logo/cover/login
    CC_SYNTHESIZE_READONLY(Loading *,m_pLoading, LoadingLayer);
    CC_SYNTHESIZE(int, m_nFightResultDelayCounter, FightResultDelayCounter); // 战斗结果延迟计数 
    CC_SYNTHESIZE(int, m_nCurrentTime, CurrentTime);
    CC_SYNTHESIZE(int, m_nStageDuration, StageDuration);
    CC_SYNTHESIZE(int, m_nStageDamage, StageDamage);// 造成的伤害 
    CC_SYNTHESIZE(int, m_nHPDecrement, HPDecrement);// 减血量 
    CC_SYNTHESIZE(int, m_nHittedAmount, HittedAmount);// 受击次数 
    CC_SYNTHESIZE(int, m_nMaxComboCount, MaxComboCount);// 最大连击数 
    CC_SYNTHESIZE(int, m_nKillCount, KillCount);// 杀敌数量 
    CC_SYNTHESIZE(cocos2d::CCPoint, m_TempPoint, TempPoint); // 记录touchend 点
    CC_SYNTHESIZE(cocos2d::CCPoint, m_CameraRecordPosition, CameraRecordPosition);// 镜头基准点当前对齐的位置 
    CC_SYNTHESIZE(cocos2d::CCPoint, m_CameraDestination, CameraDestination);// 镜头基准点将要到达的位置 
    CC_SYNTHESIZE(cocos2d::CCSize, m_CameraOffsetPerFrame, CameraOffsetPerFrame);// 镜头位移速率
    CC_SYNTHESIZE(cocos2d::CCPoint, m_CameraFocusPosition, CameraFocusPosition); // 镜头焦点位置（默认以主角为焦点）
    CC_SYNTHESIZE(float, m_fPopUpUIDelayTime, PopUpUIDelayTime); // 弹出用户界面数据获得延时时间
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pStageDataArray, StageDataArray);// 关卡界面
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pDungeonsDataArray, DungeonsDataArray);// 副本
    CC_SYNTHESIZE(cocos2d::CCDictionary * ,m_pAwardDictionary, AwardDictionary);// 随机奖励,key对应123三个难度
    CC_SYNTHESIZE(cocos2d::CCString * ,m_pAwardString, AwardString);// 固定关卡奖励
    CC_SYNTHESIZE(bool * ,m_pIsChoosedDungeon, IsChoosedDungeon);// 是否选中了某个副本
    
private:
    
    cocos2d::CCLayerColor * m_pTempColorBG;//logo,Loading,Cover,loginUI的背景色
    cocos2d::CCLayerColor *m_pBackground;// 背景
    cocos2d::CCLayer * m_pFarScenery;// 远景
    cocos2d::CCLayer * m_pMidScenery;// 中景
    cocos2d::CCLayer * m_pNearScenery;// 近景
    cocos2d::CCLayer * m_pRoadLayer;// 道路
    cocos2d::CCLayer * m_pCloseLayer;// 道路

    bool * m_pBlockTile;// 地图元素格是否不可通过 
    bool m_bNetLoading;// 是否正在联网获取数据 
    cocos2d::CCLabelTTF * m_pSceneNameLabel;// 场景名 
    cocos2d::CCPoint m_CurTouchPoint,m_PreTouchPoint;// 当前触摸点和上一触摸点 
    bool m_bTouchBegan,m_bTouchMoved,m_bTouchEnded;
    
    //提示信息
    TipType m_eTipType;// 提示信息类型
    std::string m_sTipsInfo;//记录最后一条提示信息
    TipType m_ePreTipType;//上一条提示信息类型
    cocos2d::gui::UIPanel *m_pTipPanel; // 提示面板；
    
    // —————————————震屏相关——————————————
    bool m_bVibrate;
    int m_nVibrateOffsetX,m_nVibrateOffsetY;
    int m_nVibrateCounter;
    int m_nVibrateDuration;
    
    //------------屏幕移动相关---------- 
    bool m_bCameraLocked;// 设置镜头锁定，只能被脚本指令移动 
    bool m_bCameraMovedByScript;// 脚本控制镜头移动 
    bool m_bCameraFollowSprite;// 镜头跟随精灵 
    
    // ------------死亡信息确认相关------------ 
    int m_nDeadConfirmDuration;
    int m_nDeadConfirmCounter;
    int m_nCurrentElementNum;
    
    // -----------副本UI相关------------- 
    float m_fCurrentScrollDistance,m_fMaxScrollDistance;
    //cocos2d::CCDictionary *m_pDungeonsButtonDictionary; // 副本界面的按键容器
    cocos2d::CCDictionary *m_pStageButtonDictionary;// 关卡界面的按键容器
    uint8_t m_u8DungeonAmount;// 副本总数 
    uint8_t m_u8DungeonsIndex;// 被选中副本的索引
    float m_fRotateAngle;// 关卡选择转盘旋转角度 
    float m_fMaxAngle;// 转盘最大旋转角度
    int m_nTurntableOffsetY;// 转盘中心点相对于屏幕的偏移量 
    cocos2d::CCSprite * m_pTurntable;// 转盘 
    cocos2d::CCSpriteBatchNode * m_pTurntableBatchNode;// 集合转盘上所有元素的节点
    cocos2d::CCLabelTTF * m_pDungeonsTitelLabel;// 标题 
    cocos2d::extension::CCScale9Sprite *m_pDungeonsTitleFrame;// 副本标题框 
    uint8_t m_u8DungeonStatus;
    
    // ——————关卡相关—————— 
    uint8_t m_u8StageAmount;// 关卡总数 
    uint8_t m_u8ChoosedStageIndex;// 被选中副本的索引
    bool m_bStageItemScroll;
    bool m_bStageItemChoosed;
    bool m_bCloseChoosed;
    bool m_bEnter;
    YYButton * m_pDifficultButton[3];// 难度选择按键 
    uint8_t m_u8UnlockDifficultOptions;// 已解锁的难度 
    uint8_t m_u8ChoosedDifficultOptionIndex;// 选择的关卡难度
    int32_t m_nCurrentPhysicalPower;
    int32_t m_nConsumePhysicalPower;
    cocos2d::CCLabelTTF *m_pDifficultLabel[3];// 难度标签
    
    enum
    {
        kDungeonSelectionUI = 0,// 副本选择界面 
        kStageSelectionUI,// 关卡选择界面 
        kStageAwardsUI,// 关卡奖励界面 
        kAFKConfigUI,// 挂机确认界面away from the keyboard 
    };
    
    // ------------战斗评价----------- 
    bool m_bConfirm;
    uint8_t m_u8StageStartLevel;// 关卡评级 
    uint8_t m_u8ShowItemAmount;
   // cocos2d::CCArray * m_pBonusEffectArray;// 保存奖励特效
    
public:
    
    YYScene();
    ~YYScene();
    CREATE_FUNC(YYScene);
    void touchBegan(cocos2d::CCSet *pTouches);
    void touchMoved(cocos2d::CCSet *pTouches);
    void touchEnded(cocos2d::CCSet *pTouches);
    void update(float f);
    
    void addLogo();
    void addCover();
    void addLoginUI();
    void addLoadingLayer();
    void initNetConnectLayer();// 初始化网络连接层（显示网络连接时的菊花）
    void resetGameScene();// 清空游戏场景
    void showGameScene();
    
    //提示信息相关
    void initPopUpWindow();
    void showTips(std::string tipInfo,TipType tipType = kTipNormal);
    void confirmTips(cocos2d::CCObject* pSender, cocos2d::gui::TouchEventType type);
    void cancelTips(cocos2d::CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    void updateCameraPosition();// 以屏幕左下角为基准点，随人物移动变更屏幕右下角相对于地图的位置

    void setBlockTile(cocos2d::CCArray *array);// 设置有障碍的地格
    bool isBlockTile(uint16_t col,uint16_t row);// 是否是有障碍的地格 
    void setSceneNameLabel(const char * nameStr);// 设置场景名 
    void setNetLoading(bool isNetLoading);
    
    void onCmdLoadScene(cocos2d::CCArray *arr);//场景加载
    void setSceneData(cocos2d::CCArray *arr);// 设置场景相关数据信息
    void setSceneElements(int layerIndex,cocos2d::CCArray * array);// 初始化场景中全部元素
    void addSprite(cocos2d::CCArray *arr);// 添加精灵
    void removeSprite(cocos2d::CCArray *arr);//删除精灵
    void processSpriteMove(cocos2d::CCArray *arr);//处理精灵移动
    void setDropOdds(cocos2d::CCArray *arr);//设置掉率
    void setRoleControlled(cocos2d::CCArray *arr);//设置可控角色
    void updateEquipComposeUI(cocos2d::CCArray *arr);// 处理装备合成返回的数据
    void setRoleSkillIndex(cocos2d::CCArray *arr);//设置角色可用技能的序号
    void receiveNPCTaskList(cocos2d::CCArray *arr);//接受NPC任务列表
    void processRoleRelive(cocos2d::CCArray *arr);// 处理角色复活
    void receiveDungeonsData(cocos2d::CCArray *arr);// 副本选择界面
    void receiveStageData(cocos2d::CCArray *arr);// 关卡选择界面
    void receiveStageAwardData(cocos2d::CCArray *arr);//关卡奖励及难度确认界面
    void receiveChatMessage(cocos2d::CCArray *arr);//获取聊天信息
    void editItemNumber(cocos2d::CCArray *arr);//编辑物品数量
    void receivePropDetail(cocos2d::CCArray *arr);// 接收物品信息
    void receiveEquipDetail(cocos2d::CCArray *arr);//接收装备信息
    void treasureContainerErrorTips(cocos2d::CCArray *arr);//宝箱操作错误提示
    void onCmdPutonEquip(cocos2d::CCArray *arr);//换装成功
    void reinforceFailedTips(cocos2d::CCArray *arr);// 强化失败提示
    void forgeFailedTips(cocos2d::CCArray *arr);// 锻造失败提示
    void forgeByJewel(cocos2d::CCArray *arr);//用宝石锻造
    void receiveTaskData(cocos2d::CCArray *arr);//添加任务
    void equipReinforce(cocos2d::CCArray *arr);//装备强化
    void getRankDataInfo(cocos2d::CCArray *arr);//排行榜
    void getTrainDataInfo(cocos2d::CCArray *arr);//试炼

    // 商城
    void setShopData(cocos2d::CCArray *arr);
    void receiveAllShopDataFinished(cocos2d::CCArray *arr);
    // 充值
    void setChargeData(cocos2d::CCArray *arr);
    void setChargeSerialNubmer(cocos2d::CCArray *arr);
    // 登陆奖励
    void setLoginAwardData(cocos2d::CCArray *arr);
    void setSignUpData(cocos2d::CCArray *arr);
    void setSignUpFinishData(cocos2d::CCArray *arr);
    void setSignUpBoxAwardData(cocos2d::CCArray *arr);
    // 活跃度
    void receiveActivityData(cocos2d::CCArray *arr);
    // 成就
    void receiveAchievementData(cocos2d::CCArray *arr);
    void setAchievementFinishedTipData(cocos2d::CCArray *arr);
    void setAchievementGetAwardData(cocos2d::CCArray *arr);
    // 占灵
    void receiveProtossBaseDataFinished(cocos2d::CCArray *arr);
    void receiveProtossEquipDataFinished(cocos2d::CCArray *arr);
    void setProtossWaitRecData(cocos2d::CCArray *arr);
    void setProtossWaitRecOneData(cocos2d::CCArray *arr);
    void setProtossBagData(cocos2d::CCArray *arr);
    void setProtossBagOneData(cocos2d::CCArray *arr);
    void setProtossEquipBagData(cocos2d::CCArray *arr);
    void setLightConstellationData(cocos2d::CCArray *arr);
    void setProtossPieceData(cocos2d::CCArray *arr);
    // 邮件
    void setMailListData(cocos2d::CCArray *arr);
    void setMailDetailData(cocos2d::CCArray *arr);
    void setMailNum(cocos2d::CCArray *arr);
    // 公告
    void receiveNoticeInfo(cocos2d::CCArray *arr);
    void vibrate(int duration);
    //活动
    void receivePlayTableAH(cocos2d::CCArray *arr);
    void receivePlayTableAI(cocos2d::CCArray *arr);
    void receivePlayTableTT(cocos2d::CCArray *arr);
    void receivePlayTableTR(cocos2d::CCArray *arr);
    void receivePlayTableTD(cocos2d::CCArray *arr);
    void receivePlayTableDR(cocos2d::CCArray *arr);
    void receiveDungeonDO(cocos2d::CCArray *arr);
    void receiveDungeonDA(cocos2d::CCArray *arr);
    void receiveProtossTO(cocos2d::CCArray *arr);
    void receiveProtossTW(cocos2d::CCArray *arr);
    void receiveBossDungeonBL(cocos2d::CCArray *arr);
    void receiveBossDungeonBC(cocos2d::CCArray *arr);
    void receiveBossDungeonBA(cocos2d::CCArray *arr);
    void receiveWorldBossWC(cocos2d::CCArray *arr);
    void receiveWorldBossWT(cocos2d::CCArray *arr);
    void receiveWorldBossWB(cocos2d::CCArray *arr);
    void receiveWorldBossWK(cocos2d::CCArray *arr);
    void receiveWorldBossWD(cocos2d::CCArray *arr);
    
    
    inline bool isVibrate()
    {
        return m_bVibrate;
    }
    inline bool isNetLoading()
    {
        return m_bNetLoading;
    }
    inline uint8_t getDungeonsIndex()
    {
        return m_u8DungeonsIndex;
    }
    inline uint8_t getChoosedStageIndex()
    {
        return m_u8ChoosedStageIndex;
    }
    inline uint8_t getChoosedStageDifficultIndex()
    {
        return m_u8ChoosedDifficultOptionIndex;
    }
    inline uint8_t getStageStartLevel()
    {
        return m_u8StageStartLevel;
    }
    inline uint8_t getShowItemAmount()
    {
        return m_u8ShowItemAmount;
    }
    inline void setDungeonsIndex(uint8_t u8DungeonsIndex)
    {
        m_u8DungeonsIndex = u8DungeonsIndex;
    }
    inline void setChoosedStageIndex(uint8_t u8ChoosedStageIndex)
    {
        m_u8ChoosedStageIndex = u8ChoosedStageIndex;
    }
    inline void setChoosedDifficultOptionIndex(uint8_t u8ChoosedDifficultOptionIndex)
    {
        m_u8ChoosedDifficultOptionIndex = u8ChoosedDifficultOptionIndex;
    }
    inline void setStageStartLevel(uint8_t u8StageStartLevel)
    {
        m_u8StageStartLevel = u8StageStartLevel;
    }
    inline void setCameraLocked(bool isCamaraMovedByScript)
    {
        m_bCameraLocked = isCamaraMovedByScript;
    }
    
    inline bool isCameraLocked()
    {
        return m_bCameraLocked;
    }
    inline void setCameraMovedByScript(bool isCamaraMove)
    {
        m_bCameraMovedByScript = isCamaraMove;
    }
    
    inline bool isCameraMovedByScript()
    {
        return m_bCameraMovedByScript;
    }
    
    inline void setCameraFollowSprite(bool isCamaraFollowSprite)
    {
        m_bCameraFollowSprite = isCamaraFollowSprite;
    }
    
    inline bool isCameraFollowSprite()
    {
        return m_bCameraFollowSprite;
    }
    void setDungeonUIElement(cocos2d::CCArray * array);// 设置副本界面元素 
    void setChoosedEffect();
    void updateDungeonsUIElements();
    void updateDungeonsUI();
    void dungeonsUITouchHandle();
    
    void sendChoosedDungeonsIndexMessage();
    void setStageUIElement(cocos2d::CCArray * array);// 设置关卡界面元素 
    void sendChoosedStageIndexMessage();
    void sendChoosedStageConfirmMessage();
    void setStageAwardUI(cocos2d::CCArray * array);// 设置关卡奖励界面
    void receiveUserCenterInfo(cocos2d::CCArray *array);
	void funDestroy(CCNode* node);
};

#endif /* defined(__XJ__YYScene__) */
