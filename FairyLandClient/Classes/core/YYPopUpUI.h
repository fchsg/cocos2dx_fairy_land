//
//  YYPopUILayer.h
//  FairyLand II
//
//  Created by 章敏 on 9/25/13. 
//
//

#ifndef __FairyLand_II__YYPopUILayer__
#define __FairyLand_II__YYPopUILayer__

#include <iostream>
#include <cocos2d.h>
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "../common/Common.h"
#include "YYController.h"
#include "YYScene.h"
#include "YYControlUILayer.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYButton.h"
#include "YYNPC.h"
#include "YYGuide.h"
#include "YYMailData.h"
#include "YYGlobalData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace gui;

#define ZORDER_TOPLAYER 10 // Poplayer 所有层中最上层 用于存储特效

#define TAG_FRONT_FACE 0   // 随机奖励卡片正面
#define TAG_BACK_FACE -1   // 随机奖励卡片反面
#define TAG_CHOOSE_FACE -2 // 随机奖励卡片选中面
#define TAG_ICON_BG 11
#define TAG_ITEM_ICON 12
#define TAG_AWARD_STROKE 20 // 奖励界面描边
#define TAG_AWARD_NAME 30 // 奖励界面名称
#define TAG_UI_PLAYER 200  //背包界面
#define TAG_TRIAL_UI_PLAYER 210  //试炼界面
#define TAG_UI_NPC_TEMPLAYER 220
#define TAG_BAG_TREASURE 530  // 宝箱动画标签
#define TAG_JOB_ITEM 1000
#define TAG_SHOULD_ACCEPT_JOB 1100
#define TAG_POPUP_ACHIEVE_WINDOW 2001 //UI 成就提示弹出窗

#define CHATTING_WIDTH 880 // 聊天界面显示字体实际区域宽度
#define CHATTING_HEIGHT 360 // 聊天界面显示字体区域高度
#define CHATTING_FONT_OFFSET 5 // 聊天字体偏移
#define CHATTING_BOX_OFFSET 3 // 聊天盒子偏移
#define CHATTING_FONT_SIZE 32 // 聊天字体大小
#define INTERVAL_FRAME 5

//金币 宝石 经验 icon图标
#define ICON_GLOD_PATH  getFilePath("i-5-0-1-0", kDirTypeIcon, kFileSuffixPng)
#define ICON_JEW_PATH   getFilePath("i-5-0-3-0", kDirTypeIcon, kFileSuffixPng)
#define ICON_EXP_PATH   getFilePath("i-5-0-2-0", kDirTypeIcon, kFileSuffixPng)

// 商城
#define SHOP_UI_RESOURCE_FILE_NAME  getFilePath("NewProject_5",kDirTypeUI,kFileSuffixJson)
#define MAX_PAGE_SHOP_ITEM_COUNT    18
#define TAG_UI_SHOPROOT     10
#define TAG_UI_SHOPBUY_DETAIL   11

// 充值
#define CHARGE_UI_RESOURCE_FILE_NAME        getFilePath("NewProject_26",kDirTypeUI,kFileSuffixJson)
#define MAX_CHARGE_ITEM_COUNT 12
// 神州付
#define HASEEPAY_UI_RESOURCE_FILE_NAME      getFilePath("NewProject_28",kDirTypeUI,kFileSuffixJson)
#define Mobile_CHARGE_QUOTA_NUM 7
#define UNICORN_CHARGE_QUOTA_NUM 6
#define TELECOM_CHARGE_QUOTA_NUM 4
#define CARD_NUMBER_LENGTH 17
#define CARD_PASSWORD_LENGTH 18

static const uint16_t mobile_quota[] = {10, 20, 30, 50, 100, 300, 500};
static const uint16_t unicorn_quota[] = {20, 30, 50, 100, 300, 500};
static const uint16_t telecom_quota[] = {20, 30, 50, 100};

// 占灵
#define PROTOSS_UI_RESOURCE_FILE_NAME           getFilePath("NewProject_4",kDirTypeUI,kFileSuffixJson)
#define MAX_DEVINE_KIND_NUM     5
#define MAX_DEVINE_BAG_NUM      18
#define MAX_PROTOSS_EQUIP_POS   10
#define MAX_PROTOSS_BAG_NUM     100
#define TAG_UI_PROTOSS_BASE     20
#define TAG_UI_PROTOSS_EQUIP    21
#define TAG_UI_PROTOSS_EXCHANGE 22
#define TAG_UI_PROTOSS_SWALLOW  23
#define TAG_DRAG_MOVE_PIC       101

// 连续登录奖励
#define LOGINAWARD_UI_RESOURCE_FILE_NAME        getFilePath("NewProject_9",kDirTypeUI,kFileSuffixJson)
#define MAX_LOGIN_AWARD_NUM     8
#define EACHDAY_LOGIN_AWARD_MAXNUM  4
#define TAG_UI_LOGINAWARD       23

// 登录签到
#define LOGINSIGNUP_UI_RESOURCE_FILE_NAME       getFilePath("NewProject_10",kDirTypeUI,kFileSuffixJson)
#define TAG_UI_LOGINSIGNUP       24
#define MAX_DATE_COUNT_IN_CALENDAR_PAGE     31
#define MAX_BOX_NUM  5

// 登录奖励弹出框
#define POP_UI_RESOURCE_FILE_NAME               getFilePath("NewProject_6",kDirTypeUI,kFileSuffixJson)
#define POP_ZORDER_TAG   5

// 活跃度
#define ACTIVITY_UI_RESOURCE_FILE_NAME          getFilePath("NewProject_15",kDirTypeUI,kFileSuffixJson)
#define ACTIVITY_TASK_TOTAL   5
#define ACTIVITY_TASK_ITEM   7
#define ACTIVITY_WISHTREE_ITEM   4
#define ACTIVITY_MIN_RANK 10

// 成就
#define ACHIEVE_UI_RESOURCE_FILE_NAME           getFilePath("NewProject_17",kDirTypeUI,kFileSuffixJson)
#define ACHIEVE_PER_PAGE_NUM  3
#define ACHIEVE_PAGE_NUM  5
#define ACHIEVE_TYPE_NUM  4


// 邮件
#define MAIL_UI_RESOURCE_FILE_NAME              getFilePath("NewProject_20",kDirTypeUI,kFileSuffixJson)
//排行榜
#define RANK_UI_RESOURCE_FILE_NAME              getFilePath("NewProject_18",kDirTypeUI,kFileSuffixJson)
#define RANK_COLUMN_COUNT 5
#define RANK_ROW_COUNT 6

//设置
#define SETTING_UI_RESOURCE_FILE_NAME           getFilePath("NewProject_27",kDirTypeUI,kFileSuffixJson)

//活动
#define PLAYTABLE_UI_RESOURCE_FILE_NAME         getFilePath("NewProject_22",kDirTypeUI,kFileSuffixJson)

// 活动 副本战斗结果页面
#define PLAYTABLE_RESULTUI_RESOURCE_FILE_NAME   getFilePath("NewProject_30",kDirTypeUI,kFileSuffixJson)

//试炼
#define TRAIN_UI_RESOURCE_FILE_NAME             getFilePath("NewProject_16",kDirTypeUI,kFileSuffixJson)

// 聊天
#define CHAT_UI_RESOURCE_FILE_NAME              getFilePath("NewProject_13",kDirTypeUI,kFileSuffixJson)
#define CHAT_SCENE_TYPE_NUM    4

// 关卡选择界面
#define STAGE_UI_RESOURCE_FILE_NAME             getFilePath("NewProject_11",kDirTypeUI,kFileSuffixJson)

// 关卡奖励界面
#define STAGE_AWARD_UI_RESOURCE_FILE_NAME       getFilePath("NewProject_23",kDirTypeUI,kFileSuffixJson)

// 战斗评价
#define FIGHT_EVALUATION_UI_RESOURCE_FILE_NAME  getFilePath("NewProject_12",kDirTypeUI,kFileSuffixJson)

// 公告
#define NOTICE_UI_RESOURCE_FILE_NAME            getFilePath("NewProject_29",kDirTypeUI,kFileSuffixJson)

// 天赋
#define TALENT_UI_RESOURCE_FILE_NAME            getFilePath("NewProject_8",kDirTypeUI,kFileSuffixJson)

// 背包
#define BAG_UI_RESOURCE_FILE_NAME               getFilePath("NewProject_25", kDirTypeUI, kFileSuffixJson)

// 技能
#define SKILL_UI_RESOURCE_FILE_NAME             getFilePath("NewProject_3", kDirTypeUI, kFileSuffixJson)

// vip
#define VIP_UI_RESOURCE_FILE_NAME               getFilePath("NewProject_19", kDirTypeUI, kFileSuffixJson)

// 任务
#define TASK_UI_RESOURCE_FILE_NAME              getFilePath("NewProject_14", kDirTypeUI, kFileSuffixJson)

#define BUTTON0        getFilePath("205", kDirTypeUI, kFileSuffixPng) //按键（暗）
#define BUTTON1        getFilePath("206", kDirTypeUI, kFileSuffixPng) //按键（亮）
#define BUTTON2        getFilePath("215", kDirTypeUI, kFileSuffixPng) //按键背景框（亮）
#define BUTTON3        getFilePath("215-1", kDirTypeUI, kFileSuffixPng) //按键背景框（暗）

enum TaskUIType
{
    kTaskList = 0,// 任务列表界面
    kNPCTaskList ,// npc任务列表界面
    kDialogInterface ,// npc 对话弹出界面
    kTaskUIDefault
};

enum UIType
{
    kUIBag = 0,
    kUIChatting, /* 聊天框 */
    kUISkill,  /* 技能界面 */
    kUITalent, // 天赋 
    kUITask , // 任务 
    kUIAward, // 奖励 
    kUIShop, //商城 
    kUIProtoss, //占灵 
    kUILoginAward, //连续登录奖励
    kUISigning,  //签到
    kUIActivity, // 活跃度
    kUIAchieve,  // 成就
    kUIRankWar, // 排行榜_战神榜
    kUISetting,  // 设置页面
    kUIUserCenter, //用户中心
    kUINotice,   //公告
    kUIPlayTable,// 活动
    kUIPlayTableFightResult, // 活动_战斗结果页面
    kUITrain,      // 试炼
    kUICharge,      // 充值
    kUIMail,        // 邮件
    kUIDungeons,// 副本选择界面
    kUIStage,// 关卡选择界面
    kUIStageAwardInfo,// 关卡奖励
    kUIFightEvaluation,// 战斗评价
    kUIConvertCode,    //兑换码
    kUIVip,            // vip
    kUIWipeDungeon,    // 扫荡副本页面
    kUIDefaultStatus
};

enum JewelPurpose // 宝石用途
{
    kJewelConsumeForReinforce = 0,
    kJewelConsumeForCompose = 1,
    kJewelConsumeForRelive = 2,
    kJewelConsumeForCommodity = 3,
    kJewelConsumeForLightConstellation = 4,
    kJewelConsumeForFlopAward = 5,
    kJewelConsumeForUpgradeSkill = 6,
    kJewelConsumeForUpgradeAngry = 7,
    kJewelConsumeForActivityUpdate = 8,
    kJewelConsumeForTalentReset = 9,
    kJewelConsumeForBuyChallenge = 10,
    kJewelConsumeForBuyPray = 11,
    kJewelConsumeForBuyQuickEnter = 12
};

enum ButtonType
{
    kButtonClose = 0,//关闭
    kButtonCancelTips = 1,//提示框里的取消（只有一个按键时，确定也是取消）
    kButtonConfirmTips = 2,//提示框里的确定（两个选项中非取消的那个按键）
    kButtonHelp = 3,//帮助按键
    kBagUIPage = 11,//背包页面 11-15分别对应 装备物品、强化、锻造、
    kBagUIProp = 16,//物品栏
    kBagUIEquip = 17,//装备栏
    kBagUIRoleAttributeButton = 18,//角色属性按键
    kAttributeUIBagButton  = 19,//属性界面背包按键
    kBagUIClearUpButton = 20,//整理
    kBagUIReinforceButton1 = 21,//强化
    kBagUIReinforceButton2 = 22,//100%强化
    kBagUIForgeButton = 23,//锻造
    kBagUIGetAllMaterialButton = 24,//补全材料
    kAttributeUIPage = 25,// 属性界面页面 25角色 26社会

    kPopUpUICloseButton = 27,//物品信息界面"关闭"
    kPopUpUISaleButton = 28,//物品属性界面"出售"按键
    kPopUpUIPutOnButton = 29,//物品属性界面"穿上"按键
    kPopUpUIUseButton = 30,//物品属性界面"使用"按键
    kTreasureUICloseButton = 31,//宝箱界面关闭
    kTreasureUIRefreshButton = 32,//宝箱界面刷新
    kTreasureUIConfirmButton = 33,//宝箱界面获得物品或装备
    kTalentUIResetButton = 34, // 重置天赋点
    kTalentUIConfirmButton = 35,//确定加天赋点
    kTalentUISkillIcon1 = 36, //技能图标1-5分别为36-40
    kTalentUITalentIcon1 = 41, // 天赋图标1-8分别为41-48
    kTalentUIAddPointButton = 49,//增加天赋点按钮对应天赋图标1-8分别为49-56
    kTalentUIReducePointButton = 57,//减少天赋点按钮对应天赋图标1-8分别为57-64
    kTalentUICancelDescriptionButton = 65,// 天赋界面取消描述信息
    
    kProtossLightConstellationBtn = 100, // 点亮星座
    kProtossConsumeJewelLightConstellationBtn = 105, // 花费宝石点亮
    kProtossOneKeyDevineBtn = 107, // 一键占灵
    kProtossOneKeyReceiveBtn = 108, // 一键回收
    kProtossOneKeySellBtn = 109, // 一键卖出
    kProtossToExchangeProtossBtn = 110, // 兑换星灵
    kProtossToEquipProtossBtn = 111, // 进入星盘
    kProtossToBaseUIBtn = 112, // 返回占灵界面
    kProtossExchangeBtn = 113, // 交换碎片
    kProtossSwallowBtn = 114,  // 吞噬星灵
    kProtossSelectedAllProtossBtn = 115,  // 选择将要吞噬星灵
    kProtossNomalReceiveForBaseUIBtn = 116, // 普通收取
    kProtossNomalSellForBaseUIBtn = 117, // 占灵普通卖出
    kProtossNomalSellForEquipBagUIBtn = 118, // 星灵装备背包普通卖出
    kProtossCloseProtossInfoForBaseUIBtn = 119, // 删除星灵信息
    kProtossEquipProtossForEquipBagUIBtn = 120, // 装备星灵
    kProtossUpgradeProtossForEquipBagUIBtn = 121, // 升级星灵
    kProtossCloseProtossInfoForEquipBagUIBtn = 122, // 删除星灵信息
};

//商城商品分类
enum ShopPageType
{
    kShopPageInit = -1,     //默认空数据
    kShopHotPage = 0,       //热销
    kShopEquip = 1,         //装备
    kShopItem = 2,          //物品
    kShopGem = 3,           //宝石
    kShopPageNum = 4,
    kShopGiftPage = 5       //赠送
};
// 充值类型
enum ChargePageType
{
    kChargeInit = -1,
    kChargeAlipay = 0,      //支付宝
    kChargeTelephone = 1,   //话费卡
    kChargeInternetBank = 2, //网银
    kChargeHaseePay = 3,     //神州付
    kChargePageNum = 4
};
enum HaseePayPageType
{
    kHaseePayInit = -1,
    kHaseePayMobile = 0, // 移动
    kHaseePayUnicorn = 1, // 联通
    kHaseePayTelecom = 2, // 电信
    kHaseePayPageNum = 3
};
enum ChattingPage
{
    kChattingAllPage= 0,      //综合消息
    kChattingWorldPage ,      //世界消息
    kChattingCityPage ,       //城市消息
    kChattingSystemPage,      //系统消息
    kChattingPrivatePage,     //自己可见
    kChattingNoticePage,      // 存放公告消息
    kChattingCountPage
};

enum AwardStatus
{
    kAwardChooseStatus = 0,
    kAwardNotClickStatus ,
    kAwardReceiveStatus ,
    kAwardDefaultStatus
};

enum BagStatus
{
    kBagMainFaceStatus = 0,
    kBagTreasureStatus ,
    kBagAttributeStatus ,
    kBagDefaultStatus
};

enum AwardActionStatus
{
    kAwardActionShuffleStatus = 0,
    kAwardActionRestoreStatus ,
    kAwardActionRandomStatus ,
    kAwardActionNotClickStatus ,
    kAwardActionDefaultStatus
};

enum UIAwardStatus
{
    kUISignUpAward = 0,
    kUISignUpAddAward,
    kUIBoxAward,
    kUIDefaultAward
};
enum SignUpActionStatus
{
    kActionSignUp = 0,
    kActionSignUpAdd,
    kActionSignUpBoxAward,
    kActionDefault
};
enum PlayTableFightResult
{
    kDungeonResult = 0, // 地下城
    kProtossResult = 1,  // 守卫星灵
    kBossResult = 2   // boss副本
};

class YYLayer;
class YYDragRegion;
class YYButton;
class YYTaskTargetData;
class Property;
class Equipment;
class YYPlayer;
class SkillData;
class TalentData;

class YYMenu :public cocos2d::CCMenu
{
public:
    CREATE_FUNC(YYMenu);
    virtual void registerWithTouchDispatcher(void)
    {
        cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    }
};

//----------------------试炼UI数据------------------------
typedef struct TrialUIData
{
    std::string m_playerIdx;  //玩家ID
    std::string m_playerName; //姓名
    int32_t  m_nLevel;        //等级
    int32_t  m_nRank;         //排名
    uint8_t  m_u8ProFession;  //职业
}TrialUIData;

//----------------------成就信息弹出框数据信息----------------
class AchieveTips : public CCObject
{
     CC_SYNTHESIZE(uint32_t, m_nIdx, Idx);
     CC_SYNTHESIZE(uint16_t, m_nNum, Num);
     CC_SYNTHESIZE(std::string, m_sIcon, Icon);
     CC_SYNTHESIZE(std::string, m_Desc, Desc);
    
public:
    AchieveTips():
    m_nNum(0)
    ,m_sIcon("")
    ,m_Desc("")
    ,m_nIdx(0)
    {
    }
    ~AchieveTips(){}
    inline bool init()
    {
        return true;
    }
    CREATE_FUNC(AchieveTips);
};

//----------------------活动一级页面数据----------------
class PlayTableData : public CCObject
{
    CC_SYNTHESIZE(uint32_t, m_nIndex, Index); // 索引
    CC_SYNTHESIZE(std::string, m_sName, Name); // 名称
    CC_SYNTHESIZE(std::string, m_sIcon, Icon); // 图标
    CC_SYNTHESIZE(uint8_t, m_nNeedLevel, NeedLevel); // 活动需要玩家等级
    CC_SYNTHESIZE(std::string, m_sTime, Time); // 开启时间
    CC_SYNTHESIZE(std::string, m_sDescription, Description); // 活动内容
    CC_SYNTHESIZE(std::string, m_sReward, Reward); // 活动奖励
    CC_SYNTHESIZE(bool, m_bOpen, Open);            // 是否开启
    
public:
    PlayTableData()
    :m_nIndex(0)
    ,m_sName("")
    ,m_sIcon("")
    ,m_nNeedLevel(0)
    ,m_sTime("")
    ,m_sDescription("")
    ,m_sReward("")
    ,m_bOpen(false)
    {
    }
    ~PlayTableData(){}
    inline bool init()
    {
        return true;
    }
    CREATE_FUNC(PlayTableData);
};

//-----------------------活动二级页面boss副本数据------------
class BossDungeonUIData : public CCObject
{
    CC_SYNTHESIZE(uint32_t, m_nID, ID);  // boss 副本id
    CC_SYNTHESIZE(std::string, m_sIcon, Icon); // 图标
    CC_SYNTHESIZE(std::string, m_sName, Name); // 名称
    CC_SYNTHESIZE(uint8_t, m_nNeedLevel, NeedLevel); // 需要玩家等级
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_arrItemName, ArrItemName); // 掉落物品
    
public:
    BossDungeonUIData()
    :m_nID(0)
    ,m_sIcon("")
    ,m_sName("")
    ,m_nNeedLevel(0)
    ,m_arrItemName(NULL)
    {
    }
    inline bool init()
    {
        return true;
    }
    ~BossDungeonUIData()
    {
        CC_SAFE_RELEASE(m_arrItemName);
    }
    CREATE_FUNC(BossDungeonUIData);
};

class YYTouchGroupDelegate
{
public:
    YYTouchGroupDelegate()
    {}
    ~YYTouchGroupDelegate()
    {}
    virtual void touchGroupCallBack() = 0;
};
class YYTouchGroup : public cocos2d::gui::TouchGroup
{
    YYTouchGroupDelegate *pDelegate;
    UIType kState;
public:
    CREATE_FUNC(YYTouchGroup);
    void setDelegate(YYTouchGroupDelegate *pDelegate, UIType status)
    {
        this->pDelegate = pDelegate;
        this->kState = status;
    }
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
    {
        TouchGroup::ccTouchBegan(pTouch, pEvent);
        CCPoint pos = pTouch->getLocation();
        if (Controller::getInstance()->getGameStatus() == kStatusUI)
        {
             CCSize winSize = CCDirector::sharedDirector()->getWinSize();
            if (kState == kUIProtoss)
            {
                float fWidth = 320, fHeight = 320;
                //if (pos.x <= 321 || pos.y <= 197 || pos.x >= 643 || pos.y >= 514)
                if ((pos.x < (winSize.width - fWidth) / 2)
                    || (pos.y < (winSize.height - fHeight) / 2)
                    || (pos.x > (winSize.width + fWidth) / 2)
                    || (pos.y > (winSize.height + fHeight) / 2))
                {
                    pDelegate->touchGroupCallBack();
                }
            }
            else if (kState == kUIAchieve)
            {
                float fWidth = 470, fHeight = 110;
                if ((pos.x >= (winSize.width - fWidth) / 2)
                    && (pos.y >= (winSize.height - fHeight) / 2)
                    && (pos.x <= (winSize.width + fWidth) / 2)
                    && (pos.y <= (winSize.height + fHeight) / 2))
                {
                    pDelegate->touchGroupCallBack();
                }
            }
            else if (kState == kUISigning)
            {
                pDelegate->touchGroupCallBack();
            }
        }
        return true;
    }
};

class PopUpUI : public cocos2d::CCLayer,public cocos2d::extension::CCEditBoxDelegate, public YYTouchGroupDelegate,public CCTextFieldDelegate
{
    CC_SYNTHESIZE(UIType, m_eUIType, UIType);// UI类型
    CC_SYNTHESIZE(cocos2d::CCDictionary *,m_pPlayerEquipDictionary,PlayerEquipDictionary); //玩家身上装备 
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pDragRegionDictionary, DragRegionDictionary);//UI可拖拽区域
    CC_SYNTHESIZE(YYPlayer *, m_pRoleByControlled, RoleByControlled);// 控制角色 
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pRoleWealthArray,RoleWealthArray);//打开背包时记录角色金币 和 宝石
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pChattingButtonArray,ChattingButtonArray);
    CC_SYNTHESIZE(cocos2d::gui::UIWidget* ,m_pTaskDialogueInfo,TaskDialogueInfo);// 对话信息 
    CC_SYNTHESIZE(cocos2d::gui::UIWidget* ,m_pTaskDialogueButton,TaskDialogueButton);// 对话按钮
    CC_SYNTHESIZE(YYNPC *,m_pTaskNPC,TaskNPC); // 任务NPC
    CC_SYNTHESIZE(TaskUIType,m_eTaskUIType,TaskUIType); // 当前任务界面
    CC_SYNTHESIZE(cocos2d::CCPoint ,m_cAdjustScreenPosition,AdjustScreenPosition); // 适应屏幕
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pAwardButtonArray,AwardButtonArray); // 随机奖励按钮牌(存放装备) 
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pAwardPositionArray,AwardPositionArray); // 存储卡牌坐标 
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pChooseAwardArray,ChooseAwardArray);  // 选择奖励
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pSpecialEffectArray,SpecialEffectArray);// 弹出界面特效处理
    CC_SYNTHESIZE(AwardStatus,m_u8AwardStatus,AwardStatus); // 奖励的状态 正 反 点击
    CC_SYNTHESIZE(AwardActionStatus,m_u8AwardActionStatus,AwardActionStatus); // 动作执行步骤的状态
    
    CC_SYNTHESIZE(cocos2d::CCArray **,m_pChannelBoxArray,ChannelBoxArray); //  聊天频道
    //商城 
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pShopContentDictionary, ShopContentDictionary);    //商城列表的内容
    CC_SYNTHESIZE(bool, m_bIsShopInit, IsShopInit); // 商城是否已经初始化过了 
    CC_SYNTHESIZE(uint8_t, m_u8ShopStatus, ShopStatus); //商城初始化步骤状态 
    CC_SYNTHESIZE(ShopPageType, m_eCurShopTabType, CurShopTabType);    //当前正显示商城哪个页签内容
    CC_SYNTHESIZE(int, m_nCurBuyShopItemIndex, CurBuyShopItemIndex);    //当前选中要购买的商品的id 
    CC_SYNTHESIZE(int, m_nCurBuyMoneyType, CurBuyMoneyType);    // 买当前选中商品要用的货币类型 0:money, 1:jew 
    CC_SYNTHESIZE(int, m_nBuyNum, BuyNum);  //要购买当前选中商品的数量 
    CC_SYNTHESIZE(bool, m_bIsShopDetailOpen, IsShopDetailOpen); //商品选择购买数量要购买的页面是否打开着
    
    // 充值
    CC_SYNTHESIZE(ChargePageType, m_kChargePageType, ChargePageType);
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pChargeContentDictionary, ChargeContentDictionary);    //充值列表
    CC_SYNTHESIZE(int, m_nChargeID, ChargeID);
    CC_SYNTHESIZE(std::string, m_sSerialNumber, SerialNumber);
    // 神州付
    CC_SYNTHESIZE(std::string, m_nCardNumber, CardNumber);
    CC_SYNTHESIZE(std::string, m_nCardPassword, CardPassword);
    CC_SYNTHESIZE(HaseePayPageType, m_kHaseePayPageType, HaseePayPageType);
    CC_SYNTHESIZE(int, m_nSelectedQuota, SelectedQuota);
    CC_SYNTHESIZE(int, m_nYoYoPayQuota, YoYoPayQuota);
    //占灵
    CC_SYNTHESIZE(bool, m_bIsEnterProtossEquipUI, IsEnterProtossEquipUI);
    CC_SYNTHESIZE(bool, m_bIsProtossInit, IsProtossInit); //占灵基本数据是否已经初始化过了
    CC_SYNTHESIZE(bool, m_bIsProtossEquipInit, IsProtossEquipInit); //占灵装备数据是否已经初始化过了
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pProtossEquipList, ProtossEquipList);  //星灵装备列表
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pProtossWaitforRecvList, ProtossWaitforRecvList);   //等待被收取的占灵出来的星灵列表 表
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pProtossExchangeRuleList, ProtossExchangeRuleList);    //星灵兑换规则列表
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pProtossBagList, ProtossBagList);  //星灵背包列表
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pProtossPieceList, ProtossPieceList);      //星灵碎片列表
    CC_SYNTHESIZE(int, m_nProtossCount, ProtossCount);  //碎片数据
    CC_SYNTHESIZE(int, m_pCurDevineIndex, CurDevineIndex);  //当前可进行占灵的类型的索引（从0开始）
    CC_SYNTHESIZE(int, m_nDetailIndex, DetailIndex);    //查看和处理星灵详细内容时星灵的索引值（从0开始）
    CC_SYNTHESIZE(int, m_nProtossClickTimes, ProtossClictTimes);
    CC_SYNTHESIZE(float, m_fProtossClickInterval, ProtossClickInterval);
    CC_SYNTHESIZE(bool, m_bIsFromEquipToBag, IsFromEquipToBag);
    CC_SYNTHESIZE(int, m_nPAState, PAState);   // 1：表示普通占灵，2：一键占灵；3：一键拾取，4：一键卖出
    CC_SYNTHESIZE(int, m_nPBState, PBState);   // 1：表示一键拾取，2：一键合成
    CC_SYNTHESIZE(int, m_nProtossEquipID, ProtossEquipID);
    
    //登录签到
    CC_SYNTHESIZE(bool, m_bIsSignUpInit, IsSignUpInit);
    CC_SYNTHESIZE(int, m_nIndexOfFirstDateInCalendar, IndexOfFirstDateInCalendar);
    CC_SYNTHESIZE(UIAwardStatus, m_u8UIAwardStatus, UIAwardStatus);
    CC_SYNTHESIZE(SignUpActionStatus, m_u8SignUpActionStatus, SignUpActionStatus);
    
    // 活跃度
    CC_SYNTHESIZE(bool, m_bIsActivityInit, IsActivityInit);
    CC_SYNTHESIZE(bool, m_bIsInitActivityTaskData, IsInitActivityTaskData); // 任务数据是否初始化完成
    CC_SYNTHESIZE(bool, m_bIsInitActivityWishTreeData, IsInitActivityWishTreeData); // 愿望树数据是否初始化完成
    CC_SYNTHESIZE(int, m_nFreeFromUpdateNum, FreeFromUpdateNum); // 完成次数
    CC_SYNTHESIZE(int, m_nConsumeJewNum, ConsumeJewNum); // 付费刷新需要宝石
    CC_SYNTHESIZE(int, m_nMyScore, MyScore);// 分数
    CC_SYNTHESIZE(int, m_nBoxID, BoxID);    // 宝箱ID
    CC_SYNTHESIZE(int, m_nWishTreeLevel, WishTreeLevel); // 愿望树等级
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pActivityTaskArray, ActivityTaskArray);   //任务数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pActivityTreeArray, ActivityTreeArray);   //活跃树数据
    
    //排行榜
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pDictRankData, DictRankData);      //排行榜数据
    CC_SYNTHESIZE(int, m_nRankCurPageID, RankCurPageID);     //当前按钮页面ID
    CC_SYNTHESIZE(int, m_nRankPrePageID, RankPrePageID);     //上一按钮页面ID
    cocos2d::gui::UIButton * m_pRankPageButton;              // 排行榜当前选中的页签
    
    // vip
    CC_SYNTHESIZE(int, m_nVipCurPageID, VipCurPageID);     //vip描述当前页
    CC_SYNTHESIZE(int, m_nVipPageCnt, VipPageCnt);         //vip 总页数
    
    //活动
    CC_SYNTHESIZE(int, m_nPlayTableCurItemID, PlayTableCurItemID);  //一级页面当前选中项
    CC_SYNTHESIZE(bool, m_bPlayTableWhichPage, PlayTableWhichPage); //true:一级页面 false:二级页面
    CC_SYNTHESIZE(bool , m_bPlayTableBtnTouch, PlayTableBtnTouch);    //UI播放 按钮是否可以点击
    CC_SYNTHESIZE(cocos2d::CCArray *, m_arrPalyTableData, PalyTableData); // 活动UI 内容数据
    
    //转盘数据
    CC_SYNTHESIZE(CCArray* , m_arrTurnTableData, ArrTurnTableData); //奖励物品信息
    CC_SYNTHESIZE(bool , m_bTurnOrGet, TurnOrGet);                 //true 抽奖 false 领奖
    CC_SYNTHESIZE(cocos2d::gui::UIImageView*, m_pTrunTableImageView, TrunTableImageView); //转盘UI指针
    CC_SYNTHESIZE(bool , m_bTurnTalbeRunning, TurnTalbeRunning);                          //转盘是否转动
    CC_SYNTHESIZE(uint32_t, m_nTurnTableCount, TurnTableCount);                           //转盘转动计数器
    CC_SYNTHESIZE(uint8_t, m_nTurnTablePoint, TurnTablePoint);                         //转盘转动点数
    CC_SYNTHESIZE(CCPoint, m_sTurnTableMoveSPoint, TurnTableMoveSPoint);               //获奖icon 移动开始位置
    CC_SYNTHESIZE(CCPoint, m_sTurnTableMoveEPoint, TurnTableMoveEPoint);               //获奖icon 移动终点位置
    CC_SYNTHESIZE(int, m_nTurnTableSumTicketCount, TurnTableSumTicketCount);         //当前抽奖券数量
    CC_SYNTHESIZE(int, m_nTurnTableConsumeTicketCount, TurnTableConsumeTicketCount); //消耗抽奖券数量
    CC_SYNTHESIZE(cocos2d::gui::UIImageView *, m_pTurnTableMoveImgView, TurnTableMoveImgView); //Turn 奖励UIImageView
    
    //骰子数据
    CC_SYNTHESIZE(int , m_nDiceBtnState, DiceBtnState);                             //骰子按钮状态 0 免费投骰 1 收取 2 宝石
    CC_SYNTHESIZE(int , m_nDiceCousumeJewCount, DiceCousumeJewCount);               //宝石投骰消耗数量
    CC_SYNTHESIZE(int , m_nDiceStartGrid, DiceStartGrid);                           //当前所在格子位置 -1 为开始位置
    CC_SYNTHESIZE(int , m_nDiceEndGrid, DiceEndGrid);                               //走到格子位置
    CC_SYNTHESIZE(int , m_nDicePoint, DicePoint);                                   //骰子点数
    CC_SYNTHESIZE(CCArray *, m_arrDiceData, ArrDiceData);                           //骰子奖励信息
    CC_SYNTHESIZE(cocos2d::gui::UIImageView *, m_pDiceMoveImgView, DiceMoveImgView); //骰子 选中框 UIImageView
    CC_SYNTHESIZE(YYSprite *, m_pDiceDciePointPic, DiceDciePointPic); //骰子 显示点数静态图片
    
    // 地下城数据
    CC_SYNTHESIZE(int *, m_arrDungeonResetTicket, DungeonResetTicket);     // 地下城重置所需门票个数
    CC_SYNTHESIZE(bool *, m_bDungeonEnable, DungeonEnable);              // 地下城可否挑战
    CC_SYNTHESIZE(bool *, m_bDUngeonFirstLogin, DUngeonFirstLogin);      // 地下城是否是每天第一次进入
    CC_SYNTHESIZE(int *, m_arrDungeonNeedPlayLevel, DungeonNeedPlayLevel); // 地下城所需玩家等级
    CC_SYNTHESIZE(int , m_DungeonHaveTicket, DungeonHaveTicket);         // 玩家拥有地下城门票数
    CC_SYNTHESIZE(int , m_DungeonSelectLevel, DungeonSelectLevel);       // 地下城选择等级 初 中 高
    CC_SYNTHESIZE(int * , m_arrDungeonResetCnt, DungeonResetCnt);             // 地下城每天最多重置次数
    
    // 守卫星灵数据
    CC_SYNTHESIZE(int *, m_arrProtossNeedPlayLevel, ProtossNeedPlayLevel); // 守卫星灵所需玩家等级
    CC_SYNTHESIZE(bool *, m_bProtectProtossFinish, ProtectProtossFinish);  // 守卫星灵是否已完成
    CC_SYNTHESIZE(int, m_nProtectProtossDifficulty, ProtectProtossDifficulty);  // 守卫星灵难度选择
    CC_SYNTHESIZE(int *, m_arrProtectProtossWipe, ProtectProtossWipe);  // 守卫星灵扫荡层数
    
    // boss副本数据
    CC_SYNTHESIZE(cocos2d::gui::PageView *, m_pUIBossDungeonPageView, UIBossDungeonPageView); // boss副本 pageView
    CC_SYNTHESIZE(uint8_t, m_nUIBossDungeonCurPage, UIBossDungeonCurPage); // boss 副本 当前选中页
    CC_SYNTHESIZE(uint8_t, m_nUIBossDungeonJewCnt, UIBossDungeonJewCnt); // 购买挑战次数需求钻石数量
    CC_SYNTHESIZE(uint8_t, m_nUIBossDungeonResidueCnt,UIBossDungeonResidueCnt); // 剩余挑战次数
    CC_SYNTHESIZE(uint8_t, m_nUIBossDungeonEnableBuyCnt, UIBossDungeonEnableBuyCnt); // 可购买挑战次数
    CC_SYNTHESIZE(CCArray* , m_arrUIBossDungeonData, UIBossDungeonData); // boss副本UI 数据容器
    CC_SYNTHESIZE(bool, m_bUIBossDungeonTopTouch, UIBossDungeonTopTouch); // 弹出页面屏蔽其他按钮 ture 弹出页面 false 一级页面
    CC_SYNTHESIZE(uint8_t, m_nUIBossDungeonCurSelIdx, UIBossDungeonCurSelIdx); // 当前选中的副本索引
    CC_SYNTHESIZE(uint8_t, m_nUIBossWipeCnt, UIBossWipeCnt); // 可以被扫荡的副本数
    
    // 世界boss副本数据
    CC_SYNTHESIZE(CCArray *, m_arrUIWorldBossRankList, UIWorldBossRankList); // 伤害排行榜
    CC_SYNTHESIZE(std::string, m_sUIWorldBossAniName, UIWorldBossAniPaht); // boss UI动画路径
    CC_SYNTHESIZE(int32_t, m_nUIWorldBossPrayJew, UIWorldBossPrayJew); // 祝福所需宝石数
    CC_SYNTHESIZE(int32_t, m_nUIWorldBossQuickEnterJew, UIWorldBossQuickEnterJew); // 快速进入所需宝石数
    CC_SYNTHESIZE(int32_t, m_nUIWorldBossCDTime, UIWorldBossCDTime); // 世界boss CD时间
    CC_SYNTHESIZE(int32_t, m_nUIWorldBossCurSecondTime, UIWorldBossCurSecondTime); // 世界boss 当前时间 (单位秒)
    CC_SYNTHESIZE(std::string, m_sUIWorldBossPlayerDamagePoint, UIWorldBossPlayerDamagePoint); // 玩家自身攻击数据
    CC_SYNTHESIZE(YYSprite* , m_pUIWorldBossPortrait, UIWorldBossPortrait); // 世界boss 头像
    
    
    //试炼
    CC_SYNTHESIZE(uint8_t, m_u8TrainSelIdx, TrainSelIdx);             //试炼选择对手ID
    CC_SYNTHESIZE(bool, m_bTrialResult, TrialResult);                 //试炼结果
    
    // 成就
    CC_SYNTHESIZE(bool, m_bIsAchieveInit, IsAchieveInit);
    CC_SYNTHESIZE(int, m_nCurrentAchievePage, CurrentAchievePage);
    CC_SYNTHESIZE(int, m_nCurrentAchieveClassIndex, CurrentAchieveClassIndex);
    CC_SYNTHESIZE(int, m_nCurrentAchieveGroupIndex, CurrentAchieveGroupIndex);
    CC_SYNTHESIZE(int, m_nCurrentAchieveItemIndex, CurrentAchieveItemIndex);
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pAchieveTipsArray, AchieveTipsArray);
    CC_SYNTHESIZE(bool, m_bIsPopUpAchievementTip, IsPopUpAchievementTip);

    // 邮件
    CC_SYNTHESIZE(cocos2d::CCArray * ,  m_pMailListArray, MailListArray);
    CC_SYNTHESIZE(cocos2d::CCTextFieldTTF * , m_MailTitleTextField, MailTitleTextField);
    CC_SYNTHESIZE(cocos2d::CCTextFieldTTF * , m_MailContentTextField, MailContentTextField);

    // 副本
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pRendDungeonActionArray, RendDungeonActionArray);// 存储 副本界面精灵动画
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pStageDataArray, StageDataArray);// 关卡界面
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pDungeonsDataArray, DungeonsDataArray);// 副本
    CC_SYNTHESIZE(cocos2d::CCDictionary * ,m_pDungeonsButtonDictionary, DungeonsButtonDictionary);// 副本、关卡界面的按键容器
    CC_SYNTHESIZE(cocos2d::CCDictionary * ,m_pAwardDictionary, AwardDictionary);// 随机奖励,key对应123三个难度
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pAwardArray, AwardArray);// 实际获得奖励物品id
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pBonusEffectArray, BonusEffectArray);// 保存奖励特效
    CC_SYNTHESIZE(cocos2d::CCString * ,m_pAwardString, AwardString);// 固定关卡奖励
    CC_SYNTHESIZE(bool * ,m_pIsChoosedDungeon, IsChoosedDungeon);// 是否选中了某个副本
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pFinalAwardArray,FinalAwardArray); // 记录随机最终抽取奖品
    
    // 任务
    CC_SYNTHESIZE(int32_t, m_nAcceptedTaskIndex, AcceptedTaskIndex);
    
    //背包
    CC_SYNTHESIZE(CCArray*, m_pBagAllEquip, BagAllEquip);              //所有装备
    CC_SYNTHESIZE(int, m_pBagValidEquipOnBodyCount, BagValidEquipOnBodyCount);      //可强化或锻造已装备有效个数
    CC_SYNTHESIZE(cocos2d::gui::UIImageView*, m_pBagSelImg, BagSelImg)  //背包列表选中框
    
    //兑换码
    CC_SYNTHESIZE(cocos2d::extension::CCEditBox *, m_pConvertCodeEditBox, ConvertCodeEditBox); //兑换码输入框
    CC_SYNTHESIZE(cocos2d::extension::CCEditBox *, m_pCardNumber, CardNumberEB);
    CC_SYNTHESIZE(cocos2d::extension::CCEditBox *, m_pCardPassword, CardPasswordEB);
    
private:
    
    int m_nButtonIndex;// 按键序号
    cocos2d::gui::TouchGroup *m_pUITouchPop;
    cocos2d::gui::TouchGroup *m_pUITouchGroup;  // 所有cocostudio 界面根层
    
    JewelPurpose m_eConsumePurpose;// 花费宝石的目的
    int m_nConsumeJewelNum;
    //————————————————触控相关——————————————
    cocos2d::CCPoint m_StartPoint,m_CurTouchPoint,m_PreTouchPoint;
    bool m_bTouchBegan,m_bTouchMoved,m_bTouchEnded;
    
    cocos2d::gui::UIButton * m_pBagPageButton;// 背包界面当前选中的页签
    cocos2d::gui::UIButton *m_pItemTypeButton;// 当前选中的背包分类按键
    cocos2d::gui::UIButton *m_pAttributePageButton;// 当前选中的属性页签按键
    Equipment *m_pBagEquip;// 背包内被拖动的装备
    Equipment *m_pReplacedEquip;// 被替换的装备
    cocos2d::gui::ImageView *m_pBagCell;// 操作的装备所属背包格
    cocos2d::gui::ImageView *m_pReplacedCell;// 被替换装备所在格
    uint8_t m_u8ReinforceIndex;// 被强化的装备索引
    uint8_t m_u8ComposeIndex;// 被合成的装备索引
    bool m_bCompose; // 是否有物品合成
    int32_t m_nComposeSlotX;// 装备合成区域横向位移
    bool m_bReinforceConfirmed;  // 是否确定强化，拖拽装备至强化区域时为false
    bool m_bReinforceSuccess;// 强化成功
    CCLayer* m_pTopLayer;     // 界面layer最上层 用于临时存储特殊事件响应
    bool m_bEquipDescription;// 是否为装备描述界面
    
    //---------------宝箱相关----------------
    bool m_bTreasureOpened;
    bool m_bReadingTreasureData;
    uint16_t m_u16RefreshCount;// 刷新计数
    uint16_t m_u16RefreshAmount;// 最大可刷新次数
    uint16_t m_u16RefreshConsume;// 刷新所需花费的宝石数量
    std::string m_sTreasureID;
    
    //------------- chatting --------------
    YYLayer * m_pChatingLayer;
    u_int8_t    m_nChannelIndex;
    cocos2d::extension::CCEditBox * m_pAccountTF;
    cocos2d::extension::CCScrollView * m_pScrollView;
    bool m_bChattingTouchBegan;
    cocos2d::CCPoint m_cChattingTouchPoint;
    bool m_bIsScroll; // 公告消息是否正在滚动
    
    //------------- skill -------------
    cocos2d::gui::UIButton *m_pSkillPageButton;// 技能界面当前选中的页签
    int m_nGestureType;// 当前正在设置的手势技能类型
    int m_nSkillIconIndex;// 被选中的技能图标序号
    int m_nSkillDataIndex;// 被选中的技能数据序号
    cocos2d::gui::UIButton* m_pTalentBtn;   // 被选中天赋按钮
    bool m_bSkillDataUpdateOver;
    cocos2d::CCSprite *m_pDargSkillIcon;// 被拖动的技能图标 
    
    //---------protoss-----------
    cocos2d::CCRect m_rProtossBagTouchRect;
    
    //---------task--------------
    bool m_bWaitingForFindTarget; // 等待任务寻找目标
    bool m_bWaitingForUpdateTask; // 等待下一个任务界面
    int m_nChooseTaskIndex;// 在任务列表中选中的任务的编号
    bool m_bIsOpenTask;
    int m_nRecordTaskInfoCount;
    
    //-----------翻牌奖励------------
    int m_nMaxExcludeCount;  // 最大可排除数 
    int m_nCurrentExcludeCount; // 当前可排除数 
    int m_nMaxAwardCount; // 最大奖励数量
    int m_nCurrentAwardCount; // 已获得奖励计数
    cocos2d::gui::UILabel* m_pUILabel; // 记录奖励可排除数 
    int m_nMaxZorder; // 记录最大深度 
    int m_nStartFrame ;
    int m_nIntervalFrame; // 记录随机框转动频率 
    int m_nRandomCount; // 记录随即框转动次数 
    int m_nRandomIndex; // 记录随即框最后随即索引 
    int m_nNeedJewelCount; // 记录钻石翻牌所需数量 
    cocos2d::gui::UIImageView* m_pRandomImage;
    cocos2d::gui::UIButton* m_pShuffle_Button; // 排除或选中奖励按钮
    //---------- 商城 ----------
    int m_nShopBuyEditTag;
    float m_fScrollViewLayerY;
    float m_pfItemChildren[MAX_PAGE_SHOP_ITEM_COUNT];
    bool m_bIsFirstEnterShop;
    GameStatus m_eGameStatePreviousForShop;
    UIType m_eKUITypePreviousForShop;
    TouchGroup *m_pUIShopTouchGroup;
    // -------------充值----------------
    bool m_bInitChargeInfo;
    CCPoint m_obScrollViewPoint;
    GameStatus m_eGameStatePreviousForCharge;
    UIType m_eKUITypePreviousForCharge;
    TouchGroup *m_pUIChargeTouchGroup;
    TouchGroup *m_pHaseePayTouchGroup;

    //---------- 签到 ----------
    uint8_t m_u8SignUpDayNum;
    uint8_t m_u8SignUpDay;
    uint8_t m_uSignUpAddDay;
    uint8_t m_u8SignUpBoxAwardBtnTag;
    bool m_bSignUpSuccessed;
    bool m_bAwardPosSave;
    bool m_bBoxAwardPosSave;
    CCPoint aWardPoint[4];
    CCPoint boxAwardPoint[3];
    uint8_t m_u8SignUpNum;
    uint8_t m_u8AddSignUpNum;
    cocos2d::CCArray *m_pArraySignUpDate;
    cocos2d::CCArray *m_pArrayAddSignUpDate;
    cocos2d::CCArray *m_pArrayBoxAward;
    
    //-----------登录奖励索引-----------
    uint8_t m_u8AwardIndex;
    
    //-----------占灵-------------
    cocos2d::gui::UIScrollView *m_pUIScrollView;
    cocos2d::gui::Layout *m_pEquipPanel;
    cocos2d::gui::UIButton *m_pDevineBtn;
    int m_nDragProtossID;
    int m_nWaitCompositeProtossID;
    int m_pArrayRecPosID[MAX_DEVINE_BAG_NUM];
    int m_pArrayEquitBagPosID[MAX_PROTOSS_BAG_NUM];
    int m_nProtossPosID;
    int m_pGoldProtoss[MAX_DEVINE_KIND_NUM];
    int m_pJewProtoss[MAX_DEVINE_KIND_NUM];
    int m_nLightConstellationID;
    ShopPageType m_kShopPageType;
    float m_fPosY;
    int m_nSwallowProtossPosID;
    bool m_bSelectedAllProtoss;
    //------------成就-------------
    cocos2d::gui::UIButton *m_pBtnAchievementClass; //分类UI button
    cocos2d::gui::UIPageView *m_pUIPageView;          //分项UI pageView
    float m_fStartTime;
    vector<AchieveTips> vec_AchieveTips;
    //------------邮件-------------
    bool m_bInitMailInfo;
    uint16_t m_uMailNum; // 未读邮件数量
    uint16_t m_uMailNumBackup;
    int m_nMailIndex;
    YYMailDetailData *m_pMailDetalData;
    // -----------副本UI相关-------------
    float m_fCurrentScrollDistance,m_fMaxScrollDistance;
    uint8_t m_u8DungeonAmount;// 副本总数
    uint8_t m_u8DungeonsIndex;// 被选中副本的索引
    float m_fRotateAngle;// 关卡选择转盘旋转角度
    float m_fMaxAngle;// 转盘最大旋转角度
    int m_nTurntableOffsetY;// 转盘中心点相对于屏幕的偏移量
    cocos2d::CCSprite * m_pTurntable;// 转盘
    cocos2d::CCSpriteBatchNode * m_pTurntableBatchNode;// 集合转盘上所有元素的节点
    cocos2d::CCLabelTTF * m_pDungeonsTitelLabel;// 标题
    cocos2d::extension::CCScale9Sprite *m_pDungeonsTitleFrame;// 副本标题框
    bool m_bWipeDungeonTouchEnable; // 扫荡页面按钮是否可以点击
    
    // ——————关卡相关——————
    uint32_t m_u32CurrentPhysicalStrength;
    uint32_t m_u32ConsumePhysicalStrength;
    uint8_t m_u8StageAmount;// 关卡总数
    uint8_t m_u8ChoosedStageIndex;// 被选中副本的索引
    int *m_pStageLevel;
    int m_nStageNum;
    bool m_bStageItemScroll;
    bool m_bStageItemChoosed;
    bool m_bCloseChoosed;
    bool m_bEnter;
    YYButton * m_pDifficultButton[3];// 难度选择按键
    uint8_t m_u8UnlockDifficultOptions;// 已解锁的难度
    uint8_t m_u8ChoosedDifficultOptionIndex;// 选择的关卡难度
    cocos2d::CCLabelTTF *m_pDifficultLabel[3];// 难度标签
    int m_DifficultInfo[3][3];
    cocos2d::gui::UILabel *m_pLabelRefreshNum;
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
    uint8_t m_u8CurrentActiveItemIndex;// 战斗评价界面当前活动项
    uint8_t m_u8ShowItemAmount;
    void updateFightResult();
public:
    
    PopUpUI();
    ~PopUpUI();
    CREATE_FUNC(PopUpUI);
    
    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);
    
    bool init();
    void touchBegan(cocos2d::CCTouch * touch);
    void touchMoved(cocos2d::CCTouch * touch);
    void touchEnded(cocos2d::CCTouch * touch);
    void update();
    
    inline cocos2d::gui::TouchGroup *getUITouchGroup()
    {
        return m_pUITouchGroup;
    }
    inline cocos2d::gui::TouchGroup *getUITouchTop()
    {
        return m_pUITouchPop;
    }
    void exitUI();
    void closeUI(CCObject* pSender, cocos2d::gui::TouchEventType type); //关闭UI
    
    void closePopUpUI();
    
    bool isTouchUnable(CCObject * sender, cocos2d::gui::TouchEventType type, bool bPlayEffect = true);//是否触摸无效 默认播放常规点击音效
    
    void updatePropertyInfo();// 更新角色财产信息
    
    
    void helpButtonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void hideHelpInterface(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    static cocos2d::ccColor3B getRGBValue(uint8_t nColorType);
    static cocos2d::gui::UILabel* createLabel(cocos2d::CCSize size, cocos2d::CCTextAlignment kAlignment, int fntSize, cocos2d::ccColor3B color);
    Widget* getPopWindow(std::string str1, std::string str2, bool isVisible);
    bool isDigtal(std::string str);
    #pragma mark 背包
    
    inline bool isReinforceSuccess() const
    {
        return m_bReinforceSuccess;
    }
    inline void setReinforceSuccess(bool isSuccess)
    {
        m_bReinforceSuccess = isSuccess;
    }

    void setConsumeJewelTips(JewelPurpose purpose, int num);// 宝石消耗提示
    void hideConsumeTips();
    void cancelConsume(cocos2d::CCObject *pSender ,cocos2d::gui::TouchEventType type);
    void confirmConsume(cocos2d::CCObject *pSender ,cocos2d::gui::TouchEventType type);
    void transferChildren(CCNode *node1,CCNode *node2,cocos2d::CCPoint offset = cocos2d::CCPointZero,int startNodeIndex = 0);
    void updateItemIconInBag(bool isEquip);// 物品消耗（道具添加、删除，装备删除）
    int getItemNumber(int index);// 通过物品编号获得对应物品的数量
    Equipment * getEquipOnTheBodyByPart(int part);
    void initBagUI(int nIndexBag = 0);
    void updateBag();
    void setEquipOnTheBody(cocos2d::gui::Widget * panel, int nPage); //0 属性页面 1 装备页面
    void setItemInBag(cocos2d::gui::Widget * panel, cocos2d::CCArray *arr);// 设置背包里的物品 装备
    void setItemInBagEx(cocos2d::gui::Widget * panel, cocos2d::CCArray *arr);// 设置背包里的物品 强化 锻造
    void bagPageSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);  // 页面切换
    void itemTypeSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 背包分类窗口切换（装备材料）
    void sortItem(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void bodyPartCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void bagCellCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 触碰背包内物品的按键回调
    void bagCellCallBackEx(CCObject* pSender, cocos2d::gui::TouchEventType type);// 触碰背包内物品的按键回调 强化 锻造 使用
    void reinforceCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 强化
    void reinforce100PercentCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 100%强化
    void composeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 合成
    void getAllMeterialCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 补全材料
    void updateEquipListUI();    //锻造完成 强化达到最大等级 更新装备列表
    void treasureButtonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);  // 宝物 刷新 和 获得
    void showPlayerAttributeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 显示人物属性
    void attributePageCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);  // 属性页面切换
    void returnBagInterfaceCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);  // 属性页面切换成背包界面
    void putOnEquip();
    void saleItemCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type);
    void putOnEquipCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type);
    void useProp(CCObject *pSender, cocos2d::gui::TouchEventType type);// 使用道具
    void updateAttributePage(); // 属性界面更新 
    void initBagCell(CCLayer * parent);  // 初始化背包格
    void updateItemIcon(cocos2d::gui::UIWidget * widget,const char *iconPath, int quality, cocos2d::CCPoint point = cocos2d::CCPointZero); // 更新物品图标
    void updateTreasureData();
    void updateReinforceInterface(cocos2d::CCArray * array);  // 设置强化属性
    void updateReinforceScrollCell();                         //更新左侧scrollView中cell
    void resetReinforceInterface();//重置强化界面
    void updateComposeInterface(cocos2d::CCArray * array);  // 设置合成属性
    void updateComposeScrollCell();                         //更新左侧scrollView中cell
    void resetComposeInterface();// 重置合成界面
    void updateBagUIPropertyInfo(); // 更新背包UI财产信息 
    void setRoleLabel(cocos2d::gui::UIPanel* panel,bool bArrOrBag); // 设置 角色信息 true 属性角色信息 false 背包角色信息
    void processEquipComposeSucceed(); // 处理合成成功特效 
    void initTreasureContainer();  // 宝箱初始
    void initPlayerAttribute(); // 属性初始 
    void setTreasureData(cocos2d::CCArray * array);
    void showTreasureInterface(bool isRefresh); // 宝箱两种状态 (是否有刷新和无刷新)
    void updateBagAllEquipArray();                 // 更新所有装备数组列表
    void exitBagUI(); // 关闭背包界面
    void drawRoleUI();
    void showReinforceFailedTips(int tipsType);
    void showComposeFailedTips(int tipsType);
    void showPropAttribute(Property* Property, bool bShowBtn = true);/*设置物品属性 */
    void hideItemInfo(CCObject *pSender, cocos2d::gui::TouchEventType type);
    void reinforceConsumeJewelLogic();
    void getMeterialConsumeJewlLogic();
    /**
     * 设置正在或未在读取宝箱数据
     */
    inline void setReadingTreasureData(bool isReading)
    {
        m_bReadingTreasureData = isReading;
    }
    /**
     * 设置已打开或未打开宝箱
     */
    inline void setTreasureOpened(bool isOpened)
    {
        m_bTreasureOpened = isOpened;
    }
    void treasureCellCallBack(CCObject *object, TouchEventType type);//宝箱内单元格触控回调
    void addTreasureOpeningAction();//添加开启宝箱动画
    void closeTreasureContainer();//关闭宝箱
    void showTreasureErrorTip(std::string& message);
    
    void updateItemDetailInfo(cocos2d::CCArray * array);       //更新物品详细信息
    void updateEquipDetailInfo(cocos2d::CCArray * array);      //更细装备详细信息
    //强化
    cocos2d::CCArray* m_pArrMeterialItem;   //材料详情信息
    void reinForceEquipCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type);
    void reinForceMeterialCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type);
   
    //锻造
    CCString* m_pStrAfterComposeID;       //锻造后装备ID;
    void composeEquipCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type);
    void composeAfterEquipCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void composeMeterialCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type);
    
#pragma mark vip
  
    void initVipUI();   // 初始化vipUI
    void vipCloseCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 关闭
    void vipPageUpCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 上一页
    void vipPageDownCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 上一页
    void vipChargeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 充值
    void updateVipUI();
    void updateVipDescription();
    
#pragma mark 技能
    
    void initSkillUI();   // 初始化技能
    void setGestureSkillIcon(cocos2d::gui::Widget * widget,int gestureType);
    void setShortcutsUIInfo(cocos2d::gui::Widget * widget);
    void setSkillIcon(cocos2d::gui::Widget * widget,cocos2d::CCArray * skillDataArray, bool isDefault = true);// 设置顶部技能icon 技能设置 技能升级
    void setGesturePageViewIcon(cocos2d::gui::Widget * widget,cocos2d::CCArray * skillDataArray, bool isDefault = true); // 设置顶部怒技icon 怒技升级
    void skillLevelUp(CCObject* pSender, cocos2d::gui::TouchEventType type); // 升级技能按键回调 
    void skillIconCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 设置技能快捷键
    void angrySkillIconCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); // 设置手势技能快捷键
    void skillPageButtonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 技能UI页面切换按键
    void switchGestureSkillCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 切换不同的手势技能
    void updateSkillDescription();
    void updateSkillUIPropertyInfo(cocos2d::gui::Widget * panel);
    void excludeDifferentTypeIcon(int gestureType, bool isEnable);
    void skillUpgradeConsumeJewelLogic();
#pragma mark 天赋
    
    void initTalentUI();   // 初始化天赋
    void updateTalentUI();   // 更新天赋数据
    void setTalentInfo(cocos2d::gui::Widget *widget);    //设置天赋信息 图标、名称、投入点数
    void skillSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);// 点击技能图标触发（切换不同技能） 
    bool isTalentPointsAddable(SkillData * skillData , TalentData *talentData);// 是否能加点 
    void updatePlayerTalentPointLabel();
    void hideTalentFrame();
    void showDescriptionCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void talentAddPointCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void hideTalentDescription(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void resetCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);//重置按钮
    
    //-------------shop---------------
    void initShopUI();  //初始化商城 
    void menuShopCloseCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);     //商城关闭按钮 
    void menuShopChangeType(CCObject* pSender, cocos2d::gui::TouchEventType type);        //商城选择要购买的商品类型 
    void menuShopBuy(CCObject* pSender, cocos2d::gui::TouchEventType type);               //商城买物品 
    void menuShopOpenCharge(CCObject* pSender, cocos2d::gui::TouchEventType type);        //商城打开充值页面 
    void refreshShopItemList(ShopPageType eType);                                              //展示商品列表内容 
    void menuShopItemShowDetail(CCObject* pSender, cocos2d::gui::TouchEventType type);    //显示商品详细信息 
    void menuShopItemAddNum(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuShopItemDescreaseNum(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuShopItemMaxNum(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuShopItemUpdatePriceNum();                                                    //更新宝石或金币花费数值
    void addBuyNum(int num, bool bIsToMax=false);
    void menuShopItemBuyOK(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuShopItemBuyCanel(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuMoneyInsufficiencyCanel(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void gotoCharge();
    void menuShopItemShowFriendList(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void shopBuyLogic();
    void shopBuy(int index, int num, bool bUseJew=false);//买物品
    void updateShopUIPropertyInfo();// 更新商店界面财产信息
    void drawShopBuyUI(int nIndex);
    void drawShopContent();
    void setShopDataInfo(CCArray * pArray);
    void exitShop();
    void goShopUI(ShopPageType kTypePage);
    void reSetShopDataInfo();
    //---------------------------充值-----------------------
    void initChargeUI();
    void updateChargeUIPropertyInfo();
    void refreshChargeItemList(ChargePageType kType);
    void refreshChargeBtnTypeUI(ChargePageType kType);
    void exitCharge();
    void menuChargeUIClosed(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuChargePageType(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuChargeCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuConsumeRMBOkCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuConsumeRMBCanelCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void setChargeDataInfo(cocos2d::CCArray *pArray);
    void setChargeSerialNumberInfo(cocos2d::CCArray *pArray);
    void chargeAgency();
    void HaseePaychargeAgency();
    void returnPayInfoTip(int result);
    void sendMsgForSerialNumberInfo();
    void goChargeUI(ChargePageType kTypePage);
    void initHaseePayUI();
    void refreshHaseePayPage(HaseePayPageType kType);
    void refreshSelectedQuota(HaseePayPageType kType, int nIndex);
    void menuHaseePayPageClosed(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuHaseePayPageType(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuHaseePaySelectedQuota(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuHaseePayPageCharged(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void chargeMobileMessagePay();
    //-------------------占灵--------------------
    void initProtossBaseUI();
    void initProtossEquipUI();
    void initProtossExchangeUI();
    void drawProtossSwallowUI(int nProtossPosID);
    void initExchangeListUI();
    void drawPieceInfo(int nIndex);
    void initProtossExchangeList();
    void initProtossMoenyList();
    void dragEquipBagIconCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuShowProtossEquipUI(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuShowProtossExchangeUI(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossOnekeyDevine(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossOnekeyReceive(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossOnekeySell(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossDevine(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossDevineShowFunc(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossJewOverOrder(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void protossBaseUIClose();
    void protossEquipUIClose();
    void protossExchangeUIClose();
    void menuProtossBaseUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuShowProtossBaseUI(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossOnekeyMerge(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossEquipUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossShowExchangeDetail(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossExchange(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossExchangeUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossSwallowUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossSwallow(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossSelected(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuProtossSelectedAll(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void updateProtossUIPropertyInfo();
    void updateProtossBaseUI(int nRefreshFlag=11);
    void updateProtossEquipUI(int nRefreshFlag=11);
    void updateProtossEquipBagUI();
    void updateProtossExchangeUI();
    void updateProtossSwallowUI(int nProtossPosID);
    void tipRemoveEquipFuncCallback(CCObject *pSender ,cocos2d::gui::TouchEventType type);
    void tipRemoveCallback(CCObject *pSender ,cocos2d::gui::TouchEventType type);
    void showProtossDescription(std::string desc, cocos2d::CCArray* arrLabel);

    void showProtossInfoForBaseUI(int nState, int nIndex, cocos2d::gui::SEL_TouchEvent btn_func);
    void showProtossInfoForEquipBagUI(int nIndex, cocos2d::gui::SEL_TouchEvent btn_func);
    void receiveOrSellCallback(CCObject *sender ,cocos2d::gui::TouchEventType type);
    void protossInfoForEquipBagUI(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void touchGroupCallBack();
    void setProtossMoney(int nIndex, int nGold, int nJew);
    void updateProtossAction();
    void initProtossAction();
    void lightConstellationConsumeLogic();
    YYProtossData* phagocyticUpgradeProtoss(int nProtossPosID);
    void initProtossSwallowInfo(bool bVisible);
    int getEmptyEquipIndex();
    bool isEquipCommAttributeProtoss(int nProtossPosID);
    void swallowProtoss();
    
    void setProtossWaitRecvData(cocos2d::CCArray *array);
    void setProtossWaitRecvOneData(cocos2d::CCArray *array);
    void setProtossBagData(cocos2d::CCArray *array);
    void setProtossBagOneData(cocos2d::CCArray *array);
    void setProtossEquipData(cocos2d::CCArray *array);
    void setProtossPieceData(cocos2d::CCArray *array);
    //----------------连续登录奖励---------------
    void initLoginAwardUI();
    void setLoginAwardData(cocos2d::CCArray *array);
    void menuGetLoginAward(CCObject* pSender, ::cocos2d::gui::TouchEventType type);
    bool isGetLoginAward();
    void updateLoginAwardUI(int index);
    
    //------------------登录签到-------------------
    void initSignUpUI();
    void setSignUpDateInfo(cocos2d::CCArray *array); // 签到信息的初始化
    void setSignUpFinishDataInfo(cocos2d::CCArray *array);   // 签到完成后数据
    void setSignUpBoxAwardDataInfo(cocos2d::CCArray *array);       // 累计签到获得奖励信息
    void updateSignUpUI();
    void drawSignUpFinishUI();  // 更新签到成功
    void drawSignUpAddUI();   // 更新补签
    void drawSignUpBoxAwardUI(); // 更新累计签到领取奖励;
    void drawSignUpAddCardInsufficiencyUI(); // 补签卡不足
    void transfSignUpUI();
    void menuSignUp(CCObject* pSender, ::cocos2d::gui::TouchEventType type);
    void menuSignUpAdd(CCObject* pSender, ::cocos2d::gui::TouchEventType type);
    void menuSignUpBoxAward(CCObject* pSender, ::cocos2d::gui::TouchEventType type);
    void menuSignUpGoShop(CCObject* pSender, ::cocos2d::gui::TouchEventType type);
    void exitSignUp();
    void closeAwardUI(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void removeLastObject(cocos2d::CCArray *arr);
    void setHide(cocos2d::gui::Widget *widget);
    int getMaxMonthDate(int year, int month);
    
    //-----------------活跃度---------------
    void initActivityUI();
    void updateActivityUI(cocos2d::CCArray* array);
    void updateActivityTaskUI();
    void updateActivityBoxUI();
    void drawActivityBoxAwardUI(int idx);
    void exitActivity();
    void menuFindDestination(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuUpdateTask(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuWishTreeBox(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuBoxAwardSure(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void callFounction(int nBtnId);
    void drawWishTreeHight(int nScore);

    //-----------------------成就---------------------------
    void initAchievementUI();
    void updateAchievementClassListUI();  // 更新分类列表 UI
    void updateAchievementTouchGetAward(); // 更新点击获取成就奖励之后UI状态
    void achievementClassSectionCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); //分类列表点击回调
    void openAchieventGroupList(); //默认打开左侧第一组分组列表
    void getAchievementUpdateIndex(int &nClassIdx, int &nGroupIdx); //传入id 返回index
    void updateAchievementItemUI(int nClassIdx, int nGroupIdx); //更新右侧pageView成就项UI
    void updateAchievementGroupAwardUI(int nClassIdx, int nGroupIdx);
    void drawAchievementTipUI(int nID, std::string sName, int nState, std::string sTime, std::string sDesc, std::string sIcon, int nNum);
    void drawAchievementGetAwardPopUpUI(std::string sIcon, int nNum, cocos2d::gui::SEL_TouchEvent selector);
    void receiveAchievementDataInfo(cocos2d::CCArray *array);
    void setAchievementTipDataInfo(cocos2d::CCArray *array);  //弹出成就ID
    void setAchievementGetAwardDataInfo(cocos2d::CCArray *array);
    void exitAchievement();
    void achievementGroupSectionCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void achievementDetailGetGeneralAwardCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void achievementGroupAwardCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void achievementSureGetAwardCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void scrollViewIntervalChanged(int nIndex, int nIntervalSize, int num);// 添加元素后 滑动位置间距改变
    void updateAchievementShowTip(float f);
    void resetAchievementShowTip();
    void achievementPageViewEvent(CCObject *pSender, PageViewEventType type);
    //--------------------------邮箱------------------------
    void initMailUI(bool bInBoxOrGM); // bInBoxOrGM true 收件箱 false 联系客服
    void initMailGMUI();  //初始化联系GM页面
    void updateMailUI();
    void updateMailDetailInfo(int flag);
    CCNode* getMailTipNode(uint16_t nMailNum);
    void menuMailUIClosed(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuRemoveMail(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuGetAccessory(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuAccessMail(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void setMailListData(cocos2d::CCArray *pArray);
    void setMailDetailData(cocos2d::CCArray *pArray);
    void setMailNum(uint16_t nMailNum);
    uint16_t getMailNum();
    void btnMailInBoxCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void btnMailSendGMCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void btnMailClickTitleCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void btnMailClickContentCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void btnMailClickSendCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    //TextFieldTTF 回调
    //当用户启动虚拟键盘时的回调函数
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF *pSender);
    //当用户关闭虚拟键盘时的回调函数
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF *pSender);
    //当用户进行输入时的回调函数
    virtual bool onTextFieldInsertText(CCTextFieldTTF *pSender,const char *text,int nLen);
    //当用户删除文字时的回调函数
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF *pSender,const char *delText,int nLen);

    //--------------------------设置------------------------
    void initSettingUI();
    void settingQuitCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);  //退出游戏回调
    void settingCloseCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); //关闭页面
    void settingBtnCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); //radio页面
    void settingGMCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type); //联系客服
    void settingChangeRole(CCObject* pSender, cocos2d::gui::TouchEventType type); //切换角色
    void settingRadioBoxVisible(int idx, bool isVisible);
    
    //--------------------------排行榜-----------------------
    void initRankUI();
    void updateRankDataInfo();
    void updateRankUI(cocos2d::CCDictionary* dict, int nIdx);
    void exitRank();
    void rankPageSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void rankPageArrowCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
#pragma mark 活动
    
    // 一级页面
    void initPlayTableUI();
    void updatePlayTableUI();
    void updatePlayTableDescription();
    void updatePlayTableAH(cocos2d::CCArray * arr);
    void updatePlayTableAI(cocos2d::CCArray * arr);
    void updatePlayTable();
    void playTableItemSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableSecondPageCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableCloseCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void switchPlayTablePageUI();
    void exitPlayTable();
    
    // 二级页面 转盘
    void initPalyTableTurnTable();
    void updatePlayTableTT(CCArray * arr);
    void updatePlayTableTR(CCArray * arr);
    void updateRoateTurnTable();
    void PlayTableTurnTableCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableTurnTableResetData();
    void playTableTurnTableIconMove();
    void playTableTurnTableIconEffect();
    void playTableTurnTableFinisned();
    
    // 二级页面 骰子
    void initPlayTableDice();
    void updatePlayTableTD(CCArray * arr);
    void updatePlayTableDR(CCArray * arr);
    void playTableDiceBtnCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableDiceIntroduceCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableDiceIntroduceConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableDiceRoateDiceUI();
    void playTableDiceIconMove();
    void playTableDiceFinish();
    
    // 二级页面 地下城
    void initPlayTableDungeonUI();
    void updateDungeonDO(CCArray * arr);
    void updateDungeonDA(CCArray * arr);
    void setPlayTableDungeonImageViewState(UIImageView* imageView, int state); 
    void playTableDungeonSelectCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableDungeonChallengeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableDungeonResetCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    // 二级页面 守卫星灵
    void initPlayTableProtectProtossUI();
    void updateProtectProtossTO(CCArray * arr);
    void updateProtectProtossTW(CCArray * arr);
    void playTableProtectProtossSelectCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableProtectProtossChallengeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    // 二级页面 boss副本
    void initPlayTableBossDungeonUI();
    void updatePlayTableBossDungeonUI();
    void updatePlayTableBossDungeonCurPageUI();
    void playTableBossDungeonPageViewEvent(CCObject *pSender, PageViewEventType type);
    void playTableBossDungeonBuyCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableBossDungeonChallengeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableBossDungeonWipeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableBossDungeonIntoDungeonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableBossDungeonCloseDungeonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableBossDungeonCloseDungeonSetPopTouch(bool bTouch);
    void playTableBossDungeonUpdatePopUI(bool bVisible);
    void updateplayTableBossDungeonBL(CCArray * arr);
    void updateplayTableBossDungeonBC(CCArray * arr);
    void updateplayTableBossDungeonBA(CCArray * arr);
    
    // 二级页面 世界boss
    void initPlayTableWorldBossUI();
    void updatePlayTableWorldBossUI();
    void playTableWorldBossPrayCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableWorldBossEnterCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void updateplayTableWorldBossWC(CCArray * arr);
    void updateplayTableWorldBossWT(CCArray * arr);
    void updateplayTableWorldBossWB(CCArray * arr);
    void updateplayTableWorldBossWK(CCArray * arr);
    void updateplayTableWorldBossWD(CCArray * arr);
    void playTableWorldBossResetData();
    void updateWorldBossData();
    void playTableWorldBosssSetDeleteTime(int deleteTime);
    
    // 活动 战斗结果公共页面
    void setPlayTableFightResult(CCArray* arr, PlayTableFightResult kType);
    void PlayTableFightResultConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void playTableFightResultPlayEffectCallBack(CCNode* pSender);
    void playTableFightResultFinishAnimationCallBack();
    void setPlayTableWipeFightResult(CCArray* arr, int nType); // 0 boss副本
    void PlayTableWipeFightResultConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
   
    
    
     //----------------------------试炼------------------------
    void initTrainUI();
    void trainDoneCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void updateTrainUI(CCArray* array);
    void updateTrainSelPlayerCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    void menuFinishCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuTaskInfoCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuTaskInfoCallback2(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void menuAcceptCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void updateTaskInfo(int key);
    void showTaskInfoInDialogInterface(int taskID);//在NPC对话界面显示任务信息
    void exitTaskUI();
    void initTrialResult(bool bSuccess);
    void trainConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
    // 聊天
    void initChattingUI(); // 初始化聊天界面
    void drawChatting();  // 渲染聊天界面聊天信息 
    void drawChattingBox(); // 渲染左下角聊天框信息 
    void drawChattingNotice(); // 渲染公告信息 
    void updateNoticeCall(CCNode* pSender,void* i);  // 公告消息的回调 
    void dispatchChatMessage(cocos2d::CCArray *array); // 分配服务器所发聊天信息到各个模板上
    void menuChatSceneTypeCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); // 聊天场景的选择
    void menuChatMessageCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); // 处理聊天消息的发送
    void menuChatFaceCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);  // 表情的选择
    void btnChatFaceCallback(CCObject* pSender, cocos2d::gui::TouchEventType type);  // 表情按钮
    std::string parseChattingMaterialContent(std::string content,ccColor3B &color1, ccColor3B &color2);  //解析聊天材料内容 用于颜色更改
    void exitChatting();
    void chatSceneBtnBg();
    cocos2d::gui::TouchGroup *m_pUITouchFacePop;  // 表情弹出界面

    
    //-----------任务-----------
    inline void setWaitingForFindTarget(bool isWaitingForTarget)
    {
        m_bWaitingForFindTarget = isWaitingForTarget;
    }
    inline bool isWaitingForFindTarget() const
    {
        return m_bWaitingForFindTarget;
    }
    inline void setWaitingForUpdateTask(bool isWaitingForUpdateTask)
    {
        m_bWaitingForUpdateTask = isWaitingForUpdateTask;
    }
    inline bool isWaitingForUpdateTask() const
    {
        return m_bWaitingForUpdateTask;
    }
    void initTaskUI();
    void updateTaskUI();
    void playTaskOverEffect(); // 处理完成特效 
    void updateTaskProcess(int key); // 处理任务过程 
    void updateTaskResult(int key); // 处理任务结束 
    void popUpDialogInterface(YYNPC  * taskNpc);//弹出与NPC的对话界面
    void taskCloseTipsCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); // 点击空白 关闭提示信息
    void updateTaskTipsUI(bool bShow, int nItem = 0);
    
    //-----------翻牌奖励-----------
    void showFlopAward(); // 奖励翻牌界面 
    void menuLockCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); // 点击是否 锁住(正反面切换) 获得此牌 
    void menuExcludeCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); // 去除两个不想要的奖励 
    void menuRandomCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); // 随机动画 
    void menuGoCallback(CCObject* pSender, cocos2d::gui::TouchEventType type); // 切换下一个界面
    void updateFlopCall(CCNode* pSender); // 翻转一半进行处理
    void updateCombineCall(CCNode* pSender); // 翻转结束聚合
    void updateShuffleCall(CCNode* pSender); // 洗牌 
    void updateZorderCall(CCNode* pSender,void* i);  // 洗牌从下往上覆盖 
    void updateOverCall(CCNode* pSender,void* i);  // 结束系列动作判断 
    void changeFlopCount();
    void changeReceiveCount();
    void updateAwardUI();
    void processJewelAward(cocos2d::CCArray* array); // 处理消耗钻石所开启的奖励牌 
    void setTextColorByQuality(cocos2d::gui::UILabel* label,int quality);  // 按照品质更换对应的颜色
    void updateAwardUIPropertyInfo(); // 设置 奖励界面金钱钻石
    void startShuffleCard();                // 接受服务器数据洗牌开始
    void getAwardForChoiceCard(cocos2d::CCArray* array); // 奖励信息
    void flopAwardConsumeJewelLogic();
    //--------------副本、关卡界面---------------
    void initDungeonUI(cocos2d::CCArray * array);// 设置副本界面元素
    void setChoosedEffect();
    void updateDungeonsUIElements();
    void updateDungeonsUI();
    void dungeonsUITouchHandle();
    
    void sendChoosedDungeonsIndexMessage();
    void setStageUIElement(cocos2d::CCArray * array);// 设置关卡界面元素
    void sendChoosedStageConfirmMessage();
    void sendWipeDungeonConfirmMessage(); // 发送扫荡副本信息
    void setStageAwardUI(cocos2d::CCArray * array);// 设置关卡奖励界面
    void initStageUI(cocos2d::CCArray * array);
    void stageCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    void exitStageCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type);
    void sendChoosedStageIndexMessage(int dungeonsIndex,int stageIndex);
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
    //---------------------关卡奖励界面-------------------------
    void initStageAwardUI(cocos2d::CCArray * array);
    void enterDungeonCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type);
    void difficultBtnCallBack(CCObject *pSender);
    void wipeDungeonCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type);
    
    void wipeDungeonCloseCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type);
    void wipeDungeonContinueCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type);
    
    //---------------------战斗评价-----------------------------
    void stageRating();
    void initFightEvaluationUI();
    void fightEvaluationSureCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type);
    void sendFightResultConfirmMessage();
    void initWipeDungeonAwardUI(); // 初始化扫荡页面奖励信息
    void closeWipeDungeonAwardUI(); // 关闭扫荡页面
    void updateWipeDungeonResult();
    
    //---------------------公告-----------------------------
    void initNoticeUI(CCArray* arr,bool isHaveData);
    void noticeClosedCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type);
    //---------------------兑换码----------------------------
    void initConvertCodeUI();
    void convertCodeConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    //---------------------用户中心--------------------------
    void initUserCenterUI();
    void updateUserCenterUI(CCArray* array);
    void UserCenterGMMailCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type);
    
private:
    
    void initItem(YYLayer * parent);
    void hideItemPopUpInfo();
    void showEquipAttributeInterface(Equipment* equipment,bool isEqupInBag);/*显示装备属性 */
    void setEquipAttribute(Equipment* equipment,cocos2d::gui::UIWidget *widget);
};

enum
{
    kDragRegionHead = 1, // 头盔
    kDragRegionBody = 2,// 铠甲
    kDragRegionHand = 3,// 护手
    kDragRegionShoes = 4,// 鞋
    kDragRegionHorse = 5,// 坐骑
    kDragRegionWeapon = 6,// 武器
    kDragRegionNecklace = 7,// 项链
    kDragRegionRing1 = 8,// 戒指1
    kDragRegionRing2 = 9,// 戒指2
    kDragRegionAccessorize = 10,// 饰品
    kDragRegionReinforce = 11,// 强化
    kDragRegionCompose= 12// 合成
};

enum
{
    kBagAttributeFightPage = 1,
    kBagAttributeSocietyPage
};

#endif /* defined(__FairyLand_II__YYPopUILayer__) */
