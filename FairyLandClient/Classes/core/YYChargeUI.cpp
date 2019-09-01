//
//  YYChargeUI.cpp
//  Fairyland
//
//  Created by yoyo on 14-5-22.
//
//

#include "YYPopUpUI.h"
#include "YYChargeData.h"
#include "../common/ccUtil.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <jni.h>

extern "C"
{
    void Java_com_yourcompany_fairyland_FairyLand_returnPayInfo(JNIEnv *env, jobject thiz, jint result)
    {
        int value = result;
        Controller::getInstance()->getScene()->getPopUpUI()->returnPayInfoTip(value);
    }
    
    void Java_com_yourcompany_fairyland_FairyLand_nativeSMSSuccessful()
    {
        CCLOG("mobile message success");
    }

    void Java_com_yourcompany_fairyland_FairyLand_nativeSMSFailed()
    {
        CCLOG("mobile message Failed");
    }
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

#include "ChinaMobilePay.h"

#endif


void PopUpUI::returnPayInfoTip(int result)
{
    if (result == 1)
    {
        CCMessageBox(g_sPaySuccess, "");
    }
    else
    {
        CCMessageBox(g_sPayFailed, "");
    }
}

void PopUpUI::initChargeUI()
{
    // 屏蔽ios移动基地支付
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    ChinaMobilePay::ChinaMobileChargeUI();
//    return;
#endif

    m_eGameStatePreviousForCharge = Controller::getInstance()->getGameStatus();
    m_eKUITypePreviousForCharge = getUIType();
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    if (m_eGameStatePreviousForCharge == kStatusUI)
    {
        m_pUITouchGroup->setTouchEnabled(false);
        m_pUIChargeTouchGroup = m_pUITouchGroup;
    }
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUICharge);
    setChargePageType(kChargeInit);
    m_pRoleByControlled =  Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled();
    m_bInitChargeInfo = false;
    
    m_pUITouchGroup = TouchGroup::create();
    if(!m_pUITouchGroup)
        return;
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = CHARGE_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout *pPanel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton *pBtnClose = static_cast<UIButton  *>(pPanel0->getChildByName("Button_closed"));
    if(pBtnClose != NULL)
    {
        pBtnClose->addTouchEventListener(this, toucheventselector(PopUpUI::menuChargeUIClosed));
    }

    Layout *pPanel1 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel1"));
    
    UIButton *pBtn0 = static_cast<UIButton  *>(pPanel1->getChildByName("Button0"));
    if(pBtn0 != NULL)
    {
        pBtn0->setTag(kChargeAlipay);
        pBtn0->addTouchEventListener(this, toucheventselector(PopUpUI::menuChargePageType));
    }
    UIButton *pBtn1 = static_cast<UIButton  *>(pPanel1->getChildByName("Button1"));
    if(pBtn1 != NULL)
    {
        pBtn1->setTag(kChargeTelephone);
        pBtn1->addTouchEventListener(this, toucheventselector(PopUpUI::menuChargePageType));
    }
    UIButton *pBtn2 = static_cast<UIButton  *>(pPanel1->getChildByName("Button2"));
    if(pBtn2 != NULL)
    {
        pBtn2->setTag(kChargeInternetBank);
        pBtn2->addTouchEventListener(this, toucheventselector(PopUpUI::menuChargePageType));
    }
    UIButton *pBtn3 = static_cast<UIButton  *>(pPanel1->getChildByName("Button3"));
    if(pBtn3 != NULL)
    {
        pBtn3->setEnabled(true);
        pBtn3->setVisible(true);
        pBtn3->setEnabled(true);
        pBtn3->setTag(kChargeHaseePay);
        pBtn3->setTitleText(g_sHaseePay);
        pBtn3->addTouchEventListener(this, toucheventselector(PopUpUI::menuChargePageType));
    }
    
    // 移动Android短信支付
    pBtn0->setTitleText(g_sMobileMessagePay);
    pBtn0->setTouchEnabled(false);
    pBtn1->setEnabled(false);
    pBtn2->setEnabled(false);
    pBtn3->setEnabled(false);
    
    
    
    Layout *pPanel2 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    if(!pPanel2)
        return;
    UIScrollView *pScrollView = static_cast<UIScrollView *>(pPanel2->getChildByName("ScrollView"));
    Layout *pScrollViewInnerLayer = pScrollView->getInnerContainer();
    m_obScrollViewPoint = pScrollViewInnerLayer->getPosition();
    //money and jew
    updateChargeUIPropertyInfo();
    
    if (!m_bInitChargeInfo)
    {
        Controller::getInstance()->sendMessage(ASK_CHARGE_LIST, true);
    }
}

void PopUpUI::updateChargeUIPropertyInfo()
{
    Layout *pPanel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    if(!pPanel)
        return;
    UIImageView *pImageViewMoney = static_cast<UIImageView*>(pPanel->getChildByName("ImageView_0"));
    if(pImageViewMoney != NULL)
    {
        UILabel* playerTotalMoneyValue = static_cast<UILabel*>(pImageViewMoney->getChildByName("TextArea0"));
        if(playerTotalMoneyValue != NULL)
        {
            int nGold = m_pRoleByControlled->getSpriteAttribute()->getGold();
            playerTotalMoneyValue->setText(CCString::createWithFormat("%d", nGold)->getCString());         //玩家金币数
        }
    }
    UIImageView *pImageViewJew = static_cast<UIImageView*>(pPanel->getChildByName("ImageView_1"));
    if(pImageViewJew != NULL)
    {
        UILabel* playerTotalJewValue = static_cast<UILabel*>(pImageViewJew->getChildByName("TextArea0"));
        if(playerTotalJewValue != NULL)
        {
            int nJew = m_pRoleByControlled->getSpriteAttribute()->getJewel();
            playerTotalJewValue->setText(CCString::createWithFormat("%d", nJew)->getCString());         //玩家宝石数
        }
    }
}
void PopUpUI::refreshChargeItemList(ChargePageType kType)
{
    setChargeID(-1);
    refreshChargeBtnTypeUI(kType);
    Layout *pPanel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    if(!pPanel)
        return;
    UIScrollView *pScrollView = static_cast<UIScrollView *>(pPanel->getChildByName("ScrollView"));
    if (!pScrollView)
        return;
    Layout *pScrollViewInnerLayer = pScrollView->getInnerContainer();
    pScrollViewInnerLayer->setPosition(m_obScrollViewPoint);
    ChargePageType type = kType;
    if (kType == kChargeHaseePay)
        type = kChargeInternetBank;
    CCArray *pChargeArray = (CCArray *)m_pChargeContentDictionary->objectForKey(CCString::createWithFormat("tab%d", type)->getCString());
    for (int i = 0; i < MAX_CHARGE_ITEM_COUNT; i++)
    {
        CCString *pStr = CCString::createWithFormat("ImageView%d", i);
        UIImageView *pImageView = static_cast<UIImageView *>(pScrollView->getChildByName(pStr->getCString()));
        if (!pImageView)
            continue;
        
        if (i >= pChargeArray->count())
        {
            pImageView->setEnabled(false);
            pImageView->setVisible(false);
            continue;
        }
        pImageView->setEnabled(true);
        pImageView->setVisible(true);
        pImageView->setTag(i);
        pImageView->addTouchEventListener(this, toucheventselector(PopUpUI::menuChargeCallback));
        
        YYChargeData *pChargeData = (YYChargeData *)pChargeArray->objectAtIndex(i);
        
        CCString *pStrJewelNum = CCString::createWithFormat("%d", pChargeData->getJewelNum());
        UIImageView *pImageViewJewel = static_cast<UIImageView *>(pImageView->getChildByName("ImageView_0"));
        UILabel *pLabelJewelNum = static_cast<UILabel *>(pImageViewJewel->getChildByName("Label_0"));
        pLabelJewelNum->setText(pStrJewelNum->getCString());
        
        CCString *pStrRMBNum = CCString::createWithFormat("%d", pChargeData->getRMBNum());
        UIImageView *pImageViewRMB = static_cast<UIImageView *>(pImageView->getChildByName("ImageView_1"));
        UILabel *pLabelRMBNum = static_cast<UILabel *>(pImageViewRMB->getChildByName("Label_1"));
        pLabelRMBNum->setText(pStrRMBNum->getCString());
    }
}
void PopUpUI::refreshChargeBtnTypeUI(ChargePageType kType)
{
    Layout *pPanel1 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel1"));
    for (int i = 0; i < kChargePageNum; i++)
    {
        CCString *pStr = CCString::createWithFormat("Button%d", i);
        UIButton *pButton = static_cast<UIButton *>(pPanel1->getChildByName(pStr->getCString()));
        if (kType == i)
        {
            const std::string filePath = getFilePath("206",kDirTypeUI,kFileSuffixPng);
            pButton->loadTextureNormal(filePath.c_str());
        }
        else
        {
            const std::string filePath = getFilePath("205",kDirTypeUI,kFileSuffixPng);
            pButton->loadTextureNormal(filePath.c_str());
        }
    }
}
void PopUpUI::exitCharge()
{
    for(int i = 0; i < kChargePageNum; i++)
    {
        CCArray *pChargeArray = (CCArray *)m_pChargeContentDictionary->objectForKey(CCString::createWithFormat("tab%d", i)->getCString());
        if(pChargeArray)
            pChargeArray->removeAllObjects();
    }
}
void PopUpUI::menuChargeUIClosed(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        exitCharge();
        setChargePageType(kChargeInit);
        if (m_pUITouchGroup)
        {
            removeChild(m_pUITouchGroup);
            m_pUITouchGroup = NULL;
        }
        if (m_eGameStatePreviousForCharge == kStatusUI)
        {
            m_pUITouchGroup = m_pUIChargeTouchGroup;
            m_pUIChargeTouchGroup = NULL;
            m_pUITouchGroup->setTouchEnabled(true);
        }
        Controller::getInstance()->setGameStatus(m_eGameStatePreviousForCharge);
        setUIType(m_eKUITypePreviousForCharge);
    }
}
void PopUpUI::menuChargePageType(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        UIButton *pButton = (UIButton *)pSender;
        //pButton->loadTextureNormal(UI_PATH_PIC("206"));
        ChargePageType kType = (ChargePageType)pButton->getTag();
        setChargePageType(kType);
        kType == kChargeHaseePay ? initHaseePayUI() : refreshChargeItemList(kType);
    }
}
void PopUpUI::menuChargeCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        if (m_bInitChargeInfo)
        {
            UIButton *pButton = (UIButton *)pSender;
            CCArray *pChargeArray = (CCArray *)m_pChargeContentDictionary->objectForKey(CCString::createWithFormat("tab%d", getChargePageType())->getCString());
            YYChargeData *pChargeData = (YYChargeData *)pChargeArray->objectAtIndex(pButton->getTag());
            m_nChargeID = pButton->getTag();
            CCString *pStrTip = CCString::createWithFormat(g_sRMBChargeJew, pChargeData->getRMBNum(), pChargeData->getJewelNum());
            ((YYScene*)getParent())->showTips(pStrTip->getCString(), kTipGoChargeForChargeUI);
        }
    }
}

void PopUpUI::sendMsgForSerialNumberInfo()
{
    
// 暂时屏蔽其他支付方式
//    if (getChargeID() != -1)
//    {
//        CCString *pStrMsg = CCString::createWithFormat("%s %d_%d", ASK_CHARGE_RESQUEST, getChargePageType(), getChargeID());
//        Controller::getInstance()->sendMessage(pStrMsg->getCString(), true);
//    }
    
    // 短信支付
    chargeMobileMessagePay();
}

void PopUpUI::chargeMobileMessagePay()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;//定义Jni函数信息结构体
    bool isExist = JniHelper::getStaticMethodInfo(minfo, "com/yourcompany/fairyland/FairyLand", "sendMessage", "(Ljava/lang/String;Ljava/lang/String;)V");
    if (isExist)
    {
        jstring content = minfo.env->NewStringUTF("123");
        jstring number = minfo.env->NewStringUTF("10086");
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, content, number);     //调用函数
        minfo.env->DeleteLocalRef(minfo.classID);
    }
#endif
}

void PopUpUI::goChargeUI(ChargePageType kTypePage)
{
    initChargeUI();
}
void PopUpUI::setChargeDataInfo(cocos2d::CCArray *pArray)
{
    if (!pArray)
        return;
    
    for (int i = 0; i < pArray->count(); i++)
    {
        CCArray *pArrayChargeType = (CCArray *)m_pChargeContentDictionary->objectForKey(CCString::createWithFormat("tab%d", i)->getCString());
        CCString *pStrContent = (CCString *)pArray->objectAtIndex(i);
        if (pStrContent)
        {
            CCArray* pArrayNumber = split_string(pStrContent->getCString(), ';');
            for (int j = 0; j < pArrayNumber->count(); j++)
            {
                CCString *pStrUnit = (CCString *)pArrayNumber->objectAtIndex(j);
                if (pStrUnit)
                {
                    CCArray* pArrayUnit = split_string(pStrUnit->getCString(), ',');
                    YYChargeData *pChargeData = YYChargeData::create();
                    // id
                    pChargeData->setID(j);
                    //jewel num
                    CCString *pStrJewel = (CCString *)pArrayUnit->objectAtIndex(0);
                    if (pStrJewel)
                        pChargeData->setJewelNum(pStrJewel->intValue());
                    // RMB num
                    CCString *pStrRMB= (CCString *)pArrayUnit->objectAtIndex(1);
                    if (pStrRMB)
                        pChargeData->setRMBNum(pStrRMB->intValue());
                
                    pArrayChargeType->addObject(pChargeData);
                }
            }
        }
    }
    m_bInitChargeInfo = true;
    setChargePageType(kChargeAlipay);
    refreshChargeItemList(kChargeAlipay);
}

void PopUpUI::setChargeSerialNumberInfo(cocos2d::CCArray *pArray)
{
    if (!pArray)
        return;
    CCString *pStrContent = (CCString *)pArray->objectAtIndex(0);
    if (pStrContent)
        setSerialNumber(pStrContent->m_sString);
    //CCLog("%s", getSerialNumber().c_str());
    if (getChargePageType() != kChargeHaseePay)
        chargeAgency();
    else
        HaseePaychargeAgency();
}
void PopUpUI::chargeAgency()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;//定义Jni函数信息结构体
    bool bIsHave = JniHelper::getStaticMethodInfo(minfo, "com/yourcompany/fairyland/FairyLand", "req_zfb", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (bIsHave)
    {
        CCArray *pChargeArray = (CCArray *)m_pChargeContentDictionary->objectForKey(CCString::createWithFormat("tab%d", getChargePageType())->getCString());
        YYChargeData *pChargeData = (YYChargeData *)pChargeArray->objectAtIndex(m_nChargeID);
        CCString *pStr = CCString::createWithFormat("%d", pChargeData->getRMBNum());
        std::string yuan = pStr->m_sString;
        std::string outTrade = getSerialNumber();
        std::string expend = "";
        std::string subject = "buy jewel";
        jstring jstrYuan = minfo.env->NewStringUTF(yuan.c_str());
        jstring jstrTrade = minfo.env->NewStringUTF(outTrade.c_str());
        jstring jstrExpend = minfo.env->NewStringUTF(expend.c_str());
        jstring jstrSubject = minfo.env->NewStringUTF(subject.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jstrYuan, jstrTrade, jstrExpend, jstrSubject);     //调用函数
        minfo.env->DeleteLocalRef(jstrYuan);
        minfo.env->DeleteLocalRef(jstrTrade);
        minfo.env->DeleteLocalRef(jstrExpend);
        minfo.env->DeleteLocalRef(jstrSubject);
        minfo.env->DeleteLocalRef(minfo.classID);
  }
#endif
}
void PopUpUI::HaseePaychargeAgency()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo minfo;
    bool bIsHave = JniHelper::getStaticMethodInfo(minfo, "com/yourcompany/fairyland/FairyLand", "req_szf", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (bIsHave)
    {
        CCString *pStrCardType = CCString::createWithFormat("%d", getHaseePayPageType());
        std::string strCardType = pStrCardType->getCString();
        CCString *pStrCardQuota = CCString::createWithFormat("%d", getSelectedQuota());
        std::string strCardQuota = pStrCardQuota->getCString();
        std::string expend = "";
        jstring jstrCardNumber = minfo.env->NewStringUTF(getCardNumber().c_str());
        jstring jstrCardPassword = minfo.env->NewStringUTF(getCardPassword().c_str());
        jstring jstrCardType = minfo.env->NewStringUTF(strCardType.c_str());
        jstring jstrSelectedQuota = minfo.env->NewStringUTF(strCardQuota.c_str());
        jstring jstrTrade = minfo.env->NewStringUTF(getSerialNumber().c_str());
        jstring jstrYoYoQuota = minfo.env->NewStringUTF(strCardQuota.c_str());
        jstring jstrExpend = minfo.env->NewStringUTF(expend.c_str());
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID, jstrCardNumber, jstrCardPassword, jstrCardType, jstrSelectedQuota, jstrTrade, jstrYoYoQuota, jstrExpend);     //调用函数
        minfo.env->DeleteLocalRef(jstrCardNumber);
        minfo.env->DeleteLocalRef(jstrCardPassword);
        minfo.env->DeleteLocalRef(jstrCardType);
        minfo.env->DeleteLocalRef(jstrSelectedQuota);
        minfo.env->DeleteLocalRef(jstrTrade);
        minfo.env->DeleteLocalRef(jstrYoYoQuota);
        minfo.env->DeleteLocalRef(jstrExpend);
        minfo.env->DeleteLocalRef(minfo.classID);
    }
#endif
}
void PopUpUI::initHaseePayUI()
{
    refreshChargeBtnTypeUI(getChargePageType());
    m_pUITouchGroup->setTouchEnabled(false);
    m_pHaseePayTouchGroup = m_pUITouchGroup;
    setHaseePayPageType(kHaseePayMobile);
    setSelectedQuota(0);
    m_pUITouchGroup = TouchGroup::create();
    if(!m_pUITouchGroup)
        return;
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = HASEEPAY_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    UILayout *pPanel0 =  static_cast<UILayout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton *pButton_closed = static_cast<UIButton *>(pPanel0->getChildByName("Button_closed"));
    if (pButton_closed)
    {
        pButton_closed->addTouchEventListener(this, toucheventselector(PopUpUI::menuHaseePayPageClosed));
    }
    UILayout *pPanel1 =  static_cast<UILayout *>(m_pUITouchGroup->getWidgetByName("Panel1"));
    for (int i = 0; i < kHaseePayPageNum; i++)
    {
        CCString *pStr = CCString::createWithFormat("Button_%d", i);
        UIButton *pButton = static_cast<UIButton *>(pPanel1->getChildByName(pStr->getCString()));
        if (pButton)
        {
            pButton->setTag(i);
            pButton->setTouchEnabled(true);
            pButton->addTouchEventListener(this, toucheventselector(PopUpUI::menuHaseePayPageType));
        }
    }
    UILayout *pPanel2 =  static_cast<UILayout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    UIButton *pButton_charge = static_cast<UIButton *>(pPanel2->getChildByName("Button0"));
    if (pButton_charge)
    {
        pButton_charge->addTouchEventListener(this, toucheventselector(PopUpUI::menuHaseePayPageCharged));
    }
    UILabel *pLabel_number = static_cast<UILabel *>(pPanel2->getChildByName("Label0"));
    UIImageView *pImage_number_bg = static_cast<UIImageView *>(pLabel_number->getChildByName("ImageView0"));
	if(pImage_number_bg)
	{
        pImage_number_bg->setTouchEnabled(false);
		CCSize size = CCSizeMake(pImage_number_bg->getSize().width, pImage_number_bg->getSize().height);
		m_pCardNumber = createEditBox(this, pImage_number_bg, size, CARD_NUMBER_LENGTH, g_sPleaseInputCardNumber);
        m_pCardNumber->setFontColor(ccWHITE);
        
//        CCPoint obPosition = pLabel_number->convertToWorldSpace(pImage_number_bg->getPosition());
//        CCEditBox*editBox = CCEditBox::create(size,CCScale9Sprite::create(PIC_INPUT_BACKGROUND ));
//        editBox->setPosition(obPosition);
//        editBox->setFont("Helvetica-Bold",25);
//        editBox->setFontColor(ccRED);
//        editBox->setPlaceHolder("XXXXXXXXXX");
//        editBox->setPlaceholderFontColor(ccWHITE);
//        editBox->setMaxLength(CARD_NUMBER_LENGTH);
//        editBox->setReturnType(kKeyboardReturnTypeDone);
//        editBox->setDelegate(this);
//        editBox->setOpacity(0); // 设置透明度实际是后面为输入框 因为此背景并不从最左边出来 不能设置为输入框
//        editBox->setInputMode(kEditBoxInputModeSingleLine);
//        editBox->setTouchPriority(0);
//        m_pUITouchGroup->addChild(editBox,100);
	}
    UILabel *pLabel_password = static_cast<UILabel *>(pPanel2->getChildByName("Label1"));
    UIImageView *pImage_password_bg = static_cast<UIImageView *>(pLabel_password->getChildByName("ImageView0"));
	if(pImage_password_bg)
	{
        pImage_password_bg->setTouchEnabled(false);
        CCSize size = CCSizeMake(pImage_password_bg->getSize().width, pImage_password_bg->getSize().height);
		m_pCardPassword = createEditBox(this, pImage_password_bg, size, CARD_PASSWORD_LENGTH, g_sPleaseInputCardPassword);
        m_pCardPassword->setFontColor(ccWHITE);
        m_pCardPassword->setTouchPriority(0);
		m_pCardPassword->setInputFlag(kEditBoxInputFlagPassword);
	}
    refreshHaseePayPage(kHaseePayMobile);
}
void PopUpUI::refreshHaseePayPage(HaseePayPageType kType)
{
    setChargeID(-1);
    UILayout *pPanel1 =  static_cast<UILayout *>(m_pUITouchGroup->getWidgetByName("Panel1"));
    UILayout *pPanel2 =  static_cast<UILayout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    for (int i = 0; i < kHaseePayPageNum; i++)
    {
        CCString *pStr = CCString::createWithFormat("Button_%d", i);
        UIButton *pButton = static_cast<UIButton *>(pPanel1->getChildByName(pStr->getCString()));
        std::string filePath;
        if (kType == i)
        {
            filePath = getFilePath("222",kDirTypeUI,kFileSuffixPng);
        }
        else
        {
            filePath = getFilePath("223",kDirTypeUI,kFileSuffixPng);
        }
        pButton->loadTextureNormal(filePath.c_str());
    }
    for (int i = 0; i < kHaseePayPageNum; i++)
    {
        CCString *pStr = CCString::createWithFormat("Panel2_%d", i);
        UILayout *pPanel =  static_cast<UILayout *>(pPanel2->getChildByName(pStr->getCString()));
        if (kType == i)
        {
            pPanel->setEnabled(true);
            pPanel->setVisible(true);
            pPanel->setTouchEnabled(true);
        }
        else
        {
            pPanel->setEnabled(false);
            pPanel->setVisible(false);
            pPanel->setTouchEnabled(false);
        }
    }
    CCString *pStr = CCString::createWithFormat("Panel2_%d", kType);
    UILayout *pPanel =  static_cast<UILayout *>(pPanel2->getChildByName(pStr->getCString()));
    if (kType == kHaseePayMobile)
    {
        UIScrollView *pScrollView =  static_cast<UIScrollView *>(pPanel->getChildByName("ScrollView_0"));
        for (int i = 0; i < pScrollView->getChildrenCount(); i++)
        {
            CCString *pStr = CCString::createWithFormat("Button%d", i);
            UIButton *pButton = static_cast<UIButton *>(pScrollView->getChildByName(pStr->getCString()));
            if (pButton)
            {
                pButton->setTag(i);
                std::string filePath = getFilePath("273",kDirTypeUI,kFileSuffixPng);
                pButton->loadTextureNormal(filePath.c_str());
                pButton->addTouchEventListener(this, toucheventselector(PopUpUI::menuHaseePaySelectedQuota));
            }
        }
    }
    else if (kType == kHaseePayUnicorn || kType == kHaseePayTelecom)
    {
        int num = kType == kHaseePayUnicorn ? UNICORN_CHARGE_QUOTA_NUM : TELECOM_CHARGE_QUOTA_NUM;
        for (int i = 0; i < num; i++)
        {
            CCString *pStr = CCString::createWithFormat("Button%d", i);
            UIButton *pButton = static_cast<UIButton *>(pPanel->getChildByName(pStr->getCString()));
            if (pButton)
            {
                pButton->setTag(i);
                std::string filePath = getFilePath("273",kDirTypeUI,kFileSuffixPng);
                pButton->loadTextureNormal(filePath.c_str());
                pButton->addTouchEventListener(this, toucheventselector(PopUpUI::menuHaseePaySelectedQuota));
            }
        }
    }
}
void PopUpUI::refreshSelectedQuota(HaseePayPageType kType, int nIndex)
{
    UILayout *pPanel2 =  static_cast<UILayout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CCString *pStr = CCString::createWithFormat("Panel2_%d", kType);
    UILayout *pPanel =  static_cast<UILayout *>(pPanel2->getChildByName(pStr->getCString()));
    if (kType == kHaseePayMobile)
    {
        UIScrollView *pScrollView =  static_cast<UIScrollView *>(pPanel->getChildByName("ScrollView_0"));
        for (int i = 0; i < pScrollView->getChildrenCount(); i++)
        {
            if (i != nIndex)
            {
                CCString *pStr = CCString::createWithFormat("Button%d", i);
                UIButton *pButton = static_cast<UIButton *>(pScrollView->getChildByName(pStr->getCString()));
                std::string filePath = getFilePath("273",kDirTypeUI,kFileSuffixPng);
                pButton->loadTextureNormal(filePath.c_str());
            }
        }
    }
    else if (kType == kHaseePayUnicorn || kType == kHaseePayTelecom)
    {
        int num = kType == kHaseePayUnicorn ? UNICORN_CHARGE_QUOTA_NUM : TELECOM_CHARGE_QUOTA_NUM;
        for (int i = 0; i < num; i++)
        {
            if (i != nIndex)
            {
                CCString *pStr = CCString::createWithFormat("Button%d", i);
                UIButton *pButton = static_cast<UIButton *>(pPanel->getChildByName(pStr->getCString()));
                std::string filePath = getFilePath("273",kDirTypeUI,kFileSuffixPng);
                pButton->loadTextureNormal(filePath.c_str());
            }
        }
    }
}
void PopUpUI::menuHaseePayPageClosed(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        if (m_pUITouchGroup)
        {
            removeChild(m_pUITouchGroup);
            m_pUITouchGroup = NULL;
            m_pUITouchGroup = m_pHaseePayTouchGroup;
            m_pUITouchGroup->setTouchEnabled(true);
            setChargePageType(kChargeAlipay);
            setHaseePayPageType(kHaseePayMobile);
        }
    }
}
void PopUpUI::menuHaseePayPageType(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        UIButton *pButton = (UIButton *)pSender;
        HaseePayPageType kType = (HaseePayPageType)pButton->getTag();
        setHaseePayPageType(kType);
        refreshHaseePayPage(kType);
    }
}
void PopUpUI::menuHaseePaySelectedQuota(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        UIButton *pButton = (UIButton *)pSender;
        std::string filePath = getFilePath("272",kDirTypeUI,kFileSuffixPng);
        pButton->loadTextureNormal(filePath.c_str());
        if (getHaseePayPageType() == kHaseePayMobile)
            setSelectedQuota(mobile_quota[pButton->getTag()]);
        else if (getHaseePayPageType() == kHaseePayUnicorn)
            setSelectedQuota(unicorn_quota[pButton->getTag()]);
        else if (getHaseePayPageType() == kHaseePayTelecom)
            setSelectedQuota(telecom_quota[pButton->getTag()]);
        setYoYoPayQuota(getSelectedQuota());
        setChargeID(pButton->getTag());
        refreshSelectedQuota(getHaseePayPageType(), pButton->getTag());
    }
}
void PopUpUI::menuHaseePayPageCharged(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        std::string sAccount = m_pCardNumber->getText();
        std::string sPassword = m_pCardPassword->getText();
        if (sAccount.length() < CARD_NUMBER_LENGTH || sPassword.length() < CARD_PASSWORD_LENGTH)
        {
            ((YYScene*)getParent())->showTips(g_sChargeCardInfoTip, kTipHaseePayNomal);
            return;
        }
        if (!isDigtal(sAccount) || !isDigtal(sPassword))
        {
            ((YYScene*)getParent())->showTips(g_sDigtalTipForChargeCardInfo, kTipHaseePayNomal);
            return;
        }
        if (getSelectedQuota() == 0 || getChargeID() == -1)
        {
            ((YYScene*)getParent())->showTips(g_sPleaseSelectedChargeQuota, kTipHaseePayNomal);
            return;
        }
        setCardNumber(sAccount);
        setCardPassword(sPassword);
//        CCLog("%s", getCardNumber().c_str());
//        CCLog("%s", getCardPassword().c_str());
//        CCLog("%d", getHaseePayPageType());
//        CCLog("%d", getSelectedQuota());
//        CCLog("%s", getSerialNumber().c_str());
//        CCLog("%d", getYoYoPayQuota());
        CCString *pStrTip = CCString::createWithFormat(g_sHaseePayChargeTip, getSelectedQuota());
        ((YYScene*)getParent())->showTips(pStrTip->getCString(), kTipGoChargeForHaseePayUI);
    }
}
