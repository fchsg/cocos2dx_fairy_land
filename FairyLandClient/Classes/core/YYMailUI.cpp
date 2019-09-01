//
//  YYMailUI.cpp
//  Fairyland
//
//  Created by yoyo on 14-6-4.
//
//

#include "YYPopUpUI.h"

void PopUpUI::initMailUI(bool bInBoxOrGM)
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIMail);
    m_bInitMailInfo = false;
    m_uMailNumBackup = 0;
    m_nMailIndex = -1;
    
    m_pUITouchGroup = TouchGroup::create();
    if(!m_pUITouchGroup)
        return;
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = MAIL_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout *pPanel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton *pBtnClose = static_cast<UIButton  *>(pPanel0->getChildByName("Button_closed"));
    if(pBtnClose != NULL)
    {
        pBtnClose->addTouchEventListener(this, toucheventselector(PopUpUI::menuMailUIClosed));
    }
    
    UIButton *pBtnInBox = static_cast<UIButton  *>(pPanel0->getChildByName("Button0"));  //收件箱
    if(pBtnInBox != NULL)
    {
        pBtnInBox->addTouchEventListener(this, toucheventselector(PopUpUI::btnMailInBoxCallBack));
    }
    
    UIButton *pBtnSendGM = static_cast<UIButton  *>(pPanel0->getChildByName("Button1")); //GM
    if(pBtnSendGM != NULL)
    {
        pBtnSendGM->addTouchEventListener(this, toucheventselector(PopUpUI::btnMailSendGMCallBack));
    }
    
    Layout *pPanel2 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    UIButton *pBtnRemove = static_cast<UIButton *>(pPanel2->getChildByName("Button_0"));
    if(pBtnRemove != NULL)
    {
        pBtnRemove->addTouchEventListener(this, toucheventselector(PopUpUI::menuRemoveMail));
    }
    UIButton *pBtnGetAccessory = static_cast<UIButton *>(pPanel2->getChildByName("Button_1"));
    if(pBtnGetAccessory != NULL)
    {
        pBtnGetAccessory->addTouchEventListener(this, toucheventselector(PopUpUI::menuGetAccessory));
    }
    
    // 隐藏3个附件icon图标
    UIImageView* imageView3  = static_cast<UIImageView *>(pPanel2->getChildByName("ImageView3"));
    if (imageView3)
    {
        for (int i = 0; i < 3; i++)
        {
            UIImageView* pImg = static_cast<UIImageView *>(imageView3->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
            pImg->setVisible(false);
        }
    }
    
    Layout *pPanel1 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel1"));
    UIScrollView *pScrollView = static_cast<UIScrollView *>(pPanel1->getChildByName("ScrollView_0"));
    if (pScrollView)
    {
        pScrollView->setBounceEnabled(true); // 开启回弹
    }
    UIImageView *pImageView = static_cast<UIImageView *>(pScrollView->getChildByName("ImageView_0"));
    pImageView->setEnabled(false);
    pImageView->setVisible(false);

    Layout *pPanel3 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel3"));

    initMailGMUI(); //初始化联系GM页面
    if (bInBoxOrGM)   //收件箱
    {
        if (pPanel1)
        {
            pPanel1->setEnabled(true);
            pPanel1->setVisible(true);
        }
        if (pPanel2)
        {
            pPanel2->setEnabled(true);
            pPanel2->setVisible(true);
        }
        if (pPanel3)
        {
            pPanel3->setEnabled(false);
            pPanel3->setVisible(false);
        }
        if (!m_bInitMailInfo)
        {
            Controller::getInstance()->sendMessage(ASK_MAIL_LIST, true);
        }
    }
    else //联系客服
    {
        if (pPanel1)
        {
            pPanel1->setEnabled(false);
            pPanel1->setVisible(false);
        }
        if (pPanel2)
        {
            pPanel2->setEnabled(false);
            pPanel2->setVisible(false);
        }
        if (pPanel3)
        {
            pPanel3->setEnabled(true);
            pPanel3->setVisible(true);
        }
    }
}

void PopUpUI::initMailGMUI()
{
    Layout *pPanel3 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel3"));
    if (!pPanel3)
    {
        return;
    }
    
    //标题
    UILabel* label_1 = static_cast<UILabel*>(pPanel3->getChildByName("Label_1"));
    if (label_1)
    {
        UILabel* label_0 = static_cast<UILabel*>(label_1->getChildByName("Label_0"));
        if(label_0 != NULL)
        {
            m_MailTitleTextField= CCTextFieldTTF::textFieldWithPlaceHolder(g_sMailGMTitle,CCSizeMake(680.0f, 45.0f),kCCTextAlignmentLeft,"Helvetica-Bold",25);
            m_MailTitleTextField->setAnchorPoint(ccp(0.0f,0.5f));
            m_MailTitleTextField->setDelegate(this);
            label_0->addNode(m_MailTitleTextField);
            label_0->setTouchEnabled(true);
            label_0->addTouchEventListener(this, toucheventselector(PopUpUI::btnMailClickTitleCallBack));
        }
    }
    
    //内容
    UILabel* label_2 = static_cast<UILabel*>(pPanel3->getChildByName("Label_2"));
    if (label_2)
    {
        UILabel* label_0 = static_cast<UILabel*>(label_2->getChildByName("Label_0"));
        if(label_0 != NULL)
        {
            m_MailContentTextField= CCTextFieldTTF::textFieldWithPlaceHolder(g_sMailGMContent,CCSizeMake(680.0f, 240.0f),kCCTextAlignmentLeft,"Helvetica-Bold",25);
            m_MailContentTextField->setAnchorPoint(ccp(0.0f,1.0f));
            m_MailContentTextField->setDelegate(this);
            label_0->addNode(m_MailContentTextField);
            label_0->setTouchEnabled(true);
            label_0->addTouchEventListener(this, toucheventselector(PopUpUI::btnMailClickContentCallBack));
        }
    }
    
    //发送
    UIButton *pBtnSend = static_cast<UIButton  *>(pPanel3->getChildByName("Button_0_1"));
    if(pBtnSend != NULL)
    {
        pBtnSend->addTouchEventListener(this, toucheventselector(PopUpUI::btnMailClickSendCallBack));
    }
    
}
void PopUpUI::btnMailClickContentCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        m_MailContentTextField->attachWithIME();
    }
}
void PopUpUI::btnMailClickTitleCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        m_MailTitleTextField->attachWithIME();
    }
}
void PopUpUI::btnMailClickSendCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        const char* strTitlie = m_MailTitleTextField->getString();
        const char* strContent = m_MailContentTextField->getString();
        CCString* msg = CCString::createWithFormat(SEND_MAIL_GM,strTitlie,strContent);
        Controller::getInstance()->sendMessage(msg->getCString(), true);
        m_MailTitleTextField->setString("");
        m_MailContentTextField->setString("");
    }
}
void PopUpUI::updateMailUI()
{
    m_nMailIndex = -1;
    Layout *pPanel1 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel1"));
    UIScrollView *pScrollView = static_cast<UIScrollView *>(pPanel1->getChildByName("ScrollView_0"));
    UIImageView *pImageView = static_cast<UIImageView *>(pScrollView->getChildByName("ImageView_0"));
    pImageView->setSize(CCSizeMake(350, 100));
    
    m_pUIScrollView = pScrollView;
    Layout *panel = m_pUIScrollView->getInnerContainer();
    pScrollView->setContentSize(CCSizeMake(pImageView->getSize().width, m_pMailListArray->count() * pImageView->getSize().height));
    pScrollView->setInnerContainerSize(CCSizeMake(pImageView->getSize().width, m_pMailListArray->count() * pImageView->getSize().height));
    pImageView->setEnabled(false);
    pImageView->setVisible(false);
    for (int i = 0; i < m_uMailNumBackup; i++)
    {
        UIImageView *pImageView = static_cast<UIImageView *>(pScrollView->getChildByTag(10 + i));
        if (pImageView)
            pScrollView->removeChild(pImageView);
    }
    for (int i = 0; i < m_pMailListArray->count(); i++)
    {
        YYMailData *pMailData = (YYMailData *)m_pMailListArray->objectAtIndex(i);
        UIImageView *pImageViewClone = (UIImageView *)pImageView->clone();
        pImageViewClone->setPosition(ccp(pImageView->getPosition().x, panel->getSize().height - i * pImageView->getSize().height - pImageView->getSize().height / 2));
        if (!pImageViewClone)
            continue;
        pImageViewClone->setEnabled(true);
        pImageViewClone->setVisible(true);
        pImageViewClone->setTag(10 + i);
        pImageViewClone->addTouchEventListener(this, toucheventselector(PopUpUI::menuAccessMail));
        panel->addChild(pImageViewClone);
        
        UILabel *pLabel_postName = static_cast<UILabel *>(pImageViewClone->getChildByName("Label0"));
        pLabel_postName->setText(pMailData->getPostName());
        
        UILabel *pLabel_title = static_cast<UILabel *>(pImageViewClone->getChildByName("Label1"));
        pLabel_title->setText(pMailData->getTitle());
    }
    m_uMailNumBackup = m_pMailListArray->count();
}
void PopUpUI::updateMailDetailInfo(int flag)
{
    Layout *pPanel2 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
    UIImageView *pImageView0 = static_cast<UIImageView *>(pPanel2->getChildByName("ImageView0"));
    UILabel *pLabel_postName = static_cast<UILabel *>(pImageView0->getChildByName("Label_1"));
    pLabel_postName->setTextHorizontalAlignment(kCCTextAlignmentCenter);
    pLabel_postName->setTextVerticalAlignment(kCCVerticalTextAlignmentCenter);
    if (flag == 0)
        pLabel_postName->setText(m_pMailDetalData->getPostName());
    else
        pLabel_postName->setText(" ");
    
    UIImageView *pImageView1 = static_cast<UIImageView *>(pPanel2->getChildByName("ImageView1"));
    UILabel *pLabel_title = static_cast<UILabel *>(pImageView1->getChildByName("Label_1"));
    pLabel_title->setTextHorizontalAlignment(kCCTextAlignmentCenter);
    pLabel_title->setTextVerticalAlignment(kCCVerticalTextAlignmentCenter);
    if (flag == 0)
        pLabel_title->setText(m_pMailDetalData->getTitle());
    else
        pLabel_title->setText(" ");
    
    UIImageView *pImageView2 = static_cast<UIImageView *>(pPanel2->getChildByName("ImageView2"));
    UILabel *pLabel_mailContent = static_cast<UILabel *>(pImageView2->getChildByName("Label_1"));
    pLabel_mailContent->setTextHorizontalAlignment(kCCTextAlignmentCenter);
    pLabel_mailContent->setTextVerticalAlignment(kCCVerticalTextAlignmentCenter);
    if (flag == 0)
        pLabel_mailContent->setText(m_pMailDetalData->getContent());
    else
        pLabel_mailContent->setText(" ");
    
    if (m_pMailDetalData)
    {
        CCArray *pArrayItem = m_pMailDetalData->getArrayAccessory();
        
        // 如果没有附件 隐藏附件获取按钮
        UIButton* pBtn2 = static_cast<UIButton *>(pPanel2->getChildByName("Button_1"));
        if (pArrayItem->count() == 0)
        {
            pBtn2->setVisible(false);
            pBtn2->setTouchEnabled(false);
        }
        else
        {
            pBtn2->setVisible(true);
            pBtn2->setTouchEnabled(true);
        }
        
        UIImageView *pImageView3 = static_cast<UIImageView *>(pPanel2->getChildByName("ImageView3"));
        for (int i = 0; i < 3; i++)
        {
            CCString *pStr = CCString::createWithFormat("ImageView_%d", i);
            UIImageView *pImageView = static_cast<UIImageView *>(pImageView3->getChildByName(pStr->getCString()));
            pImageView->removeAllChildren();
            pImageView->setVisible(false);
            if (flag == 0)
            {
                if (pArrayItem->count() > i)
                {
                    YYItemData *pItemData = (YYItemData *)pArrayItem->objectAtIndex(i);
                    int nType = pItemData->getType();
                    UIImageView *pAccessoryImage = UIImageView::create();
                    std::string str;
                    CCString *pStrNum;
                    if(nType == 1)  //服务器下发物品
                    {
                       str = getFilePath(pItemData->getIcon(), kDirTypeIcon, kFileSuffixPng);
                       pStrNum = CCString::createWithFormat("x%s" ,pItemData->getNum().c_str());
                    }
                    else            //金钱 宝石 经验
                    {
                       str = pItemData->getIcon();
                       pStrNum = CCString::createWithFormat("%s" ,pItemData->getNum().c_str());
                    }
                    pAccessoryImage->loadTexture(str.c_str());
                    pImageView->addChild(pAccessoryImage);
                    UILabel *pLabelGetTip = createLabel(CCSizeMake(80, 30), kCCTextAlignmentCenter, 20, ccWHITE);
                    pLabelGetTip->setPosition(ccp(pImageView->getSize().width / 2 - 15, -pImageView->getSize().height / 2));
                    pImageView->addChild(pLabelGetTip);
                    pLabelGetTip->setText(pStrNum->getCString());
                    pImageView->setVisible(true);
                }
            }
        }
    }
}
void PopUpUI::menuMailUIClosed(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        m_bInitMailInfo = false;
        m_uMailNumBackup = 0;
        m_nMailIndex = -1;
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        if (m_pMailListArray)
        {
            m_pMailListArray->removeAllObjects();
            CC_SAFE_DELETE(m_pMailListArray);
        }
        CC_SAFE_RELEASE_NULL(m_pMailDetalData);
        Controller::getInstance()->switchGameStatus(kStatusScene);
        setUIType(kUIDefaultStatus);
    }
}
void PopUpUI::btnMailInBoxCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        for (int i = 1; i < 4; i++)
        {
            CCString* pStr = CCString::createWithFormat("Panel%d",i);
            Layout *pPanel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName(pStr->getCString()));
            if (pPanel)
            {
                switch (i)
                {
                    case 1:   //收件箱
                    case 2:
                        pPanel->setEnabled(true);
                        pPanel->setVisible(true);
                        break;
                    case 3:   //联系GM
                        pPanel->setEnabled(false);
                        pPanel->setVisible(false);
                        break;
                    default:
                        break;
                }
           
            }
        }
        if (!m_bInitMailInfo)
        {
            Controller::getInstance()->sendMessage(ASK_MAIL_LIST, true);
        }
    }
}
void PopUpUI::btnMailSendGMCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        for (int i = 1; i < 4; i++)
        {
            CCString* pStr = CCString::createWithFormat("Panel%d",i);
            Layout *pPanel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName(pStr->getCString()));
            if (pPanel)
            {
                switch (i)
                {
                    case 1:  //收件箱
                    case 2:
                        pPanel->setEnabled(false);
                        pPanel->setVisible(false);
                        break;
                    case 3:  //联系GM
                        pPanel->setEnabled(true);
                        pPanel->setVisible(true);
                        break;
                    default:
                        break;
                }
                
            }
        }
    }
}
void PopUpUI::menuRemoveMail(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        if (m_nMailIndex == -1)
        {
            ((YYScene*)getParent())->showTips(g_sSelectedMail);
            return;
        }
        std::string strMsg = (CCString::createWithFormat(SEND_REMOVE_MAIL, m_pMailDetalData->getIndex()))->getCString();
        Controller::getInstance()->sendMessage(strMsg.c_str(), true);
    }
}
void PopUpUI::menuGetAccessory(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        } if (m_nMailIndex == -1)
        {
            ((YYScene*)getParent())->showTips(g_sSelectedMail);
            return;
        }
        std::string strMsg = (CCString::createWithFormat(SEND_GET_ACCESSORY, m_pMailDetalData->getIndex()))->getCString();
        Controller::getInstance()->sendMessage(strMsg.c_str(), true);
    }
}
void PopUpUI::menuAccessMail(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        int nIndex = pImageView->getTag() - 10;
        if (m_nMailIndex == nIndex)
            return;
        
        m_nMailIndex = nIndex;
        Layout *panel = m_pUIScrollView->getInnerContainer();
        for (int i = 0; i < m_pMailListArray->count(); i++)
        {
            UIImageView *pImageViewChild = (UIImageView *)panel->getChildByTag(i + 10);
            UIImageView *pImageSelectedMark = static_cast<ImageView *>(pImageViewChild ->getChildByName("ImageView_0"));
            if (nIndex == i)
            {
                pImageSelectedMark->setVisible(true);
            }
            else
            {
                pImageSelectedMark->setVisible(false);
            }
        }
        std::string strMsg = (CCString::createWithFormat(SEND_READ_MAIL, nIndex))->getCString();
        Controller::getInstance()->sendMessage(strMsg.c_str(), true);
    }
}
void PopUpUI::setMailListData(cocos2d::CCArray *pArray)
{// [邮件数量]_[发件人];[标题];[读取状态];[发送时间]]_[发件人];[标题];[读取状态];[发送时间]]...
    if (!pArray)
        return;

    CCString *pStrNum = (CCString *)pArray->objectAtIndex(0);
    int MailNum = pStrNum->intValue();
    if (MailNum == 0)
        return;
    
    if (!m_pMailListArray)
        m_pMailListArray = new CCArray();
    m_pMailListArray->removeAllObjects();
    
    for (int i = 0; i < pArray->count() - 1; i++)
    {
        YYMailData *pMailData = YYMailData::create();
        CCString *pStr = (CCString *)pArray->objectAtIndex(i + 1);
        if (pStr && pStr->length() > 0)
        {
            CCArray *pArrayMail = split_string(pStr->getCString(), ';');
            if (pArrayMail)
            {
                pMailData->setIndex(i);
                pMailData->setPostName(((CCString *)pArrayMail->objectAtIndex(0))->getCString());
                pMailData->setTitle(((CCString *)pArrayMail->objectAtIndex(1))->getCString());
                pMailData->setReadState(((CCString *)pArrayMail->objectAtIndex(2))->intValue());
                pMailData->setPostTime(((CCString *)pArrayMail->objectAtIndex(3))->getCString());
            }
        }
        m_pMailListArray->addObject(pMailData);
    }
    m_bInitMailInfo = true;
    updateMailUI();
    updateMailDetailInfo(1);
}

void PopUpUI::setMailDetailData(cocos2d::CCArray *pArray)
{
 //[邮件index]_[发送人ID]_[发送人姓名]_[邮件标题]_[邮件内容]_[附件物品]_[附件金钱]_[附件宝石]_[附件经验]_[是否读取]_[发送时间]
    if (!pArray)
        return;
    
    if (!m_pMailDetalData)
    {
        m_pMailDetalData = YYMailDetailData::create();
        m_pMailDetalData->retain();
    }
    m_pMailDetalData->getArrayAccessory()->removeAllObjects();
    
    CCString *pStrIndex = (CCString *)pArray->objectAtIndex(0);  //邮件Idx
    if (pStrIndex)
        m_pMailDetalData->setIndex(pStrIndex->intValue());
    
    CCString *pPostIndex = (CCString *)pArray->objectAtIndex(1); //发送人ID
    if (pPostIndex)
        m_pMailDetalData->setPostInx(pPostIndex->getCString());
    
    CCString *pStrPostName = (CCString *)pArray->objectAtIndex(2); //发送人姓名
    if (pStrPostName)
        m_pMailDetalData->setPostName(pStrPostName->getCString());
    
    CCString *pStrTitle = (CCString *)pArray->objectAtIndex(3); //邮件标题
    if (pStrTitle)
        m_pMailDetalData->setTitle(pStrTitle->getCString());
    
    CCString *pStrContent = (CCString *)pArray->objectAtIndex(4); //邮件内容
    if (pStrContent)
        m_pMailDetalData->setContent(pStrContent->getCString());
    
    CCString *pStrAccessory = (CCString *)pArray->objectAtIndex(5); //附件物品
    if (pStrAccessory && pStrAccessory->compare("0"))
    {
        CCArray* pSeg = split_string(pStrAccessory->getCString(), ';');
        if (pSeg)
        {
            for (int i; i < pSeg->count(); i++)
            {
                YYItemData *pItemData = YYItemData::create();
                CCString *pStrItem = (CCString*)(pSeg->objectAtIndex(i));
                CCArray *pArrayItem = split_string(pStrItem->getCString(), ',');
                CCString *pStrNum = (CCString *)pArrayItem->objectAtIndex(1);
                if (pStrNum)
                    pItemData->setNum(pStrNum->getCString());
                CCString *pStrIcon = (CCString *)pArrayItem->objectAtIndex(2);
                if (pStrIcon)
                    pItemData->setIcon(pStrIcon->getCString());
                
                m_pMailDetalData->getArrayAccessory()->addObject(pItemData);
            }
        }
    }
    
    CCString *pStrGlod = (CCString *)pArray->objectAtIndex(6); //附件金钱
    if(pStrGlod && pStrGlod->compare("0"))
    {
        YYItemData *pItemData = YYItemData::create();
        pItemData->setNum(pStrGlod->getCString());
        std::string filePath = ICON_GLOD_PATH;
        pItemData->setIcon(filePath.c_str());
        pItemData->setType(0);
        m_pMailDetalData->getArrayAccessory()->addObject(pItemData);
    }
    CCString *pStrJew = (CCString *)pArray->objectAtIndex(7); //附件宝石
    if(pStrJew && pStrJew->compare("0"))
    {
        YYItemData *pItemData = YYItemData::create();
        pItemData->setNum(pStrJew->getCString());
        std::string filePath = ICON_JEW_PATH;
        pItemData->setIcon(filePath.c_str());
        pItemData->setType(0);
        m_pMailDetalData->getArrayAccessory()->addObject(pItemData);
    }
    CCString *pStrExperience = (CCString *)pArray->objectAtIndex(8); //附件经验
    if(pStrExperience && pStrExperience->compare("0"))
    {
        YYItemData *pItemData = YYItemData::create();
        pItemData->setNum(pStrExperience->getCString());
        std::string filePath = ICON_EXP_PATH;
        pItemData->setIcon(filePath.c_str());
        pItemData->setType(0);
        m_pMailDetalData->getArrayAccessory()->addObject(pItemData);
    }

    CCString *pStrState = (CCString *)pArray->objectAtIndex(9); //是否读取
    if (pStrState)
        m_pMailDetalData->setReadState(pStrState->intValue());
    
    CCString *pStrPostTime = (CCString *)pArray->objectAtIndex(10);//发送时间
    if (pStrPostTime)
        m_pMailDetalData->setPostTime(pStrPostTime->getCString());
    
    updateMailDetailInfo(0);
}
uint16_t PopUpUI::getMailNum()
{
    return m_uMailNum;
}
void PopUpUI::setMailNum(uint16_t nMailNum)
{
    m_uMailNum = nMailNum;
}
CCNode* PopUpUI::getMailTipNode(uint16_t nMailNum)
{
    std::string filePath = getFilePath("89", kDirTypeUI, kFileSuffixPng);
    CCSprite *pSpriteCountBg = CCSprite::create(filePath.c_str());
    const char* countStr = CCString::createWithFormat("%d", nMailNum)->getCString();
    CCLabelTTF *pNumberLabel = CCLabelTTF::create(countStr, DEFAULT_FONT_NAME, 12);
    pNumberLabel->setPosition(ccp(pSpriteCountBg->getContentSize().width - 14, pSpriteCountBg->getContentSize().height - 12));
    pSpriteCountBg->addChild(pNumberLabel, 1);
    return pSpriteCountBg;
}

//当用户启动虚拟键盘时的回调函数
bool PopUpUI::onTextFieldAttachWithIME(CCTextFieldTTF *pSender)
{
   // CCLOG("启动输入");
    return false;
    //return true:不启动
}
//当用户关闭虚拟键盘时的回调函数
bool PopUpUI::onTextFieldDetachWithIME(CCTextFieldTTF *pSender)
{
    //CCLOG("关闭输入");
    return false;
    //return true:不关闭
}
//当用户进行输入时的回调函数
bool PopUpUI::onTextFieldInsertText(CCTextFieldTTF *pSender,const char *text,int nLen)
{
   // CCLOG("输入字符...");
    return false;
    //return true:不会输入进字符
    
}
//当用户删除文字时的回调函数
bool PopUpUI::onTextFieldDeleteBackward(CCTextFieldTTF *pSender,const char *delText,int nLen)
{
   // CCLOG("删除字符");
    return false;
    //return true:不删除
}

