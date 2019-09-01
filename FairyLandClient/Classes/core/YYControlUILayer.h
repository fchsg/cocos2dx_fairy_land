//
//  YYControlUILayer.h
//  XJ
//  UI操控层 
//  Created by fuchenhao on 13-4-23. 
//
//

#ifndef __XJ__YYControlUILayer__
#define __XJ__YYControlUILayer__

#include <cocos2d.h>
#include "../common/GeometricRecognizer.h"
#include "cocos-ext.h"
using namespace DollarRecognizer;
USING_NS_CC;

class YYPlayer;
class YYLayer;
class YYButton;
class YYSprite;
class YYNPC;
class YYFightSprite;
class YYTrialPlayer;

// ————————————虚拟键——————————————
#define DIRECT_VIRTUAL_KEY_POSITION ccp(115,115)// 方向键圆心
#define DIRECT_AREA_RADIUS 88 // 方向键作用区域半径 
#define ROCKER_RADIUS 50 // 摇杆半径 
#define FIRE_RADIUS 72 // 攻击键半径 
#define SKILL_RADIUS 40 // 技能栏半径 
// ————————————虚拟方向键end—————————————— 

#define SKILL_SHORTCUT_KEY_AMOUNT 3// 技能快捷键总数 

#define TAG_CONFIRM 20
#define TAG_CANCEL 21

#define TAG_HELP 27 // 指引图示 
#define TAG_GESTURE_ANIMATION 1003 //判断手势动画是否结束

#define GAME_UI_RESOURCE_FILE_NAME  "ui/NewProject_20.json"

enum YYTouchEventType
{
    kTouchEventDefault = -1,
    kTouchEventRun = 0,// 摇杆
    kTouchEventFire = 1,// 攻击
    kTouchEventSkillShortCut0 = 2,// 技能快捷键0
    kTouchEventSkillShortCut1 = 3,// 技能快捷键1
    kTouchEventSkillShortCut2 = 4,// 技能快捷键2
    kTouchEventGestureSkill = 5, // 开始手势技
    kTouchEventHideUIMenu = 8,// 隐藏或显示按键
    kTouchEventTask = 9,// 任务
    kTouchEventTrain  = 10,// 试炼
    kTouchEventPlayTable = 11,//活动
    kTouchEventActivity = 12,//活跃度
    kTouchEventSignIn = 13,//签到
    kTouchEventLoginAward = 14,//登录奖励
    kTouchEventShop = 15,// 商城
    kTouchEventBag = 16,// 背包
    kTouchEventSkill = 17,// 技能
    kTouchEventTalent = 18,// 天赋
    kTouchEventProtoss = 19,// 占星
    kTouchEventRoleAttribute = 20,//属性
    kTouchEventSetting = 21,//设置
    kTouchEventAchievement = 22,//成就
    kTouchEventEmail = 23,//邮件
    kTouchEventChat = 24,// 聊天
    kTouchEventRolePortrait = 25,//点中角色头像
    kTouchEventCharge = 26,// 充值
    kTouchEventFindingWay = 27,// 寻路快捷键
    kTouchEventPageDown = 28,// 谈话内容翻页
    kTouchEventSetDestination = 29,// 设置目的地
    kTouchEventReturnCity = 30,// 回城(离开副本)
    kTouchEventExit = 31,//
    kTouchEventNotice = 32,// 公告
    kTouchEventConvertCode = 33,// 兑换码
    kTouchEventVip = 34 // vip
};

class YYControlUILayer : public cocos2d::CCLayer,public cocos2d::extension::CCEditBoxDelegate
{
    CC_SYNTHESIZE_READONLY(int, m_nComboCounter, ComboCounter);// 连击数 
    CC_SYNTHESIZE_READONLY(int, m_nComboInterval, ComboInterval);// 连击间隔 
    CC_SYNTHESIZE_READONLY(int, m_nComboIntervalCounter, ComboIntervalCounter);// 连击间隔计数 
    CC_SYNTHESIZE_READONLY(int, m_nKeyPressIntervalCounter, KeyPressIntervalCounter);// 按键间隔计数 
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pTouchDictionary, TouchDictionary);// 存储指令 
    CC_SYNTHESIZE(cocos2d::CCPoint,m_FireKeyPosition,FireKeyPosition);// 攻击键中心坐标 
    CC_SYNTHESIZE(cocos2d::CCPoint,m_UIHideButtonPosition,UIHideButtonPosition);// UI隐藏按键中心坐标 
    CC_SYNTHESIZE(YYPlayer *, m_pRoleByControlled, RoleByControlled);// 当前控制角色
    CC_SYNTHESIZE(YYTrialPlayer *, m_pRoleByTrial, RoleByTrial);// 当前试炼角色
    CC_SYNTHESIZE(YYFightSprite *, m_pBoss, Boss);// 当前场景Boss 
    CC_SYNTHESIZE(float,m_fEnegyPercentage,EnegyPercentage);// 能量进度
    CC_SYNTHESIZE(int8_t, m_n8TouchEventType, TouchEventType);// 触控事件类型
    CC_SYNTHESIZE(cocos2d::CCLabelTTF * , m_pLevelLabel, LevelLabel);// 等级标签
    CC_SYNTHESIZE(bool ,m_bIsShowTaskInfo,IsShowTaskInfo);// 是否显示 副本任务信息 
    CC_SYNTHESIZE(YYButton* ,m_pGuideButton,GuideButton);// 显示 右上角任务NPC信息
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pSpecialEffectArray,SpecialEffectArray);// 特效处理 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pTalkArray, TalkArray);// 对话数组 
    CC_SYNTHESIZE(cocos2d::extension::CCScale9Sprite *, m_pChattingBox, ChattingBox); // 左下角显示聊天框 
    CC_SYNTHESIZE(cocos2d::extension::CCScrollView *, m_pChatBoxScrollView, ChatBoxScrollView); // 左下角显示聊天区域 
    CC_SYNTHESIZE(cocos2d::CCLayer *, m_pNoticeLayer, NoticeLayer); // 屏幕上方公告层
    CC_SYNTHESIZE(int32_t, m_nHelpDuration, HelpDuration);// 帮助界面显示时间
    CC_SYNTHESIZE(YYNPC *, m_pChoosedNPC, ChoosedNPC);
    CC_SYNTHESIZE(bool, m_bWorldBossEnableDead, WorldBossEnableDead); // 活动 世界boss 玩家是否可以打死boss
    CC_SYNTHESIZE(float, m_fWorldBossRemainHP, WorldBossRemainHP); // 活动 世界boss 记录Boss剩余血量百分比
    CC_SYNTHESIZE(uint32_t, m_nWorldBossAmountHP, WorldBossAmountHP); // 活动 世界boss 记录Boss拥有的总血量
   
private:
    
    bool m_bTalk;// 对话,翻页
    bool m_bContinueAttack;// 是否持续攻击 
    bool m_bHoldDirectionKey;// 方向键被按下 
    bool m_bHoldAttackKey;// 攻击键被持续按下 
    bool m_bUIButtonMoving;//UI按键是否正在移动中
    cocos2d::CCPoint m_SkillShortcutKeyPostion[SKILL_SHORTCUT_KEY_AMOUNT];// 技能快捷图标位置 
    
    cocos2d::CCPoint m_MoveTouchPoint;// 计算人物移动
    cocos2d::CCSprite* m_pVirtualRockerFrame;// 摇杆框
    cocos2d::CCSprite* m_pVirtualRocker;// 虚拟摇杆
    cocos2d::CCSprite* m_pVirtualFireKey;// 攻击键

    // ------------------手势--------------------
    cocos2d::CCPoint m_StartPoint,m_TouchPoint,m_PreTouchPoint;// 起始触摸点/当前触摸点/上一触摸点
    
    
    GeometricRecognizer * m_pGemertricRecognizer;
    Path2D p_2dPath;// 手势轨迹
    //手势轨迹渲染相关对象
    cocos2d::CCRenderTexture *m_pTarget;
	cocos2d::CCSprite *m_pBrush;
    
    // 连击标签 
    cocos2d::CCSprite * m_pComboLabel;
    cocos2d::CCLabelAtlas * m_pComboNumber;
    
    // 挂机
    bool m_bHang;
    cocos2d::CCSprite *m_pSpriteHang;
    cocos2d::CCSprite *m_pPortraitBGFrame;// 头像背景框
    cocos2d::CCSprite *m_pVipBG; // vip背景框
    cocos2d::CCSprite *m_pPlayerPortrait;// 头像
    cocos2d::CCSprite *m_pPlayerStatusBar;// 玩家状态栏 
    cocos2d::CCSprite *m_pPlayerAngerStatusBar;// 玩家怒气栏 
    cocos2d::CCSprite *m_pPlayerJewel;// 玩家宝石 
    cocos2d::CCSprite *m_pPlayerMoney;// 玩家金币
    cocos2d::CCLabelTTF *m_pMoneyLabel;//
    cocos2d::CCLabelTTF *m_pJewelLabel;//
    cocos2d::CCSprite *m_pPlayerPhysicalStrengthPoint;// 玩家体力
    cocos2d::CCSprite *m_pPlayerFighttingPoint;// 玩家战斗力
    cocos2d::CCProgressTimer *m_pPlayerBloodBar;// 玩家血条 
    cocos2d::CCProgressTimer *m_pPlayerManaBar;//玩家法力条 
    cocos2d::CCProgressTimer *m_pEnergyBar;// 能量条 
    cocos2d::CCProgressTimer *m_pExperienceBar;// 经验条 
    cocos2d::CCSprite *m_pBossPortraitBGFrame;// boss头像背景框 
    cocos2d::CCSprite *m_pBossPortrait;// boss头像 
    cocos2d::CCSprite *m_pBossStatusBar;// boss状态栏 
    cocos2d::CCProgressTimer *m_pBossBloodBar;// boss血条 
    cocos2d::CCSprite *m_pBossBGBloodBar;// 被当前层血条叠压的血条 
    cocos2d::CCSprite *m_pLevelLabelFrame; // 等级标签框 
    cocos2d::CCSprite *m_pLineMark;// 直线手势技能标记 
    cocos2d::CCSprite *m_pCircleMark;// 圆手势技能标记 
    int m_nPlayerHP,m_nPlayerMP,m_nPlayerExperience;// 判断血、魔、怒气值是否有变化 
    int m_nBossID,m_nBossHP;// boss血值变化 
    int8_t m_n8BossCurrentHPLayerNum;// 记录boss当前血条层数
    cocos2d::CCProgressFromTo * m_pActionCooling;
    bool m_bComboCountChange;// 连击计数改变 
    bool m_bHideUIButton;
    cocos2d::CCLabelTTF * m_pHPLabel;// 血量 
    cocos2d::CCLabelTTF * m_pMPLabel;// 魔法量 
    cocos2d::CCLabelTTF * m_pTaskNumberLabel;// 任务数 
    cocos2d::CCProgressTimer *m_pCDEffect[3];
    cocos2d::CCSprite *m_pSkillIconBGFrame[3];
    cocos2d::CCSprite *m_pSkillIcon[3];
    cocos2d::extension::CCScrollView *m_pScrollView;
    CCLayer *m_pTaskShowLayer;
    YYSprite *m_pAutoFindTips;         // 自动寻路精灵
    cocos2d::CCArray* m_pUIButtonArray;
    
    int m_nCurrentTime;//当前用时 
    cocos2d::CCLabelAtlas *m_pTimeLabelAtlas;// 副本用时
    
    //试炼
    cocos2d::CCSprite *m_pTrialPortraitBGFrame;// 头像背景框
    cocos2d::CCSprite *m_pTrialPlayerPortrait;// 头像
    cocos2d::CCSprite *m_pTrialPlayerStatusBar;// 玩家状态栏
    cocos2d::CCSprite *m_pTrialPlayerAngerStatusBar;// 玩家怒气栏
    cocos2d::CCSprite *m_pTrialPlayerJewel;// 玩家宝石
    cocos2d::CCSprite *m_pTrialPlayerMoney;// 玩家金币
    cocos2d::CCSprite *m_pTrialPlayerPhysicalStrengthPoint;// 玩家体力
    cocos2d::CCSprite *m_pTrialPlayerFighttingPoint;// 玩家战斗力
    cocos2d::CCProgressTimer *m_pTrialPlayerBloodBar;// 玩家血条
    cocos2d::CCProgressTimer *m_pTrialPlayerManaBar;//玩家法力条
    cocos2d::CCProgressTimer *m_pTrialEnergyBar;// 能量条
    cocos2d::CCProgressTimer *m_pTrialExperienceBar;// 经验条
    cocos2d::CCLabelTTF * m_pTrialHPLabel;// 血量
    cocos2d::CCLabelTTF * m_pTrialMPLabel;// 魔法量
    CC_SYNTHESIZE(cocos2d::CCLabelTTF * , m_pTrialLevelLabel, TrialLevelLabel);// 等级标签
    cocos2d::CCSprite   * m_pTrialLevelLabelFrame; // 等级标签框
    int m_nTrialHP,m_nTrialMP,m_nTrialExperience;// 判断血、魔、怒气值是否有变化
    CC_SYNTHESIZE(bool ,m_bPlayTrialCounter,PlayTrialCounter); //是否播放试炼FightUI
    CC_SYNTHESIZE(bool ,m_bPlayTrialTime,PlayTrialTime); //是否播放试炼时钟UI

    
    // 场景按钮
    cocos2d::CCSprite *m_pSpriteTrain;
    cocos2d::CCSprite *m_pSpritePlayTable;
    cocos2d::CCSprite *m_pSpriteTask;
    cocos2d::CCSprite *m_pSpriteShop;
    cocos2d::CCSprite *m_pSpriteLoginAward;
    cocos2d::CCSprite *m_pSpriteSignUp;
    cocos2d::CCSprite *m_pSpriteActivity;
    cocos2d::CCSprite *m_pSpriteCharge;
    cocos2d::CCSprite *m_pSpriteChat;
    cocos2d::CCSprite *m_pSpriteNotice;
    cocos2d::CCSprite *m_pSpriteConvertCode;
    // ------------- function --------------
    void touchHandle();
    void calcComboAvailable();
    
    // ------------- face --------------
    int m_nCurrentExp;
    cocos2d::CCSpriteBatchNode* m_pExpSpriteBatch;
    int m_nExpMaxWidth;
    
    cocos2d::CCArray* m_pLabelArray;
    
public:
    
    YYControlUILayer();
    ~YYControlUILayer();
    CREATE_FUNC(YYControlUILayer);
    
    void setVirtualKeyPosition(cocos2d::CCPoint touchedPoint);
    void returnInitPositon();
    void update(float f);
    void touchBegan(cocos2d::CCTouch * touch);
    void touchMoved(cocos2d::CCTouch * touch);
    void touchEnded(cocos2d::CCTouch * touch);
    
    void clearKeyState();
    void gestureEnded();//手势完结
    void calcCombo(int comboCount);
    void setSkillIcon();
    void setShortcutSkillIcon(cocos2d::CCArray *array);
    void cooling(int index,int time);
    void updatePlayerStatusBar();// 更新玩家状态栏 
    void updateBossStatusBar();// 更新boss状态栏 
    void updateDungeonsTime();// 更新副本时间
    void initElements();
    void updateCombo();
    void hideUIButton();
    void showUIButton();
    void showTaskInfo();  // 显示任务信息 
    void updateTaskGuide();// 更新右上角任务指引信息
    void updateGuideButton(int taskIndex);
    void setUIVisible(bool isVisible);
    void say(const char * name,const char * content,const char * picFileName);// 对话 说话人的名字，内容，图片 
    void updateTalkContent();// 更新对话内容（切换下两行文字） 
    void updateRoleWealth(); // 更新角色财富
    void showTheWay();// 指路 
    void showLabelStampEffect(const char * str,cocos2d::CCPoint startPosition,int fontSize,bool isClean);// 盖章标记效果 
    void updateNextLabelEffect(CCNode* pSender,void* i); // 
    
    void updateExpBar(int currentPoint,int maxPoint,cocos2d::CCPoint position);
    
    void commonUI();

    inline void setComboCountChange(bool isChange)
    {
        m_bComboCountChange = isChange;
    }
    inline void setHideUIButton(bool isHideUIButton)
    {
        m_bHideUIButton = isHideUIButton;
    }
    inline bool isHideUIButton()
    {
        return m_bHideUIButton;
    }
    
    inline bool isHoldAttackKey()const
    {
        return m_bHoldAttackKey;
    }
    
    inline void setHoldAttackKey(bool isHoldAttackKey)
    {
        m_bHoldAttackKey = isHoldAttackKey;
    }
    
    inline cocos2d::CCPoint getCurrentTouchPoint()const
    {
        return m_MoveTouchPoint;
    }
    inline cocos2d::CCPoint getSkillShortcutKeyPosition(int index)
    {// 技能快捷键位置 
        return m_SkillShortcutKeyPostion[index];
    }
    inline void setSkillShortcutKeyPosition(int index,cocos2d::CCPoint position)
    {
        m_SkillShortcutKeyPostion[index] = position;
    }
    inline cocos2d::CCLabelTTF* getTaskNumberLabel()
    {
        return m_pTaskNumberLabel;
    }

    virtual void editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox);
    virtual void editBoxTextChanged(cocos2d::extension::CCEditBox *editBox,const std::string &text);
    virtual void editBoxReturn(cocos2d::extension::CCEditBox *editBox);
    
    void setMenuEnable(bool isEnable);
    
    //试炼
    void trialCommonUI();      // 初始化状态栏
    void updateTrialTime();    // 更新试炼时间
    void updateTrialFightTime(); //更新试炼fightUI动画
    void updateTrialStatusBar(); // 更新试炼状态栏
    void intiTrialCounterUI();  //倒计时开始
    void trialOver();           //试炼结束

public:
	CCPoint m_PlayerPortraitPoint;
};

#endif /* defined(__XJ__YYControlUILayer__) */
