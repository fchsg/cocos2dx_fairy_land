//
//  YYControlUILayer.cpp
//  XJ
//
//  Created by fuchenhao on 13-4-23.
//
//

#include "YYControlUILayer.h"
#include "../common/Common.h"
#include "YYController.h"
#include "YYScene.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYTrialPlayer.h"
#include "YYLayer.h"
#include "YYButton.h"
#include "YYDragRegion.h"
#include "YYSpriteAttribute.h"
#include "YYPopUpUI.h"
#include "YYNPC.h"
#include "YYTaskTargetData.h"
#include "YYTaskData.h"
#include "YYGuide.h"
#include "YYGlobalData.h"

// 子节点标签
#define TAG_UI_BUTTON 10
#define TAG_HIDE_BUTTON 40// 隐藏UI按键的标签
#define TAG_EXIT 41// 退出
#define TAG_RETURN_CITY_LABEL 42// 回城标签文字
#define TAG_RETURN_BUTTON 43// 回城按键1未按下
#define TAG_ENERGY_EFFECT 45
#define TAG_SHOW_TASK_BUTTON 47  // 副本任务标签
#define TAG_SHOW_TASK_ACTION 50
#define TAG_TALK_BG_FRAME 58 // 对话框
#define TAG_PORTRAIT 59 // 对话调用的人物头像
#define TAG_NAME_LABEL 60 // 说话人的名称
#define TAG_CONTENT_LABEL 61 // 对话内容
#define TAG_EXPERIENCE_ICON 64 // 经验条
#define TAG_EXPERIENCE_CUBE 65 // 经验条
#define TAG_EXPERIENCE_TEXT 66 // 经验数值
#define TAG_SHORTCUT_EFFECT 70// 寻路快捷键指示特效
#define TAG_FTINGHTTING 75 // 临时战斗力
#define TAG_LOGINAWARD_EFFECT   76  //连续登录奖励特效

#define SHOW_TASKINFO_WIDTH  300
#define SHOW_TASKINFO_HEIGHT  300

#define FINGER_MOVED_DISTANCE 100// 手指在屏幕上的移动距离
#define KEYPRESS_INTERVAL 15// 按键间隔

#define OBLIGATE_WIDTH 15 // 经验条预留宽度

#define PLAYER_PORTRAIT_BG_FRAME            getFilePath("80",kDirTypeUI,kFileSuffixPng).c_str() //头像背景框
#define LEVEL_LABEL_BG_FRAME                getFilePath("83",kDirTypeUI,kFileSuffixPng).c_str() // 等级标签背景框
#define GESTURE_LINE1                       getFilePath("113",kDirTypeUI,kFileSuffixPng) //手势标记直线未激活时
#define GESTURE_V1                          getFilePath("110",kDirTypeUI,kFileSuffixPng) // 手势标记V未激活时
#define GESTURE_CIRCLE1                     getFilePath("111",kDirTypeUI,kFileSuffixPng) // 手势标记圆未激活时
#define GESTURE_BG                          getFilePath("83-1",kDirTypeUI,kFileSuffixPng).c_str()// 手势背景
#define GESTURE_LINE2                       getFilePath("109",kDirTypeUI,kFileSuffixPng).c_str() // 手势标记直线激活时
#define GESTURE_V2                          getFilePath("106",kDirTypeUI,kFileSuffixPng).c_str()// 手势标记V激活时
#define GESTURE_CIRCLE2                     getFilePath("107",kDirTypeUI,kFileSuffixPng).c_str()// 手势标记圆激活时
#define GESTURE_A                           getFilePath("108",kDirTypeUI,kFileSuffixPng).c_str()
#define GESTURE_TRACK                       getFilePath("snow", kDirTypePic, kFileSuffixPng).c_str() //手势轨迹
#define PLAYER_STATUS_BAR                   getFilePath("373",kDirTypeUI,kFileSuffixPng).c_str()// 状态栏
#define PLAYER_BLOOD                        getFilePath("374",kDirTypeUI,kFileSuffixPng).c_str()// 血条
#define PLAYER_MANA                         getFilePath("375",kDirTypeUI,kFileSuffixPng).c_str()// 法力条
#define PLAYER_ENERGY_BAR                   getFilePath("382",kDirTypeUI,kFileSuffixPng).c_str()// 能量槽
#define PLAYER_ENERGY                       getFilePath("383",kDirTypeUI,kFileSuffixPng).c_str()// 能量条
#define EXPERIENCE_BG_1                     getFilePath("304", kDirTypeUI, kFileSuffixPng).c_str()    //黑条
#define EXPERIENCE_BG_2                     getFilePath("306", kDirTypeUI, kFileSuffixPng).c_str()    //经验条背景框
#define EXPERIENCE_PROGRESS                 getFilePath("305", kDirTypeUI, kFileSuffixPng).c_str()    //经验条

#define FINDING_WAY_TIPS                    getFilePath("se24", kDirTypeEffect, kFileSuffixPng).c_str()  //自动寻路提示
#define PLAYER2_ATTACK_ICON                 getFilePath("2", kDirTypeUI, kFileSuffixPng).c_str()//法师普通图标
#define PLAYER3_ATTACK_ICON                 getFilePath("0", kDirTypeUI, kFileSuffixPng).c_str()//战士普通图标

#define COMBO_LABEL                         getFilePath("combolabel", kDirTypePic, kFileSuffixPng).c_str()  //连击标签
#define COMBO_NUM                           getFilePath("combo", kDirTypePic, kFileSuffixPng).c_str()       //连击数字

#define BOSS_PORTRAIT_BG_FRAME              getFilePath("601",kDirTypeUI,kFileSuffixPng).c_str()   // BOSS头像背景图
#define BOSS_BLOOD_BAR                      getFilePath("363",kDirTypeUI,kFileSuffixPng).c_str() // BOSS血槽
#define BOSS_BLOOD                          getFilePath("bossbloodbar%d",kDirTypeUI,kFileSuffixPng).c_str() // BOSS血条

#define VIP_BG_FRAME                        getFilePath("82",kDirTypeUI,kFileSuffixPng).c_str()   // vip 背景图
#define VIP_LOGO                            getFilePath("379",kDirTypeUI,kFileSuffixPng).c_str()  // vip icon
#define VIP_NUM                             getFilePath("360",kDirTypePiczy,kFileSuffixPng).c_str()  // vip icon


// 离开副本
#define RETURN_CITY_NOMAL               getFilePath("605",kDirTypeUI,kFileSuffixPng).c_str()
#define RETURN_CITY_PRESS               getFilePath("605-1",kDirTypeUI,kFileSuffixPng).c_str()

//虚拟键
#define VIRTUAL_DIR_BG_NOMAL            getFilePath("158",kDirTypeUI,kFileSuffixPng).c_str()
#define VIRTUAL_DIR_BG_PRESS            getFilePath("158-1",kDirTypeUI,kFileSuffixPng).c_str()
#define VIRTUAL_ROCKER_NOMAL            getFilePath("162",kDirTypeUI,kFileSuffixPng).c_str()
#define VIRTUAL_ROCKER_PRESS            getFilePath("162-1",kDirTypeUI,kFileSuffixPng).c_str()

// 攻击快捷键
#define VIRTUAL_FIRE_BG_NOMAL           getFilePath("159",kDirTypeUI,kFileSuffixPng).c_str()
#define VIRTUAL_FIRE_BG_PRESS           getFilePath("159-1",kDirTypeUI,kFileSuffixPng).c_str()
#define VIRTUAL_SKILL_BG_NOMAL          getFilePath("163",kDirTypeUI,kFileSuffixPng).c_str()
#define VIRTUAL_SKILL_BG_PRESS          getFilePath("163-1",kDirTypeUI,kFileSuffixPng).c_str()

// 挂机
#define PLAY_HANG_NOMAL                 getFilePath("606",kDirTypeUI,kFileSuffixPng).c_str()
#define PLAY_HANG_PRESS                 getFilePath("606-1",kDirTypeUI,kFileSuffixPng).c_str()

#define PLAY_LEAVE_HANG_NOMAL           getFilePath("604",kDirTypeUI,kFileSuffixPng).c_str()
#define PLAY_LEAVE_HANG_PRESS           getFilePath("604-1",kDirTypeUI,kFileSuffixPng).c_str()

#define DIALOG_BOX                      getFilePath("177", kDirTypeUI, kFileSuffixPng).c_str()  // 对话框
#define PLAYER_PORTRAIT1                getFilePath("501-1", kDirTypeUI, kFileSuffixPng)  // 弓手半身像
#define PLAYER_PORTRAIT2                getFilePath("502-1", kDirTypeUI, kFileSuffixPng)  // 法师半身像
#define PLAYER_PORTRAIT3                getFilePath("500-1", kDirTypeUI, kFileSuffixPng)  // 战士半身像

#define JEWEL_ICON                      getFilePath("298", kDirTypeUI, kFileSuffixPng).c_str() // 宝石
#define GOLD_ICON                       getFilePath("300", kDirTypeUI, kFileSuffixPng).c_str() // 金币

#define HIDE_UI_BUTTON_ICON             getFilePath("92", kDirTypeUI, kFileSuffixPng).c_str()  //隐藏用户界面的按键图标
#define UI_VISBLE_BUTTON_ICON           getFilePath("93", kDirTypeUI, kFileSuffixPng).c_str()  //显示用户界面的按键图标

// 战力
#define MILITARY_POWER_BG1              getFilePath("191", kDirTypeUI, kFileSuffixPng).c_str()  //战力进度条背景框
#define MILITARY_POWER_BG2              getFilePath("191-2", kDirTypeUI, kFileSuffixPng).c_str()  //战力背景（火焰）
#define MILITARY_POWER_PROGRESS         getFilePath("191-3", kDirTypeUI, kFileSuffixPng).c_str()  //战力进度条
#define MILITARY_POWER_LABEL            getFilePath("191-4", kDirTypeUI, kFileSuffixPng).c_str()  //战力文字标签
#define MILITARY_POWER_NUM              getFilePath("500", kDirTypeUI, kFileSuffixPng).c_str()  //战力数字

// 任务
#define TASK_ICON1                      getFilePath("85", kDirTypeUI, kFileSuffixPng).c_str()
#define TASK_ICON2                      getFilePath("89", kDirTypeUI, kFileSuffixPng).c_str()

#define MALL_ICON                       getFilePath("283", kDirTypeUI, kFileSuffixPng).c_str()// 商城
#define LOGIN_AWARD_ICON                getFilePath("284", kDirTypeUI, kFileSuffixPng).c_str()// 奖励
#define SIGNUP_ICON                     getFilePath("354", kDirTypeUI, kFileSuffixPng).c_str()// 签到
#define ACTIVITY_ICON                   getFilePath("355", kDirTypeUI, kFileSuffixPng).c_str()// 活跃度
#define RECHARGE_ICON                   getFilePath("602", kDirTypeUI, kFileSuffixPng).c_str()// 充值
#define PHYSICAL_POWER_ICON             getFilePath("299", kDirTypeUI, kFileSuffixPng).c_str()// 体力
#define CHAT_BOX                        getFilePath("94", kDirTypeUI, kFileSuffixPng).c_str()// 聊天框
#define CHAT_BUTTON                     getFilePath("431", kDirTypeUI, kFileSuffixPng).c_str()// 聊天按键
#define NOTICE_ICON                     getFilePath("357", kDirTypeUI, kFileSuffixPng).c_str()// 公告
#define CONCODE_ICON                    getFilePath("358", kDirTypeUI, kFileSuffixPng).c_str()// 兑换码
#define PLAYTABLE_ICON                  getFilePath("356", kDirTypeUI, kFileSuffixPng).c_str()// 活动
#define TRIAL_ICON                      getFilePath("84", kDirTypeUI, kFileSuffixPng).c_str()// 试炼

// 试炼倒计时
#define UI_TRIAL_COUNTER 1
#define TRIAL_COUNTER_LAYER 51
#define TRIAL_COUNTER_SPRITE 101

static const uint8_t s_u8EnergyConsume[] ={30,60,90};
enum
{
    kGestureLine = 101,
    kGestureCircle,
    kGestureLineAction,
    kGestureCircleAction
};

USING_NS_CC;
using namespace gui;

YYControlUILayer::YYControlUILayer()
:m_pRoleByControlled(NULL)
,m_pVirtualFireKey(NULL)
,m_pVirtualRockerFrame(NULL)
,m_pVirtualRocker(NULL)
,m_pComboLabel(NULL)
,m_pComboNumber(NULL)
,m_pPlayerStatusBar(NULL)
,m_pPlayerBloodBar(NULL)
,m_pPlayerManaBar(NULL)
,m_pBossBloodBar(NULL)
,m_pBossStatusBar(NULL)
,m_pBossBGBloodBar(NULL)
,m_pBossPortrait(NULL)
,m_pBossPortraitBGFrame(NULL)
,m_pGemertricRecognizer(NULL)
,m_pLevelLabel(NULL)
,m_pTarget(NULL)
,m_pBrush(NULL)
,m_pTouchDictionary(NULL)
,m_pActionCooling(NULL)
,m_pHPLabel(NULL)
,m_pMPLabel(NULL)
,m_pTaskNumberLabel(NULL)
,m_pEnergyBar(NULL)
,m_pScrollView(NULL)
,m_pTaskShowLayer(NULL)
,m_pGuideButton(NULL)
,m_pSpecialEffectArray(NULL)
,m_pTalkArray(NULL)
,m_pExperienceBar(NULL)
,m_pUIButtonArray(NULL)
,m_pPlayerJewel(NULL)
,m_pPlayerMoney(NULL)
,m_pPlayerPhysicalStrengthPoint(NULL)
,m_pPlayerFighttingPoint(NULL)
,m_pExpSpriteBatch(NULL)
,m_pPlayerAngerStatusBar(NULL)
,m_pLabelArray(NULL)
,m_pChattingBox(NULL)
,m_pChatBoxScrollView(NULL)
,m_bComboCountChange(false)
,m_bHoldAttackKey(false)
,m_bHideUIButton(true)
,m_bContinueAttack(false)// 是否持续攻击
,m_bHoldDirectionKey(false)// 方向键被按下
,m_bIsShowTaskInfo(false)
,m_bUIButtonMoving(false)
,m_bTalk(false)
,m_nPlayerHP(-1)
,m_nPlayerMP(-1)
,m_nTrialHP(-1)
,m_nTrialMP(-1)
,m_nBossHP(-1)
,m_nBossID(-1)
,m_bWorldBossEnableDead(false)
,m_fWorldBossRemainHP(0.0f)
,m_nWorldBossAmountHP(0)
,m_nPlayerExperience(-1)
,m_n8TouchEventType(kTouchEventDefault)
,m_pBoss(NULL)
,m_pLineMark(NULL)// 手势标识（线）
,m_pCircleMark(NULL)
,m_nComboCounter(0)
,m_nComboInterval(0)
,m_nComboIntervalCounter(0)
,m_nKeyPressIntervalCounter(0)
,m_fEnegyPercentage(0)
,m_nCurrentTime(-1)
,m_nCurrentExp(0)
,m_nExpMaxWidth(0)
,m_nHelpDuration(0)
,m_pTimeLabelAtlas(NULL)
,m_pChoosedNPC(NULL)
,m_pTrialPortraitBGFrame(NULL)
,m_pTrialPlayerPortrait(NULL)
,m_pTrialPlayerStatusBar(NULL)
,m_pTrialPlayerAngerStatusBar(NULL)
,m_pTrialPlayerJewel(NULL)
,m_pTrialPlayerMoney(NULL)
,m_pTrialPlayerPhysicalStrengthPoint(NULL)
,m_pTrialPlayerFighttingPoint(NULL)
,m_pTrialPlayerBloodBar(NULL)
,m_pTrialPlayerManaBar(NULL)
,m_pTrialEnergyBar(NULL)
,m_pTrialExperienceBar(NULL)
,m_pTrialHPLabel(NULL)
,m_pTrialMPLabel(NULL)
,m_pTrialLevelLabel(NULL)
,m_pTrialLevelLabelFrame(NULL)
,m_bPlayTrialCounter(false)
,m_bPlayTrialTime(false)
{
    m_pTouchDictionary = new CCDictionary();// 用于映射玩家控制角色触控指令
    for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i++)
    {
        m_pCDEffect[i] = NULL;
        m_pSkillIconBGFrame[i] = NULL;
        m_pSkillIcon[i] = NULL;
    }
    m_pSpecialEffectArray = new CCArray(10);
    m_pTalkArray = new CCArray(10);
    m_pLabelArray = CCArray::createWithCapacity(10);
    m_pLabelArray->retain();
    intiTrialCounterUI();
    m_pGemertricRecognizer = new GeometricRecognizer;
    m_pGemertricRecognizer->loadTemplates();
}

YYControlUILayer::~YYControlUILayer()
{
    CC_SAFE_RELEASE(m_pUIButtonArray);
    CC_SAFE_DELETE(m_pTouchDictionary);
    CC_SAFE_DELETE(m_pGemertricRecognizer);
    CC_SAFE_RELEASE(m_pTarget);
    CC_SAFE_RELEASE(m_pBrush);
    CC_SAFE_DELETE(m_pSpecialEffectArray);
    CC_SAFE_DELETE(m_pTalkArray);
    CC_SAFE_RELEASE(m_pLabelArray);
}

void YYControlUILayer::commonUI()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // 头像背景
    m_pPortraitBGFrame = CCSprite::create(PLAYER_PORTRAIT_BG_FRAME);
    int width = m_pPortraitBGFrame->getContentSize().width;
    m_pPortraitBGFrame->setPosition(ccp(width / 2,winSize.height - m_pPortraitBGFrame->getContentSize().height / 2 - 10));
    addChild(m_pPortraitBGFrame,1);
    
    // 头像icon
    const char *picPath= CCString::createWithFormat("i-0-%d-0-0",m_pRoleByControlled->getSpriteData()->getSpriteIndex() - 1)->getCString();
    std::string filePath = getFilePath(picPath, kDirTypeIcon, kFileSuffixPng);
    m_pPlayerPortrait = CCSprite::create(filePath.c_str());
    m_pPlayerPortrait->setPosition(ccp(m_pPortraitBGFrame->getPosition().x - 9, m_pPortraitBGFrame->getPosition().y));
    addChild(m_pPlayerPortrait, 2);
	m_PlayerPortraitPoint = m_pPlayerPortrait->getPosition();
    
    // vip
    m_pVipBG = CCSprite::create(VIP_BG_FRAME);
    m_pVipBG->setPosition(ccp(m_pPortraitBGFrame->getPositionX() - 10,m_pPortraitBGFrame->getPositionY() - 60));
    addChild(m_pVipBG,3);
    int vipBGWid = m_pVipBG->getContentSize().width;
    int vipBGHei = m_pVipBG->getContentSize().height;
    
    // vip icon
    CCSprite* vipIcon = CCSprite::create(VIP_LOGO);
    vipIcon->setScale(1.4f);
    vipIcon->setAnchorPoint(ccp(0.0f, 0.5f));
    vipIcon->setPosition(ccp(12, vipBGHei / 2));
    m_pVipBG->addChild(vipIcon);
    
    // vip 等级
    std::string str = getFilePath("500",kDirTypePic,kFileSuffixPng);
    CCLabelAtlas* vipLevel = CCLabelAtlas::create("0",str.c_str(),30,43,'0');
    vipLevel->setScale(0.6f);
    vipLevel->setPosition(ccp(vipBGWid - 16 , vipBGHei / 2));
    vipLevel->setAnchorPoint(ccp(1.0f, 0.5f));
    m_pVipBG->addChild(vipLevel,0,10); // tag 10 更新vip等级时使用

    
    // 状态栏
    m_pPlayerStatusBar = CCSprite::create(PLAYER_STATUS_BAR);
    m_pPlayerStatusBar->setPosition(ccp(width + m_pPlayerStatusBar->getContentSize().width / 2 - 37 ,winSize.height - m_pPlayerStatusBar->getContentSize().height / 2 - 34));
    addChild(m_pPlayerStatusBar);
    
    // 血条
    m_pPlayerBloodBar = CCProgressTimer::create(CCSprite::create(PLAYER_BLOOD));
    m_pPlayerBloodBar->setPosition(ccp(m_pPlayerStatusBar->getPositionX() - m_pPlayerStatusBar->getContentSize().width / 2 + 27 ,m_pPlayerStatusBar->getPositionY() + m_pPlayerBloodBar->getContentSize().height / 2 + 1));
    m_pPlayerBloodBar->setMidpoint(ccp(0,0));
    m_pPlayerBloodBar->setBarChangeRate(ccp(1, 0));
    m_pPlayerBloodBar->setType(kCCProgressTimerTypeBar);
    m_pPlayerBloodBar->setAnchorPoint(ccp(0,0.5));
    m_pPlayerBloodBar->setPercentage(100);
    addChild(m_pPlayerBloodBar);
    
    // 血值
    m_pHPLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 18);
    m_pHPLabel->setPosition(ccp(m_pPlayerBloodBar->getPositionX() + m_pPlayerBloodBar->getContentSize().width / 2, m_pPlayerBloodBar->getPositionY() - 1));
    addChild(m_pHPLabel);
    
    // 蓝条
    m_pPlayerManaBar = CCProgressTimer::create(CCSprite::create(PLAYER_MANA));
    m_pPlayerManaBar->setPosition(ccp(m_pPlayerStatusBar->getPositionX() - m_pPlayerStatusBar->getContentSize().width / 2 + 23 ,m_pPlayerStatusBar->getPositionY() - m_pPlayerManaBar->getContentSize().height / 2 - 7));
    m_pPlayerManaBar->setMidpoint(ccp(0,0));
    m_pPlayerManaBar->setBarChangeRate(ccp(1, 0));
    m_pPlayerManaBar->setType(kCCProgressTimerTypeBar);
    m_pPlayerManaBar->setAnchorPoint(ccp(0,0.5));
    m_pPlayerManaBar->setPercentage(100);
    addChild(m_pPlayerManaBar);
    
    // 法力值
    m_pMPLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 15);
    m_pMPLabel->setPosition(ccp(m_pPlayerManaBar->getPositionX() + m_pPlayerManaBar->getContentSize().width / 2, m_pPlayerManaBar->getPositionY()));
    addChild(m_pMPLabel);
    
    // 能量槽
    m_pPlayerAngerStatusBar = CCSprite::create(PLAYER_ENERGY_BAR);
    m_pPlayerAngerStatusBar->setPosition(ccp(width + m_pPlayerAngerStatusBar->getContentSize().width / 2 - 27 ,m_pPlayerStatusBar->getPositionY() - (m_pPlayerStatusBar->getContentSize().height + m_pPlayerAngerStatusBar->getContentSize().height) / 2));
    addChild(m_pPlayerAngerStatusBar);
    
    // 能量条
    m_pEnergyBar = CCProgressTimer::create(CCSprite::create(PLAYER_ENERGY));
    m_pEnergyBar->setPosition(ccp(m_pPlayerAngerStatusBar->getContentSize().width/2-10,m_pPlayerAngerStatusBar->getContentSize().height/2 + 2));
    m_pEnergyBar->setMidpoint(ccp(0,0));
    m_pEnergyBar->setBarChangeRate(ccp(1, 0));
    m_pEnergyBar->setType(kCCProgressTimerTypeBar);
    m_pPlayerAngerStatusBar->addChild(m_pEnergyBar);
    
    // 等级
    m_pLevelLabelFrame = CCSprite::create(LEVEL_LABEL_BG_FRAME);
    m_pLevelLabelFrame->setPosition(ccp(177,winSize.height - 132));
    addChild(m_pLevelLabelFrame);
    
    m_pLevelLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 20);
    m_pLevelLabel->setPosition(ccp(m_pLevelLabelFrame->getContentSize().width / 2 - 5, m_pLevelLabelFrame->getContentSize().height / 2));
    m_pLevelLabelFrame->addChild(m_pLevelLabel);
}

void YYControlUILayer::initElements()
{
    commonUI();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (Controller::getInstance()->getSceneType() == DUNGEONS || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {
        m_pLevelLabelFrame->setPositionY(m_pLevelLabelFrame->getPosition().y + 5);
        // 设置触控UI层虚拟按键位置
        setFireKeyPosition(ccp(winSize.width - 100, 90));
        setSkillShortcutKeyPosition(0, ccp(winSize.width - 100,225));
        setSkillShortcutKeyPosition(1, ccp(winSize.width - 217,175));
        setSkillShortcutKeyPosition(2, ccp(winSize.width - 235,60));
        
        for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i++)
        {
            m_pSkillIconBGFrame[i] = CCSprite::create(VIRTUAL_SKILL_BG_NOMAL);
            m_pSkillIconBGFrame[i]->setPosition(getSkillShortcutKeyPosition(i));
            addChild(m_pSkillIconBGFrame[i]);
        }
        
        // 手势轨迹
        m_pTarget = CCRenderTexture::create(winSize.width, winSize.height, kCCTexture2DPixelFormat_RGBA8888);
        m_pTarget->retain();
        m_pTarget->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        addChild(m_pTarget, -1);
        
        m_pBrush =  CCSprite::create(GESTURE_TRACK);
        m_pBrush->retain();
        m_pBrush->setColor(ccRED);
        m_pBrush->setOpacity(20);
        
        // 虚拟方向键
        m_pVirtualRockerFrame = CCSprite::create(VIRTUAL_DIR_BG_NOMAL);
        m_pVirtualRockerFrame->setPosition(DIRECT_VIRTUAL_KEY_POSITION);
        addChild(m_pVirtualRockerFrame);
        
        m_pVirtualRocker = CCSprite::create(VIRTUAL_ROCKER_NOMAL);
        m_pVirtualRocker->setPosition(DIRECT_VIRTUAL_KEY_POSITION);
        addChild(m_pVirtualRocker);
        // 虚拟攻击键
        m_pVirtualFireKey = CCSprite::create(VIRTUAL_FIRE_BG_NOMAL);
        m_pVirtualFireKey->setPosition(getFireKeyPosition());
        addChild(m_pVirtualFireKey);
        
        // 攻击键中间部分
        CCSprite *spr = NULL;
        if (Controller::getInstance()->getProfessionIndex() == 3)
        {
            spr = CCSprite::create(PLAYER3_ATTACK_ICON);
        }
        else if (Controller::getInstance()->getProfessionIndex() == 2)
        {
            spr = CCSprite::create(PLAYER2_ATTACK_ICON);
        }
        spr->setAnchorPoint(ccp(0.5, 0.5));
        spr->setPosition(ccp(m_pVirtualFireKey->getContentSize().width / 2, m_pVirtualFireKey->getContentSize().height / 2));
        m_pVirtualFireKey->addChild(spr);
        
        // 设置技能快捷键
        CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
        setShortcutSkillIcon(array);
        
        // 连击数
        m_pComboLabel = CCSprite::create(COMBO_LABEL);
        addChild(m_pComboLabel);
        m_pComboLabel->setAnchorPoint(ccp(0.5,0.5));
        m_pComboLabel->setVisible(false);
        
        m_pComboNumber = CCLabelAtlas::create("",COMBO_NUM,49,70,'0');// 按ascii排列顺序排在9后面的是:;对应图片里的-+
        addChild(m_pComboNumber);
        m_pComboNumber->setAnchorPoint(ccp(0.5,0.5));
        m_pComboNumber->setVisible(false);
  
        // 返回 在新手引导阶段屏蔽此按钮
        if(Controller::getInstance()->getSceneType() == DUNGEONS)
        {
            CCSprite *button = CCSprite::create(RETURN_CITY_NOMAL);
            button->setPosition(ccp(winSize.width / 4 - 25, winSize.height - 20));
            button->setTag(TAG_RETURN_BUTTON);
            addChild(button);
        }
        
        m_pTaskShowLayer = CCLayer::create();
        m_pScrollView = cocos2d::extension::CCScrollView::create();
        m_pScrollView->ignoreAnchorPointForPosition(false);
        m_pScrollView->setViewSize(CCSizeMake(SHOW_TASKINFO_WIDTH, SHOW_TASKINFO_HEIGHT));
        m_pScrollView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
        m_pScrollView->setAnchorPoint(CCPointZero);
        m_pScrollView->setContainer(m_pTaskShowLayer);
        m_pScrollView->setZoomScale(0.001);
        m_pScrollView->setContentSize(ccp(SHOW_TASKINFO_WIDTH,SHOW_TASKINFO_HEIGHT));
        m_pTaskShowLayer->setContentSize(ccp(SHOW_TASKINFO_WIDTH,SHOW_TASKINFO_HEIGHT));
        
        cocos2d::extension::CCScrollView* m_pScrollView1 = cocos2d::extension::CCScrollView::create();
        m_pScrollView1->setViewSize(CCSizeMake(SHOW_TASKINFO_WIDTH, SHOW_TASKINFO_HEIGHT));
        m_pScrollView1->ignoreAnchorPointForPosition(false);
        m_pScrollView1->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
        m_pScrollView1->setAnchorPoint(CCPointZero);
        m_pScrollView1->setContainer(m_pScrollView);
        m_pScrollView1->setZoomScale(0.001);
        m_pScrollView1->setPosition(ccp(winSize.width - SHOW_TASKINFO_WIDTH,winSize.height - 450));
        m_pScrollView1->setContentSize(ccp(SHOW_TASKINFO_WIDTH,SHOW_TASKINFO_HEIGHT));
        addChild(m_pScrollView1,10);
        m_pScrollView1->setTouchEnabled(false);

        if(Controller::getInstance()->getSceneType() == DUNGEONS)
        {
            m_bIsShowTaskInfo = true;
            showTaskInfo();
        }
        // 手势标记
        std::string pGesture[] = {GESTURE_LINE1, GESTURE_V1, GESTURE_CIRCLE1};
        for (int i = 0; i < 3; i++)
        {
            CCSprite *pSpriteBg = CCSprite::create(GESTURE_BG);
            pSpriteBg->setPosition(ccp(i * (m_pPlayerAngerStatusBar->getContentSize().width / 3 - 30) + 90, -12));
            m_pPlayerAngerStatusBar->addChild(pSpriteBg, 0, 201 + i);
            CCSprite *pSpriteGesture = CCSprite::create(pGesture[i].c_str());

            pSpriteGesture->setPosition(ccp(i * (m_pPlayerAngerStatusBar->getContentSize().width / 3 - 30) + 91, -16));
            m_pPlayerAngerStatusBar->addChild(pSpriteGesture, 0, 101 + i);
        }
    }
    else
    {//非战斗区域
        m_pPlayerAngerStatusBar->setVisible(false);
        // 宝石
        m_pPlayerJewel = CCSprite::create(JEWEL_ICON);
        m_pPlayerJewel->setPosition(ccp(m_pPlayerStatusBar->getPositionX() - m_pPlayerStatusBar->getContentSize().width / 2 + 30 ,m_pPlayerStatusBar->getPositionY() + m_pPlayerStatusBar->getContentSize().height/2 + 10));
        addChild(m_pPlayerJewel);
        m_pJewelLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 20);
        m_pJewelLabel->setColor(ccYELLOW);
        m_pJewelLabel->setAnchorPoint(ccp(0,0.5));
        m_pJewelLabel->setPosition(ccp(m_pPlayerJewel->getContentSize().width + 10,m_pPlayerJewel->getContentSize().height/2));
        m_pPlayerJewel->addChild(m_pJewelLabel,1,1);
        
        // 金币
        m_pPlayerMoney = CCSprite::create(GOLD_ICON);
        m_pPlayerMoney->setPosition(ccp(m_pPlayerStatusBar->getPositionX() + 10 ,m_pPlayerStatusBar->getPositionY() + m_pPlayerStatusBar->getContentSize().height/2 + 10));
        addChild(m_pPlayerMoney);
        m_pMoneyLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 20);
        m_pMoneyLabel->setColor(ccYELLOW);
        m_pMoneyLabel->setAnchorPoint(ccp(0,0.5));
        m_pMoneyLabel->setPosition(ccp(m_pPlayerMoney->getContentSize().width + 10,m_pPlayerMoney->getContentSize().height/2));
        m_pPlayerMoney->addChild(m_pMoneyLabel,1,1);
        
        // 体力
        m_pPlayerPhysicalStrengthPoint = CCSprite::create(PHYSICAL_POWER_ICON);
        m_pPlayerPhysicalStrengthPoint->setPosition(ccp(m_pPlayerStatusBar->getPositionX(),m_pPlayerStatusBar->getPositionY() - m_pPlayerStatusBar->getContentSize().height/2 - 15));
        addChild(m_pPlayerPhysicalStrengthPoint);
        CCLabelTTF* pPSLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 20);
        pPSLabel->setColor(ccYELLOW);
        pPSLabel->setAnchorPoint(ccp(0,0.5));
        pPSLabel->setPosition(ccp(m_pPlayerPhysicalStrengthPoint->getContentSize().width + 10,m_pPlayerPhysicalStrengthPoint->getContentSize().height/2));
        m_pPlayerPhysicalStrengthPoint->addChild(pPSLabel,1,1);
    
        setUIHideButtonPosition(ccp(winSize.width - 80,80));
        CCSprite * sprite = CCSprite::create(HIDE_UI_BUTTON_ICON);
        addChild(sprite, 1,TAG_HIDE_BUTTON);
        sprite->setPosition(getUIHideButtonPosition());
        
        m_pPlayerFighttingPoint = CCSprite::create(MILITARY_POWER_BG1);
        addChild(m_pPlayerFighttingPoint,0,TAG_FTINGHTTING);
        m_pPlayerFighttingPoint->setAnchorPoint(ccp(0,0.5));
        m_pPlayerFighttingPoint->setPosition(ccp(0,winSize.height - 240));
        // 火
        CCSprite *pSpriteFightFire = CCSprite::create(MILITARY_POWER_BG2);
        pSpriteFightFire->setAnchorPoint(ccp(0.5, 0.5));
        pSpriteFightFire->setPosition(ccp(m_pPlayerFighttingPoint->getContentSize().width / 2 + 20, m_pPlayerFighttingPoint->getContentSize().height + 20));

        CCLabelAtlas* pFighttingValue = CCLabelAtlas::create("", MILITARY_POWER_NUM, 30, 43,'0');
        pFighttingValue->setAnchorPoint(ccp(0,0.5));

        pFighttingValue->setPosition(ccp(m_pPlayerFighttingPoint->getContentSize().width / 4 * 3 - 95, m_pPlayerFighttingPoint->getContentSize().height));
        pFighttingValue->setScale(0.8);
        m_pPlayerFighttingPoint->addChild(pFighttingValue, 1, 1);
        
        // 战斗力
        CCSprite *pSpriteFightIcon = CCSprite::create(MILITARY_POWER_LABEL);
        pSpriteFightIcon->setAnchorPoint(ccp(0.5, 0.5));
        pSpriteFightIcon->setPosition(ccp(m_pPlayerFighttingPoint->getContentSize().width / 2 - 100, m_pPlayerFighttingPoint->getContentSize().height + 18));
        m_pPlayerFighttingPoint->addChild(pSpriteFightIcon);
        
        // 战力进度条
        CCSprite *pSpriteFightBlood = CCSprite::create(MILITARY_POWER_PROGRESS);
        pSpriteFightBlood->setAnchorPoint(ccp(0.5, 0.5));
        pSpriteFightBlood->setPosition(ccp(m_pPlayerFighttingPoint->getContentSize().width / 2 + 33, m_pPlayerFighttingPoint->getContentSize().height / 2 - 1));
        m_pPlayerFighttingPoint->addChild(pSpriteFightBlood);
        
        //任务
        m_pSpriteTask = NULL;
        m_pSpriteTask = CCSprite::create(TASK_ICON1);
        m_pSpriteTask->setAnchorPoint(ccp(0, 0.5));
        m_pSpriteTask->setPosition(ccp(15, winSize.height - 320));
        addChild(m_pSpriteTask);
        
        // 已接任务数
        CCSprite *pSpriteTaskCountBg = CCSprite::create(TASK_ICON2);
        pSpriteTaskCountBg->setPosition(ccp(m_pSpriteTask->getContentSize().width - 7, m_pSpriteTask->getContentSize().height - 11));
        m_pSpriteTask->addChild(pSpriteTaskCountBg);
        
        const char* taskCount = "0";
        if(GlobalData::getInstance()->getTaskDataDict())
        {
            taskCount = CCString::createWithFormat("%d",GlobalData::getInstance()->getTaskDataDict()->count())->getCString();
        }
        m_pTaskNumberLabel = CCLabelTTF::create(taskCount, DEFAULT_FONT_NAME, 12);
        m_pTaskNumberLabel->setPosition(ccp(pSpriteTaskCountBg->getContentSize().width - 14, pSpriteTaskCountBg->getContentSize().height - 12));
        pSpriteTaskCountBg->addChild(m_pTaskNumberLabel,1);

        
        // 试炼
        m_pSpriteTrain = NULL;
        m_pSpriteTrain = CCSprite::create(TRIAL_ICON);
        m_pSpriteTrain->setAnchorPoint(ccp(0, 0.5));
        m_pSpriteTrain->setPosition(ccp(105,winSize.height-410));
        addChild(m_pSpriteTrain);
        
        
        //公告
        m_pSpriteNotice = NULL;
        m_pSpriteNotice = CCSprite::create(NOTICE_ICON);
        m_pSpriteNotice->setAnchorPoint(ccp(0, 0.5));
        m_pSpriteNotice->setPosition(ccp(15, winSize.height - 410));
        addChild(m_pSpriteNotice);
        
        //兑换码
        m_pSpriteConvertCode = NULL;
        m_pSpriteConvertCode = CCSprite::create(CONCODE_ICON);
        m_pSpriteConvertCode->setAnchorPoint(ccp(0,0.5));
        m_pSpriteConvertCode->setPosition(ccp(15,winSize.height - 500));
        addChild(m_pSpriteConvertCode);
        
        //活动
        m_pSpritePlayTable = NULL;
        m_pSpritePlayTable = CCSprite::create(PLAYTABLE_ICON);
        m_pSpritePlayTable->setAnchorPoint(ccp(0,0.5));
        m_pSpritePlayTable->setPosition(ccp(105,winSize.height - 500));
        addChild(m_pSpritePlayTable);

        // 商城
        m_pSpriteShop = NULL;
        m_pSpriteShop = CCSprite::create(MALL_ICON);
        m_pSpriteShop->setPosition(ccp(winSize.width - 200,winSize.height - 60));
        addChild(m_pSpriteShop);
        
        // 登录奖励
        m_pSpriteLoginAward = NULL;
        m_pSpriteLoginAward = CCSprite::create(LOGIN_AWARD_ICON);
        m_pSpriteLoginAward->setPosition(ccp(winSize.width - 290,winSize.height - 60));
        addChild(m_pSpriteLoginAward);
        
        // 签到
        m_pSpriteSignUp = NULL;
        m_pSpriteSignUp = CCSprite::create(SIGNUP_ICON);
//        m_pSpriteSignUp->setPosition(ccp(winSize.width - 290,winSize.height - 60));
        m_pSpriteSignUp->setPosition(ccp(winSize.width - 380,winSize.height - 60));
        addChild(m_pSpriteSignUp);
        
        // 活跃度
        m_pSpriteActivity = NULL;
        m_pSpriteActivity = CCSprite::create(ACTIVITY_ICON);
//        m_pSpriteActivity->setPosition(ccp(winSize.width - 380,winSize.height - 60));
        m_pSpriteActivity->setPosition(ccp(winSize.width - 470,winSize.height - 60));
        addChild(m_pSpriteActivity);

        // 充值
        m_pSpriteCharge = NULL;
        m_pSpriteCharge = CCSprite::create(RECHARGE_ICON);
        m_pSpriteCharge->setPosition(ccp(m_pPlayerStatusBar->getPositionX() - m_pPlayerStatusBar->getContentSize().width/4 - 10, m_pPlayerStatusBar->getPositionY() - 36));
        addChild(m_pSpriteCharge);
        
        // 聊天框
        m_pChattingBox = cocos2d::extension::CCScale9Sprite::create(CHAT_BOX);
        m_pChattingBox->setContentSize(CCSize(700, 43));
        m_pChattingBox->setAnchorPoint(ccp(0,0));
        
        m_pChatBoxScrollView = cocos2d::extension::CCScrollView::create();
        m_pChatBoxScrollView->ignoreAnchorPointForPosition(false);
        m_pChatBoxScrollView->setViewSize(CCSizeMake(m_pChattingBox->getContentSize().width - 10, m_pChattingBox->getContentSize().height));
        m_pChatBoxScrollView->setDirection(cocos2d::extension::kCCScrollViewDirectionVertical);
        m_pChatBoxScrollView->setBounceable(false);
        m_pChatBoxScrollView->setAnchorPoint(CCPointZero);
        m_pChatBoxScrollView->setContainer(m_pChattingBox);
        m_pChattingBox->setScaleX(0.9);
        m_pChatBoxScrollView->setPosition(ccp(150, 50));
        addChild(m_pChatBoxScrollView,2);

        // 聊天
        m_pSpriteChat = NULL;
        m_pSpriteChat = CCSprite::create(CHAT_BUTTON);
        m_pSpriteChat->setPosition(ccp(60, m_pChatBoxScrollView->getPositionY() + m_pChatBoxScrollView->getContentSize().height/2));
        addChild(m_pSpriteChat);
        
        // 右上角任务头像信息
        m_pGuideButton = YYButton::create();
        m_pGuideButton->initWithFile(BOSS_PORTRAIT_BG_FRAME);
        m_pGuideButton->setPosition(ccp(winSize.width - 85,winSize.height - 85));
        addChild(m_pGuideButton, 2);
        
        YYSprite * shortcutEffect = YYSprite::createWithActionSet("shortcuteffect", "shortcuteffect", kDirTypeEffect);
        shortcutEffect->setAction(0,true);
        shortcutEffect->setTag(TAG_SHORTCUT_EFFECT);
        shortcutEffect->setPosition(ccp(m_pGuideButton->getPosition().x + 3, m_pGuideButton->getPosition().y - 2));
        addChild(shortcutEffect,2);
        
        // 寻路提示
        m_pAutoFindTips = YYSprite::create();
        m_pAutoFindTips->initWithFile(FINDING_WAY_TIPS);
        m_pAutoFindTips->setPosition(ccp(winSize.width/2,winSize.height/2));
        addChild(m_pAutoFindTips);
        m_pAutoFindTips->setVisible(false);
        
        CCSprite* pExperienceBackSprite = CCSprite::create(EXPERIENCE_BG_1);
        pExperienceBackSprite->setPosition(ccp(winSize.width / 2, pExperienceBackSprite->getContentSize().height/2 + 2));
        addChild(pExperienceBackSprite);
        pExperienceBackSprite->setTag(TAG_EXPERIENCE_ICON);

        //经验条背景框
        m_nExpMaxWidth = pExperienceBackSprite->getContentSize().width;
        m_pExpSpriteBatch = CCSpriteBatchNode::create(EXPERIENCE_BG_2);
        addChild(m_pExpSpriteBatch,1);
        m_pExpSpriteBatch->setPosition(ccp(winSize.width / 2, 10));
        
        //经验条
        CCSprite* pExperienceFrontSprite = CCSprite::create(EXPERIENCE_PROGRESS);
        pExperienceFrontSprite->setPosition(ccp(winSize.width / 2, pExperienceFrontSprite->getContentSize().height / 2));
        addChild(pExperienceFrontSprite, 2);
        pExperienceFrontSprite->setTag(TAG_EXPERIENCE_CUBE);
        
        m_pNoticeLayer = CCLayerColor::create(ccc4(95, 95, 95, 196));
        m_pNoticeLayer->setContentSize(CCSize(winSize.width, 50));
        m_pNoticeLayer->setPosition(ccp(0, winSize.height*4 / 5 - 30));
        m_pNoticeLayer->setVisible(false);
        addChild(m_pNoticeLayer, 5);
    }
}

void YYControlUILayer::updateTaskGuide()
{
    if (Controller::getInstance()->getSceneType() == NEWBIE_TASK_ZONE || Controller::getInstance()->getSceneType() == MAIN_CITY)
    {
        if(!m_pGuideButton)
        {
            return;
        }
        if(m_pGuideButton->getChildByTag(1))
        {
            m_pGuideButton->removeChildByTag(1);
        }
        CCArray *arr = GlobalData::getInstance()->getTaskDataArray();
        for(int i = 0; i < arr->count(); i++)
        {
            TaskData * taskData = (TaskData *)arr->objectAtIndex(i);
            if(taskData->getTargetType() == 5)
            {
                continue;
            }
            if (taskData && !taskData->getTargetIcon().empty())
            {
                std::string filePath = getFilePath(taskData->getTargetIcon(), kDirTypeIcon, kFileSuffixPng);
                CCSprite* pSprite = CCSprite::create(filePath.c_str());
                m_pGuideButton->addChild(pSprite,1,1);
                m_pGuideButton->setIndex(taskData->getTaskIndex());
                pSprite->setPosition(ccp(m_pGuideButton->getContentSize().width / 2, m_pGuideButton->getContentSize().height / 2));
                Controller::getInstance()->getScene()->getPopUpUI()->setAcceptedTaskIndex(taskData->getTaskIndex());
                Controller::getInstance()->getScene()->getSpriteLayer()->openPortal(taskData->getStatus());
            }
            break;
        }
    }
}

void YYControlUILayer::updateGuideButton(int taskIndex)
{
    TaskData * taskData = (TaskData *) GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
    if(m_pGuideButton->getChildByTag(1))
    {
        m_pGuideButton->removeChildByTag(1);
    }
    if (taskData && !taskData->getTargetIcon().empty())
    {
        std::string path = getFilePath(taskData->getTargetIcon(), kDirTypeIcon, kFileSuffixPng);
        CCSprite* pSprite = CCSprite::create(path.c_str());
        m_pGuideButton->addChild(pSprite,1,1);
        m_pGuideButton->setIndex(taskData->getTaskIndex());
        pSprite->setPosition(ccp(m_pGuideButton->getContentSize().width / 2 , m_pGuideButton->getContentSize().height / 2));
    }
}

void YYControlUILayer::showTaskInfo()
{
    if(m_pTaskShowLayer->getChildrenCount() > 0)
    {
        m_pTaskShowLayer->removeAllChildren();
    }
    m_pScrollView->setVisible(true);

    CCArray* arr = GlobalData::getInstance()->getTaskDataArray();

    float taskWidth = m_pScrollView->getViewSize().width;
    float taskHeight = m_pScrollView->getViewSize().height;
    for(int i = 0; i< arr->count(); ++ i)
    {
        TaskData * task = (TaskData *)arr->objectAtIndex(i);
        if(task->getTargetType() == kTaskCollect || task->getTargetType() == kTaskKillMonster)
        {

            CCArray * arrayAttribute = task->getTargetArray();
            CCLabelTTF* pTaskTitleLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 24);
            pTaskTitleLabel->setString(CCString::createWithFormat("[%s]%s",task->getTaskType().c_str(),task->getTitle().c_str())->getCString());
            pTaskTitleLabel->setAnchorPoint(ccp(0,1));
            pTaskTitleLabel->setPosition(ccp(0,taskHeight));
            taskHeight -= pTaskTitleLabel->getContentSize().height;
            m_pTaskShowLayer->addChild(pTaskTitleLabel);
            for (int j = 0; j < arrayAttribute->count(); ++j)
            {
                TaskTarget *taskTarget = (TaskTarget *)arrayAttribute->objectAtIndex(j);
                CCLabelTTF* pTaskTargetLabel = CCLabelTTF::create(taskTarget->getTargetName().c_str(), DEFAULT_FONT_NAME, 20);
                pTaskTargetLabel->setDimensions(CCSize(120, 0));
                pTaskTargetLabel->setHorizontalAlignment(kCCTextAlignmentLeft);
                pTaskTargetLabel->setAnchorPoint(ccp(0,1));
                pTaskTargetLabel->setPosition(ccp(30,taskHeight));
                m_pTaskShowLayer->addChild(pTaskTargetLabel);
                
                CCLabelTTF* pShowKillerLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 20);
                if (task->getTargetType() == kTaskCollect)
                {
                    pShowKillerLabel->setString(CCString::createWithFormat("%d / %d",taskTarget->getTargetItemCount(),taskTarget->getTargetItemAmount())->getCString());
                }
                else
                {
                    pShowKillerLabel->setString(CCString::createWithFormat("%d / %d",taskTarget->getTargetMonsterCount(),taskTarget->getTargetMonsterAmount())->getCString());
                }
                pShowKillerLabel->setAnchorPoint(ccp(0,0));
                pShowKillerLabel->setPosition(ccp(210,taskHeight - pTaskTargetLabel->getContentSize().height));
                m_pTaskShowLayer->addChild(pShowKillerLabel);
                taskHeight -= pTaskTargetLabel->getContentSize().height;
            }
        }
    }
    if(taskHeight < 0)
    {
        m_pTaskShowLayer->setContentSize(CCSize(taskWidth, m_pScrollView->getViewSize().height - taskHeight));
        m_pTaskShowLayer->setPosition(ccp(0,taskHeight));
        for(int i=0; i < m_pTaskShowLayer->getChildrenCount();++i)
        {
            CCNode* pNode = (CCNode*)(m_pTaskShowLayer->getChildren()->objectAtIndex(i));
            pNode->setPosition(ccpAdd(pNode->getPosition(), ccp(0,-taskHeight)));
        }
    }
}


#pragma mark 触摸控制

void YYControlUILayer::touchBegan(CCTouch * touch)
{
    m_n8TouchEventType = kTouchEventDefault;
    if (Controller::getInstance()->isTutorial())// 新手引导
    {
        YYGuide *guide = static_cast<YYGuide *>(getChildByTag(TAG_GUIDE));
        if (guide->getTouchBeganValue() == kTouchEventDefault)
        {
            if (Controller::getInstance()->isTouchGuideRect(touch->getLocation()))
            {
                Controller::getInstance()->setTutorial(false);
                removeChildByTag(TAG_GUIDE);
                Controller::getInstance()->setScriptStatus(kScriptStatusRunning);
            }
            else
            {
                return;
            }
        }
    }
    else if(m_bTalk)// 对话状态下点击事件为翻页
    {
        m_n8TouchEventType = kTouchEventPageDown;
        return;
    }
    else if(m_pRoleByControlled->isLocked())// 主角被锁定且未进行新手引导，触摸操作无效
    {
        return;
    }
    //未锁定操作或新手指引时按指示触摸允许有下列操作
    if(Controller::getInstance()->getSceneType() == MAIN_CITY || Controller::getInstance()->getSceneType() == NEWBIE_TASK_ZONE)
    {
        CCRect rect = CCRectMake(20, 54, m_pChattingBox->boundingBox().size.width, m_pChattingBox->boundingBox().size.height);
        m_TouchPoint = touch->getLocation();
        if(m_pGuideButton && Controller::isButtonTouched(m_pGuideButton,m_TouchPoint))
        {// 点中右上角的寻路快捷按键
            m_n8TouchEventType = kTouchEventFindingWay;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (getChildByTag(TAG_HIDE_BUTTON) && Controller::isButtonTouched(getChildByTag(TAG_HIDE_BUTTON), m_TouchPoint))
        {// 隐藏或显示UI按键
            if(!m_bUIButtonMoving)
            {
                m_n8TouchEventType = kTouchEventHideUIMenu;
                const std::string filePath = getFilePath(MENU_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                PLAY_EFFECT(filePath.c_str());
            }
        }
        else if ((m_pChatBoxScrollView->isVisible() &&  rect.containsPoint(m_TouchPoint))
                 || (m_pSpriteChat && Controller::isButtonTouched(m_pSpriteChat, m_TouchPoint)))
        {
            m_n8TouchEventType = kTouchEventChat;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (Controller::isButtonTouched(m_pVipBG, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventVip;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (Controller::isButtonTouched(m_pPortraitBGFrame, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventRolePortrait;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteTask && Controller::isButtonTouched(m_pSpriteTask, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventTask;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteShop && Controller::isButtonTouched(m_pSpriteShop, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventShop;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteLoginAward && Controller::isButtonTouched(m_pSpriteLoginAward, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventLoginAward;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteSignUp && Controller::isButtonTouched(m_pSpriteSignUp, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventSignIn;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteActivity && Controller::isButtonTouched(m_pSpriteActivity, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventActivity;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteCharge && Controller::isButtonTouched(m_pSpriteCharge, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventCharge;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteTrain && Controller::isButtonTouched(m_pSpriteTrain, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventTrain;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteNotice && Controller::isButtonTouched(m_pSpriteNotice, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventNotice;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if (m_pSpriteConvertCode && Controller::isButtonTouched(m_pSpriteConvertCode, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventConvertCode;
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else if(m_pSpritePlayTable && Controller::isButtonTouched(m_pSpritePlayTable, m_TouchPoint))
        {
            m_n8TouchEventType = kTouchEventPlayTable;
            const std::string filePath = getFilePath(CLICKED_EFFECT, kDirTypeSound, kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else
        {
            if (!isHideUIButton())
            {
                if(m_pUIButtonArray)
                {
                    for (int i = 0 ; i < m_pUIButtonArray->count(); i++)
                    {
                        CCSprite * sprite = (CCSprite*)m_pUIButtonArray->objectAtIndex(i);
                        if (Controller::isButtonTouched(sprite, m_TouchPoint))
                        {
                            m_n8TouchEventType = kTouchEventBag + i;
                            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                            PLAY_EFFECT(filePath.c_str());
                            break;
                        }
                    }
                }
            }
            if (m_n8TouchEventType == kTouchEventDefault)
            {// 未点中任何按键则主角位移
                m_MoveTouchPoint = m_TouchPoint;
                m_pRoleByControlled->setRunningStatus(kRunningStatusNormal);
                m_n8TouchEventType = kTouchEventSetDestination;
            }
        }
    }
    else
    {// 战斗区域
        int counter = Controller::getInstance()->getScene()->getFightResultDelayCounter();
        if(counter < 0)
        {
            CCSprite *pSpriteReturn = (CCSprite *)getChildByTag(TAG_RETURN_BUTTON);
            if(pSpriteReturn
               && Controller::isButtonTouched(pSpriteReturn,touch->getLocation()))
            {// 返回主城
                pSpriteReturn->setVisible(true);
                pSpriteReturn->setTexture(CCTextureCache::sharedTextureCache()->addImage(RETURN_CITY_PRESS));
                m_n8TouchEventType = kTouchEventReturnCity;
                const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                PLAY_EFFECT(filePath.c_str());
            }
        }
        if (m_n8TouchEventType != kTouchEventReturnCity)
        {
            for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i++)
            {// 快捷栏技能
                if(isPointInCircle(touch->getLocation(), m_SkillShortcutKeyPostion[i], SKILL_RADIUS * 1.5))
                {
                    m_n8TouchEventType = kTouchEventSkillShortCut0 + i;
                    if(m_pSkillIconBGFrame[i])
                    {
                        m_pSkillIconBGFrame[i]->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_SKILL_BG_PRESS));
                    }
                    break;
                }
            }
            if (m_n8TouchEventType == kTouchEventDefault)
            {
                if (isPointInCircle(touch->getLocation(), m_FireKeyPosition, FIRE_RADIUS))
                {// 点击虚拟攻击键
                    m_bHoldAttackKey = true;
                    m_pVirtualFireKey->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_FIRE_BG_PRESS));
                    m_n8TouchEventType = kTouchEventFire;
                }
                else if (isPointInCircle(touch->getLocation(), DIRECT_VIRTUAL_KEY_POSITION, DIRECT_AREA_RADIUS))
                {// 点击虚拟方向键
                    m_bHoldDirectionKey = true;
                    m_MoveTouchPoint = touch->getLocation();
                    m_n8TouchEventType = kTouchEventRun;
                }
                else
                {// 手势技能
                    m_StartPoint = touch->getLocation();
                    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                    if (CCRectMake(0 , 0,DIRECT_VIRTUAL_KEY_POSITION.x * 2 + 100,DIRECT_VIRTUAL_KEY_POSITION.y * 2 + 100).containsPoint(m_StartPoint))
                    {
                    }
                    else if (CCRectMake(winSize.width - 300, 0, 300, 300).containsPoint(m_StartPoint))
                    {
                    }
                    else
                    {
                        m_n8TouchEventType = kTouchEventGestureSkill;
                    }
                }
            }
        }
    }
    if (m_n8TouchEventType != kTouchEventDefault)
    {
        if (Controller::getInstance()->isTutorial())// 新手引导
        {
            YYGuide *guide = static_cast<YYGuide *>(getChildByTag(TAG_GUIDE));
            CC_ASSERT(guide);
            if (guide->getTouchBeganValue() == m_n8TouchEventType)
            {
                if (guide->getTouchEndedValue() == -1)
                {
                    Controller::getInstance()->setTutorial(false);
                    removeChildByTag(TAG_GUIDE);
                    Controller::getInstance()->setScriptStatus(kScriptStatusRunning);
                }
            }
            else
            {
                m_n8TouchEventType = kTouchEventDefault;
                return;
            }
        }
        m_pTouchDictionary->setObject(touch, m_n8TouchEventType);
        
    }
    
}

void YYControlUILayer::touchMoved(CCTouch * touch)
{
    for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i ++)
    {// 快捷栏技能
        if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventSkillShortCut0 + i)))
        {
            if(!isPointInCircle(touch->getLocation(), m_SkillShortcutKeyPostion[i], SKILL_RADIUS * 1.5))
            {
                m_pTouchDictionary->removeObjectForKey(kTouchEventSkillShortCut0 + i);
                if(m_pSkillIconBGFrame[i])
                {
                    m_pSkillIconBGFrame[i]->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_SKILL_BG_NOMAL));
                }
            }
        }
    }
    if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventRun)))
    {
        m_MoveTouchPoint = touch->getLocation();
    }
    if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventGestureSkill)))
    {
        m_TouchPoint = touch->getLocation();
        m_PreTouchPoint = touch->getPreviousLocation();
        p_2dPath.push_back(m_TouchPoint);
        float distance = ccpDistance(m_TouchPoint, m_PreTouchPoint);
        m_pTarget->begin();
        if (distance > 1)
        {
            int d = (int)distance;
            for (int i = 0; i < d; i++)
            {
                float difx = m_PreTouchPoint.x - m_TouchPoint.x;
                float dify = m_PreTouchPoint.y - m_TouchPoint.y;
                float delta = (float)i / distance;
                m_pBrush->setPosition(ccp(m_TouchPoint.x + (difx * delta), m_TouchPoint.y + (dify * delta)));
                m_pBrush->setRotation(rand() % 360);
                float r = (float)(rand() % 50 / 150.f) + 0.25f;
                m_pBrush->setScale(r);
                m_pBrush->setColor(ccc3(rand() % 127 + 128, rand() % 127 + 128, rand() % 127 + 128));
                m_pBrush->visit();
            }
        }
        m_pTarget->end();
    }
}

void YYControlUILayer::touchEnded(CCTouch * touch)
{
    for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i ++)
    {// 快捷栏技能
        if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventSkillShortCut0 + i)))
        {
            m_pTouchDictionary->removeObjectForKey(kTouchEventSkillShortCut0 + i);
            if(m_pSkillIconBGFrame[i])
            {
                m_pSkillIconBGFrame[i]->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_SKILL_BG_NOMAL));
            }
        }
    }
    if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventFire)))
    {// 松开攻击键
        m_bHoldAttackKey = false;
        m_bContinueAttack = true;
        m_pTouchDictionary->removeObjectForKey(kTouchEventFire);
        m_pVirtualFireKey->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_FIRE_BG_NOMAL));
    }
    if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventRun)))
    {// 松开方向键
        m_bHoldDirectionKey = false;
        m_pTouchDictionary->removeObjectForKey(kTouchEventRun);
    }
    if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventGestureSkill)))
    {
        m_pTouchDictionary->removeObjectForKey(kTouchEventGestureSkill);
        gestureEnded();
    }
    
    if(touch->isEqual(m_pTouchDictionary->objectForKey(kTouchEventReturnCity)))
    {
        CCSprite *pSpriteReturn = (CCSprite *)getChildByTag(TAG_RETURN_BUTTON);
        if (pSpriteReturn
            && Controller::isButtonTouched(pSpriteReturn,touch->getLocation()))
        {// 返回主城
            pSpriteReturn->setTexture(CCTextureCache::sharedTextureCache()->addImage(RETURN_CITY_NOMAL));
            bool bIsTrial = Controller::getInstance()->isTrial();  //是否为试炼
            if (bIsTrial)
            {
                Controller::getInstance()->sendMessage("trial F_1",true);
                Controller::getInstance()->setTrial(false);
                Controller::getInstance()->switchGameStatus(kStatusLoading); // 设置游戏状态为loading状态
            }
            else
            {
                DungeonType type =  Controller::getInstance()->getDungeonType(); // 获取副本类型
                switch (type)
                {
                    case kDungeonNormal: // 正常副本
                    {
                        Controller::getInstance()->sendMessage(ASK_RETURN_CITY,false);
                        Controller::getInstance()->switchGameStatus(kStatusLoading); // 设置游戏状态为loading状态
                    }
                        break;
                    case kDungeonDungeon: // 活动_地下城
                    {
                        clearKeyState();//  清除按键状态
                        CCString* msg = CCString::createWithFormat(ASK_DUNGEON_RESULT,0);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                    }
                        break;
                    case kDungeonProtoss: // 活动_守卫星灵
                    {
                        clearKeyState();//  清除按键状态
                        CCString* msg = CCString::createWithFormat(ASK_PROTECT_PROTOSS_RESULT,0);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                    }
                        break;
                    case kDungeonBoss: // 活动_Boss副本
                    {
                        clearKeyState();//  清除按键状态
                        CCString* msg = CCString::createWithFormat(ASK_BOSS_DUNGEON_RESULT,0);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                    }
                        break;
                    case kWorldBoss: // 活动_世界Boss
                    {
                        clearKeyState();//  清除按键状态
                        int damagePoint = ((YYScene *)getParent())->getSpriteLayer()->getWorldBossDamageHP();
                        CCString* msg = CCString::createWithFormat(ASK_WORLD_BOSS_RESULT,damagePoint);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                    }
                        break;
                    default:
                    {
                        Controller::getInstance()->sendMessage(ASK_RETURN_CITY,false);
                        Controller::getInstance()->switchGameStatus(kStatusLoading); // 设置游戏状态为loading状态
                    }
                        break;
                }
            }
        }
    }
}

void YYControlUILayer::touchHandle()
{
    if (m_n8TouchEventType == kTouchEventPageDown)// 对话翻页
    {
        m_n8TouchEventType = kTouchEventDefault;
        updateTalkContent();
        return;
    }
    if (Controller::getInstance()->getSceneType() == DUNGEONS || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {// 可战斗区域
        if (m_pRoleByControlled->getCurrentHP() <= 0)
        {// 玩家死亡
            return;
        }
        if(Controller::getInstance()->isTrial() && m_pRoleByControlled->getTrialPause())//等待试炼阶段 攻击无效
        {
            return;
        }
        for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i ++)
        {
            if (m_n8TouchEventType == kTouchEventSkillShortCut0 + i)
            {
                m_n8TouchEventType = kTouchEventDefault;
                if (m_pRoleByControlled->getActionStatus() == kActionNormal
                    || m_pRoleByControlled->getActionStatus() == kActionAttack)
                {
                    if(m_pCDEffect[i] && m_pCDEffect[i]->getPercentage() == 0)
                    {
                        // 技能编号
                        CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
                        CCString * skillID = (CCString *)array->objectAtIndex(i);
                        if (!skillID->isEqual(CCString::create("")))
                        {
                            int skillIndex = ((CCString *)skillID)->intValue();
                            SkillData * skillData = m_pRoleByControlled->getSkillData(skillIndex);
                            if( m_pRoleByControlled->getCurrentMP() >= skillData->getConsumePoint())
                            {
                                m_pRoleByControlled->setCurrentMP(m_pRoleByControlled->getCurrentMP() - skillData->getConsumePoint());
                                m_pRoleByControlled->useSkill(skillData);
                                cooling(i,skillData->getCoolingDuration());
                            }
                            
                        }
                    }
                    return;
                }
            }
        }
        if (m_bHoldAttackKey)
        {
            m_bContinueAttack = false;// 连续攻击计数重置
            m_nKeyPressIntervalCounter = 0;
            if (m_pRoleByControlled->getActionStatus() == kActionNormal)
            {
                m_pRoleByControlled->attack();
            }
        }
        else if(m_bHoldDirectionKey)
        {// 按住方向键
            if(m_pRoleByControlled->getActionStatus() == kActionNormal// 站立或行走状态或释放特殊技能时
               || (m_pRoleByControlled->getActionStatus() == kActionSkill
                   && m_pRoleByControlled->getSkill()->getSkillAction()->getSkillActionData()->isMovable()))
            {
                m_pRoleByControlled->setMoving(true);
                m_pRoleByControlled->setMoveType(MOVE_BY_CONTROLLED);
            }
        }
        else
        {// 未按键
            if ( m_pRoleByControlled->isMoving())
            {// 从行走状态切换至站立状态
                m_pRoleByControlled->setMoving(false);
                if(m_pRoleByControlled->getActionStatus() == kActionNormal)
                {
                    m_pRoleByControlled->switchAction(ACTION_STAND0);
                }
            }
        }
    }
    else
    {// 非战斗区域
        switch (m_n8TouchEventType)
        {
            case kTouchEventHideUIMenu:
                // 隐藏或显示UI按键
                setHideUIButton(!isHideUIButton());
                isHideUIButton() ? hideUIButton() : showUIButton();
                break;
            case kTouchEventFindingWay:
                if(!Controller::getInstance()->getScene()->isNetLoading())
                {
                    if(m_pGuideButton->getIndex() > 0)
                    {
                        Controller::getInstance()->getScene()->getPopUpUI()->setAcceptedTaskIndex(m_pGuideButton->getIndex());
                        Controller::getInstance()->getScene()->getPopUpUI()->updateTaskResult(m_pGuideButton->getIndex());
                    }
                }
                break;
            case kTouchEventTask:// 任务界面
                Controller::getInstance()->getScene()->getPopUpUI()->initTaskUI();
                break;
            case kTouchEventShop:// 商店界面
                Controller::getInstance()->getScene()->getPopUpUI()->initShopUI();
                break;
            case kTouchEventLoginAward:// 登录奖励界面
                Controller::getInstance()->getScene()->getPopUpUI()->initLoginAwardUI();
                break;
            case kTouchEventSignIn://签到界面
                Controller::getInstance()->getScene()->getPopUpUI()->initSignUpUI();
                break;
            case kTouchEventActivity://活跃度界面
                Controller::getInstance()->getScene()->getPopUpUI()->initActivityUI();
                break;
            case kTouchEventNotice://公告
                Controller::getInstance()->getScene()->getPopUpUI()->initNoticeUI(NULL,false);
                break;
            case kTouchEventConvertCode://兑换码
                Controller::getInstance()->getScene()->getPopUpUI()->initConvertCodeUI();
                break;
            case kTouchEventPlayTable: //活动
                Controller::getInstance()->getScene()->getPopUpUI()->initPlayTableUI();
                break;
            case kTouchEventRolePortrait:// 主角头像
                Controller::getInstance()->getScene()->getPopUpUI()->drawRoleUI();
                break;
            case kTouchEventVip: // vip
                Controller::getInstance()->getScene()->getPopUpUI()->initVipUI();
                break;
            case kTouchEventChat://聊天界面
                Controller::getInstance()->getScene()->getPopUpUI()->initChattingUI();
                break;
            case kTouchEventCharge://充值界面
                Controller::getInstance()->getScene()->getPopUpUI()->initChargeUI();
                break;
            case kTouchEventTrain://试炼界面
                Controller::getInstance()->getScene()->getPopUpUI()->initTrainUI();
                break;
            case kTouchEventBag://背包界面
                Controller::getInstance()->getScene()->getPopUpUI()->initBagUI();
                hideUIButton();
                break;
            case kTouchEventSkill://技能界面
                Controller::getInstance()->getScene()->getPopUpUI()->initSkillUI();
                hideUIButton();
                break;
            case kTouchEventTalent://天赋界面
                Controller::getInstance()->getScene()->getPopUpUI()->initTalentUI();
                hideUIButton();
                break;
            case kTouchEventProtoss://星灵界面
                Controller::getInstance()->getScene()->getPopUpUI()->initProtossBaseUI();
                hideUIButton();
                break;
            case kTouchEventRoleAttribute://角色信息界面
                Controller::getInstance()->getScene()->getPopUpUI()->drawRoleUI();
                hideUIButton();
                break;
            case kTouchEventEmail:// 邮箱界面
                Controller::getInstance()->getScene()->getPopUpUI()->initMailUI(true);
                hideUIButton();
                break;
            case kTouchEventAchievement://成就界面
                Controller::getInstance()->getScene()->getPopUpUI()->initAchievementUI();
                hideUIButton();
                break;
            case kTouchEventSetting://设置界面
                Controller::getInstance()->getScene()->getPopUpUI()->initSettingUI();
                hideUIButton();
                break;
            case kTouchEventSetDestination:// 设置目的地
            {
                m_pRoleByControlled->setFindingTaskNPC(false);
                int offsetX = m_MoveTouchPoint.x + ((YYScene *)getParent())->getScreenDatumPoint().x - m_pRoleByControlled->getPositionX();
                int offsetY = m_MoveTouchPoint.y + ((YYScene *)getParent())->getScreenDatumPoint().y - m_pRoleByControlled->getPositionY();
                m_pRoleByControlled->setDestination(ccpAdd(ccp(offsetX,offsetY), m_pRoleByControlled->getPosition()));
                // 检测是否点中了NPC
                m_pChoosedNPC = NULL;
                CCArray* pNPCArray = Controller::getInstance()->getScene()->getSpriteLayer()->getNPCArray();
                for(int i = 0; i < pNPCArray->count();++i)
                {
                    CCPoint point = ccpAdd(m_MoveTouchPoint, Controller::getInstance()->getScene()->getScreenDatumPoint());
                    YYNPC * npc =(YYNPC *)pNPCArray->objectAtIndex(i);
                    if(npc->isTouched(point))
                    {
                        m_pChoosedNPC = npc;
                        if(fabs(m_pRoleByControlled->getPositionY() - npc->getPositionY()) < 100 && fabs(m_pRoleByControlled->getPositionX() - npc->getPositionX()) < 100)
                        {
                            const char *msg = CCString::createWithFormat(ASK_FUNCTION_FROM_NPC,npc->getNPCID().c_str())->getCString();
                            Controller::getInstance()->sendMessage(msg,true);
                        }
                        else
                        {
                            m_pRoleByControlled->setFindingTaskNPC(true);
                            m_pRoleByControlled->setTargetTile(npc->getCurrentTile());
                            ((YYScene *)getParent())->getPopUpUI()->setAcceptedTaskIndex(-1);
                            m_pRoleByControlled->setDestination(npc->getPosition());
                            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
                        }
                        break;
                    }
                }
                
                m_pRoleByControlled->setMoving(true);
                m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            }
                break;
            default:
                break;
        }
        m_n8TouchEventType = kTouchEventDefault;
    }

}

/**
 *手势完结
 */
void YYControlUILayer::gestureEnded()
{
    m_pTarget->clear(0, 0, 0, 0);
    if (p_2dPath.size() > 1)
    {
        RecognitionResult result = m_pGemertricRecognizer->recognize(p_2dPath);
        float similar = result.score;
        string astring = result.name;
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        CCActionInterval* scale = CCScaleTo::create(1.0, 1.5f);
        CCActionInterval* scale_back = CCScaleTo::create(1.0, 1.0f);
        CCSequence* scale_seq = CCSequence::create(scale, scale_back,NULL);
        scale_seq->setTag(TAG_GESTURE_ANIMATION);
        int i = -1;
        do
        {
            if ((astring == "ObliqueLine" || astring == "Line") && similar > 0.7)
            {//直线
                if(m_TouchPoint.getDistanceSq(m_StartPoint) > FINGER_MOVED_DISTANCE * FINGER_MOVED_DISTANCE)
                {
                    std::string str = getFilePath("100", kDirTypePic, kFileSuffixPng);
                    CCSprite* sprite = CCSprite::create(str.c_str());
                    addChild(sprite);
                    ((YYScene *)getParent())->getSpriteLayer()->getGestureSkillShadeArray()->addObject(sprite);
                    sprite->runAction(scale_seq);
                    CCPoint center =ccp((m_StartPoint.x + m_TouchPoint.x)/2,(m_StartPoint.y + m_TouchPoint.y)/2) ;
                    double a = atan2(m_TouchPoint.y - m_StartPoint.y , m_TouchPoint.x - m_StartPoint.x) * 180 / M_PI;
                    sprite->setPosition(center);
                    sprite->setRotation(-a);
                    i = 0;
                    break;
                }
            }
            //V
            if ((astring == "V" || astring == "LV" || astring == "VL" ) && similar > 0.7)
            {
                std::string str = getFilePath("v", kDirTypePic, kFileSuffixPng);
                CCSprite* sprite = CCSprite::create(str.c_str());
                sprite->setPosition(ccp(screenSize.width/2,screenSize.height/2));
                addChild(sprite);
                ((YYScene *)getParent())->getSpriteLayer()->getGestureSkillShadeArray()->addObject(sprite);
                sprite->runAction(scale_seq);
                i = 1;
                break;
            }
            //圆
            if ((astring == "Circle" || astring == "CircleRestore")&& similar > 0.7)
            {
                std::string str = getFilePath("circle", kDirTypePic, kFileSuffixPng);
                CCSprite* sprite = CCSprite::create(str.c_str());
                sprite->setPosition(ccp(screenSize.width/2,screenSize.height/2));
                addChild(sprite);
                ((YYScene *)getParent())->getSpriteLayer()->getGestureSkillShadeArray()->addObject(sprite);
                sprite->runAction(scale_seq);
                i = 2;
                break;
            }
            
        }while (0);
        
        if(i != -1)
        {
            if(m_fEnegyPercentage >= s_u8EnergyConsume[i]
               && (m_pRoleByControlled->getActionStatus() == kActionNormal
                   || m_pRoleByControlled->getActionStatus() == kActionAttack))
            {
                CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
                CCString * object = (CCString *)array->objectAtIndex(i);// 如果第一个手势技能是圆
                if (!object->isEqual(CCString::create("")))
                {
                    int skillIndex = ((CCString *)object)->intValue();
                    SkillData * skillData = m_pRoleByControlled->getSkillData(skillIndex);
                    m_pRoleByControlled->useSkill(skillData);
                    m_fEnegyPercentage -= s_u8EnergyConsume[i];
                }
            }
        }
    }
    p_2dPath.clear();
}
void YYControlUILayer::hideUIButton()
{
    setHideUIButton(true);
    m_bUIButtonMoving = true;
    ((CCSprite *)getChildByTag(TAG_HIDE_BUTTON))->setTexture(CCTextureCache::sharedTextureCache()->addImage(HIDE_UI_BUTTON_ICON));
    if(m_pUIButtonArray)
    {
        for (int i = 0 ; i < m_pUIButtonArray->count(); i++)
        {
            CCSprite * sprite = (CCSprite*)m_pUIButtonArray->objectAtIndex(i);
            sprite->stopAllActions();
            sprite->runAction(CCMoveTo::create(0.5, getUIHideButtonPosition()));
        }
    }
    
    YYSprite *loginAwardEffect = (YYSprite *)getChildByTag(TAG_LOGINAWARD_EFFECT);
    if (loginAwardEffect)
    {
        loginAwardEffect->setVisible(true);
    }
}

void YYControlUILayer::showUIButton()
{
    setHideUIButton(false);
    m_pChatBoxScrollView->setVisible(false);
    CCSprite * hideUIButtonKey = (CCSprite *)getChildByTag(TAG_HIDE_BUTTON);
    hideUIButtonKey->setTexture(CCTextureCache::sharedTextureCache()->addImage(UI_VISBLE_BUTTON_ICON));
    m_pUIButtonArray = CCArray::createWithCapacity(10);
    m_pUIButtonArray->retain();
    if (Controller::getInstance()->getSceneType() == NEWBIE_TASK_ZONE)
    {
        
    }
    for(int i = 0; i < 8; i++)
    {
        int picNameIndex = 345 + i;
        const char *str = CCString::createWithFormat("%d", picNameIndex)->getCString();
        std::string filePath = getFilePath(str, kDirTypeUI, kFileSuffixPng);
        CCSprite * sprite = CCSprite::create(filePath.c_str());
        sprite->setPosition(getUIHideButtonPosition());
        addChild(sprite);

        m_pUIButtonArray->addObject(sprite);
        CCMoveTo * move = NULL;
        if(i < 5)
        {
            move = CCMoveTo::create(0.5, ccpAdd(getUIHideButtonPosition(),
                                                ccp(- hideUIButtonKey->getContentSize().width / 2 - (20 + sprite->getContentSize().width / 2) - (15 + sprite->getContentSize().width) * i, 0)));
        }
        else
        {
            int n = i - 5;
            move = CCMoveTo::create(0.5, ccpAdd(getUIHideButtonPosition(),
                                                ccp(0, hideUIButtonKey->getContentSize().height / 2 + (20 + sprite->getContentSize().height / 2) + (15 + sprite->getContentSize().height) * n)));
            
        }
        if (move)
        {
            sprite->runAction(move);
        }
    }
    
    YYSprite *loginAwardEffect = (YYSprite *)getChildByTag(TAG_LOGINAWARD_EFFECT);
    if (loginAwardEffect)
    {
        loginAwardEffect->setVisible(false);
    }
}


/**
 *计算连击有效性
 */
void YYControlUILayer::calcComboAvailable()
{
    // 连击计数有效性验证
    if (m_nComboCounter > 0)
    {
        if (m_nComboCounter < 100)
        {
            m_nComboInterval = 60;
        }
        else if(m_nComboCounter < 150)
        {
            m_nComboInterval = 50;
        }
        else if(m_nComboCounter < 200)
        {
            m_nComboInterval = 40;
        }
        else if(m_nComboCounter < 250)
        {
            m_nComboInterval = 30;
        }
        else if(m_nComboCounter < 300)
        {
            m_nComboInterval = 20;
        }
        if (m_nComboIntervalCounter < m_nComboInterval)
        {
            m_nComboIntervalCounter ++;
        }
        else
        {
            m_nComboCounter = 0;
        }
    }
    // 计算虚拟攻击键的按键间隔
    if (m_bContinueAttack)
    {
        if (m_nKeyPressIntervalCounter <= KEYPRESS_INTERVAL)
        {
            m_nKeyPressIntervalCounter ++;
        }
        else
        {
            m_nKeyPressIntervalCounter = 0;
            m_pRoleByControlled->setAttackIndex(0);
            m_bContinueAttack = false;
        }
    }
}

void YYControlUILayer::cooling(int index,int time)
{
    m_pCDEffect[index]->setReverseDirection(true);
    m_pCDEffect[index]->setType( kCCProgressTimerTypeRadial );
    m_pCDEffect[index]->setPosition(getSkillShortcutKeyPosition(index));
    CCProgressFromTo * coolingAction= CCProgressFromTo::create(time, 100, 0);
    m_pCDEffect[index]->runAction(coolingAction);
}

void YYControlUILayer::setVirtualKeyPosition(CCPoint touchedPoint)
{
    if (isPointInCircle(touchedPoint,DIRECT_VIRTUAL_KEY_POSITION,DIRECT_AREA_RADIUS - ROCKER_RADIUS))
    {
        m_pVirtualRocker->setPosition(touchedPoint);
    }
    else
    {
        float offsetX = (DIRECT_AREA_RADIUS - ROCKER_RADIUS) * cos(atan2f(touchedPoint.y - DIRECT_VIRTUAL_KEY_POSITION.y,touchedPoint.x - DIRECT_VIRTUAL_KEY_POSITION.x));
        float offsetY = (DIRECT_AREA_RADIUS - ROCKER_RADIUS) * sin(atan2f(touchedPoint.y - DIRECT_VIRTUAL_KEY_POSITION.y,touchedPoint.x - DIRECT_VIRTUAL_KEY_POSITION.x));
        m_pVirtualRocker->setPosition(ccpAdd(DIRECT_VIRTUAL_KEY_POSITION, ccp(offsetX, offsetY)));
    }
}

void YYControlUILayer::returnInitPositon()
{
    m_pVirtualRocker->setPosition(DIRECT_VIRTUAL_KEY_POSITION);
}

void YYControlUILayer::updateCombo()
{
    if(m_bComboCountChange)
    {
        m_bComboCountChange = false;
        m_pComboLabel->setVisible(true);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        m_pComboNumber->setScale(0.8);
        CCActionInterval* scale = CCScaleTo::create(0.075, 2.0f);
        CCActionInterval* scale_reverse = CCScaleTo::create(0.075, 0.75f);
        CCActionInterval* scale_back = CCScaleTo::create(0.075, 1.0f);
        CCSequence* scale_seq = CCSequence::create(scale,scale_reverse, scale_back,NULL);
        m_pComboNumber->runAction(scale_seq);
        m_pComboLabel->setPosition(ccp(m_pComboLabel->getContentSize().width/2 ,size.height - 210));
        m_pComboNumber->setPosition(ccp(m_pComboLabel->getContentSize().width + m_pComboNumber->getContentSize().width/2 - 30,size.height - 210));
        
    }
}

/**
 * 计算连击
 */
void YYControlUILayer::calcCombo(int comboCount)
{
    m_nComboCounter += comboCount;
    m_nComboIntervalCounter = 0;
    setComboCountChange(true);
    if (m_nComboCounter > ((YYScene *)getParent())->getMaxComboCount())
    {
        ((YYScene *)getParent())->setMaxComboCount(((YYScene *)getParent())->getMaxComboCount() + 1);
    }
}

#pragma mark 更新

void YYControlUILayer::update(float f)
{
    touchHandle();
    calcComboAvailable();
    if (Controller::getInstance()->getSceneType() == MAIN_CITY
        || Controller::getInstance()->getSceneType() == NEWBIE_TASK_ZONE)
    {// 主城UI
        // 成就提示显示
        if (! Controller::getInstance()->getScene()->getPopUpUI()->getIsPopUpAchievementTip())
        {
            PopUpUI* popUpUI = Controller::getInstance()->getScene()->getPopUpUI();
            CCArray* arrTips = popUpUI->getAchieveTipsArray();
            if (arrTips->count() > 0)
            {
                AchieveTips* tips = (AchieveTips*)arrTips->objectAtIndex(0);
                popUpUI->drawAchievementTipUI(tips->getIdx(), "", 1, "", tips->getDesc(), tips->getIcon(), tips->getNum());
                popUpUI->setIsPopUpAchievementTip(true);
                arrTips->removeObjectAtIndex(0);
            }
        }
        Controller::getInstance()->getScene()->getPopUpUI()->updateAchievementShowTip(f);
        
        if (m_pUIButtonArray)
        {
            if (isHideUIButton())
            {
                for (int i = 0 ; i < m_pUIButtonArray->count();)
                {
                    CCSprite * sprite = (CCSprite*)m_pUIButtonArray->objectAtIndex(i);
                    if(sprite->getPositionX() == getUIHideButtonPosition().x && sprite->getPositionY() == getUIHideButtonPosition().y)
                    {
                        m_pUIButtonArray->removeObjectAtIndex(i);
                        sprite->removeFromParent();
                    }
                    else
                    {
                        i++;
                    }
                }
                if (m_pUIButtonArray->count() == 0)
                {
                    CC_SAFE_RELEASE_NULL(m_pUIButtonArray);
                    m_bUIButtonMoving = false;
                    m_pChatBoxScrollView->setVisible(true);
                }
            }
            else
            {
                uint16_t nMailNum = ((YYScene *)getParent())->getPopUpUI()->getMailNum();
                if (nMailNum > 0 && m_pUIButtonArray->count() >= 8)
                {
                    CCSprite * pSpriteMail = (CCSprite*)m_pUIButtonArray->objectAtIndex(7);
                    if (pSpriteMail)
                    {
                        pSpriteMail->removeAllChildren();
                        CCSprite *pSpriteTip = (CCSprite *)((YYScene *)getParent())->getPopUpUI()->getMailTipNode(nMailNum);
                        pSpriteTip->setPosition(ccp(pSpriteMail->getContentSize().width - 7, pSpriteMail->getContentSize().height - 11));
                        pSpriteMail->addChild(pSpriteTip);
                    }
                }
            }
        }
        m_pAutoFindTips->setVisible(m_pRoleByControlled->getRunningStatus() == kRunningStatusAuto);
        if(m_pSpecialEffectArray)
        {
            for(int i = 0; i < m_pSpecialEffectArray->count();)
            {
                YYSprite* pSprite = (YYSprite*)m_pSpecialEffectArray->objectAtIndex(i);
                if((pSprite->isActionOver() && !pSprite->isRepeatAction()) || pSprite->isDeleted())
                {
                    this->removeChild(pSprite);
                    m_pSpecialEffectArray->removeObject(pSprite);
                }
                else
                {
                    pSprite->updateAnimation();
                    i++;
                }
            }
        }
        updateExpBar(m_pRoleByControlled->getCurrentExp(), m_pRoleByControlled->getLevelUpExp(), ccp(0,0));
        if(getChildByTag(TAG_SHORTCUT_EFFECT))
        {
            ((YYSprite *)getChildByTag(TAG_SHORTCUT_EFFECT))->update();
        }
        if(getChildByTag(TAG_LOGINAWARD_EFFECT))
        {
            if(!Controller::getInstance()->getScene()->getPopUpUI()->isGetLoginAward())
                removeChildByTag(TAG_LOGINAWARD_EFFECT);
            else
                ((YYSprite *)getChildByTag(TAG_LOGINAWARD_EFFECT))->update();
        }
    }
    else
    {
        // 成就提示消失
        Controller::getInstance()->getScene()->getPopUpUI()->getAchieveTipsArray()->removeAllObjects();
        Controller::getInstance()->getScene()->getPopUpUI()->resetAchievementShowTip();
        // 战斗UI
        CCSprite *button = (CCSprite *)getChildByTag(TAG_RETURN_BUTTON);
        if (button)
        {
            int counter = Controller::getInstance()->getScene()->getFightResultDelayCounter();
            if (counter >= 0 && counter < 40)
            {
                button->setVisible(false);
            }
            else
            {
                button->setVisible(true);
            }
        }
        const char * temp = CCString::createWithFormat(";%d",m_nComboCounter)->getCString();
        m_pComboNumber->setString(temp);
        m_pComboLabel->setVisible(m_nComboCounter > 0);
        m_pComboNumber->setVisible(m_nComboCounter > 0);
        if(m_pComboNumber->isVisible())
        {
            updateCombo();
        }
        if (m_bHoldDirectionKey)
        {
            m_pVirtualRocker->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_ROCKER_PRESS));
            m_pVirtualRockerFrame->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_DIR_BG_PRESS));
            setVirtualKeyPosition(getCurrentTouchPoint());
        }
        else
        {
            m_pVirtualRocker->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_ROCKER_NOMAL));
            m_pVirtualRockerFrame->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_DIR_BG_NOMAL));
            returnInitPositon();
        }
        
        if(m_pScrollView && m_pScrollView->getActionByTag(TAG_SHOW_TASK_ACTION) == NULL)
        {
            if(!m_bIsShowTaskInfo)
            {
                m_pScrollView->setVisible(false);
            }
        }
        updateTrialFightTime();
        updateBossStatusBar();
        if(Controller::getInstance()->getSceneType() == DUNGEONS)
        {
            if (Controller::getInstance()->isTrial())
            {
                if(m_bPlayTrialTime)
                {
                    updateTrialTime();
                }
            }
            else
            {
                updateDungeonsTime();
            }
        }
    }
    //试炼
    if (Controller::getInstance()->isTrial())
    {
        updateTrialStatusBar();
    }
    updatePlayerStatusBar();
    if (Controller::getInstance()->isTutorial())// 新手引导
    {
        YYGuide *guide = (YYGuide *)getChildByTag(TAG_GUIDE);
        if (guide)
        {
            guide->update();
        }
    }
    if(m_nHelpDuration > 0)
    {
        YYGuide *guide = (YYGuide *)getChildByTag(TAG_HELP);
        if (guide)
        {
            guide->update();
        }
        m_nHelpDuration--;
        if (m_nHelpDuration == 0)
        {
            removeChildByTag(TAG_HELP);
        }
    }
}

void YYControlUILayer::updatePlayerStatusBar()
{
    if (Controller::getInstance()->getSceneType() == DUNGEONS
        || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {
        if(m_fEnegyPercentage <= 97.8)
        {
            m_fEnegyPercentage += 0.2;
        }
        m_pEnergyBar->setPercentage(m_fEnegyPercentage);
        std::string pGesture1[] = {GESTURE_LINE2, GESTURE_V2, GESTURE_CIRCLE2};
        std::string pGesture2[] = {GESTURE_LINE1, GESTURE_V1, GESTURE_CIRCLE1};
        for (int i = 0; i < 3; i++)
        {
            if (m_pPlayerAngerStatusBar->getChildByTag(101 + i))
            {
                CCSprite *pSprite = (CCSprite *)m_pPlayerAngerStatusBar->getChildByTag(101 + i);
                pSprite->setTexture(m_fEnegyPercentage >= s_u8EnergyConsume[i]
                                    ? CCTextureCache::sharedTextureCache()->addImage(pGesture1[i].c_str())
                                    : CCTextureCache::sharedTextureCache()->addImage(pGesture2[i].c_str()));
            }
        }
    }
    if (m_nPlayerHP != m_pRoleByControlled->getCurrentHP())
    {
        m_nPlayerHP = m_pRoleByControlled->getCurrentHP();
        m_pPlayerBloodBar->setPercentage(m_nPlayerHP * 100 / (float)m_pRoleByControlled->getSpriteAttribute()->getMaxHP());
        m_pHPLabel->setString(CCString::createWithFormat("%d/%d",m_nPlayerHP,m_pRoleByControlled->getSpriteAttribute()->getMaxHP())->getCString());
    }
    if (m_nPlayerMP != m_pRoleByControlled->getCurrentMP())
    {
        m_nPlayerMP = m_pRoleByControlled->getCurrentMP();
        m_pPlayerManaBar->setPercentage(m_nPlayerMP * 100 / (float)m_pRoleByControlled->getSpriteAttribute()->getMaxMP());
        m_pMPLabel->setString(CCString::createWithFormat("%d/%d",m_nPlayerMP,m_pRoleByControlled->getSpriteAttribute()->getMaxMP())->getCString());
        if(Controller::getInstance()->getSceneType() == DUNGEONS
           || Controller::getInstance()->getSceneType() == WAR_ZONE)
        {// 战斗场景魔法值有变化时判断技能图标是否需要替换
            setSkillIcon();
        }
    }
    if (m_pRoleByControlled && m_pLevelLabel)
    {
        m_pLevelLabel->setString(CCString::createWithFormat("%d",m_pRoleByControlled->getLevel())->getCString());
    }
}

/**
 * 更新boss状态栏
 */

void YYControlUILayer::updateBossStatusBar()
{
    if (m_pBoss)
    {
        if (m_nBossHP != m_pBoss->getCurrentHP() || m_nBossID != m_pBoss->getSpriteID())
        {
            if (m_nBossHP == -1)
            {
                CCSize winSize = CCDirector::sharedDirector()->getWinSize();
                // boss头像背景框
                m_pBossPortraitBGFrame = CCSprite::create(BOSS_PORTRAIT_BG_FRAME);
                m_pBossPortraitBGFrame->setPosition(ccp(winSize.width,winSize.height));
                m_pBossPortraitBGFrame->setAnchorPoint(ccp(1, 1));
                addChild(m_pBossPortraitBGFrame,2);// 头像及背景框覆盖血槽
                // boss头像
                std::string filePath = getFilePath(m_pBoss->getSpriteData()->getPortraitFileName(), kDirTypeIcon, kFileSuffixPng);
                m_pBossPortrait = CCSprite::create(filePath.c_str());
                m_pBossPortrait->setPosition(ccp(m_pBossPortraitBGFrame->getPositionX() - m_pBossPortraitBGFrame->getContentSize().width / 2 + 3,m_pBossPortraitBGFrame->getPositionY() - m_pBossPortraitBGFrame->getContentSize().height / 2 + 5));
               addChild(m_pBossPortrait,2);

                // boss 血槽
                m_pBossStatusBar = CCSprite::create(BOSS_BLOOD_BAR);
                m_pBossStatusBar->setPosition(ccp(winSize.width - m_pBossStatusBar->getContentSize().width / 2 - m_pBossPortraitBGFrame->getContentSize().width + 40,winSize.height - m_pBossPortraitBGFrame->getContentSize().height / 2 + 5));
               // m_pBossStatusBar->setPosition(ccp(winSize.width - 275,winSize.height - 91));
                addChild(m_pBossStatusBar);
                
                // boss血条
                if (m_pBoss->getCurrentHP() > 0)
                {
                    int layerNum = m_pBoss->getCurrentHP() * m_pBoss->getSpriteData()->getBloodBarLayerNum() / m_pBoss->getSpriteAttribute()->getMaxHP() ;//当前层数
                    float tempHP = (float)m_pBoss->getSpriteAttribute()->getMaxHP() / m_pBoss->getSpriteData()->getBloodBarLayerNum();// 每层血量
                    float currentLayerHP = m_pBoss->getCurrentHP();
                    if (currentLayerHP != tempHP * layerNum)
                    {
                        layerNum++;
                        while (currentLayerHP > tempHP)
                        {
                            currentLayerHP -= tempHP;
                        }
                        if (layerNum > 1) // 两层以上增加底层血条的显示
                        {
                            CCString * string = CCString::createWithFormat(BOSS_BLOOD,layerNum - 2);
                            if (!m_pBossBGBloodBar)
                            {
                                m_pBossBGBloodBar = CCSprite::create(string->getCString());
                                m_pBossBGBloodBar->setPosition(m_pBossStatusBar->getPosition());
                                addChild(m_pBossBGBloodBar);
                            }
                            else
                            {
                                m_pBossBGBloodBar->setTexture(CCTextureCache::sharedTextureCache()->addImage(string->getCString()));
                            }
                        }
                    }
                    m_n8BossCurrentHPLayerNum = layerNum;
                    CCString * string = CCString::createWithFormat(BOSS_BLOOD,layerNum - 1);
                    m_pBossBloodBar = CCProgressTimer::create(CCSprite::create(string->getCString()));
                    m_pBossBloodBar->setPosition(m_pBossStatusBar->getPosition());
                    m_pBossBloodBar->setMidpoint(ccp(1,0));
                    m_pBossBloodBar->setBarChangeRate(ccp(1, 0));
                    m_pBossBloodBar->setType(kCCProgressTimerTypeBar);
                    addChild(m_pBossBloodBar,1);
                    m_pBossBloodBar->setPercentage(currentLayerHP * 100 / tempHP);
                    
                    if(Controller::getInstance()->getDungeonType() == kWorldBoss) // 活动世界Boss 设置剩余血量
                    {
                        YYSpriteLayer* spriteLayer =  (static_cast<YYScene*>(getParent()))->getSpriteLayer();
                        if (spriteLayer)
                        {
                            CCLOG("ramian HP %f",m_fWorldBossRemainHP);
                            m_pBossBloodBar->setPercentage(m_fWorldBossRemainHP);
                            m_pBoss->setCurrentHP(m_fWorldBossRemainHP/100 * m_nWorldBossAmountHP);
                        }
                    }
                }
                
            }
            else
            {
                if (m_pBoss->getCurrentHP() > 0)
                {
                    uint32_t nMaxHP = 1;
                    if(Controller::getInstance()->getDungeonType() == kWorldBoss)
                    {
                        nMaxHP = m_nWorldBossAmountHP;
                    }
                    else
                    {
                        nMaxHP = m_pBoss->getSpriteAttribute()->getMaxHP();
                    }
                        int layerNum = m_pBoss->getCurrentHP() * m_pBoss->getSpriteData()->getBloodBarLayerNum() / nMaxHP;//当前层数
                        float tempHP = (float)nMaxHP / m_pBoss->getSpriteData()->getBloodBarLayerNum();// 每层血量
                        float currentLayerHP = m_pBoss->getCurrentHP();
                        if (currentLayerHP != tempHP * layerNum)
                        {
                            layerNum++;
                            while (currentLayerHP > tempHP)//计算当前层血值
                            {
                                currentLayerHP -= tempHP;
                            }
                            if (layerNum > 1) // 两层以上增加底层血条的显示
                            {
                                CCString * string = CCString::createWithFormat(BOSS_BLOOD,layerNum - 2);
                                if (!m_pBossBGBloodBar)
                                {
                                    m_pBossBGBloodBar = CCSprite::create(string->getCString());
                                    m_pBossBGBloodBar->setPosition(m_pBossBloodBar->getPosition());
                                    addChild(m_pBossBGBloodBar);
                                }
                                else
                                {
                                    m_pBossBGBloodBar->setTexture(CCTextureCache::sharedTextureCache()->addImage(string->getCString()));
                                }
                            }
                            else if(m_pBossBGBloodBar)
                            {
                                removeChild(m_pBossBGBloodBar);
                                m_pBossBGBloodBar = NULL;
                            }
                        }
                        if(layerNum != m_n8BossCurrentHPLayerNum)
                        {
                            m_n8BossCurrentHPLayerNum = layerNum;
                            CCString * string = CCString::createWithFormat(BOSS_BLOOD,layerNum - 1);
                            m_pBossBloodBar->setSprite(CCSprite::create(string->getCString()));
                        }
                        m_pBossBloodBar->setPercentage(currentLayerHP * 100 / tempHP);
                        
                        if (m_nBossID != m_pBoss->getSpriteID())
                        {
                            std::string filePath = getFilePath(m_pBoss->getSpriteData()->getPortraitFileName(), kDirTypePic, kFileSuffixPng);
                            m_pBossPortrait->setTexture(CCTextureCache::sharedTextureCache()->addImage(filePath.c_str()));
                        }
                    }
                else if(m_pBossBloodBar)
                {
                    m_pBossBloodBar->setPercentage(0);
                }
                
                // 活动世界Boss BOSS 死亡
                // CCLOG("BOSS cur HP %d",m_pBoss->getCurrentHP());
                
                if(Controller::getInstance()->getDungeonType() == kWorldBoss && m_bWorldBossEnableDead)
                {
                    if (m_pBoss->getCurrentHP() < 300)
                    {
                        Controller::getInstance()->sendMessage(ASK_WORLD_BOSS_KILL,true);
                        m_bWorldBossEnableDead = false;
                    }
                }
            }
            m_nBossHP = m_pBoss->getCurrentHP();
            m_nBossID = m_pBoss->getSpriteID();
        }
        m_pBossPortraitBGFrame->setVisible(true);
        m_pBossPortrait->setVisible(true);
        m_pBossStatusBar->setVisible(true);
        m_pBossBloodBar->setVisible(true);
        if (m_pBossBGBloodBar)
        {
            m_pBossBGBloodBar->setVisible(true);
        }
    }
    else if(Controller::getInstance()->isTrial()) // 试炼隐藏boss显示
    {
        if(m_pBossPortraitBGFrame)
        {
            m_pBossPortraitBGFrame->setVisible(true);
        }
        if(m_pBossPortrait)
        {
            m_pBossPortrait->setVisible(true);
        }
        if(m_pBossStatusBar)
        {
            m_pBossStatusBar->setVisible(true);
        }
        if (m_pBossBloodBar)
        {
            m_pBossBloodBar->setVisible(true);
        }
        if (m_pBossBGBloodBar)
        {
            m_pBossBGBloodBar->setVisible(true);
        }
    }
    else
    {
        if(m_pBossPortraitBGFrame)
        {
            m_pBossPortraitBGFrame->setVisible(false);
        }
        if(m_pBossPortrait)
        {
            m_pBossPortrait->setVisible(false);
        }
        if(m_pBossStatusBar)
        {
            m_pBossStatusBar->setVisible(false);
        }
        if (m_pBossBloodBar)
        {
            m_pBossBloodBar->setVisible(false);
        }
        if (m_pBossBGBloodBar)
        {
            m_pBossBGBloodBar->setVisible(false);
        }
    }
}

void YYControlUILayer::updateExpBar(int currentPoint,int maxPoint,CCPoint position)
{
    if (m_nCurrentExp == currentPoint)
    {
        return;
    }
    m_nCurrentExp = currentPoint;
    m_pExpSpriteBatch->removeAllChildren();
    int width = currentPoint * ( m_nExpMaxWidth) / maxPoint;
    int texWidth = m_pExpSpriteBatch->getTexture()->getContentSize().width;
    int texHeight = m_pExpSpriteBatch->getTexture()->getContentSize().height;
    if (width < OBLIGATE_WIDTH)
    {// 宽度小于圆角部分
        CCSprite * sprite = CCSprite::createWithTexture(m_pExpSpriteBatch->getTexture(),CCRectMake(0, 0, width, texHeight));
        sprite->setAnchorPoint(ccp(0,0.5));
        sprite->setPosition(ccp(position.x - m_nExpMaxWidth / 2 + 2,position.y));
        m_pExpSpriteBatch->addChild(sprite);
    }
    else
    {
        // 血条左端的圆角
        CCSprite * sprite = CCSprite::createWithTexture(m_pExpSpriteBatch->getTexture(),CCRectMake(0, 0, OBLIGATE_WIDTH, texHeight));
        sprite->setAnchorPoint(ccp(0,0.5));
        sprite->setPosition(ccp(position.x - m_nExpMaxWidth / 2 + 2,position.y));
        m_pExpSpriteBatch->addChild(sprite);
        if (width < OBLIGATE_WIDTH * 2)
        {
            sprite = CCSprite::createWithTexture(m_pExpSpriteBatch->getTexture(),CCRectMake(OBLIGATE_WIDTH, 0, width - OBLIGATE_WIDTH, texHeight));
            sprite->setAnchorPoint(ccp(0,0.5));
            sprite->setPosition(ccp(position.x + OBLIGATE_WIDTH - m_nExpMaxWidth / 2 + 2,position.y));
            m_pExpSpriteBatch->addChild(sprite);
            return;
        }
        else
        {// 血条右端的圆角
            CCSprite * sprite = CCSprite::createWithTexture(m_pExpSpriteBatch->getTexture(),CCRectMake(0,0, OBLIGATE_WIDTH, texHeight));
            sprite->setScaleX(-1);
            sprite->setPosition(ccp(position.x + width - m_nExpMaxWidth / 2 - OBLIGATE_WIDTH / 2,position.y));
            m_pExpSpriteBatch->addChild(sprite);
            int num = (width - OBLIGATE_WIDTH * 2) / (texWidth - OBLIGATE_WIDTH);
            int remainder = (width - OBLIGATE_WIDTH * 2) - num * (texWidth - OBLIGATE_WIDTH);
            for (int i = 0; i < num; i++)
            {
                CCSprite * sprite = CCSprite::createWithTexture(m_pExpSpriteBatch->getTexture(),CCRectMake(OBLIGATE_WIDTH, 0, texWidth - OBLIGATE_WIDTH, texHeight));
                sprite->setAnchorPoint(ccp(0,0.5));
                sprite->setPosition(ccp(position.x + 2 + i * (texWidth - OBLIGATE_WIDTH) + OBLIGATE_WIDTH - m_nExpMaxWidth / 2,position.y));
                m_pExpSpriteBatch->addChild(sprite);
            }
            if(remainder > 0)
            {
                CCSprite * sprite = CCSprite::createWithTexture(m_pExpSpriteBatch->getTexture(),CCRectMake(OBLIGATE_WIDTH, 0, remainder, texHeight));
                sprite->setAnchorPoint(ccp(0,0.5));
                sprite->setPosition(ccp(position.x + 2 + num * (texWidth - OBLIGATE_WIDTH)  + OBLIGATE_WIDTH - m_nExpMaxWidth / 2,position.y));
                m_pExpSpriteBatch->addChild(sprite);
            }
        }
    }
    
    // 添加经验具体数值
    CCSprite* pExperienceFrontSprite = static_cast<CCSprite*>(this->getChildByTag(TAG_EXPERIENCE_CUBE));
    if (pExperienceFrontSprite)
    {
        CCPoint cp = pExperienceFrontSprite->getPosition();
        char ch[50] = {0};
        sprintf(ch, "%d / %d", currentPoint, maxPoint);
        CCLabelTTF* textExp = dynamic_cast<CCLabelTTF*>(this->getChildByTag(TAG_EXPERIENCE_CUBE));
        if(!textExp)
        {
           textExp =  CCLabelTTF::create(ch, DEFAULT_FONT_NAME, 15);
           float offsetH =  pExperienceFrontSprite->getContentSize().height * 0.2f;
           addChild(textExp,5,TAG_EXPERIENCE_TEXT);
           textExp->setPosition(ccpSub(cp, ccp(0,offsetH)));
           textExp->setColor(ccWHITE);

        }
        textExp->setString(ch);
    }
}

#pragma mark 更新副本用时

void YYControlUILayer::updateDungeonsTime()
{
    int currentTime = Controller::getInstance()->getCurrentSecond();
    int second = Controller::getInstance()->getScene()->getStageDuration()+ currentTime - Controller::getInstance()->getScene()->getCurrentTime();
    if (m_nCurrentTime != second)
    {
        const char * temp = NULL;
        if (second % 60 > 9)
        {
            if (second / 60 > 9)
            {
                temp = CCString::createWithFormat("%d:%d:%d",second / 3600,second / 60,second % 60)->getCString();
            }
            else
            {
                temp = CCString::createWithFormat("%d:0%d:%d",second / 3600,second / 60,second % 60)->getCString();
            }
        }
        else
        {
            if (second / 60 > 9)
            {
                temp = CCString::createWithFormat("%d:%d:0%d",second / 3600,second / 60,second % 60)->getCString();
            }
            else
            {
                temp = CCString::createWithFormat("%d:0%d:0%d",second / 3600,second / 60,second % 60)->getCString();
            }
        }
        if (m_nCurrentTime == -1)
        {
            std::string filePath = getFilePath("360", kDirTypePic, kFileSuffixPng);
            m_pTimeLabelAtlas = CCLabelAtlas::create(temp,filePath.c_str(),27,35,'0');// 按ascii排列顺序排在9后面的是:;
            m_pTimeLabelAtlas->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 30));
            m_pTimeLabelAtlas->setAnchorPoint(ccp(0.5, 0.5));
            addChild(m_pTimeLabelAtlas);
        }
        else
        {
            m_pTimeLabelAtlas->setString(temp);
        }
        m_nCurrentTime = second;
    }
}

void YYControlUILayer::updateRoleWealth()
{
    if(m_pJewelLabel)
    {
        m_pJewelLabel->setString(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel())->getCString());
    }
    if(m_pMoneyLabel)
    {
        m_pMoneyLabel->setString(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getGold())->getCString());
    }
    if(m_pPlayerPhysicalStrengthPoint)
    {
        ((CCLabelTTF*)m_pPlayerPhysicalStrengthPoint->getChildByTag(1))->setString(CCString::createWithFormat("%d/100",m_pRoleByControlled->getSpriteAttribute()->getPhysicalStrengthPoint())->getCString());
    }
    if(m_pPlayerFighttingPoint)
    {
        ((CCLabelAtlas*)m_pPlayerFighttingPoint->getChildByTag(1))->setString(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getFightingCapacity())->getCString());
    }
}

void YYControlUILayer::setShortcutSkillIcon(cocos2d::CCArray *array)
{
    // 技能图标CD特效
    if (array)
    {
        for (int i = 0; i < array->count(); i++)
        {
            CCString * skillID = (CCString *)array->objectAtIndex(i);
            if(skillID && !skillID->isEqual(CCString::create("")))
            {
                SkillData * skillData = (SkillData *)GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(), skillID->intValue());
                if(skillData)
                {
                    string path = getFilePath((m_pRoleByControlled->getCurrentMP() > skillData->getConsumePoint() ? skillData->getSkillIcon() :skillData->getSkillUnableIcon()), kDirTypeIcon, kFileSuffixPng);
                    m_pSkillIcon[i] = CCSprite::create(path.c_str());
                    m_pSkillIcon[i]->setPosition(getSkillShortcutKeyPosition(i));
                    addChild(m_pSkillIcon[i]);
                    std::string filePath = getFilePath("1", kDirTypePic, kFileSuffixPng);
                    m_pCDEffect[i] = CCProgressTimer::create(CCSprite::create(filePath.c_str()));
                    addChild(m_pCDEffect[i],1);
                }
            }
            else
            {// 技能快捷栏空缺技能
                if (m_pSkillIconBGFrame[i])
                {
                    m_pSkillIconBGFrame[i]->setTexture(CCTextureCache::sharedTextureCache()->addImage(VIRTUAL_SKILL_BG_NOMAL));
                }
            }
        }
    }
}

void YYControlUILayer::setSkillIcon()
{
    CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
    if (array)
    {
        for (int i = 0; i < 3; i++)
        {
            CCString * skillID = (CCString *)array->objectAtIndex(i);
            if (skillID && !skillID->isEqual(CCString::create("")))
            {
                SkillData * skillData = (SkillData *)GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(), skillID->intValue());
                if (skillData)
                {
                    string path = getFilePath((m_pRoleByControlled->getCurrentMP() > skillData->getConsumePoint() ? skillData->getSkillIcon() :skillData->getSkillUnableIcon()), kDirTypeIcon, kFileSuffixPng);
                    m_pSkillIcon[i]->setTexture(CCTextureCache::sharedTextureCache()->addImage(path.c_str()));
                }
            }
        }
    }
}

void YYControlUILayer::clearKeyState()
{
    m_bHoldDirectionKey = false;
    m_bHoldAttackKey = false;
    m_pRoleByControlled->setMoving(false);
    m_pRoleByControlled->setRunningStatus(kRunningStatusDefault);
    m_pTouchDictionary->removeAllObjects();
    if (m_pRoleByControlled->getActionStatus() == kActionNormal)
    {
        m_pRoleByControlled->switchAction(ACTION_STAND0);
    }
    if (Controller::getInstance()->getSceneType() == DUNGEONS
        || Controller::getInstance()->getSceneType() == WAR_ZONE) // 战斗区域让 方向键还原
    {
        returnInitPositon();
    }
}

#pragma mark UI隐藏或取消隐藏

void YYControlUILayer::setUIVisible(bool isVisible)
{
    // 头像
    m_pPlayerAngerStatusBar->setVisible(isVisible);
    m_pPortraitBGFrame->setVisible(isVisible);
    m_pPlayerPortrait->setVisible(isVisible);
    
    // vip
    m_pVipBG->setVisible(isVisible);
    
    m_pPlayerStatusBar->setVisible(isVisible);// 状态栏（能量槽、血槽、法力槽）
    m_pPlayerBloodBar->setVisible(isVisible);// 血条
    m_pPlayerManaBar->setVisible(isVisible);// 蓝条
    
    // 等级
    m_pLevelLabelFrame->setVisible(isVisible);
    m_pHPLabel->setVisible(isVisible);
    m_pMPLabel->setVisible(isVisible);
    if (Controller::getInstance()->getSceneType() == DUNGEONS
        || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {
        // 手势
        m_pPlayerAngerStatusBar->setVisible(isVisible);
        m_pEnergyBar->setVisible(isVisible);// 能量条
        // 手势轨迹
        m_pTarget->setVisible(isVisible);
        m_pBrush->setVisible(isVisible);
        
        // 虚拟方向键
        m_pVirtualRockerFrame->setVisible(isVisible);
        
        m_pVirtualRocker->setVisible(isVisible);
        
        // 虚拟攻击键
        m_pVirtualFireKey->setVisible(isVisible);
        
        // 技能快捷键
        for (int i = 0; i < 3; i++)
        {
            if (m_pSkillIconBGFrame[i])
            {
                m_pSkillIconBGFrame[i]->setVisible(isVisible);
            }
            if (m_pSkillIcon[i])
            {
                m_pSkillIcon[i]->setVisible(isVisible);
            }
            if (m_pCDEffect[i])
            {
                m_pCDEffect[i]->setVisible(isVisible);
            }
        }
        
        // 连击数
        m_pComboLabel->setVisible(isVisible);
        m_pComboNumber->setVisible(isVisible);
        
        // 蓄满能量后的特效
        CCSprite *energyEffect = (CCSprite *)getChildByTag(TAG_ENERGY_EFFECT);
        if (energyEffect)
        {
            energyEffect->setVisible(isVisible);
        }
        
        // 返回
        CCSprite *button = (CCSprite *)getChildByTag(TAG_RETURN_BUTTON);
        if (button)
        {
            button->setVisible(isVisible);
        }
        
        CCLabelTTF *label = (CCLabelTTF *)getChildByTag(TAG_RETURN_CITY_LABEL);
        if (label)
        {
            label->setVisible(isVisible);
        }
        
        // 显示任务按键
        button = (CCSprite *)getChildByTag(TAG_SHOW_TASK_BUTTON);
        if (button)
        {
            button->setVisible(isVisible);
        }
        
        if(m_pBossPortraitBGFrame)
        {
            m_pBossPortraitBGFrame->setVisible(isVisible);
        }
        if(m_pBossPortrait)
        {
            m_pBossPortrait->setVisible(isVisible);
        }
        if(m_pBossStatusBar)
        {
            m_pBossStatusBar->setVisible(isVisible);
        }
        if(m_pBossBloodBar)
        {
            m_pBossBloodBar->setVisible(isVisible);
        }
    }
    else
    {//非战斗区域
        CCSprite *sprite = (CCSprite *)getChildByTag(TAG_HIDE_BUTTON);
        if (sprite)
        {
            sprite->setVisible(isVisible);
        }
        // 任务
        if (m_pSpriteTask)
        {
            m_pSpriteTask->setVisible(isVisible);
        }
        // 已接任务数
        m_pTaskNumberLabel->setVisible(isVisible);
        // 商城
        if (m_pSpriteShop)
        {
            m_pSpriteShop->setVisible(isVisible);
        }
        // 试炼
        if (m_pSpriteTrain)
        {
            m_pSpriteTrain->setVisible(isVisible);
        }
        // 活动
        if (m_pSpritePlayTable)
        {
            m_pSpritePlayTable->setVisible(isVisible);
        }
        // 奖励
        if (m_pSpriteLoginAward)
        {
            m_pSpriteLoginAward->setVisible(isVisible);
        }
        YYSprite *loginAwardEffect = (YYSprite *)getChildByTag(TAG_LOGINAWARD_EFFECT);
        if (loginAwardEffect)
        {
            loginAwardEffect->setVisible(isVisible);
        }
        // 签到
        if (m_pSpriteSignUp)
        {
            m_pSpriteSignUp->setVisible(isVisible);
        }
        // 活跃度
        if (m_pSpriteActivity)
        {
            m_pSpriteActivity->setVisible(isVisible);
        }
        // 聊天
        if (m_pSpriteChat)
        {
            m_pSpriteChat->setVisible(isVisible);
        }
        // 充值
        if (m_pSpriteCharge)
        {
            m_pSpriteCharge->setVisible(isVisible);
        }
        // 公告
        if (m_pSpriteNotice)
        {
            m_pSpriteNotice->setVisible(isVisible);
        }
        // 兑换码
        if (m_pSpriteConvertCode)
        {
            m_pSpriteConvertCode->setVisible(isVisible);
        }
        // 退出
        CCSprite *button = (CCSprite *)getChildByTag(TAG_EXIT);
        if (button)
        {
            button->setVisible(isVisible);
        }
        
        // 经验条
        CCSprite *pExperience = (CCSprite *)getChildByTag(TAG_EXPERIENCE_ICON);
        if (pExperience)
        {
            pExperience->setVisible(isVisible);
        }
        CCSprite *pExperienceCube = (CCSprite *)getChildByTag(TAG_EXPERIENCE_CUBE);
        if (pExperienceCube)
        {
            pExperienceCube->setVisible(isVisible);
        }
        CCLabelTTF* pExperienceText = (CCLabelTTF *)getChildByTag(TAG_EXPERIENCE_TEXT);
        if(pExperienceText)
        {
            pExperienceText->setVisible(isVisible);
        }
        m_pChatBoxScrollView->setVisible(isVisible);
        m_pExpSpriteBatch->setVisible(isVisible);
        m_pGuideButton->setVisible(isVisible);// 右上角任务头像信息
        YYSprite *shortcutEffect = (YYSprite *)getChildByTag(TAG_SHORTCUT_EFFECT);
        if (shortcutEffect)
        {
            shortcutEffect->setVisible(isVisible);
        }
        m_pAutoFindTips->setVisible(isVisible);
        m_pPlayerJewel->setVisible(isVisible);
        m_pPlayerMoney->setVisible(isVisible);
        m_pPlayerPhysicalStrengthPoint->setVisible(isVisible);
        m_pPlayerFighttingPoint->setVisible(isVisible);

        m_pPlayerAngerStatusBar->setVisible(false);
    }
}

void YYControlUILayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //     CCLog("editBox %p DidBegin !", editBox);
}

void YYControlUILayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //     CCLog("editBox %p DidEnd !", editBox);
}

void YYControlUILayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    //     CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void YYControlUILayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    //     CCLog("editBox %p was returned !");
}


#pragma mark 对话

void YYControlUILayer::say(const char *name, const char *content, const char *picFileName)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCScale9Sprite *talkFrame = CCScale9Sprite::create(DIALOG_BOX);
    
    talkFrame->setPosition(ccp(winSize.width / 2, talkFrame->getContentSize().height / 2));
    addChild(talkFrame,2);
    talkFrame->setContentSize(CCSizeMake(winSize.width,talkFrame->getContentSize().height));
    talkFrame->setTag(TAG_TALK_BG_FRAME);
    m_bTalk = true;
    
    // 头像
    std::string filePath;
    if (picFileName && (std::string(picFileName).compare("") != 0))
    {
        filePath = getFilePath(picFileName, kDirTypeIcon, kFileSuffixPng);
    }
    else if (std::string(name).compare("-1") == 0)
    {
        int index = Controller::getInstance()->getProfessionIndex();
        if (index == 1)
        {
            filePath = PLAYER_PORTRAIT1;
        }
        else if(index == 2)
        {
            filePath = PLAYER_PORTRAIT2;
        }
        else if(index == 3)
        {
            filePath = PLAYER_PORTRAIT3;
        }
    }
    CCSprite * portrait = CCSprite::create(filePath.c_str());
    int lineWidth = talkFrame->getContentSize().width - portrait->getContentSize().width - 50;
    int fontSize = 30;
    m_pTalkArray->addObjectsFromArray(split_string(content, lineWidth, fontSize));
    assert(m_pTalkArray != NULL);
    // 名字
    CCLabelTTF * nameLabel = NULL;
    if (std::string(name).compare("-1") == 0 || std::string(name).compare("") == 0)
    {
        nameLabel = CCLabelTTF::create(m_pRoleByControlled->getSpriteName()->getCString(), DEFAULT_FONT_NAME, 40);
    }
    else if (name)
    {
        nameLabel = CCLabelTTF::create(name, DEFAULT_FONT_NAME, 40);
    }
    nameLabel->setPosition(ccp(portrait->getContentSize().width + 10, talkFrame->getContentSize().height - 35));
    nameLabel->setAnchorPoint(ccp(0,0.5));
	ccColor3B tc1 = {10,255,30};
    nameLabel->setColor(tc1);
    addChild(nameLabel,2);
    nameLabel->setTag(TAG_NAME_LABEL);
    
    if (portrait)
    {
        portrait->setPosition(ccp(0, 0));
        portrait->setAnchorPoint(ccp(0,0));
        addChild(portrait,2);
        portrait->setTag(TAG_PORTRAIT);
    }
    
    // 内容
    CCString * contentStr = NULL;
    if (m_pTalkArray->count() > 1)
    {
        contentStr = (CCString *)m_pTalkArray->objectAtIndex(0);
        int height = 80;
        CCLabelTTF *talkLabel = CCLabelTTF::create(contentStr->getCString(), DEFAULT_FONT_NAME, fontSize,CCSizeMake(lineWidth, height),kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        talkLabel->setPosition(ccp(portrait->getContentSize().width + 10, 80));
        talkLabel->setAnchorPoint(ccp(0,0.5));
        addChild(talkLabel,2);
        talkLabel->setTag(TAG_CONTENT_LABEL);
        contentStr = (CCString *)m_pTalkArray->objectAtIndex(1);
        talkLabel = CCLabelTTF::create(contentStr->getCString(), DEFAULT_FONT_NAME, fontSize,CCSizeMake(lineWidth, height),kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        talkLabel->setPosition(ccp(portrait->getContentSize().width + 10, 80 - fontSize));
        talkLabel->setAnchorPoint(ccp(0,0.5));
        addChild(talkLabel,2);
        talkLabel->setTag(TAG_CONTENT_LABEL+1);
    }
    else
    {
        contentStr = (CCString *)m_pTalkArray->objectAtIndex(0);
        int height = 80;
        CCLabelTTF *talkLabel = CCLabelTTF::create(contentStr->getCString(), DEFAULT_FONT_NAME, fontSize,CCSizeMake(lineWidth, height),kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        talkLabel->setPosition(ccp(portrait->getContentSize().width + 10, 80));
        talkLabel->setAnchorPoint(ccp(0,0.5));
        addChild(talkLabel,2);
        talkLabel->setTag(TAG_CONTENT_LABEL);
    }

}

void YYControlUILayer::updateTalkContent()
{
    if (m_pTalkArray->count() > 2)
    {
        m_pTalkArray->removeObjectAtIndex(0);
        m_pTalkArray->removeObjectAtIndex(0);
        CCString * contentStr = NULL;
        if (m_pTalkArray->count() > 1)
        {
            contentStr = (CCString *)m_pTalkArray->objectAtIndex(0);
            CCLabelTTF *talkLabel = (CCLabelTTF *)getChildByTag(TAG_CONTENT_LABEL);
            talkLabel->setString(contentStr->getCString());
            contentStr = (CCString *)m_pTalkArray->objectAtIndex(1);
            talkLabel = (CCLabelTTF *)getChildByTag(TAG_CONTENT_LABEL+1);
            talkLabel->setString(contentStr->getCString());
        }
        else if (m_pTalkArray->count() > 0)
        {
            contentStr = (CCString *)m_pTalkArray->objectAtIndex(0);
            CCLabelTTF *talkLabel = (CCLabelTTF *)getChildByTag(TAG_CONTENT_LABEL);
            talkLabel->setString(contentStr->getCString());
            talkLabel = (CCLabelTTF *)getChildByTag(TAG_CONTENT_LABEL+1);
            talkLabel->setString("");

        }
    }
    else
    {
        m_bTalk = false;
        m_pTalkArray->removeAllObjects();
        removeChildByTag(TAG_TALK_BG_FRAME);
        removeChildByTag(TAG_PORTRAIT);
        removeChildByTag(TAG_NAME_LABEL);
        removeChildByTag(TAG_CONTENT_LABEL);
        removeChildByTag(TAG_CONTENT_LABEL+1);
        Controller::getInstance()->setScriptStatus(kScriptStatusRunning);
    }
}

#pragma mark 指路

void YYControlUILayer::showTheWay()
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSprite* spriteTemp = CCSprite::create();
    CCAnimation* animation = CCAnimation::create();
    for( int i = 0;i < 2;i++)
    {
        const char *str = CCString::createWithFormat("%d",i + 101)->getCString();
        std::string filePath = getFilePath(str, kDirTypeSprite, kFileSuffixPng);
        animation->addSpriteFrameWithFileName(filePath.c_str());
    }
    animation->setDelayPerUnit(0.5f / 2.0f);
    animation->setRestoreOriginalFrame(true);
    CCAnimate* action = CCAnimate::create(animation);
    spriteTemp->runAction(CCRepeatForever::create(CCSequence::create(action, action->reverse(), NULL)));
    spriteTemp->setPosition(ccp(size.width - 100,size.height - 170));
    addChild(spriteTemp);
}


void YYControlUILayer::showLabelStampEffect(const char * str,cocos2d::CCPoint startPosition,int fontSize,bool isClean)
{
    int length = strlen(str);
    int byte = 0,tag = 0;
    CCArray* pLabelArray = CCArray::createWithCapacity(10);
    pLabelArray->retain();
    int zorder = isClean ? 1 : 0;
    for(int i =0;i < length;)
    {
        unsigned char c = *(str+i);
        int byte;
        UTF8_NUMBER_OF_BYTES(c,byte);
        char * font = new char[byte + 1]();
        memcpy(font, str + i,byte);
        CCLabelTTF* pTemp = CCLabelTTF::create(font, DEFAULT_FONT_NAME, fontSize);
        delete [] font;
        pTemp->setAnchorPoint(ccp(0,0.5));
        pTemp->setColor(ccRED);
        pTemp->setScale(3.0);
        pTemp->setTag(tag++);
        pTemp->setZOrder(zorder);
        pLabelArray->addObject(pTemp);
        i+= byte;
    }
    CCFiniteTimeAction* actionEffect = CCSpawn::create(CCScaleTo::create(0.2,1),NULL);
    CCFiniteTimeAction* action = CCSequence::create(actionEffect,CCCallFuncND::create(this, callfuncND_selector(YYControlUILayer::updateNextLabelEffect),(void*)pLabelArray),NULL);
    if(pLabelArray && pLabelArray->count())
    {
        CCLabelTTF* pLabel = (CCLabelTTF*)pLabelArray->objectAtIndex(0);
        pLabel->runAction(action);
        pLabel->setPosition(startPosition);
        addChild(pLabel);
    }
}

void YYControlUILayer::updateNextLabelEffect(CCNode* pSender,void* i)
{
    CCArray* pLabelArray = (CCArray*)i;
    CCLabelTTF* pPreLabel = (CCLabelTTF*)pSender;
    int index = pPreLabel->getTag();
    index++;
    if(index == pLabelArray->count())
    {
        for(int m = 0;m < pLabelArray->count();)
        {
            CCLabelTTF* pLabel = (CCLabelTTF*)pLabelArray->objectAtIndex(m);
            pLabelArray->removeObject(pLabel);
            if(pPreLabel->getZOrder())
            {
                pLabel->removeFromParent();
            }
        }
        pLabelArray->release();
        return;
    }
    CCFiniteTimeAction* actionEffect = CCSpawn::create(CCScaleTo::create(0.2,1),NULL);
    CCFiniteTimeAction* action = CCSequence::create(actionEffect,CCCallFuncND::create(this, callfuncND_selector(YYControlUILayer::updateNextLabelEffect),(void*)pLabelArray),NULL);
    
    CCLabelTTF* pLabel = (CCLabelTTF*)pLabelArray->objectAtIndex(index);
    pLabel->runAction(action);
    pLabel->setPosition(ccp(pPreLabel->getPosition().x + pPreLabel->getContentSize().width,pPreLabel->getPosition().y));
    addChild(pLabel);
}

#pragma mark 试炼

//试炼界面倒计时UI
void YYControlUILayer::intiTrialCounterUI()
{
    CCLayer *layer = CCLayer::create();
    layer->setPosition(CCPointZero);
    addChild(layer,UI_TRIAL_COUNTER,TRIAL_COUNTER_LAYER);
    YYSprite * sprite = YYSprite::createWithActionSet("se30", "se30", kDirTypeEffect);
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    sprite->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    sprite->setAction(0,false);
    layer->addChild(sprite,0,TRIAL_COUNTER_SPRITE);
    layer->setVisible(false);
}

void YYControlUILayer::trialCommonUI()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    // 头像背景
    m_pTrialPortraitBGFrame = CCSprite::create(PLAYER_PORTRAIT_BG_FRAME);
    int width = m_pTrialPortraitBGFrame->getContentSize().width;
    m_pTrialPortraitBGFrame->setPosition(ccp(winSize.width - width / 2,winSize.height - m_pTrialPortraitBGFrame->getContentSize().height / 2 - 10));
    m_pTrialPortraitBGFrame->setFlipX(true);
    addChild(m_pTrialPortraitBGFrame,1);
    
    const char *picPath= CCString::createWithFormat(("i-0-%d-0-0"),m_pRoleByTrial->getSpriteData()->getSpriteIndex() - 1)->getCString();
    std::string str = getFilePath(picPath,kDirTypeIcon,kFileSuffixPng);
    m_pTrialPlayerPortrait = CCSprite::create(str.c_str());
    m_pTrialPlayerPortrait->setPosition(ccp(m_pTrialPortraitBGFrame->getPosition().x + 9, m_pTrialPortraitBGFrame->getPosition().y));
    addChild(m_pTrialPlayerPortrait, 2);
    
    // 状态栏
    m_pTrialPlayerStatusBar = CCSprite::create(PLAYER_STATUS_BAR);
    m_pTrialPlayerStatusBar->setPosition(ccp(winSize.width - width - m_pTrialPlayerStatusBar->getContentSize().width / 2 + 37 ,winSize.height - m_pTrialPlayerStatusBar->getContentSize().height / 2 - 34));
    m_pTrialPlayerStatusBar->setFlipX(true);
    addChild(m_pTrialPlayerStatusBar);
    
    // 能量槽
    m_pTrialPlayerAngerStatusBar = CCSprite::create(PLAYER_ENERGY_BAR);
    m_pTrialPlayerAngerStatusBar->setPosition(ccp(winSize.width - width - m_pTrialPlayerAngerStatusBar->getContentSize().width / 2 + 27 ,m_pTrialPlayerStatusBar->getPositionY() - (m_pTrialPlayerStatusBar->getContentSize().height + m_pTrialPlayerAngerStatusBar->getContentSize().height) / 2));
    m_pTrialPlayerAngerStatusBar->setFlipX(true);
    addChild(m_pTrialPlayerAngerStatusBar);
    
    // 血条
    CCSprite* sprBlood = CCSprite::create(PLAYER_BLOOD);
    sprBlood->setFlipX(true);
    m_pTrialPlayerBloodBar = CCProgressTimer::create(sprBlood);
    m_pTrialPlayerBloodBar->setPosition(ccp(m_pTrialPlayerStatusBar->getPositionX() - m_pTrialPlayerStatusBar->getContentSize().width / 2 + 13 ,m_pTrialPlayerStatusBar->getPositionY() + m_pTrialPlayerBloodBar->getContentSize().height / 2 + 1));
    m_pTrialPlayerBloodBar->setMidpoint(ccp(1,0));
    m_pTrialPlayerBloodBar->setBarChangeRate(ccp(1, 0));
    m_pTrialPlayerBloodBar->setType(kCCProgressTimerTypeBar);
    m_pTrialPlayerBloodBar->setAnchorPoint(ccp(0,0.5));
    m_pTrialPlayerBloodBar->setPercentage(100);
    addChild(m_pTrialPlayerBloodBar);
    
    // 血值
    CCString * tempStr = CCString::createWithFormat("%d/%d",m_pRoleByTrial->getCurrentHP(),m_pRoleByTrial->getSpriteAttribute()->getMaxHP());
    m_pTrialHPLabel = CCLabelTTF::create(tempStr->getCString(), DEFAULT_FONT_NAME, 18);
    m_pTrialHPLabel->setPosition(ccp(m_pTrialPlayerBloodBar->getPositionX() + m_pTrialPlayerBloodBar->getContentSize().width / 2, m_pTrialPlayerBloodBar->getPositionY() - 1));
    addChild(m_pTrialHPLabel);
    
    // 蓝条
    CCSprite* sprMana = CCSprite::create(PLAYER_MANA);
    sprMana->setFlipX(true);
    m_pTrialPlayerManaBar = CCProgressTimer::create(sprMana);
    m_pTrialPlayerManaBar->setPosition(ccp(m_pTrialPlayerStatusBar->getPositionX() - m_pTrialPlayerStatusBar->getContentSize().width / 2 + 13 ,m_pTrialPlayerStatusBar->getPositionY() - m_pTrialPlayerManaBar->getContentSize().height / 2 - 7));
    m_pTrialPlayerManaBar->setMidpoint(ccp(1,0));
    m_pTrialPlayerManaBar->setBarChangeRate(ccp(1, 0));
    m_pTrialPlayerManaBar->setType(kCCProgressTimerTypeBar);
    m_pTrialPlayerManaBar->setAnchorPoint(ccp(0,0.5));
    m_pTrialPlayerManaBar->setPercentage(100);
    addChild(m_pTrialPlayerManaBar);
    
    // 法力值
    tempStr = CCString::createWithFormat("%d/%d",m_pRoleByTrial->getCurrentMP(),m_pRoleByTrial->getSpriteAttribute()->getMaxMP());
    m_pTrialMPLabel = CCLabelTTF::create(tempStr->getCString(), DEFAULT_FONT_NAME, 15);
    m_pTrialMPLabel->setPosition(ccp(m_pTrialPlayerManaBar->getPositionX() + m_pTrialPlayerManaBar->getContentSize().width / 2, m_pTrialPlayerManaBar->getPositionY()));
    addChild(m_pTrialMPLabel);
    
    // 能量条
    m_pTrialEnergyBar = CCProgressTimer::create(CCSprite::create(PLAYER_ENERGY));
    m_pTrialEnergyBar->setPosition(ccp(m_pTrialPlayerAngerStatusBar->getContentSize().width/2 + 8,m_pTrialPlayerAngerStatusBar->getContentSize().height/2 + 2));
    m_pTrialEnergyBar->setMidpoint(ccp(1,0));
    m_pTrialEnergyBar->setBarChangeRate(ccp(1,0));
    m_pTrialEnergyBar->setType(kCCProgressTimerTypeBar);
    m_pTrialPlayerAngerStatusBar->addChild(m_pTrialEnergyBar);
    
    // 等级
    m_pTrialLevelLabelFrame = CCSprite::create(LEVEL_LABEL_BG_FRAME);
    m_pTrialLevelLabelFrame->setPosition(ccp(winSize.width - 177,winSize.height - 132));
    m_pTrialLevelLabelFrame->setFlipX(true);
    addChild(m_pTrialLevelLabelFrame);
    
    const char *temp = CCString::createWithFormat("%d",m_pRoleByTrial->getLevel())->getCString();
    m_pTrialLevelLabel = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 20);
    m_pTrialLevelLabel->setPosition(ccp(m_pTrialLevelLabelFrame->getContentSize().width / 2 + 5, m_pTrialLevelLabelFrame->getContentSize().height / 2));
    m_pTrialLevelLabelFrame->addChild(m_pTrialLevelLabel);
    
    // 手势
    const char *pGesture[] = {"ui/111.png", "ui/110.png" ,"ui/113.png"};
    for (int i = 0; i < 3; i++)
    {
        
        CCSprite *pSpriteBg = CCSprite::create(GESTURE_BG);
        pSpriteBg->setPosition(ccp(i * (m_pTrialPlayerAngerStatusBar->getContentSize().width / 3 - 30) + 60, -12));
        m_pTrialPlayerAngerStatusBar->addChild(pSpriteBg, 0, 201 + i);
        
        CCSprite *pSpriteGesture = CCSprite::create(pGesture[i]);
        pSpriteGesture->setPosition(ccp(i * (m_pTrialPlayerAngerStatusBar->getContentSize().width / 3 - 30) + 61, -16));
        m_pTrialPlayerAngerStatusBar->addChild(pSpriteGesture, 0, 101 + i);
    }
}

//试炼播放倒计时动画
void YYControlUILayer::updateTrialFightTime()
{
    if (Controller::getInstance()->isTrial())
    {
        if (m_bPlayTrialCounter)
        {
            CCLayer* layer = (CCLayer*)getChildByTag(TRIAL_COUNTER_LAYER);
            if (layer)
            {
                layer->setVisible(true);
                YYSprite* sprite = (YYSprite*)layer->getChildByTag(TRIAL_COUNTER_SPRITE);
                if (sprite)
                {
                    sprite->updateAnimation();
                    if (sprite->isActionOver())
                    {
                        m_bPlayTrialCounter = false;
                        m_bPlayTrialTime = true;
                        getChildByTag(TRIAL_COUNTER_LAYER)->setVisible(false);
                        m_pRoleByTrial->setTrialPause(false);
                        m_pRoleByControlled->setTrialPause(false);
                    }
                }
            }
        }
    }
}

//更新试炼时间
void YYControlUILayer::updateTrialTime()
{
    int currentTime = Controller::getInstance()->getCurrentSecond();
    int second = Controller::getInstance()->getScene()->getStageDuration()+ currentTime - Controller::getInstance()->getScene()->getCurrentTime();
    second = 180 - second;
    
    if (second <= 0) //试炼结束
    {
        m_bPlayTrialTime = false;
        bool bSuccess = false;
        m_pRoleByTrial->setTrialPause(true);
        float fPlayerRemainHP = m_pRoleByControlled->getCurrentHP() / m_pRoleByControlled->getSpriteAttribute()->getMaxHP();
        float fTrialRemainHP = m_pRoleByTrial->getCurrentHP() / m_pRoleByTrial->getSpriteAttribute()->getMaxHP();
        (fPlayerRemainHP - fTrialRemainHP > 0) ? bSuccess = true : bSuccess=false;
        Controller::getInstance()->getScene()->getPopUpUI()->initTrialResult(bSuccess);
        Controller::getInstance()->getScene()->getControlUILayer()->trialOver();  //试炼结束
        Controller::getInstance()->getScene()->getControlUILayer()->clearKeyState();//清除按键状态
    }
    
    if (m_nCurrentTime != second)
    {
        const char * temp = NULL;
        if (second % 60 > 9)
        {
            if (second / 60 > 9)
            {
                temp = CCString::createWithFormat("%d:%d:%d",second / 3600,second / 60,second % 60)->getCString();
            }
            else
            {
                temp = CCString::createWithFormat("%d:0%d:%d",second / 3600,second / 60,second % 60)->getCString();
            }
        }
        else
        {
            if (second / 60 > 9)
            {
                temp = CCString::createWithFormat("%d:%d:0%d",second / 3600,second / 60,second % 60)->getCString();
            }
            else
            {
                temp = CCString::createWithFormat("%d:0%d:0%d",second / 3600,second / 60,second % 60)->getCString();
            }
        }
        if (m_nCurrentTime == -1)
        {
            std::string str = getFilePath("360",kDirTypePic,kFileSuffixPng);
            m_pTimeLabelAtlas = CCLabelAtlas::create(temp,str.c_str(),27,35,'0');// 按ascii排列顺序排在9后面的是:;
            m_pTimeLabelAtlas->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, 30));
            m_pTimeLabelAtlas->setAnchorPoint(ccp(0.5, 0.5));
            addChild(m_pTimeLabelAtlas);
        }
        else
        {
            m_pTimeLabelAtlas->setString(temp);
        }
        m_nCurrentTime = second;
    }
}

void YYControlUILayer::updateTrialStatusBar()
{
    if (Controller::getInstance()->getSceneType() == DUNGEONS || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {
        {
            float tmpEnegyPercentage =  m_pRoleByTrial->getEnegyPercentage();
            if(tmpEnegyPercentage <= 97.8f)
            {
                tmpEnegyPercentage += 0.2f;
                m_pRoleByTrial->setEnegyPercentage(tmpEnegyPercentage);
            }
            m_pTrialEnergyBar->setPercentage(tmpEnegyPercentage);
            
            const char *pGesture1[] = {"ui/107.png", "ui/106.png", "ui/109.png"};
            const char *pGesture2[] = {"ui/111.png", "ui/110.png", "ui/113.png"};
            for (int i = 2; i >= 0; i--)
            {
                if (m_pTrialPlayerAngerStatusBar->getChildByTag(101 + i))
                {
                    CCSprite *pSprite = (CCSprite *)m_pTrialPlayerAngerStatusBar->getChildByTag(101 + i);
                    pSprite->setTexture(m_pTrialEnergyBar->getPercentage() >= s_u8EnergyConsume[2-i]
                                        ? CCTextureCache::sharedTextureCache()->addImage(pGesture1[i])
                                        : CCTextureCache::sharedTextureCache()->addImage(pGesture2[i]));
                }
            }
        }
        if (m_nTrialHP != m_pRoleByTrial->getCurrentHP())
        {
            m_nTrialHP = m_pRoleByTrial->getCurrentHP();
            m_pTrialPlayerBloodBar->setPercentage(m_nTrialHP * 100 / (float)m_pRoleByTrial->getSpriteAttribute()->getMaxHP());
            m_pTrialHPLabel->setString(CCString::createWithFormat("%d/%d",m_nTrialHP,m_pRoleByTrial->getSpriteAttribute()->getMaxHP())->getCString());
        }
        if (m_nTrialMP != m_pRoleByTrial->getCurrentMP())
        {
            m_nTrialMP = m_pRoleByTrial->getCurrentMP();
            m_pTrialPlayerManaBar->setPercentage(m_nTrialMP * 100 / (float)m_pRoleByTrial->getSpriteAttribute()->getMaxMP());
            m_pTrialMPLabel->setString(CCString::createWithFormat("%d/%d",m_nTrialMP,m_pRoleByTrial->getSpriteAttribute()->getMaxMP())->getCString());
            //        if(Controller::getInstance()->getSceneType() == DUNGEONS
            //           || Controller::getInstance()->getSceneType() == WAR_ZONE)
            //        {// 战斗场景魔法值有变化时判断技能图标是否需要替换
            //            setSkillIcon();
            //        }
        }
    }
}

void YYControlUILayer::trialOver()
{
    if (m_pRoleByTrial)
    {
        m_pRoleByTrial->setMoving(false);
        if (m_pRoleByTrial->getActionStatus() == kActionNormal)
        {
            m_pRoleByTrial->switchAction(ACTION_STAND0);
        }
    }
}
