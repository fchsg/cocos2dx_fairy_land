
//  PopUpUI.cpp
//  FairyLand II
//
//  Created by 章敏 on 9/25/13.
//
//

#include "YYPopUpUI.h"
#include "YYLayer.h"
#include "YYTrialPlayer.h"
#include "../common/ccUtil.h"
#include "YYShopData.h"
#include "YYLoginAwardData.h"
#include "YYActivityData.h"
#include "YYAchievementData.h"
#include "YYConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

PopUpUI::PopUpUI()
:m_eUIType(kUIDefaultStatus)
,m_nRecordTaskInfoCount(-1)
,m_nComposeSlotX(0)
,m_nChooseTaskIndex(-1)
,m_nAcceptedTaskIndex(-1)
,m_nSkillDataIndex(0)
,m_pBagPageButton(NULL)
,m_pItemTypeButton(NULL)
,m_pSkillPageButton(NULL)
,m_pPlayerEquipDictionary(NULL)
,m_pDragRegionDictionary(NULL)
,m_pChattingButtonArray(NULL)
,m_pAccountTF(NULL)
,m_pScrollView(NULL)
,m_pRoleWealthArray(NULL)
,m_pUITouchGroup(NULL)
,m_pTaskDialogueInfo(NULL)
,m_pTaskDialogueButton(NULL)
,m_pUILabel(NULL)
,m_pRandomImage(NULL)
,m_pAwardButtonArray(NULL)
,m_pAwardPositionArray(NULL)
,m_pChooseAwardArray(NULL)
,m_pBagEquip(NULL)
,m_pReplacedEquip(NULL)
,m_pBagCell(NULL)
,m_pReplacedCell(NULL)
,m_bEquipDescription(false)
,m_bCompose(false)
,m_bTouchBegan(false)
,m_bTouchMoved(false)
,m_bTouchEnded(false)
,m_bIsOpenTask(false)
,m_bWaitingForFindTarget(false)
,m_bWaitingForUpdateTask(false)
,m_bReinforceConfirmed(false)
,m_bIsScroll(false)
,m_bSkillDataUpdateOver(false)
,m_pTopLayer(NULL)
,m_nChannelIndex(0)
,m_u8AwardStatus(kAwardDefaultStatus)
,m_u8AwardActionStatus(kAwardActionDefaultStatus)
,m_nMaxZorder(0)
,m_nStartFrame(0)
,m_nIntervalFrame(INTERVAL_FRAME)
,m_nRandomCount(0)
,m_nRandomIndex(-1)
,m_nNeedJewelCount(-1)
,m_pShopContentDictionary(NULL)
,m_bIsShopInit(false)
,m_u8ShopStatus(0)
,m_eCurShopTabType(kShopPageInit)
,m_nBuyNum(0)
,m_nCurBuyMoneyType(0)
,m_nCurBuyShopItemIndex(-1)
,m_kShopPageType(kShopPageInit)
,m_bIsShopDetailOpen(false)
,m_pUIShopTouchGroup(NULL)
,m_bInitChargeInfo(false)
,m_pChargeContentDictionary(NULL)
,m_pUIChargeTouchGroup(NULL)
,m_pHaseePayTouchGroup(NULL)
,m_bIsEnterProtossEquipUI(false)
,m_bIsProtossInit(false)
,m_bIsProtossEquipInit(false)
,m_pProtossEquipList(NULL)
,m_pProtossWaitforRecvList(NULL)
,m_pProtossExchangeRuleList(NULL)
,m_pProtossBagList(NULL)
,m_pProtossPieceList(NULL)
,m_u8UIAwardStatus(kUIDefaultAward)
,m_uSignUpAddDay(0)
,m_pCurDevineIndex(-1)
,m_nDetailIndex(-1)
,m_bIsInitActivityTaskData(false)
,m_bIsInitActivityWishTreeData(false)
,m_nFreeFromUpdateNum(0)
,m_nConsumeJewNum(0)
,m_nWishTreeLevel(0)
,m_pDictRankData(NULL)
,m_pRankPageButton(NULL)
,m_pRoleByControlled(NULL)
,m_nRankCurPageID(0)
,m_nRankPrePageID(0)
,m_nVipCurPageID(0)
,m_nVipPageCnt(0)
,m_u8TrainSelIdx(0)
,m_nProtossCount(0)
,m_nButtonIndex(-1)
,m_bIsPopUpAchievementTip(false)
,m_fStartTime(0)
,m_pDungeonsDataArray(NULL)
,m_pIsChoosedDungeon(NULL)
,m_pStageDataArray(NULL)
,m_pAwardDictionary(NULL)
,m_pAwardString(NULL)
,m_pAwardArray(NULL)
,m_pDungeonsButtonDictionary(NULL)
,m_pRendDungeonActionArray(NULL)
,m_pBonusEffectArray(NULL)
,m_bTrialResult(false)
,m_pMailListArray(NULL)
,m_pMailDetalData(NULL)
,m_uMailNum(0)
,m_pActivityTaskArray(NULL)
,m_pActivityTreeArray(NULL)
,m_arrTurnTableData(NULL)
,m_arrPalyTableData(NULL)
,m_bWipeDungeonTouchEnable(false)
{
    m_pAchieveTipsArray = CCArray::create();
    m_pAchieveTipsArray->retain();
}

PopUpUI::~PopUpUI()
{
    CC_SAFE_DELETE(m_pDragRegionDictionary);
    CC_SAFE_DELETE(m_pChattingButtonArray);
    CC_SAFE_DELETE(m_pSpecialEffectArray);
    CC_SAFE_RELEASE(m_pAccountTF);
    for(int i = 0;i < kChattingCountPage; ++i)
    {
        CC_SAFE_DELETE(m_pChannelBoxArray[i]);
    }
    CC_SAFE_DELETE_ARRAY(m_pChannelBoxArray);
    CC_SAFE_DELETE(m_pShopContentDictionary);
    
    
//    for(int i = 0; i < kChargePageNum; i++)
//    {
//        CCArray *pChargeArray = (CCArray *)m_pChargeContentDictionary->objectForKey(CCString::createWithFormat("tab%d", i)->getCString());
//        if(pChargeArray)
//            pChargeArray->removeAllObjects();
//        CC_SAFE_RELEASE(pChargeArray);
//    }
    // CC_SAFE_DELETE(m_pChargeContentDictionary);
    
    
    CC_SAFE_DELETE(m_pProtossEquipList);
    CC_SAFE_DELETE(m_pProtossWaitforRecvList);
    CC_SAFE_DELETE(m_pProtossExchangeRuleList);
    CC_SAFE_DELETE(m_pProtossBagList);
    CC_SAFE_DELETE(m_pProtossPieceList);
    
    // 副本界面
    CC_SAFE_DELETE(m_pRendDungeonActionArray);
    CC_SAFE_DELETE(m_pDungeonsButtonDictionary);
    CC_SAFE_DELETE(m_pBonusEffectArray);
    
    CC_SAFE_RELEASE(m_pAchieveTipsArray);
}

bool PopUpUI::init()
{
    // 副本
    m_pDungeonsButtonDictionary = new CCDictionary();
    m_pRendDungeonActionArray = new CCArray(5);
    m_pDragRegionDictionary = new CCDictionary();
    m_pSpecialEffectArray = new CCArray(10);
    m_pShopContentDictionary = new CCDictionary();
    m_pChargeContentDictionary = new CCDictionary();
    m_pProtossEquipList = new CCArray();
    m_pProtossWaitforRecvList = new CCArray();
    m_pProtossExchangeRuleList = new CCArray();
    m_pProtossBagList = new CCArray();
    m_pProtossPieceList = new CCDictionary();
    
    for(int i = 0; i < kShopPageNum; i++)
    {
        CCArray *pShopArray = CCArray::create();
        if(pShopArray)
            m_pShopContentDictionary->setObject(pShopArray, CCString::createWithFormat("tab%d", i)->getCString());
    }
    for(int i = 0; i < kChargePageNum; i++)
    {
        CCArray *pChargeArray = CCArray::create();
        if(pChargeArray)
            m_pChargeContentDictionary->setObject(pChargeArray, CCString::createWithFormat("tab%d", i)->getCString());
    }
    m_pChannelBoxArray = new CCArray*[kChattingCountPage];
    for (int i = 0; i < kChattingCountPage; i++)
    {
        m_pChannelBoxArray[i] = new CCArray(10);
    }
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_cAdjustScreenPosition = ccp((winSize.width - STANDARD_SCENE_WIDTH) / 2,(winSize.height - STANDARD_SCENE_HEIGHT) / 2);

    return true;
}

#pragma mark 触控

void PopUpUI::touchBegan(cocos2d::CCTouch * touch)
{
    if(Controller::getInstance()->isWaitingReceiveMessage()
       || Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    
    m_bTouchBegan = true;
    m_CurTouchPoint = touch->getLocation();
    m_StartPoint = touch->getLocation();
}

void PopUpUI::touchMoved(cocos2d::CCTouch * touch)
{
    m_bTouchMoved = true;
    m_PreTouchPoint = m_CurTouchPoint;
    m_CurTouchPoint = touch->getLocation();
}

void PopUpUI::touchEnded(cocos2d::CCTouch * touch)
{
    m_bTouchEnded = true;
    m_CurTouchPoint = touch->getLocation();
}

void PopUpUI::update()
{
    if (Controller::getInstance()->isTutorial())// 新手引导
    {
        YYGuide *guide = (YYGuide *)getChildByTag(TAG_GUIDE);
        if (guide)
        {
            guide->update();
        }
    }
    switch (m_eUIType)
    {
        case kUIBag:
        {
            YYSprite *player = dynamic_cast<YYSprite *>(getChildByTag(TAG_UI_PLAYER));
            if (player && player->isVisible())
            {
                player->update();
            }
            updateTreasureData();
        }
            break;
        case kUITrain:
        {
            YYSprite *player = dynamic_cast<YYSprite *>(getChildByTag(TAG_TRIAL_UI_PLAYER));
            if (player && player->isVisible())
            {
                player->update();
            }
        }
            break;
        case kUITask:
        {
            updateTaskUI();
        }
            break;
        case kUIAward:
        {
            updateAwardUI();
        }
            break;
        case kUIProtoss:
        {
            updateProtossAction();
        }
            break;
        case kUIAchieve:
        {
        }
            break;
        case kUIDungeons:
        {
            dungeonsUITouchHandle();
        }
            break;
        case kUIStage:
        {
            updateDungeonsUI();
        }
            break;
        case kUIFightEvaluation:
        {
            updateFightResult();
        }
            break;
            case kUIWipeDungeon:
        {
            updateWipeDungeonResult();
        }
            break;
        case kUIPlayTable:
        {
            updatePlayTable();
        }
            break;
        default:
            break;
    }
    if(m_pSpecialEffectArray)
    {
        for(int i = 0; i < m_pSpecialEffectArray->count();)
        {
            YYSprite* pSprite = (YYSprite*)m_pSpecialEffectArray->objectAtIndex(i);
            if((pSprite->isActionOver() && !pSprite->isRepeatAction()) || pSprite->isDeleted())
            {
                if(pSprite->getParent())
                {
                    pSprite->removeFromParent();
                }
                m_pSpecialEffectArray->removeObject(pSprite);
            }
            else
            {
                pSprite->updateAnimation();
                i++;
            }
        }
    }
}

/**
 * 是否触摸无效
 */
bool PopUpUI::isTouchUnable(CCObject * sender, cocos2d::gui::TouchEventType type, bool bPlayEffect)
{
    Widget *widget = static_cast<Widget *>(sender);
    if (!sender)
    {
        return true;
    }
    if (bPlayEffect)
    {
        const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
    }
    if (Controller::getInstance()->isTutorial())// 新手引导
    {
        bool isContinue = false;//继续执行脚本
        YYGuide *guide = static_cast<YYGuide *>(getChildByTag(TAG_GUIDE));
        if (guide)
        {
            if (guide->getTouchBeganValue() == -1)
            {
                if (type == TOUCH_EVENT_BEGAN && guide->getTouchRegion().containsPoint(widget->getTouchStartPos()))
                {
                    isContinue = true;
                }
                if (type == TOUCH_EVENT_ENDED && guide->getTouchRegion().containsPoint(widget->getTouchEndPos()))
                {
                    isContinue = true;
                }
            }
            else if(guide->getTouchBeganValue() == widget->getTag())
            {
                isContinue = true;
            }
        }
        if (isContinue)
        {
            Controller::getInstance()->setTutorial(false);
            Controller::getInstance()->setScriptStatus(kScriptStatusRunning);
            removeChildByTag(TAG_GUIDE);
        }
        return !isContinue;
    }
    else if(m_pRoleByControlled->isLocked())
    {
        return true;
    }
    return  false;
}

bool PopUpUI::isDigtal(std::string str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9')
        {
            return false;
        }
    }
    return true;
}
cocos2d::ccColor3B PopUpUI::getRGBValue(uint8_t nColorType)
{
    ccColor3B color;
    switch (nColorType)
    {
        case 1:// 白色
            color = ccc3(255, 255, 255);
            break;
        case 2:// 绿色
            color = ccc3(56, 254, 3);
            break;
        case 3:// 蓝色
            color = ccc3(24, 159, 254);
            break;
        case 4:// 紫色
            color = ccc3(170, 27, 252);
            break;
        case 5:// 橙色
            color = ccc3(254, 145, 4);
            break;
        case 6:// 红色
            color = ccc3(252, 27, 27);
            break;
        case 7:// 粉色
            color = ccc3(255, 142, 193);
            break;
        case 8:// 金色
            color = ccc3(206, 187, 20);
            break;
        default:
            color = ccc3(255, 255, 255);
            break;
    }
    return color;
}
UILabel* PopUpUI::createLabel(CCSize size, CCTextAlignment kAlignment, int fntSize, ccColor3B color)
{
    UILabel *pLabel = UILabel::create();
    pLabel->setTextHorizontalAlignment(kAlignment);
    pLabel->setTextVerticalAlignment(kCCVerticalTextAlignmentCenter);
    pLabel->setTextAreaSize(size);
    pLabel->setColor(color);
    pLabel->setFontSize(fntSize);
    return pLabel;
}
Widget* PopUpUI::getPopWindow(std::string str1, std::string str2, bool isVisible)
{
    std::string str_parent = str1;
    std::string str_child = str2;
    TouchGroup *touchGroup = static_cast<TouchGroup *>(getParent()->getChildByTag(TAG_POPUP_WINDOW));
    Layout *pPanel = static_cast<Layout *>(touchGroup->getWidgetByName("Panel"));
    for (int i = 0; i < pPanel->getChildrenCount(); i++)
    {
        Widget * widget= static_cast<Widget *>(pPanel->getChildren()->objectAtIndex(i));
        widget->setEnabled(false);
        widget->setVisible(false);
        widget->setTouchEnabled(false);
    }
    Layout *pPanel_parent = static_cast<Layout *>(pPanel->getChildByName(str_parent.c_str()));
    pPanel_parent->setEnabled(isVisible);
    pPanel_parent->setVisible(isVisible);
    pPanel_parent->setTouchEnabled(isVisible);
    for (int i = 0; i < pPanel_parent->getChildrenCount(); i++)
    {
        Widget * widget= static_cast<Widget *>(pPanel_parent->getChildren()->objectAtIndex(i));
        widget->setEnabled(false);
        widget->setVisible(false);
        widget->setTouchEnabled(false);
    }
    Layout *pPanel_child = static_cast<Layout *>(pPanel_parent->getChildByName(str_child.c_str()));
    pPanel_child->setEnabled(isVisible);
    pPanel_child->setVisible(isVisible);
    pPanel_child->setTouchEnabled(isVisible);
    return pPanel_child;
}

void PopUpUI::exitUI()
{
    const std::string filePath = getFilePath(CLOSEUI_EFFECT ,kDirTypeSound ,kFileSuffixWav);
    PLAY_EFFECT(filePath.c_str());
    removeChild(m_pUITouchGroup);
    m_pUITouchGroup = NULL;
    Controller::getInstance()->setGameStatus(kStatusScene);
    switch (getUIType())
    {
        case kUIBag:
            exitBagUI();
            break;
        case kUIChatting:
            exitChatting();
            break;
        case kUITask:
            exitTaskUI();
            break;
        case kUILoginAward:
        {
        }
            break;
        case kUISigning:
            exitSignUp();
            break;
        case kUITalent:
            break;
        case kUIActivity:
            exitActivity();
            break;
        case kUIRankWar:
            exitRank();
            break;
        case kUIPlayTable:
            exitPlayTable();
            break;
        case kUITrain:
            removeChildByTag(TAG_TRIAL_UI_PLAYER);
            break;
        case kUIProtoss:
        {
            m_pDevineBtn = NULL;
            setIsProtossInit(false);
            setIsProtossEquipInit(false);
            setIsEnterProtossEquipUI(false);
            protossBaseUIClose();
            protossEquipUIClose();
            protossExchangeUIClose();
        }
            break;
        case kUIAchieve:
            exitAchievement();
            break;
        case kUIConvertCode:
            if(m_pConvertCodeEditBox)
            {
                m_pConvertCodeEditBox = NULL;
            }
            break;
        default:
            break;
    }
    setUIType(kUIDefaultStatus);
}

void PopUpUI::closeUI(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        m_nButtonIndex = 0;
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        exitUI();
    }
}

#pragma mark 弹出提示信息
/**
 * 设置宝石消耗提示
 */
void PopUpUI::setConsumeJewelTips(JewelPurpose purpose, int num)
{
    m_eConsumePurpose = purpose;
    TouchGroup *touchGroup = static_cast<TouchGroup *>(getParent()->getChildByTag(TAG_POPUP_WINDOW));
    Layout * panel = static_cast<Layout *>(touchGroup->getWidgetByName("Panel"));
    Layout * panel_1 = static_cast<Layout *>(panel->getChildByName("Panel_1"));
    panel_1->setEnabled(true);
    panel_1->setVisible(true);
    panel_1->setTouchEnabled(true);
    Layout * panel_e = static_cast<Layout *>(panel_1->getChildByName("Panel_e"));
    panel_e->setEnabled(true);
    panel_e->setVisible(true);
    panel_e->setTouchEnabled(true);
    Button *button0 = static_cast<Button *>(panel_e->getChildByName("Button0"));
    button0->setTouchEnabled(true);
    button0->addTouchEventListener(this, toucheventselector(PopUpUI::cancelConsume));
    Button *button1 = static_cast<Button *>(panel_e->getChildByName("Button1"));
    button1->setTouchEnabled(true);
    button1->addTouchEventListener(this, toucheventselector(PopUpUI::confirmConsume));
    ImageView * imageView = static_cast<ImageView *>(panel_e->getChildByName("ImageView"));
    Label *label_1 = static_cast<Label *>(imageView->getChildByName("TextArea_1"));
    label_1->setText(CCString::createWithFormat("%d",num)->getCString());
    Label *label_2 = static_cast<Label *>(imageView->getChildByName("TextArea_2"));
    if (m_eConsumePurpose == kJewelConsumeForCompose)
    {
        label_2->setText(g_sComposeTip);
    }
    else if (m_eConsumePurpose == kJewelConsumeForReinforce)
    {
        label_2->setText(g_sReinforce100Tip);
    }
    else if(m_eConsumePurpose == kJewelConsumeForRelive)
    {
        label_2->setText(g_sReliveConfirm);
    }
    else if (m_eConsumePurpose == kJewelConsumeForCommodity)
    {
        label_2->setText(g_sPurchaseCommodity);
    }
    else if (m_eConsumePurpose == kJewelConsumeForLightConstellation)
    {
        label_2->setText(g_sLightConstellation);
    }
    else if (m_eConsumePurpose == kJewelConsumeForFlopAward)
    {
        label_2->setText(g_sGetOneChoiceForAward);
    }
    else if (m_eConsumePurpose == kJewelConsumeForUpgradeSkill)
    {
        label_2->setText(g_sUpgradeSkill);
    }
    else if (m_eConsumePurpose == kJewelConsumeForUpgradeAngry)
    {
        label_2->setText(g_sUpgradeAngry);
    }
    else if(m_eConsumePurpose == kJewelConsumeForActivityUpdate)
    {
        label_2->setText(g_sActivityUpdate);
    }
    else if(m_eConsumePurpose == kJewelConsumeForTalentReset)
    {
        label_2->setText(g_sTalentReset);
    }
    else if(m_eConsumePurpose == kJewelConsumeForBuyChallenge)
    {
        label_2->setText(g_sPlayTableBossDungeonJew);
    }
    else if(m_eConsumePurpose == kJewelConsumeForBuyPray)
    {
        label_2->setText(g_sWorldBossPrayConfirm);
    }
    else if(m_eConsumePurpose == kJewelConsumeForBuyQuickEnter)
    {
        label_2->setText(g_sImmediatelyRelive);
    }
    if (m_pUITouchGroup)
    {
        m_pUITouchGroup->setTouchEnabled(false);
    }
}

/**
 * 隐藏宝石消耗提示
 */
void PopUpUI::hideConsumeTips()
{
    TouchGroup *touchGroup = static_cast<TouchGroup *>(getParent()->getChildByTag(TAG_POPUP_WINDOW));
    Layout * panel = static_cast<Layout *>(touchGroup->getWidgetByName("Panel"));
    Layout * panel_1 = static_cast<Layout *>(panel->getChildByName("Panel_1"));
    Layout * panel_e = static_cast<Layout *>(panel_1->getChildByName("Panel_e"));
    panel_e->setEnabled(false);
    panel_e->setVisible(false);
    panel_e->setTouchEnabled(false);
    panel_1->setEnabled(false);
    panel_1->setVisible(false);
    panel_1->setTouchEnabled(false);
}

/**
 * 取消花费宝石
 */
void PopUpUI::cancelConsume(CCObject *pSender ,cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        hideConsumeTips();
        if (m_pUITouchGroup)
        {
            m_pUITouchGroup->setTouchEnabled(true);
        }
        if (m_eConsumePurpose == kJewelConsumeForRelive)
        {
            Controller::getInstance()->sendMessage(ASK_RETURN_CITY,true);
            Controller::getInstance()->switchGameStatus(kStatusLoading);
        }
        else if (m_eConsumePurpose == kJewelConsumeForCommodity)
        {
            m_pUITouchGroup->setTouchEnabled(false);
            getPopWindow("Panel_0", "Panel_a", true);
        }
        else if (m_eConsumePurpose == kJewelConsumeForFlopAward)
        {
            m_pUITouchGroup->setTouchEnabled(true);
        }
    }
}

/**
 * 确认花费宝石
 */
void PopUpUI::confirmConsume(CCObject *pSender ,cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        hideConsumeTips();
        if (m_pUITouchGroup)
        {
            m_pUITouchGroup->setTouchEnabled(true);
        }
        if (m_eConsumePurpose == kJewelConsumeForCompose)
        {
            getMeterialConsumeJewlLogic();
        }
        else if (m_eConsumePurpose == kJewelConsumeForReinforce)
        {
            reinforceConsumeJewelLogic();
        }
        else if(m_eConsumePurpose == kJewelConsumeForRelive)
        {
            Controller::getInstance()->sendMessage(ASK_RELIVE_JEWEL,true);
        }
        else if (m_eConsumePurpose == kJewelConsumeForCommodity)
        {
            m_pUITouchGroup->setTouchEnabled(false);
            shopBuyLogic();
        }
        else if (m_eConsumePurpose == kJewelConsumeForLightConstellation)
        {
            lightConstellationConsumeLogic();
        }
        else if (m_eConsumePurpose == kJewelConsumeForFlopAward)
        {
            flopAwardConsumeJewelLogic();
        }
        else if (m_eConsumePurpose == kJewelConsumeForUpgradeSkill || m_eConsumePurpose == kJewelConsumeForUpgradeAngry)
        {
            skillUpgradeConsumeJewelLogic();
        }
        else if (m_eConsumePurpose == kJewelConsumeForActivityUpdate)
        {
            Controller::getInstance()->sendMessage(ASK_ACTIVITY_TASK_UPDATE,true);
        }
        else if (m_eConsumePurpose == kJewelConsumeForTalentReset)
        {
            SkillData *skillData = static_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));// 当前选中的技能
            if (skillData)
            {
                const char *msg = CCString::createWithFormat(ASK_RESET_TALENT,skillData->getIndex())->getCString();
                Controller::getInstance()->sendMessage(msg,true);
            }
        }
        else if (m_eConsumePurpose == kJewelConsumeForBuyChallenge)
        {
            const char *msg = CCString::createWithFormat(ASK_BOSS_DUNGEON_BUY)->getCString();
            Controller::getInstance()->sendMessage(msg,true);
        }
        else if(m_eConsumePurpose == kJewelConsumeForBuyPray)
        {
            const char *msg = CCString::createWithFormat(ASK_WORLD_BOSS_PRAY)->getCString();
            Controller::getInstance()->sendMessage(msg,true);
        }
        else if(m_eConsumePurpose == kJewelConsumeForBuyQuickEnter)
        {
            const char *msg = CCString::createWithFormat(ASK_WORLD_BOSS_CLEARCD)->getCString();
            Controller::getInstance()->sendMessage(msg,true);
        }
    }
}

void PopUpUI::helpButtonCallBack(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        Button *button = static_cast<Button *>(pSender);
        ImageView *imageView_0 = static_cast<ImageView *>(button->getChildByName("ImageView_0"));
        imageView_0->setVisible(true);
        imageView_0->setEnabled(true);
        imageView_0->setTouchEnabled(true);
        if (getUIType() == kUIBag)
        {
            YYSprite *player = dynamic_cast<YYSprite *>(getChildByTag(TAG_UI_PLAYER));
            if (player)
            {
                player->setVisible(false);
            }
        }
    }
}

void PopUpUI::hideHelpInterface(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        ImageView *imageView_0 = static_cast<ImageView *>(pSender);
        imageView_0->setVisible(false);
        imageView_0->setEnabled(false);
        imageView_0->setTouchEnabled(false);
        if (getUIType() == kUIBag)
        {
            if (strcmp(m_pBagPageButton->getName(), "Button0") == 0)
            {
                YYSprite *player = dynamic_cast<YYSprite *>(getChildByTag(TAG_UI_PLAYER));
                if (player)
                {
                    player->setVisible(true);
                }
            }
        }
    }
}

#pragma mark 聊天

void PopUpUI::menuChatMessageCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        //发送时隐藏表情按钮
        UIPanel* pPanel04 = static_cast<UIPanel *>(m_pUITouchFacePop->getWidgetByName("Panel04"));
        pPanel04->setVisible(false);
        pPanel04->setEnabled(false);
        
        if(std::string(m_pAccountTF->getText()).compare("") == 0)
        {
            return;
        }
        const char * message = NULL;
        const char* strInput =m_pAccountTF->getText();
        
        //屏蔽敏感词
        CCArray* pArrNickName = GlobalData::getInstance()->getNickNameArray();
        uint32_t pArrNickNameCount = pArrNickName->count();
        for (uint32_t i = 0; i < pArrNickNameCount; i++)
        {
            const char* pElemWord = ((CCString*)pArrNickName->objectAtIndex(i))->getCString();
            if(strstr(strInput,pElemWord) != NULL)
            {
                m_pAccountTF->setText("***");
                break;
            }
        }
        if(m_nChannelIndex == 1)
        {   //世界
            message = CCString::createWithFormat(SEND_CHAT_ALL,m_pAccountTF->getText())->getCString();
            Controller::getInstance()->sendMessage(message,false);
        }
        else if (m_nChannelIndex == 0 || m_nChannelIndex == 2)
        { //城市
            message = CCString::createWithFormat(SEND_CHAT_CITY,m_pAccountTF->getText())->getCString();
            Controller::getInstance()->sendMessage(message,false);
        }
        m_pAccountTF->setText("");
    }
}

void PopUpUI::menuChatFaceCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIImageView* imgView = static_cast<UIImageView*>(pSender);
        int tag = imgView->getTag()+1;
        const char * strHasInput = m_pAccountTF->getText();
        const char * inPut = CCString::createWithFormat("%s#f%0.2d",strHasInput,tag)->getCString();
        m_pAccountTF->setText(inPut);
        UIPanel* pPanel04 = static_cast<UIPanel *>(m_pUITouchFacePop->getWidgetByName("Panel04"));
        pPanel04->setVisible(false);
        pPanel04->setEnabled(false);
    }
}

void PopUpUI::btnChatFaceCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_pUITouchFacePop)
        {
            UIPanel* pPanel04 = static_cast<UIPanel *>(m_pUITouchFacePop->getWidgetByName("Panel04"));
            bool bVisible = pPanel04->isVisible();
            pPanel04->setVisible(!bVisible);
            pPanel04->setEnabled(!bVisible);
        }
    }
}

void PopUpUI::menuChatSceneTypeCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIButton *pButton = (UIButton *)pSender;
        int nTag = pButton->getTag();
        m_nChannelIndex = nTag;
        chatSceneBtnBg(); //切换按钮选中状态
        drawChatting();//刷新聊天页面
    }
}

void PopUpUI::chatSceneBtnBg()
{
    if (m_pChatingLayer)
    {
        m_pChatingLayer->removeAllChildren();
    }
    Layout* pPanel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    for (int i = 0; i < CHAT_SCENE_TYPE_NUM; i++)
    {
        CCString *pStr = CCString::createWithFormat("Button%i_0", i);
        UIButton *pButton = static_cast<UIButton *>(pPanel1->getChildByName(pStr->getCString()));
        std::string filePath;
        if (i == m_nChannelIndex)
        {
            pButton->setTouchEnabled(false);
            filePath = BUTTON1;
        }
        else
        {
            pButton->setTouchEnabled(true);
            filePath = BUTTON0;
        }
        pButton->loadTextureNormal(filePath.c_str());
    }
}

void PopUpUI::initChattingUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIChatting);
    //Controller::getInstance()->getScene()->getControlUILayer()->getMainMenu()->setEnabled(false);
    //Controller::getInstance()->getScene()->getControlUILayer()->getMainMenuChat()->setEnabled(false);
    
    m_nChannelIndex = 0;
    string str = getFilePath("420",kDirTypeUI, kFileSuffixPng);
    m_pAccountTF = cocos2d::extension::CCEditBox::create(CCSizeMake(400, 50), cocos2d::extension::CCScale9Sprite::create(str.c_str()));
    m_pAccountTF->retain();
    m_pAccountTF->setFont("Helvetica-Bold",32);
    m_pAccountTF->setFontColor(ccRED);
    m_pAccountTF->setPlaceHolder(g_sInputHere);
    m_pAccountTF->setPlaceholderFontColor(ccWHITE);
    m_pAccountTF->setMaxLength(70);
    m_pAccountTF->setReturnType(cocos2d::extension::kKeyboardReturnTypeDone);
    m_pAccountTF->setDelegate(this);
    
    m_pChatingLayer = YYLayer::create();
    m_pChatingLayer->setContentSize(CCSizeMake(CHATTING_WIDTH, 480));
    m_pChatingLayer->setPosition(m_cAdjustScreenPosition);
    addChild(m_pChatingLayer, 1);
    
    m_pUITouchGroup = cocos2d::gui::TouchGroup::create();
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    addChild(m_pUITouchGroup);
    std::string filePath = CHAT_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout* pPanel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton *pButtonClose = static_cast<UIButton *>(pPanel0->getChildByName("Button_closed"));
    pButtonClose->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    Layout* pPanel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    for (int i = 0; i < CHAT_SCENE_TYPE_NUM; i++)
    {
        CCString *pStr = CCString::createWithFormat("Button%i_0", i);
        UIButton *pButton = static_cast<UIButton *>(pPanel1->getChildByName(pStr->getCString()));
        if (!pButton)
            continue;
        pButton->setTag(i);
        pButton->addTouchEventListener(this, toucheventselector(PopUpUI::menuChatSceneTypeCallback));
    }
    Layout* pPanel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    UIImageView *pImageView0 = static_cast<UIImageView *>(pPanel2->getChildByName("ImageView0"));
    pImageView0->addNode(m_pAccountTF);
    
    UIButton *pButtonSendMsg = static_cast<UIButton *>(pPanel2->getChildByName("Button0"));
    pButtonSendMsg->setTouchEnabled(true);
    pButtonSendMsg->addTouchEventListener(this, toucheventselector(PopUpUI::menuChatMessageCallback));
    
    UIImageView *pButtonSendFace = static_cast<UIImageView *>(pPanel2->getChildByName("Button1"));
    pButtonSendFace->addTouchEventListener(this, toucheventselector(PopUpUI::btnChatFaceCallback));
    
    chatSceneBtnBg();
    
    //屏蔽scrillView 用于列表滚动显示 暂时不用
    UIScrollView* scrollViewList = static_cast<UIScrollView*>(pPanel2->getChildByName("ScrollView_0"));
    if (scrollViewList)
    {
        scrollViewList->setVisible(false);
        scrollViewList->setEnabled(false);
    }
    
    //单独添加聊天页面"NewProject_6.json"
    m_pUITouchFacePop = TouchGroup::create();
    CC_ASSERT(m_pUITouchFacePop);
    addChild(m_pUITouchFacePop, 2);
    filePath = POP_UI_RESOURCE_FILE_NAME;
    m_pUITouchFacePop->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    if (m_pUITouchFacePop)
    {
        //屏蔽所有panel
        Layout *pPanel = static_cast<Layout *>(m_pUITouchFacePop->getWidgetByName("Panel"));
        for (int i = 0; i < pPanel->getChildrenCount(); i++)
        {
            Widget * widget= static_cast<Widget *>(pPanel->getChildren()->objectAtIndex(i));
            widget->setEnabled(false);
            widget->setVisible(false);
            widget->setTouchEnabled(false);
        }
        
        UIPanel* pPanel04 = static_cast<UIPanel *>(m_pUITouchFacePop->getWidgetByName("Panel04"));
        UIScrollView* scrollView = static_cast<UIScrollView*>(pPanel04->getChildByName("ScrollView_0_0"));
        if (scrollView)
        {
            for (int i =0; i < scrollView->getChildrenCount(); i++)
            {
                UIImageView* imgView = static_cast<UIImageView*>(scrollView->getChildren()->objectAtIndex(i));
                imgView->setTag(i);
                imgView->setTouchEnabled(true);
                imgView->addTouchEventListener(this, toucheventselector(PopUpUI::menuChatFaceCallback));
            }
        }
    }
    drawChatting();
}

void PopUpUI::drawChatting()
{
    while (m_pChannelBoxArray[m_nChannelIndex]->count() > 7)  // 消息数超过指定数得删除
    {
        m_pChannelBoxArray[m_nChannelIndex]->removeObjectAtIndex(0);
    }
    if(m_pChatingLayer && m_pChannelBoxArray[m_nChannelIndex])
    {
        m_pChatingLayer->removeAllChildren();
        m_pChatingLayer->setPosition(m_cAdjustScreenPosition);  //重置layer的位置
        CCArray* pArray = CCArray::createWithCapacity(10);
        float layerHeight =0.0f;
        uint16_t  arrSpriteLineCon[10] = {0}; //记录每个sprite 所占行数
        float fpDataLineHei = 0.0f;   //单行高度
        uint8_t  nLayerLineCounter = 0; //layer内数据总行数
        for(int i = 0 ;i < m_pChannelBoxArray[m_nChannelIndex]->count();++i)
        {
            YYChatData* pChatData = (YYChatData*)m_pChannelBoxArray[m_nChannelIndex]->objectAtIndex(i);
            cocos2d::extension::CCScale9Sprite* pSprite = NULL;
            string str = getFilePath("94",kDirTypeUI,kFileSuffixPng);
            pSprite = cocos2d::extension::CCScale9Sprite::create(str.c_str());
            pSprite->setAnchorPoint(ccp(0,1));
            std::vector<FontInfo>::iterator it;
			for (it = pChatData->ChatArray.begin(); it != pChatData->ChatArray.end(); it++)
            {
                FontInfo pFontInfo = (*it);
                if(pFontInfo.type == 1)
                {
                    CCLabelTTF* pLabel = CCLabelTTF::create();
                    pLabel->setString(pFontInfo.name.c_str());
                    pLabel->setAnchorPoint(ccp(0,1));
                    pLabel->setColor(pFontInfo.color);
                    pLabel->setPosition(pFontInfo.point);
                    pLabel->setFontName(pFontInfo.fontName.c_str());
                    pLabel->setFontSize(pFontInfo.fontSize);
                    pSprite->addChild(pLabel);
                }
                else if(pFontInfo.type == 2)
                {
                    CCSprite* pFace= CCSprite::create(pFontInfo.name.c_str());
                    pFace->setAnchorPoint(ccp(0,1));
                    pFace->setPosition(pFontInfo.point);
                    pFace->setScale(1.5f);  //精灵图标缩放
                    pSprite->addChild(pFace);
                }
            }
            pArray->addObject(pSprite);
            arrSpriteLineCon[i] = pChatData->getLineAmount();
            
            pSprite->setContentSize(CCSize(880,pChatData->getLineAmount() * pChatData->getLineHeight() + CHATTING_FONT_OFFSET));  // 界面宽度是880
            for(int k = 1;k < pSprite->getChildrenCount();++k)  //确定sprite内node位置
            {
                CCNode* pNode = (CCNode*)(pSprite->getChildren()->objectAtIndex(k));
                pNode->setPosition(ccpAdd(pNode->getPosition(), ccp(0,pChatData->getLineAmount() * pChatData->getLineHeight() + CHATTING_FONT_OFFSET)));
            }
            layerHeight += pSprite->getContentSize().height;
            
            fpDataLineHei = pChatData->getLineHeight();  //单行高度
            
            nLayerLineCounter+=pChatData->getLineAmount();
        }
        //    if(layerHeight <= CHATTING_HEIGHT)
        {
            layerHeight = CHATTING_HEIGHT;
        }
        m_pChatingLayer->setContentSize(CCSize(CHATTING_WIDTH, layerHeight)); // 880 是整体区域宽度
        //  CCLOG("layer 总行数: %d",nLayerLineCounter);
        for(int i=0 ;i < pArray->count();++i)
        {
            cocos2d::extension::CCScale9Sprite* pBox = (CCScale9Sprite*)pArray->objectAtIndex(i);
            if (arrSpriteLineCon[i] > 1)  //多行数据特殊处理
            {
                layerHeight-=(arrSpriteLineCon[i] -1)*fpDataLineHei;
            }
            
            pBox->setAnchorPoint(ccp(0.0f, 0.0f));
            pBox->setPosition(ccp(30.0f,layerHeight + 60.0f));
            m_pChatingLayer->addChild(pBox,1);
            
            if (arrSpriteLineCon[i] > 1) //多行数据特殊处理
            {
                layerHeight+=(arrSpriteLineCon[i] -1)*fpDataLineHei;
            }
            layerHeight -=(pBox->getContentSize().height);  //减去上一个sprite 的 contentSize
        }
        if (nLayerLineCounter > 7) //如果显示行数大于7行
        {
            m_pChatingLayer->setPosition(ccpAdd(m_pChatingLayer->getPosition(), ccp(0,(nLayerLineCounter-7)*fpDataLineHei))); //调整图层整体位置
            
            uint8_t u8TmpCountLine = 0;
            for (int i = pArray->count()-1; i >= 0; i--)   //隐藏多出node元素
            {
                cocos2d::extension::CCScale9Sprite* pBox = (CCScale9Sprite*)pArray->objectAtIndex(i);
                uint8_t u8PBoxLine = pBox->getContentSize().height / fpDataLineHei;
                u8TmpCountLine+=u8PBoxLine;
                if (u8TmpCountLine <=7)
                {
                    pBox->setVisible(true);
                }
                else
                {
                    pBox->setVisible(false);
                }
            }
        }
    }
}

void PopUpUI::drawChattingBox()
{
    if(m_pChannelBoxArray[kChattingPrivatePage]->count())
    {
        CCScrollView* pScrollView = Controller::getInstance()->getScene()->getControlUILayer()->getChatBoxScrollView();
        CCNode* pNode = (CCNode*)pScrollView->getContainer();
        for(int i = 1; i < pNode->getChildrenCount();)
        {
            pNode->removeChild((CCNode*)pNode->getChildren()->objectAtIndex(i));
        }
        YYChatData* pChatData = (YYChatData*)m_pChannelBoxArray[kChattingPrivatePage]->lastObject();
        std::vector<FontInfo>::iterator it;
        for (it = pChatData->ChatArray.begin(); it != pChatData->ChatArray.end(); it++)
        {
            FontInfo pFontInfo = (*it);
            if(pFontInfo.type == 1)
            {
                CCLabelTTF* pLabel = CCLabelTTF::create();
                pLabel->setString(pFontInfo.name.c_str());
                pLabel->setAnchorPoint(ccp(0,1));
                pLabel->setColor(pFontInfo.color);
                pLabel->setPosition(pFontInfo.point);
                pLabel->setFontName(pFontInfo.fontName.c_str());
                pLabel->setFontSize(pFontInfo.fontSize);
                pNode->addChild(pLabel);
            }
            else if(pFontInfo.type == 2)
            {
                CCSprite* pFace= CCSprite::create(pFontInfo.name.c_str());
                pFace->setAnchorPoint(ccp(0,1));
                pFace->setPosition(pFontInfo.point);
                pFace->setScale(1.5f);  //精灵图标缩放
                pNode->addChild(pFace);
            }
        }
        
        for(int k = 1;k < pNode->getChildrenCount();++k)
        {
            CCNode* pChild = (CCNode*)(pNode->getChildren()->objectAtIndex(k));
            pChild->setPosition(ccpAdd(pChild->getPosition(), ccp(0,pChatData->getLineAmount()*pChatData->getLineHeight() + CHATTING_FONT_OFFSET)));
            
        }
    }
}

void PopUpUI::drawChattingNotice()
{
    if(m_pChannelBoxArray[kChattingNoticePage]->count() && !m_bIsScroll)
    {
        CCLayerColor* pLayer = (CCLayerColor*)Controller::getInstance()->getScene()->getControlUILayer()->getNoticeLayer();
        pLayer->setVisible(true);
        YYChatData* pChatData = (YYChatData*)m_pChannelBoxArray[kChattingNoticePage]->objectAtIndex(0);
        float width = pLayer->getContentSize().width;
        float height = pLayer->getContentSize().height/2;
        
        CCLayer* pLayerBox = CCLayer::create();
        pLayer->addChild(pLayerBox);
        std::vector<FontInfo>::iterator it;
        for (it = pChatData->ChatArray.begin(); it != pChatData->ChatArray.end(); it++)
        {
            FontInfo pFontInfo = (*it);
            if(pFontInfo.type == 1)
            {
                CCLabelTTF* pLabel = CCLabelTTF::create();
                pLabel->setString(pFontInfo.name.c_str());
                pLabel->setAnchorPoint(ccp(0,0.5));
                pLabel->setColor(pFontInfo.color);
                pLabel->setFontName(pFontInfo.fontName.c_str());
                pLabel->setFontSize(pFontInfo.fontSize);
                pLabel->setPosition(ccp(width,height));
                width += pLabel->getContentSize().width;
                pLayerBox->addChild(pLabel);
            }
            else if(pFontInfo.type == 2)
            {
                CCSprite* pFace= CCSprite::create(pFontInfo.name.c_str());
                pFace->setAnchorPoint(ccp(0,0.5));
                pFace->setPosition(ccp(width,height));
                width += pFace->getContentSize().width;
                pLayerBox->addChild(pFace);
            }
        }
        
        CCActionInterval*  move = CCMoveBy::create(10, ccp(-width , 0));
        CCFiniteTimeAction*  action = CCSequence::create(move,CCCallFuncND::create(this, callfuncND_selector(PopUpUI::updateNoticeCall),(void*)0), NULL);
        pLayerBox->runAction(action);
        m_bIsScroll = true;
    }
}

void PopUpUI::updateNoticeCall(CCNode* pSender,void* i)
{
    CCLayerColor* pNoticeLayer = (CCLayerColor*)Controller::getInstance()->getScene()->getControlUILayer()->getNoticeLayer();
    pNoticeLayer->setVisible(false);
    CCLayer* pLayer =(CCLayer*)pSender;
    pLayer->removeFromParent();
    m_pChannelBoxArray[kChattingNoticePage]->removeObjectAtIndex(0);
    m_bIsScroll = false;
    drawChattingNotice();
}

void PopUpUI::dispatchChatMessage(cocos2d::CCArray *array)
{
    int i = 0;
    const char* msgType = static_cast<CCString *>(array->objectAtIndex(i++))->getCString();  //频道类型
    std::string strProperty = static_cast<CCString *>(array->objectAtIndex(i++))->getCString();  //频道私有属性 没有返回0
    std::string strContent  = static_cast<CCString *>(array->objectAtIndex(i++))->getCString();  //频道内容

    YYChatData* pChatData = NULL;
    kChattingType chatType;
    float fontSizeHeight = 0;
    CCLabelTTF* channelLabel = CCLabelTTF::create(g_sWorld, DEFAULT_FONT_NAME, CHATTING_FONT_SIZE);
    fontSizeHeight = channelLabel->getContentSize().height;

    if(strcmp(msgType, "10") == 0)       //世界消息
    {
        if (strcmp(strProperty.c_str(),"0")) //频道私有属性不为"0"
        {
            int i = 0;
            CCArray * tmpContent = split_string(strProperty.c_str(), ';');
            std::string strProperty1 = static_cast<CCString *>(tmpContent->objectAtIndex(i++))->getCString();  //玩家名称
            strContent = strProperty1 + ":" + strContent;
        }
        
        chatType = kChattingWorldType;
        std::string tipsString = std::string(g_sWorld);
        std::string path = tipsString + strContent;
        pChatData = split_string(path.c_str(), CHATTING_WIDTH, CHATTING_FONT_SIZE, 10, fontSizeHeight,CHATTING_FONT_SIZE,DEFAULT_FONT_NAME,chatType,'#');
        getChannelBoxArray()[kChattingWorldPage]->addObject(pChatData);
        getChannelBoxArray()[kChattingNoticePage]->addObject(pChatData); //世界消息添加到公告消息
    }
    else  if(strcmp(msgType, "11") == 0) //城市消息
    {
        if (strcmp(strProperty.c_str(),"0")) //频道私有属性不为"0"
        {
            int i = 0;
            CCArray * tmpContent = split_string(strProperty.c_str(), ';');
            std::string strProperty1 = static_cast<CCString *>(tmpContent->objectAtIndex(i++))->getCString();  //地点
            std::string strProperty2  = static_cast<CCString *>(tmpContent->objectAtIndex(i++))->getCString();  //玩家名称
            strContent = "<" + strProperty1 + ">" + strProperty2 + ":" + strContent;
        }
        
        chatType = kChattingCityType;
        std::string tipsString = std::string(g_sCity);
        std::string path = tipsString + strContent;
        
        pChatData = split_string(path.c_str(), CHATTING_WIDTH, CHATTING_FONT_SIZE, 10, fontSizeHeight,CHATTING_FONT_SIZE,DEFAULT_FONT_NAME,chatType,'#');
        getChannelBoxArray()[kChattingCityPage]->addObject(pChatData);
        getChannelBoxArray()[kChattingWorldPage]->addObject(pChatData); //城市信息添加到世界信息中
        getChannelBoxArray()[kChattingNoticePage]->addObject(pChatData);//城市消息添加公告消息
    }
    else if(strcmp(msgType, "00") == 0) //系统消息
    {
         chatType = kChattingSystemType;
         std::string tipsString = std::string(g_sSystem);
         ccColor3B color1 = ccWHITE; //材料颜色
         ccColor3B color2 = ccWHITE; //材料颜色
         strContent = parseChattingMaterialContent(strContent,color1,color2);  // 解析材料颜色
         std::string path = tipsString + strContent;
         pChatData = split_string(path.c_str(), CHATTING_WIDTH, CHATTING_FONT_SIZE, 10, fontSizeHeight,CHATTING_FONT_SIZE,DEFAULT_FONT_NAME,chatType,'#',color1,color2);
         getChannelBoxArray()[kChattingSystemPage]->addObject(pChatData);
    }
    else  if(strcmp(msgType, "01") == 0) //自己可见
    {
        chatType = kChattingPriviteType;
        ccColor3B color1 = ccWHITE; //材料颜色
        ccColor3B color2 = ccWHITE; //材料颜色
        strContent = parseChattingMaterialContent(strContent,color1,color2);  // 解析材料颜色
        std::string path = strContent;
        pChatData = split_string(path.c_str(), CHATTING_WIDTH, CHATTING_FONT_SIZE, 10, fontSizeHeight,CHATTING_FONT_SIZE,DEFAULT_FONT_NAME,chatType,'#',color1,color2);
        getChannelBoxArray()[kChattingPrivatePage]->addObject(pChatData);
    }
    else  if(strcmp(msgType, "02") == 0) //公告信息
    {
        chatType = kChattingNoticeType;
        ccColor3B color1 = ccWHITE; //材料颜色
        ccColor3B color2 = ccWHITE; //材料颜色
        strContent = parseChattingMaterialContent(strContent,color1,color2);  // 解析材料颜色
        std::string path = strContent;
        pChatData = split_string(path.c_str(), CHATTING_WIDTH, CHATTING_FONT_SIZE, 10, fontSizeHeight,CHATTING_FONT_SIZE,DEFAULT_FONT_NAME,chatType,'#',color1,color2);
        getChannelBoxArray()[kChattingNoticePage]->addObject(pChatData);
    }
 
    getChannelBoxArray()[kChattingAllPage]->addObject(pChatData);  //聊天UI界面 综合消息
    
    //聊天UI界面
    if(Controller::getInstance()->getGameStatus() == kStatusUI && getUIType() == kUIChatting)
    {
        drawChatting();
    }
    //主城 新手引导
    if(Controller::getInstance()->getSceneType() == MAIN_CITY
       || Controller::getInstance()->getSceneType() == NEWBIE_TASK_ZONE)
    {
        drawChattingBox();     //自己可见
        drawChattingNotice();  //公告信息
    }
}

std::string PopUpUI::parseChattingMaterialContent(std::string content, ccColor3B &color1, ccColor3B &color2)
{
    const char * src = content.c_str();
    if(src != NULL)
    {
        std::vector<std::string> vec_result;
        std::vector<bool> vec_flag;
        char *start = (char *)src;
        char *end   = NULL;
        int  count  = 0;
        int  nLen = strlen(src);
        bool bSwitch = true;
        bool bFind = false;    //是否有材料类型
        while (count < nLen)
        {
            if((end = strchr(start, '[')) != NULL && bSwitch)
            {
                std::string buffer(start, end - start);
                vec_result.push_back(buffer);
                vec_flag.push_back(false);
                start = end + 1;
                count+=strlen(buffer.c_str())+1;
                bSwitch = false;
                bFind = true;
            }
            else if((end = strchr(start, ']')) != NULL && !bSwitch)
            {
                std::string buffer(start, end - start);
                vec_result.push_back(buffer);
                vec_flag.push_back(true);
                start = end + 1;
                count+=strlen(buffer.c_str())+1;
                bSwitch = true;
            }
            else
            {
                end = (char*)src + nLen;
                std::string buffer(start, end - start);
                vec_result.push_back(buffer);
                vec_flag.push_back(false);
                count+=strlen(buffer.c_str());
            }
        }
        
        std:: string strContent = "";
        ccColor3B color[2] = {ccWHITE, ccWHITE};
        int nColorIdx = 0;
        for (int i = 0; i < vec_result.size(); i++)
        {
            std::string str = vec_result.at(i);
            bool b = vec_flag.at(i);
            if (b)
            {
                    CCArray* arr = split_string(str.c_str(), ';');
                    int i = 0;
                    std::string index = static_cast<CCString *>(arr->objectAtIndex(i++))->getCString();    //材料id
                    std::string strColor = static_cast<CCString *>(arr->objectAtIndex(i++))->getCString(); //颜色索引
                    std::string name = static_cast<CCString *>(arr->objectAtIndex(i++))->getCString();     //材料名称
                    int nColor = atoi(strColor.c_str());
                    color[nColorIdx++] = getRGBValue(nColor);
                    strContent +=( "[" + name + "]");
            }
            else
            {
                     strContent += str;
            }
        }
        color1 = color[0];
        color2 = color[1];
        if (bFind)
        {
            content = strContent;
        }
    }
    return content;
}

void PopUpUI::exitChatting()
{
    if (m_pChatingLayer)
    {
        removeChild(m_pChatingLayer);
        m_pChatingLayer = NULL;
    }
    if (m_pUITouchFacePop)
    {
        removeChild(m_pUITouchFacePop);
        m_pUITouchFacePop = NULL;
    }
}


#pragma mark 商城

void PopUpUI::initShopUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    m_eGameStatePreviousForShop = Controller::getInstance()->getGameStatus();
    m_eKUITypePreviousForShop = getUIType();
    if (m_eGameStatePreviousForShop == kStatusUI)
    {
        m_pUITouchGroup->setTouchEnabled(false);
        m_pUIShopTouchGroup = m_pUITouchGroup;
    }
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIShop);
    Controller::getInstance()->getScene()->setPopUpUIDelayTime(0);
    setCurShopTabType(kShopPageInit);
    
    setIsShopInit(false);
    //add layer to parent
    m_pUITouchGroup = TouchGroup::create();
    if(!m_pUITouchGroup)
        return;
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = SHOP_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    //init shop panel content
    Layout* pPanel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(pPanel0 != NULL)
    {
        UIButton* pButtonClose = static_cast<UIButton*>(pPanel0->getChildByName("Button_closed"));
        if(pButtonClose != NULL)
            pButtonClose->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopCloseCallback));
    }
    
    //refreshShopItemList(kShopHotPage);
    
    Layout* pPanel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    if(pPanel2 != NULL)
    {
        UIButton* pButtonCharge = static_cast<UIButton*>(pPanel2->getChildByName("Button"));
        if(pButtonCharge != NULL)
        {
            //pButtonCharge->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopOpenCharge));
            //fch 暂时屏蔽充值
            pButtonCharge->setEnabled(false);
        }
        Layout* pPanel2_0 = static_cast<Layout*>(pPanel2->getChildByName("Panel2_0"));
        if (pPanel2_0)
        {
            UILabel* item_desc = static_cast<UILabel*>(pPanel2_0->getChildByName("TextArea2"));
            if(item_desc != NULL)
            {
                item_desc->setText(g_sShopItemDesInfo);            //物品描述信息
            }
        }
        UILabel* playerTotalMoneyType = static_cast<UILabel*>(pPanel2->getChildByName("TextArea0"));
        if(playerTotalMoneyType != NULL)
        {
            UILabel* playerTotalMoneyValue = static_cast<UILabel*>(playerTotalMoneyType->getChildByName("TextArea00"));
            if(playerTotalMoneyValue != NULL)
            {
                int nGold = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getGold();
                playerTotalMoneyValue->setText(CCString::createWithFormat("%d", nGold)->getCString());         //玩家金币数
            }
        }
        UILabel* playerTotalJewType = static_cast<UILabel*>(pPanel2->getChildByName("TextArea1"));
        if(playerTotalJewType != NULL)
        {
            UILabel* playerTotalJewValue = static_cast<UILabel*>(playerTotalJewType->getChildByName("TextArea10"));
            if(playerTotalJewValue != NULL)
            {
                int nJew = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getJewel();
                playerTotalJewValue->setText(CCString::createWithFormat("%d", nJew)->getCString());           //玩家钻石数
            }
        }
    }
    
    Layout* pPanel3 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel3"));
    if(pPanel3 != NULL)
    {
        for(int i = 0; i < kShopPageNum; i++)
        {
            UIButton* pButtonType = static_cast<UIButton*>(pPanel3->getChildByName(CCString::createWithFormat("Button%d", i)->getCString()));
            if(pButtonType != NULL)
            {
                pButtonType->setTag(i);
                pButtonType->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopChangeType));
            }
        }
    }
    Layout* shop = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel"));
    if(!shop)
        return;
    Layout* pPanel = static_cast<Layout*>(shop->getChildByName("Panel1"));
    if(!pPanel)
        return;
    UIScrollView* pView = static_cast<UIScrollView*>(pPanel->getChildByName("ScrollView"));
    if(!pView)
        return;
    
    pView->setBounceEnabled(true);
    Layout *pScrollViewInnerLayer = pView->getInnerContainer();
    m_fScrollViewLayerY = pScrollViewInnerLayer->getPositionY();
    m_bIsFirstEnterShop = true;
    Controller::getInstance()->sendMessage(ASK_SHOP_LIST,true);
}

void PopUpUI::drawShopContent()
{
    CCDictionary *pShopDictionary = getShopContentDictionary();
    int nIndex = 0;
    for (int i = 0; i < kShopPageNum; i++)
    {
        CCArray *pShopArray = (CCArray *)pShopDictionary->objectForKey(CCString::createWithFormat("tab%d", i)->getCString());
        if (pShopArray->count() > 0)
        {
            nIndex = i;
            break;
        }
    }
    ShopPageType kType = kShopHotPage;
    switch (nIndex) {
        case 1:
            kType = kShopEquip;
            break;
        case 2:
            kType = kShopItem;
            break;
        case 3:
            kType = kShopGem;
            break;
        case 4:
            kType = kShopGiftPage;
            break;
        default:
            break;
    }
    if (m_kShopPageType != kShopPageInit)
        kType = m_kShopPageType;
    refreshShopItemList(kType);
    setIsShopInit(true);
}
void PopUpUI::menuShopCloseCallback(CCObject* pSender, TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CLOSEUI_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (isTouchUnable(pSender, type,false))
        {
            return;
        }
        if(!getIsShopDetailOpen())
        {
            Controller::getInstance()->setGameStatus(m_eGameStatePreviousForShop);
            setUIType(m_eKUITypePreviousForShop);
            reSetShopDataInfo();
            exitShop();
            if (m_pUITouchGroup) {
                removeChild(m_pUITouchGroup);
                m_pUITouchGroup = NULL;
            }
            if (m_eGameStatePreviousForShop == kStatusUI)
            {
                m_pUITouchGroup = m_pUIShopTouchGroup;
                m_pUIShopTouchGroup = NULL;
                m_pUITouchGroup->setTouchEnabled(true);
            }
        }
    }
}

void PopUpUI::menuShopChangeType(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if(!getIsShopDetailOpen() && getIsShopInit())
        {
            Widget *widget = static_cast<Widget *>(pSender);
            ShopPageType eType = (ShopPageType)widget->getTag();
            if((eType >= kShopHotPage) && (eType < kShopPageNum))
            {
                refreshShopItemList(eType);
                //clean item desc content
                Layout* shop = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel"));
                if(shop)
                {
                    Layout* pPanel2 = static_cast<Layout*>(shop->getChildByName("Panel2"));
                    if(pPanel2)
                    {
                        Layout* pPanel2_0 = static_cast<Layout*>(pPanel2->getChildByName("Panel2_0"));
                        if(pPanel2_0)
                        {
                            UILabel* item_desc = static_cast<UILabel*>(pPanel2_0->getChildByName("TextArea2"));
                            if(item_desc)
                            {
                                item_desc->setText(g_sShopItemDesInfo);            //物品描述信息
                            }
                        }
                    }
                    Layout* pPanel = static_cast<Layout*>(shop->getChildByName("Panel1"));
                    if(pPanel)
                    {
                        UIScrollView* pView = static_cast<UIScrollView*>(pPanel->getChildByName("ScrollView"));
                        if(pView)
                        {
                            pView->scrollToTop(1, true);
                        }
                    }
                }
            }
        }
    }
}

void PopUpUI::menuShopBuy(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if(isTouchUnable(pSender,type))
            return;
        if(!getIsShopDetailOpen())
        {
            UIWidget* pButtonBuy = (UIWidget *)pSender;
            int nIndex = pButtonBuy->getTag();
            ShopPageType eType = getCurShopTabType();
            
            //find data
            CCDictionary *pShopContent = getShopContentDictionary();
            if(!pShopContent)
                return;
            CCArray *pTypeContent = (CCArray *)pShopContent->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
            if(!pTypeContent)
                return;
            
            //data
            YYShopData *pItemData = NULL;
            for(unsigned i = 0; i < pTypeContent->count(); i++)
            {
                pItemData = (YYShopData *)pTypeContent->objectAtIndex(i);
                if(!pItemData)
                    continue;
                if((int)pItemData->getIndex() == nIndex)
                    break;
                else
                    pItemData = NULL;
            }
            if(!pItemData)
                return;
            
            //set index
            setCurBuyShopItemIndex(nIndex);
            
            m_pUITouchGroup->setTouchEnabled(false);
            Layout *pPanel = (Layout *)getPopWindow("Panel_0", "Panel_a", true);
            if (!pPanel)
                return;
            //icon
            UIImageView* itemIcon =static_cast<UIImageView*>(pPanel->getChildByName("ImageView_icon"));
            itemIcon->removeAllChildren();
            if(pItemData->getPriceMoney() > 0 || pItemData->getPriceJew() > 0)
            {
                std::string path = getFilePath(pItemData->getIconName(),kDirTypeIcon,kFileSuffixPng);
                UIImageView *pImage = UIImageView::create();
                pImage->loadTexture(path.c_str());    //物品icon
                itemIcon->addChild(pImage);
            }
            
            //name
            UILabel* itemName = static_cast<UILabel*>(pPanel->getChildByName("TextArea0"));
            if(itemName != NULL)
            {
                itemName->setText(pItemData->getName().c_str());             //物品名字
            }
            //price
            std::string strPrice = "";
            std::string strMoneyTypeIconFileName = "";
            UILabel* itemPrice = static_cast<UILabel*>(pPanel->getChildByName("TextArea1"));
            itemPrice->removeAllChildren();
            if(itemPrice != NULL)
            {
                itemPrice->setText(" ");
                if(pItemData->getPriceMoney() > 0)
                {
                    strPrice = CCString::createWithFormat("%d", pItemData->getPriceMoney())->getCString();
                    setCurBuyMoneyType(0);
                    strMoneyTypeIconFileName = getFilePath("300",kDirTypeUI,kFileSuffixPng);
                }
                else if(pItemData->getPriceJew() > 0)
                {
                    strPrice = CCString::createWithFormat("%d", pItemData->getPriceJew())->getCString();
                    setCurBuyMoneyType(1);
                    strMoneyTypeIconFileName = getFilePath("298",kDirTypeUI,kFileSuffixPng);
                }
                Label *label = Label::create();
                label->setTextHorizontalAlignment(kCCTextAlignmentLeft);
                CCSize labelSize = CCSizeMake(80, 20);
                label->setTextAreaSize(labelSize);
                label->setColor(ccWHITE);
                label->setFontSize(20);
                label->setText(strPrice);
                label->setAnchorPoint(ccp(0, 0.5));
                label->setPosition(ccp(30, 15));
                label->setTag(10);
                itemPrice->addChild(label);
            }
            //add price type icon
            if(strMoneyTypeIconFileName != "")
            {
                UIImageView *pPriceIcon = UIImageView::create();
                if(pPriceIcon != NULL)
                {
                    pPriceIcon->loadTexture(strMoneyTypeIconFileName.c_str());
                    pPriceIcon->setAnchorPoint(ccp(0.5, 0.5));
                    pPriceIcon->setPosition(ccp(0, 12));
                    pPriceIcon->setZOrder(10);
                    itemPrice->addChild(pPriceIcon);
                }
            }
            
            //num
            UIImageView* pBuyNumBG =static_cast<UIImageView*>(pPanel->getChildByName("ImageView"));
            pBuyNumBG->removeAllChildren();
            if(pBuyNumBG != NULL)
            {
                Label *label = Label::create();
                label->setTextHorizontalAlignment(kCCTextAlignmentLeft);
                CCSize labelSize = CCSizeMake(100, 20);
                label->setTextAreaSize(labelSize);
                label->setColor(ccWHITE);
                label->setFontSize(20);
                label->setText("1");
                label->setAnchorPoint(ccp(0, 0.5));
                label->setPosition(ccp(-(pBuyNumBG->getSize().width / 2.0) + 10, 0));
                pBuyNumBG->addChild(label, 0, 10);
                
            }
            setBuyNum(1);
            
            //num decrease,add,max button
            UIButton* pBtnDecreaseNum = static_cast<UIButton*>(pPanel->getChildByName("Button0"));
            if(pBtnDecreaseNum != NULL)
            {
                pBtnDecreaseNum->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopItemDescreaseNum));
            }
            UIButton* pBtnAddNum = static_cast<UIButton*>(pPanel->getChildByName("Button1"));
            if(pBtnAddNum != NULL)
            {
                pBtnAddNum->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopItemAddNum));
            }
            
            UIButton* pBtnMaxNum = static_cast<UIButton*>(pPanel->getChildByName("Button2"));
            if(pBtnMaxNum != NULL)
            {
                pBtnMaxNum->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopItemMaxNum));
            }
            
            //ok,canel button
            UIButton* pBtnOK = static_cast<UIButton*>(pPanel->getChildByName("Button01"));
            if(pBtnOK != NULL)
            {
                pBtnOK->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopItemBuyOK));
            }
            UIButton* pBtnCanel = static_cast<UIButton*>(pPanel->getChildByName("Button00"));
            if(pBtnCanel != NULL)
            {
                pBtnCanel->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopItemBuyCanel));
            }
            setIsShopDetailOpen(true);
        }
    }
}

void PopUpUI::menuShopOpenCharge(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if(isTouchUnable(pSender,type))
            return;
        if(!getIsShopDetailOpen())
        {
            //exitUI();
            gotoCharge();
        }
    }
}

void PopUpUI::refreshShopItemList(ShopPageType eType)
{
    if(getCurShopTabType() == eType)
        return;
    setCurShopTabType(eType);
    
    CCDictionary *pShopContent = getShopContentDictionary();
    if(!pShopContent)
        return;
    
    Layout* shop = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel"));
    if(shop)
    {
        Layout* pPanel = static_cast<Layout*>(shop->getChildByName("Panel1"));
        if(!pPanel)
            return;
        UIScrollView* pView = static_cast<UIScrollView*>(pPanel->getChildByName("ScrollView"));
        if(!pView)
            return;
        
        CCArray *pTypeContent = (CCArray *)pShopContent->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
        
        if(!pTypeContent)
            return;
        
//        Layout *panel = pView->getInnerContainer();
//        panel->setPositionY(m_fScrollViewLayerY);
        
        for(int i = 0; i < MAX_PAGE_SHOP_ITEM_COUNT; i++)
        {
            UIImageView* pShopItemRoot = static_cast<UIImageView*>(pView->getChildByName(CCString::createWithFormat("ImageView%d", i)->getCString()));
            if(!pShopItemRoot)
                continue;
            pShopItemRoot->setEnabled(true);
            pShopItemRoot->setVisible(true);
            if (i >= pTypeContent->count())
            {
                pShopItemRoot->setEnabled(false);
                pShopItemRoot->setVisible(false);
                continue;
            }
            //data
            YYShopData *pItemData = (YYShopData *)pTypeContent->objectAtIndex(i);//i-nStart
            if(!pItemData)
                continue;
            
            //details
            UILabel* itemName = static_cast<UILabel*>(pShopItemRoot->getChildByName("TextArea0"));
            if(itemName != NULL)
            {
                itemName->setText(pItemData->getName().c_str());             //物品名字
            }
            //price
            string strMoneyTypeIconFileName = "";
            UILabel* itemPrice = static_cast<UILabel*>(pShopItemRoot->getChildByName("TextArea1"));
            if(itemPrice != NULL)
            {
                itemPrice->setText(" ");
                if(pItemData->getPriceMoney() > 0)
                {
                    itemPrice->setText(CCString::createWithFormat("%d", pItemData->getPriceMoney())->getCString());            //物品价格
                    strMoneyTypeIconFileName = getFilePath("300",kDirTypeUI,kFileSuffixPng);
                }
                else if(pItemData->getPriceJew() > 0)
                {
                    itemPrice->setText(CCString::createWithFormat("%d", pItemData->getPriceJew())->getCString());
                    strMoneyTypeIconFileName = getFilePath("298",kDirTypeUI,kFileSuffixPng);
                }
            }
            //add price type icon
            if(strMoneyTypeIconFileName != "")
            {
                UIImageView *pPriceIcon = (UIImageView *)itemPrice->getChildByTag(20);
                if(!pPriceIcon)
                {
                    pPriceIcon = UIImageView::create();
                    if(pPriceIcon != NULL)
                    {
                        pPriceIcon->loadTexture(strMoneyTypeIconFileName.c_str());
                        pPriceIcon->setAnchorPoint(ccp(0.5, 0.5));
                        pPriceIcon->setPosition(ccp(0, 0));
                        pPriceIcon->setZOrder(10);
                        pPriceIcon->setTag(20);
                        itemPrice->addChild(pPriceIcon);
                    }
                }
                else
                {
                    pPriceIcon->loadTexture(strMoneyTypeIconFileName.c_str());
                }
            }
            
            UIImageView* itemIcon =static_cast<UIImageView*>(pShopItemRoot->getChildByName("ImageView"));
            if(itemIcon != NULL)
            {
                std::string path = getFilePath(pItemData->getIconName(), kDirTypeIcon, kFileSuffixPng);
                UIImageView *pImage = (UIImageView *)itemIcon->getChildByTag(10);
                if (!pImage)
                {
                    UIImageView *pImage = UIImageView::create();
                    pImage->setTag(10);
                    pImage->loadTexture(path.c_str());    //物品icon
                    itemIcon->addChild(pImage);
                }
                else
                {
                    pImage->loadTexture(path.c_str());    //物品icon
                }
                UILabel* numLabel = static_cast<UILabel *>(itemIcon->getChildByName("Label_0"));
                if (numLabel)
                {
                    numLabel->setZOrder(10);
                    numLabel->setText(CCString::createWithFormat("%d",pItemData->getNum())->getCString());
                }
            }
            UIButton* pButtonBuy = static_cast<UIButton*>(pShopItemRoot->getChildByName("Button"));
            if(pButtonBuy != NULL)
            {
                pButtonBuy->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopBuy));
                pButtonBuy->setTag(pItemData->getIndex());
            }
            
            pShopItemRoot->setTag(pItemData->getIndex());
            pShopItemRoot->addTouchEventListener(this, toucheventselector(PopUpUI::menuShopItemShowDetail));
        }
        
        //修改按钮按下状态
        Layout* pPanelMenu = static_cast<Layout*>(shop->getChildByName("Panel3"));
        if(!pPanelMenu)
            return;
        string str;
        for(int i=0; i<kShopPageNum; i++)
        {
            UIButton* pButtonType = static_cast<UIButton*>(pPanelMenu->getChildByName(CCString::createWithFormat("Button%d", i)->getCString()));
            if(!pButtonType)
                continue;
            if(i == (int)eType)
            {
                str = BUTTON1;
            }
            else
            {
                str = BUTTON0;
            }
            pButtonType->loadTextureNormal(str.c_str());
        }
    }
}

void PopUpUI::menuShopItemShowDetail(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
            return;
        if(!getIsShopDetailOpen())
        {
            ShopPageType eType = getCurShopTabType();
            int nIndex = ((UIImageView *)pSender)->getTag();
            
            CCArray *pArr = (CCArray *)getShopContentDictionary()->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
            if(!pArr)
                return;
            YYShopData *pTarData = NULL;
            for(unsigned i=0; i<pArr->count(); i++)
            {
                pTarData = (YYShopData *)pArr->objectAtIndex(i);
                if(!pTarData)
                    continue;
                if((int)pTarData->getIndex() == nIndex)
                    break;
                else
                    pTarData = NULL;
            }
            if(!pTarData)
                return;
            
            Layout* shop = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel"));
            if(!shop)
                return;
            
            Layout* pPanel2 = static_cast<Layout*>(shop->getChildByName("Panel2"));
            if(!pPanel2)
                return;
            
            Layout* pPanel2_0 = static_cast<Layout*>(pPanel2->getChildByName("Panel2_0"));
            if(!pPanel2)
                return;
            
            UILabel* item_desc = static_cast<UILabel*>(pPanel2_0->getChildByName("TextArea2"));
            if(item_desc != NULL)
            {
                item_desc->setText(pTarData->getDesc().c_str());            //物品描述信息
            }
        }
    }
}

void PopUpUI::menuShopItemAddNum(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        if (m_nBuyNum < 100)
        {
            ++m_nBuyNum;
            Layout *pPanel = (Layout *)getPopWindow("Panel_0", "Panel_a", true);
            if (!pPanel)
                return;
            UIImageView* pBuyNumBG =static_cast<UIImageView*>(pPanel->getChildByName("ImageView"));
            UILabel *pLabel = (UILabel *)pBuyNumBG->getChildByTag(10);
            CCString *pStrNum = CCString::createWithFormat("%d", m_nBuyNum);
            pLabel->setText(pStrNum->getCString());
            menuShopItemUpdatePriceNum();
        }
    }
}
void PopUpUI::menuShopItemDescreaseNum(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        if (m_nBuyNum > 1)
        {
            --m_nBuyNum;
            Layout *pPanel = (Layout *)getPopWindow("Panel_0", "Panel_a", true);
            if (!pPanel)
                return;
            UIImageView* pBuyNumBG =static_cast<UIImageView*>(pPanel->getChildByName("ImageView"));
            UILabel *pLabel = (UILabel *)pBuyNumBG->getChildByTag(10);
            CCString *pStrNum = CCString::createWithFormat("%d", m_nBuyNum);
            pLabel->setText(pStrNum->getCString());
            menuShopItemUpdatePriceNum();
        }
    }
}
void PopUpUI::menuShopItemMaxNum(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        m_nBuyNum = 100;
        Layout *pPanel = (Layout *)getPopWindow("Panel_0", "Panel_a", true);
        if (!pPanel)
            return;
        UIImageView* pBuyNumBG =static_cast<UIImageView*>(pPanel->getChildByName("ImageView"));
        UILabel *pLabel = (UILabel *)pBuyNumBG->getChildByTag(10);
        CCString *pStrNum = CCString::createWithFormat("%d", m_nBuyNum);
        pLabel->setText(pStrNum->getCString());
        menuShopItemUpdatePriceNum();
    }
}

void PopUpUI::menuShopItemUpdatePriceNum()
{
    ShopPageType eType = getCurShopTabType();
    CCArray *pArr = (CCArray *)getShopContentDictionary()->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
    if(!pArr)
        return;
    YYShopData *pItemData = NULL;
    for(unsigned i=0; i<pArr->count(); i++)
    {
        pItemData = (YYShopData *)pArr->objectAtIndex(i);
        if(!pItemData)
            continue;
        if((int)pItemData->getIndex() == getCurBuyShopItemIndex())
            break;
        else
            pItemData = NULL;
    }
    if(!pItemData)
        return;
    int nPriceGold = (int)(pItemData->getPriceMoney());
    int nPriceJew = (int)(pItemData->getPriceJew());
    Layout *pPanel = (Layout *)getPopWindow("Panel_0", "Panel_a", true);
    if (pPanel)
    {
        UILabel* itemPrice = static_cast<UILabel*>(pPanel->getChildByName("TextArea1"));
        if (itemPrice)
        {
            UILabel* label = (UILabel*)itemPrice->getChildByTag(10);
            if (label)
            {
                if  (nPriceJew > 0)
                {
                    label->setText(CCString::createWithFormat("%d",nPriceJew * getBuyNum())->getCString());
                }
                else if(nPriceGold > 0)
                {
                    label->setText(CCString::createWithFormat("%d",nPriceGold * getBuyNum())->getCString());
                }
            }
        }
    }
}

void PopUpUI::addBuyNum(int num, bool bIsToMax)
{
    int nMaxNum = 0;
    ShopPageType eType = getCurShopTabType();
    CCArray *pArr = (CCArray *)getShopContentDictionary()->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
    if(!pArr)
        return;
    YYShopData *pTarData = NULL;
    for(unsigned i = 0; i<pArr->count(); i++)
    {
        pTarData = (YYShopData *)pArr->objectAtIndex(i);
        if(!pTarData)
            continue;
        if((int)pTarData->getIndex() == getCurBuyShopItemIndex())
            break;
        else
            pTarData = NULL;
    }
    if(!pTarData)
        return;
    
    int nPlayerGold = (int)(pTarData->getPriceMoney());
    int nPlayerJew = (int)(pTarData->getPriceJew());
    int nGold = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getGold();
    int nJew = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getJewel();
    
    if(getCurBuyMoneyType() == 0)
    {
        if(nPlayerGold != 0)
            nMaxNum = nGold / nPlayerGold;
    }
    else if(getCurBuyMoneyType() == 1)
    {
        if(nPlayerJew != 0)
            nMaxNum = nJew / nPlayerJew;
    }
    
    Layout* pShopBuyDetail = static_cast<Layout*>(m_pUITouchGroup->getWidgetByTag(TAG_UI_SHOPBUY_DETAIL));
    if(!pShopBuyDetail)
        return;
    Layout* pPanel = NULL;
    if(eType == kShopGiftPage)
        pPanel = static_cast<Layout*>(pShopBuyDetail->getChildByName("Panel_b"));
    else
        pPanel = static_cast<Layout*>(pShopBuyDetail->getChildByName("Panel_a"));
    if(!pPanel)
        return;
    UIImageView* pBuyNumBG =static_cast<UIImageView*>(pPanel->getChildByName("ImageView"));
    if(!pBuyNumBG)
        return;
    CCEditBox *pEdit = (CCEditBox *)pBuyNumBG->getChildByTag(13);
    if(!pEdit)
        return;
    
    int nTarNum = 0;
    if(bIsToMax == false)
    {
        int nNum = atoi(pEdit->getText());
        if(nNum + num < 0)
            nTarNum = 0;
        else if(nNum + num > nMaxNum)
            nTarNum = nMaxNum;
        else
            nTarNum = nNum + num;
    }
    else
    {
        nTarNum = nMaxNum;
    }
    setBuyNum(nTarNum);
    
    //更新界面显示
    pEdit->setText(CCString::createWithFormat("%d", nTarNum)->getCString());
    
}

void PopUpUI::menuShopItemBuyOK(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        //钱不够
        ShopPageType eType = getCurShopTabType();
        CCArray *pArr = (CCArray *)getShopContentDictionary()->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
        if(!pArr)
            return;
        YYShopData *pItemData = NULL;
        for(unsigned i=0; i<pArr->count(); i++)
        {
            pItemData = (YYShopData *)pArr->objectAtIndex(i);
            if(!pItemData)
                continue;
            if((int)pItemData->getIndex() == getCurBuyShopItemIndex())
                break;
            else
                pItemData = NULL;
        }
        if(!pItemData)
            return;
        int nPriceGold = (int)(pItemData->getPriceMoney());
        int nPriceJew = (int)(pItemData->getPriceJew());
        if  (nPriceJew > 0)
        {
            Layout *pPanel_Buy = (Layout *)getPopWindow("Panel_0", "Panel_a", true);
            pPanel_Buy->setTouchEnabled(false);
            setConsumeJewelTips(kJewelConsumeForCommodity, nPriceJew * getBuyNum());
        }
        else if (nPriceGold > 0)
            shopBuyLogic();
    }
}
void PopUpUI::shopBuyLogic()
{
    ShopPageType eType = getCurShopTabType();
    CCArray *pArr = (CCArray *)getShopContentDictionary()->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
    if(!pArr)
        return;
    YYShopData *pItemData = NULL;
    for(unsigned i=0; i<pArr->count(); i++)
    {
        pItemData = (YYShopData *)pArr->objectAtIndex(i);
        if(!pItemData)
            continue;
        if((int)pItemData->getIndex() == getCurBuyShopItemIndex())
            break;
        else
            pItemData = NULL;
    }
    if(!pItemData)
        return;
    int nPriceGold = (int)(pItemData->getPriceMoney());
    int nPriceJew = (int)(pItemData->getPriceJew());
    int nGold = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getGold();
    int nJew = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getJewel();
    if(getBuyNum() > 0)
    {
        if((((getCurBuyMoneyType() == 0) && (getBuyNum() * nPriceGold > nGold))
            || ((getCurBuyMoneyType() == 1) && (getBuyNum() * nPriceJew > nJew)))
           && pItemData->getDateType() == 0)
        {
            getPopWindow("Panel_0", "Panel_a", false);
            if (getCurBuyMoneyType() == 0)
            {
                ((YYScene*)getParent())->showTips(g_sLackGold, kTipGoldChargeForShopUI);
            }
            else if (getCurBuyMoneyType() == 1)
            {
                ((YYScene*)getParent())->showTips(g_sLackJew, kTipJewCharge);
            }
            return;
        }
        shopBuy(getCurBuyShopItemIndex(), getBuyNum(), ((getCurBuyMoneyType() == 0) ? false : true));
    }
    m_pUITouchGroup->setTouchEnabled(true);
    getPopWindow("Panel_0", "Panel_a", false);
    setIsShopDetailOpen(false);
}

void PopUpUI::menuShopItemBuyCanel(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        //关闭子页面
        m_pUITouchGroup->setTouchEnabled(true);
        getPopWindow("Panel_0", "Panel_a", false);
        //reset
        reSetShopDataInfo();
    }
}

void PopUpUI::gotoCharge()
{
    goChargeUI(kChargeInit);
}

void PopUpUI::menuShopItemShowFriendList(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        //TODO
        CCLog("---- show friend list ----");
    }
}
void PopUpUI::exitShop()
{
    for(int i = 0; i < kShopPageNum; i++)
    {
        CCArray *pShopArray = (CCArray *)m_pShopContentDictionary->objectForKey(CCString::createWithFormat("tab%d", i)->getCString());
        if(pShopArray)
            pShopArray->removeAllObjects();
    }
}
void PopUpUI::shopBuy(int index, int num, bool bUseJew)
{
    ShopPageType eType = getCurShopTabType();
    
    CCArray *pArr = (CCArray *)getShopContentDictionary()->objectForKey(CCString::createWithFormat("tab%d", (int)eType)->getCString());
    if(!pArr)
        return;
    YYShopData *pTarData = NULL;
    for(unsigned i=0; i<pArr->count(); i++)
    {
        pTarData = (YYShopData *)pArr->objectAtIndex(i);
        if(!pTarData)
            continue;
        if((int)pTarData->getIndex() == index)
            break;
    }
    if(!pTarData)
        return;
    
    uint32_t nGold = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getGold();
    uint32_t nJew = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getJewel();
    
    if (pTarData->getDateType() == 0)
    {
        if(bUseJew == false)
        {
            if(pTarData->getPriceMoney() * num <= nGold)
            {
                //TODO
                Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->setGold(nGold - pTarData->getPriceMoney() * num);
                //return;
            }
        }
        else
        {
            if(pTarData->getPriceJew() * num <= nJew)
            {
                //TODO
                Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->setJewel(nJew - pTarData->getPriceJew() * num);
                //return;
            }
        }
    }
    
    //send buy request to server
    // CCString *strBuyMsg = CCString::createWithFormat("s B%d_%d_%s", index, num, ((bUseJew == true) ? "jew" : "money"));
    CCString *strBuyMsg = CCString::createWithFormat(ASK_SHOP_BUG, index, num);
    if(!strBuyMsg)
        return;
    Controller::getInstance()->sendMessage(strBuyMsg->getCString(),false);
    
    setBuyNum(0);
    setCurBuyShopItemIndex(-1);
}

void PopUpUI::updateShopUIPropertyInfo()
{
    Layout* shop = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel"));
    if(!shop)
        return;
    Layout* pPanel2 = static_cast<Layout*>(shop->getChildByName("Panel2"));
    if(pPanel2 != NULL)
    {
        UILabel* playerTotalMoneyType = static_cast<UILabel*>(pPanel2->getChildByName("TextArea0"));
        if(playerTotalMoneyType != NULL)
        {
            UILabel* playerTotalMoneyValue = static_cast<UILabel*>(playerTotalMoneyType->getChildByName("TextArea00"));
            if(playerTotalMoneyValue != NULL)
            {
                int nGold = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getGold();
                playerTotalMoneyValue->setText(CCString::createWithFormat("%d", nGold)->getCString());         //玩家金币数
            }
        }
        UILabel* playerTotalJewType = static_cast<UILabel*>(pPanel2->getChildByName("TextArea1"));
        if(playerTotalJewType != NULL)
        {
            UILabel* playerTotalJewValue = static_cast<UILabel*>(playerTotalJewType->getChildByName("TextArea10"));
            if(playerTotalJewValue != NULL)
            {
                int nJew = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getJewel();
                playerTotalJewValue->setText(CCString::createWithFormat("%d", nJew)->getCString());           //玩家钻石数
            }
        }
    }
}
void PopUpUI::goShopUI(ShopPageType kTypePage)
{
    reSetShopDataInfo();
    initShopUI();
    m_kShopPageType = kTypePage;
}
void PopUpUI::reSetShopDataInfo()
{
    m_kShopPageType = kShopPageInit;
    setBuyNum(0);
    setCurBuyShopItemIndex(-1);
    setIsShopDetailOpen(false);
}
void PopUpUI::setShopDataInfo(cocos2d::CCArray *array)
{
    if((!array) || (array->count() <= 0))
    {
        CCLog("Shop Content empty from server!");
        return;
    }

    if(!m_pShopContentDictionary)
    {
        CCLog("Shop data error!");
        return;
    }
    
    for(unsigned i= 0; i < array->count(); i++)
    {
        CCString *tpString = (CCString *)array->objectAtIndex(i);
        if(!tpString)
            continue;
        CCArray *tpArr = split_string(tpString->getCString(), ';');
        if((!tpArr) || (tpArr->count() < 14))
            continue;
        
        YYShopData *pUnitData = YYShopData::create();
        if(!pUnitData)
            continue;
        
        //index
        CCString *strIndex = (CCString *)tpArr->objectAtIndex(0);
        if(strIndex != NULL)
            pUnitData->setIndex(strIndex->intValue());
        
        //tab
        int nPage = -1;
        CCString *strTab = (CCString *)tpArr->objectAtIndex(1);
        if(strTab != NULL)
        {
            pUnitData->setShopItemType((ShopPageType)(strTab->intValue()));
            nPage = strTab->intValue();
        }
        
        //name
        CCString *strName = (CCString *)tpArr->objectAtIndex(2);
        if(strName != NULL)
            pUnitData->setName(strName->getCString());
        
        //icon
        CCString *strIcon = (CCString *)tpArr->objectAtIndex(3);
        if(strIcon != NULL)
            pUnitData->setIconName(strIcon->getCString());
        
        //desc
        CCString *strDesc = (CCString *)tpArr->objectAtIndex(4);
        if(strDesc != NULL)
            pUnitData->setDesc(strDesc->getCString());
        
        //num
        CCString *strNum = (CCString *)tpArr->objectAtIndex(6);
        if(strNum != NULL)
            pUnitData->setNum(strNum->intValue());
        
        //price money
        CCString *strMoneyType = (CCString *)tpArr->objectAtIndex(7);
        CCString *strMoneyPrice = (CCString *)tpArr->objectAtIndex(8);
        if(strMoneyType != NULL)
        {
            pUnitData->setPriceMoney(0);
            pUnitData->setPriceJew(0);
            if (strMoneyType->intValue() == 1)
                pUnitData->setPriceMoney(strMoneyPrice->intValue());
            else if (strMoneyType->intValue() == 0)
                pUnitData->setPriceJew(strMoneyPrice->intValue());
        }
        
        //date limit
        CCString *strDateLimitStart = (CCString *)tpArr->objectAtIndex(9);
        if (strDateLimitStart != NULL)
            pUnitData->setDateContentStart(strDateLimitStart->getCString());
        
        CCString *strDateLimitEnd = (CCString *)tpArr->objectAtIndex(10);
        if (strDateLimitEnd != NULL)
            pUnitData->setDateContentEnd(strDateLimitEnd->getCString());
        
        CCString *strTimeLimitStart = (CCString *)tpArr->objectAtIndex(11);
        if (strTimeLimitStart != NULL)
            pUnitData->setDateContentEnd(strTimeLimitStart->getCString());
        
        CCString *strTimeLimitEnd = (CCString *)tpArr->objectAtIndex(12);
        if (strTimeLimitEnd != NULL)
            pUnitData->setDateContentEnd(strTimeLimitEnd->getCString());
        
        //amount
        CCString *strAmount = (CCString *)tpArr->objectAtIndex(13);
        if(strAmount != NULL)
            pUnitData->setAmount(strAmount->intValue());
        
        //check tab
        if((nPage < kShopHotPage) || (nPage >= kShopPageNum))
            continue;
        
        //add to shop content
        CCArray *pShopArray = (CCArray *)m_pShopContentDictionary->objectForKey(CCString::createWithFormat("tab%d", nPage)->getCString());
        if(pShopArray != NULL)
        {
            pShopArray->addObject(pUnitData);
        }
    }
}
#pragma mark 连续登录奖励

void PopUpUI::initLoginAwardUI()
{
    do{
        ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
        Controller::getInstance()->setGameStatus(kStatusUI);
        setUIType(kUILoginAward);
        m_u8AwardIndex = -1;
        m_pUITouchGroup = TouchGroup::create();
        CC_ASSERT(m_pUITouchGroup);
        addChild(m_pUITouchGroup);
        m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
        std::string filePath = LOGINAWARD_UI_RESOURCE_FILE_NAME;
        Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str());
        CC_BREAK_IF(!mainWidget);
        mainWidget->setTag(TAG_UI_LOGINAWARD);
        m_pUITouchGroup->addWidget(mainWidget);
        
        //start ui init
        Layout *pPanel = static_cast<Layout*>(mainWidget->getChildByName("Panel0"));
        CC_BREAK_IF(!pPanel);
        
        //close button
        UIButton *pBtnClose = static_cast<UIButton*>(pPanel->getChildByName("Button_closed"));
        if(pBtnClose)
        {
            pBtnClose->addTouchEventListener(this, toucheventselector(PopUpUI::closePopUpUI));
        }
        
    }while (0);
    Controller::getInstance()->sendMessage(ASK_LOGIN_AWARD, true);
}


void PopUpUI::menuGetLoginAward(CCObject* pSender, ::cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        //get tag
        Button* button = (Button *)pSender;
        if(!button)
            return;
        const char * msg = CCString::createWithFormat(ASK_LOGIN_GET_AWARD, button->getTag())->getCString();
        Controller::getInstance()->sendMessage(msg,false);
    }
}

bool PopUpUI::isGetLoginAward()
{
    CCArray *arr = static_cast<CCArray *>(GlobalData::getInstance()->getLoginAwardDict()->objectForKey(m_u8AwardIndex));
    if (arr)
    {
        for(int i = 0; i < arr->count(); i++)
        {
            LoginAwardData *pInfo = static_cast<LoginAwardData *>(arr->objectAtIndex(i));
            if(pInfo && pInfo->getStatus() == kLoginAwardStatusGotten)
            {
                return true;
            }
        }
    }
    return false;
}

void PopUpUI::setLoginAwardData(CCArray *array)
{
    m_u8AwardIndex = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
    CCArray *arr = static_cast<CCArray *>(GlobalData::getInstance()->getLoginAwardDict()->objectForKey(m_u8AwardIndex));
    if (arr)
    {
        //panel1
        Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
        ScrollView *scrollView = static_cast<ScrollView *>(panel1->getChildByName("ScrollView_0"));
        for (int i = 0; i < arr->count(); i++)
        {
            LoginAwardData *loginAwardData = static_cast<LoginAwardData *>(arr->objectAtIndex(i));
            loginAwardData->setStatus(kLoginAwardStatusGetable);
            UIPanel * panel = static_cast<UIPanel *>(scrollView->getChildByName(CCString::createWithFormat("Panel2_%d",i)->getCString()));
            for (int j = 0; j < loginAwardData->getItemInfoArray()->count(); j++)
            {
                CCArray *array = split_string(static_cast<CCString *>(loginAwardData->getItemInfoArray()->objectAtIndex(j))->getCString(),',');
                ImageView * iconBG = static_cast<ImageView *>(panel->getChildByName(CCString::createWithFormat("ImageView%d",j)->getCString()));
                ImageView *icon = ImageView::create();
                iconBG->addChild(icon);
                iconBG->setTouchEnabled(true);
//                iconBG->addTouchEventListener(this, toucheventselector(PopUpUI::showPropAttributeInterface()));
                const char *fileName = static_cast<CCString *>(array->objectAtIndex(0))->getCString();
                icon->loadTexture(getFilePath(fileName, kDirTypeIcon, kFileSuffixPng).c_str());
                const char * num = static_cast<CCString *>(array->objectAtIndex(1))->getCString();
                Label *label = Label::create();
                label->setAnchorPoint(ccp(1,0));
                label->setPosition(ccp(iconBG->getSize().width / 2 - 5,-iconBG->getSize().height / 2 + 5));
                label->setFontSize(16);
                label->setText(num);
                iconBG->addChild(label);
            }

            Button * button = static_cast<Button *>(panel->getChildByName("Button0"));
            button->addTouchEventListener(this, toucheventselector(PopUpUI::menuGetLoginAward));
            button->setTag(i);
            button->setTitleText(g_sGetable);
            button->setBright(false);
            button->setTouchEnabled(false);
            button->setUserData(loginAwardData);
            if(i < array->count() - 1)
            {
                int status = static_cast<CCString *>(array->objectAtIndex(i + 1))->intValue();
                if (status == kLoginAwardStatusGetable)
                {
                    button->setBright(true);
                    button->setTouchEnabled(true);
                }
                else if(status == kLoginAwardStatusGotten)
                {
                    button->setTitleText(g_sGotten);
                }
                loginAwardData->setStatus((LoginAwardStatus)status);
            }
        }
    }

}

void PopUpUI::updateLoginAwardUI(int index)
{
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
    ScrollView *scrollView = static_cast<ScrollView *>(panel1->getChildByName("ScrollView_0"));
    if (index < 7)
    {
        UIPanel * panel = static_cast<UIPanel *>(scrollView->getChildByName(CCString::createWithFormat("Panel2_%d",index)->getCString()));
        Button * button = static_cast<Button *>(panel->getChildByName("Button0"));
        button->addTouchEventListener(this, toucheventselector(PopUpUI::menuGetLoginAward));
        button->setTitleText(g_sGotten);
        button->setBright(false);
        button->setTouchEnabled(false);
        LoginAwardData *awardData = static_cast<LoginAwardData *>(button->getUserData());
        awardData->setStatus(kLoginAwardStatusGotten);
    }
}

//-------------------------
void PopUpUI::closePopUpUI()
{
    switch (getUIType())
    {
        case kUILoginAward:
            exitUI();
            break;
        case kUISigning:
            if (!getIsSignUpInit()) {
                exitUI();
            }
            break;
        case kUIActivity:
            if (!getIsActivityInit()) {
                exitUI();
            }
            break;
        case kUIShop:
            if (!getIsShopInit()) {
                exitUI();
            }
        case kUIProtoss:
            if (!getIsProtossInit()) {
                exitUI();
            }
            break;
        case kUIAchieve:
            if (!getIsAchieveInit()) {
                exitUI();
            }
            break;
        default:
            break;
    }
    
}

#pragma mark 签到

void PopUpUI::initSignUpUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUISigning);
    Controller::getInstance()->getScene()->setPopUpUIDelayTime(0);

    m_bAwardPosSave = false;
    m_bBoxAwardPosSave = false;
    m_bSignUpSuccessed = false;
    setIsSignUpInit(false);
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = LOGINSIGNUP_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    
    UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(closeButton);
    
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    //help
    UIButton *helpButton = static_cast<UIButton *>(panel0->getChildByName("Button_0"));
    if(helpButton)
    {
        helpButton->addTouchEventListener(this, toucheventselector(PopUpUI::helpButtonCallBack));
        ImageView * image = static_cast<ImageView *>(helpButton->getChildByName("ImageView_0"));
        image->addTouchEventListener(this, toucheventselector(PopUpUI::hideHelpInterface));
        image->setEnabled(false);
    }
    
    Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
    CC_ASSERT(panel1);
    
    Layout *panel1_2 = static_cast<Layout*>(panel1->getChildByName("Panel1_2"));
    CC_ASSERT(panel1_2);
    
    // 签到次数
    ImageView *imageViewTotal = static_cast<ImageView*>(panel1_2->getChildByName("ImageView0"));
    Label *labelSignTotalNum = static_cast<Label*>(imageViewTotal->getChildByName("Label"));
    labelSignTotalNum->setText(CCString::createWithFormat("%s%s%s%s%s%i", g_sTotal, g_sSignUp, g_sNum, g_sCount, g_sSymbol, 0)->getCString());
    // 领取
    Layout *panel1_3 = static_cast<Layout*>(panel1->getChildByName("Panel1_3"));
    CC_ASSERT(panel1_3);
    for (int i = 0; i < MAX_BOX_NUM; i++)
    {
        CCString *pImgViewStr = CCString::createWithFormat("%s%i","ImageView0_", i);
        ImageView *pImgView = static_cast<ImageView *>(panel1_3->getChildByName(pImgViewStr->getCString()));
        CC_ASSERT(pImgView);
        Label *pLabel = static_cast<Label*>(pImgView->getChildByName("Label"));
        CC_ASSERT(pLabel);
        pLabel->setText(CCString::createWithFormat("%s%i%s", g_sTotal, 0, g_sNum)->getCString());
        
        CCString *pButtonStr = CCString::createWithFormat("%s%i","Button0_", i);
        UIButton *pButtonGetAward = static_cast<UIButton *>(panel1_3->getChildByName(pButtonStr->getCString()));
        CC_ASSERT(pButtonGetAward);
        pButtonGetAward->setTitleText(g_sCanNotGet);
    }

    Controller::getInstance()->sendMessage(ASK_SIGNUP_INFO, true);
}

void PopUpUI::setSignUpDateInfo(cocos2d::CCArray *array)
{
    if(!array)
        return;
    
    m_pArraySignUpDate = new CCArray();
    m_pArrayAddSignUpDate = new CCArray();
    m_u8SignUpDay = ((CCString *)array->objectAtIndex(1))->intValue();
    // 签到日期列表
    CCString *pStrSignUp = (CCString *)array->objectAtIndex(2);
    CCArray *pArray = split_string(pStrSignUp->getCString(), ';');
    removeLastObject(pArray);
    for (int i = 0; i < pArray->count(); i++)
    {
        CCString *pStr = (CCString *)pArray->objectAtIndex(i);
        m_pArraySignUpDate->addObject(pStr);
        if (pStr->intValue() == m_u8SignUpDay)
            m_bSignUpSuccessed = true;
    }
    // 未签到日期列表
    for (int i = 0; i < m_u8SignUpDay - 1; i++)
    {
        bool isHaveDay = false;
        for (int j = 0; j < pArray->count(); j++)
        {
            CCString *pStr = (CCString *)pArray->objectAtIndex(j);
            if (pStr->intValue() == (i + 1))
                isHaveDay = true;
        }
        if (!isHaveDay)
        {
            CCString *pStr = CCString::createWithFormat("%i", i + 1);
            m_pArrayAddSignUpDate->addObject(pStr);
        }
    }
    m_u8SignUpNum = m_pArraySignUpDate->count();
    m_u8AddSignUpNum = m_pArrayAddSignUpDate->count();
    m_u8SignUpDayNum = m_u8SignUpNum;
    // 领取宝盒
    m_pArrayBoxAward = new CCArray();
    CCString *pStrBoxAward = (CCString *)array->objectAtIndex(3);
    CCArray *pArrayBoxAward = split_string(pStrBoxAward->getCString(), ';');
    removeLastObject(pArrayBoxAward);
    for (int i = 0; i < pArrayBoxAward->count(); i++)
    {
        YYSignUpBoxAwardData *pBoxAward = YYSignUpBoxAwardData::create();
        pBoxAward->setIndex(i);
        CCString *pStr = (CCString *)pArrayBoxAward->objectAtIndex(i);
        if (pStr->intValue() == 1)
        {
            pBoxAward->setSignuPAwardStatus(kStatusAwardCanGet);
        }
        else if (pStr->intValue() == 2)
        {
            pBoxAward->setSignuPAwardStatus(kStatusAwardHaveGot);
        }
        else
            pBoxAward->setSignuPAwardStatus(kStatusAwardCanNotGet);
        m_pArrayBoxAward->addObject(pBoxAward);
    }
    setIsSignUpInit(true);
    updateSignUpUI();
}

// 签到完成
void PopUpUI::setSignUpFinishDataInfo(cocos2d::CCArray *array)
{
    if(!array)
        return;
    getSignUpActionStatus() == kActionSignUp ? drawSignUpFinishUI() : drawSignUpAddUI();
    transfSignUpUI();
}

void PopUpUI::setSignUpBoxAwardDataInfo(cocos2d::CCArray *array)
{
    if (!array)
        return;
    drawSignUpBoxAwardUI();
}

void PopUpUI::updateSignUpUI()
{
    // 累计签到
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
    Layout *panel1_2 = static_cast<Layout*>(panel1->getChildByName("Panel1_2"));
    ImageView *imageViewTotal = static_cast<ImageView*>(panel1_2->getChildByName("ImageView0"));
    Label *labelSignTotalNum = static_cast<Label*>(imageViewTotal->getChildByName("Label"));
    CCString *pStrSignUpNum = CCString::createWithFormat("%s%s%s%s%s%i", g_sTotal, g_sSignUp, g_sNum, g_sCount, g_sSymbol, m_u8SignUpNum);
    labelSignTotalNum->setText(pStrSignUpNum->getCString());
    
    // 签到
    UIButton *pButtonSignUp = static_cast<UIButton *>(panel1_2->getChildByName("Button0"));
    CC_ASSERT(pButtonSignUp);
    pButtonSignUp->addTouchEventListener(this, toucheventselector(PopUpUI::menuSignUp));
    
    // 签到完成后按钮的变化
    if (m_bSignUpSuccessed)
    {
        std::string str = getFilePath("212-1", kDirTypeUI, kFileSuffixPng);
        pButtonSignUp->loadTextureNormal(str.c_str());
        pButtonSignUp->loadTexturePressed(str.c_str());
    }
    // 签到奖励
    Layout *panel1_0 = static_cast<Layout*>(panel1->getChildByName("Panel1_0"));
    UIScrollView *pScrollView = static_cast<UIScrollView*>(panel1_0->getChildByName("ScrollView"));

    CCArray *pArraySignUpData = GlobalData::getInstance()->getSignUpDataArray();
    for (int i = 0; i < pArraySignUpData->count(); i++)
    {
        YYSignUpData *pSignUpData = (YYSignUpData *)pArraySignUpData->objectAtIndex(i);
        CCArray *pArrayAwardData = pSignUpData->getArrayAwardData();
        CCString *pImgViewStr = CCString::createWithFormat("%s%i","ImageView_", pSignUpData->getIndex() - 1);
        ImageView *pImgView= static_cast<ImageView*>(pScrollView->getChildByName(pImgViewStr->getCString()));
        for (int j = 0; j < pArrayAwardData->count(); j++)
        {
            AwardData *pAward = (AwardData *)pArrayAwardData->objectAtIndex(j);
            ImageView *pImg = ImageView::create();
            string str = getFilePath(pAward->getIcon(), kDirTypeIcon, kFileSuffixPng);
            pImg->loadTexture(str.c_str());
            pImgView->addChild(pImg, 0, 10);

            UILabel *pLabel = createLabel(CCSizeMake(60, 30), kCCTextAlignmentCenter, 20, ccWHITE);
            CCString *pStrNum = CCString::createWithFormat("x%d", pAward->getNum());
            pLabel->setText(pStrNum->getCString());
            pLabel->setPosition(ccp(pImgView->getSize().width / 2 - 20, -pImgView->getSize().height / 2 + 15));
            pImgView->addChild(pLabel, 0, 20);
        }
    }
    // 已签到
    for (int nIndexSignUpDate = 0; nIndexSignUpDate < m_pArraySignUpDate->count(); nIndexSignUpDate++)
    {
        CCString *pStrSignUpDate = (CCString *)m_pArraySignUpDate->objectAtIndex(nIndexSignUpDate);
        CCString *pImgViewStr = CCString::createWithFormat("%s%i","ImageView_", pStrSignUpDate->intValue() - 1);
        ImageView *pImgView= static_cast<ImageView*>(pScrollView->getChildByName(pImgViewStr->getCString()));
        ImageView *pImgView0= static_cast<ImageView*>(pImgView->getChildByName("ImageView0"));
        setHide(pImgView);
        pImgView0->setVisible(true);
    }
    // 补签
    for (int nIndexAddSignUpDate = 0; nIndexAddSignUpDate < m_pArrayAddSignUpDate->count(); nIndexAddSignUpDate++)
    {
        CCString *pStrAddSignUpDate = (CCString *)m_pArrayAddSignUpDate->objectAtIndex(nIndexAddSignUpDate);
        CCString *pImgViewStr = CCString::createWithFormat("%s%i","ImageView_", pStrAddSignUpDate->intValue() - 1);
        ImageView *pImgView = static_cast<ImageView *>(pScrollView->getChildByName(pImgViewStr->getCString()));
        ImageView *pImgView1 = static_cast<ImageView *>(pImgView->getChildByName("ImageView1"));
        setHide(pImgView);
        ImageView *pImageAward = (ImageView *)pImgView->getChildByTag(10);
        if (pImageAward)
            pImageAward->setVisible(true);
        
        UILabel *pLabel = (UILabel *)pImgView->getChildByTag(20);
        if (pLabel)
            pLabel->setVisible(true);
        
        pImgView1->setVisible(true);
        pImgView1->setPosition(ccp(pImgView->getSize().width / 2 - 10, -pImgView->getSize().height / 2 + 10));
        pImgView1->setZOrder(2);
        
        pImgView->setTag(pStrAddSignUpDate->intValue() - 1);
        pImgView->addTouchEventListener(this, toucheventselector(PopUpUI::menuSignUpAdd));
        
        ImageView *pImgViewDate = static_cast<ImageView *>(pImgView->getChildByName("ImageView"));
        pImgViewDate->setVisible(true);
    }
    // 宝箱
    Layout *panel1_3 = static_cast<Layout*>(panel1->getChildByName("Panel1_3"));
    CCArray *pArrayBoxAward = GlobalData::getInstance()->getSigUpBoxAwardDataArray();
    for (int i = 0; i < pArrayBoxAward->count(); i++)
    {
        YYSignUpBoxAwardData *pBoxAwardData = (YYSignUpBoxAwardData *)pArrayBoxAward->objectAtIndex(i);
        CCString *pImgViewStr = CCString::createWithFormat("%s%i","ImageView0_", i);
        ImageView *pImgView = static_cast<ImageView *>(panel1_3->getChildByName(pImgViewStr->getCString()));
        Label *pLabel = static_cast<Label*>(pImgView->getChildByName("Label"));
        pLabel->setText(CCString::createWithFormat("%s%i%s", g_sTotal, pBoxAwardData->getIndex(), g_sNum)->getCString());
        
        CCString *pButtonStr = CCString::createWithFormat("%s%i","Button0_", i);
        UIButton *pButtonGetAward = static_cast<UIButton *>(panel1_3->getChildByName(pButtonStr->getCString()));
        pButtonGetAward->setTag(i);
        pButtonGetAward->addTouchEventListener(this, toucheventselector(PopUpUI::menuSignUpBoxAward));
        pButtonGetAward->setTitleText(g_sCanNotGet);
        // 宝箱奖励
        AwardData *pAward = pBoxAwardData->getAwardData();
        ImageView *pImgBox = ImageView::create();
        pImgBox->setScale(0.95f);
        string strAppend = getFilePath(pAward->getIcon(), kDirTypeIcon, kFileSuffixPng);
        pImgBox->loadTexture(strAppend.c_str());
        CCString *pImgBoxStr = CCString::createWithFormat("%s%i","ImageView1_", i);
        UIImageView *pImgViewBG = static_cast<ImageView *>(panel1_3->getChildByName(pImgBoxStr->getCString()));
        pImgViewBG->addChild(pImgBox);
   
    }
    for (int i = 0; i < m_pArrayBoxAward->count(); i++)
    {
        YYSignUpBoxAwardData *pBoxAwardData = (YYSignUpBoxAwardData *)m_pArrayBoxAward->objectAtIndex(i);
        CCString *pButtonStr = CCString::createWithFormat("%s%i","Button0_", i);
        UIButton *pButtonGetAward = static_cast<UIButton *>(panel1_3->getChildByName(pButtonStr->getCString()));
        if (pBoxAwardData->getSignuPAwardStatus() == kStatusAwardCanGet)
        {
            pButtonGetAward->setTitleText(g_sHaveNotGet);
        }
        else if (pBoxAwardData->getSignuPAwardStatus() == kStatusAwardHaveGot)
        {
            pButtonGetAward->setTitleText(g_sGotten);
        }
        if (m_u8SignUpDayNum < pBoxAwardData->getIndex() || pBoxAwardData->getSignuPAwardStatus() == kStatusAwardCanNotGet)
        {
            pButtonGetAward->setTitleText(g_sCanNotGet);
        }
    }
}

void PopUpUI::drawSignUpFinishUI()
{
    setUIAwardStatus(kUISignUpAward); // 签到
    m_pUITouchGroup->setTouchEnabled(false);
    Layout *panel_a = (Layout *)getPopWindow("Panel_2", "Panel_a", true);
    UIButton *pButtonSure = static_cast<UIButton *>(panel_a->getChildByName("Button0"));
    CC_ASSERT(pButtonSure);
    pButtonSure->addTouchEventListener(this, toucheventselector(PopUpUI::closeAwardUI));
    
    if (getSignUpActionStatus() == kActionSignUpAdd)
    {
        UIImageView *pImageTitle = static_cast<UIImageView *>(panel_a->getChildByName("ImageView"));
        CC_ASSERT(pImageTitle);
        UILabel *labelSignUp = static_cast<UILabel *>(pImageTitle->getChildByName("TextArea_0"));
        CC_ASSERT(labelSignUp);
        labelSignUp->setVisible(false);
        UILabel *labelSignUpAdd = static_cast<UILabel *>(pImageTitle->getChildByName("TextArea_1"));
        CC_ASSERT(labelSignUpAdd);
        labelSignUpAdd->setVisible(true);
    }
    ImageView *pImageView_0 = static_cast<ImageView *>(panel_a->getChildByName("ImageView_0"));
    ImageView *pImageView_1 = static_cast<ImageView *>(panel_a->getChildByName("ImageView_1"));
    ImageView *pImageView_2 = static_cast<ImageView *>(panel_a->getChildByName("ImageView_2"));
    ImageView *pImageView0 = static_cast<ImageView *>(panel_a->getChildByName("ImageView0"));
    pImageView_0->setVisible(false);
    pImageView_1->setVisible(false);
    pImageView_2->setVisible(false);
    pImageView0->setVisible(false);
    pImageView_0->removeAllChildren();
    pImageView_1->removeAllChildren();
    pImageView_2->removeAllChildren();
    if (!m_bAwardPosSave)
    {
        m_bAwardPosSave = true;
        aWardPoint[0] = pImageView_0->getPosition();
        aWardPoint[1] = pImageView_1->getPosition();
        aWardPoint[2] = pImageView_2->getPosition();
        aWardPoint[3] = pImageView0->getPosition();
    }
    pImageView_0->setPosition(aWardPoint[0]);
    pImageView_1->setPosition(aWardPoint[1]);
    pImageView_2->setPosition(aWardPoint[2]);
    pImageView0->setPosition(aWardPoint[3]);

    int nCountStatic = 0;
    int nCountExtra = 0;
    YYSignUpData* pSignUpAwardData = (YYSignUpData*)GlobalData::getInstance()->getSignUpData(m_u8SignUpDay);
    if (pSignUpAwardData != NULL)
    {
        nCountExtra = pSignUpAwardData->getArrayAwardData()->count();
    }
    CCArray *pArrayStaticAward = GlobalData::getInstance()->getSignUpStaticAwardArray();
    nCountStatic = pArrayStaticAward->count();
    if (nCountStatic == 1 && nCountExtra == 0)
    {
        float moveDis = (abs(pImageView_1->getPosition().x - pImageView_0->getPosition().x) * 5) / 4.0;
        pImageView_0->setPosition(ccp(pImageView_0->getPosition().x + moveDis, pImageView_0->getPosition().y));
    }
    else if (nCountStatic == 1 && nCountExtra == 1)
    {
        float moveDis = (abs(pImageView_1->getPosition().x - pImageView_0->getPosition().x)) / 2.0;
        pImageView_0->setPosition(ccp(pImageView_0->getPosition().x + moveDis, pImageView_0->getPosition().y));
        pImageView_2->setPosition(ccp(pImageView_2->getPosition().x - moveDis, pImageView_2->getPosition().y));
        pImageView0->setPosition(ccp(pImageView0->getPosition().x - moveDis, pImageView0->getPosition().y));
    }
    else if (nCountStatic == 3 && nCountExtra == 0)
    {
        float moveDis = (abs(pImageView_1->getPosition().x - pImageView_0->getPosition().x) * 3) / 4.0;
        pImageView_0->setPosition(ccp(pImageView_0->getPosition().x + moveDis, pImageView_0->getPosition().y));
        pImageView_1->setPosition(ccp(pImageView_1->getPosition().x + moveDis, pImageView_0->getPosition().y));
    }
    for (int i = 0; i < nCountStatic && i < 2; i++)
    {
        AwardData *pAward = (AwardData *)pArrayStaticAward->objectAtIndex(i);
        CCString *pStr = CCString::createWithFormat("ImageView_%i", i);
        ImageView *pImageView = static_cast<ImageView *>(panel_a->getChildByName(pStr->getCString()));
        pImageView->removeAllChildren();
        ImageView *pImg = ImageView::create();
        std::string strIcon = getFilePath(pAward->getIcon(), kDirTypeIcon, kFileSuffixPng);
        pImg->loadTexture(strIcon.c_str());
        pImageView->addChild(pImg);

        UILabel *pTitle= createLabel(CCSizeMake(80, 30), kCCTextAlignmentCenter, 20, ccWHITE);
        pTitle->setPosition(ccp(15, -pImageView->getSize().height * 1 / 2.0 + 15));
        pImageView->addChild(pTitle);
        CCString *pStrNum = CCString::createWithFormat("%d", pAward->getNum());
        pTitle->setText("x" + pStrNum->m_sString);
        pImageView->setVisible(true);
    }
    if (nCountExtra == 1)
    {
        CCArray *pArrayAward = pSignUpAwardData->getArrayAwardData();
        AwardData *pAward = (AwardData *)pArrayAward->objectAtIndex(0);
        CCString *pStr = CCString::createWithFormat("ImageView_%i", 2);
        ImageView *pImageView = static_cast<ImageView *>(panel_a->getChildByName(pStr->getCString()));
        pImageView->removeAllChildren();
        ImageView *pImg = ImageView::create();
        std::string strIcon = getFilePath(pAward->getIcon(), kDirTypeIcon, kFileSuffixPng);
        pImg->loadTexture(strIcon.c_str());
        pImageView->addChild(pImg);

        UILabel *pTitle= createLabel(CCSizeMake(80, 30), kCCTextAlignmentCenter, 20, ccWHITE);
        pTitle->setPosition(ccp(15, -pImageView->getSize().height * 1 / 2.0 + 15));
        pImageView->addChild(pTitle);
        CCString *pStrNum = CCString::createWithFormat("%d", pAward->getNum());
        pTitle->setText("x" + pStrNum->m_sString);
        pImageView->setVisible(true);
        pImageView0->setVisible(true);
    }
}
void PopUpUI::drawSignUpAddUI()
{
    drawSignUpFinishUI();
    setUIAwardStatus(kUISignUpAddAward);
    --m_uSignUpAddDay;
}
void PopUpUI::drawSignUpAddCardInsufficiencyUI()
{
    setUIAwardStatus(kUISignUpAddAward);
    m_pUITouchGroup->setTouchEnabled(false);
    
    Layout *panel_c = (Layout *)getPopWindow("Panel_1", "Panel_c", true);
    UIButton *pButton0 = static_cast<UIButton *>(panel_c->getChildByName("Button0"));
    pButton0->addTouchEventListener(this, toucheventselector(PopUpUI::closeAwardUI));
    
    UIButton *pButton1 = static_cast<UIButton *>(panel_c->getChildByName("Button1"));
    pButton1->addTouchEventListener(this, toucheventselector(PopUpUI::menuSignUpGoShop));
}
void PopUpUI::drawSignUpBoxAwardUI()
{
    setUIAwardStatus(kUIBoxAward); // 宝盒奖励
    m_pUITouchGroup->setTouchEnabled(false);
    Layout *panel_b = (Layout *)getPopWindow("Panel_2", "Panel_b", true);
    UIButton *pButtonSure = static_cast<UIButton *>(panel_b->getChildByName("Button0"));
    CC_ASSERT(pButtonSure);
    pButtonSure->addTouchEventListener(this, toucheventselector(PopUpUI::closeAwardUI));
    
    UIImageView *pImageTitle = static_cast<UIImageView *>(panel_b->getChildByName("ImageView"));
    CC_ASSERT(pImageTitle);
    UILabel *label1 = static_cast<UILabel *>(pImageTitle->getChildByName("TextArea"));
    CC_ASSERT(label1);
    label1->setVisible(false);
    UILabel *label2 = static_cast<UILabel *>(pImageTitle->getChildByName("TextArea_0"));
    CC_ASSERT(label2);
    label2->setVisible(true);
    
    
    ImageView *pImageView_0 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_0"));
    ImageView *pImageView_1 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_1"));
    ImageView *pImageView_2 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_2"));
    pImageView_0->setVisible(false);
    pImageView_1->setVisible(false);
    pImageView_2->setVisible(false);
    if (!m_bBoxAwardPosSave)
    {
        m_bBoxAwardPosSave = true;
        boxAwardPoint[0] = pImageView_0->getPosition();
        boxAwardPoint[1] = pImageView_1->getPosition();
        boxAwardPoint[2] = pImageView_2->getPosition();
    }
    pImageView_0->setPosition(boxAwardPoint[0]);
    pImageView_1->setPosition(boxAwardPoint[1]);
    pImageView_2->setPosition(boxAwardPoint[2]);
    int nCount = 1;
    float moveDis = nCount < 3 ? (abs(pImageView_2->getPosition().x - pImageView_0->getPosition().x) * 1.0 / (nCount + 1)) : 0;
    moveDis = nCount == 2 ? (moveDis - 20) : moveDis;
    CCArray *pArrayBoxAward = GlobalData::getInstance()->getSigUpBoxAwardDataArray();
    for (int i = 0; (i < nCount) && (nCount <= 3); i++)
    {
        CCString *pStr = CCString::createWithFormat("ImageView_%i", i);
        ImageView *pImageView = static_cast<ImageView *>(panel_b->getChildByName(pStr->getCString()));
        pImageView->setVisible(true);
        pImageView->setPosition(ccp(pImageView->getPosition().x + moveDis, pImageView->getPosition().y));
        pImageView->removeAllChildren();
        YYSignUpBoxAwardData *pBoxAwardData = (YYSignUpBoxAwardData *)pArrayBoxAward->objectAtIndex(i);
        AwardData *pAward = pBoxAwardData->getAwardData();
        ImageView *pImgIcon = ImageView::create();
        std::string str = getFilePath(pAward->getIcon(), kDirTypeIcon, kFileSuffixPng);
        pImgIcon->loadTexture(str.c_str());
        pImageView->addChild(pImgIcon);

        UILabel *pTitle= createLabel(CCSizeMake(80, 30), kCCTextAlignmentCenter, 20, ccWHITE);
        pTitle->setPosition(ccp(15, -pImageView->getSize().height * 1 / 2.0 + 15));
        pImageView->addChild(pTitle);
        CCString *pStrNum = CCString::createWithFormat("%d", pAward->getNum());
        pTitle->setText("x" + pStrNum->m_sString);
    }
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
    Layout *panel1_3 = static_cast<Layout*>(panel1->getChildByName("Panel1_3"));
    CCString *pButtonStr = CCString::createWithFormat("%s%i","Button0_", m_u8SignUpBoxAwardBtnTag);
    UIButton *pButtonGetAward = static_cast<UIButton *>(panel1_3->getChildByName(pButtonStr->getCString()));
    pButtonGetAward->setTitleText(g_sGotten);
    
    YYSignUpBoxAwardData *pBoxAwardData = (YYSignUpBoxAwardData *)m_pArrayBoxAward->objectAtIndex(m_u8SignUpBoxAwardBtnTag);
    pBoxAwardData->setSignuPAwardStatus(kStatusAwardHaveGot);
}

void PopUpUI::transfSignUpUI()
{
    // 签到界面信息的变化
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
    Layout *panel1_2 = static_cast<Layout*>(panel1->getChildByName("Panel1_2"));
    ImageView *imageViewTotal = static_cast<ImageView*>(panel1_2->getChildByName("ImageView0"));
    Label *labelSignTotalNum = static_cast<Label*>(imageViewTotal->getChildByName("Label"));
    labelSignTotalNum->setText(CCString::createWithFormat("%s%s%s%s%s%i", g_sTotal, g_sSignUp, g_sNum, g_sCount, g_sSymbol, ++m_u8SignUpDayNum)->getCString());
    
    Layout *panel1_0 = static_cast<Layout *>(panel1->getChildByName("Panel1_0"));
    UIScrollView *pScrollView = static_cast<UIScrollView *>(panel1_0->getChildByName("ScrollView"));
    ImageView *pImageView = static_cast<ImageView *>(pScrollView->getChildByName(CCString::createWithFormat("%s%i", "ImageView_", m_u8SignUpDay - 1)->getCString()));
    setHide(pImageView);
    ImageView *pImageViewGou = static_cast<ImageView *>(pImageView->getChildByName("ImageView0"));
    pImageViewGou->setVisible(true);
    
    // 宝箱奖励
    Layout *panel1_3 = static_cast<Layout*>(panel1->getChildByName("Panel1_3"));
    
    CCArray *pArrayBoxAwardData = GlobalData::getInstance()->getSigUpBoxAwardDataArray();
    for (int i = 0; i < pArrayBoxAwardData->count(); i++)
    {
        YYSignUpBoxAwardData *pBoxAwardData = (YYSignUpBoxAwardData *)pArrayBoxAwardData->objectAtIndex(i);
        CCString *pImgViewStr = CCString::createWithFormat("%s%i","ImageView0_", i);
        ImageView *pImgView = static_cast<ImageView *>(panel1_3->getChildByName(pImgViewStr->getCString()));
        Label *pLabel = static_cast<Label*>(pImgView->getChildByName("Label"));
        pLabel->setText(CCString::createWithFormat("%s%i%s", g_sTotal, pBoxAwardData->getIndex(), g_sNum)->getCString());
        
        CCString *pButtonStr = CCString::createWithFormat("%s%i","Button0_", i);
        UIButton *pButtonGetAward = static_cast<UIButton *>(panel1_3->getChildByName(pButtonStr->getCString()));
        if (m_u8SignUpDayNum >= pBoxAwardData->getIndex())
        {
            pButtonGetAward->setTitleText(g_sHaveNotGet);
        }
        else if (m_u8SignUpDayNum < pBoxAwardData->getIndex())
        {
            pButtonGetAward->setTitleText(g_sCanNotGet);
        }
        if (pBoxAwardData->getSignuPAwardStatus() == kStatusAwardHaveGot)
        {
            pButtonGetAward->setTitleText(g_sGotten);
        }
    }
}

void PopUpUI::menuSignUp(CCObject* pSender, ::cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        setSignUpActionStatus(kActionSignUp);
        CCString *pStrMsg = CCString::createWithFormat(ASK_SIGNUP, m_u8SignUpDay);
        Controller::getInstance()->sendMessage(pStrMsg->getCString(), false);
    }
}

void PopUpUI::menuSignUpAdd(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
            setSignUpActionStatus(kActionSignUpAdd);
            ImageView *pImageView = (ImageView *)pSender;
            m_u8SignUpDay = pImageView->getTag() + 1;
            CCString *pStrMsg = CCString::createWithFormat(ASK_SIGNUP, pImageView->getTag() + 1);
            Controller::getInstance()->sendMessage(pStrMsg->getCString(), false);
    }
}

void PopUpUI::menuSignUpBoxAward(CCObject* pSender, ::cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        setSignUpActionStatus(kActionSignUpBoxAward);
        UIButton *pButton = (UIButton *)pSender;
        m_u8SignUpBoxAwardBtnTag = pButton->getTag();
        YYSignUpBoxAwardData *pBoxAwardData = NULL;
        if  (m_u8SignUpBoxAwardBtnTag < m_pArrayBoxAward->count())
            pBoxAwardData = (YYSignUpBoxAwardData *)m_pArrayBoxAward->objectAtIndex(m_u8SignUpBoxAwardBtnTag);
        else
        {
            CCArray *pArrayBoxAward = GlobalData::getInstance()->getSigUpBoxAwardDataArray();
            pBoxAwardData = (YYSignUpBoxAwardData *)pArrayBoxAward->objectAtIndex(m_u8SignUpBoxAwardBtnTag);
        }
        if (pBoxAwardData->getSignuPAwardStatus() == kStatusAwardHaveGot)
        {
            ((YYScene*)getParent())->showTips(g_sGotten, kTipNormal);
            return;
        }
        else if (pBoxAwardData->getSignuPAwardStatus() == kStatusAwardCanNotGet)
        {
            ((YYScene*)getParent())->showTips(g_sCanNotGet, kTipNormal);
            return;
        }
        
        CCString *pStrMsg = CCString::createWithFormat(ASK_SIGNUP_BOX_AWARD, m_u8SignUpBoxAwardBtnTag);
        Controller::getInstance()->sendMessage(pStrMsg->getCString(), false);
    }
}
void PopUpUI::menuSignUpGoShop(CCObject* pSender, ::cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        setUIAwardStatus(kUIDefaultAward);
        getPopWindow("Panel_1", "Panel_c", false);
        goShopUI(kShopHotPage);
    }
}
void PopUpUI::exitSignUp()
{
    if (m_pArraySignUpDate)
    {
        m_pArraySignUpDate->removeAllObjects();
        CC_SAFE_DELETE(m_pArraySignUpDate);
    }
    if (m_pArrayAddSignUpDate)
    {
        m_pArrayAddSignUpDate->removeAllObjects();
        CC_SAFE_DELETE(m_pArrayAddSignUpDate);
    }
    if (m_pArrayBoxAward)
    {
        m_pArrayBoxAward->removeAllObjects();
        CC_SAFE_DELETE(m_pArrayBoxAward);
    }
}

void PopUpUI::closeAwardUI(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    
    if (type == TOUCH_EVENT_ENDED)
    {
        if (getUIAwardStatus() == kUISignUpAward || getUIAwardStatus() == kUISignUpAddAward)
            getPopWindow("Panel_2", "Panel_a", false);
        else if (getUIAwardStatus() == kUIBoxAward)
            getPopWindow("Panel_2", "Panel_b", false);

        m_pUITouchGroup->setTouchEnabled(true);
        setUIAwardStatus(kUIDefaultAward);
        Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
        Layout *panel1_2 = static_cast<Layout*>(panel1->getChildByName("Panel1_2"));
        
        // 签到完成后按钮的变化
        UIButton *pButtonSignUp = static_cast<UIButton *>(panel1_2->getChildByName("Button0"));
        CC_ASSERT(pButtonSignUp);
        std::string str = getFilePath("212-1", kDirTypeUI, kFileSuffixPng);
        pButtonSignUp->loadTextureNormal(str.c_str());
        pButtonSignUp->loadTexturePressed(str.c_str());
    }
}

void PopUpUI::setHide(cocos2d::gui::Widget *widget)
{
    CC_ASSERT(widget);
    for (int nIndexChild = 0; nIndexChild < widget->getChildren()->count(); nIndexChild++)
    {
        Widget *child = (Widget *)widget->getChildren()->objectAtIndex(nIndexChild);
        if (child->isEnabled())
            child->setVisible(false);
    }
}
int PopUpUI::getMaxMonthDate(int year, int month)
{
    int nDay = 0;
    return nDay;
}
void PopUpUI::removeLastObject(cocos2d::CCArray *arr)
{
    if (arr && arr->count() >= 1)
    {
        std::string str = "";
        CCString *pStr = (CCString *)arr->objectAtIndex(arr->count() - 1);
        if (pStr->compare(str.c_str()) == 0)
        {
            arr->removeLastObject();
        }
    }
}

void PopUpUI::updatePropertyInfo()
{
    if(getUIType() == kUIBag)
    {
        updateBagUIPropertyInfo();
    }
    else if(getUIType() == kUIAward)
    {
        updateAwardUIPropertyInfo();
    }
    else if(getUIType() == kUIShop)
    {
        updateShopUIPropertyInfo();
    }
    else if (getUIType() == kUICharge)
    {
        updateChargeUIPropertyInfo();
    }
    else if(getUIType() == kUIProtoss)
    {
        updateProtossUIPropertyInfo();
    }
    else if(getUIType() == kUISkill)
    {
        Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
        Layout *panel_b = static_cast<Layout *>(panel0->getChildByName("Panel_b"));
        Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
        Layout * panel2_0_1 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_1"));
        updateSkillUIPropertyInfo(panel2_0_1);
    }
}

#pragma mark 用户中心

void PopUpUI::initUserCenterUI()
{
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIUserCenter);
    
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = POP_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    

    Layout *pPanel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel"));
    for (int i = 0; i < pPanel->getChildrenCount(); i++)
    {
        Widget * widget= static_cast<Widget *>(pPanel->getChildren()->objectAtIndex(i));
        widget->setEnabled(false);
        widget->setVisible(false);
        widget->setTouchEnabled(false);
    }
   
    Layout *panel00 = static_cast<Layout*>(pPanel->getChildByName("Panel00"));
    panel00->setEnabled(true);
    panel00->setVisible(true);
    panel00->setTouchEnabled(true);
    UIImageView* imgView= static_cast<UIImageView *>(panel00->getChildByName("ImageView0"));
    
    //关闭页面
    UIButton *closeButton = static_cast<UIButton *>(imgView->getChildByName("Button_closed"));
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    //客服mail
    UIButton *GMMailButton = static_cast<UIButton *>(imgView->getChildByName("Button_0"));
    GMMailButton->addTouchEventListener(this, toucheventselector(PopUpUI::UserCenterGMMailCallBack));
    
    //暂时屏蔽客服信息
    for (int i = 0; i < 4; i++)
    {
        CCString* path = CCString::createWithFormat("Label_%d",i);
        UILabel* label = static_cast<UILabel *>(imgView->getChildByName(path->getCString()));
        label->setVisible(false);
    }
    
    //Controller::getInstance()->sendMessage(SEND_USERCENTER,true);
}

void PopUpUI::updateUserCenterUI(CCArray* array)
{
    if (array)
    {
//        Layout *pPanel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel"));
//        Layout *panel00 = static_cast<Layout*>(pPanel->getChildByName("Panel00"));
//        UIImageView* imgView= static_cast<UIImageView *>(panel00->getChildByName("ImageView0"));
//        for (int i = 0; i < 4; i++)
//        {
//            CCString* txt = (CCString*)array->objectAtIndex(i);
//            CCString* path = CCString::createWithFormat("Label_%d",i);
//            UILabel* label = static_cast<UILabel *>(imgView->getChildByName(path->getCString()));
//            UILabel* label0 = static_cast<UILabel *>(label->getChildByName("Label_0"));
//            label0->setText(txt->getCString());
//        }
    }
}

void PopUpUI::UserCenterGMMailCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        exitUI();  //关闭当前页面
        initMailUI(false); //切换到联系客服页面
    }
}

#pragma mark 设置

void PopUpUI::initSettingUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUISetting);
  
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = SETTING_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    //关闭页面
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(closeButton);
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    //退出游戏
    Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    CC_ASSERT(panel1);
    UIButton *BtnQuit = static_cast<UIButton *>(panel1->getChildByName("Button_3"));
    CC_ASSERT(BtnQuit);
    BtnQuit->setTouchEnabled(true);
    BtnQuit->addTouchEventListener(this, toucheventselector(PopUpUI::settingQuitCallBack));
    
    //切换角色
    UIButton *BtnChangeRole = static_cast<UIButton *>(panel1->getChildByName("Button_0"));
    CC_ASSERT(BtnChangeRole);
    BtnChangeRole->setTouchEnabled(true);
    BtnChangeRole->addTouchEventListener(this, toucheventselector(PopUpUI::settingChangeRole));
    
    //联系客服
    UIButton *BtnGM = static_cast<UIButton *>(panel1->getChildByName("Button_2"));
    CC_ASSERT(BtnGM);
    BtnGM->setTouchEnabled(true);
    BtnGM->addTouchEventListener(this, toucheventselector(PopUpUI::settingGMCallBack));
    
    
    //音乐 音效 静音 可玩玩家
    for (int i = 0; i < 4; i++)
    {
        CCString *pStrImgView = CCString::createWithFormat("ImageView_%d", i);
        UIImageView* imgView = static_cast<UIImageView*>(panel1->getChildByName(pStrImgView->getCString()));
        UIImageView* imgView0 = static_cast<UIImageView*>(imgView->getChildByName("ImageView0"));
        UIImageView* imgView00 = static_cast<UIImageView*>(imgView0->getChildByName("ImageView0"));
        imgView00->setTag(i);
        imgView00->setTouchEnabled(true);
        imgView00->addTouchEventListener(this, toucheventselector(PopUpUI::settingBtnCallBack));
        
        switch (i)
        {
            case 0: //音乐
            {
              bool bBGMusic = Config::getInstance()->isBackgroundMusicPlay();
              if (!bBGMusic)
              {
                  imgView00->setVisible(false);
              }
              else
              {
                  imgView00->setVisible(true);
              }
            }
            break;
            case 1://音效
            {
                bool bEffect = Config::getInstance()->isEffectPlay();
                if (!bEffect)
                {
                    imgView00->setVisible(false);
                }
                else
                {
                    imgView00->setVisible(true);
                }
            }
            break;
            case 2://静音
            {
                bool bSoundOff = Config::getInstance()->isSoundOff();
                if (!bSoundOff)
                {
                    imgView00->setVisible(false);
                }
                else
                {
                    imgView00->setVisible(true);
                }
            }
            break;
            case 3: //周围玩家
            {
                bool bAroundPlayer = Config::getInstance()->isAroundPlayerShow();
                if (!bAroundPlayer)
                {
                    imgView00->setVisible(false);
                }
                else
                {
                    imgView00->setVisible(true);
                }
            }
                break;
            default:
                break;
        }
    }
}

void PopUpUI::settingRadioBoxVisible(int idx, bool isVisible)
{
    Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    CC_ASSERT(panel1);
    CCString *pStrImgView = CCString::createWithFormat("ImageView_%d", idx);
    UIImageView* imgView = static_cast<UIImageView*>(panel1->getChildByName(pStrImgView->getCString()));
    UIImageView* imgView0 = static_cast<UIImageView*>(imgView->getChildByName("ImageView0"));
    UIImageView* imgView00 = static_cast<UIImageView*>(imgView0->getChildByName("ImageView0"));
    imgView00->setVisible(isVisible);
}


void PopUpUI::settingGMCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        exitUI();  //关闭当前页面
        initUserCenterUI();
    }
}

void PopUpUI::settingChangeRole(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        Controller::getInstance()->returnLoginUI(false); //切换角色
    }
}

void PopUpUI::settingQuitCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        ((YYScene*)getParent())->showTips(g_ExitTip,kTipExit);
    }
    return;
}

void PopUpUI::settingBtnCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        UIImageView* imgView = static_cast<UIImageView*>(pSender);
        bool bVisble = imgView->isVisible();
        imgView->setVisible(!bVisble);
        switch (imgView->getTag())
        {
            case 0:   //音乐
            {
                bool bBGMusic = Config::getInstance()->isBackgroundMusicPlay();
                if (bBGMusic) //关闭
                {
                    Config::getInstance()->setBackgroundMusicPlay(false);
                    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                    if (!Config::getInstance()->isEffectPlay())  //音效关闭
                    {
                        Config::getInstance()->setSoundOff(true); //静音开启
                        settingRadioBoxVisible(2, true);
                    }
                }
                else //开启
                {
                    Config::getInstance()->setBackgroundMusicPlay(true);
                    const std::string filePath = getFilePath(MAINCITY_MUSIC ,kDirTypeSound ,kFileSuffixMp3);
                    PLAY_BGMUSIC(filePath.c_str());
                    Config::getInstance()->setSoundOff(false);
                    settingRadioBoxVisible(2, false);
                }
                Config::getInstance()->saveConfig();
            }
               break;
            case 1:   //音效
            {
                bool bEffect = Config::getInstance()->isEffectPlay();
                if (bEffect) //关闭
                {
                    Config::getInstance()->setEffectPlay(false);
                    SimpleAudioEngine::sharedEngine()->stopAllEffects();
                    if (!Config::getInstance()->isBackgroundMusicPlay())  //音乐关闭
                    {
                        Config::getInstance()->setSoundOff(true); //静音开启
                        settingRadioBoxVisible(2, true);
                    }
                }
                else //开启
                {
                    Config::getInstance()->setEffectPlay(true);
                    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
                    Config::getInstance()->setSoundOff(false);
                    settingRadioBoxVisible(2, false);
                }
                Config::getInstance()->saveConfig();
            }
                break;
            case 2:  //静音
            {
                bool bSoundOff = Config::getInstance()->isSoundOff();
                if (bSoundOff)
                {
                    //取消静音状态
                    Config::getInstance()->setSoundOff(false);
                    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
                    Config::getInstance()->setEffectPlay(true);
                    Config::getInstance()->setBackgroundMusicPlay(true);
                    const std::string filePath = getFilePath(MAINCITY_MUSIC ,kDirTypeSound ,kFileSuffixMp3);
                    PLAY_BGMUSIC(filePath.c_str());
                    settingRadioBoxVisible(0, true);
                    settingRadioBoxVisible(1, true);
                    settingRadioBoxVisible(2, false);
                }
                else
                {
                    //开启静音状态
                    Config::getInstance()->setSoundOff(true);
                    SimpleAudioEngine::sharedEngine()->stopAllEffects();
                    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
                    Config::getInstance()->setBackgroundMusicPlay(false);
                    Config::getInstance()->setEffectPlay(false);
                    settingRadioBoxVisible(0, false);
                    settingRadioBoxVisible(1, false);
                    settingRadioBoxVisible(2, true);
                }
                Config::getInstance()->saveConfig();
            }
                break;
            case 3:
            {
                bool bPlayer =Config::getInstance()->isAroundPlayerShow();
                if (bPlayer)
                {
                    settingRadioBoxVisible(3, false);
                    Config::getInstance()->setAroundPlayerShow(false);
                    ((YYScene *)getParent())->getSpriteLayer()->setAroundPlayerVisible(false);
                }
                else
                {
                    settingRadioBoxVisible(3, true);
                    Config::getInstance()->setAroundPlayerShow(true);
                    ((YYScene *)getParent())->getSpriteLayer()->setAroundPlayerVisible(true);
                }
                Config::getInstance()->saveConfig();
            }
             break;
            default:
                break;
        }
    }
    return;
}

void PopUpUI::settingCloseCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    return;
}

#pragma mark 排行榜

void PopUpUI::initRankUI()
{
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIRankWar); //默认战神榜
    //Controller::getInstance()->getScene()->getControlUILayer()->getMainMenu()->setEnabled(false);
    
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = RANK_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(closeButton);
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    m_nRankCurPageID = 0;
    m_nRankPrePageID = 0;
    Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CC_ASSERT(panel2);
    Layout* panel2_2 = static_cast<Layout*>(panel2->getChildByName("Panel2_2"));
    CC_ASSERT(panel2_2);
    for (int i = 0 ; i < panel2_2->getChildrenCount()-2; i++)
    {
        Button * button = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",i)->getCString()));
        button->addTouchEventListener(this, toucheventselector(PopUpUI::rankPageSwitchCallBack));
        if (i == 0)
        {
            m_pRankPageButton = button;
            string str = getFilePath("412", kDirTypeUI, kFileSuffixPng);
            button->loadTextureNormal(str.c_str());
        }
    }
    
    Button * btnL = dynamic_cast<UIButton *>(panel2_2->getChildByName("Button_L"));
    btnL->addTouchEventListener(this, toucheventselector(PopUpUI::rankPageArrowCallBack));
    
    Button * btnR = dynamic_cast<UIButton *>(panel2_2->getChildByName("Button_R"));
    btnR->addTouchEventListener(this, toucheventselector(PopUpUI::rankPageArrowCallBack));
    
    //特殊处理 不使用panel2_3
    Layout* panel2_3 = static_cast<Layout*>(panel2->getChildByName("Panel2_3"));
    CC_ASSERT(panel2_3);
    panel2_3->setEnabled(false);
    
    Controller::getInstance()->sendMessage("top P_L",false);
}

void PopUpUI::updateRankUI(cocos2d::CCDictionary* dict, int nIdx)
{
    if (!m_pDictRankData) {
        return;
    }
    Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CC_ASSERT(panel2);
    Layout* panel2_1 = static_cast<Layout*>(panel2->getChildByName("Panel2_1"));
    CC_ASSERT(panel2_1);
    CCArray* arrPageData = (CCArray*)m_pDictRankData->objectForKey(nIdx);
    for (int i  = 0; i < arrPageData->count(); i++) {
        CCString *pStrImgView = CCString::createWithFormat("ImageView%d", i);
        CCArray* arrData =  (CCArray*)arrPageData->objectAtIndex(i);
        UIImageView* imgView = static_cast<UIImageView*>(panel2_1->getChildByName(pStrImgView->getCString()));
        CC_ASSERT(imgView);
        for (int  j = 0;  j < RANK_COLUMN_COUNT; j++) {
            CCString *pStrLabel = CCString::createWithFormat("Label_%d", j);
            UILabel* label = static_cast<UILabel*>(imgView->getChildByName(pStrLabel->getCString()));
            
            if(j == 0)
            {
                int nRankID = nIdx*6+i+1;  //客户端手动添加"排名"字段
                CCString *pStrRankID = CCString::createWithFormat("%d", nRankID);
                label->setText((pStrRankID)->getCString());
            }
            else
            {
                label->setText(((CCString*)arrData->objectAtIndex(j-1))->getCString());
            }
        }
    }
    
    if (nIdx == 16) {    //最后一页 最后两项 特殊处理
        for (int i= 4; i < 6; i++) {
            CCString *pStrImgView = CCString::createWithFormat("ImageView%d", i);
            UIImageView* imgView = static_cast<UIImageView*>(panel2_1->getChildByName(pStrImgView->getCString()));
            CC_ASSERT(imgView);
            for (int  j = 0;  j < RANK_COLUMN_COUNT; j++) {
                CCString *pStrLabel = CCString::createWithFormat("Label_%d", j);
                UILabel* label = static_cast<UILabel*>(imgView->getChildByName(pStrLabel->getCString()));
                label->setText(" ");
            }
        }
    }
}

void PopUpUI::updateRankDataInfo()
{
    Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CC_ASSERT(panel2);
    Layout* panel2_2 = static_cast<Layout*>(panel2->getChildByName("Panel2_2"));
    CC_ASSERT(panel2_2);
    for (int i = 0 ; i < panel2_2->getChildrenCount(); i++)
    {
        if((strcmp(m_pRankPageButton->getName(), CCString::createWithFormat("Button_%d",i)->getCString()) == 0))
        {
            updateRankUI(m_pDictRankData,i);
            m_nRankPrePageID = m_nRankCurPageID;
            m_nRankCurPageID = i;
        }
    }
}

void PopUpUI::rankPageSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        UIButton* button = static_cast<UIButton*>(pSender);
        //  if (m_pRankPageButton != button)
        {
            std::string str =  getFilePath("411", kDirTypeUI, kFileSuffixPng);  //正常
            
            //恢复箭头所选按钮正常状态
            Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
            CC_ASSERT(panel2);
            Layout* panel2_2 = static_cast<Layout*>(panel2->getChildByName("Panel2_2"));
            CC_ASSERT(panel2_2);
            Button * btnArrow = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",(m_nRankCurPageID>10?10:m_nRankCurPageID))->getCString()));
            btnArrow->loadTextureNormal(str.c_str());
            
            str = getFilePath("411",kDirTypeUI,kFileSuffixPng); //正常
            m_pRankPageButton->loadTextureNormal(str.c_str());
            str = getFilePath("412",kDirTypeUI,kFileSuffixPng); //选中
            button->loadTextureNormal(str.c_str());
            m_pRankPageButton = button;
            updateRankDataInfo();
            // CCLOG("touch curPage %d",m_nRankCurPageID);
        }
    }
}

void PopUpUI::rankPageArrowCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        UIButton* button = static_cast<UIButton*>(pSender);
        Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_ASSERT(panel2);
        Layout* panel2_2 = static_cast<Layout*>(panel2->getChildByName("Panel2_2"));
        CC_ASSERT(panel2_2);
        
        std::string str =  getFilePath("411",kDirTypeUI,kFileSuffixPng);
        m_pRankPageButton->loadTextureNormal(str.c_str());
        
        if((strcmp(button->getName(), "Button_L") == 0))
        {
            if (m_nRankCurPageID > 0) {
                m_nRankPrePageID = m_nRankCurPageID;
                m_nRankCurPageID--;
                //CCLOG("L curPage %d",m_nRankCurPageID);
                if(m_nRankCurPageID < 11)
                {
                    Button * btnPre = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",m_nRankPrePageID>10?10:m_nRankPrePageID)->getCString()));
                    Button * btnCur = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",m_nRankCurPageID)->getCString()));
                    str =  getFilePath("411",kDirTypeUI,kFileSuffixPng);
                    btnPre->loadTextureNormal(str.c_str());
                    str = getFilePath("412",kDirTypeUI,kFileSuffixPng);
                    btnCur->loadTextureNormal(str.c_str());
                    //更新当前选中btn
                    m_pRankPageButton = btnCur;
                }
                else
                {
                    Button * btnCur = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",10)->getCString()));
                    str = getFilePath("412",kDirTypeUI,kFileSuffixPng);
                    btnCur->loadTextureNormal(str.c_str());
                }
                updateRankUI(m_pDictRankData,m_nRankCurPageID);
            }
            if (m_nRankCurPageID == 0)
            {
                Button * btnCur = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",0)->getCString()));
                str = getFilePath("412",kDirTypeUI,kFileSuffixPng);
                btnCur->loadTextureNormal(str.c_str());
            }
        }
        else if((strcmp(button->getName(), "Button_R") == 0))
        {
            if (m_nRankCurPageID < 16)//总页数
            {
                m_nRankPrePageID = m_nRankCurPageID;
                m_nRankCurPageID++;
                // CCLOG("R curPage %d",m_nRankCurPageID);
                if (m_nRankCurPageID < 11)
                {
                    Button * btnPre = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",m_nRankPrePageID>10?10:m_nRankPrePageID)->getCString()));
                    Button * btnCur = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",m_nRankCurPageID)->getCString()));
                    str =  getFilePath("411",kDirTypeUI,kFileSuffixPng);
                    btnPre->loadTextureNormal(str.c_str());
                    str = getFilePath("412",kDirTypeUI,kFileSuffixPng);
                    btnCur->loadTextureNormal(str.c_str());
                    //更新当前选中btn
                    m_pRankPageButton = btnCur;
                }
                else
                {
                    Button * btnCur = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",10)->getCString()));
                    str = getFilePath("412",kDirTypeUI,kFileSuffixPng);
                    btnCur->loadTextureNormal(str.c_str());
                }
                updateRankUI(m_pDictRankData,m_nRankCurPageID);
            }
            if (m_nRankCurPageID == 16)
            {
                Button * btnCur = dynamic_cast<UIButton *>(panel2_2->getChildByName(CCString::createWithFormat("Button_%d",10)->getCString()));
                str = getFilePath("412",kDirTypeUI,kFileSuffixPng);
                btnCur->loadTextureNormal(str.c_str());
            }
        }
    }
}

void PopUpUI::exitRank()
{
    if (m_pDictRankData)
    {
        m_pDictRankData->removeAllObjects();
        CC_SAFE_DELETE(m_pDictRankData);
    }
}

#pragma mark 试炼

void PopUpUI::initTrainUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUITrain);
    Controller::getInstance()->getScene()->setPopUpUIDelayTime(0);
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = TRAIN_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    //关闭
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(closeButton);
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    //决斗
    UIButton *doneButton = static_cast<UIButton *>(panel0->getChildByName("Button_done"));
    CC_ASSERT(doneButton);
    doneButton->addTouchEventListener(this, toucheventselector(PopUpUI::trainDoneCallBack));
    
    //imageView选择
    Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CC_ASSERT(panel2);
    Layout *panel2_1 = static_cast<Layout*>(panel2->getChildByName("Panel2_1"));
    CC_ASSERT(panel2_1);
    for (int i = 0; i < 5; i++)
    {
        CCString *pStrImgID = CCString::createWithFormat("ImageView_%i", i);
        UIImageView *pImagePage = static_cast<UIImageView *>(panel2_1->getChildByName(pStrImgID->getCString()));
        if(pImagePage)
        {
             UIImageView *imgView0 = static_cast<UIImageView *>(pImagePage->getChildByName("ImageView0"));
             if(imgView0)
             {
                 imgView0->setTag(i);
                 imgView0->setTouchEnabled(true);
                 imgView0->addTouchEventListener(this, toucheventselector(PopUpUI::updateTrainSelPlayerCallback));
                 if(i == 0)
                 {
                     imgView0->setVisible(true);
                 }
             }
        }
    }

    Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    YYSprite * player = dynamic_cast<YYSprite *>(getChildByTag(TAG_TRIAL_UI_PLAYER));
    if(!player)
    {
        player = (YYPlayer *)YYSprite::copy(m_pRoleByControlled);
        player->setAction(ACTION_STAND0, true);
        player->setPosition(ccp(m_cAdjustScreenPosition.x + panel1->getPositionX() + panel1->getContentSize().width / 2,m_cAdjustScreenPosition.y + panel1->getPosition().y+10));
        player->setTag(TAG_TRIAL_UI_PLAYER);
        addChild(player, 1);
    }
    
    Layout *panel3 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel3"));
    if(panel3)
    {
        panel3->setVisible(false);
        panel3->setEnabled(false);
    }

    //数据初始化
    m_u8TrainSelIdx = 0;
    Controller::getInstance()->sendMessage("trial L",true);
}

void PopUpUI::updateTrainUI(CCArray* array)
{
    int nCount = array->count();
    std::vector<TrialUIData> vec_TrialUIData;
    for (int i = 0; i < nCount; i++)
    {
        CCString* pStr = (CCString*)array->objectAtIndex(i);
        CCArray* arrData = split_string(pStr->getCString(),';');
        TrialUIData trainData;
        if (arrData)
        {
            trainData.m_playerIdx = string(((CCString*)arrData->objectAtIndex(0))->getCString());
            trainData.m_playerName = string(((CCString*)arrData->objectAtIndex(1))->getCString());
            trainData.m_nLevel =  atoi(((CCString*)arrData->objectAtIndex(2))->getCString());
            trainData.m_nRank =  atoi(((CCString*)arrData->objectAtIndex(3))->getCString());
            trainData.m_u8ProFession = atoi(((CCString*)arrData->objectAtIndex(4))->getCString());
            vec_TrialUIData.push_back(trainData);
        }
    }

    Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CC_ASSERT(panel2);
    Layout* panel2_1 = static_cast<Layout*>(panel2->getChildByName("Panel2_1"));
    CC_ASSERT(panel2_1);
    for (int i = 0; i < nCount; i++)
    {
        TrialUIData trainData = vec_TrialUIData[i];
        CCString *pStrImgView = CCString::createWithFormat("ImageView_%d", i);
        UIImageView* imgView = static_cast<UIImageView*>(panel2_1->getChildByName(pStrImgView->getCString()));
        
        //姓名
        UILabel* label0 = static_cast<UILabel*>(imgView->getChildByName("Label_0"));
        label0->setText(trainData.m_playerName.c_str());
        
        //等级
        UILabel* label1 = static_cast<UILabel*>(imgView->getChildByName("Label_1"));
        UILabel* label_1_0 = static_cast<UILabel*>(label1->getChildByName("Label_1_0"));
        label_1_0->setText((CCString::createWithFormat("%d",trainData.m_nLevel))->getCString());
        
        //排名
        UILabel* label2 = static_cast<UILabel*>(imgView->getChildByName("Label_2"));
        label2->setText((CCString::createWithFormat(g_sTrialRankName,trainData.m_nRank))->getCString());
        
        //职业
        CCString *pStrImgID = CCString::createWithFormat("ImageView_%i", i);
        UIImageView *pImagePage = static_cast<UIImageView *>(panel2_1->getChildByName(pStrImgID->getCString()));
        if(pImagePage)
        {
             CCSprite* spr = NULL;
             switch(trainData.m_u8ProFession)
             {
                case 3: //战士
                 {
                     std::string str = getFilePath("0",kDirTypeUI,kFileSuffixPng);
                     spr = CCSprite::create(str.c_str());
                 }
                    break;
                case 2: //法师
                 {
                     std::string str = getFilePath("2",kDirTypeUI,kFileSuffixPng);
                     spr = CCSprite::create(str.c_str());
                 }
                    break;
                case 1: //弓手
                 {
                     std::string str = getFilePath("3",kDirTypeUI,kFileSuffixPng);
                     spr = CCSprite::create(str.c_str());
                 }
                    break;
                default:
                    break;
             }
             pImagePage->addNode(spr,5); //添加职业ICON
        }
      }
    
    //战力
    UILayout* panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIImageView* imgView1 = static_cast<UIImageView*>(panel0->getChildByName("ImageView1"));
    UILabel* label_1 = static_cast<UILabel*>(imgView1->getChildByName("Label_1"));
    label_1->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getFightingCapacity())->getCString());
    
    //排名
    UIImageView* imgView2 = static_cast<UIImageView*>(panel0->getChildByName("ImageView2"));
    label_1 = static_cast<UILabel*>(imgView2->getChildByName("Label_1"));
    label_1->setText("5"); //todo 服务器下发
    
    //修养时间
    UILabel* label_0 = static_cast<UILabel*>(panel0->getChildByName("Label_0"));
    UILabel* label_0_0 = static_cast<UILabel*>(label_0->getChildByName("Label_0_0"));
    label_0_0->setText("5:00"); //todo 服务器下发
    
    //战斗次数
    label_1 = static_cast<UILabel*>(panel0->getChildByName("Label_1"));
    UILabel* label_1_0 = static_cast<UILabel*>(label_1->getChildByName("Label_1_0"));
    label_1_0->setText("1"); //todo 服务器下发
    UILabel* label_1_2 = static_cast<UILabel*>(label_1->getChildByName("Label_1_2"));
    label_1_2->setText("5"); //todo 服务器下发
    UILabel* label_1_1 = static_cast<UILabel*>(label_1->getChildByName("Label_1_1"));
    UIImageView *pImageSlash = UIImageView::create();
    std::string str = getFilePath("slash",kDirTypeUI,kFileSuffixPng);
    pImageSlash->loadTexture(str.c_str());
    label_1_1->addChild(pImageSlash);
}

void PopUpUI::updateTrainSelPlayerCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        UIImageView *pImgView= (UIImageView *)pSender;
        int nTag = pImgView->getTag();
        if (nTag != m_u8TrainSelIdx)
        {
            //隐藏其他选中框
            Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
            CC_ASSERT(panel2);
            Layout *panel2_1 = static_cast<Layout*>(panel2->getChildByName("Panel2_1"));
            CC_ASSERT(panel2_1);
            for (int i = 0; i < 5; i++)
            {
                CCString *pStrImgID = CCString::createWithFormat("ImageView_%i", i);
                UIImageView *pImagePage = static_cast<UIImageView *>(panel2_1->getChildByName(pStrImgID->getCString()));
                if(pImagePage)
                {
                    UIImageView *imgView0 = static_cast<UIImageView *>(pImagePage->getChildByName("ImageView0"));
                    if(imgView0)
                    {
                        imgView0->setVisible(false);
                    }
                }
            }
            m_u8TrainSelIdx = nTag;
            pImgView->setVisible(true);
        }
    }
}

void PopUpUI::trainDoneCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN) {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        removeChildByTag(TAG_TRIAL_UI_PLAYER);
        Controller::getInstance()->switchGameStatus(kStatusLoading);
        const char *temp = CCString::createWithFormat( "trial S_%d",m_u8TrainSelIdx)->getCString();
        Controller::getInstance()->sendMessage(temp,false);
        Controller::getInstance()->setTrial(true);
    }
}

void PopUpUI::initTrialResult(bool bSuccess)
{
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUITrain);
    m_bTrialResult = bSuccess;
    
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup,2);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = TRAIN_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    //屏蔽其他panel
    for (int i = 0; i < 3; i++)
    {
        CCString *pStrID = CCString::createWithFormat("Panel%i", i);
        Layout *panel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName(pStrID->getCString()));
        panel->setVisible(false);
        panel->setEnabled(false);
    }
    
    Layout *panel3 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel3"));
    if (panel3)
    {
        panel3->setVisible(true);
        panel3->setEnabled(true);
        //确定
        UIButton *confirmButton = static_cast<UIButton *>(panel3->getChildByName("Button0_0"));
        if (confirmButton)
        {
            confirmButton->addTouchEventListener(this, toucheventselector(PopUpUI::trainConfirmCallBack));
        }
        
        //试炼胜利或失败
        
        UIImageView *pImagePage0 = static_cast<UIImageView *>(panel3->getChildByName("ImageView_0_0"));
        if(pImagePage0)
        {
            UILabel* label0 = static_cast<UILabel *>(pImagePage0->getChildByName("Label_0_1"));
            UILabel* label1 = static_cast<UILabel *>(pImagePage0->getChildByName("Label_1_0"));
            if (label0 && label1)
            {
                if (bSuccess)
                {
                    label0->setVisible(true);
                    label1->setVisible(false);
                }
                else
                {
                    label0->setVisible(false);
                    label1->setVisible(true);
                }
            }
        }
        
        //自己头像，名字
        UIImageView *pImagePage1 = static_cast<UIImageView *>(panel3->getChildByName("ImageView0_0"));
        if(pImagePage1)
        {
            const char *picPath= CCString::createWithFormat("i-0-%d-0-0",((YYScene*)getParent())->getControlUILayer()->getRoleByControlled()->getSpriteData()->getSpriteIndex() - 1)->getCString();
            std::string filePath = getFilePath(picPath,kDirTypeIcon,kFileSuffixPng);
            if (picPath)
            {
                pImagePage1->loadTexture(filePath.c_str());
            }
            
            UILabel* label0 = static_cast<UILabel *>(pImagePage1->getChildByName("Label0_2"));
            if (label0)
            {
                CCString* name = CCString::create("");
                name = (CCString*)((YYScene*)getParent())->getControlUILayer()->getRoleByControlled()->getSpriteName();
                label0->setText(name->getCString());
            }
        }
        //对手头像，名字
        UIImageView *pImagePage2 = static_cast<UIImageView *>(panel3->getChildByName("ImageView1_0"));
        if(pImagePage2)
        {
            const char *picPath= CCString::createWithFormat("i-0-%d-0-0",((YYScene*)getParent())->getControlUILayer()->getRoleByTrial()->getSpriteData()->getSpriteIndex() - 1)->getCString();
             std::string filePath = getFilePath(picPath,kDirTypeIcon,kFileSuffixPng);
            if (picPath)
            {
                pImagePage2->loadTexture(filePath.c_str());
            }
            UILabel* label0 = static_cast<UILabel *>(pImagePage2->getChildByName("Label0_1"));
            if (label0)
            {
                CCString* name = CCString::create("");
                name = (CCString*)((YYScene*)getParent())->getControlUILayer()->getRoleByTrial()->getSpriteName();
                label0->setText(name->getCString());
            }
        }
        
        //获得奖励
        UILabel* label3 = static_cast<UILabel*>(panel3->getChildByName("Label0_3"));
        if (label3)
        {
            UILabel* label0 = static_cast<UILabel*>(label3->getChildByName("Label0_0_1"));
            label0->setText("12");
        }
    }
}

void PopUpUI::trainConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        //发送试炼结果消息
        Controller::getInstance()->getScene()->setNetLoading(true);
        int nSuccess = 0;
        m_bTrialResult ? nSuccess = 1 : nSuccess = 0;
        Controller::getInstance()->sendMessage((CCString::createWithFormat("trial F_%d",nSuccess))->getCString(),true);  //0 失败 1 成功
        
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;

      Controller::getInstance()->setTrial(false);
      Controller::getInstance()->setGameStatus(kStatusScene);
      setUIType(kUIDefaultStatus);
    }
}


#pragma mark 活跃度

void PopUpUI::initActivityUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIActivity);
    Controller::getInstance()->getScene()->setPopUpUIDelayTime(0);
    m_pRoleByControlled = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled();
    setIsActivityInit(false);
    
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = ACTIVITY_UI_RESOURCE_FILE_NAME;
    Widget *pWidget = GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str());
    m_pUITouchGroup->addWidget(pWidget);
    
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(closeButton);
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    // 寻路
    Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    if(panel2)
    {
        for (int nIndexTask = 0; nIndexTask < ACTIVITY_TASK_TOTAL; nIndexTask++)
        {
            CCString *pStrButton = CCString::createWithFormat("Button_%i", nIndexTask);
            UIButton *pButton = static_cast<UIButton*>(panel2->getChildByName(pStrButton->getCString()));
            CC_ASSERT(pButton);
            pButton->setTouchEnabled(false);
            
            // button_find_destination
            UIButton *pButtonFindPath = static_cast<UIButton*>(pButton->getChildByName("Button0"));
            CC_ASSERT(pButtonFindPath);
            pButtonFindPath->setEnabled(true);
            pButtonFindPath->setVisible(true);
            pButtonFindPath->setTag(nIndexTask);
            pButtonFindPath->addTouchEventListener(this, toucheventselector(PopUpUI::menuFindDestination));
        }
    }
    
    // 全部刷新
    UIButton *pButtonUpdateTask = static_cast<UIButton*>(panel2->getChildByName("Button0"));
    if(pButtonUpdateTask)
    {
        pButtonUpdateTask->addTouchEventListener(this, toucheventselector(PopUpUI::menuUpdateTask));
    }
 
    // 宝箱奖励
    Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    if(panel1)
    {
        for (int nIndex = 0; nIndex < ACTIVITY_TASK_TOTAL; nIndex++)
        {
            CCString *pStrButtonBox = CCString::createWithFormat("Button_%i", nIndex);
            UIButton *pButtonBox = static_cast<UIButton*>(panel1->getChildByName(pStrButtonBox->getCString()));
            CC_ASSERT(pButtonBox);
            pButtonBox->setTag(nIndex);
            pButtonBox->addTouchEventListener(this, toucheventselector(PopUpUI::menuWishTreeBox));
        }
    }

    Controller::getInstance()->sendMessage(ASK_ACTIVITY_INFO,true);
}

void PopUpUI::updateActivityUI(cocos2d::CCArray* array)
{
    if (!array)
    {
        return;
    }
    if(!m_pActivityTaskArray)
         m_pActivityTaskArray = new CCArray();
     m_pActivityTaskArray->removeAllObjects();
   
    if(m_pActivityTreeArray)
    {
        m_pActivityTreeArray = NULL;
    }
    
    m_nWishTreeLevel = ((CCString *)array->objectAtIndex(0))->intValue();  //许愿树等级
    m_nFreeFromUpdateNum = ((CCString *)array->objectAtIndex(1))->intValue();  //免费刷新次数
    m_nConsumeJewNum = ((CCString *)array->objectAtIndex(2))->intValue();  //免费刷新次数
    m_nMyScore = ((CCString *)array->objectAtIndex(3))->intValue();  //活跃度得分
    
    //任务列表数据
    CCString* strTask = (CCString *)array->objectAtIndex(4);
    CCArray* arrTask = split_string(strTask->getCString(), ';');
    for (int i = 0; i < arrTask->count(); i++)
    {
        CCString* strTmp = (CCString *)arrTask->objectAtIndex(i);
        CCArray*  arrTmp = split_string(strTmp->getCString(), ',');
        int nID = ((CCString*)arrTmp->objectAtIndex(0))->intValue();         //任务ID
        int nFinshCount = ((CCString*)arrTmp->objectAtIndex(1))->intValue(); //完成次数
        
        YYActivityTaskData* taskData = static_cast<YYActivityTaskData *>(GlobalData::getInstance()->getActivityTask()->objectAtIndex(nID));
        taskData->setFinishNum(nFinshCount);
        m_pActivityTaskArray->addObject(taskData);
        
    }
    
    //活跃树数据
    CCString* strTree = (CCString *)array->objectAtIndex(5);
    CCArray* arrTree = split_string(strTree->getCString(), ';');
    m_pActivityTreeArray = static_cast<CCArray *>(GlobalData::getInstance()->getActivityWishTreeDict()->objectForKey(m_nWishTreeLevel));
    for (int i = 0; i < arrTree->count(); i++)
    {
        int nStatus = ((CCString *)arrTree->objectAtIndex(i))->intValue(); // 0 不可领取 1 未领取 2 已领取
        YYActivityWishTreeData * treeData = (YYActivityWishTreeData *)m_pActivityTreeArray->objectAtIndex(i);
        treeData->setGetStatus(nStatus);
    }
    
    //更新任务数据
    updateActivityTaskUI();
    //更新活跃数高度
    drawWishTreeHight(m_nMyScore);
    //更新活跃树宝箱数据
    updateActivityBoxUI();
    
    //更新宝箱数据 todo 宝箱数据还需更新
    int nBoxAward  = ((CCString *)array->objectAtIndex(6))->intValue();  //是否有宝箱提示ID -1 没有 其他 显示获得宝箱奖励提示
    if(nBoxAward > -1)
    {
        drawActivityBoxAwardUI(nBoxAward);
    }
    
    setIsActivityInit(true);   //更新数据完成
    
}

void PopUpUI::updateActivityTaskUI()
{
    CC_ASSERT(m_pActivityTaskArray);
    
    Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CC_ASSERT(panel2);
    
    for (int nIndexTask = 0; nIndexTask < ACTIVITY_TASK_TOTAL; nIndexTask++)
    {
        YYActivityTaskData *pTaskData = (YYActivityTaskData *)m_pActivityTaskArray->objectAtIndex(nIndexTask);
        CCString *pStrButton = CCString::createWithFormat("Button_%i", nIndexTask);
        UIButton *pButton = static_cast<UIButton*>(panel2->getChildByName(pStrButton->getCString()));
        CC_ASSERT(pButton);
        pButton->setTouchEnabled(false);
        // description
        UILabel *label_0 = static_cast<UILabel*>(pButton->getChildByName("Label_0"));
        CC_ASSERT(label_0);
        label_0->setText(pTaskData->getDescription());
        // score
        UILabel *label_1 = static_cast<UILabel*>(pButton->getChildByName("Label_1"));
        CC_ASSERT(label_1);
        CCString* strAwardScore = CCString::createWithFormat("+%d",pTaskData->getAwardScore());
        label_1->setText(strAwardScore->getCString());
        // finishNum
        UILabel *label_2 = static_cast<UILabel*>(pButton->getChildByName("Label_2"));
        CC_ASSERT(label_2);
        CCString* strFinishNum = CCString::createWithFormat("%d",pTaskData->getFinishNum());
        label_2->setText(strFinishNum->getCString());
        
        UILabel *label_2_0 = static_cast<UILabel*>(label_2->getChildByName("Label_2_0"));
        CC_ASSERT(label_2_0);
        UIImageView *pImageSlash = UIImageView::create();
        std::string filePath = getFilePath("slash", kDirTypeUI, kFileSuffixPng);
        pImageSlash->loadTexture(filePath.c_str());
        label_2_0->addChild(pImageSlash);
        
        // totalNum
        UILabel *label_2_1 = static_cast<UILabel*>(label_2->getChildByName("Label_2_1"));
        CC_ASSERT(label_2_1);
        CCString* strTotalNum = CCString::createWithFormat("%d",pTaskData->getTotalNum());
        label_2_1->setText(strTotalNum->getCString());
        
        if (pTaskData->getFinishNum() >= pTaskData->getTotalNum())
        {
            UIButton *pButton0 = static_cast<UIButton*>(pButton->getChildByName("Button0"));
            CC_ASSERT(pButton0);
            pButton0->setTitleText(g_sGetScore);
        }
    }
    // free
    UILabel *label_1 = static_cast<UILabel*>(panel2->getChildByName("Label_1"));
    CC_ASSERT(label_1);
    UILabel *label_1_0 = static_cast<UILabel*>(label_1->getChildByName("Label_1_0"));
    CC_ASSERT(label_1_0);
    CCString *pStrFreeNum = (CCString *)CCString::createWithFormat("%i", m_nFreeFromUpdateNum);
    label_1_0->setText(pStrFreeNum->getCString());
}

void PopUpUI::updateActivityBoxUI()
{
    // wish tree
    Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    CC_ASSERT(panel1);
    UIButton *pButtonWishTree = static_cast<UIButton*>(panel1->getChildByName("Button0"));
    CC_ASSERT(pButtonWishTree);
    UILabel *labelLvel = static_cast<UILabel*>(pButtonWishTree->getChildByName("Label_0"));
    CC_ASSERT(labelLvel);
    CCString *pStrLevel = (CCString *)CCString::createWithFormat("%i", getWishTreeLevel()); //活跃树等级
    labelLvel->setText(pStrLevel->getCString());
    for (int nIndex = 0; nIndex < ACTIVITY_TASK_TOTAL; nIndex++)
    {
        YYActivityWishTreeData *pWishTreeData = (YYActivityWishTreeData *)m_pActivityTreeArray->objectAtIndex(nIndex);
        
        CCString *pStrButtonBox = CCString::createWithFormat("Button_%i", nIndex);
        UIButton *pButtonBox = static_cast<UIButton*>(panel1->getChildByName(pStrButtonBox->getCString()));
        CC_ASSERT(pButtonBox);
        
        // box need score
        UILabel *label = static_cast<UILabel *>(pButtonBox->getChildByName("Label_0"));
        CCString *pStrNeedScore = CCString::createWithFormat("%i", pWishTreeData->getNeedScore());
        label->setText(pStrNeedScore->getCString());
        
        // box Btn pic
        CCString *pStr = NULL;
        int nStatus = pWishTreeData->getGetStatus();
        switch (nStatus)
        {
            case 0: //不可领取状态
                pStr = CCString::createWithFormat("activityBox_%i-0", nIndex);
                break;
            case 1:
                pStr = CCString::createWithFormat("activityBox_%i-1", nIndex);
                break;
            case 2:
                pStr = CCString::createWithFormat("activityBox_%i-2", nIndex);
                break;
            default:
                break;
        }
        if (pStr)
        {
            std::string str = getFilePath(pStr->m_sString, kDirTypeIcon, kFileSuffixPng);
            pButtonBox->loadTextures(str.c_str(), str.c_str(), str.c_str());
        }
    }
}

void PopUpUI::drawActivityBoxAwardUI(int idx)
{
    m_pUITouchGroup->setTouchEnabled(false);
    Layout *panel_b = (Layout *)getPopWindow("Panel_2", "Panel_b", true);
    UIImageView *pImgView = static_cast<UIImageView *>(panel_b->getChildByName("ImageView"));
    CC_ASSERT(pImgView);
    UILabel *label1 = static_cast<UILabel *>(pImgView->getChildByName("TextArea"));
    CC_ASSERT(label1);
    label1->setVisible(false);
    UILabel *label2 = static_cast<UILabel *>(pImgView->getChildByName("TextArea_0"));
    CC_ASSERT(label2);
    label2->setVisible(true);
    
    UIButton *pButtonSure = static_cast<UIButton *>(panel_b->getChildByName("Button0"));
    pButtonSure->addTouchEventListener(this, toucheventselector(PopUpUI::menuBoxAwardSure));
    
    ImageView *pImageView_0 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_0"));
    ImageView *pImageView_1 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_1"));
    ImageView *pImageView_2 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_2"));
    pImageView_0->setVisible(false);
    pImageView_1->setVisible(false);
    pImageView_2->setVisible(false);
}

void PopUpUI::drawWishTreeHight(int nScore)
{
    Layout *panel1 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel1"));
    UIImageView *pImageView_0 = static_cast<UIImageView *>(panel1->getChildByName("ImageView_0"));
    UIImageView *pImageView_1 = static_cast<UIImageView *>(pImageView_0->getChildByName("ImageView_1"));
    if (pImageView_1->isVisible())
        pImageView_1->setVisible(false);
    
    float percengage = nScore / 100.0;
    percengage = percengage > 1.0 ? 1.0 : percengage;
    UIImageView *pImage = (UIImageView *)pImageView_0->getChildByTag(10);
    if (!pImage)
    {
        pImage = UIImageView::create();
        std::string filePath = getFilePath("383-1", kDirTypeUI, kFileSuffixPng);
        pImage->loadTexture(filePath.c_str());
        pImage->setPosition(ccp((percengage - 1) / 2.0  * pImage->getSize().width, 0));
        pImage->setTextureRect(CCRectMake(0, 0, percengage * pImage->getSize().width, pImage->getSize().height));
        pImageView_0->addChild(pImage, 0, 10);
    }
    else
    {
        pImage->setPosition(ccp((percengage - 1) / 2.0 * pImage->getSize().width, 0));
        pImage->setTextureRect(CCRectMake(0, 0, percengage * pImage->getSize().width, pImage->getSize().height));
    }
}
void PopUpUI::menuBoxAwardSure(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        m_pUITouchGroup->setTouchEnabled(true);
        getPopWindow("Panel_2", "Panel_b", false);
        YYActivityWishTreeData *pWishTreeData = (YYActivityWishTreeData *)m_pActivityTreeArray->objectAtIndex(m_nBoxID);
        pWishTreeData->setGetStatus(2); //标记为已领取状态
        updateActivityBoxUI();
    }
}
void PopUpUI::menuFindDestination(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        if (getIsActivityInit())
        {
            int nLevel = m_pRoleByControlled->getLevel();
            if (nLevel < ACTIVITY_MIN_RANK)
            {
                ((YYScene*)getParent())->showTips(g_sActivityOperateRequir, kTipNormal);
                return;
            }
            if (m_pActivityTaskArray && m_pActivityTaskArray->count() > 0)
            {
                UIButton *pButton = (UIButton *)pSender;
                callFounction(pButton->getTag());
            }
        }
    }
}

void PopUpUI::callFounction(int nBtnId)
{
    YYActivityTaskData *pTaskData = (YYActivityTaskData *)m_pActivityTaskArray->objectAtIndex(nBtnId);
  
    switch (pTaskData->getType())
    {
        case 0:
        {   // login
            
        }
            break;
        case 1:
        {
            // transcript
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
        }
            break;
        case 2:
        {
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
        }
            break;
        case 3:
        {
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
        }
            break;
        case 4:
        {
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
        }
            break;
        case 5:
        {
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
            Controller::getInstance()->getScene()->getPopUpUI()->initBagUI(2);
        }
            break;
        case 6:
        {
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
            Controller::getInstance()->getScene()->getPopUpUI()->initBagUI(1);
        }
            break;
        case 7:
        {
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
        }
            break;
        case 8:
        {
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
            
        }
            break;
        case 9:
        {
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
            
        }
            break;
        case 10:
        {
            int nTileIndex = pTaskData->getEnterIndex() - 10;// map index(10: first index)
            float x = nTileIndex % ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileWidth();
            float y = Controller::getInstance()->getSceneCellTopY() - nTileIndex / ((YYScene *)getParent())->getTileColumnAmount() * ((YYScene *)getParent())->getTileHeight();
            float offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
            float offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
            m_pRoleByControlled->setDestination(ccpAdd(ccp(x,y), ccp(offsetX,-offsetY)));
            m_pRoleByControlled->setMoving(true);
            m_pRoleByControlled->setMoveType(MOVE_BY_SET_DESTINATION);
            m_pRoleByControlled->setRunningStatus(kRunningStatusAuto);
            if(Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                exitUI();
            }
            
        }
            break;
        default:
            break;
    }
}

void PopUpUI::menuUpdateTask(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        if (getIsActivityInit())
        {
            if(m_nFreeFromUpdateNum > 0)
            {
                Controller::getInstance()->sendMessage(ASK_ACTIVITY_TASK_UPDATE,true);
            }
            else
            {
                
                if(m_pRoleByControlled && (m_nConsumeJewNum > m_pRoleByControlled->getSpriteAttribute()->getJewel()))
                {
                    ((YYScene*)getParent())->showTips(g_sLackJewel);
                }
                else
                {
                    setConsumeJewelTips(kJewelConsumeForActivityUpdate,m_nConsumeJewNum);
                }
            }
        }
    }
}

void PopUpUI::menuWishTreeBox(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        if (getIsActivityInit())
        {
            UIButton *pButton = (UIButton *)pSender;
            m_nBoxID = pButton->getTag();
            CCString *pStrBoxIndex = CCString::createWithFormat(ASK_ACTIVITY_BOX_AWARD, pButton->getTag());
            Controller::getInstance()->sendMessage(pStrBoxIndex->getCString(),false);
        }
    }
}

void PopUpUI::exitActivity()
{
    if (m_pActivityTreeArray)
    {
        m_pActivityTaskArray = NULL;
    }
    if (m_pActivityTaskArray)
    {
        m_pActivityTaskArray->removeAllObjects();
        CC_SAFE_DELETE(m_pActivityTaskArray);
    }
}

#pragma mark 公告

void PopUpUI::initNoticeUI(CCArray* arr,bool isHaveData)
{
    if(isHaveData && !arr)
    {
       return;
    }
    if (!m_pUITouchGroup)
    {
        ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
        Controller::getInstance()->setGameStatus(kStatusUI);
        m_pUITouchGroup = TouchGroup::create();
        CC_ASSERT(m_pUITouchGroup);
        addChild(m_pUITouchGroup);
        m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
        std::string filePath = NOTICE_UI_RESOURCE_FILE_NAME;
        m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    }
    
    //文本显示
    Layout* panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    if (panel1 && isHaveData)
    {
        UIScrollView* sc = static_cast<UIScrollView*>(panel1->getChildByName("ScrollView"));
        if(sc)
        {
            sc->setBounceEnabled(true);
            UILabel* label = static_cast<UILabel*>(sc->getChildByName("Label_0"));
            if (label)
            {
                std::string strNotice = "";
                for(int i = 0; i < arr->count(); i++)
                {
                    CCArray* segStr = split_string(((CCString*)arr->objectAtIndex(i))->getCString(),';');
                    for (int j = 0; j < segStr->count(); j++)
                    {
                        std::string str= ((CCString*)segStr->objectAtIndex(j))->getCString();
                        strNotice+=str;
                        strNotice+="\n";
                    }
                    strNotice+="\n";
                }
                
                label->setText(strNotice);
                label->setFontSize(24);
            }
        }
    }
    
    //关闭页面
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(panel0)
    {
        UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
        if (closeButton)
        {
             closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::noticeClosedCallBack));
        }
    }
    
    if (!isHaveData)
    {
         Controller::getInstance()->sendMessage(SEND_NOTICE,true);
    }
}

void PopUpUI::noticeClosedCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        m_nButtonIndex = 0;
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        exitUI();
    }
}

#pragma mark 兑换码

void PopUpUI::initConvertCodeUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIConvertCode);
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = POP_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout * panel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel"));
    for (int i = 0; i < panel->getChildrenCount(); i++)
    {
        Widget * widget= static_cast<Widget *>(panel->getChildren()->objectAtIndex(i));
        widget->setEnabled(false);
        widget->setVisible(false);
    }
    
    Layout * panel_0 = static_cast<Layout *>(panel->getChildByName("Panel_0"));
    panel_0->setEnabled(true);
    panel_0->setVisible(true);
    panel_0->setTouchEnabled(true);
    Layout * panel_f = static_cast<Layout *>(panel_0->getChildByName("Panel_f"));
    panel_f->setEnabled(true);
    panel_f->setVisible(true);
    panel_f->setTouchEnabled(true);
    Button *button0 = static_cast<Button *>(panel_f->getChildByName("Button00"));
    button0->setTouchEnabled(true);
    button0->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    Button *button1 = static_cast<Button *>(panel_f->getChildByName("Button01"));
    button1->setTouchEnabled(true);
    button1->addTouchEventListener(this, toucheventselector(PopUpUI::convertCodeConfirmCallBack));
    ImageView * imageView = static_cast<ImageView *>(panel_f->getChildByName("ImageView_0"));
    Label *label_1 = static_cast<Label *>(imageView->getChildByName("TextArea0_0"));
    if(label_1)
    {
        label_1->setVisible(false);
        label_1->setEnabled(false);
        label_1->removeFromParent();
    }
    
    //ccDEditBox
    filePath = getFilePath("223",kDirTypeUI,kFileSuffixPng);
    m_pConvertCodeEditBox = CCEditBox::create(CCSizeMake(300.0f, 60.0f),CCScale9Sprite::create(filePath.c_str()));
    m_pConvertCodeEditBox->setPosition(imageView->getPosition());
    m_pConvertCodeEditBox->setFont("Helvetica-Bold",25);
    m_pConvertCodeEditBox->setFontColor(ccWHITE);
    m_pConvertCodeEditBox->setPlaceholderFontColor(ccWHITE);
    m_pConvertCodeEditBox->setMaxLength(12);
    m_pConvertCodeEditBox->setReturnType(kKeyboardReturnTypeDone);
    m_pConvertCodeEditBox->setAnchorPoint(ccp(0.5f,0.5f));
    m_pConvertCodeEditBox->setPlaceHolder(g_sConvertCodePlaceHolder);
    m_pConvertCodeEditBox->setText("");
    m_pConvertCodeEditBox->setTouchEnabled(true);
    m_pConvertCodeEditBox->setInputMode(kEditBoxInputModeSingleLine);
    m_pConvertCodeEditBox->setTouchPriority(-1);
    panel_f->addNode(m_pConvertCodeEditBox);
    imageView->removeFromParent();
    panel_f->setTouchEnabled(true);

}

void PopUpUI::convertCodeConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        m_nButtonIndex = 0;
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        if (m_pConvertCodeEditBox)
        {
            const char* str = m_pConvertCodeEditBox->getText();
            Controller::getInstance()->sendMessage((CCString::createWithFormat(SEND_EXCHANGE,str))->getCString(), true);
            m_pConvertCodeEditBox->setPlaceHolder(g_sConvertCodePlaceHolder);
            m_pConvertCodeEditBox->setText("");
            m_pConvertCodeEditBox->setTouchEnabled(false);
        }
    }
}

#pragma mark

void PopUpUI::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    //    CCLog("editBox %p DidBegin !", editBox);
}

void PopUpUI::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    //    CCLog("editBox %p DidEnd !", editBox);
}

void PopUpUI::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    //    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void PopUpUI::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    //    CCLog("editBox %p was returned !");
}
