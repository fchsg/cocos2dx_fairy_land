//
//  YYAchievement.cpp
//  Fairyland
//
//  Created by fuchenhao on 14-6-25.
//
//

#include "YYPopUpUI.h"

#pragma mark 成就初始化
void PopUpUI::initAchievementUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIAchieve);
    Controller::getInstance()->getScene()->setPopUpUIDelayTime(0);
    
    setIsAchieveInit(false);
    setCurrentAchievePage(0);
    m_nCurrentAchieveClassIndex = -1;
    m_nCurrentAchieveGroupIndex= -1;
    m_nCurrentAchieveItemIndex = -1;
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = ACHIEVE_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(closeButton);
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    CC_ASSERT(panel1);
    
    UIScrollView *pScrollView = static_cast<UIScrollView *>(panel1->getChildByName("ScrollView_0"));
    CC_ASSERT(pScrollView);
    m_pUIScrollView = pScrollView;
    m_pUIScrollView->setBounceEnabled(true);
    
    UIButton *pButton0 = static_cast<UIButton *>(pScrollView->getChildByName("Button_0"));
    CC_ASSERT(pButton0);
    m_pBtnAchievementClass = pButton0;
    
    UIImageView* pImgView = static_cast<UIImageView *>(m_pBtnAchievementClass->getChildByName("ImageView_0")); //按钮选中框
    if(pImgView)
    {
        pImgView->setVisible(false);
    }
    pButton0->setEnabled(false);
    pButton0->setVisible(false);
    
    UIButton *pButton1 = static_cast<UIButton *>(pScrollView->getChildByName("Button_0_0"));
    if(pButton1)
    {
        pButton1->setEnabled(false);
        pButton1->setVisible(false);
    }
    
    // achieve section
    Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    CC_ASSERT(panel2);
    Layout *panel2_0 = static_cast<Layout*>(panel2->getChildByName("Panel2_0"));
    CC_ASSERT(panel2_0);
    
    //pageView
    UIPageView* pageView = static_cast<UIPageView*>(panel2->getChildByName("PageView_0"));
    pageView->setTouchEnabled(true);
    pageView->addEventListenerPageView(this, pagevieweventselector(PopUpUI::achievementPageViewEvent));
    m_pUIPageView = pageView;
    
    Layout *panel2_1 = static_cast<Layout*>(panel2->getChildByName("Panel2_1"));
    panel2_1->removeFromParent();
    
    for (int i = 0; i < ACHIEVE_PAGE_NUM; ++i)
    {
        UILayout* layout = static_cast<UILayout*>(panel2_1->clone());
        //隐藏右侧imageView icon 背景
        for (int j = 0; j < 3; j++)
        {
            CCString *pStr = CCString::createWithFormat("ImageView1_%i", j);
            UIImageView *pImageView = static_cast<UIImageView *>(layout->getChildByName(pStr->getCString()));
            if (!pImageView)
                continue;
            UIImageView *pImageView_award_bg = static_cast<UIImageView *>(pImageView->getChildByName("ImageView0"));
            if (!pImageView_award_bg)
                continue;
            pImageView_award_bg->setVisible(false);
        }
        pageView->addPage(layout);
    }
    
    UIButton *pBtnGroupAward = static_cast<UIButton *>(panel2->getChildByName("Button_done"));
    CC_ASSERT(pBtnGroupAward);
    pBtnGroupAward->addTouchEventListener(this, toucheventselector(PopUpUI::achievementGroupAwardCallback));
    
    Controller::getInstance()->sendMessage(ASK_ACHIEVEMENT_LIST, true);
}

#pragma mark 获取服务器成就列表信息
void PopUpUI::receiveAchievementDataInfo(cocos2d::CCArray *array)
{
    if (!array)
        return;
    
    //分组状态
    //0:包含未完成项 下发分项数据
    //1:分项全部完成且已领取 当前组未领取 不下发分项数据
    //2:分项全部完成且已领取 当前组已领取 不下发分项数据
    
    //分项状态
    //0 未完成
    //1 已完成 未领取
    //2 已领取
    
    //test
    //lkjpoi 123123
//    CCString * test = CCString::createWithFormat("%s","10;1002,0,100204:1,100203:1,100202:1,100201:2,100200:2;1001,0,100104:1,100103:1,100102:1,100101:1,100100:1;1000,0,100004:2,100003:2,100000:2,100001:2,100002:1_11;1100,0,110000:2");
//    array = split_string(test->getCString(), '_');
    //test
    
    //重置成就已完成数据
    {
        CCDictionary* pClassDic = GlobalData::getInstance()->getAchievementAllDataDict();
        CCArray* arrClassKey = pClassDic->allKeys();
        for (int i = 0; i < arrClassKey->count(); i++)
        {
            int nClassKey = ((CCInteger *)arrClassKey->objectAtIndex(i))->getValue();
            AchievementClassData* pClassData = (AchievementClassData *)pClassDic->objectForKey(nClassKey);
            CC_BREAK_IF(!pClassData);
            pClassData->setFinishNum(0);
            CCDictionary* pGroupDic = pClassData->getChildDic();
            CC_BREAK_IF(!pGroupDic);
            CCArray* arrGroupKey = pGroupDic->allKeys();
            CC_BREAK_IF(!arrGroupKey);
            for (int j = 0; j < arrGroupKey->count(); j++)
            {
                int nGroupKey = ((CCInteger *)arrGroupKey->objectAtIndex(j))->getValue();
                AchievementGroupData* pGroupData = (AchievementGroupData *)pGroupDic->objectForKey(nGroupKey);
                CC_BREAK_IF(!pGroupData);
                pGroupData->setFinishNum(0);
            }
        }
    }
    
    //解析服务器下发数据
    for (int i = 0; i < array->count(); i++)
    {
        CCLog("Class %s",((CCString*)(array->objectAtIndex(i)))->getCString());
        CCString* strClassInfo = (CCString*)(array->objectAtIndex(i));
        CCArray* arrClassInfo = split_string(strClassInfo->getCString(), ';');
        int nClassIndex = ((CCString *)arrClassInfo->objectAtIndex(0))->intValue(); //分类Index
        AchievementClassData *pClassData = GlobalData::getInstance()->getAchievementClassData(nClassIndex);
        
        for (int j = 1; j < arrClassInfo->count(); j++)
        {
            CCLog("Group %s",((CCString *)arrClassInfo->objectAtIndex(j))->getCString());
            CCString *strGroupInfo = (CCString *)arrClassInfo->objectAtIndex(j);
            CCArray *arrGroupInfo = split_string(strGroupInfo->getCString(), ',');
            removeLastObject(arrGroupInfo);
            // 分组
            int nGroupIndex = ((CCString *)arrGroupInfo->objectAtIndex(0))->intValue();
            int nGroupStatus = ((CCString *)arrGroupInfo->objectAtIndex(1))->intValue();
            AchievementGroupData *pGroupData = GlobalData::getInstance()->getAchievementGroupData(nClassIndex,nGroupIndex);
            
            if (nGroupStatus == 1 || nGroupStatus == 2) //后续数据不下发 分组内所有项已领取并完成
            {
                pGroupData->setGetStatus(nGroupStatus);
                pGroupData->setFinishNum(pGroupData->getTotalNum());
                
                //组内所有项状态置为已完成并领取状态
                CCDictionary* pGroupChildDic = pGroupData->getChildDic();
                if (pGroupChildDic)
                {
                    CCArray* arrGroupAllKeys = pGroupChildDic->allKeys();
                    for (int m = 0; m < arrGroupAllKeys->count(); m++)
                    {
                        int nkey = ((CCInteger *)arrGroupAllKeys->objectAtIndex(m))->getValue();
                        AchievementItemData * pItemData = (AchievementItemData *)pGroupChildDic->objectForKey(nkey);
                        pItemData->setGetStatus(2);
                    }
                }
            }
            else
            {
                for (int k = 2; k < arrGroupInfo->count(); k++)
                {
                    CCLog("item %s",((CCString *)arrGroupInfo->objectAtIndex(k))->getCString());
                    CCString *strItemInfo = (CCString *)arrGroupInfo->objectAtIndex(k);
                    CCArray *arrItemInfo = split_string(strItemInfo->getCString(), ':');
                    removeLastObject(arrItemInfo);
                    int nItemIndex = ((CCString *)arrItemInfo->objectAtIndex(0))->intValue();
                    int nItemStatus = ((CCString *)arrItemInfo->objectAtIndex(1))->intValue();
                    //单项
                    AchievementItemData *pItemData = GlobalData::getInstance()->getAchievementItemData(nClassIndex, nGroupIndex, nItemIndex);
                    pItemData->setGetStatus(nItemStatus);
                    //分组内成就 单项完成数
                    if (pItemData->getGetStatus() == 2)
                    {
                        pGroupData->setFinishNum(pGroupData->getFinishNum() + 1);
                    }
                }
            }
            //分类内成就 组数已领取数
            if(pGroupData->getGetStatus() == 2)
            {
                pClassData->setFinishNum(pClassData->getFinishNum() + 1);
            }
        }
    }
    
    if (!m_bIsAchieveInit) //首次获取数据初始化
    {
        m_bIsAchieveInit = true;
        updateAchievementClassListUI(); //更新类别信息
        int nClassIdx = 0; //默认显示右侧成就列表页面
        int nGroupIdx = 0;
        getAchievementUpdateIndex(nClassIdx, nGroupIdx);
        updateAchievementItemUI(nClassIdx, nGroupIdx);
        //默认打开第一组列表
        openAchieventGroupList();
    }
    else //后续更新UI数据
    {
        updateAchievementTouchGetAward();
    }
}

#pragma mark 通过位置ID获取字典索引
void PopUpUI::getAchievementUpdateIndex(int &nClassIdx, int &nGroupIdx)
{
    CCDictionary* pClassDic = GlobalData::getInstance()->getAchievementAllDataDict();
    CCArray* arrClassKey = pClassDic->allKeys();
    int nClassKey = ((CCInteger *)arrClassKey->objectAtIndex(nClassIdx))->getValue();
    AchievementClassData* pClassData = (AchievementClassData *)pClassDic->objectForKey(nClassKey);
    nClassIdx = pClassData->getIndex();
    CCDictionary* pGroupDic = pClassData->getChildDic();
    CCArray* arrGroupKey = pGroupDic->allKeys();
    int nGroupKey = ((CCInteger *)arrGroupKey->objectAtIndex(nGroupIdx))->getValue();
    AchievementGroupData* pGroupData = (AchievementGroupData *)pGroupDic->objectForKey(nGroupKey);
    nGroupIdx = pGroupData->getIndex();
}

#pragma mark 首次进入默认打开第一组成就
void PopUpUI::openAchieventGroupList()
{
    UILayout * panel = m_pUIScrollView->getInnerContainer();
    int nTag = 0;
    UIButton * pButtonType = static_cast<UIButton*>(panel->getChildByTag(nTag + 10));
    CCDictionary* pClassDic = GlobalData::getInstance()->getAchievementAllDataDict();
    CCArray* arrClassKey = pClassDic->allKeys();
    int nClassKey = ((CCInteger *)arrClassKey->objectAtIndex(0))->getValue();
    AchievementClassData* pClassData = (AchievementClassData *)pClassDic->objectForKey(nClassKey);
    CCDictionary* pGroupDic = pClassData->getChildDic();
    
    CCArray* arrGroupAllKey = pGroupDic->allKeys();
    
    for (int i = 0; i < pGroupDic->count(); i++)
    {
        int key = ((CCInteger *)arrGroupAllKey->objectAtIndex(i))->getValue();
        AchievementGroupData *pGroupData = (AchievementGroupData *)pGroupDic->objectForKey(key);
        UIButton *pButton = (UIButton *)m_pBtnAchievementClass->clone();
        
        if (!pButton)
            return;
        //默认选中第一项
        if(i == 0)
        {
            //选中框
            UIImageView* pImgView = static_cast<UIImageView *>(pButton->getChildByName("ImageView_0"));
            if(pImgView)
            {
                pImgView->setVisible(true);
            }
            //btn背景
            std::string filePath= getFilePath("386", kDirTypeUI, kFileSuffixPng);
            pButton->loadTextureNormal(filePath.c_str());
        }
        
        pButton->setPosition(ccp(pButtonType->getPosition().x, pButtonType->getPositionY() - pButtonType->getContentSize().height * 0.5f - i * (pButtonType->getContentSize().height - 18) - (pButtonType->getContentSize().height - 18) * 0.5f));
            
        pButton->setScale(0.8f);
        pButton->setEnabled(true);
        pButton->setVisible(true);
        pButton->setTag(pGroupData->getIndex());
        pButton->addTouchEventListener(this, toucheventselector(PopUpUI::achievementGroupSectionCallback));
        panel->addChild(pButton);
            
        UILabel *pLabel_name = static_cast<UILabel *>(pButton->getChildByName("Label0"));
        pLabel_name->setText(pGroupData->getName());
            
        CCString *pStrFinishNum = CCString::createWithFormat("%i", pGroupData->getFinishNum());
        UILabel *pLabel_finish_num = static_cast<UILabel *>(pButton->getChildByName("Label2"));
        pLabel_finish_num->setText(pStrFinishNum->getCString());
            
        CCString *pStrTotalNum = CCString::createWithFormat("%i", pGroupData->getTotalNum());
        UILabel *pLabel_total_num = static_cast<UILabel *>(pButton->getChildByName("Label3"));
        pLabel_total_num->setText(pStrTotalNum->getCString());
    }
    scrollViewIntervalChanged(nTag + 1, pButtonType->getContentSize().height - 18, pGroupDic->count());
    setCurrentAchievePage(0);
}

#pragma mark pageView滑动回调
void PopUpUI::achievementPageViewEvent(CCObject *pSender, PageViewEventType type)
{
    switch (type)
    {
        case PAGEVIEW_EVENT_TURNING:
        {
            if (Controller::getInstance()->getScene()->isNetLoading())
            {
                return;
            }
            do
            {
                UIPageView* pageView = dynamic_cast<UIPageView*>(pSender);
                CC_BREAK_IF(!pageView);
                m_nCurrentAchievePage = pageView->getCurPageIndex();
                
                //圆点标记位切换
                CC_BREAK_IF(!m_pUITouchGroup);
                Layout *panel2 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
                CC_BREAK_IF(!panel2);
                Layout *panel2_0 = static_cast<Layout *>(panel2->getChildByName("Panel2_0"));
                CC_BREAK_IF(!panel2_0);
                for (int i = 0; i < ACHIEVE_PAGE_NUM; i++)
                {
                    UIImageView* pImageView = static_cast<UIImageView *>(panel2_0->getChildByName((CCString::createWithFormat("ImageView0_%i",i))->getCString()));
                     CC_BREAK_IF(!pImageView);
                    std::string filePath;
                    if (m_nCurrentAchievePage == i)
                        filePath= getFilePath("412", kDirTypeUI, kFileSuffixPng);
                    else
                        filePath= getFilePath("411", kDirTypeUI, kFileSuffixPng);
                    pImageView->loadTexture(filePath.c_str());
                }
       
            } while (0);
        }
            break;
        default:
            break;
    }
}

#pragma mark 分类列表初始化
void PopUpUI::updateAchievementClassListUI()
{
    do
    {
        Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
        CC_BREAK_IF(!panel1);
        UIScrollView *pScrollView = static_cast<UIScrollView *>(panel1->getChildByName("ScrollView_0"));
        CC_BREAK_IF(!pScrollView);
        m_pUIScrollView = pScrollView;
        
        CC_BREAK_IF(!m_pBtnAchievementClass);
        
        Layout *panel = m_pUIScrollView->getInnerContainer();
        CCDictionary* allDataDic = GlobalData::getInstance()->getAchievementAllDataDict();
        CC_BREAK_IF(!allDataDic);
        int nClassCount = allDataDic->count(); //左侧列表所含项目数
       
        pScrollView->setInnerContainerSize(CCSizeMake(m_pBtnAchievementClass->getContentSize().width, nClassCount * m_pBtnAchievementClass->getContentSize().height));
    
        CCArray* allKeys = allDataDic->allKeys();
        for (int i = 0; i < nClassCount; i++)
        {
            int key = ((CCInteger *)allKeys->objectAtIndex(i))->getValue();
            if (panel->getChildByTag(key))
                continue;
            UIButton *pButton = (UIButton *)m_pBtnAchievementClass->clone();
            if (!pButton)
                continue;
            //默认选中第一项
            if(i == 0)
            {
                UIImageView* pImgView = static_cast<UIImageView *>(pButton->getChildByName("ImageView_0"));
                if(pImgView)
                {
                    pImgView->setVisible(true);
                }
                //btn背景
                std::string filePath= getFilePath("386", kDirTypeUI, kFileSuffixPng);
                pButton->loadTextureNormal(filePath.c_str());
            }

            pButton->setPosition(ccp(m_pBtnAchievementClass->getPosition().x, pScrollView->getContentSize().height- (i+1) * m_pBtnAchievementClass->getContentSize().height + m_pBtnAchievementClass->getContentSize().height / 2));

            pButton->setEnabled(true);
            pButton->setVisible(true);
            pButton->setTag(key);
            pButton->addTouchEventListener(this, toucheventselector(PopUpUI::achievementClassSectionCallback));
            panel->addChild(pButton);
            
            UILabel *pLabel_name = static_cast<UILabel *>(pButton->getChildByName("Label0"));
            AchievementClassData *pClassData = (AchievementClassData *)allDataDic->objectForKey(key);
            pLabel_name->setText(pClassData->getName());
            
            CCString *pStrFinishNum = CCString::createWithFormat("%i", pClassData->getFinishNum());
            UILabel *pLabel_finish_num = static_cast<UILabel *>(pButton->getChildByName("Label2"));
            pLabel_finish_num->setText(pStrFinishNum->getCString());
            
            CCString *pStrTotalNum = CCString::createWithFormat("%i", pClassData->getTotalNum());
            UILabel *pLabel_total_num = static_cast<UILabel *>(pButton->getChildByName("Label3"));
            pLabel_total_num->setText(pStrTotalNum->getCString());
        }
        //panel内元素高度超出scrollView范围 元素位置需要重新修正
        if (nClassCount * m_pBtnAchievementClass->getContentSize().height > m_pUIScrollView->getContentSize().height)
        {
            int nOffSetY = nClassCount * m_pBtnAchievementClass->getContentSize().height - m_pUIScrollView->getContentSize().height;
            for(int i = 0; i < panel->getChildren()->count(); i++)
            {
                
                Widget* node = static_cast<UIWidget*> (panel->getChildren()->objectAtIndex(i));
                node->setPositionY(node->getPositionY() + nOffSetY);
            }
        }
        
    }while (0);
}

#pragma mark 更新获取成就后UI状态
void PopUpUI::updateAchievementTouchGetAward()
{
    do
    {
        CC_BREAK_IF(!m_pUIScrollView);
        Layout *panel = m_pUIScrollView->getInnerContainer();
        CC_BREAK_IF(!panel);
        
        //更新分类完成数据
        AchievementClassData* pClassData = (AchievementClassData *)GlobalData::getInstance()->getAchievementClassData(m_nCurrentAchieveClassIndex);
        UIButton *pButtonClass =  static_cast<UIButton *>(panel->getChildByTag(m_nCurrentAchieveClassIndex));
        if (pButtonClass && pClassData)
        {
                CCString *pClassFinishNum = CCString::createWithFormat("%i", pClassData->getFinishNum());
                UILabel *pLabel_Class = static_cast<UILabel *>(pButtonClass->getChildByName("Label2"));
                pLabel_Class->setText(pClassFinishNum->getCString());
        }
        
        //更新分组完成数据
        AchievementGroupData* pGroupData = (AchievementGroupData *)GlobalData::getInstance()->getAchievementGroupData(m_nCurrentAchieveClassIndex, m_nCurrentAchieveGroupIndex);
        UIButton *pButtonGroup = static_cast<UIButton *>(panel->getChildByTag(m_nCurrentAchieveGroupIndex));
        if (pButtonGroup && pGroupData)
        {
            CCString *pGroupFinishNum = CCString::createWithFormat("%i", pGroupData->getFinishNum());
            UILabel *pLabel_Group = static_cast<UILabel *>(pButtonGroup->getChildByName("Label2"));
            pLabel_Group->setText(pGroupFinishNum->getCString());
        }
        
        //更新分项数据
        CC_BREAK_IF(!m_pUIPageView);
        CCArray* pageViewItemArr = m_pUIPageView->getPages();  //pageView内元素数组
        CC_BREAK_IF(!pageViewItemArr);
        Layout *panel2_1 = static_cast<Layout* >(pageViewItemArr->objectAtIndex(m_nCurrentAchievePage));
        CC_BREAK_IF(!panel2_1);
        AchievementItemData* pItemData = (AchievementItemData *)GlobalData::getInstance()->getAchievementItemData(m_nCurrentAchieveClassIndex, m_nCurrentAchieveGroupIndex,m_nCurrentAchieveItemIndex);
        CC_BREAK_IF(!pItemData);
        UIImageView *pImageView = static_cast<UIImageView *>(panel2_1->getChildByTag(pItemData->getIndex()));
        CC_BREAK_IF (!pImageView);
        int nStatus = pItemData->getGetStatus();
        //显示成就领取状态
        switch (nStatus)
        {
            case 0: //未完成
            {
                //未完成
                UIImageView *pImageView_unfinished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView3"));
                CC_BREAK_IF(!pImageView_unfinished);
                pImageView_unfinished->setVisible(true);
            }
                break;
            case 1: //已完成
            {
                //已完成
                UIImageView *pImageView_finished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView1"));
                CC_BREAK_IF(!pImageView_finished);
                pImageView_finished->setVisible(true);
                
                //未领取
                UIImageView *pImageView_unGetted_award = static_cast<UIImageView *>(pImageView->getChildByName("ImageView4"));
                CC_BREAK_IF (!pImageView_unGetted_award);
                pImageView_unGetted_award->setVisible(true);
                pImageView_unGetted_award->setZOrder(1);
                
                pImageView->setTouchEnabled(true); //可点击
            }
                break;
            case 2: //已领取
            {
                //已完成
                UIImageView *pImageView_finished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView1"));
                CC_BREAK_IF(!pImageView_finished);
                pImageView_finished->setVisible(true);
                
                //已领取
                UIImageView *pImageView_getted_award = static_cast<UIImageView *>(pImageView->getChildByName("ImageView2"));
                if (!pImageView_getted_award)
                    return;
                pImageView_getted_award->setVisible(true);
                pImageView_getted_award->setZOrder(1);
            }
                break;
            default:
                break;
        }
        
        //更新分组奖励信息
        updateAchievementGroupAwardUI(m_nCurrentAchieveClassIndex, m_nCurrentAchieveGroupIndex);
    }while (0);
}

#pragma mark 更新分项UI
void PopUpUI::updateAchievementItemUI(int nClassIdx, int nGroupIdx)
{
    do {
        m_nCurrentAchieveClassIndex  = nClassIdx;
        m_nCurrentAchieveGroupIndex = nGroupIdx;
        
        CC_BREAK_IF(!m_pUITouchGroup);
        Layout *panel2 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_BREAK_IF(!panel2);
        Layout *panel2_0 = static_cast<Layout *>(panel2->getChildByName("Panel2_0"));
        CC_BREAK_IF(!panel2_0);
        UIPageView* pageView = static_cast<UIPageView*>(panel2->getChildByName("PageView_0"));
        CC_BREAK_IF(!pageView);
        CCArray* pageViewItemArr = pageView->getPages();  //pageView内元素数组
        CC_BREAK_IF(!pageViewItemArr);
        
        //pageView 移到第一页
        pageView->scrollToPage(0);
        m_nCurrentAchievePage = 0;
        //圆点标记位切换
        for (int i = 0; i < ACHIEVE_PAGE_NUM; i++)
        {
            UIImageView* pImageView = static_cast<UIImageView *>(panel2_0->getChildByName((CCString::createWithFormat("ImageView0_%i",i))->getCString()));
            CC_BREAK_IF(!pImageView);
            std::string filePath;
            if (0 == i)
                filePath= getFilePath("412", kDirTypeUI, kFileSuffixPng);
            else
                filePath= getFilePath("411", kDirTypeUI, kFileSuffixPng);
            pImageView->loadTexture(filePath.c_str());
        }

        //重置pageView显示内容
        for (int i = 0; i < ACHIEVE_PAGE_NUM; i++)
        {
            Layout *panel2_1 = static_cast<Layout* >(pageViewItemArr->objectAtIndex(i));
            CC_BREAK_IF(!panel2_1);
            for (int j = 0; j < ACHIEVE_PER_PAGE_NUM; j++)
            {
                CCString *pStr = CCString::createWithFormat("ImageView1_%i", j);
                UIImageView *pImageView = static_cast<UIImageView *>(panel2_1->getChildByName(pStr->getCString()));
                CC_BREAK_IF(!pImageView);
                pImageView->setEnabled(true);
                pImageView->setVisible(true);
                pImageView->setTouchEnabled(false);
           
                UIImageView *pImageView_award_bg = static_cast<UIImageView *>(pImageView->getChildByName("ImageView0"));
                CC_BREAK_IF(!pImageView_award_bg);
                pImageView_award_bg->removeAllChildren();
                pImageView_award_bg->setVisible(false);
                
                UILabel *pLabel_name = static_cast<UILabel *>(pImageView->getChildByName("Label0"));
                CC_BREAK_IF (!pLabel_name);
                pLabel_name->setText(" ");
                UILabel *pLabel_desc = static_cast<UILabel *>(pImageView->getChildByName("Label1"));
                CC_BREAK_IF (!pLabel_desc);
                pLabel_desc->setText(" ");

                UIImageView *pImageView_finished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView1"));
                CC_BREAK_IF (!pImageView_finished);
                pImageView_finished->setVisible(false);
                
                UIImageView *pImageView_getted_award = static_cast<UIImageView *>(pImageView->getChildByName("ImageView2"));
                CC_BREAK_IF (!pImageView_getted_award);
                pImageView_getted_award->setVisible(false);
                
                UIImageView *pImageView_unGetted_award = static_cast<UIImageView *>(pImageView->getChildByName("ImageView4"));
                CC_BREAK_IF (!pImageView_unGetted_award);
                pImageView_unGetted_award->setVisible(false);
                
                UIImageView *pImageView_unfinished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView3"));
                CC_BREAK_IF(!pImageView_unfinished);
                pImageView_unfinished->setVisible(false);
            }
        }
    
        AchievementGroupData* pGroupData = GlobalData::getInstance()->getAchievementGroupData(nClassIdx, nGroupIdx);
        CCDictionary* pItmeDic = pGroupData->getChildDic();
        int nItemCount = pItmeDic->count();     //单项成就个数
        CCArray* arrItemAllKey = pItmeDic->allKeys();
        bool bFinishUpdateUI = false;
        
        for (int i = 0; i < ACHIEVE_PAGE_NUM; i++)
        {
            CC_BREAK_IF(bFinishUpdateUI);
            Layout *panel2_1 = static_cast<Layout* >(pageViewItemArr->objectAtIndex(i));
            CC_BREAK_IF(!panel2_1);
            
            for (int j = 0; j < ACHIEVE_PER_PAGE_NUM; j++)
            {
                int keyIdx = (i * ACHIEVE_PER_PAGE_NUM + j);
                if (keyIdx >= nItemCount)
                {
                    bFinishUpdateUI = true;
                    break;
                }
                int key = ((CCInteger *)arrItemAllKey->objectAtIndex(keyIdx))->getValue();
                AchievementItemData *pItemData = (AchievementItemData *)pItmeDic->objectForKey(key);
                CC_BREAK_IF(!pItemData);
                AwardData *pAwardData = pItemData->getAwardData();
                CC_BREAK_IF(!pAwardData);
                
                CCString *pStr = CCString::createWithFormat("ImageView1_%i", j);
                UIImageView *pImageView = static_cast<UIImageView *>(panel2_1->getChildByName(pStr->getCString()));
                CC_BREAK_IF (!pImageView);
                
                pImageView->setTag(pItemData->getIndex());
                pImageView->addTouchEventListener(this, toucheventselector(PopUpUI::achievementDetailGetGeneralAwardCallback));
                
                //描述
                UILabel *pLabel_desc = static_cast<UILabel *>(pImageView->getChildByName("Label1"));
                CC_BREAK_IF (!pLabel_desc);
                pLabel_desc->setText(pItemData->getDesc());
                
                //奖励背景
                UIImageView *pImageView_award_bg = static_cast<UIImageView *>(pImageView->getChildByName("ImageView0"));
                CC_BREAK_IF (!pImageView_award_bg);
                pImageView_award_bg->setVisible(true);
                
                //奖励图片
                UIImageView *pImageAward = UIImageView::create();
                std::string str = getFilePath(pAwardData->getIcon(), kDirTypeIcon, kFileSuffixPng);
                pImageAward->loadTexture(str.c_str());
                pImageView_award_bg->addChild(pImageAward);
                
                //奖励数量
                UILabel *pLabelAwardNum = createLabel(CCSizeMake(80, 30), kCCTextAlignmentRight, 20, ccWHITE);
                pLabelAwardNum->setPosition(ccp(0.0f, -pImageView_award_bg->getSize().height / 2 + 15));
                pImageView_award_bg->addChild(pLabelAwardNum);
                CCString *pStrNum = CCString::createWithFormat("%d", pAwardData->getNum());
                std::string strNum = string("x") + pStrNum->getCString();
                pLabelAwardNum->setText(strNum);
                
                int nStatus = pItemData->getGetStatus();
                
                //显示成就领取状态
                switch (nStatus)
                {
                    case 0: //未完成
                    {
                      //未完成
                      UIImageView *pImageView_unfinished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView3"));
                      CC_BREAK_IF(!pImageView_unfinished);
                      pImageView_unfinished->setVisible(true);
                    }
                        break;
                    case 1: //已完成
                    {
                      //已完成
                      UIImageView *pImageView_finished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView1"));
                      CC_BREAK_IF(!pImageView_finished);
                      pImageView_finished->setVisible(true);
                        
                      //未领取
                      UIImageView *pImageView_unGetted_award = static_cast<UIImageView *>(pImageView->getChildByName("ImageView4"));
                      CC_BREAK_IF (!pImageView_unGetted_award);
                      pImageView_unGetted_award->setVisible(true);
                      pImageView_unGetted_award->setZOrder(1);

                        pImageView->setTouchEnabled(true); //可点击
                    }
                        break;
                    case 2: //已领取
                    {
                        //已完成
                        UIImageView *pImageView_finished = static_cast<UIImageView *>(pImageView->getChildByName("ImageView1"));
                        CC_BREAK_IF(!pImageView_finished);
                        pImageView_finished->setVisible(true);
                        
                        //已领取
                        UIImageView *pImageView_getted_award = static_cast<UIImageView *>(pImageView->getChildByName("ImageView2"));
                        if (!pImageView_getted_award)
                            return;
                        pImageView_getted_award->setVisible(true);
                        pImageView_getted_award->setZOrder(1);
                    }
                        break;
                    default:
                        break;
                }
            }
        }
        updateAchievementGroupAwardUI(nClassIdx, nGroupIdx);  //更新分组奖励信息
    } while (0);
}

#pragma mark 更新分组UI
void PopUpUI::updateAchievementGroupAwardUI(int nClassIdx, int nGroupIdx)
{
    do
    {
        CC_BREAK_IF(!m_pUITouchGroup);
        Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_BREAK_IF(!panel2);
        UIButton *pBtnGroupAward = static_cast<UIButton *>(panel2->getChildByName("Button_done"));
        CC_BREAK_IF(!pBtnGroupAward);
        UIImageView* pImageBox = static_cast<UIImageView *>(panel2->getChildByName("ImageView_1"));
        CC_BREAK_IF(!pImageBox);
        UILabel *pLabel_name = static_cast<UILabel *>(panel2->getChildByName("Label_0"));
        CC_BREAK_IF(!pLabel_name);
        UILabel *pLabel_desc = static_cast<UILabel *>(panel2->getChildByName("Label_1"));
        CC_BREAK_IF(!pLabel_desc);
        
        AchievementGroupData *pGroupData = GlobalData::getInstance()->getAchievementGroupData(nClassIdx, nGroupIdx);
        CC_BREAK_IF(!pGroupData);
        AwardData *pAward = pGroupData->getAwardData();
        CC_BREAK_IF(!pAward);
        
        //本组奖励名称
        pLabel_name->setText(pGroupData->getName());

        //本组奖励数量
        UILabel *pLabelGetTip = (UILabel *)pImageBox->getChildByTag(20);
        if (!pLabelGetTip)
        {
            pLabelGetTip = createLabel(CCSizeMake(80, 30), kCCTextAlignmentRight, 20, ccWHITE);
            pLabelGetTip->setPosition(ccp(0.0f, -pImageBox->getSize().height / 2 + 10));
            pImageBox->addChild(pLabelGetTip, 0, 20);
        }

        int nStatus = pGroupData->getGetStatus();
        switch (nStatus)
        {
            case 0: //未完成
            {
                //图片
                std::string str = getFilePath("51", kDirTypeUI, kFileSuffixPng);
                pImageBox->loadTexture(str.c_str());
                //btn
                pBtnGroupAward->setBright(false);
                pBtnGroupAward->setTouchEnabled(false);
                pBtnGroupAward->setTitleText(g_sAchiStatus0);
                //描述
                pLabel_desc->setText(g_sAchiStatusDesc0);
                pLabelGetTip->setVisible(false);
                
            }
                break;
            case 1: //已完成
            {
                //图片
                std::string str;
                if (pAward->getIcon().compare(std::string("")))
                {
                    str = getFilePath(pAward->getIcon(), kDirTypeIcon, kFileSuffixPng);
                }
                else
                {
                    str = getFilePath("51", kDirTypeUI, kFileSuffixPng);
                }
                pImageBox->loadTexture(str.c_str());
                //btn
                pBtnGroupAward->setBright(true);
                pBtnGroupAward->setTouchEnabled(true);
                pBtnGroupAward->setTitleText(g_sAchiStatus1);
                //描述
                pLabel_desc->setText(g_sAchiStatusDesc1);
                //数量
                pLabelGetTip->setVisible(true);
                CCString *pStrNum = CCString::createWithFormat("%d", pAward->getNum());
                std::string strNum = string("x") + pStrNum->getCString();
                pLabelGetTip->setText(strNum);
            }
                break;
            case 2: //可领取
            {
                //图片
                std::string str;
                if (pAward->getIcon().compare(std::string("")))
                {
                    str = getFilePath(pAward->getIcon(), kDirTypeIcon, kFileSuffixPng);
                }
                else
                {
                    str = getFilePath("51", kDirTypeUI, kFileSuffixPng);
                }
                pImageBox->loadTexture(str.c_str());
                //btn
                pBtnGroupAward->setBright(false);
                pBtnGroupAward->setTouchEnabled(false);
                pBtnGroupAward->setTitleText(g_sAchiStatus2);
                //描述
                pLabel_desc->setText(g_sAchiStatusDesc2);
                //数量
                pLabelGetTip->setVisible(true);
                CCString *pStrNum = CCString::createWithFormat("%d", pAward->getNum());
                std::string strNum = string("x") + pStrNum->getCString();
                pLabelGetTip->setText(strNum);
                
            }
                break;
            default:
                break;
        }
    } while (0);
}

#pragma mark 在主线程中弹出任务完成提示界面UI
void PopUpUI::drawAchievementTipUI(int nID, std::string sName, int nState, std::string sTime, std::string sDesc, std::string sIcon, int nNum)
{
    m_bIsPopUpAchievementTip = true;
    m_fStartTime = 0;
    
    YYTouchGroup *touchPop = YYTouchGroup::create();
    touchPop->setPosition(m_cAdjustScreenPosition);
    addChild(touchPop, 10);
    touchPop->setDelegate(this, kUIAchieve);
    touchPop->setTouchEnabled(true);
    touchPop->setVisible(true);
    touchPop->setTag(TAG_POPUP_ACHIEVE_WINDOW);
    std::string filePath = POP_UI_RESOURCE_FILE_NAME;
    touchPop->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    Layout *pPanel = static_cast<Layout *>(touchPop->getWidgetByName("Panel"));
    for (int i = 0; i < pPanel->getChildrenCount(); i++)
    {
        Widget * widget= static_cast<Widget *>(pPanel->getChildren()->objectAtIndex(i));
        widget->setEnabled(false);
        widget->setVisible(false);
        widget->setTouchEnabled(false);
    }
    Layout *pPanel_0 = static_cast<Layout *>(pPanel->getChildByName("Panel_0"));
    pPanel_0->setEnabled(true);
    pPanel_0->setVisible(true);
    pPanel_0->setTouchEnabled(true);
    for (int i = 0; i < pPanel_0->getChildrenCount(); i++)
    {
        Widget * widget= static_cast<Widget *>(pPanel_0->getChildren()->objectAtIndex(i));
        widget->setEnabled(false);
        widget->setVisible(false);
        widget->setTouchEnabled(false);
    }
    Layout *pPanel_d = static_cast<Layout *>(pPanel_0->getChildByName("Panel_d"));
    if (pPanel_d)
    {
        pPanel_d->setEnabled(true);
        pPanel_d->setVisible(true);
        pPanel_d->setTouchEnabled(true);
    }
    
    UILabel *label_name = static_cast<UILabel *>(pPanel_d->getChildByName("Label0"));
    if (label_name)
        label_name->setText(sName);
    
    UILabel *label_des = static_cast<UILabel *>(pPanel_d->getChildByName("Label1"));
    if (label_des)
        label_des->setText(sDesc);
    
    UILabel *label_time = static_cast<UILabel *>(pPanel_d->getChildByName("Label2"));
    if (label_time)
        label_time->setText(sTime);
    
    UIImageView *pImage_icon_bg = static_cast<UIImageView *>(pPanel_d->getChildByName("ImageView0"));
    if (pImage_icon_bg)
    {
        UIImageView *pImageIcon = UIImageView::create();
        std::string str = getFilePath(sIcon, kDirTypeIcon, kFileSuffixPng);
        pImageIcon->loadTexture(str.c_str());
        pImage_icon_bg->addChild(pImageIcon);
        
        CCString *pStrNum = CCString::createWithFormat("%d", nNum);
        UILabel *pLabelGetTip = createLabel(CCSizeMake(80, 30), kCCTextAlignmentCenter, 20, ccWHITE);
        std::string strNum = string("x") + pStrNum->getCString();
        pLabelGetTip->setText(strNum);
        pLabelGetTip->setPosition(ccp(pImage_icon_bg->getSize().width / 2 - 20, -pImage_icon_bg->getSize().height / 2 + 15));
        pImage_icon_bg->addChild(pLabelGetTip);
    }
    if (nState != 0)
    {
        UIImageView *pImage_finish = static_cast<UIImageView *>(pPanel_d->getChildByName("ImageView1"));
        pImage_finish->setVisible(true);
    }
}

#pragma mark 在主线程中隐藏提示界面UI
void PopUpUI::updateAchievementShowTip(float f)
{
    if (m_bIsPopUpAchievementTip)
    {
        m_fStartTime += f;
        if (m_fStartTime >= 3.0f)
        {
            m_bIsPopUpAchievementTip = false;
            m_fStartTime = 0;
            YYTouchGroup *touchPop = static_cast<YYTouchGroup *>(getChildByTag(TAG_POPUP_ACHIEVE_WINDOW));
            if (touchPop)
                removeChild(touchPop);
        }
    }
}

#pragma mark 在主线程中重置提示界面UI
void PopUpUI::resetAchievementShowTip()
{
    if (m_bIsPopUpAchievementTip)
    {
        m_bIsPopUpAchievementTip = false;
        m_fStartTime = 0;
        YYTouchGroup *touchPop = static_cast<YYTouchGroup *>(getChildByTag(TAG_POPUP_ACHIEVE_WINDOW));
        if (touchPop)
            removeChild(touchPop);
    }
}

#pragma mark 成就界面弹出奖励领取Tips
void PopUpUI::drawAchievementGetAwardPopUpUI(std::string sIcon, int nNum, cocos2d::gui::SEL_TouchEvent selector)
{
    m_pUITouchGroup->setTouchEnabled(false);
    
    Layout *panel_b = (Layout *)getPopWindow("Panel_2", "Panel_b", true);
    UIImageView *pImgView = static_cast<UIImageView *>(panel_b->getChildByName("ImageView"));
    CC_ASSERT(pImgView);
    UILabel *label1 = static_cast<UILabel *>(pImgView->getChildByName("TextArea"));
    CC_ASSERT(label1);
    label1->setText(g_sCongratulateGetAward);
    
    UILabel *label2 = static_cast<UILabel *>(pImgView->getChildByName("TextArea_0"));
    CC_ASSERT(label2);
    label2->setVisible(false);
    
    UIButton *pButtonSure = static_cast<UIButton *>(panel_b->getChildByName("Button0"));
    pButtonSure->addTouchEventListener(this, selector);
    
    ImageView *pImageView_0 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_0"));
    ImageView *pImageView_1 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_1"));
    ImageView *pImageView_2 = static_cast<ImageView *>(panel_b->getChildByName("ImageView_2"));
    pImageView_0->setVisible(false);
    pImageView_1->setVisible(false);
    pImageView_2->setVisible(false);
    
    CCString *pStr = CCString::createWithFormat("ImageView_%i", 1);
    ImageView *pImageView = static_cast<ImageView *>(panel_b->getChildByName(pStr->getCString()));
    pImageView->setVisible(true);
    pImageView->removeAllChildren();
    
    ImageView *pImgIcon = ImageView::create();
    std::string str = getFilePath(sIcon, kDirTypeIcon, kFileSuffixPng);
    pImgIcon->loadTexture(str.c_str());
    pImageView->addChild(pImgIcon);
    
    UILabel *pLabelGetTip = createLabel(CCSizeMake(80, 30), kCCTextAlignmentRight, 20, ccWHITE);
    pLabelGetTip->setPosition(ccp(0.0f, -pImageView->getSize().height / 2 + 15));
    pImageView->addChild(pLabelGetTip);
    CCString *pStrDes = CCString::createWithFormat("x%d" ,nNum);
    pLabelGetTip->setText(pStrDes->getCString());
}

#pragma mark 成就界面弹出奖励Tips确认按钮回调
void PopUpUI::achievementSureGetAwardCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        Controller::getInstance()->sendMessage(ASK_ACHIEVEMENT_LIST, true); //更新UI状态
    }
}

#pragma mark 主线程接收服务器下发的奖励数据
void PopUpUI::setAchievementTipDataInfo(cocos2d::CCArray *array)
{
    do
    {
        CC_BREAK_IF(!array);
        CCString *pStrIndex = (CCString *)array->objectAtIndex(0);
        CC_BREAK_IF(!pStrIndex);
        int nIndex = pStrIndex->intValue();
        int nClassidx = getFirstNBitNumber(nIndex, 2); //分类ID
        int nGroupidx = getFirstNBitNumber(nIndex, 4); //分组ID
        int nItemidx  = nIndex; //分项ID
        AchievementItemData *pGlobalDetailData = GlobalData::getInstance()->getAchievementItemData(nClassidx,nGroupidx,nItemidx);
        CC_BREAK_IF(!pGlobalDetailData);
        AwardData *pAward = pGlobalDetailData->getAwardData();
        CC_BREAK_IF(!pAward);
        AchieveTips* tips = AchieveTips::create();
        CC_BREAK_IF(!tips);
        tips->setNum(pAward->getNum());
        tips->setDesc(pGlobalDetailData->getDesc());
        tips->setIcon(pAward->getIcon());
        tips->setIdx(nIndex);
        CC_BREAK_IF(!m_pAchieveTipsArray);
        m_pAchieveTipsArray->addObject(tips);
    } while (0);

}

#pragma mark 成就界面接收服务器下发的获取奖励确认信息
void PopUpUI::setAchievementGetAwardDataInfo(cocos2d::CCArray *array)
{
    do
    {
        CC_BREAK_IF(!array);
        CCString *pStrIndex = (CCString *)array->objectAtIndex(0);
        CC_BREAK_IF(!pStrIndex);
        int nIndex = pStrIndex->intValue();
        if (pStrIndex->length() == 6)  //完成单项成就
        {
            int nClassidx = getFirstNBitNumber(nIndex, 2); //分类ID
            int nGroupidx = getFirstNBitNumber(nIndex, 4); //分组ID
            int nItemidx  = nIndex; //分项ID
            AchievementItemData *pItemData = GlobalData::getInstance()->getAchievementItemData(nClassidx,nGroupidx,nItemidx);
            pItemData->setGetStatus(2);  //分项成就状态已领取
            drawAchievementGetAwardPopUpUI(pItemData->getAwardData()->getIcon(),pItemData->getAwardData()->getNum(),SEL_TouchEvent(&PopUpUI::achievementSureGetAwardCallback));
        }
        else if (pStrIndex->length() == 4) //完成分组成就
        {
            int nClassidx = getFirstNBitNumber(nIndex, 2); //分类ID
            int nGroupidx = nIndex;                        //分组ID
            AchievementGroupData *pGroupData = GlobalData::getInstance()->getAchievementGroupData(nClassidx,nGroupidx);
            pGroupData->setGetStatus(2); //分组成就状态已领取
            drawAchievementGetAwardPopUpUI(pGroupData->getAwardData()->getIcon(),pGroupData->getAwardData()->getNum(),SEL_TouchEvent(&PopUpUI::achievementSureGetAwardCallback));
        }
    } while (0);
    return;
 }

#pragma mark 退出成就UI
void PopUpUI::exitAchievement()
{
    do {
        //重置成就已完成数据
        CCDictionary* pClassDic = GlobalData::getInstance()->getAchievementAllDataDict();
        CC_BREAK_IF(!pClassDic);
        CCArray* arrClassKey = pClassDic->allKeys();
        CC_BREAK_IF(!arrClassKey);
        for (int i = 0; i < arrClassKey->count(); i++)
        {
            int nClassKey = ((CCInteger *)arrClassKey->objectAtIndex(i))->getValue();
            AchievementClassData* pClassData = (AchievementClassData *)pClassDic->objectForKey(nClassKey);
            CC_BREAK_IF(!pClassData);
            pClassData->setFinishNum(0);
            CCDictionary* pGroupDic = pClassData->getChildDic();
            CC_BREAK_IF(!pGroupDic);
            CCArray* arrGroupKey = pGroupDic->allKeys();
            CC_BREAK_IF(!arrGroupKey);
            for (int j = 0; j < arrGroupKey->count(); j++)
            {
                int nGroupKey = ((CCInteger *)arrGroupKey->objectAtIndex(j))->getValue();
                AchievementGroupData* pGroupData = (AchievementGroupData *)pGroupDic->objectForKey(nGroupKey);
                CC_BREAK_IF(!pGroupData);
                pGroupData->setFinishNum(0);
            }
        }
        m_pUIPageView = NULL;
        m_pUIScrollView = NULL;
    } while (0);
}

#pragma mark 点击分类事件回调
void PopUpUI::achievementClassSectionCallback(CCObject* pSender, cocos2d::gui::TouchEventType type) //左侧类型数据选择
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
        if (!getIsAchieveInit())
            return;
        UIButton *pButtonType = (UIButton *)pSender;
        int nTag = pButtonType->getTag() - 10;
        CCDictionary* allDataDic = GlobalData::getInstance()->getAchievementAllDataDict();
        CCArray* allKeys = allDataDic->allKeys();
        int nClassCount = allDataDic->count();
        int key = -1;
        
        //重置成就UI列表
        Layout *panel = m_pUIScrollView->getInnerContainer();
        for (int i = 0; i < nClassCount; i++)
        {
            key = ((CCInteger *)allKeys->objectAtIndex(i))->getValue();
            AchievementClassData *pClassData = (AchievementClassData *)allDataDic->objectForKey(key);
            UIButton *pButton = (UIButton *)panel->getChildByTag(key);
            pButton->setPositionY(m_pUIScrollView->getContentSize().height- (i+1) * pButtonType->getContentSize().height + pButtonType->getContentSize().height * 0.5f);
            
            UIImageView* pImgView = static_cast<UIImageView *>(pButton->getChildByName("ImageView_0"));
            if(pImgView)
            {
                //选中框不显示
                pImgView->setVisible(false);
                
                //btn背景
                std::string filePath= getFilePath("385", kDirTypeUI, kFileSuffixPng);
                pButton->loadTextureNormal(filePath.c_str());
            }
            
            CCDictionary* pGroupDic = pClassData->getChildDic();
            CC_BREAK_IF(!pGroupDic);
            CCArray* arrGroupAllKeys = pGroupDic->allKeys();
            CC_BREAK_IF(!arrGroupAllKeys);
            for (int j = 0; j < pGroupDic->count(); j++)
            {
                int nGroupKey = ((CCInteger *)arrGroupAllKeys->objectAtIndex(j))->getValue();
                if (panel->getChildByTag(nGroupKey))
                {
                    panel->removeChildByTag(nGroupKey);
                }
            }
        }
        
        //重置列表高度
        m_pUIScrollView->setInnerContainerSize(CCSizeMake(m_pBtnAchievementClass->getContentSize().width, nClassCount * m_pBtnAchievementClass->getContentSize().height));
        
        //设置选中框
        if (pButtonType)
        {
            UIImageView* pImgView = static_cast<UIImageView *>(pButtonType->getChildByName("ImageView_0"));
            if(pImgView)
            {
                pImgView->setVisible(true);
                
                //btn背景
                std::string filePath= getFilePath("386", kDirTypeUI, kFileSuffixPng);
                pButtonType->loadTextureNormal(filePath.c_str());
            }
        }
    
        //点击后列表数据展开
        key = ((CCInteger *)allKeys->objectAtIndex(nTag))->getValue();
        AchievementClassData* pClassData = (AchievementClassData *)allDataDic->objectForKey(key);
        int nGroupCount = pClassData->getTotalNum();
        CCDictionary* pGroupDic = pClassData->getChildDic();
        CCArray* GroupAllKeys = pGroupDic->allKeys();
        for (int i = 0; i < nGroupCount; i++)
        {
            int nGroupKey = ((CCInteger *)GroupAllKeys->objectAtIndex(i))->getValue();
            AchievementGroupData *pGroupData = (AchievementGroupData *)pGroupDic->objectForKey(nGroupKey);
            UIButton *pButton = (UIButton *)m_pBtnAchievementClass->clone();
            if (!pButton)
                    return;
            pButton->setPosition(ccp(pButtonType->getPosition().x, pButtonType->getPositionY() - pButtonType->getContentSize().height * 0.5f - i * (pButtonType->getContentSize().height - 18) - (pButtonType->getContentSize().height - 18) * 0.5f));
                
            pButton->setScale(0.8f);
            pButton->setEnabled(true);
            pButton->setVisible(true);
            pButton->setTag(pGroupData->getIndex());
            pButton->addTouchEventListener(this, toucheventselector(PopUpUI::achievementGroupSectionCallback));
            panel->addChild(pButton);
            
            //默认选中第一项 选中框
            if(i == 0)
            {
                UIImageView* pImgView = static_cast<UIImageView *>(pButton->getChildByName("ImageView_0"));
                if(pImgView)
                {
                    pImgView->setVisible(true);
                    
                    //btn背景
                    std::string filePath= getFilePath("386", kDirTypeUI, kFileSuffixPng);
                    pButton->loadTextureNormal(filePath.c_str());
                }
            }
            
            UILabel *pLabel_name = static_cast<UILabel *>(pButton->getChildByName("Label0"));
            pLabel_name->setText(pGroupData->getName());
                
            CCString *pStrFinishNum = CCString::createWithFormat("%i", pGroupData->getFinishNum());
            UILabel *pLabel_finish_num = static_cast<UILabel *>(pButton->getChildByName("Label2"));
            pLabel_finish_num->setText(pStrFinishNum->getCString());
                
            CCString *pStrTotalNum = CCString::createWithFormat("%i", pGroupData->getTotalNum());
            UILabel *pLabel_total_num = static_cast<UILabel *>(pButton->getChildByName("Label3"));
            pLabel_total_num->setText(pStrTotalNum->getCString());
        }
        //分组按钮位置位移
        scrollViewIntervalChanged(nTag + 1, pButtonType->getContentSize().height - 18, nGroupCount);
        
        int nClassIdx = nTag;
        int nGroupIdx = 0;
        getAchievementUpdateIndex(nClassIdx, nGroupIdx);
        updateAchievementItemUI(nClassIdx, nGroupIdx);
    }
}

#pragma mark 点击分组事件回调
void PopUpUI::achievementGroupSectionCallback(CCObject* pSender, cocos2d::gui::TouchEventType type) //左侧分组数据选择
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            CC_BREAK_IF(!getIsAchieveInit());
            UIButton *pButtonType = (UIButton *)pSender;
            CC_BREAK_IF(!pButtonType);
            int nGroupIdx = pButtonType->getTag();
            int nClassIdx = getFirstNBitNumber(nGroupIdx, 2);
            updateAchievementItemUI(nClassIdx, nGroupIdx);
            
            //设置分组选中框
            CC_BREAK_IF(!m_pUIScrollView);
            Layout *panel = m_pUIScrollView->getInnerContainer();
         
            AchievementClassData* pClassData = GlobalData::getInstance()->getAchievementClassData(nClassIdx);
            CCDictionary* pGroupDic = pClassData->getChildDic();
            CC_BREAK_IF(!pGroupDic);
            CCArray* arrGroupAllKeys = pGroupDic->allKeys();
            CC_BREAK_IF(!arrGroupAllKeys);
            for (int i = 0; i < pGroupDic->count(); i++)
            {
                int nGroupKey = ((CCInteger *)arrGroupAllKeys->objectAtIndex(i))->getValue();
                UIButton* pButton = static_cast<UIButton *>(panel->getChildByTag(nGroupKey));
                CC_BREAK_IF(!pButton);
                UIImageView* pImgView = static_cast<UIImageView *>(pButton->getChildByName("ImageView_0"));
                CC_BREAK_IF(!pImgView);
                if (nGroupKey == nGroupIdx)
                {
                    pImgView->setVisible(true);
                    //btn背景
                    std::string filePath= getFilePath("386", kDirTypeUI, kFileSuffixPng);
                    pButton->loadTextureNormal(filePath.c_str());
                }
                else
                {
                    pImgView->setVisible(false);
                    //btn背景
                    std::string filePath= getFilePath("385", kDirTypeUI, kFileSuffixPng);
                    pButton->loadTextureNormal(filePath.c_str());
                }
            }
        }

    } while (0);
}

#pragma mark 请求分项奖励回调
void PopUpUI::achievementDetailGetGeneralAwardCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            CC_BREAK_IF (!getIsAchieveInit());
            UIButton *pButton = (UIButton *)pSender;
            CC_BREAK_IF(!pButton);
            int nItemIdx = pButton->getTag();
            m_nCurrentAchieveItemIndex = nItemIdx;
            CCString *pStr = CCString::createWithFormat(ASK_ACHIEVEMENT_AWARD, nItemIdx);
            Controller::getInstance()->sendMessage(pStr->getCString(), false);
        }
    } while (0);
}

#pragma mark 请求分组奖励回调
void PopUpUI::achievementGroupAwardCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        if (getIsAchieveInit())
        {
            CCString *pStr = CCString::createWithFormat(ASK_ACHIEVEMENT_BOX_AWARD, m_nCurrentAchieveGroupIndex);
            Controller::getInstance()->sendMessage(pStr->getCString(), false);
        }
    }
}

#pragma mark scrollView 高度动态修正
void PopUpUI::scrollViewIntervalChanged(int nIndex, int nIntervalSize, int num)
{
    Layout *panel = m_pUIScrollView->getInnerContainer();
    CCDictionary* allDataDic = GlobalData::getInstance()->getAchievementAllDataDict();
    int nClassCount = allDataDic->count();
    CCArray* allKeys = allDataDic->allKeys();
    for (int i = nIndex; i < nClassCount; i++)
    {
        int key = ((CCInteger *)allKeys->objectAtIndex(i))->getValue();
        UIButton *pButton = (UIButton *)panel->getChildByTag(key);
        pButton->setPositionY(pButton->getPositionY() - num * nIntervalSize);
    }
 
     m_pUIScrollView->setInnerContainerSize(CCSizeMake(m_pBtnAchievementClass->getContentSize().width, nClassCount * m_pBtnAchievementClass->getContentSize().height + num * nIntervalSize));
    
    //panel 内元素位置需要重新修正
    if (nClassCount * m_pBtnAchievementClass->getContentSize().height + num * nIntervalSize > m_pUIScrollView->getContentSize().height)
    {
        int nOffSetY = nClassCount * m_pBtnAchievementClass->getContentSize().height + num * nIntervalSize - m_pUIScrollView->getContentSize().height;
        for(int i = 0; i < panel->getChildren()->count(); i++)
        {
            
            Widget* node = static_cast<UIWidget*> (panel->getChildren()->objectAtIndex(i));
            node->setPositionY(node->getPositionY() + nOffSetY);
        }
    }

    //如果点击最后一个node 移动panel位置
    if (nIndex == nClassCount)
    {
        panel->setPositionY(0.0f);
    }
}
