//
//  PopUpUI.cpp
//  FairyLand II
//
//  Created by 章敏 on 9/25/13.  
//
//
#include "SimpleAudioEngine.h"
#include "YYPopUpUI.h"
#include "YYController.h"
#include "../common/Common.h"
#include "YYControlUILayer.h"
#include "YYScene.h"
#include "YYLayer.h"
#include "YYButton.h"
#include "YYDragRegion.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYSpriteAttribute.h"
#include "YYTaskTargetData.h"
#include "YYShopData.h"
#include "YYProtossData.h"
#include "YYLoginAwardData.h"
#include "YYTalentData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace gui;

#pragma mark 占灵

// 星灵兑换列表
void PopUpUI::initProtossExchangeList()
{
    CCArray *pArrayProtossRule = getProtossExchangeRuleList();
    if (!pArrayProtossRule)
        return;
    
    for (int i = 0; i < GlobalData::getInstance()->getScrapDataArray()->count(); i++)
    {
        YYProtossData *pLocalScrapData = (YYProtossData *)GlobalData::getInstance()->getScrapDataArray()->objectAtIndex(i);
        YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pLocalScrapData->getID());
        if (pLocalData)
        {
            YYProtossData *pProtossData = YYProtossData::create();
            pProtossData->clone(pLocalData);
            pProtossData->setExchangeNum(pLocalScrapData->getExchangeNum());
            pArrayProtossRule->addObject(pProtossData);
        }
        
    }
}
// 占灵价格
void PopUpUI::initProtossMoenyList()
{
    for (int i = 0; i < GlobalData::getInstance()->getConstellationDataArray()->count(); i++)
    {
        YYConstellationData *pLacalData = (YYConstellationData *)GlobalData::getInstance()->getConstellationDataArray()->objectAtIndex(i);
        setProtossMoney(pLacalData->getConstellationID(), pLacalData->getProtossMoeny(), pLacalData->getLightConstellationJew());
    }
}

void PopUpUI::initProtossBaseUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIProtoss);
    Controller::getInstance()->getScene()->setPopUpUIDelayTime(0);
    m_pRoleByControlled = Controller::getInstance()->getScene()->getSpriteLayer()->getRoleByControlled();
    m_pDevineBtn = NULL;
    if (!m_bIsProtossInit)
    {
       memset(m_pGoldProtoss, 0, sizeof(m_pGoldProtoss));
       memset(m_pJewProtoss, 0, sizeof(m_pJewProtoss));
       initProtossMoenyList();
    }
    //add layer to parent
    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = PROTOSS_UI_RESOURCE_FILE_NAME;
    Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str());
    if(!mainWidget)
    {
        return;
    }
    mainWidget->setTag(TAG_UI_PROTOSS_BASE);
    m_pUITouchGroup->addWidget(mainWidget);
    
    //start ui init
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    Layout *pUIEquip = static_cast<Layout*>(pPanel->getChildByName("Panel2"));
    Layout *pUIExchange = static_cast<Layout*>(pPanel->getChildByName("Panel3"));
    Layout *pUISwallow = static_cast<Layout*>(pPanel->getChildByName("Panel4"));
    if(!pUIBase || !pUIEquip || !pUIExchange || !pUISwallow)
        return;
    
    pUIBase->setEnabled(true);
    pUIBase->setVisible(true);
    pUIEquip->setEnabled(false);
    pUIEquip->setVisible(false);
    pUIExchange->setEnabled(false);
    pUIExchange->setVisible(false);
    pUISwallow->setEnabled(false);
    pUISwallow->setVisible(false);
    
    //close button
    UIButton *pBtnClose = static_cast<UIButton*>(pUIBase->getChildByName("Button_closed"));
    if(pBtnClose != NULL)
    {
        pBtnClose->setTag(kButtonClose);
        pBtnClose->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossBaseUIClose));
    }
    
    //help
    UIButton *helpButton = static_cast<UIButton *>(pUIBase->getChildByName("Button_0"));
    if(helpButton)
    {
        helpButton->setTag(kButtonHelp);
        helpButton->addTouchEventListener(this, toucheventselector(PopUpUI::helpButtonCallBack));
        ImageView * image = static_cast<ImageView *>(helpButton->getChildByName("ImageView_0"));
        image->addTouchEventListener(this, toucheventselector(PopUpUI::hideHelpInterface));
        image->setEnabled(false);
    }
    
    //devine buttons
    Layout *pDevineBtnPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_2"));
    if(!pDevineBtnPanel)
        return;
    for(int i = 0; i < MAX_DEVINE_KIND_NUM; i++)
    {
        CCString *tpStrName = CCString::createWithFormat("Button%d", i);
        if(!tpStrName)
            continue;
        
        UIButton *pDevineBtn = static_cast<UIButton *>(pDevineBtnPanel->getChildByName(tpStrName->getCString()));
        if(!pDevineBtn)
            continue;
        
        pDevineBtn->setTag(i + kProtossLightConstellationBtn);
        pDevineBtn->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossDevine));

    }
    for(int i = 0; i < 2; i++)
    {
        CCString *tpStrName = CCString::createWithFormat("Button_jew%d", i);
        if(!tpStrName)
            continue;
        UIButton *pLightBtn = static_cast<UIButton *>(pDevineBtnPanel->getChildByName(tpStrName->getCString()));
        if(!pLightBtn)
            continue;
        pLightBtn->setTag(i + kProtossConsumeJewelLightConstellationBtn);
        pLightBtn->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossJewOverOrder));
    }
    
    //money and jew
    Layout *pMoneyPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_0"));
    if(!pMoneyPanel)
        return;
    UIImageView *pImageViewMoney = static_cast<UIImageView*>(pMoneyPanel->getChildByName("ImageView_money"));
    if(pImageViewMoney != NULL)
    {
        UILabel* playerTotalMoneyValue = static_cast<UILabel*>(pImageViewMoney->getChildByName("TextArea"));
        if(playerTotalMoneyValue != NULL)
        {
            int nGold = m_pRoleByControlled->getSpriteAttribute()->getGold();
            playerTotalMoneyValue->setText(CCString::createWithFormat("%d", nGold)->getCString());         //玩家金币数
        }
    }
    UIImageView *pImageViewJew = static_cast<UIImageView*>(pMoneyPanel->getChildByName("ImageView_jew"));
    if(pImageViewJew != NULL)
    {
        UILabel* playerTotalJewValue = static_cast<UILabel*>(pImageViewJew->getChildByName("TextArea"));
        if(playerTotalJewValue != NULL)
        {
            int nJew = m_pRoleByControlled->getSpriteAttribute()->getJewel();
            playerTotalJewValue->setText(CCString::createWithFormat("%d", nJew)->getCString());         //玩家宝石数
        }
    }
    //onekey operation buttons
    Layout *pOneKeyBtnPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_3"));
    if(!pOneKeyBtnPanel)
        return;
    UIButton *pBtnOneKeyDevine = static_cast<UIButton *>(pOneKeyBtnPanel->getChildByName("Button0"));   // 一键占灵
    if(pBtnOneKeyDevine != NULL)
    {
        pBtnOneKeyDevine->setTag(kProtossOneKeyDevineBtn);
        pBtnOneKeyDevine->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossOnekeyDevine));
    }
    UIButton *pBtnOneKeyRecv = static_cast<UIButton *>(pOneKeyBtnPanel->getChildByName("Button1"));     // 一键收取
    if(pBtnOneKeyRecv != NULL)
    {
        pBtnOneKeyRecv->setTag(kProtossOneKeyReceiveBtn);
        pBtnOneKeyRecv->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossOnekeyReceive));
    }
    UIButton *pBtnOneKeySell = static_cast<UIButton *>(pOneKeyBtnPanel->getChildByName("Button2"));    // 一键卖出
    if(pBtnOneKeySell != NULL)
    {
        pBtnOneKeySell->setTag(kProtossOneKeySellBtn);
        pBtnOneKeySell->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossOnekeySell));
    }
    UIButton *pBtnToExchange = static_cast<UIButton *>(pOneKeyBtnPanel->getChildByName("Button3"));    // 兑换星灵
    if(pBtnToExchange != NULL)
    {
        pBtnToExchange->setTag(kProtossToExchangeProtossBtn);
        pBtnToExchange->addTouchEventListener(this, toucheventselector(PopUpUI::menuShowProtossExchangeUI));
    }
    UIButton *pBtnToEquip = static_cast<UIButton *>(pOneKeyBtnPanel->getChildByName("Button4"));       // 进入星盘
    if(pBtnToEquip != NULL)
    {
        pBtnToEquip->setTag(kProtossToEquipProtossBtn);
        pBtnToEquip->addTouchEventListener(this, toucheventselector(PopUpUI::menuShowProtossEquipUI));
    }
    //send request to server
    if (!getIsProtossInit())
    {
        Controller::getInstance()->sendMessage(ASK_PROTOSS_LIST_A, true);
    }
}

void PopUpUI::initProtossEquipUI()
{
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIProtoss);
    Controller::getInstance()->getScene()->setPopUpUIDelayTime(0);
    
    m_pTopLayer = CCLayer::create();
    addChild(m_pTopLayer, 1);
    
    //add layer to parent
    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = PROTOSS_UI_RESOURCE_FILE_NAME;
    Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str());
    if(!mainWidget)
    {
        return;
    }
    mainWidget->setTag(TAG_UI_PROTOSS_EQUIP);
    m_pUITouchGroup->addWidget(mainWidget);
    
    //start ui init
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    Layout *pUIEquip = static_cast<Layout*>(pPanel->getChildByName("Panel2"));
    Layout *pUIExchange = static_cast<Layout*>(pPanel->getChildByName("Panel3"));
    Layout *pUISwallow = static_cast<Layout*>(pPanel->getChildByName("Panel4"));
    if(!pUIBase || !pUIEquip || !pUIExchange || !pUISwallow)
        return;
    
    pUIBase->setEnabled(false);
    pUIBase->setVisible(false);
    pUIEquip->setEnabled(true);
    pUIEquip->setVisible(true);
    pUIExchange->setEnabled(false);
    pUIExchange->setVisible(false);
    pUISwallow->setEnabled(false);
    pUISwallow->setVisible(false);
    
    //close button
    UIButton *pBtnClose = static_cast<UIButton*>(pUIEquip->getChildByName("Button_closed"));
    if(pBtnClose != NULL)
    {
        pBtnClose->setTag(kButtonClose);
        pBtnClose->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossEquipUIClose));
    }
    
    //main buttons
    UIButton *pBtnMerge = static_cast<UIButton*>(pUIEquip->getChildByName("Button0"));
    if(pBtnMerge != NULL)
    {
        pBtnMerge->setVisible(false);
        pBtnMerge->setEnabled(false);
        pBtnMerge->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossOnekeyMerge));
    }
    UIButton *pBtnToBase = static_cast<UIButton*>(pUIEquip->getChildByName("Button1"));
    if(pBtnToBase != NULL)
    {
        pBtnToBase->setTag(kProtossToBaseUIBtn);
        pBtnToBase->addTouchEventListener(this, toucheventselector(PopUpUI::menuShowProtossBaseUI));
    }
    //equip positon
//    Layout *pEquipPanel = static_cast<Layout*>(pUIEquip->getChildByName("Panel2_0"));
//    if(!pEquipPanel)
//        return;
//    Layout *pEquipPosPanel = static_cast<Layout*>(pEquipPanel->getChildByName("Panel2_0_0"));
//    if(!pEquipPosPanel)
//        return;
//    for(int i = 0; i < MAX_PROTOSS_EQUIP_POS; i++)
//    {
//        CCString *strName = CCString::createWithFormat("ImageView%d", i);
//        if(!strName)
//            continue;
//        UIImageView *pPos = static_cast<UIImageView*>(pEquipPosPanel->getChildByName(strName->getCString()));
//        if(!pPos)
//            continue;
//        
//        //lock state
//        UIImageView *pLock = static_cast<UIImageView*>(pPos->getChildByName("ImageView"));
//        if(pLock != NULL)
//        {
//            //TODO
//            //pLock->setVisible(false);
//        }
//        
//        //set tag
//        pPos->setTag(i);
//    }
//    
    if (!getIsProtossEquipInit())
    {
        Controller::getInstance()->sendMessage(ASK_PROTOSS_LIST_B,true);
    }
    setIsEnterProtossEquipUI(true);
}

void PopUpUI::initProtossExchangeUI()
{
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIProtoss);
    
    //add layer to parent
    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = PROTOSS_UI_RESOURCE_FILE_NAME;
    Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str());
    if(!mainWidget)
    {
        return;
    }
    mainWidget->setTag(TAG_UI_PROTOSS_EXCHANGE);
    m_pUITouchGroup->addWidget(mainWidget);
    
    //start ui init
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    Layout *pUIEquip = static_cast<Layout*>(pPanel->getChildByName("Panel2"));
    Layout *pUIExchange = static_cast<Layout*>(pPanel->getChildByName("Panel3"));
    Layout *pUISwallow = static_cast<Layout*>(pPanel->getChildByName("Panel4"));
    if(!pUIBase || !pUIEquip || !pUIExchange || !pUISwallow)
        return;
    
    pUIBase->setEnabled(false);
    pUIBase->setVisible(false);
    pUIEquip->setEnabled(false);
    pUIEquip->setVisible(false);
    pUIExchange->setEnabled(true);
    pUIExchange->setVisible(true);
    pUISwallow->setEnabled(false);
    pUISwallow->setVisible(false);
    
    //close button
    UIButton *pBtnClose = static_cast<UIButton*>(pUIExchange->getChildByName("Button_closed"));
    if(pBtnClose != NULL)
    {
        pBtnClose->setTag(kButtonClose);
        pBtnClose->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossExchangeUIClose));
    }
    
    //main buttons
    Layout *pMainPanel = static_cast<Layout*>(pUIExchange->getChildByName("Panel3_1"));
    if(!pMainPanel)
        return;
    UIButton *pBtnExchange = static_cast<UIButton*>(pMainPanel->getChildByName("Button"));
    if(pBtnExchange != NULL)
    {
        pBtnExchange->setTag(kProtossExchangeBtn);
        pBtnExchange->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossExchange));
    }
    
    //更新星灵兑换列表数据
    initExchangeListUI();
}

void PopUpUI::drawProtossSwallowUI(int nProtossPosID)
{
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIProtoss);
    
    m_bSelectedAllProtoss = false;
    m_nSwallowProtossPosID = nProtossPosID;
    
    //add layer to parent
    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = PROTOSS_UI_RESOURCE_FILE_NAME;
    Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str());
    if(!mainWidget)
    {
        return;
    }
    mainWidget->setTag(TAG_UI_PROTOSS_SWALLOW);
    m_pUITouchGroup->addWidget(mainWidget);
    
    //start ui init
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    Layout *pUIEquip = static_cast<Layout*>(pPanel->getChildByName("Panel2"));
    Layout *pUIExchange = static_cast<Layout*>(pPanel->getChildByName("Panel3"));
    Layout *pUISwallow = static_cast<Layout*>(pPanel->getChildByName("Panel4"));
    if(!pUIBase || !pUIEquip || !pUIExchange || !pUISwallow)
        return;
    
    pUIBase->setEnabled(false);
    pUIBase->setVisible(false);
    pUIEquip->setEnabled(false);
    pUIEquip->setVisible(false);
    pUIExchange->setEnabled(false);
    pUIExchange->setVisible(false);
    pUISwallow->setEnabled(true);
    pUISwallow->setVisible(true);
    
    //close button
    UIButton *pBtnClose = static_cast<UIButton*>(pUISwallow->getChildByName("Button_closed"));
    if(pBtnClose != NULL)
    {
        pBtnClose->setTag(kButtonClose);
        pBtnClose->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossSwallowUIClose));
    }
    
    Layout *pPanel4_0 = (Layout *)pUISwallow->getChildByName("Panel4_0");
    UIButton *pBtnSwallow = static_cast<UIButton*>(pUISwallow->getChildByName("Button0"));
    if(pBtnSwallow != NULL)
    {
        pBtnClose->setTag(kProtossSwallowBtn);
        pBtnSwallow->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossSwallow));
    }
    
    // selected protoss;
    YYProtossData *pSelectedProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(nProtossPosID);
    UIImageView *pImageViewSelectedProtossBg = (UIImageView *)pPanel4_0->getChildByName("ImageView0");
    pImageViewSelectedProtossBg->removeAllChildren();
    
    UIImageView *pImageViewProtoss = UIImageView::create();
    filePath = getFilePath(pSelectedProtossData->getIcon(),kDirTypeIcon,kFileSuffixPng);
    pImageViewProtoss->loadTexture(filePath.c_str());
    pImageViewSelectedProtossBg->addChild(pImageViewProtoss);
    
    UILabel *pTitle= createLabel(CCSizeMake(40, 30), kCCTextAlignmentCenter, 20, ccRED);
    pTitle->setPosition(ccp(0, -pImageViewSelectedProtossBg->getSize().width / 2 + 25));
    pImageViewSelectedProtossBg->addChild(pTitle, 1);
    std::string strTitle = pSelectedProtossData->getTitle();
    pTitle->setText(strTitle);

    UILabel *pLabelName = (UILabel *)pPanel4_0->getChildByName("TextArea_name");
    pLabelName->setText(pSelectedProtossData->getName());
    pLabelName->setColor(getRGBValue(pSelectedProtossData->getColorType()));
    
    UILabel *pLabelRankDes = (UILabel *)pPanel4_0->getChildByName("TextArea00");
    UILabel *pLabelRank = (UILabel *)pLabelRankDes->getChildByName("TextArea0");
    CCString *pStrRank = CCString::createWithFormat("%d", pSelectedProtossData->getLevel());
    pLabelRank->setText(pStrRank->getCString());

    // 星灵描述
    CCArray* arrLabel = CCArray::create();
    for (int i = 0; i < 4; i++)
    {
        CCString* labelPath = CCString::createWithFormat("TextArea0%d",i+1);
        UILabel* pTextDesc = static_cast<UILabel *>(pPanel4_0->getChildByName(labelPath->getCString()));
        arrLabel->addObject(pTextDesc);
    }
    showProtossDescription(pSelectedProtossData->getDesc(),arrLabel);
    
    // experence
    UIImageView *pImageExpBg = (UIImageView *)pPanel4_0->getChildByName("ImageView_0");
    pImageExpBg->removeAllChildren();
    UIImageView *pImageExp = UIImageView::create();
    pImageExp->setAnchorPoint(ccp(0, 0.5));
    pImageExp->setPosition(ccp(-pImageExpBg->getContentSize().width / 2.0 + 11, 0));
    filePath = getFilePath("383-2",kDirTypeUI,kFileSuffixPng);
    pImageExp->loadTexture(filePath.c_str());
    pImageExpBg->addChild(pImageExp);
    float fWidth = (pSelectedProtossData->getCurrentExp() * 1.0 / pSelectedProtossData->getExp()) * pImageExp->getContentSize().width;
    pImageExp->setTextureRect(CCRectMake(0, 0, fWidth, pImageExp->getContentSize().height));
    
    CCString *pStrPerent = CCString::createWithFormat("%d / %d", pSelectedProtossData->getCurrentExp(), pSelectedProtossData->getExp());
    UILabel *pLabelPerent = createLabel(CCSizeMake(100, 30), kCCTextAlignmentCenter, 20, ccWHITE);
    pLabelPerent->setText(pStrPerent->getCString());
    pImageExpBg->addChild(pLabelPerent);
}

#pragma mark 绘制兑换星灵列表UI

void PopUpUI::initExchangeListUI()
{
    do {
        unsigned nListCount = 0;
        CCArray *pArrayProtossData = getProtossExchangeRuleList();
        CC_BREAK_IF(!pArrayProtossData);

        Layout* protossUI = static_cast<Layout*>(m_pUITouchGroup->getWidgetByTag(TAG_UI_PROTOSS_EXCHANGE));
        CC_BREAK_IF(!protossUI);
        
        Layout *pPanel = static_cast<Layout*>(UIHelper::seekWidgetByName(protossUI,"Panel3_0"));
        CC_BREAK_IF(!pPanel);
        
        UIScrollView * pSV = static_cast<UIScrollView *>(pPanel->getChildByName("ScrollView_0"));
        CC_BREAK_IF(!pSV);
        m_pUIScrollView = pSV;
        m_pUIScrollView->setTag(10);
        m_pUIScrollView->setBounceEnabled(true);
        
        Layout *pSvInnerPanel = m_pUIScrollView->getInnerContainer();
        
        UIImageView* pImg = static_cast<UIImageView *>(pSV->getChildByName("ImageView_0"));
        CC_BREAK_IF(!pImg);
        
        UIImageView *pImgSelect = static_cast<UIImageView *>(pImg->getChildByName("ImageView_1")); //隐藏选中框
        CC_BREAK_IF(!pImgSelect);
        pImgSelect->setVisible(false);
        
        float fImgHei = pImg->getSize().height;
        nListCount = pArrayProtossData->count();
        float fSVHei = nListCount * fImgHei;
        
        m_pUIScrollView->setInnerContainerSize(CCSizeMake(m_pUIScrollView->getContentSize().width,fSVHei));
 
        
        for(int i = 0; i < nListCount; i++)
        {
            YYProtossData *pData = (YYProtossData *)pArrayProtossData->objectAtIndex(i);
            CC_BREAK_IF(!pData);
    
            UIImageView* pImgClone = static_cast<UIImageView *>(pImg->clone());
            CC_BREAK_IF(!pImgClone);
            pSvInnerPanel->addChild(pImgClone);
            pImgClone->setPosition(ccp(pImgClone->getPosition().x, fSVHei - (i+0.5f) * fImgHei));
            pImgClone->setTouchEnabled(true);
            pImgClone->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossShowExchangeDetail));
            pImgClone->setTag(i);
            UIImageView* pIconBG = static_cast<UIImageView *>(pImgClone->getChildByName("ImageView_0"));
            CC_BREAK_IF(!pIconBG);
            
            //默认选中第一个选中框
            if (i == 0)
            {
                UIImageView *pSelect = static_cast<UIImageView *>(pImgClone->getChildByName("ImageView_1")); //隐藏选中框
                CC_BREAK_IF(!pSelect);
                pSelect->setVisible(true);
            }
            //icon
            std::string filePath = getFilePath(pData->getIcon(),kDirTypeIcon,kFileSuffixPng);
            UIImageView *pIcon = UIImageView::create();
            pIcon->loadTexture(filePath.c_str());
            CC_BREAK_IF(!pIcon);
            pIconBG->addChild(pIcon, 2);
            
            //name label
            UILabel* pName = static_cast<UILabel *>(pImgClone->getChildByName("Label_0"));
            pName->setColor(getRGBValue(pData->getColorType()));
            pName->setText(pData->getName().c_str());
    
            //desc label
            CCArray* arrLabel = CCArray::create();
            for (int i = 0; i < 4; i++)
            {
                CCString* labelPath = CCString::createWithFormat("Label_%d",i+1);
                UILabel* pDesc = static_cast<UILabel *>(pImgClone->getChildByName(labelPath->getCString()));
                arrLabel->addObject(pDesc);
            }
            showProtossDescription((pData->getDesc()),arrLabel);
  
        }
        pImg->removeFromParentAndCleanup(true);
        drawPieceInfo(0);
    } while (0);
}

void  PopUpUI::drawPieceInfo(int nIndex)
{
    if (nIndex < 0 || nIndex > getProtossExchangeRuleList()->count() - 1)
        return;
    
    setDetailIndex(nIndex);
    
    //change pic bg
    Layout* protossUI = static_cast<Layout*>(m_pUITouchGroup->getWidgetByTag(TAG_UI_PROTOSS_EXCHANGE));
    if(!protossUI)
        return;
    Layout *pPanel = static_cast<Layout*>(protossUI->getChildByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIExchange = static_cast<Layout*>(pPanel->getChildByName("Panel3"));
    if(!pUIExchange)
        return;
    
    //detail information
    Layout *pUIDesc = static_cast<Layout*>(pUIExchange->getChildByName("Panel3_1"));
    if(!pUIDesc)
        return;
    
    CCArray *pArrayProtossData = getProtossExchangeRuleList();
    if((!pArrayProtossData))
        return;
    
    //pre define
    std::string strProtossName = "";
    std::string strProtossDesc = "";
    std::string strPieceNeed = "0";
    std::string strPieceOwn = "0";
    
    YYProtossData *pData = (YYProtossData *)pArrayProtossData->objectAtIndex(nIndex);
    strProtossName = pData->getName();
    strProtossDesc = pData->getDesc();
    strPieceNeed = CCString::createWithFormat("%d", pData->getExchangeNum())->getCString();
    strPieceOwn = CCString::createWithFormat("%d", getProtossCount())->getCString();
    setDetailIndex(nIndex);
    
    UILabel *pTextName = static_cast<UILabel*>(pUIDesc->getChildByName("TextArea_name"));
    if(!pTextName)
        return;
    pTextName->setText(strProtossName.c_str());
    pTextName->setColor(getRGBValue(pData->getColorType()));
    //set desc
    
    CCArray* arrLabel = CCArray::create();
    for (int i = 0; i < 4; i++)
    {
        CCString* labelPath = CCString::createWithFormat("TextArea0%d",i);
        UILabel* pDesc = static_cast<UILabel *>(pUIDesc->getChildByName(labelPath->getCString()));
        arrLabel->addObject(pDesc);
    }
    showProtossDescription(strProtossDesc,arrLabel);
    
    //set piece state 星灵碎片
    UILabel *pTextState = static_cast<UILabel*>(pUIDesc->getChildByName("TextArea1"));
    if(!pTextState)
        return;
    UILabel *pTextPieceOwn = static_cast<UILabel*>(pTextState->getChildByName("TextArea12"));
    if(!pTextPieceOwn)
        return;
    pTextPieceOwn->setText(strPieceNeed.c_str());
    
    UILabel *pTextPieceNeed = static_cast<UILabel*>(pTextState->getChildByName("TextArea10"));
    if(!pTextPieceNeed)
        return;
    pTextPieceNeed->setText(strPieceOwn.c_str());
}
void PopUpUI::menuShowProtossEquipUI(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        setUIType(kUIDefaultStatus);
        Controller::getInstance()->setGameStatus(kStatusScene);
        setDetailIndex(-1);
        m_pDevineBtn = NULL;
        initProtossEquipUI();
        if (getIsProtossEquipInit())
        {
            updateProtossEquipUI();
            updateProtossEquipBagUI();
        }
    }
}

void PopUpUI::menuShowProtossExchangeUI(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        setUIType(kUIDefaultStatus);
        Controller::getInstance()->setGameStatus(kStatusScene);
        setDetailIndex(-1);
        m_pDevineBtn = NULL;
        initProtossExchangeList();
        initProtossExchangeUI();
    }
}
void PopUpUI::menuShowProtossBaseUI(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        setUIType(kUIDefaultStatus);
        Controller::getInstance()->setGameStatus(kStatusScene);
        setDetailIndex(-1);
        initProtossBaseUI();
        updateProtossBaseUI();
    }
}

void PopUpUI::menuProtossOnekeyDevine(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }

    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(QUICKROTOSS_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (isTouchUnable(pSender, type, false))
        {
            return;
        }
        //send request to server
        int nGold = m_pRoleByControlled->getSpriteAttribute()->getGold();
        if (nGold <= 0 || nGold < m_pGoldProtoss[0])
        {
           ((YYScene*)getParent())->showTips(g_sLackGold, kTipGoldCharge);
            return;
        }
        Controller::getInstance()->sendMessage(ASK_ONEKEY_PROTOSS,true);
    }
}
void PopUpUI::menuProtossOnekeyReceive(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }

    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(QUICKGET_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        //send request to server
        bool isHaveProtoss = false;
        for (int i = 0; i < m_pProtossWaitforRecvList->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)m_pProtossWaitforRecvList->objectAtIndex(i);
            if (pProtossData->getID() != 0)
            {
                isHaveProtoss = true;
            }
        }
        if (!isHaveProtoss)
        {
            ((YYScene*)getParent())->showTips(g_sEmptyBag, kTipNormal);
            return;
        }
        Controller::getInstance()->sendMessage(ASK_ONEKEY_COLLECT, true);
    }
}
void PopUpUI::menuProtossOnekeySell(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }

    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(QUICKSALE_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (isTouchUnable(pSender, type, false))
        {
            return;
        }
        //send request to server
        bool isHaveProtoss = false;
        for (int i = 0; i < m_pProtossWaitforRecvList->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)m_pProtossWaitforRecvList->objectAtIndex(i);
            if (pProtossData->getID() != 0)
            {
                isHaveProtoss = true;
            }
        }
        
        if (!isHaveProtoss)
        {
            ((YYScene*)getParent())->showTips(g_sEmptyBag, kTipNormal);
            return;
        }
        Controller::getInstance()->sendMessage(ASK_ONEKEY_SELL,true);
    }
}

void PopUpUI::menuProtossDevine(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(TOUCHPROTOSS_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (isTouchUnable(pSender, type, false))
        {
            return;
        }
        //get tag
        UIWidget* pBtn = (UIWidget *)pSender;
        if(!pBtn)
            return;
        int nTag = pBtn->getTag() - kProtossLightConstellationBtn;
        if(getCurDevineIndex() != nTag)
            return;

        int nGold = m_pRoleByControlled->getSpriteAttribute()->getGold();
        if (nGold <= 0 || nGold < m_pGoldProtoss[nTag])
        {
            ((YYScene*)getParent())->showTips(g_sLackGold, kTipGoldCharge);
            return;
        }
        //send request to server
        const char * msg = CCString::createWithFormat("%s_%d", ASK_NORMAL_PROTOSS, nTag)->getCString();
        Controller::getInstance()->sendMessage(msg, true);
    }
}


void PopUpUI::menuProtossDevineShowFunc(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        //get tag
        UIWidget* pBtn = (UIWidget *)pSender;
        if(!pBtn)
            return;
        int nTag = pBtn->getTag();
        setDetailIndex(nTag);
        showProtossInfoForBaseUI(0, nTag, SEL_TouchEvent(&PopUpUI::receiveOrSellCallback));
    }
}

void PopUpUI::menuProtossJewOverOrder(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        //get tag
        UIWidget* pBtn = (UIWidget *)pSender;
        if(!pBtn)
            return;
        int nTag = pBtn->getTag() + 3 - kProtossConsumeJewelLightConstellationBtn;
        m_nLightConstellationID = nTag;
        setConsumeJewelTips(kJewelConsumeForLightConstellation, m_pJewProtoss[nTag]);
    }
}
void PopUpUI::lightConstellationConsumeLogic()
{
    int nJew = m_pRoleByControlled->getSpriteAttribute()->getJewel();
    if (nJew <= 0 || nJew < m_pJewProtoss[m_nLightConstellationID])
    {
        ((YYScene*)getParent())->showTips(g_sLackJew, kTipJewCharge);
        return;
    }
    //send request to server
    CCString *strMsg = CCString::createWithFormat("%s_%d", ASK_BUTTON_LIGHT, m_nLightConstellationID);
    Controller::getInstance()->sendMessage(strMsg->getCString(),false);

}

void PopUpUI::protossBaseUIClose()
{
    if (m_pProtossWaitforRecvList)
        m_pProtossWaitforRecvList->removeAllObjects();
    setDetailIndex(-1);
    setCurDevineIndex(-1);
}

void PopUpUI::protossEquipUIClose()
{
    if (m_pProtossEquipList)
        m_pProtossEquipList->removeAllObjects();
    setDetailIndex(-1);
    setCurDevineIndex(-1);
}

void PopUpUI::protossExchangeUIClose()
{
    if (m_pProtossExchangeRuleList)
        m_pProtossExchangeRuleList->removeAllObjects();
    setDetailIndex(-1);
    setCurDevineIndex(-1);
}

void PopUpUI::menuProtossBaseUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        exitUI();
    }
}

void PopUpUI::menuProtossOnekeyMerge(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        //send request to server
        bool isHaveProtoss = false;
        int num = 0;
        for (int i = 0; i < m_pProtossBagList->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(i);
            if (pProtossData->getID() != 0)
            {
                ++num;
                if (num >= 2)
                {
                    isHaveProtoss = true;
                    break;
                }
            }
        }
        if (!isHaveProtoss)
        {
            ((YYScene*)getParent())->showTips(g_sCanNotMerge, kTipNormal);
            return;
        }
        ((YYScene*)getParent())->showTips(g_sAllMerge,kTipMergeProtoss);
    }
}
void PopUpUI::menuProtossEquipUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        setUIType(kUIDefaultStatus);
        Controller::getInstance()->setGameStatus(kStatusScene);
        setDetailIndex(-1);
        initProtossBaseUI();
        updateProtossBaseUI();
    }
}

void PopUpUI::menuProtossShowExchangeDetail(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender, type));
            UIImageView* pImg = (UIImageView *)pSender;
            CC_BREAK_IF(!pImg);
            int nTag = pImg->getTag();
            drawPieceInfo(nTag);
            
            UILayout* panel = m_pUIScrollView->getInnerContainer();
            int nCount = panel->getChildrenCount();
            for (int i = 0 ; i < nCount; i++)
            {
                UIImageView* pImgElem = static_cast<UIImageView *>(panel->getChildByTag(i));
                CC_BREAK_IF(!pImgElem);
                
                UIImageView *pImgSelect = static_cast<UIImageView *>(pImgElem->getChildByName("ImageView_1")); //隐藏选中框
                CC_BREAK_IF(!pImgSelect);
                if (i == nTag)
                {
                     pImgSelect->setVisible(true);
                }
                else
                {
                     pImgSelect->setVisible(false);
                }
            }
            
        }
    } while (0);
}

void PopUpUI::menuProtossExchange(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        int nDstId = getDetailIndex();
        if(nDstId < 0)
        {
            ((YYScene*)getParent())->showTips(g_sChoiceChangeProtoss, kTipNormal);
            return;
        }
        CCArray *pArrayProtossData = getProtossExchangeRuleList();
        YYProtossData *pData = (YYProtossData *)pArrayProtossData->objectAtIndex(getDetailIndex());
        //send request to server
        CCString *strMsg = CCString::createWithFormat("%s_%d", ASK_SCRAP_EXCHANGE, pData->getID());	//wxf,20140228 nDstId-1
        if(!strMsg)
            return;
        Controller::getInstance()->sendMessage(strMsg->getCString(), false);
    }
}

void PopUpUI::menuProtossExchangeUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_pUIScrollView)
        {
            m_pUIScrollView->removeAllChildrenWithCleanup(true);
            m_pUIScrollView = NULL;
        }
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        setUIType(kUIDefaultStatus);
        Controller::getInstance()->setGameStatus(kStatusScene);
        setDetailIndex(-1);
        initProtossBaseUI();
        updateProtossBaseUI();
    }
}

void PopUpUI::menuProtossSwallowUIClose(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        setDetailIndex(-1);
        m_bSelectedAllProtoss = false;
        m_pDevineBtn = NULL;
        initProtossEquipUI();
        if (getIsProtossEquipInit())
        {
            updateProtossEquipUI();
            updateProtossEquipBagUI();
        }
    }
}

void PopUpUI::menuProtossSwallow(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        bool isHaveSelected = false;
        for (int i = 0; i < MAX_PROTOSS_BAG_NUM; i++)
        {
            if (m_pArrayEquitBagPosID[i] == -1)
                break;
            YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(m_pArrayEquitBagPosID[i]);
            if (pProtossData->getIsSwallowed())
            {
                isHaveSelected = true;
            }
        }
        if (isHaveSelected)
        {
            ((YYScene*)getParent())->showTips(g_sSwallowProtoss,kTipSwallowProtoss);
        }
        else
        {
            ((YYScene*)getParent())->showTips(g_sPleaseSelecteProtoss, kTipNormal);
        }
    }
}
void PopUpUI::swallowProtoss()
{
    std::string strMsg = CCString::createWithFormat("%s_%d_", ASK_NORMAL_COMBO, m_nProtossPosID)->getCString();
    YYProtossData *pSelectedProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(m_nProtossPosID);
    pSelectedProtossData->setIsSwallowed(false);
    for (int i = 0; i < MAX_PROTOSS_BAG_NUM; i++)
    {
        YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(i);
        if (pProtossData->getID() == 0)
            continue;
        if (pProtossData->getIsSwallowed())
        {
            std::string str = CCString::createWithFormat("%d,", pProtossData->getPosID())->getCString();
            strMsg += str;
        }
    }
    strMsg = strMsg.substr(0, strMsg.length() - 1);
    Controller::getInstance()->sendMessage(strMsg.c_str(), true);
    
    removeChild(m_pUITouchGroup);
    m_pUITouchGroup = NULL;
    setDetailIndex(-1);
    m_bSelectedAllProtoss = false;
    m_pDevineBtn = NULL;
    initProtossEquipUI();
    if (getIsProtossEquipInit())
    {
        updateProtossEquipUI();
        updateProtossEquipBagUI();
    }
}
void PopUpUI::menuProtossSelected(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIImageView *pImageView = (UIImageView *)pSender;
        int nTag = pImageView->getTag();
        YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(nTag);
        if (pProtossData->getIsSwallowed())
        {
            pProtossData->setIsSwallowed(false);
        }
        else
        {
            pProtossData->setIsSwallowed(true);
        }
        updateProtossSwallowUI(nTag);
    }
}

void PopUpUI::menuProtossSelectedAll(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        m_bSelectedAllProtoss = m_bSelectedAllProtoss == true ? false : true;
        initProtossSwallowInfo(m_bSelectedAllProtoss);
        updateProtossSwallowUI(m_nProtossPosID);
    }
}

void PopUpUI::updateProtossUIPropertyInfo()
{
    Layout* protossUI = static_cast<Layout*>(m_pUITouchGroup->getWidgetByTag(TAG_UI_PROTOSS_BASE));
    if(!protossUI)
        return;
    Layout *pPanel = static_cast<Layout*>(protossUI->getChildByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    if(!pUIBase)
        return;
    Layout *pMoneyPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_0"));
    if(!pMoneyPanel)
        return;
    
    UIImageView *pImageViewMoney = static_cast<UIImageView*>(pMoneyPanel->getChildByName("ImageView_money"));
    if(pImageViewMoney != NULL)
    {
        UILabel* playerTotalMoneyValue = static_cast<UILabel*>(pImageViewMoney->getChildByName("TextArea"));
        if(playerTotalMoneyValue != NULL)
        {
            int nGold = m_pRoleByControlled->getSpriteAttribute()->getGold();
            playerTotalMoneyValue->setText(CCString::createWithFormat("%d", nGold)->getCString());         //玩家金币数 
        }
    }
    UIImageView *pImageViewJew = static_cast<UIImageView*>(pMoneyPanel->getChildByName("ImageView_jew"));
    if(pImageViewJew != NULL)
    {
        UILabel* playerTotalJewValue = static_cast<UILabel*>(pImageViewJew->getChildByName("TextArea"));
        if(playerTotalJewValue != NULL)
        {
            int nJew = m_pRoleByControlled->getSpriteAttribute()->getJewel();
            playerTotalJewValue->setText(CCString::createWithFormat("%d", nJew)->getCString());         //玩家宝石数 
        }
    }
}
void PopUpUI::setProtossMoney(int nIndex, int nGold, int nJew)
{
    m_pGoldProtoss[nIndex] = nGold;
    m_pJewProtoss[nIndex] = nJew;
}

void PopUpUI::updateProtossBaseUI(int nRefreshFlag)
{
    //find layer
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    if(!pUIBase)
        return;
    
    Layout *pDevineBtnPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_2"));
    if(!pDevineBtnPanel)
        return;
    
    int nCurIndex = getCurDevineIndex();
    if (nCurIndex < 0)
    {
        nCurIndex = 0;
        setCurDevineIndex(0);
    }
    for(int i = 0; i < MAX_DEVINE_KIND_NUM; i++)
    {
        UIButton *pDevineBtn = static_cast<UIButton *>(pDevineBtnPanel->getChildByName(CCString::createWithFormat("Button%d", i)->getCString()));
        if(!pDevineBtn)
            continue;
        
        UILabel *pLabelMoney = static_cast<UILabel *>(pDevineBtn->getChildByName("Label_0_0"));
        if (!pLabelMoney)
            continue;
        
        UIImageView *pImageIcon = (UIImageView *)pLabelMoney->getChildByTag(20);
        if (!pImageIcon)
        {
            pImageIcon = UIImageView::create();
            pImageIcon->setPosition(ccp(-30, 15));
            pLabelMoney->addChild(pImageIcon, 0, 20);
        }
        string filePath = getFilePath("300",kDirTypeUI,kFileSuffixPng);
        pImageIcon->loadTexture(filePath.c_str());
        
        CCString *pStrMoney = CCString::createWithFormat("%d", m_pGoldProtoss[i]);
        UILabel *pMoneyNum = (UILabel *)pLabelMoney->getChildByTag(10);
        if (!pMoneyNum)
        {
            pMoneyNum = createLabel(CCSizeMake(100, 30), kCCTextAlignmentCenter, 20, ccWHITE);
            pMoneyNum->setPosition(ccp(25, 20));
            pLabelMoney->addChild(pMoneyNum, 0, 10);
        }
        pMoneyNum->setText(pStrMoney->getCString());

        CCString *pStrIcon = NULL;
        if(i == nCurIndex)
        {
            pStrIcon = CCString::createWithFormat("%d", 457 + 2 * i);
        }
        else
        {
            pDevineBtn->setTouchEnabled(false);
            pStrIcon = CCString::createWithFormat("%d", 456 + 2 * i);
        }
        if (pStrIcon)
        {
            string filePath = getFilePath(pStrIcon->getCString(),kDirTypeUI,kFileSuffixPng);
            pDevineBtn->loadTextureNormal(filePath.c_str());
            pDevineBtn->loadTexturePressed(filePath.c_str());
        }
    }

//set devine bag content
    Layout *pProtossPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_1"));
    if(!pProtossPanel)
        return;
    CCArray *pArrayWaitForRecvList = getProtossWaitforRecvList();
    
    if (pArrayWaitForRecvList)
    {
        for(unsigned i = 0; i < MAX_DEVINE_BAG_NUM; i++)
        {
            UIImageView *pProtossUnit = static_cast<UIImageView *>(pProtossPanel->getChildByName(CCString::createWithFormat("ImageView%d", i)->getCString()));
            if(!pProtossUnit)
                continue;
            pProtossUnit->removeAllChildren();
            m_pArrayRecPosID[i] = -1;
        }
        int nParentIndex = 0;
        for(unsigned i = 0; i < pArrayWaitForRecvList->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)pArrayWaitForRecvList->objectAtIndex(i);
            if (pProtossData->getID() == 0)
                continue;
            UIImageView *pImageProtossWaitForRecvIcon = UIImageView::create();
            string filePath = getFilePath(pProtossData->getIcon(),kDirTypeIcon,kFileSuffixPng);
            pImageProtossWaitForRecvIcon->loadTexture(filePath.c_str());
           
            while (nParentIndex < MAX_DEVINE_BAG_NUM)
            {
                UIImageView *pProtossUnit = static_cast<UIImageView *>(pProtossPanel->getChildByName(CCString::createWithFormat("ImageView%d", nParentIndex)->getCString()));
                if(!pProtossUnit)
                    continue;
                ++nParentIndex;
                UIImageView *pImageIcon = (UIImageView *)pProtossUnit->getChildByTag(10);
                if (!pImageIcon)
                {
                    m_pArrayRecPosID[nParentIndex - 1] = pProtossData->getPosID();
                    pProtossUnit->addChild(pImageProtossWaitForRecvIcon, 0, 10);
                    pProtossUnit->setTag(nParentIndex - 1);
                    pProtossUnit->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossDevineShowFunc));
                    
                    UILabel *pTitle= createLabel(CCSizeMake(40, 30), kCCTextAlignmentCenter, 20, ccRED);
                    pTitle->setPosition(ccp(0, -pProtossUnit->getSize().width / 2 + 15));
                    pProtossUnit->addChild(pTitle);
                    std::string strTitle = pProtossData->getTitle();
                    pTitle->setText(strTitle);
                    break;
                }
            }
        }
    }
    //set enable
    pPanel->setVisible(true);
    pPanel->setTouchEnabled(true);
}
void PopUpUI::initProtossAction()
{
    Layout* protossUI = static_cast<Layout*>(m_pUITouchGroup->getWidgetByTag(TAG_UI_PROTOSS_BASE));
    if(!protossUI)
        return;
    Layout *pPanel = static_cast<Layout*>(protossUI->getChildByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    if(!pUIBase)
        return;
    
    Layout *pDevineBtnPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_2"));
    if(!pDevineBtnPanel)
        return;
        
    int nCurIndex = getCurDevineIndex();
    if (nCurIndex < 0)
    {
        nCurIndex = 0;
        setCurDevineIndex(0);
    }
    for(int i = 0; i < MAX_DEVINE_KIND_NUM; i++)
    {
        UIButton *pDevineBtn = static_cast<UIButton *>(pDevineBtnPanel->getChildByName(CCString::createWithFormat("Button%d", i)->getCString()));
        if(!pDevineBtn)
            continue;
        
        if(i == nCurIndex)
        {
            m_pDevineBtn = pDevineBtn;
            pDevineBtn->setTouchEnabled(true);
            pDevineBtn->setZOrder(3);

            YYSprite *aniSprite = (YYSprite *)m_pDevineBtn->getNodeByTag(20);
            if  (!aniSprite)
            {
                aniSprite = YYSprite::createWithActionSet("zhanling", "zhanling", kDirTypeEffect);
                aniSprite->setPosition(ccp(4, 7));
                pDevineBtn->addNode(aniSprite);
                aniSprite->setTag(20);
                aniSprite->setAction(0, false);
            }
            aniSprite->setVisible(true);
            
            YYSprite *aniSpriteSuccessed = (YYSprite *)m_pDevineBtn->getNodeByTag(30);
            if  (!aniSpriteSuccessed)
            {
                aniSpriteSuccessed = YYSprite::createWithActionSet("protossSuccess", "protossSuccess", kDirTypeEffect);
                aniSpriteSuccessed->setPosition(ccp(4, 7));
                pDevineBtn->addNode(aniSpriteSuccessed);
                aniSpriteSuccessed->setTag(30);
                aniSpriteSuccessed->setAction(0, false);
            }
            aniSpriteSuccessed->setVisible(true);
        }
    }
}
void PopUpUI::updateProtossAction()
{
    if (!getIsProtossInit() || getCurDevineIndex() < 0)
        return;
    
//    Layout* protossUI = static_cast<Layout*>(m_pUITouchGroup->getWidgetByTag(TAG_UI_PROTOSS_BASE));
//    if(!protossUI)
//        return;
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIBase = static_cast<Layout*>(pPanel->getChildByName("Panel1"));
    if(!pUIBase)
        return;
    
    Layout *pDevineBtnPanel = static_cast<Layout *>(pUIBase->getChildByName("Panel1_2"));
    if(!pDevineBtnPanel)
        return;
    for(int i = 0; i < MAX_DEVINE_KIND_NUM; i++)
    {
        UIButton *pDevineBtn = static_cast<UIButton *>(pDevineBtnPanel->getChildByName(CCString::createWithFormat("Button%d", i)->getCString()));
        if(!pDevineBtn)
            continue;
    
        YYSprite *aniSprite = (YYSprite *)pDevineBtn->getNodeByTag(20);
        if  (!aniSprite)
            continue;
        if (aniSprite->isVisible())
        {
            if (aniSprite->isActionOver())
            {
                aniSprite->setAction(0, false);
                aniSprite->setVisible(false);
            }
            else
            {
                aniSprite->updateAnimation();
            }
        }
        YYSprite *aniSpriteSuccessed = (YYSprite *)pDevineBtn->getNodeByTag(30);
        if  (!aniSpriteSuccessed)
            continue;
        if (aniSpriteSuccessed->isVisible())
        {
            if (aniSpriteSuccessed->isActionOver())
            {
                aniSpriteSuccessed->setAction(0, false);
                aniSpriteSuccessed->setVisible(false);
            }
            else
            {
                aniSpriteSuccessed->updateAnimation();
            }
        }
    }
}

void PopUpUI::updateProtossEquipBagUI()
{
    //find layer
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIEquip = static_cast<Layout*>(pPanel->getChildByName("Panel2"));
    if(!pUIEquip)
        return;

    Layout *pPanel2_1 = static_cast<Layout*>(pUIEquip->getChildByName("Panel2_1"));
    if(!pPanel2_1)
        return;
    
    UIScrollView *pScrollView = static_cast<UIScrollView *>(pPanel2_1->getChildByName("ScrollView0_0"));
    if (!pScrollView)
        return;
    m_pUIScrollView = pScrollView;
    
    CCArray *pBagArray = getProtossBagList();

    for (int i = 0; i < MAX_PROTOSS_BAG_NUM; i++)
    {
        CCString *pStr = CCString::createWithFormat("ImageView0_%d_0", i);
        UIImageView *pImageView = static_cast<UIImageView *>(pScrollView->getChildByName(pStr->getCString()));
        if (!pImageView)
            continue;
        pImageView->removeAllChildren();
        m_pArrayEquitBagPosID[i] = -1;
    }
    int nParentIndex = 0;
    for (int i = 0; i < MAX_PROTOSS_BAG_NUM; i++)
    {
        YYProtossData *pProtossData = (YYProtossData *)pBagArray->objectAtIndex(i);
        if (pProtossData->getID() == 0)
            continue;
        pProtossData->setIsSwallowed(false);
        UIImageView *pImageProtossIcon = UIImageView::create();
        string filePath = getFilePath(pProtossData->getIcon(),kDirTypeIcon,kFileSuffixPng);
        pImageProtossIcon->loadTexture(filePath.c_str());
        
        while (nParentIndex < MAX_PROTOSS_BAG_NUM)
        {
            CCString *pStr = CCString::createWithFormat("ImageView0_%d_0", nParentIndex);
            UIImageView *pImageView = static_cast<UIImageView *>(pScrollView->getChildByName(pStr->getCString()));
            if (!pImageView)
                continue;
            ++nParentIndex;
            m_pArrayEquitBagPosID[nParentIndex - 1] = pProtossData->getPosID();
            pImageView->addChild(pImageProtossIcon);
            pImageView->setTag(nParentIndex - 1);
            pImageView->addTouchEventListener(this, toucheventselector(PopUpUI::dragEquipBagIconCallback));

            UILabel *pTitle= createLabel(CCSizeMake(40, 30), kCCTextAlignmentCenter, 20, ccRED);
            pTitle->setPosition(ccp(0, -pImageView->getSize().width / 2 + 25));
            pImageView->addChild(pTitle, 1);
            std::string strTitle = pProtossData->getTitle();
            pTitle->setText(strTitle);
            break;
        }
    }
    //set enable
    pPanel->setVisible(true);
    pPanel->setTouchEnabled(true);
}
void PopUpUI::updateProtossEquipUI(int nRefreshFlag)
{
    //find layer
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIEquip = static_cast<Layout*>(pPanel->getChildByName("Panel2"));
    if(!pUIEquip)
        return;
    Layout *pPanel2_0 = static_cast<Layout*>(pUIEquip->getChildByName("Panel2_0"));
    if(!pPanel2_0)
        return;
    Layout *pPanel2_0_0 = static_cast<Layout*>(pPanel2_0->getChildByName("Panel2_0_0"));
    if(!pPanel2_0_0)
        return;
    m_pEquipPanel = pPanel2_0_0;
    
    // 重置
    for (int i = 0; i < MAX_PROTOSS_EQUIP_POS; i++)
    {
        CCString *pStr = CCString::createWithFormat("ImageView%d", i);
        UIImageView *pImageView = static_cast<UIImageView *>(pPanel2_0_0->getChildByName(pStr->getCString()));
        UIImageView *pImageProtossEquip = (UIImageView *)pImageView->getChildByTag(10);
        if (pImageProtossEquip)
        {
            pImageView->removeChildByTag(10);
        }
        UILabel *pLabel = (UILabel *)pImageView->getChildByTag(20);
        if (pLabel)
        {
            pImageView->removeChildByTag(20);
        }
        
        // 隐藏解锁等级
        UILabel* label_0 = static_cast<UILabel *>(pImageView->getChildByName("Label_0"));
        UILabel* label_1 = static_cast<UILabel *>(pImageView->getChildByName("Label_1"));
        UILabel* label_2 = static_cast<UILabel *>(pImageView->getChildByName("Label_2"));
        label_0->setVisible(false);
        label_1->setVisible(false);
        label_2->setVisible(false);
        
    }
    
    CCArray *pArrayEquip = (CCArray *)getProtossEquipList();
    for (int i = 0; i < MAX_PROTOSS_EQUIP_POS; i++)
    {
        YYProtossData *pProtossData = (YYProtossData *)pArrayEquip->objectAtIndex(i);
        CCString *pStr = CCString::createWithFormat("ImageView%d", pProtossData->getPosID());
        UIImageView *pImageView = static_cast<UIImageView *>(pPanel2_0_0->getChildByName(pStr->getCString()));
        if (pProtossData->getID() == -1)
        {
            UILabel* label_0 = static_cast<UILabel *>(pImageView->getChildByName("Label_0"));
            UILabel* label_1 = static_cast<UILabel *>(pImageView->getChildByName("Label_1"));
            UILabel* label_2 = static_cast<UILabel *>(pImageView->getChildByName("Label_2"));
            label_0->setVisible(true);
            label_1->setVisible(true);
            label_2->setVisible(true);
            CCString* tmpStr = CCString::createWithFormat("%d",pProtossData->getUnLockLevel());
            label_0->setText(tmpStr->getCString());
            label_0->setTextHorizontalAlignment(kCCTextAlignmentLeft);
            label_0->setFontSize(24);
            continue;
        }

        pImageView->setTag(i);
        pImageView->setTouchEnabled(true);
        pImageView->addTouchEventListener(this, toucheventselector(PopUpUI::tipRemoveEquipFuncCallback));
        // 隐藏锁
        UIImageView *pImageView0 = static_cast<UIImageView *>(pImageView->getChildByName("ImageView0"));
        pImageView0->setVisible(false);
        if (pProtossData->getID() != 0)
        {
            UIImageView *pImageProtoss = UIImageView::create();
            string filePath = getFilePath(pProtossData->getIcon(),kDirTypeIcon,kFileSuffixPng);
            pImageProtoss->loadTexture(filePath.c_str());
            pImageView->addChild(pImageProtoss, 0, 10);
            
            UILabel *pTitle= createLabel(CCSizeMake(40, 30), kCCTextAlignmentCenter, 20, ccRED);
            pTitle->setPosition(ccp(0, -pImageView->getSize().width / 2 + 25));
            pImageView->addChild(pTitle, 0, 20);
            std::string strTitle = pProtossData->getTitle();
            pTitle->setText(strTitle);
        }
    }
    pPanel->setVisible(true);
    pPanel->setTouchEnabled(true);
}

void PopUpUI::updateProtossExchangeUI()
{
    //find layer
    Layout* protossUI = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!protossUI)
        return;
    
    int nIndex = getDetailIndex();
    CCArray *pProtossData = getProtossExchangeRuleList();
    //CCDictionary *pProtossPieceData = getProtossPieceList();
    if(!pProtossData)
        return;
    
    //pre define
    std::string strProtossName = "";
    std::string strProtossDesc = "";
    std::string strPieceNeed = "0";
    std::string strPieceOwn = "0";
    strPieceOwn = CCString::createWithFormat("%d", m_nProtossCount)->getCString();

    if((nIndex >= 0) || (nIndex < (int)pProtossData->count()))
    {
        YYProtossData *pData = (YYProtossData *)pProtossData->objectAtIndex(nIndex);
        if(pData != NULL)
        {
            strProtossName = pData->getName();
            strProtossDesc = pData->getDesc();
            strPieceNeed = CCString::createWithFormat("%d", pData->getExchangeNum())->getCString();
        }
    }
    
    //find layers
    Layout *pPanel = static_cast<Layout*>(protossUI->getChildByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUIExchange = static_cast<Layout*>(pPanel->getChildByName("Panel3"));
    if(!pUIExchange)
        return;
    Layout *pUIDesc = static_cast<Layout*>(pUIExchange->getChildByName("Panel3_1"));
    if(!pUIDesc)
        return;
    
    //set name
    UILabel *pTextName = static_cast<UILabel*>(pUIDesc->getChildByName("TextArea_name"));	//wxf,20140227
    if(!pTextName)
        return;
    pTextName->setText(strProtossName.c_str());
    
    // 星灵描述
    CCArray* arrLabel = CCArray::create();
    for (int i = 0; i < 4; i++)
    {
        CCString* labelPath = CCString::createWithFormat("TextArea0%d",i);
        UILabel* pTextDesc = static_cast<UILabel *>(pUIDesc->getChildByName(labelPath->getCString()));
        arrLabel->addObject(pTextDesc);
    }
    showProtossDescription(strProtossDesc,arrLabel);
    
    //set piece state
    UILabel *pTextState = static_cast<UILabel*>(pUIDesc->getChildByName("TextArea1"));
    if(!pTextState)
        return;
    UILabel *pTextPieceOwn = static_cast<UILabel*>(pTextState->getChildByName("TextArea10"));
    if(!pTextPieceOwn)
        return;
    pTextPieceOwn->setText(strPieceOwn.c_str());
    
    UILabel *pTextPieceNeed = static_cast<UILabel*>(pTextState->getChildByName("TextArea12"));
    if(!pTextPieceNeed)
        return;
    pTextPieceNeed->setText(strPieceNeed.c_str());
    
    //set enable
    protossUI->setVisible(true);
    protossUI->setTouchEnabled(true);
}

void PopUpUI::updateProtossSwallowUI(int nProtossPosID)
{
    Layout *pPanel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    if(!pPanel)
        return;
    Layout *pUISwallow = static_cast<Layout*>(pPanel->getChildByName("Panel4"));
    if(!pUISwallow)
        return;
    
    Layout *pPanel4_1 = static_cast<Layout*>(pUISwallow->getChildByName("Panel4_1"));
    if(!pPanel4_1)
        return;

    UIScrollView *pScrollView = (UIScrollView *)pPanel4_1->getChildByName("ScrollView0");
    
    bool isHaveSelectedProtoss = false;
    int nProtossIndex = 0, num = 0;
    for (int i = 0; i < MAX_PROTOSS_BAG_NUM; i++)
    {
        if (m_pArrayEquitBagPosID[i] != -1)
            num++;
    }
    for (int i = 0; i < num; i++)
    {
        if (m_pArrayEquitBagPosID[i] == -1)
            break;
        CCString *pStr = CCString::createWithFormat("ImageView0_%d_0_0", i);
        UIImageView *pImageViewBg = (UIImageView *)pScrollView->getChildByName(pStr->getCString());
        pImageViewBg->removeAllChildren();
        while (nProtossIndex < num)
        {
            YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(m_pArrayEquitBagPosID[nProtossIndex]);
            if (pProtossData->getPosID() == m_nProtossPosID)
            {
                ++nProtossIndex;
                continue;
            }
            pImageViewBg->setTag(m_pArrayEquitBagPosID[nProtossIndex]);
            pImageViewBg->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossSelected));
            
            
            UIImageView *pImageViewProtoss = UIImageView::create();
            string filePath = getFilePath(pProtossData->getIcon(),kDirTypeIcon,kFileSuffixPng);
            pImageViewProtoss->loadTexture(filePath.c_str());
            pImageViewBg->addChild(pImageViewProtoss);
            
            UILabel *pTitle= createLabel(CCSizeMake(40, 30), kCCTextAlignmentCenter, 20, ccRED);
            pTitle->setPosition(ccp(0, -pImageViewBg->getSize().width / 2 + 25));
            pImageViewBg->addChild(pTitle, 1);
            std::string strTitle = pProtossData->getTitle();
            pTitle->setText(strTitle);
            if (pProtossData->getIsSwallowed())
            {
                isHaveSelectedProtoss = true;
                UIImageView *pImageViewGou = UIImageView::create();
                string filePath = getFilePath("467",kDirTypeUI,kFileSuffixPng);
                pImageViewGou->loadTexture(filePath.c_str());
                pImageViewBg->addChild(pImageViewGou);
            }
            ++nProtossIndex;
            break;
        }
    }

    Layout *pPanel4_0 = static_cast<Layout*>(pUISwallow->getChildByName("Panel4_0"));
    if(!pPanel4_0)
        return;
    
    // create new protoss
    int nProtossID = nProtossPosID;
    if (!isHaveSelectedProtoss)
        nProtossID = m_nProtossPosID;
    YYProtossData *pProtossData = phagocyticUpgradeProtoss(nProtossID);
    UIImageView *pImageViewIconBg = static_cast<UIImageView *>(pPanel4_0->getChildByName("ImageView0_0"));
    pImageViewIconBg->removeAllChildren();
    
    UIImageView *pImageViewIcon = UIImageView::create();
    string filePath = getFilePath(pProtossData->getIcon(),kDirTypeIcon,kFileSuffixPng);
    pImageViewIcon->loadTexture(filePath.c_str());
    pImageViewIconBg->addChild(pImageViewIcon);
    
    UILabel *pTitle= createLabel(CCSizeMake(40, 30), kCCTextAlignmentCenter, 20, ccRED);
    pTitle->setPosition(ccp(0, -pImageViewIconBg->getSize().width / 2 + 25));
    pImageViewIconBg->addChild(pTitle, 1);
    std::string strTitle = pProtossData->getTitle();
    pTitle->setText(strTitle);
    
    UILabel *pLabelName = static_cast<UILabel *>(pPanel4_0->getChildByName("TextArea_name_0"));
    pLabelName->setText(pProtossData->getName());
    pLabelName->setColor(getRGBValue(pProtossData->getColorType()));
    
    UILabel *pLabelRankDes = static_cast<UILabel*>(pPanel4_0->getChildByName("TextArea10"));
    UILabel *pLabelRank = static_cast<UILabel *>(pLabelRankDes->getChildByName("TextArea0"));
    CCString *pStrRank = CCString::createWithFormat("%d", pProtossData->getLevel());
    pLabelRank->setText(pStrRank->getCString());
    
    // 星灵描述
    CCArray* arrLabel = CCArray::create();
    for (int i = 0; i < 4; i++)
    {
        CCString* labelPath = CCString::createWithFormat("TextArea1%d",i+1);
        UILabel* pTextDesc = static_cast<UILabel *>(pPanel4_0->getChildByName(labelPath->getCString()));
        arrLabel->addObject(pTextDesc);
    }
    showProtossDescription(pProtossData->getDesc(),arrLabel);
    
    UIImageView *pImageSelectedAll = static_cast<UIImageView *>(pUISwallow->getChildByName("ImageView_all"));
    pImageSelectedAll->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossSelectedAll));
    pImageSelectedAll->setEnabled(true);
    pImageSelectedAll->setTouchEnabled(true);
    UIImageView *pImageSelected = static_cast<UIImageView *>(pImageSelectedAll->getChildByName("ImageView_0"));
    if (m_bSelectedAllProtoss)
    {
        pImageSelected->setVisible(true);
    }
    else
        pImageSelected->setVisible(false);
    
    UILabel *pLabelSelectedAllProtossDesc = static_cast<UILabel *>(pImageSelectedAll->getChildByName("TextArea_0"));
    pLabelSelectedAllProtossDesc->setEnabled(true);
    pLabelSelectedAllProtossDesc->setTouchEnabled(true);
    pLabelSelectedAllProtossDesc->setTag(kProtossSelectedAllProtossBtn);
    pLabelSelectedAllProtossDesc->addTouchEventListener(this, toucheventselector(PopUpUI::menuProtossSelectedAll));
    CC_SAFE_RELEASE(pProtossData);
}
void PopUpUI::dragEquipBagIconCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    UIImageView *pIcon = (UIImageView *)pSender;
    int nTag = pIcon->getTag();
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        showProtossInfoForEquipBagUI(nTag, SEL_TouchEvent(&PopUpUI::protossInfoForEquipBagUI));
    }
}


void PopUpUI::showProtossInfoForBaseUI(int nState, int nIndex, cocos2d::gui::SEL_TouchEvent btn_func)
{
    CCArray *pArray = NULL;
    YYProtossData *pProtossData = NULL;
    if (nState == 0)
    {
        if (m_pArrayRecPosID[nIndex] == -1)
            return;
        pArray = getProtossWaitforRecvList();
        pProtossData = (YYProtossData *)pArray->objectAtIndex(m_pArrayRecPosID[nIndex]);
        m_nProtossPosID = m_pArrayRecPosID[nIndex];
    }
    else
    {
        if (m_pArrayEquitBagPosID[nIndex] == -1)
            return;
        pArray = getProtossBagList();
        pProtossData = (YYProtossData *)pArray->objectAtIndex(m_pArrayEquitBagPosID[nIndex]);
        m_nProtossPosID = m_pArrayEquitBagPosID[nIndex];
    }
    
    if (!pArray || !pProtossData || pProtossData->getID() == 0)
        return;
    
    Layout *pPanel = (Layout *)getPopWindow("Panel_0", "Panel_c", true);
    UILabel *label_name = static_cast<UILabel *>(pPanel->getChildByName("Label_name"));
    CC_ASSERT(label_name);
    label_name->setText(pProtossData->getName());
    label_name->setColor(getRGBValue(pProtossData->getColorType()));
    
    // 星灵描述
    CCArray* arrLabel = CCArray::create();
    for (int i = 0; i < 4; i++)
    {
        CCString* labelPath = CCString::createWithFormat("Label_des%d",i);
        UILabel* pTextDesc = static_cast<UILabel *>(pPanel->getChildByName(labelPath->getCString()));
        arrLabel->addObject(pTextDesc);
    }
    showProtossDescription(pProtossData->getDesc(),arrLabel);
    
    UILabel *label_value = static_cast<UILabel *>(pPanel->getChildByName("Label_value"));
    CC_ASSERT(label_value);
    UILabel *label_value_0 = static_cast<UILabel *>(label_value->getChildByName("Label_0"));
    CC_ASSERT(label_value_0);
    CCString *pStrPrice = CCString::createWithFormat("%d", pProtossData->getPrice());
    label_value_0->setText(pStrPrice->getCString());
    
    UILabel *label_exp = static_cast<UILabel *>(pPanel->getChildByName("Label_exp"));
    CC_ASSERT(label_exp);
    UILabel *label_exp_0 = static_cast<UILabel *>(label_exp->getChildByName("Label_0"));
    CC_ASSERT(label_exp_0);
    CCString *pStrExp = CCString::createWithFormat("%d", pProtossData->getPhagocytic());
    label_exp_0->setText(pStrExp->getCString());
    
    UIButton *btn_left = static_cast<UIButton *>(pPanel->getChildByName("Button_0"));
    CC_ASSERT(btn_left);
    btn_left->setTag(kProtossNomalReceiveForBaseUIBtn);
    btn_left->addTouchEventListener(this, SEL_TouchEvent(btn_func));

    UIButton *btn_rigth = static_cast<UIButton *>(pPanel->getChildByName("Button_1"));
    CC_ASSERT(btn_rigth);
    btn_rigth->setTag(kProtossNomalSellForBaseUIBtn);
    btn_rigth->addTouchEventListener(this, SEL_TouchEvent(btn_func));
    
    UIButton *btn_middle = static_cast<UIButton *>(pPanel->getChildByName("Button_2"));
    CC_ASSERT(btn_middle);
    btn_middle->setTag(kProtossNomalSellForEquipBagUIBtn);
    btn_middle->addTouchEventListener(this, SEL_TouchEvent(btn_func));
    
    UIButton *btn_close = static_cast<UIButton *>(pPanel->getChildByName("Button_closed"));
    CC_ASSERT(btn_close);
    btn_close->setTag(kProtossCloseProtossInfoForBaseUIBtn);
    btn_close->addTouchEventListener(this, SEL_TouchEvent(btn_func));
    
    if (nState == 0)
    {
        btn_left->setTouchEnabled(true);
        btn_rigth->setTouchEnabled(true);
        btn_middle->setTouchEnabled(false);
        btn_left->setVisible(true);
        btn_rigth->setVisible(true);
        btn_middle->setVisible(false);
    }
    else
    {
        btn_left->setTouchEnabled(false);
        btn_rigth->setTouchEnabled(false);
        btn_middle->setTouchEnabled(true);
        btn_left->setVisible(false);
        btn_rigth->setVisible(false);
        btn_middle->setVisible(true);
    }
    m_pUITouchGroup->setTouchEnabled(false);
}
void PopUpUI::receiveOrSellCallback(CCObject *pSender ,cocos2d::gui::TouchEventType type)
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
        m_pUITouchGroup->setTouchEnabled(true);
        getPopWindow("Panel_0", "Panel_c", false);
        UIButton *button = (UIButton *)pSender;
        int nTag = button->getTag();
        switch (nTag)
        {
            case kProtossNomalReceiveForBaseUIBtn:
            {//receive
                YYProtossData *pProtossData = (YYProtossData *)getProtossWaitforRecvList()->objectAtIndex(m_nProtossPosID);
                if (pProtossData->getQuality() == 0)
                {
                    ((YYScene*)getParent())->showTips(g_sGrayProtossNoGet, kTipNormal);
                }
                else
                {
                    CCString *strMsg = CCString::createWithFormat("%s_%d", ASK_NORMAL_COLLECT, m_nProtossPosID);
                    Controller::getInstance()->sendMessage(strMsg->getCString(),false);
                }
            }
                break;
            case kProtossNomalSellForBaseUIBtn:
            {//BaseUI sell
                CCString *strMsg = CCString::createWithFormat("%s_0_%d", ASK_NORMAL_SELL, m_nProtossPosID);
                Controller::getInstance()->sendMessage(strMsg->getCString(),false);
            }
                break;
            case kProtossNomalSellForEquipBagUIBtn:
            {//EquipUI sell
                CCString *strMsg = CCString::createWithFormat("%s_1_%d", ASK_NORMAL_SELL, m_nProtossPosID);
                Controller::getInstance()->sendMessage(strMsg->getCString(),false);
            }
                break;
            case kProtossCloseProtossInfoForBaseUIBtn:
            {
                touchGroupCallBack();
            }
                break;
            default:
                break;
        }
    }
}

void PopUpUI::showProtossInfoForEquipBagUI(int nIndex, cocos2d::gui::SEL_TouchEvent btn_func)
{
    if (m_pArrayEquitBagPosID[nIndex] == -1)
        return;
    CCArray *pArray = getProtossBagList();
    YYProtossData *pProtossData = (YYProtossData *)pArray->objectAtIndex(m_pArrayEquitBagPosID[nIndex]);
    m_nProtossPosID = m_pArrayEquitBagPosID[nIndex];
    
    if (!pArray || !pProtossData || pProtossData->getID() == 0)
        return;

    Layout *pPanel = (Layout *)getPopWindow("Panel_0", "Panel_e", true);
    UILabel *label_name = static_cast<UILabel *>(pPanel->getChildByName("Label_name"));
    CC_ASSERT(label_name);
    label_name->setText(pProtossData->getName());
    label_name->setColor(getRGBValue(pProtossData->getColorType()));
    
    // 星灵描述
    CCArray* arrLabel = CCArray::create();
    for (int i = 0; i < 4; i++)
    {
        CCString* labelPath = CCString::createWithFormat("Label_des%d",i);
        UILabel* pTextDesc = static_cast<UILabel *>(pPanel->getChildByName(labelPath->getCString()));
        arrLabel->addObject(pTextDesc);
    }
    showProtossDescription(pProtossData->getDesc(),arrLabel);
    
    UILabel *label_value = static_cast<UILabel *>(pPanel->getChildByName("Label_value"));
    CC_ASSERT(label_value);
    UILabel *label_value_0 = static_cast<UILabel *>(label_value->getChildByName("Label_0"));
    CC_ASSERT(label_value_0);
    CCString *pStrPrice = CCString::createWithFormat("%d", pProtossData->getPrice());
    label_value_0->setText(pStrPrice->getCString());
    
    UILabel *label_exp = static_cast<UILabel *>(pPanel->getChildByName("Label_exp"));
    CC_ASSERT(label_exp);
    UILabel *label_exp_0 = static_cast<UILabel *>(label_exp->getChildByName("Label_0"));
    CC_ASSERT(label_exp_0);
    CCString *pStrExp = CCString::createWithFormat("%d", pProtossData->getPhagocytic());
    label_exp_0->setText(pStrExp->getCString());
    
    UIButton *btn_left = static_cast<UIButton *>(pPanel->getChildByName("Button_0"));
    CC_ASSERT(btn_left);
    btn_left->setTag(kProtossEquipProtossForEquipBagUIBtn);
    btn_left->addTouchEventListener(this, SEL_TouchEvent(btn_func));
    
    UIButton *btn_rigth = static_cast<UIButton *>(pPanel->getChildByName("Button_1"));
    CC_ASSERT(btn_rigth);
    btn_rigth->setTag(kProtossUpgradeProtossForEquipBagUIBtn);
    btn_rigth->addTouchEventListener(this, SEL_TouchEvent(btn_func));
    
    UIButton *btn_middle = static_cast<UIButton *>(pPanel->getChildByName("Button_2"));
    CC_ASSERT(btn_middle);
    btn_middle->setTag(kProtossNomalSellForEquipBagUIBtn);
    btn_middle->addTouchEventListener(this, SEL_TouchEvent(btn_func));
    
    UIButton *btn_close = static_cast<UIButton *>(pPanel->getChildByName("Button_closed"));
    CC_ASSERT(btn_close);
    btn_close->setTag(kProtossCloseProtossInfoForEquipBagUIBtn);
    btn_close->addTouchEventListener(this, SEL_TouchEvent(btn_func));
    
    m_pUITouchGroup->setTouchEnabled(false);
}

void PopUpUI::protossInfoForEquipBagUI(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        m_pUITouchGroup->setTouchEnabled(true);
        getPopWindow("Panel_0", "Panel_e", false);
        UIButton *button = (UIButton *)pSender;
        int nTag = button->getTag();
        switch (nTag)
        {
            case kProtossEquipProtossForEquipBagUIBtn:
            {//equip
                if (isEquipCommAttributeProtoss(m_nProtossPosID))
                {
                    ((YYScene*)getParent())->showTips(g_sEquipCommAttributeProtoss, kTipNormal);
                }
                else
                {
                    int nReturnValue = getEmptyEquipIndex();
                    if (nReturnValue == -1)
                    {
                        ((YYScene*)getParent())->showTips(g_sEquipNoEmpty, kTipNormal);
                    }
                    else
                    {
                        CCString *strMsg = CCString::createWithFormat("%s_%d_%d", ASK_PROTOSS_EQUIP, m_nProtossPosID, nReturnValue);
                        Controller::getInstance()->sendMessage(strMsg->getCString(),false);
                    }
                }
            }
                break;
            case kProtossUpgradeProtossForEquipBagUIBtn:
            {//upgrade
                removeChild(m_pUITouchGroup);
                m_pUITouchGroup = NULL;
                setUIType(kUIDefaultStatus);
                Controller::getInstance()->setGameStatus(kStatusScene);
                setDetailIndex(-1);
                drawProtossSwallowUI(m_nProtossPosID);
                updateProtossSwallowUI(m_nProtossPosID);
            }
                break;
            case kProtossNomalSellForEquipBagUIBtn:
            {//sell
                CCString *strMsg = CCString::createWithFormat("%s_1_%d", ASK_NORMAL_SELL, m_nProtossPosID);
                Controller::getInstance()->sendMessage(strMsg->getCString(),false);
            }
                break;
            case kProtossCloseProtossInfoForEquipBagUIBtn:
            {
                touchGroupCallBack();
            }
                break;
            default:
                break;
        }
    }

}
int PopUpUI::getEmptyEquipIndex()
{
    CCArray *pEquitArray = getProtossEquipList();
    for (int i = 0; i < pEquitArray->count(); i++)
    {
        YYProtossData *pProtossData = (YYProtossData *)pEquitArray->objectAtIndex(i);
        if (pProtossData->getID() != 0)
            continue;
        return i;
    }
    return -1;
}
bool PopUpUI::isEquipCommAttributeProtoss(int nProtossPosID)
{
    CCArray *pBagArray = getProtossBagList();
    CCArray *pEquitArray = getProtossEquipList();
    YYProtossData *pProtossBagData = (YYProtossData *)pBagArray->objectAtIndex(nProtossPosID);
    for (int i = 0; i < pEquitArray->count(); i++)
    {
        YYProtossData *pProtossEquipData = (YYProtossData *)pEquitArray->objectAtIndex(i);
        if (pProtossEquipData->getID() == 0 || pProtossEquipData->getID() == -1)
            continue;
        if (pProtossEquipData->getQuality() != 0 && pProtossBagData->getQuality() != 0)
        {
            if (pProtossEquipData->getType1() > 0 && pProtossBagData->getType1() > 0)
            {
                if (pProtossEquipData->getType1() == pProtossBagData->getType1())
                {
                    return true;
                }
            }
            if (pProtossEquipData->getType2() > 0 && pProtossBagData->getType2() > 0)
            {
                if (pProtossEquipData->getType2() == pProtossBagData->getType2())
                {
                    return true;
                }
            }
        }
        else if (pProtossEquipData->getQuality() == 0 && pProtossBagData->getQuality() == 0)
        {
            if (pProtossEquipData->getType1() == 0 && pProtossBagData->getType1() == 0)
            {
                if (pProtossEquipData->getType1() == pProtossBagData->getType1())
                {
                    return true;
                }
            }
            if (pProtossEquipData->getType2() == 0 && pProtossBagData->getType2() == 0)
            {
                if (pProtossEquipData->getType2() == pProtossBagData->getType2())
                {
                    return true;
                }
            }
        }
    }
    return false;
}
YYProtossData* PopUpUI::phagocyticUpgradeProtoss(int nProtossPosID)
{
    YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(m_nProtossPosID);
    YYProtossData *pProtossDataBackup = YYProtossData::create();
    pProtossDataBackup->clone(pProtossData);
    int nCurrentExp = pProtossDataBackup->getCurrentExp();
    
    //CCLog("nCurrentExp0 = %d, getPhagocytic = %d", nCurrentExp, pProtossDataBackup->getPhagocytic());
    for (int i = 0; i < MAX_PROTOSS_BAG_NUM; i++)
    {
        if (m_pArrayEquitBagPosID[i] == -1)
            break;
        YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(m_pArrayEquitBagPosID[i]);
        if (pProtossData->getIsSwallowed())
        {
            // compute rank;
            //CCLog("nCurrentExp1 = %d, getPhagocytic = %d", nCurrentExp, pProtossData->getPhagocytic());
            nCurrentExp += pProtossData->getPhagocytic();
            pProtossDataBackup->setCurrentExp(nCurrentExp);
            //CCLog("nCurrentExp2= %d, getPhagocytic = %d", nCurrentExp, pProtossData->getPhagocytic());
        }
    }
    int nIndex = 0;
    
    CCArray *pArrayProtossData = GlobalData::getInstance()->getProtossDataArray();
    if (pProtossDataBackup->getCurrentExp() >= pProtossDataBackup->getExp())
    {
        for (int i = 0; i < pArrayProtossData->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)pArrayProtossData->objectAtIndex(i);
            if (pProtossDataBackup->getNext() == pProtossData->getID())
            {
                //CCLog("geID2 = %d", pProtossData->getID());
                nIndex = i;
                break;
            }
        }
    }
   
    if (nIndex != 0)
    {
        YYProtossData *pProtossData_current = (YYProtossData *)pArrayProtossData->objectAtIndex(nIndex);
        //CCLog("nCurrentExp_current = %d, nPhagocytic_current = %d, ninitExp_current = %d", pProtossData_current->getCurrentExp(), pProtossData_current->getPhagocytic(),
        //pProtossData_current->getExp());
        pProtossDataBackup->clone(pProtossData_current);
    }
    return pProtossDataBackup;
}

void PopUpUI::initProtossSwallowInfo(bool bVisible)
{
    for (int i = 0; i < MAX_PROTOSS_BAG_NUM; i++)
    {
        if (m_pArrayEquitBagPosID[i] == -1)
            break;
        if (m_pArrayEquitBagPosID[i] == m_nProtossPosID)
            continue;
        YYProtossData *pProtossData = (YYProtossData *)m_pProtossBagList->objectAtIndex(m_pArrayEquitBagPosID[i]);
        pProtossData->setIsSwallowed(bVisible);
    }
}
void PopUpUI::tipRemoveEquipFuncCallback(CCObject *pSender ,cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    UIImageView *pImageView = (UIImageView *)pSender;
    int nTag = pImageView->getTag();

    if (type == TOUCH_EVENT_ENDED)
    {
        CCArray *pArrayEquip = (CCArray *)getProtossEquipList();
        YYProtossData *pProtossData = (YYProtossData *)pArrayEquip->objectAtIndex(nTag);
        m_nProtossEquipID = pProtossData->getPosID();
        if (pProtossData->getID() == -1)
        {
            ((YYScene*)getParent())->showTips(g_sLockProtossEquip, kTipNormal);
        }
        else if (pProtossData->getID() == 0)
        {
            ((YYScene*)getParent())->showTips(g_sHaveNotEquipProtoss, kTipNormal);
        }
        else
        {
            CCString *pStr = CCString::createWithFormat("%s%s%s%s%s%s", g_sYes, g_sNo, g_sRemove, g_sThis, g_sProtoss, g_sQuestionMark);
            ((YYScene*)getParent())->showTips(pStr->getCString(), kTipRemoveProtoss);
        }
    }
}

void PopUpUI::touchGroupCallBack()
{
    if (getUIType() == kUIProtoss)
    {
        m_pUITouchGroup->setTouchEnabled(true);
        //星灵界面点击删除成就提示框
        if (m_bIsPopUpAchievementTip)
        {
            m_fStartTime = 0;
            m_bIsPopUpAchievementTip = false;
            YYTouchGroup *touchPop = static_cast<YYTouchGroup *>(getChildByTag(TAG_POPUP_ACHIEVE_WINDOW));
            if (touchPop)
                removeChild(touchPop);
        }
    }
    else if (getUIType() == kUIAchieve)
    {
        m_fStartTime = 0;
        m_bIsPopUpAchievementTip = false;
        YYTouchGroup *touchPop = static_cast<YYTouchGroup *>(getChildByTag(TAG_POPUP_ACHIEVE_WINDOW));
        if (touchPop)
            removeChild(touchPop);
    }
}

// PA
void PopUpUI::setProtossWaitRecvData(cocos2d::CCArray *array)
{
    if (!array)
        return;
    
    CCArray *pRecvListArray = getProtossWaitforRecvList();
    if(!pRecvListArray)
        return;
    
    if (pRecvListArray->count() == MAX_DEVINE_BAG_NUM)
    {
        for (int i = 0; i < pRecvListArray->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)pRecvListArray->objectAtIndex(i);
            CCString *pStrID = (CCString *)array->objectAtIndex(i);
            if (pStrID->length() > 0)
            {
                if (pProtossData->getPosID() == i)
                {
                    if (pStrID->intValue() != 0)
                    {
                        YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                        if (pLocalData)
                        {
                            pProtossData->clone(pLocalData);
                        }
                    }
                    else
                        pProtossData->setID(0);
                }
            }
        }
        updateProtossBaseUI();
    }
    else if (pRecvListArray->count() < MAX_DEVINE_BAG_NUM)
    {
        for (int i = 0; i < array->count(); i++)
        {
            CCString *pStrID = (CCString *)array->objectAtIndex(i);
            if (pStrID->length() > 0)
            {
                YYProtossData *pProtossData = YYProtossData::create();
                pProtossData->setPosID(i);
                pProtossData->setID(pStrID->intValue());
                if (pStrID->intValue() != 0)
                {
                    YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                    if (pLocalData)
                    {
                        pProtossData->clone(pLocalData);
                    }
                }
                pRecvListArray->addObject(pProtossData);
            }
        }
    }
}
// PB
void PopUpUI::setProtossWaitRecvOneData(cocos2d::CCArray *array)
{
    if (!array)
        return;
    
    CCArray *pRecvListArray = getProtossWaitforRecvList();
    if(!pRecvListArray)
        return;
    
    if (pRecvListArray->count() == MAX_DEVINE_BAG_NUM)
    {
        CCString *pStrPosID = (CCString *)array->objectAtIndex(0);
        for (int i = 0; i < pRecvListArray->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)pRecvListArray->objectAtIndex(i);
            if (pStrPosID->length() > 0)
            {
                if (pProtossData->getPosID() == pStrPosID->intValue())
                {
                    CCString *pStrID = (CCString *)array->objectAtIndex(1);
                    if (pStrID->length() > 0)
                    {
                        if (pStrID->intValue() != 0)
                        {
                            YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                            if (pLocalData)
                            {
                                pProtossData->clone(pLocalData);
                            }
                        }
                        else
                            pProtossData->setID(0);
                    }
                }
            }
        }
        updateProtossBaseUI();
    }
}

// PC
void PopUpUI::setProtossBagData(cocos2d::CCArray *array)
{
    if (!array)
        return;
    
    CCArray *pProtossBagArray = getProtossBagList();
    if(!pProtossBagArray)
        return;
    
    if (pProtossBagArray->count() == MAX_PROTOSS_BAG_NUM)
    {
        for (int i = 0; i < pProtossBagArray->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)pProtossBagArray->objectAtIndex(i);
            if (pProtossData->getPosID() == i)
            {
                CCString *pStr = (CCString *)array->objectAtIndex(i);
                if (pStr->length() > 0)
                {
                    CCArray* pArray = split_string(pStr->getCString(), ';');
                    if (pArray)
                    {
                        CCString *pStrID = (CCString *)pArray->objectAtIndex(0);
                        if (pStrID->length() > 0)
                        {
                            if (pStrID->intValue() != 0)
                            {
                                YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                                if (pLocalData)
                                {
                                    pProtossData->clone(pLocalData);
                                }
                            }
                            else
                            {
                                pProtossData->setID(0);
                            }
                            if (pArray->count() >= 2)
                            {
                                CCString *pStrCurrentExp = (CCString *)pArray->objectAtIndex(1);
                                if (pStrCurrentExp->length() > 0)
                                {
                                    pProtossData->setCurrentExp(pStrCurrentExp->intValue());
                                }
                            }
                        }
                    }
                }
            }
        }
        updateProtossEquipBagUI();
    }
    else if (pProtossBagArray->count() < MAX_PROTOSS_BAG_NUM)
    {
        if (getIsEnterProtossEquipUI())
        {
            for (int i = 0; i < array->count(); i++)
            {
                CCString *pStr = (CCString *)array->objectAtIndex(i);
                if (pStr->length() > 0)
                {
                    CCArray* pArray = split_string(pStr->getCString(), ';');
                    if (pArray)
                    {
                        CCString *pStrID = (CCString *)pArray->objectAtIndex(0);
                        if (pStrID->length() > 0)
                        {
                            YYProtossData *pProtossData = YYProtossData::create();
                            pProtossData->setPosID(i);
                            pProtossData->setID(pStrID->intValue());
                            if (pStrID->intValue() != 0)
                            {
                                YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                                if (pLocalData)
                                {
                                    pProtossData->clone(pLocalData);
                                }
                            }
                            if (pArray->count() >= 2)
                            {
                                CCString *pStrCurrentExp = (CCString *)pArray->objectAtIndex(1);
                                if (pStrCurrentExp->length() > 0)
                                {
                                    pProtossData->setCurrentExp(pStrCurrentExp->intValue());
                                }
                            }
                            pProtossBagArray->addObject(pProtossData);
                        }
                    }
                }
            }
        }
    }
}
// PD
void PopUpUI::setProtossBagOneData(cocos2d::CCArray *array)
{
    if (!array)
        return;
    
    CCArray *pProtossBagArray = getProtossBagList();
    if(!pProtossBagArray)
        return;
    
    if (pProtossBagArray->count() == MAX_PROTOSS_BAG_NUM)
    {
        CCString *pStrPosID = (CCString *)array->objectAtIndex(0);
        for (int i = 0; i < pProtossBagArray->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)pProtossBagArray->objectAtIndex(i);
            if (pStrPosID->length() > 0)
            {
                if (pProtossData->getPosID() == pStrPosID->intValue())
                {
                    CCString *pStr = (CCString *)array->objectAtIndex(1);
                    if (pStr->length() > 0)
                    {
                        CCArray* pArray = split_string(pStr->getCString(), ';');
                        if (pArray)
                        {
                            CCString *pStrID = (CCString *)pArray->objectAtIndex(0);
                            if (pStrID->length() > 0)
                            {
                                if (pStrID->intValue() != 0)
                                {
                                    YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                                    if (pLocalData)
                                    {
                                        pProtossData->clone(pLocalData);
                                    }
                                }
                                else
                                {
                                    pProtossData->setID(0);
                                }
                                if (pArray->count() >= 2)
                                {
                                    CCString *pStrCurrentExp = (CCString *)pArray->objectAtIndex(1);
                                    if (pStrCurrentExp->length() > 0)
                                    {
                                        pProtossData->setCurrentExp(pStrCurrentExp->intValue());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        updateProtossEquipBagUI();
    }
}
// PE_PF PE_2071;0_0_25_30_35_40_45_50_55_60 0 表示没有星灵 单个数字表示星灵解锁等级 2个数字 星灵ID;经验
void PopUpUI::setProtossEquipData(cocos2d::CCArray *array)
{
    if (!array)
        return;
    
    CCArray *pProtossEquipArray = getProtossEquipList();
    if(!pProtossEquipArray)
        return;
    if (pProtossEquipArray->count() == MAX_PROTOSS_EQUIP_POS)
    {
        CCString *pStrPosID = (CCString *)array->objectAtIndex(0);
        for (int i = 0; i < pProtossEquipArray->count(); i++)
        {
            YYProtossData *pProtossData = (YYProtossData *)pProtossEquipArray->objectAtIndex(i);
            if (pStrPosID->length() > 0)
            {
                if (pProtossData->getPosID() == pStrPosID->intValue())
                {
                    CCString *pStr = (CCString *)array->objectAtIndex(1);
                    if (pStr->length() > 0)
                    {
                        CCArray* pArray = split_string(pStr->getCString(), ';');
                        if (pArray)
                        {
                            CCString *pStrID = (CCString *)pArray->objectAtIndex(0);
                            if (pStrID->length() > 0)
                            {
                                if (pStrID->intValue() != -1 && pStrID->intValue() != 0)
                                {
                                    YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                                    if (pLocalData)
                                    {
                                        pProtossData->clone(pLocalData);
                                    }
                                }
                                else
                                {
                                    pProtossData->setID(pStrID->intValue());
                                }
                                if (pArray->count() >= 2)
                                {
                                    CCString *pStrCurrentExp = (CCString *)pArray->objectAtIndex(1);
                                    if (pStrCurrentExp->length() > 0)
                                    {
                                        pProtossData->setCurrentExp(pStrCurrentExp->intValue());
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        updateProtossEquipUI();
    }
    else if (pProtossEquipArray->count() < MAX_PROTOSS_EQUIP_POS)
    {
        for (int i = 0; i < array->count(); i++)
        {
            CCString *pStr = (CCString *)array->objectAtIndex(i);
            if (pStr->length() > 0)
            {
                CCArray* pArray = split_string(pStr->getCString(), ';');
                if (pArray)
                {
                    CCString *pStrID = (CCString *)pArray->objectAtIndex(0);
                    if (pStrID->length() > 0)
                    {
                        YYProtossData *pProtossData = YYProtossData::create();
                        pProtossData->setPosID(i);
                        pProtossData->setID(pStrID->intValue());
                        if (pStrID->intValue() != 0)
                        {
                            YYProtossData *pLocalData = GlobalData::getInstance()->getProtossData(pStrID->intValue());
                            if (pLocalData)
                            {
                                pProtossData->clone(pLocalData);
                            }
                            if (pArray->count() == 1)
                            {
                                pProtossData->setID(-1);
                                pProtossData->setUnLockLevel(pStrID->intValue());
                            }
                        }
                        if (pArray->count() >= 2)
                        {
                            CCString *pStrCurrentExp = (CCString *)pArray->objectAtIndex(1);
                            if (pStrCurrentExp->length() > 0)
                            {
                                pProtossData->setCurrentExp(pStrCurrentExp->intValue());
                            }
                        }
                        pProtossEquipArray->addObject(pProtossData);
                    }
                }
            }
        }
    }
}

#pragma mark 碎片数量改变

void PopUpUI::setProtossPieceData(cocos2d::CCArray *array)
{
	if(array->count() > 0)
	{
		int count = 0;
		CCString *szCount = (CCString*)(array->objectAtIndex(0));
		count = szCount->intValue();
		setProtossCount(count);
        
        //碎片总数UI改变
        Layout* protossUI = static_cast<Layout*>(m_pUITouchGroup->getWidgetByTag(TAG_UI_PROTOSS_EXCHANGE));
        if(!protossUI)
            return;
        Layout *pPanel = static_cast<Layout*>(protossUI->getChildByName("Panel0"));
        if(!pPanel)
            return;
        Layout *pUIExchange = static_cast<Layout*>(pPanel->getChildByName("Panel3"));
        if(!pUIExchange)
            return;
        Layout *pUIDesc = static_cast<Layout*>(pUIExchange->getChildByName("Panel3_1"));
        if(!pUIDesc)
            return;

        std::string strPieceOwn = "0";
        strPieceOwn = CCString::createWithFormat("%d", getProtossCount())->getCString();
        UILabel *pTextState = static_cast<UILabel*>(pUIDesc->getChildByName("TextArea1"));
        if(!pTextState)
            return;
        
        UILabel *pTextPieceNeed = static_cast<UILabel*>(pTextState->getChildByName("TextArea10"));
        if(!pTextPieceNeed)
            return;
        pTextPieceNeed->setText(strPieceOwn.c_str());
	}
}

#pragma mark 显示星灵描述

void PopUpUI::showProtossDescription(std::string desc, cocos2d::CCArray* arrLabel)
{
    CCArray* arrDesc = split_string(desc.c_str(), '_');
    for (int i = 0; i < 4; i++)
    {
        UILabel* pDesc = static_cast<UILabel *>(arrLabel->objectAtIndex(i));
        if (i < arrDesc->count())
        {
            pDesc->setVisible(true);
            std::string tmpDesc = static_cast<CCString*>(arrDesc->objectAtIndex(i))->getCString();
            pDesc->setText(tmpDesc.c_str());
        }
        else
        {
            pDesc->setVisible(false);
            pDesc->setText(" ");
        }
    }
}
