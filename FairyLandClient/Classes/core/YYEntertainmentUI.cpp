//
//  YYEntertainmentUI.cpp
//  Fairyland
//
//  Created by 付辰昊 on 7/23/14.
//
//

#include "YYPopUpUI.h"

#define PLAYTABLE_COUNT 6  // 活动包含项

#pragma mark 活动初始化

void PopUpUI::initPlayTableUI()
{
    do {
        ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
        Controller::getInstance()->setGameStatus(kStatusUI);
        setUIType(kUIPlayTable);
        m_pUITouchGroup = TouchGroup::create();
        CC_BREAK_IF(!m_pUITouchGroup);
        addChild(m_pUITouchGroup);
        m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
        std::string filePath = PLAYTABLE_UI_RESOURCE_FILE_NAME;
        m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));

        //一级页面初始化
        
        //关闭
        Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        CC_BREAK_IF(!panel0);
        UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
        CC_BREAK_IF(!closeButton);
        closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::playTableCloseCallBack));
        
        //立即进入
        Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_ASSERT(panel2);
        panel2->setVisible(true);
        UIButton* intoBtn=  static_cast<UIButton*>(panel2->getChildByName("Button_0"));
        intoBtn->addTouchEventListener(this, toucheventselector(PopUpUI::playTableSecondPageCallBack));
        
        //活动按钮
        Layout* panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
        CC_BREAK_IF(!panel1);
        panel1->setVisible(true);
        Layout* panel_0 = static_cast<Layout*>(panel1->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel_0);
        panel_0->setVisible(true);

        for (int i = 0; i < PLAYTABLE_COUNT; i++)
        {
            UIImageView *pImageView = dynamic_cast<UIImageView *>(panel_0->getChildByName(CCString::createWithFormat("ImageView0_%d",i)->getCString()));
            pImageView->setTouchEnabled(true);
            pImageView->addTouchEventListener(this, toucheventselector(PopUpUI::playTableItemSwitchCallBack));
            pImageView->setTag(i);
            UIImageView* pImgSel = static_cast<UIImageView *>(pImageView->getChildByName("ImageView_1"));
            CC_BREAK_IF(!pImgSel);
            //默认选中第一项
            if (i == 0)
            {
                pImgSel->setVisible(true);
            }
            else
            {
                pImgSel->setVisible(false);
            }
        }
        
        //屏蔽pageView
        UIPageView* pageView = static_cast<UIPageView *>(panel1->getChildByName("PageView_0"));
        pageView->setVisible(false);
        pageView->setEnabled(false);

        //隐藏活动分项
        for (int i= 0; i < PLAYTABLE_COUNT; i++)
        {
            char chID = static_cast<char>(i+97);
            CCString* panelPath = CCString::createWithFormat("Panel_%c",chID);
            Layout* panel= static_cast<Layout*>(m_pUITouchGroup->getWidgetByName(panelPath->getCString()));
            CC_BREAK_IF(!panel);
            panel->setVisible(false);
            panel->setEnabled(false);
        }
        m_arrPalyTableData = CCArray::create();
        m_arrPalyTableData->retain();
      
        m_nPlayTableCurItemID = 0;       //默认选中第一项
        m_bPlayTableWhichPage = true;    //默认选中一级页面
        m_bPlayTableBtnTouch = true;    //按钮是否可以点击
        
        // 二级页面初始化
        // 转盘
        initPalyTableTurnTable();
        // 骰子
        initPlayTableDice();
        // 地下城
        initPlayTableDungeonUI();
        // 守卫星灵
        initPlayTableProtectProtossUI();
        // boss副本
        initPlayTableBossDungeonUI();
        // 世界boss
        initPlayTableWorldBossUI();

        // 一级页面发送网络请求
        Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_PLAYTABLE_DATA))->getCString(),true);
    } while (0);
}

#pragma mark 刷新一级页面icon数据

void PopUpUI::updatePlayTableUI()
{
    do
    {
        CC_BREAK_IF(!m_arrPalyTableData);
        CC_BREAK_IF(!m_pUITouchGroup);
        Layout* panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
        CC_BREAK_IF(!panel1);
        Layout* panel_0 = static_cast<Layout*>(panel1->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel_0);
        for (int i = 0; i < m_arrPalyTableData->count(); i++)
        {
            PlayTableData* data = static_cast<PlayTableData *>(m_arrPalyTableData->objectAtIndex(i));
            UIImageView* pImageView = static_cast<UIImageView *>(panel_0->getChildByName(CCString::createWithFormat("ImageView0_%d",i)->getCString()));
            UIImageView* pBG = static_cast<UIImageView *>(pImageView->getChildByName("ImageView_0"));
            UILabel* name = static_cast<UILabel *>(pImageView->getChildByName("TextArea_0"));
            name->setText(data->getName());
            UIImageView* icon = static_cast<UIImageView *>(pBG->getChildByTag(10));
            if (!icon)
            {
                icon = UIImageView::create();
                std::string path = getFilePath(data->getIcon(), kDirTypeUI, kFileSuffixPng);
                icon->loadTexture(path.c_str());
                pBG->addChild(icon);
                icon->setTag(10);
            }
        }
        updatePlayTableDescription();
    } while (0);
}

#pragma mark 刷新一级页面描述数据

void PopUpUI::updatePlayTableDescription()
{
    do
    {
        CC_BREAK_IF(!m_arrPalyTableData);
        CC_BREAK_IF(!m_pUITouchGroup);
        Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_BREAK_IF(!panel2);
        PlayTableData* data = static_cast<PlayTableData *>(m_arrPalyTableData->objectAtIndex(m_nPlayTableCurItemID));
        
        UIImageView* imageView = static_cast<UIImageView *>(panel2->getChildByName("ImageView_name"));
        UILabel* name = static_cast<UILabel *>(imageView->getChildByName("Label_name"));
        name->setText(data->getName());
        
        UILabel* level = static_cast<UILabel *>(panel2->getChildByName("Label_level"));
        UILabel* label0 = static_cast<UILabel *>(level->getChildByName("Label0"));
        label0->setText(CCString::createWithFormat("%d",data->getNeedLevel())->getCString());
        
        UILabel* label_time = static_cast<UILabel *>(panel2->getChildByName("Label_time"));
        UILabel* label_time0 = static_cast<UILabel *>(label_time->getChildByName("Label0"));
        label_time0->setText(data->getTime());
        
        UILabel* label_des = static_cast<UILabel *>(panel2->getChildByName("Label_des"));
        label_des->setText(data->getDescription());
        
        UILabel* label_reward = static_cast<UILabel *>(panel2->getChildByName("Label_reword"));
        label_reward->setText(data->getReward());
        
        UIButton* btn0 = static_cast<UIButton *>(panel2->getChildByName("Button_0"));
        if (data->getOpen())
        {
            btn0->setBright(true);
            btn0->setTouchEnabled(true);
        }
        else
        {
            btn0->setBright(false);
            btn0->setTouchEnabled(false);
        }
        
    } while (0);
}

#pragma mark 接收活动是否开启

void PopUpUI::updatePlayTableAI(cocos2d::CCArray* arr)
{
   // AI_1_1_1_1_1_1 (-1 未开启)  (>-1 开启)
    do
    {
        CC_BREAK_IF(!arr);
        CC_BREAK_IF(!m_arrPalyTableData);
        for (int i = 0; i < arr->count(); i++)
        {
            CCString* str = static_cast<CCString *>(arr->objectAtIndex(i));
            PlayTableData* data = static_cast<PlayTableData *>(m_arrPalyTableData->objectAtIndex(i));
            int nOpen = str->intValue();
            (nOpen > -1) ? (data->setOpen(true)) : (data->setOpen(false));
        }
        updatePlayTableUI();
    } while (0);
}

#pragma mark 接收活动内容数据

void PopUpUI::updatePlayTableAH(cocos2d::CCArray* arr)
{
    // AH_100;BOSS副本;356-1;15;00:00-24:00;举起你的武器，铲除葛丹派遣的恶魔部队;奖励：装备卷轴、星灵石;1_
    do
    {
        CC_BREAK_IF(!arr);
        if (m_arrPalyTableData)
        {
            m_arrPalyTableData->removeAllObjects();
        }
        for (int i = 0; i < arr->count(); i++)
        {
            CCString* str = static_cast<CCString *>(arr->objectAtIndex(i));
            CCArray* arrData = split_string(str->getCString(), ';');
            int index = 0;
            PlayTableData* data = PlayTableData::create();
            m_arrPalyTableData->addObject(data);
            data->setIndex(static_cast<CCString *>(arrData->objectAtIndex(index++))->intValue());
            data->setName(static_cast<CCString *>(arrData->objectAtIndex(index++))->getCString());
            data->setIcon(static_cast<CCString *>(arrData->objectAtIndex(index++))->getCString());
            data->setNeedLevel(static_cast<CCString *>(arrData->objectAtIndex(index++))->intValue());
            data->setTime(static_cast<CCString *>(arrData->objectAtIndex(index++))->getCString());
            data->setDescription(static_cast<CCString *>(arrData->objectAtIndex(index++))->getCString());
            data->setReward(static_cast<CCString *>(arrData->objectAtIndex(index++))->getCString());
            int nOpen = static_cast<CCString *>(arrData->objectAtIndex(index))->intValue();
            nOpen > -1 ? (data->setOpen(true)) : (data->setOpen(false));
        }
        updatePlayTableUI();
    } while (0);
}

#pragma mark 一级页面活动按钮回调

void PopUpUI::playTableItemSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_BEGAN)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            UIImageView* imgView = static_cast<UIImageView*>(pSender);
            CC_BREAK_IF(!imgView);
            CC_BREAK_IF(!m_pUITouchGroup);
            UIPanel* panel1 = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel1"));
            CC_BREAK_IF(!panel1);
            UIPanel* panel_0 = static_cast<UIPanel *>(panel1->getChildByName("Panel_0"));
            CC_BREAK_IF(!panel_0);
            for (int i = 0; i < PLAYTABLE_COUNT; i++)
            {
                //选中框
                UIImageView* pImgElem = static_cast<UIImageView *>(panel_0->getChildByTag(i));
                CC_BREAK_IF(!pImgElem);
                UIImageView* pImgSel = static_cast<UIImageView *>(pImgElem->getChildByName("ImageView_1"));
                CC_BREAK_IF(!pImgSel);
                if((strcmp(imgView->getName(), CCString::createWithFormat("ImageView0_%d",i)->getCString()) == 0))
                {
                    pImgSel->setVisible(true);
                    m_nPlayTableCurItemID = i;
                }
                else
                {
                    pImgSel->setVisible(false);
                }
            }
            updatePlayTableDescription();  //更新描述
        }
    } while (0);
}

#pragma mark 立即进入

void PopUpUI::playTableSecondPageCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_BEGAN)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            m_bPlayTableWhichPage = false;    //进入二级页面
            switchPlayTablePageUI();          //切换UI
        }
    } while (0);
}

#pragma mark 一,二级页面关闭按钮

void PopUpUI::playTableCloseCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        if (m_bPlayTableWhichPage)   // 一级页面关闭
        {
            exitUI();
        }
        else                         // 二级页面关闭
        {
            if (!m_bPlayTableBtnTouch) // 动画播放 按钮不可点击
            {
                return;
            }
            m_bPlayTableWhichPage = true;
            switchPlayTablePageUI();
            
            // 重置转盘数据
            playTableTurnTableResetData();
            // 重置世界BOSS数据
            playTableWorldBossResetData();
        }
    }
}

#pragma mark 退出清空堆区数据

void PopUpUI::exitPlayTable()
{
    // 清空活动内容数据
    CC_SAFE_RELEASE(m_arrPalyTableData);
    // 清空转盘数据
    if ( m_arrTurnTableData)
    {
        m_arrTurnTableData->removeAllObjects();
        CC_SAFE_DELETE(m_arrTurnTableData);
    }
    if (m_pTurnTableMoveImgView)
    {
        m_pTurnTableMoveImgView->removeFromParentAndCleanup(true);
    }
    // 清空骰子数据
    if (m_arrDiceData)
    {
        m_arrDiceData->removeAllObjects();
        CC_SAFE_DELETE(m_arrDiceData);
    }
    if (m_pDiceMoveImgView)
    {
        m_pDiceMoveImgView->removeFromParentAndCleanup(true);
        m_pDiceMoveImgView = NULL;
    }
    if (m_pDiceDciePointPic)
    {
        m_pDiceDciePointPic->removeFromParentAndCleanup(true);
        m_pDiceDciePointPic = NULL;
    }
    // 清空地下城数据
    CC_SAFE_DELETE_ARRAY(m_arrDungeonResetTicket);
    CC_SAFE_DELETE_ARRAY(m_bDungeonEnable);
    CC_SAFE_DELETE_ARRAY(m_arrDungeonNeedPlayLevel);
    CC_SAFE_DELETE_ARRAY(m_bDUngeonFirstLogin);
    CC_SAFE_DELETE_ARRAY(m_arrDungeonResetCnt);
    
    // 清空守卫星灵数据
    CC_SAFE_DELETE_ARRAY(m_arrProtossNeedPlayLevel);
    CC_SAFE_DELETE_ARRAY(m_bProtectProtossFinish);
    CC_SAFE_DELETE_ARRAY(m_arrProtectProtossWipe);
    
    
    // 清空boss副本数据
    m_pUIBossDungeonPageView = NULL;
    m_nUIBossDungeonCurPage = 0;
    m_nUIBossDungeonJewCnt = 0;
    m_nUIBossDungeonResidueCnt = 0;
    m_nUIBossDungeonEnableBuyCnt = 0;
    m_bUIBossDungeonTopTouch = false;
    m_nUIBossDungeonCurSelIdx = 0;
    if (m_arrUIBossDungeonData)
    {
        m_arrUIBossDungeonData->removeAllObjects();
        CC_SAFE_RELEASE(m_arrUIBossDungeonData);
    }
    
    // 清空世界boss 数据
    if(m_arrUIWorldBossRankList)
    {
        m_arrUIWorldBossRankList->removeAllObjects();
        CC_SAFE_RELEASE(m_arrUIWorldBossRankList);
    }
    if (m_pUIWorldBossPortrait)
    {
        m_pUIWorldBossPortrait->removeFromParent();
        m_pUIWorldBossPortrait = NULL;
    }
    
}

#pragma mark 一,二级页面切换

void PopUpUI::switchPlayTablePageUI()
{
    do
    {
        bool bSwitch = m_bPlayTableWhichPage;
        Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
        CC_BREAK_IF(!panel1);
        panel1->setVisible(bSwitch);
        panel1->setEnabled(bSwitch);
        
        Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_BREAK_IF(!panel2);
        panel2->setVisible(bSwitch);
        panel2->setEnabled(bSwitch);
        
        // 重置panel_a b c d e f
        for (int i= 0; i < PLAYTABLE_COUNT; i++)
        {
            char chID = static_cast<char>(i+97);
            CCString* panelPath = CCString::createWithFormat("Panel_%c",chID);
            Layout* panel= static_cast<Layout*>(m_pUITouchGroup->getWidgetByName(panelPath->getCString()));
            CC_BREAK_IF(!panel);
            panel->setVisible(false);
            panel->setEnabled(false);
        }
        PlayTableData* data = static_cast<PlayTableData *>(m_arrPalyTableData->objectAtIndex(m_nPlayTableCurItemID));
        // 请求二级页面数据
        if (!bSwitch)
        {
            switch (data->getIndex())
            {
                case 103: // 地下城
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_DUNGEON_INFO))->getCString(),true);
                    break;
                case 105: // 守卫星灵
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_PROTECT_PROTOSS_INFO))->getCString(),true);
                    break;
                case 100: // BOSS副本
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_BOSS_DUNGEON_INFO))->getCString(),true);
                    break;
                case 101: // 转盘游戏
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_TURNTABLE_INFO))->getCString(),true);
                    break;
                case 104: // 投骰游戏
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_DICE_INFO))->getCString(),true);
                    break;
                case 102: // 世界boss
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_WORLD_BOSS_INFO))->getCString(),true);
                    break;
                default:
                    break;
            }
        }
        // 显示二级页面UI
        char chID = '0';
        switch (data->getIndex())
        {
            case 103: // 地下城
                chID = 'a';
                break;
            case 105: // 守卫星灵
                chID = 'b';
                break;
            case 100: // BOSS副本
                chID = 'c';
                break;
            case 101: // 转盘游戏
                chID = 'd';
                break;
            case 104: // 投骰游戏
                chID = 'e';
                break;
            case 102: // 世界boss
                chID = 'f';
                break;
            default:
                break;
        }
        
        Layout *panel = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName((CCString::createWithFormat("Panel_%c",chID))->getCString()));
        CC_BREAK_IF(!panel);
        panel->setVisible(!bSwitch);
        panel->setEnabled(!bSwitch);

    } while (0);
}

#pragma mark 实时更新UI

void PopUpUI::updatePlayTable()
{
    do
    {
        // 转盘转动更新
        updateRoateTurnTable();
        
        // 世界boss更新
        updateWorldBossData();

    } while (0);

}

#pragma mark 转动转盘

void PopUpUI::updateRoateTurnTable()
{
    do
    {
        if(m_bTurnTalbeRunning)
        {
            float fAngle = 0;
            m_nTurnTableCount++;
            int finialPoiont = 0;
            switch (m_nTurnTablePoint)
            {
                case 0:
                    finialPoiont = 4;
                    break;
                case 1:
                    finialPoiont = 3;
                    break;
                case 2:
                    finialPoiont = 2;
                    break;
                case 3:
                    finialPoiont = 1;
                    break;
                case 4:
                    finialPoiont = 8;
                    break;
                case 5:
                    finialPoiont = 7;
                    break;
                case 6:
                    finialPoiont = 6;
                    break;
                case 7:
                    finialPoiont = 5;
                    break;
                default:
                    break;
            }
            
            finialPoiont = 24 + finialPoiont * 3;
            
            if (m_nTurnTableCount <= 9)
            {
                fAngle = 20;
            }
            else if(m_nTurnTableCount <= 15)
            {
                fAngle = 30;
            }
            else if(m_nTurnTableCount <= 24)
            {
                fAngle = 20;
            }
            else if(m_nTurnTableCount <= (finialPoiont))
            {
                fAngle = 15;
            }
 
            
            if (360 == m_pTrunTableImageView->getRotation())
            {
                m_pTrunTableImageView->setRotation(0);
            }
            fAngle+= m_pTrunTableImageView->getRotation();
            m_pTrunTableImageView->setRotation(fAngle);
        
            //CCLOG("m_nTurnTableCount %d  fAngle %f", m_nTurnTableCount, fAngle);
     
            if (m_nTurnTableCount >= (finialPoiont))
            {
                m_bTurnTalbeRunning = false;
                playTableTurnTableIconMove();
            }
        }
    } while (0);
}

#pragma mark 初始化转盘UI

void PopUpUI::initPalyTableTurnTable()
{
    do
    {
        // UI 初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_d = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_d"));
        CC_BREAK_IF(!panel_d);
        
        // 抽奖 领奖 按钮
        UIButton* pBtn = static_cast<UIButton *>(panel_d->getChildByName("Button0"));
        CC_BREAK_IF(!pBtn);
        pBtn->addTouchEventListener(this, toucheventselector(PopUpUI::PlayTableTurnTableCallBack));
        
        // 获取转盘指针
        UIPanel* panel0 = static_cast<UIPanel *>(panel_d->getChildByName("Panel0"));
        CC_BREAK_IF(!panel0);
        UIImageView* pRootImg = static_cast<UIImageView *>(panel0->getChildByName("ImageView1"));
        CC_BREAK_IF(!pRootImg);
        m_pTrunTableImageView = pRootImg;
        
        // 获取位置
        UIImageView* img0 = static_cast<UIImageView *>(pRootImg->getChildByName("ImageView0"));
        UIImageView* img1 = static_cast<UIImageView *>(panel_d->getChildByName("ImageView_1"));
        
        // 数据初始化
        m_arrTurnTableData = new CCArray();
        playTableTurnTableResetData();
        
        m_pTurnTableMoveImgView = UIImageView::create();
        panel_d->addChild(m_pTurnTableMoveImgView);
        m_pTurnTableMoveImgView->setZOrder(20);
        CCPoint tmpPoint = pRootImg->convertToWorldSpace(img0->getPosition());
        m_sTurnTableMoveSPoint = ccp(tmpPoint.x - img0->getSize().width * 1.2f,tmpPoint.y - img0->getSize().height * 0.2f);
        m_sTurnTableMoveEPoint = img1->getPosition();
        m_pTurnTableMoveImgView->setPosition(m_sTurnTableMoveSPoint);
        
    } while (0);
}

#pragma mark 更新转盘页面信息

void PopUpUI::updatePlayTableTT(CCArray * arr)
{
    do {
        CC_BREAK_IF(!arr);
        int index = 0;
        std::string pStr;
        m_arrTurnTableData->removeAllObjects();
        
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_d = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_d"));
        CC_BREAK_IF(!panel_d);
        UIPanel* panel0 = static_cast<UIPanel *>(panel_d->getChildByName("Panel0"));
        CC_BREAK_IF(!panel0);
        UIImageView* pRootImg = static_cast<UIImageView *>(panel0->getChildByName("ImageView1"));
        CC_BREAK_IF(!pRootImg);
        
        // 抽奖按钮
        UIButton* pBtn = static_cast<UIButton *>(panel_d->getChildByName("Button0"));
        CC_BREAK_IF(!pBtn);
        
        // 抽奖消耗奖劵个数
        UILabel* pTextArea0 = static_cast<UILabel *>(panel_d->getChildByName("TextArea0"));
        UILabel* pTextArea0_0 = static_cast<UILabel *>(pTextArea0->getChildByName("TextArea0"));
        CCString * pConsumeTicket = static_cast<CCString *>(arr->objectAtIndex(index++));
        pTextArea0_0->setText(pConsumeTicket->getCString());
        
        // 玩家剩余奖券个数
        UILabel* pTextArea1 = static_cast<UILabel *>(panel_d->getChildByName("TextArea1"));
        UILabel* pTextArea1_0 = static_cast<UILabel *>(pTextArea1->getChildByName("TextArea0"));
        CCString * pSumTicket = static_cast<CCString *>(arr->objectAtIndex(index++));
        pTextArea1_0->setText(pSumTicket->getCString());
        
        // 记录奖券个数
        m_nTurnTableSumTicketCount = atoi(pSumTicket->getCString());
        m_nTurnTableConsumeTicketCount =atoi(pConsumeTicket->getCString());
        
        // 当前奖励 没有为0
        CCString* pStrCurAward = static_cast<CCString *>(arr->objectAtIndex(index++));
        pStr = pStrCurAward->getCString();
    
        if (pStr.compare("0") == 0) // 抽奖
        {
            m_bTurnOrGet = true;
            pBtn->setTitleText(g_sTurnTableGetArard);
        }
        else                        // 领奖
        {
            m_bTurnOrGet = false;
            pBtn->setTitleText(g_sTurnTableReceiveArard);
            
            // 奖励详细信
            CCArray* info = split_string(pStrCurAward->getCString(), ';');
            
            // icon
            CCString* pIcon = static_cast<CCString *>(info->objectAtIndex(2));
            std::string path = getFilePath(pIcon->getCString(), kDirTypeIcon, kFileSuffixPng);
            m_pTurnTableMoveImgView->loadTexture(path.c_str());
            m_pTurnTableMoveImgView->setPosition(m_sTurnTableMoveEPoint);
            m_pTurnTableMoveImgView->setVisible(true);
            
            // label
            UILabel* label0_0 = static_cast<UILabel *>(panel_d->getChildByName("Label_0_0"));
            CCString* count =static_cast<CCString *>(info->objectAtIndex(1));
            CCString* name = static_cast<CCString * >(info->objectAtIndex(3));
            CCString* content = CCString::createWithFormat("%s X %s",name->getCString(),count->getCString());
            label0_0->setTextHorizontalAlignment(kCCTextAlignmentCenter);
            label0_0->setText(content->getCString());
        }
        
        // 转盘内奖励
        for (int i = 0; i < 8; i++)
        {
            CCString* pStrAward = static_cast<CCString *>(arr->objectAtIndex(index++));
            m_arrTurnTableData->addObject(pStrAward); // 缓存数据
            
            CCArray* arrInfo = split_string(pStrAward->getCString(), ';');
            pStr = ((CCString *)arrInfo->objectAtIndex(2))->getCString(); //icon
            pStr = getFilePath(pStr, kDirTypeIcon, kFileSuffixPng);
            
            CCString* strPath = CCString::createWithFormat("ImageView%d",i);
            UIImageView* pImgAward = static_cast<UIImageView *>(pRootImg->getChildByName(strPath->getCString()));
            pImgAward->removeAllChildren();
            
            UIImageView* pImgIcon = UIImageView::create(); 
            pImgIcon->loadTexture(pStr.c_str(),UI_TEX_TYPE_LOCAL);
            pImgIcon->setRotation(0);
            pImgIcon->setAnchorPoint(ccp(0.5f,0.5f));
            pImgAward->addChild(pImgIcon);
            
            pStr = getFilePath("619-1", kDirTypeUI, kFileSuffixPng);
            UIImageView* pImgShade = UIImageView::create();
            pImgShade->loadTexture(pStr.c_str(),UI_TEX_TYPE_LOCAL);
            pImgAward->addChild(pImgShade);         
        }
        
    } while (0);
}

#pragma mark 获取转盘领取信息

void PopUpUI::updatePlayTableTR(CCArray * arr)
{
//    返回消息 TR_消耗奖券个数_剩余奖券个数_[转到位置（0到7）]
//    返回消息 TR_-1(重置转盘)
    do {
        CC_BREAK_IF(!arr);
        int index = 0;
        std::string pStr;
        
        CCString* cStr = static_cast<CCString *>(arr->objectAtIndex(index++));
        pStr = cStr->getCString();
        if (pStr == "-1") //领奖返回信息
        {
            m_pTrunTableImageView->setRotation(0);
            m_nTurnTableCount = 0;
            m_bTurnOrGet = true;
            
            CC_BREAK_IF(!m_pUITouchGroup);
            UIPanel* panel_d = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_d"));
            CC_BREAK_IF(!panel_d);

            UIImageView* img1 = static_cast<UIImageView *>(panel_d->getChildByName("ImageView_1"));
            CC_BREAK_IF(!img1);
            
            //抽奖按钮
            UIButton* pBtn = static_cast<UIButton *>(panel_d->getChildByName("Button0"));
            CC_BREAK_IF(!pBtn);
            
            //按钮文字设为抽奖
            pBtn->setTitleText(g_sTurnTableGetArard);
            
            //奖励详细信息
            UILabel* label0_0 = static_cast<UILabel *>(panel_d->getChildByName("Label_0_0"));
            label0_0->setText(" ");
            
            //隐藏奖励物品
            m_pTurnTableMoveImgView->setVisible(false);
            
            //获取奖励特效
            YYSprite *sprite = YYSprite::createWithActionSet("se5", "se5", kDirTypeEffect);
            sprite->setAction(0, false);
            img1->addNode(sprite);
            m_pSpecialEffectArray->addObject(sprite);
            
        }
        else
        {
            m_nTurnTableCount = 0;
            m_pTrunTableImageView->setRotation(0);
            m_bTurnTalbeRunning = true;
            m_bPlayTableBtnTouch = false;  //转盘转动 按钮不可点击
            
            CCString * pConsumeTicket = cStr; //抽奖消耗奖劵个数
            CCString * pSumTicket = static_cast<CCString *>(arr->objectAtIndex(index++)); //玩家剩余奖券个数
            CCString * point = static_cast<CCString *>(arr->objectAtIndex(index++)); //转盘转动点数
            
            m_nTurnTablePoint = atoi(point->getCString());
            
            CC_BREAK_IF(!m_pUITouchGroup);
            UIPanel* panel_d = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_d"));
            CC_BREAK_IF(!panel_d);
            
            //抽奖消耗奖劵个数
            UILabel* pTextArea0 = static_cast<UILabel *>(panel_d->getChildByName("TextArea0"));
            UILabel* pTextArea0_0 = static_cast<UILabel *>(pTextArea0->getChildByName("TextArea0"));
            pTextArea0_0->setText(pConsumeTicket->getCString());
            
            //玩家剩余奖券个数
            UILabel* pTextArea1 = static_cast<UILabel *>(panel_d->getChildByName("TextArea1"));
            UILabel* pTextArea1_0 = static_cast<UILabel *>(pTextArea1->getChildByName("TextArea0"));
            pTextArea1_0->setText(pSumTicket->getCString());
            
            
            //记录奖券个数
            m_nTurnTableSumTicketCount = atoi(pSumTicket->getCString());
            m_nTurnTableConsumeTicketCount =atoi(pConsumeTicket->getCString());
        }

    }while (0);
}

#pragma mark 抽奖,领奖 按钮回调

void PopUpUI::PlayTableTurnTableCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        CC_BREAK_IF(!m_bPlayTableBtnTouch); //UI 播放 按钮不可点击
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            
            if(m_bTurnOrGet) //摇奖
            {
                if (m_nTurnTableSumTicketCount >= m_nTurnTableConsumeTicketCount)
                {
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_TURNTABLE_ROATE))->getCString(),true);
                }
                else //奖券数量不足
                {
                     ((YYScene*)getParent())->showTips(g_sTurnTableLackOfTicket);
                }
            }
            else //领奖
            {
                Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_TURNTABLE_RECEIVE))->getCString(),true);
            }
        }
    } while (0);
}

#pragma mark 重置转盘数据

void PopUpUI::playTableTurnTableResetData()
{
    m_nTurnTableCount = 0;
    m_nTurnTablePoint = 0;
    m_pTrunTableImageView->setRotation(0);
    m_bTurnTalbeRunning = false;
    m_bPlayTableBtnTouch = true;
}

#pragma mark 移动ICON到奖励区域区域

void PopUpUI::playTableTurnTableIconMove()
{
    do
    {
        CC_BREAK_IF(!m_arrTurnTableData);
        CCString* pStr = static_cast<CCString *>(m_arrTurnTableData->objectAtIndex(m_nTurnTablePoint));
        CCArray* arrIcon = split_string(pStr->getCString(), ';');
        CCString* pIcon = static_cast<CCString *>(arrIcon->objectAtIndex(2));
        std::string path = getFilePath(pIcon->getCString(), kDirTypeIcon, kFileSuffixPng);
        m_pTurnTableMoveImgView->loadTexture(path.c_str());
        m_pTurnTableMoveImgView->setPosition(m_sTurnTableMoveSPoint);
        m_pTurnTableMoveImgView->setVisible(true);
        
        ccBezierConfig bezierCo;
        bezierCo.controlPoint_1 = ccp(m_sTurnTableMoveSPoint.x + 40, m_sTurnTableMoveSPoint.y + 40);
        bezierCo.controlPoint_2 = ccp(m_sTurnTableMoveEPoint.x + 80, m_sTurnTableMoveEPoint.y - 10);
        bezierCo.endPosition = m_sTurnTableMoveEPoint;
        CCBezierTo* bezierTo = CCBezierTo::create(1.5f, bezierCo);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(PopUpUI::playTableTurnTableIconEffect));
        CCSequence*  seq = CCSequence::create(bezierTo, callFunc, NULL);
        m_pTurnTableMoveImgView->runAction(seq);
        
    } while (0);
}

#pragma mark 播放奖励物品特效

void PopUpUI::playTableTurnTableIconEffect()
{
    YYSprite *sprite = YYSprite::createWithActionSet("se2", "se2", kDirTypeEffect);
    sprite->setAction(0, false);
    m_pTurnTableMoveImgView->addNode(sprite);
    m_pSpecialEffectArray->addObject(sprite);
    CCDelayTime* delay = CCDelayTime::create(0.5f);
    CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(PopUpUI::playTableTurnTableFinisned));
    CCSequence*  seq = CCSequence::create(delay, callFunc, NULL);
    this->runAction(seq);
}

#pragma mark 转盘抽奖完成回调

void PopUpUI::playTableTurnTableFinisned()
{
    do {
        CC_BREAK_IF(!m_arrTurnTableData);
        
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_d = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_d"));
        CC_BREAK_IF(!panel_d);
        UIPanel* panel0 = static_cast<UIPanel *>(panel_d->getChildByName("Panel0"));
        CC_BREAK_IF(!panel0);
        UIImageView* pRootImg = static_cast<UIImageView *>(panel0->getChildByName("ImageView1"));
        CC_BREAK_IF(!pRootImg);
        
        //抽奖按钮
        UIButton* pBtn = static_cast<UIButton *>(panel_d->getChildByName("Button0"));
        CC_BREAK_IF(!pBtn);
        
        //按钮文字设为领奖
        pBtn->setTitleText(g_sTurnTableReceiveArard);
        
        //奖励详细信息
        CCString* str = static_cast<CCString *>(m_arrTurnTableData->objectAtIndex(m_nTurnTablePoint));
        CCArray* info = split_string(str->getCString(), ';');
        UILabel* label0_0 = static_cast<UILabel *>(panel_d->getChildByName("Label_0_0"));
        CCString* count =static_cast<CCString *>(info->objectAtIndex(1));
        CCString* name = static_cast<CCString * >(info->objectAtIndex(3));
        CCString* content = CCString::createWithFormat("%s X %s",name->getCString(),count->getCString());
        label0_0->setTextHorizontalAlignment(kCCTextAlignmentCenter);
        label0_0->setText(content->getCString());
        
        // 领奖状态
        m_bTurnOrGet = false;
        m_bPlayTableBtnTouch = true;  //转盘转动 按钮可点击
        
    }while (0);
}

#pragma mark 初始化摇骰子UI

void PopUpUI::initPlayTableDice()
{
    do
    {
        // UI 初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
        CC_BREAK_IF(!panel_e);
        UIPanel* panel_0 = static_cast<UIPanel *>(panel_e->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel_0);
        
        // 投骰 收取 宝石 按钮
        UIButton* pBtn = static_cast<UIButton *>(panel_e->getChildByName("Button_go"));
        CC_BREAK_IF(!pBtn);
        pBtn->addTouchEventListener(this, toucheventselector(PopUpUI::playTableDiceBtnCallBack));
        
        for (int i = 0;  i < 3;  i++)
        {
            CCString* path = CCString::createWithFormat("Label_%d",i);
            UILabel * label = static_cast<UILabel *>(pBtn->getChildByName(path->getCString()));
            CC_BREAK_IF(!label);
            label->setVisible(false);
        }
        
        // 玩家拥有宝石数量
        UIImageView* jewImg = static_cast<UIImageView *>(panel_e->getChildByName("ImageView_jew"));
        CC_BREAK_IF(!jewImg);
        UILabel* label_0 = static_cast<UILabel *>(jewImg->getChildByName("Label_0"));
        CC_BREAK_IF(!label_0);
        CC_BREAK_IF(!m_pRoleByControlled);
        CCString* strJew = CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel());
        label_0->setText(strJew->getCString());
        
        // 玩法按钮回调
        UIButton* pIntroBtn = static_cast<UIButton *>(panel_e->getChildByName("Button_0"));
        CC_BREAK_IF(!pIntroBtn);
        pIntroBtn->addTouchEventListener(this, toucheventselector(PopUpUI::playTableDiceIntroduceCallBack));
        
        // 玩法按钮确定回调
        UIImageView* imageView_0 = static_cast<UIImageView * >(pIntroBtn->getChildByName("ImageView_0"));
        CC_BREAK_IF(!imageView_0);
        UIButton* pIntroConfirmBtn = static_cast<UIButton *>(imageView_0->getChildByName("Button_0"));
        CC_BREAK_IF(!pIntroConfirmBtn);
        pIntroConfirmBtn->addTouchEventListener(this, toucheventselector(PopUpUI::playTableDiceIntroduceConfirmCallBack));
        pIntroConfirmBtn->setEnabled(false);
        pIntroConfirmBtn->setTouchEnabled(false);
        
        // 数据初始化
        m_arrDiceData = new CCArray();
        
        UIImageView* img0_0 = static_cast<UIImageView *>(panel_0->getChildByName("ImageView_0_0"));
        CC_BREAK_IF(!img0_0);
        UIImageView* img0 = static_cast<UIImageView *>(img0_0->getChildByName("ImageView_0"));
        CC_BREAK_IF(!img0);
        
        // 复制选中框
        m_pDiceMoveImgView = static_cast<UIImageView *>(img0->clone());
        panel_0->addChild(m_pDiceMoveImgView);
        CC_BREAK_IF(!m_pDiceMoveImgView);
        m_pDiceMoveImgView->setVisible(false);
        img0->removeFromParentAndCleanup(true);
        
        //摇色子点数静态图片
        m_pDiceDciePointPic = YYSprite::createWithActionSet("dice", "dice", kDirTypeEffect);
        m_pDiceDciePointPic->setZOrder(20);
        m_pDiceDciePointPic->setScale(0.8f);
        panel_e->addNode(m_pDiceDciePointPic);
        CCSize size = panel_e->getContentSize();
        m_pDiceDciePointPic->setPosition(ccp(size.width * 0.5f,size.height * 0.5f));
        m_pDiceDciePointPic->setVisible(false);
        
    } while (0);
}

#pragma mark 摇骰子按钮回调

void PopUpUI::playTableDiceBtnCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        CC_BREAK_IF(!m_bPlayTableBtnTouch); //UI 播放 按钮不可点击
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            switch (m_nDiceBtnState)
            {
                case 0:    //免费投骰
                {
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_DICE_THROW))->getCString(),true);
                }
                    break;
                case 1:   //收取
                {
                    Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_DICE_RECEIVE))->getCString(),true);
                }
                    break;
                case 2:   //宝石投骰
                {
                    CC_BREAK_IF(!m_pRoleByControlled);
                    if(m_pRoleByControlled->getSpriteAttribute()->getJewel() > m_nDiceCousumeJewCount)
                    {
                        Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_DICE_THROW))->getCString(),true);
                    }
                    else
                    {
                        ((YYScene*)getParent())->showTips(g_sLackJew);
                    }
                }
                    break;
                default:
                    break;
            }
        }
    } while (0);
}


#pragma mark 更新骰子页面信息

void PopUpUI::updatePlayTableTD(CCArray * arr)
{
    //返回 TD_[当前所在格子]_[已投掷次数]_下次花费钻石_[当前奖励(没有时为0)]_[活动所有奖励(共18个)]   //花费宝石为0时 为免费投骰
    do {
        CC_BREAK_IF(!arr);
        int index = 0;
        std::string pStr;
        m_arrDiceData->removeAllObjects();
        m_pDiceMoveImgView->setVisible(false);
        
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
        CC_BREAK_IF(!panel_e);
        UIPanel* panel0 = static_cast<UIPanel *>(panel_e->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel0);
        
        //当前所在格数
        pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
        m_nDiceStartGrid = atoi(pStr.c_str());
        m_nDiceEndGrid = m_nDiceStartGrid;
        //已投次数(未使用)
        pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
        
        //花费宝石
        pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
        if (pStr == "0")
        {
            m_nDiceBtnState = 0;
        }
        else
        {
            m_nDiceBtnState = 2;
            m_nDiceCousumeJewCount = atoi(pStr.c_str());
        }
        
        //当前奖励
        pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
        if (pStr != "0")
        {
            m_nDiceBtnState = 1;
            CCArray* tmpArr = split_string(pStr.c_str(), ';');
            UIImageView* img_1 = static_cast<UIImageView *>(panel_e->getChildByName("ImageView_1"));
            CC_BREAK_IF(!img_1);
            img_1->removeAllChildren();
            
            //icon
            pStr = (static_cast<CCString *>(tmpArr->objectAtIndex(2)))->getCString();
            UIImageView* tmpImg = UIImageView::create();
            std::string path = getFilePath(pStr.c_str(), kDirTypeIcon, kFileSuffixPng);
            tmpImg->loadTexture(path.c_str());
            img_1->addChild(tmpImg);
            
            //label
            UILabel* label_0_0 = static_cast<UILabel *>(panel_e->getChildByName("Label_0_0"));
            CC_BREAK_IF(!label_0_0);
            CCString* count =static_cast<CCString *>(tmpArr->objectAtIndex(1));
            CCString* name = static_cast<CCString * >(tmpArr->objectAtIndex(3));
            CCString* content = CCString::createWithFormat("%s X %s",name->getCString(),count->getCString());
            label_0_0->setTextHorizontalAlignment(kCCTextAlignmentCenter);
            label_0_0->setText(content->getCString());
            
        }
   
        // 骰子按钮label 显示
        
        UIButton* pBtn = static_cast<UIButton *>(panel_e->getChildByName("Button_go"));
        CC_BREAK_IF(!pBtn);
        
        for (int i = 0;  i < 3;  i++)
        {
            CCString* path = CCString::createWithFormat("Label_%d",i);
            UILabel * label = static_cast<UILabel *>(pBtn->getChildByName(path->getCString()));
            CC_BREAK_IF(!label);
            if (i == m_nDiceBtnState)
            {
                label->setVisible(true);
                if (m_nDiceBtnState == 2) // 花费宝石
                {
                    label->setText(CCString::createWithFormat("%d",m_nDiceCousumeJewCount)->getCString());
                }
            }
            else
            {
                label->setVisible(false);
            }
        }
        
        //骰子奖励
        for (int i = 0; i < 18; i++)
        {
            CCString* pStrAward = static_cast<CCString *>(arr->objectAtIndex(index++));
            m_arrDiceData->addObject(pStrAward); //缓存数据
            
            CCArray* arrInfo = split_string(pStrAward->getCString(), ';');
            pStr = ((CCString *)arrInfo->objectAtIndex(2))->getCString(); //icon
            pStr = getFilePath(pStr, kDirTypeIcon, kFileSuffixPng);
            
            CCString* strPath = CCString::createWithFormat("ImageView_0_%d",i);
            UIImageView* pImgAward = static_cast<UIImageView *>(panel0->getChildByName(strPath->getCString()));
            CC_BREAK_IF(!pImgAward);
            
            //添加icon
            UIImageView* img = UIImageView::create();
            pImgAward->addChild(img);
            img->loadTexture(pStr.c_str());
            
            //选中框
            if (m_nDiceStartGrid == i)
            {
               m_pDiceMoveImgView->setVisible(true);
               m_pDiceMoveImgView->setPosition(pImgAward->getPosition());
            }
        }
        
    } while (0);
}

#pragma mark 摇骰子信息

void PopUpUI::updatePlayTableDR(CCArray * arr)
{
//    throw T 投掷色子
//    返回DR_[投掷的色子点数(1-6)]_[走到第几格]_[投掷了几次]_[下次花费钻石]
//    
//    throw R 领取奖励
//    返回DR_-1_-1_[投掷了几次]_[下次花费钻石]  表示奖励领取成功
    do {
        CC_BREAK_IF(!arr);
        int index = 0;
        std::string pStr;
        
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
        CC_BREAK_IF(!panel_e);
        
        // 更新玩家宝石数量
        UIImageView* jewImg = static_cast<UIImageView *>(panel_e->getChildByName("ImageView_jew"));
        CC_BREAK_IF(!jewImg);
        UILabel* label_0 = static_cast<UILabel *>(jewImg->getChildByName("Label_0"));
        CC_BREAK_IF(!label_0);
        CC_BREAK_IF(!m_pRoleByControlled);
        CCString* strJew = CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel());
        label_0->setText(strJew->getCString());
        
        CCString* cStr = static_cast<CCString *>(arr->objectAtIndex(index++));
        pStr = cStr->getCString();
        if (pStr == "-1") //领取奖励
        {

            UIImageView* img1 = static_cast<UIImageView *>(panel_e->getChildByName("ImageView_1"));
            CC_BREAK_IF(!img1);
            
            img1->removeAllChildren();
            
            //label置空
            
            UILabel* label0_0 = static_cast<UILabel *>(panel_e->getChildByName("Label_0_0"));
            CC_BREAK_IF(!label0_0);
            label0_0->setText(" ");
            
            //获取奖励特效
            YYSprite *sprite = YYSprite::createWithActionSet("se5", "se5", kDirTypeEffect);
            sprite->setAction(0, false);
            img1->addNode(sprite);
            m_pSpecialEffectArray->addObject(sprite);
            
            //投骰花费宝石
            pStr = static_cast<CCString *>(arr->objectAtIndex(3))->getCString();
            m_nDiceCousumeJewCount = atoi(pStr.c_str());
            
            //骰子按钮状态变化
            if (m_nDiceCousumeJewCount > 0)
            {
                m_nDiceBtnState = 2;  //宝石投骰
            }
            else
            {
                m_nDiceBtnState = 0; //免费投骰
            }
            // 骰子按钮
            
            UIButton* pBtn = static_cast<UIButton *>(panel_e->getChildByName("Button_go"));
            CC_BREAK_IF(!pBtn);
            
            for (int i = 0;  i < 3;  i++)
            {
                CCString* path = CCString::createWithFormat("Label_%d",i);
                UILabel * label = static_cast<UILabel *>(pBtn->getChildByName(path->getCString()));
                CC_BREAK_IF(!label);
                if (i == m_nDiceBtnState)
                {
                    label->setVisible(true);
                    if (m_nDiceBtnState == 2) // 花费宝石
                    {
                        label->setText(CCString::createWithFormat("%d",m_nDiceCousumeJewCount)->getCString());
                    }
                }
                else
                {
                    label->setVisible(false);
                }
            }
            
            //骰子图片隐藏
            if (m_pDiceDciePointPic)
            {
                m_pDiceDciePointPic->setVisible(false);
            }
            
        }
        else  //返回DR_[投掷的色子点数(1-6)]_[走到第几格]_[投掷了几次]_[下次花费钻石]
        {
            m_nDicePoint = atoi(pStr.c_str());  //骰子点数
            pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
            m_nDiceStartGrid = m_nDiceEndGrid;   //获取当前个数
            m_nDiceEndGrid = atoi(pStr.c_str()); //走到第几格
            pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString(); // 投掷次数暂时不用
            pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
            m_nDiceCousumeJewCount = atoi(pStr.c_str()); // 下次花费钻石
            
            m_nDiceBtnState = 1; // 置为领取状态
            
            playTableDiceRoateDiceUI();
        }
        
    }while (0);
}

#pragma mark 玩法介绍回调

void PopUpUI::playTableDiceIntroduceCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        CC_BREAK_IF(!m_bPlayTableBtnTouch); //UI 播放 按钮不可点击
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            
            m_bPlayTableBtnTouch = false;
            // UI 初始化
            CC_BREAK_IF(!m_pUITouchGroup);
            UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
            CC_BREAK_IF(!panel_e);

            // 玩法按钮
            UIButton* pIntroBtn = static_cast<UIButton *>(panel_e->getChildByName("Button_0"));
            CC_BREAK_IF(!pIntroBtn);
          
            // 玩法按钮确定回调
            UIImageView* imageView_0 = static_cast<UIImageView * >(pIntroBtn->getChildByName("ImageView_0"));
            CC_BREAK_IF(!imageView_0);
            UIButton* pIntroConfirmBtn = static_cast<UIButton *>(imageView_0->getChildByName("Button_0"));
            CC_BREAK_IF(!pIntroConfirmBtn);
            pIntroConfirmBtn->setEnabled(true);
            pIntroConfirmBtn->setTouchEnabled(true);
            imageView_0->setVisible(true);
            
    
        }
    } while (0);

}

#pragma mark 玩法介绍确认按钮回调

void PopUpUI::playTableDiceIntroduceConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());

        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            m_bPlayTableBtnTouch = true;
            
            // UI 初始化
            CC_BREAK_IF(!m_pUITouchGroup);
            UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
            CC_BREAK_IF(!panel_e);
            
            // 玩法按钮
            UIButton* pIntroBtn = static_cast<UIButton *>(panel_e->getChildByName("Button_0"));
            CC_BREAK_IF(!pIntroBtn);
            
            // 玩法按钮确定回调
            UIImageView* imageView_0 = static_cast<UIImageView * >(pIntroBtn->getChildByName("ImageView_0"));
            CC_BREAK_IF(!imageView_0);
            UIButton* pIntroConfirmBtn = static_cast<UIButton *>(imageView_0->getChildByName("Button_0"));
            CC_BREAK_IF(!pIntroConfirmBtn);
            pIntroConfirmBtn->setEnabled(false);
            pIntroConfirmBtn->setTouchEnabled(false);
            imageView_0->setVisible(false);
            
        }
    } while (0);
}

#pragma mark 摇骰子UI动画

void PopUpUI::playTableDiceRoateDiceUI()
{
    do
    {
        //播放动画 按钮不可点击
        m_bPlayTableBtnTouch = false;
        
        //UI 初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
        CC_BREAK_IF(!panel_e);
        
        //骰子动画显示
        YYSprite *sprite = YYSprite::createWithActionSet("dice", "dice", kDirTypeEffect);
        sprite->setAction(0, false);
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        sprite->setPosition(ccp(size.width * 0.5f,size.height * 0.45f));
        sprite->setZOrder(20);
        sprite->setScale(0.8f);
        this->addChild(sprite);
        sprite->convertToNodeSpace(sprite->getPosition());
        m_pSpecialEffectArray->addObject(sprite);
        
        float fDiceAniTime = 0.75f;             //骰子动画播放时间

        //骰子静态图片显示
        m_pDiceDciePointPic->setAction(m_nDicePoint, true);
        CCSequence* seq = CCSequence::create(CCHide::create(),CCDelayTime::create(fDiceAniTime),CCShow::create(),NULL);
        m_pDiceDciePointPic->runAction(seq);
        
        //骰子动画播放完 调用选框移动
        CCDelayTime* delay = CCDelayTime::create(fDiceAniTime);
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(PopUpUI::playTableDiceIconMove));
        CCSequence*  seq2 = CCSequence::create(delay, callFunc, NULL);
        this->runAction(seq2);
        
        
    } while (0);

}

#pragma mark 摇骰子选中框移动

void PopUpUI::playTableDiceIconMove()
{
    do
    {
        //UI 初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
        CC_BREAK_IF(!panel_e);
        UIPanel* panel0 = static_cast<UIPanel *>(panel_e->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel0);
        
        //获取移动位置集合
        vector<CCPoint > vec_poi;
        for (int i = 0; i < 18; i++)
        {
           CCString* strPath = CCString::createWithFormat("ImageView_0_%d",i);
           UIImageView* pImgAward = static_cast<UIImageView *>(panel0->getChildByName(strPath->getCString()));
           CC_BREAK_IF(!pImgAward);
           vec_poi.push_back(pImgAward->getPosition());
        }
        
        CCArray* arrActions = CCArray::create();
        
        if (m_nDiceEndGrid > m_nDiceStartGrid)  // 顺序
        {
            for (int i = m_nDiceStartGrid + 1; i <=m_nDiceEndGrid ; i++)
            {
                CCMoveTo* moveTo = CCMoveTo::create(0.4f, vec_poi[i]);
                arrActions->addObject(moveTo);
            }
        }
        else                                   // 逆序
        {
            for (int i = m_nDiceStartGrid + 1; i <= 17; i++)
            {
                CCMoveTo* moveTo = CCMoveTo::create(0.4f, vec_poi[i]);
                arrActions->addObject(moveTo);
            }
            for (int i = 0; i <= m_nDiceEndGrid; i++)
            {
                CCMoveTo* moveTo = CCMoveTo::create(0.4f, vec_poi[i]);
                arrActions->addObject(moveTo);
            }
        }
        arrActions->addObject(CCDelayTime::create(0.4f));
        CCCallFunc* callFunc = CCCallFunc::create(this, callfunc_selector(PopUpUI::playTableDiceFinish));
        arrActions->addObject(callFunc);
        CCSequence* seq = CCSequence::create(arrActions);
        m_pDiceMoveImgView->runAction(seq);

    } while (0);
}

#pragma mark 摇骰子动画播放结束

void PopUpUI::playTableDiceFinish()
{
    do
    {
        //播放动画结束 按钮可点击
        m_bPlayTableBtnTouch = true;
        
        //UI 初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_e = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_e"));
        CC_BREAK_IF(!panel_e);
        
        //显示物品奖励
        UIImageView* img1 = static_cast<UIImageView *>(panel_e->getChildByName("ImageView_1"));
        CC_BREAK_IF(!img1);
        
        img1->removeAllChildren();
        
        std::string pStr;
        pStr = static_cast<CCString *>(m_arrDiceData->objectAtIndex(m_nDiceEndGrid))->getCString();
        CCArray* tmpArr = split_string(pStr.c_str(), ';');
        
        //icon
        pStr = (static_cast<CCString *>(tmpArr->objectAtIndex(2)))->getCString();
        UIImageView* tmpImg = UIImageView::create();
        std::string path = getFilePath(pStr.c_str(), kDirTypeIcon, kFileSuffixPng);
        tmpImg->loadTexture(path.c_str());
        img1->addChild(tmpImg);
        
        //label
        UILabel* label_0_0 = static_cast<UILabel *>(panel_e->getChildByName("Label_0_0"));
        CC_BREAK_IF(!label_0_0);
        CCString* count =static_cast<CCString *>(tmpArr->objectAtIndex(1));
        CCString* name = static_cast<CCString * >(tmpArr->objectAtIndex(3));
        CCString* content = CCString::createWithFormat("%s X %s",name->getCString(),count->getCString());
        label_0_0->setTextHorizontalAlignment(kCCTextAlignmentCenter);
        label_0_0->setText(content->getCString());

        //获取奖励特效
        YYSprite *sprite = YYSprite::createWithActionSet("se2", "se2", kDirTypeEffect);
        sprite->setAction(0, false);
        img1->addNode(sprite);
        m_pSpecialEffectArray->addObject(sprite);
        
        // 骰子按钮置为领取
        
        UIButton* pBtn = static_cast<UIButton *>(panel_e->getChildByName("Button_go"));
        CC_BREAK_IF(!pBtn);
        
        for (int i = 0;  i < 3;  i++)
        {
            CCString* path = CCString::createWithFormat("Label_%d",i);
            UILabel * label = static_cast<UILabel *>(pBtn->getChildByName(path->getCString()));
            CC_BREAK_IF(!label);
            if (i == m_nDiceBtnState)
            {
                label->setVisible(true);
            }
            else
            {
                label->setVisible(false);
            }
        }
    } while (0);

}

#pragma mark 初始化地下城UI

void PopUpUI::initPlayTableDungeonUI()
{
    do
    {
        // UI 初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_a = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_a"));
        CC_BREAK_IF(!panel_a);
        
        //选中框
        for (int i = 0;  i < 3;  i++)
        {
            //背景选框
            CCString* path = CCString::createWithFormat("ImageView_%d",i);
            UIImageView* imageView = static_cast<UIImageView *>(panel_a->getChildByName(path->getCString()));
            CC_BREAK_IF(!imageView);
            imageView->addTouchEventListener(this, toucheventselector(PopUpUI::playTableDungeonSelectCallBack));
            imageView->setTag(i);
            imageView->setTouchEnabled(true);
            
            //挑战
            UIButton* btn_0 = static_cast<UIButton *>(imageView->getChildByName("Button_0"));
            CC_BREAK_IF(!btn_0);
            btn_0->addTouchEventListener(this, toucheventselector(PopUpUI::playTableDungeonChallengeCallBack));
            btn_0->setTag(i);
            
            //重置
            UIButton* btn_1 = static_cast<UIButton *>(imageView->getChildByName("Button_1"));
            CC_BREAK_IF(!btn_1);
            btn_1->addTouchEventListener(this, toucheventselector(PopUpUI::playTableDungeonResetCallBack));
            btn_1->setTag(i);
            
        }
        
        //屏蔽免费进入次数
        UILabel* label_1 = static_cast<UILabel *>(panel_a->getChildByName("Label_1"));
        CC_BREAK_IF(!label_1);
        label_1->setVisible(false);
    
        // 数据初始化
        m_arrDungeonResetTicket = new int[3];  // 地下城重置所需门票个数
        m_bDungeonEnable = new bool[3];        // 地下城可否挑战
        m_arrDungeonNeedPlayLevel = new int[3];  // 地下城所需玩家等级
        m_bDUngeonFirstLogin = new bool[3];      // 是否是每天第一次进入
        m_arrDungeonResetCnt = new int[3];       // 每天重置上限
        for (int i = 0; i < 3; i++)
        {
            m_arrDungeonResetTicket[i]   = 0;
            m_arrDungeonNeedPlayLevel[i] = 0;
            m_bDungeonEnable[i] = false;
            m_bDUngeonFirstLogin[i] = false;
            m_arrDungeonResetCnt[i] = 0;
        }
        
        m_DungeonHaveTicket  = 0;
        m_DungeonSelectLevel = 0; // 0 1 2 初 中 高
    
    }while (0);

}

#pragma mark 更新地下城页面信息

void PopUpUI::updateDungeonDO(CCArray * arr)
{
// dungeon L  打开地下城
// 返回 DO_[门票剩余数量]_[是否通关该难度（0未通关，1已通关）];[重置初级难度需要的门票];[开启初级需要的等级];[是否为打通全部关卡状态 0 是 关卡ID 不是];[是否可以重置 0 可以重置 >0每天最多重置次数]...
    do
    {
        CC_BREAK_IF(!arr);
        int index = 0;
        std::string pStr;
        
        CC_BREAK_IF(!m_pRoleByControlled);
        int nCurJew = m_pRoleByControlled->getSpriteAttribute()->getJewel(); // 宝石
        int nPlayerLevel = m_pRoleByControlled->getLevel(); // 等级

        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_a = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_a"));
        CC_BREAK_IF(!panel_a);
        
        // 宝石数量
        UIImageView* imgView_jew = static_cast<UIImageView *>(panel_a->getChildByName("ImageView_jew"));
        CC_BREAK_IF(!imgView_jew);
        UILabel* label_jew = static_cast<UILabel *>(imgView_jew->getChildByName("Label_0"));
        CC_BREAK_IF(!label_jew);
        label_jew->setText(CCString::createWithFormat("%d",nCurJew)->getCString());
        
        // 拥有门票数量
        pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
        m_DungeonHaveTicket = atoi(pStr.c_str());
        UILabel* label_ticket = static_cast<UILabel *>(panel_a->getChildByName("Label_0"));
        CC_BREAK_IF(!label_ticket);
        UILabel* label_ticket0 = static_cast<UILabel *>(label_ticket->getChildByName("Label_0"));
        CC_BREAK_IF(!label_ticket0);
        label_ticket0->setText(pStr.c_str());
  
        // 3个ImageView
        int nPass[3] = {0,0,0};
        for (int i = 0;  i < 3;  i++)
        {
            // 背景选框
            CCString* path = CCString::createWithFormat("ImageView_%d",i);
            UIImageView* imageView = static_cast<UIImageView *>(panel_a->getChildByName(path->getCString()));
            CC_BREAK_IF(!imageView);
        
            pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
            CCArray* arrData = split_string(pStr.c_str(), ';');
            pStr = (static_cast<CCString *>(arrData->objectAtIndex(0)))->getCString();
            nPass[i] = atoi(pStr.c_str());  // 是否通关该难度（0未通关，1已通关）
            pStr = (static_cast<CCString *>(arrData->objectAtIndex(1)))->getCString();
            m_arrDungeonResetTicket[i] = atoi(pStr.c_str());  // 重置需要的门票
            pStr = (static_cast<CCString *>(arrData->objectAtIndex(2)))->getCString();
            m_arrDungeonNeedPlayLevel[i] = atoi(pStr.c_str());  // 开启初级需要的等级
            pStr = (static_cast<CCString *>(arrData->objectAtIndex(3)))->getCString();
            int nPassRound = atoi(pStr.c_str());  // 是否为打通关状态
            pStr = (static_cast<CCString *>(arrData->objectAtIndex(4)))->getCString();
            m_arrDungeonResetCnt[i] = atoi(pStr.c_str());  // 重置次数 (0 可以重置 > 0 不可重置 重置上限次数)
            
            switch (i)
            {
                case 0:
                {
                    if (nPlayerLevel < m_arrDungeonNeedPlayLevel[i]) // 玩家等级 < 所需等级
                    {
                        m_bDungeonEnable[i] = false;        // 不可进入
                    }
                    else
                    {
                        m_bDungeonEnable[i] = true;        // 可进入
                        (m_arrDungeonResetTicket[i] == 0) ? (m_bDUngeonFirstLogin[i] = true) : (m_bDUngeonFirstLogin[i] = false);// 判定是否是第一次进入 重置门票为0
                    }
                }
                    break;
                case 1:
                {
                    if (nPlayerLevel < m_arrDungeonNeedPlayLevel[i] || nPass[0] == 0) // 玩家等级 < 所需等级 初级地下城未通过
                    {
                        m_bDungeonEnable[i] = false;        // 不可进入
                    }
                    else
                    {
                        m_bDungeonEnable[i] = true;        // 可进入
                        (m_arrDungeonResetTicket[i] == 0) ? (m_bDUngeonFirstLogin[i] = true) : (m_bDUngeonFirstLogin[i] = false);//判定是否是第一次进入 重置门票为0
                    }
                }
                    break;
                case 2:
                {
                    if (nPlayerLevel < m_arrDungeonNeedPlayLevel[i] || nPass[0] == 0 || nPass[1] == 0) // 玩家等级 < 所需等级 初,中 级地下城未通过
                    {
                        m_bDungeonEnable[i] = false;        // 不可进入
                    }
                    else
                    {
                        m_bDungeonEnable[i] = true;        // 可进入
                        (m_arrDungeonResetTicket[i] == 0) ? (m_bDUngeonFirstLogin[i] = true) : (m_bDUngeonFirstLogin[i] = false);// 判定是否是第一次进入 重置门票为0
                    }
                }
                    break;
                default:
                    break;
            }
            
            // 设置UI 显示
            int nState = 0;
            if(!m_bDungeonEnable[i])
            {
                nState = 0;
            }
            else
            {
                m_bDUngeonFirstLogin[i] ? nState = 1 : nState = 2;
            }
            
            (nPassRound == 0) ? nState = 3 : nState = nState;  //玩家打通全部关卡 挑战置灰 需要强制重置
            
            setPlayTableDungeonImageViewState(imageView,nState);
        }
    } while (0);
}

#pragma mark 设置地下城页面初级 中级 高级 页面状态

void PopUpUI::setPlayTableDungeonImageViewState(UIImageView* imageView, int state)
{
    do
    {
        CC_BREAK_IF(!imageView);
        
        // 挑战
        UIButton* btn_0 = static_cast<UIButton *>(imageView->getChildByName("Button_0"));
        CC_BREAK_IF(!btn_0);
        btn_0->setTouchEnabled(false); // 默认不可点击
        btn_0->setBright(true);
        
        // 开始挑战label
        UILabel* label0= static_cast<UILabel *>(btn_0->getChildByName("Label_0"));
        CC_BREAK_IF(!label0);
        
        // 继续挑战label
        UILabel* label1= static_cast<UILabel *>(btn_0->getChildByName("Label_1"));
        CC_BREAK_IF(!label1);
    
        // 重置
        UIButton* btn_1 = static_cast<UIButton *>(imageView->getChildByName("Button_1"));
        CC_BREAK_IF(!btn_1);
        btn_1->setTouchEnabled(false); // 默认不可点击
        
        // 置灰蒙版
        UIImageView* img_Mask = static_cast<UIImageView *>(imageView->getChildByName("ImageView_2"));
        CC_BREAK_IF(!img_Mask);
        
        // 选中框
        UIImageView* img_Select = static_cast<UIImageView *>(imageView->getChildByName("ImageView_1"));
        CC_BREAK_IF(!img_Select);
        
        // 等级label
        UILabel * lab_level  = static_cast<UILabel *>(imageView->getChildByName("Label_1"));
        CC_BREAK_IF(!lab_level);
        int nLevel = m_arrDungeonNeedPlayLevel[imageView->getTag()];
        CCString* strLevel= CCString::createWithFormat("%d",nLevel);
        lab_level->setText(strLevel->getCString());
        
        switch (state)
        {
            case 0: // 不可挑战
            {
                btn_1->setVisible(false);   // 重置隐藏
                label0->setVisible(true);   // 开始挑战label
                label1->setVisible(false);  // 继续挑战label
                img_Mask->setVisible(true); // 置灰蒙版
                img_Select->setVisible(false); // 选中框
            }
                break;
            case 1: // 首次挑战
            {
                btn_1->setVisible(false);   // 重置隐藏
                label0->setVisible(true);   // 开始挑战label
                label1->setVisible(false);  // 继续挑战label
                img_Mask->setVisible(false); // 置灰蒙版
                img_Select->setVisible(false); // 选中框
            }
                break;
            case 2: // 第二次挑战 可重置
            {
                btn_1->setVisible(true);      // 重置显示
                label0->setVisible(false);   // 开始挑战label
                label1->setVisible(true);    // 继续挑战label
                img_Mask->setVisible(false); // 置灰蒙版
                img_Select->setVisible(false); // 选中框
            }
                break;
            case 3: // 通过全部关卡 挑战按钮被置灰 不可点击 只可重置
            {
                btn_1->setVisible(true);      // 重置显示
                label0->setVisible(true);   // 开始挑战label
                label1->setVisible(false);    // 继续挑战label
                img_Mask->setVisible(false); // 置灰蒙版
                img_Select->setVisible(false); // 选中框
                btn_0->setBright(false);      // 开始挑战按钮置灰
            }
                break;
            default:
                break;
        }
    } while (0);
}

#pragma mark 更新地下城战斗结果信息

void PopUpUI::updateDungeonDA(CCArray * arr)
{
    setPlayTableFightResult(arr, kDungeonResult);
}

#pragma mark 地下城选中框回调

void PopUpUI::playTableDungeonSelectCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            //重置按钮点击状态
            {
                CC_BREAK_IF(!m_pUITouchGroup);
                UIPanel* panel_a = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_a"));
                CC_BREAK_IF(!panel_a);
                
                //选中框
                for (int i = 0;  i < 3;  i++)
                {
                    // 背景选框
                    CCString* path = CCString::createWithFormat("ImageView_%d",i);
                    UIImageView* imageView = static_cast<UIImageView *>(panel_a->getChildByName(path->getCString()));
                    CC_BREAK_IF(!imageView);
                    
                    // 挑战
                    UIButton* btn_0 = static_cast<UIButton *>(imageView->getChildByName("Button_0"));
                    CC_BREAK_IF(!btn_0);
                    btn_0->setTouchEnabled(false);
                    
                    // 重置
                    UIButton* btn_1 = static_cast<UIButton *>(imageView->getChildByName("Button_1"));
                    CC_BREAK_IF(!btn_1);
                    btn_1->setTouchEnabled(false);
                    
                    // 隐藏选中框
                    UIImageView* img_Select = static_cast<UIImageView *>(imageView->getChildByName("ImageView_1"));
                    CC_BREAK_IF(!img_Select);
                    img_Select->setVisible(false);
                    
                }
            }
            
            UIImageView* image = static_cast<UIImageView *>(pSender);
            CC_BREAK_IF(!image);
            int nTag = image->getTag();
            
            if(m_bDungeonEnable[nTag])  // 可以进入
            {
               //显示选中框
                UIImageView* img_Select = static_cast<UIImageView *>(image->getChildByName("ImageView_1"));
                CC_BREAK_IF(!img_Select);
                img_Select->setVisible(true);
                
                
                if(m_bDUngeonFirstLogin[nTag]) // 首次挑战
                {
                    // 挑战
                    UIButton* btn_0 = static_cast<UIButton *>(image->getChildByName("Button_0"));
                    CC_BREAK_IF(!btn_0);
                    btn_0->setTouchEnabled(true);
                }
                else                           // 第二次挑战
                {
                    // 挑战
                    UIButton* btn_0 = static_cast<UIButton *>(image->getChildByName("Button_0"));
                    CC_BREAK_IF(!btn_0);
                    btn_0->setTouchEnabled(true);
                    
                    bool bBright = btn_0->isBright(); //如果挑战被置灰
                    if (!bBright)
                    {
                        btn_0->setTouchEnabled(false);
                    }
                    
                    // 重置
                    UIButton* btn_1 = static_cast<UIButton *>(image->getChildByName("Button_1"));
                    CC_BREAK_IF(!btn_1);
                    btn_1->setTouchEnabled(true);
                }
            }
            else    // 不可挑战 提示信息
            {
                CCString* tips = CCString::createWithFormat(g_sPlayTableDungeonTipMsgStr[nTag],m_arrDungeonNeedPlayLevel[nTag]);
                ((YYScene*)getParent())->showTips(tips->getCString());
            }
            
        }
    }while (0);
}

#pragma mark 挑战地下城回调

void PopUpUI::playTableDungeonChallengeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            UIButton* btn = static_cast<UIButton *>(pSender);
            CC_BREAK_IF(!btn);
            int nTag = btn->getTag();
            CCString* msg = CCString::createWithFormat(ASK_DUNGEON_CHALLENGE,nTag);
            Controller::getInstance()->sendMessage(msg->getCString(),true);
            Controller::getInstance()->switchGameStatus(kStatusLoading);
            Controller::getInstance()->setDungeonType(kDungeonDungeon); //设置进入副本类型
        }
    }while (0);
}

#pragma mark  重置地下城回调

void PopUpUI::playTableDungeonResetCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            UIButton* btn = static_cast<UIButton *>(pSender);
            CC_BREAK_IF(!btn);
            int nTag = btn->getTag();
            m_DungeonSelectLevel = nTag;  //地下城选择等级
            int nResetTickets = m_arrDungeonResetTicket[nTag];
            int nResetLimit = m_arrDungeonResetCnt[nTag];
            if (nResetLimit > 0)  // 达到每天重置最大上限 不可重置
            {
                CCString* tips = CCString::createWithFormat(g_sPlayTableDungeonResetLimit,nResetLimit);
                ((YYScene*)getParent())->showTips(tips->getCString());
            }
            else if (m_DungeonHaveTicket < nResetTickets) // 重置门票不足
            {
                ((YYScene*)getParent())->showTips(g_sPlayTableDungeonResetLackMsgStr);
            }
            else
            {
                CCString* tips = CCString::createWithFormat(g_sPlayTableDungeonResetMsgStr,nResetTickets);
                ((YYScene*)getParent())->showTips(tips->getCString(),kTipPlayTableDungeonReset);
            }
        }
    }while (0);
}

#pragma mark 初始化保卫星灵UI

void PopUpUI::initPlayTableProtectProtossUI()
{
    do
    {
        // UI初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_b = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_b"));
        CC_BREAK_IF(!panel_b);
        
        // 选中框
        for (int i = 0;  i < 3;  i++)
        {
            // 背景选框
            CCString* path = CCString::createWithFormat("ImageView_%d",i);
            UIImageView* imageView = static_cast<UIImageView *>(panel_b->getChildByName(path->getCString()));
            CC_BREAK_IF(!imageView);
            imageView->addTouchEventListener(this, toucheventselector(PopUpUI::playTableProtectProtossSelectCallBack));
            imageView->setTag(i);
            imageView->setTouchEnabled(true);
            
            // 挑战
            UIButton* btn_0 = static_cast<UIButton *>(imageView->getChildByName("Button_0"));
            CC_BREAK_IF(!btn_0);
            btn_0->addTouchEventListener(this, toucheventselector(PopUpUI::playTableProtectProtossChallengeCallBack));
            btn_0->setTag(i);
            btn_0->setTouchEnabled(true);
        }
        
        // 数据初始化
        m_arrProtossNeedPlayLevel = new int[3]; // 守卫星灵所需玩家等级
        m_bProtectProtossFinish = new bool[3];  // 守卫星灵是否已完成
        m_arrProtectProtossWipe = new int[3]; // 守卫星灵扫荡
        for (int i = 0; i < 3; i++)
        {
            m_arrProtossNeedPlayLevel[i]   = 0;
            m_bProtectProtossFinish[i] = false;
            m_arrProtectProtossWipe[i] = -1;
        }
        m_nProtectProtossDifficulty = 0;
        
    } while (0);
}

#pragma mark 守卫星灵 UI数据更新

void PopUpUI::updateProtectProtossTO(CCArray * arr)
{
//  tower L 打开守卫星灵
//  返回TO_[开启等级;所在层数;是否可以扫荡(可以扫荡可扫荡关卡数量，否则-1)]_[开启等级;所在层数;是否可以扫荡(可以扫荡可扫荡关卡数量，否则-1)]_[开启等级;所在层数;是否可以扫荡(可以扫荡可扫荡关卡数量，否则-1)]
//  所在层数: 0 表示已通关, 其他 %1000+1 返回层数
    do
    {
        CC_BREAK_IF(!arr);
        int index = 0;
        std::string pStr;
        
        CC_BREAK_IF(!m_pRoleByControlled);
        int nCurJew = m_pRoleByControlled->getSpriteAttribute()->getJewel(); // 宝石
        int nPlayerLevel = m_pRoleByControlled->getLevel(); // 等级
        
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_b = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_b"));
        CC_BREAK_IF(!panel_b);
        
        // 宝石数量
        UIImageView* imgView_jew = static_cast<UIImageView *>(panel_b->getChildByName("ImageView_jew"));
        CC_BREAK_IF(!imgView_jew);
        UILabel* label_jew = static_cast<UILabel *>(imgView_jew->getChildByName("Label_0"));
        CC_BREAK_IF(!label_jew);
        label_jew->setText(CCString::createWithFormat("%d",nCurJew)->getCString());
        

        // 3个ImageView
        for (int i = 0;  i < 3;  i++)
        {
            // 背景选框
            CCString* path = CCString::createWithFormat("ImageView_%d",i);
            UIImageView* imageView = static_cast<UIImageView *>(panel_b->getChildByName(path->getCString()));
            CC_BREAK_IF(!imageView);
            
            // 置灰面板
            UIImageView* imageView_2 = static_cast<UIImageView *>(imageView->getChildByName("ImageView_2"));
            CC_BREAK_IF(!imageView_2);
            
            // 选中框隐藏
            UIImageView* imageView_1 = static_cast<UIImageView *>(imageView->getChildByName("ImageView_1"));
            CC_BREAK_IF(!imageView_1);
            imageView_1->setVisible(false);
            
            // 挑战按钮
            UIButton* btn = static_cast<UIButton *>(imageView->getChildByName("Button_0"));
            CC_BREAK_IF(!btn);
            btn->setTouchEnabled(false);
            
            // 已完成层数
            UILabel* label_1 = static_cast<UILabel *>(imageView->getChildByName("Label_1"));
            UILabel* label_5 = static_cast<UILabel *>(imageView->getChildByName("Label_5"));
            UILabel* label_6 = static_cast<UILabel *>(imageView->getChildByName("Label_6"));
            UILabel* label_7 = static_cast<UILabel *>(imageView->getChildByName("Label_7"));
            
            // 默认显示即将进行层数
            label_5->setVisible(true);
            label_6->setVisible(true);
            label_7->setVisible(true);
  
            pStr = (static_cast<CCString *>(arr->objectAtIndex(index++)))->getCString();
            CCArray* arrData = split_string(pStr.c_str(), ';');
            m_arrProtossNeedPlayLevel[i] = (static_cast<CCString *>(arrData->objectAtIndex(0)))->intValue();  // 开启所需等级
            label_1->setText((static_cast<CCString *>(arrData->objectAtIndex(0)))->getCString());
            if(nPlayerLevel < m_arrProtossNeedPlayLevel[i])
            {
                imageView_2->setVisible(true);
                label_5->setVisible(false);
                label_6->setVisible(false);
                label_7->setVisible(false);
            }
            else
            {
                imageView_2->setVisible(false);
            }
 
            int nLayer = (static_cast<CCString *>(arrData->objectAtIndex(1)))->intValue(); // 当前所在层
            if (nLayer == 0) // 已完成
            {
                m_bProtectProtossFinish[i] = true;
                btn->setTitleText(g_sPlayTableProtossBtnState[2]);
                btn->setBright(false);
                label_5->setVisible(false);
                label_6->setVisible(false);
                label_7->setVisible(false);
            }
            else
            {
                m_bProtectProtossFinish[i] = false;
                btn->setBright(true);
                nLayer = (nLayer % 1000) + 1;
                label_6->setText((CCString::createWithFormat("%d",nLayer))->getCString());
                if (nLayer == 1) // 开始挑战
                {
                     btn->setTitleText(g_sPlayTableProtossBtnState[0]);
                }
                else // 继续挑战
                {
                     btn->setTitleText(g_sPlayTableProtossBtnState[1]);
                }
            }
            // 扫荡数据
            m_arrProtectProtossWipe[i] = (static_cast<CCString *>(arrData->objectAtIndex(2)))->intValue();
        }
    } while (0);
}

#pragma mark 守卫星灵 战斗结果

void PopUpUI::updateProtectProtossTW(CCArray * arr)
{
    setPlayTableFightResult(arr, kProtossResult);
}

#pragma mark 守卫星灵 选择框回调

void PopUpUI::playTableProtectProtossSelectCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            //重置按钮点击状态
            {
                CC_BREAK_IF(!m_pUITouchGroup);
                UIPanel* panel_b = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_b"));
                CC_BREAK_IF(!panel_b);
                
                //选中框
                for (int i = 0;  i < 3;  i++)
                {
                    // 背景选框
                    CCString* path = CCString::createWithFormat("ImageView_%d",i);
                    UIImageView* imageView = static_cast<UIImageView *>(panel_b->getChildByName(path->getCString()));
                    CC_BREAK_IF(!imageView);
                    
                    // 挑战
                    UIButton* btn_0 = static_cast<UIButton *>(imageView->getChildByName("Button_0"));
                    CC_BREAK_IF(!btn_0);
                    btn_0->setTouchEnabled(false);
                
                    // 隐藏选中框
                    UIImageView* img_Select = static_cast<UIImageView *>(imageView->getChildByName("ImageView_1"));
                    CC_BREAK_IF(!img_Select);
                    img_Select->setVisible(false);
                }
            }
            
            UIImageView* image = static_cast<UIImageView *>(pSender);
            CC_BREAK_IF(!image);
            int nTag = image->getTag();
            UIImageView* imageView_2 = static_cast<UIImageView *>(image->getChildByName("ImageView_2"));
            if (imageView_2->isVisible())  //等级未达到
            {
                CCString* tips = CCString::createWithFormat(g_sPlayTableProtossTipMsgStr[nTag],m_arrProtossNeedPlayLevel[nTag]);
                ((YYScene*)getParent())->showTips(tips->getCString());
            }
            else if(m_bProtectProtossFinish[nTag]) // 已完成
            {
                ((YYScene*)getParent())->showTips(g_sPlayTableProtossFinish[nTag]);
            }
            else // 开始挑战
            {
                //显示选中框
                UIImageView* img_Select = static_cast<UIImageView *>(image->getChildByName("ImageView_1"));
                CC_BREAK_IF(!img_Select);
                img_Select->setVisible(true);
                
                UIButton* btn_0 = static_cast<UIButton *>(image->getChildByName("Button_0"));
                CC_BREAK_IF(!btn_0);
                btn_0->setTouchEnabled(true);
            }
        }
    }while (0);
}

#pragma mark 守卫星灵 挑战回调

void PopUpUI::playTableProtectProtossChallengeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            UIButton* btn = static_cast<UIButton *>(pSender);
            CC_BREAK_IF(!btn);
            int nTag = btn->getTag();
            m_nProtectProtossDifficulty = nTag; // 难度选择
            if(m_arrProtectProtossWipe[nTag] > 0) // 可以扫荡
            {
                CCString* tipInfo = CCString::createWithFormat(g_sPlayTableProtossWipe,m_arrProtectProtossWipe[nTag]);
                ((YYScene*)getParent())->showTips(tipInfo->getCString(),kTipWipeProtoss);
            }
            else
            {
                CCString* msg = CCString::createWithFormat(ASK_PROTECT_PROTOSS_CHALLENGE,nTag);
                Controller::getInstance()->sendMessage(msg->getCString(),true);
                Controller::getInstance()->switchGameStatus(kStatusLoading);
                Controller::getInstance()->setDungeonType(kDungeonProtoss); //设置进入副本类型
            }
        }
    }while (0);
}

#pragma mark boss副本UI 初始化

void PopUpUI::initPlayTableBossDungeonUI()
{
    do
    {
        // UI初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
        CC_BREAK_IF(!panel_c);
        
        // pageView 滑动回调
        UIPageView* pageView = static_cast<UIPageView *>(panel_c->getChildByName("PageView_0"));
        CC_BREAK_IF(!pageView);
        pageView->setTouchEnabled(true);
        pageView->addEventListenerPageView(this, pagevieweventselector(PopUpUI::playTableBossDungeonPageViewEvent));
        
        // 挑战按钮回调
        UIPanel* panel_0 = static_cast<UIPanel *>(panel_c->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel_0);
        panel_0->removeFromParent();
        for (int i = 0;  i < 4;  i++)
        {
            UILayout* layout = static_cast<UILayout*>(panel_0->clone());
            pageView->addPage(layout);
            for (int j = 0; j < 8; j++)
            {
                CCString* path = CCString::createWithFormat("ImageView%d",j);
                UIImageView* imageView = static_cast<UIImageView *>(layout->getChildByName(path->getCString()));
                CC_BREAK_IF(!imageView);
                imageView->addTouchEventListener(this, toucheventselector(PopUpUI::playTableBossDungeonChallengeCallBack));
                imageView->setTag(i*8 + j); // 设置tag唯一值 获取数据使用
                imageView->setTouchEnabled(true);
            }
        }
        
        // 购买回调
        UIButton* btnBuy = static_cast<UIButton *>(panel_c->getChildByName("Button_buy"));
        CC_BREAK_IF(!btnBuy);
        btnBuy->setTouchEnabled(true);
        btnBuy->addTouchEventListener(this, toucheventselector(PopUpUI::playTableBossDungeonBuyCallBack));
        
        // 进入副本回调
        UIPanel* panel_1 = static_cast<UIPanel *>(panel_c->getChildByName("Panel_1"));
        CC_BREAK_IF(!panel_1);
        UIButton* btnIn = static_cast<UIButton *>(panel_1->getChildByName("Button1"));
        CC_BREAK_IF(!btnIn);
        btnIn->setTouchEnabled(true);
        btnIn->addTouchEventListener(this, toucheventselector(PopUpUI::playTableBossDungeonIntoDungeonCallBack));
        
        // 扫荡副本回调
        UIButton* btnWipe = static_cast<UIButton *>(panel_1->getChildByName("Button2"));
        CC_BREAK_IF(!btnWipe);
        btnWipe->setTouchEnabled(true);
        btnWipe->addTouchEventListener(this, toucheventselector(PopUpUI::playTableBossDungeonWipeCallBack));
        
        // 二级页面关闭
        UIButton* btnClose = static_cast<UIButton *>(panel_1->getChildByName("Button0"));
        CC_BREAK_IF(!btnClose);
        btnClose->setTouchEnabled(true);
        btnClose->addTouchEventListener(this, toucheventselector(PopUpUI::playTableBossDungeonCloseDungeonCallBack));
        
        // 屏蔽二级页面点击
        panel_1->setTouchEnabled(false);
        
        // 设置二级页面icon背景不可点击
        UIImageView* imageView2 = static_cast<UIImageView *>(panel_1->getChildByName("ImageView2"));
        CC_BREAK_IF(!imageView2);
        imageView2->setTouchEnabled(false);
  
        // 数据初始化
        m_pUIBossDungeonPageView = pageView;
        m_nUIBossDungeonCurPage = 0;
        m_nUIBossDungeonJewCnt = 0;
        m_nUIBossDungeonResidueCnt = 0;
        m_nUIBossDungeonEnableBuyCnt = 0;
        m_arrUIBossDungeonData = CCArray::create();
        m_arrUIBossDungeonData->retain();
        m_bUIBossDungeonTopTouch = false;
        m_nUIBossDungeonCurSelIdx = 0;
        
        updatePlayTableBossDungeonCurPageUI(); // 更新当前显示页数
  
    } while (0);
}

#pragma mark boss副本UI 更新pageView 圆点 显示页数回调

void PopUpUI::updatePlayTableBossDungeonCurPageUI()
{
    do
    {
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
        CC_BREAK_IF(!panel_c);
        
        for (int i = 0;  i < 4;  i++)
        {
            CCString * path = CCString::createWithFormat("Button%d",i);
            UIButton* btn = static_cast<UIButton *>(panel_c->getChildByName(path->getCString()));
            CC_BREAK_IF(!btn);
            btn->setTouchEnabled(false);
            btn->setBright(true);
            if (i == m_nUIBossDungeonCurPage)
            {
                btn->setBright(false);
            }
        }
        
    }while (0);
}

#pragma mark boss副本UI 列表信息

void PopUpUI::updateplayTableBossDungeonBL(CCArray * arr)
{
    // BL_[剩余挑战总次数]_[购买挑战次数花费钻石数量]_[剩余购买次数]_[前几个关卡可以扫荡]_[id ; icon; 地图名字; 开启等级 ; item1 , item2 ]_...
    do
    {
        CC_BREAK_IF(!arr);
        std::string pStr;
        m_arrUIBossDungeonData->removeAllObjects();
        m_nUIBossDungeonResidueCnt = (static_cast<CCString *>(arr->objectAtIndex(0)))->intValue();
        m_nUIBossDungeonJewCnt = (static_cast<CCString *>(arr->objectAtIndex(1)))->intValue();
        m_nUIBossDungeonEnableBuyCnt = (static_cast<CCString *>(arr->objectAtIndex(2)))->intValue();
        m_nUIBossWipeCnt = (static_cast<CCString *>(arr->objectAtIndex(3)))->intValue();
 
        for (int i = 4; i < arr->count(); i++)
        {
            BossDungeonUIData* data = BossDungeonUIData::create();
            m_arrUIBossDungeonData->addObject(data);
            pStr = (static_cast<CCString *>(arr->objectAtIndex(i)))->getCString();
            CCArray* tmp = split_string(pStr.c_str(), ';');
            data->setID((static_cast<CCString *>(tmp->objectAtIndex(0)))->intValue());
            data->setIcon((static_cast<CCString *>(tmp->objectAtIndex(1)))->getCString());
            data->setName((static_cast<CCString *>(tmp->objectAtIndex(2)))->getCString());
            data->setNeedLevel((static_cast<CCString *>(tmp->objectAtIndex(3)))->intValue());
            pStr = (static_cast<CCString *>(tmp->objectAtIndex(4)))->getCString();
            CCArray* tmp2 = split_string(pStr.c_str(), ',');
            data->setArrItemName(tmp2);
        }
        updatePlayTableBossDungeonUI(); // 更新UI
    }while (0);
}

#pragma mark boss副本 UI更新

void PopUpUI::updatePlayTableBossDungeonUI()
{
    do
    {
        // UI初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
        CC_BREAK_IF(!panel_c);
        
        // pageView 初始化为第一页
        CC_BREAK_IF(!m_pUIBossDungeonPageView);
        m_nUIBossDungeonCurPage = 0;
        m_pUIBossDungeonPageView->scrollToPage(0);
        
        
        int nCount = m_arrUIBossDungeonData->count(); // 可挑战个数
        CC_BREAK_IF(!m_pRoleByControlled);
        int nLevel=  m_pRoleByControlled->getLevel(); // 玩家等级
        int nCurJew = m_pRoleByControlled->getSpriteAttribute()->getJewel(); // 宝石
        
        // 挑战按钮
        for (int i = 0;  i < 4;  i++)
        {
            UILayout* layout = static_cast<UILayout*>(m_pUIBossDungeonPageView->getPage(i));
            for (int j = 0; j < 8; j++)
            {
                CCString* path = CCString::createWithFormat("ImageView%d",j);
                UIImageView* imageView = static_cast<UIImageView *>(layout->getChildByName(path->getCString()));
                CC_BREAK_IF(!imageView);
                
                // 重置
                UIImageView *icon = static_cast<UIImageView *>(imageView->getChildByTag(10));  // 图片
                if (icon)
                {
                    icon->removeFromParent();
                }
                UIImageView *cover = static_cast<UIImageView *>(imageView->getChildByTag(20));  // 蒙版
                if (cover)
                {
                    cover->removeFromParent();
                }
                UILabel * label = static_cast<UILabel *>(imageView->getChildByName("TextArea_0"));
                CC_BREAK_IF(!label);
                label->setText(" ");
                
                UILabel* levText = static_cast<UILabel *>(imageView->getChildByName("Label_0"));
                CC_BREAK_IF(!levText);
                levText->setVisible(false);
                
                UILabel* levNum = static_cast<UILabel *>(imageView->getChildByName("Label_1"));
                CC_BREAK_IF(!levNum);
                levNum->setVisible(false);
                
                imageView->setOpacity(255);

                int nCurIdx = i*8 + j;
                if (nCurIdx < nCount)
                {
                    imageView->setTouchEnabled(true);
                    BossDungeonUIData * data = static_cast<BossDungeonUIData *>(m_arrUIBossDungeonData->objectAtIndex(nCurIdx));
                    label->setText(data->getName());
                    
                    icon = UIImageView::create();
                    icon->setTag(10);
                    imageView->addChild(icon);
                    std::string iconPath = getFilePath(data->getIcon(), kDirTypeIcon, kFileSuffixPng);
                    icon->loadTexture(iconPath.c_str());
                    
                    if (nLevel < data->getNeedLevel()) // 当前等级 < 玩家所需等级 添加蒙版
                    {
                        cover = UIImageView::create();
                        cover->setTag(20);
                        imageView->addChild(cover);
                        std::string coverPath = getFilePath("234-1", kDirTypeUI, kFileSuffixPng);
                        cover->loadTexture(coverPath.c_str());
                        
                        levText->setVisible(true);
                        levNum->setVisible(true);
                        CCString* tmpStr = CCString::createWithFormat("%d",data->getNeedLevel());
                        levNum->setText(tmpStr->getCString());
                        imageView->setOpacity(200);
                    }
                }
                else // 没有活动数据 添加蒙版
                {
                    imageView->setTouchEnabled(false);
                    imageView->setOpacity(200);
                    cover = UIImageView::create();
                    cover->setTag(20);
                    imageView->addChild(cover);
                    std::string coverPath = getFilePath("234-1", kDirTypeUI, kFileSuffixPng);
                    cover->loadTexture(coverPath.c_str());
                }
            }
        }
        
        // 剩余挑战次数
        UILabel* label_0 = static_cast<UILabel *>(panel_c->getChildByName("Label_0"));
        CC_BREAK_IF(!label_0);
        UILabel* label_00 = static_cast<UILabel *>(label_0->getChildByName("Label_0"));
        CC_BREAK_IF(!label_00);
        label_00->setText(CCString::createWithFormat("%d",m_nUIBossDungeonResidueCnt)->getCString());
        
        // 宝石更新
        UIImageView* imgView_jew = static_cast<UIImageView *>(panel_c->getChildByName("ImageView_jew"));
        CC_BREAK_IF(!imgView_jew);
        UILabel* label_jew = static_cast<UILabel *>(imgView_jew->getChildByName("Label_0"));
        CC_BREAK_IF(!label_jew);
        label_jew->setText(CCString::createWithFormat("%d",nCurJew)->getCString());
        
    } while (0);
}

#pragma mark boss副本UI 宝石购买信息

void PopUpUI::updateplayTableBossDungeonBC(CCArray * arr)
{
    // BC_[购买的之后剩余挑战次数]_[购买挑战次数花费钻石数量]_[剩余购买次数]_[玩家拥有宝石数量]
    do
    {
        CC_BREAK_IF(!arr);
        m_nUIBossDungeonResidueCnt = static_cast<CCString *>(arr->objectAtIndex(0))->intValue();
        m_nUIBossDungeonJewCnt = static_cast<CCString *>(arr->objectAtIndex(1))->intValue();
        m_nUIBossDungeonEnableBuyCnt = static_cast<CCString *>(arr->objectAtIndex(2))->intValue();
        int nCurJew = static_cast<CCString *>(arr->objectAtIndex(3))->intValue();
        
        // UI初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
        CC_BREAK_IF(!panel_c);
        
        // 剩余挑战次数
        UILabel* label_0 = static_cast<UILabel *>(panel_c->getChildByName("Label_0"));
        CC_BREAK_IF(!label_0);
        UILabel* label_00 = static_cast<UILabel *>(label_0->getChildByName("Label_0"));
        CC_BREAK_IF(!label_00);
        label_00->setText(CCString::createWithFormat("%d",m_nUIBossDungeonResidueCnt)->getCString());
        
        // 宝石更新
        UIImageView* imgView_jew = static_cast<UIImageView *>(panel_c->getChildByName("ImageView_jew"));
        CC_BREAK_IF(!imgView_jew);
        UILabel* label_jew = static_cast<UILabel *>(imgView_jew->getChildByName("Label_0"));
        CC_BREAK_IF(!label_jew);
        label_jew->setText(CCString::createWithFormat("%d",nCurJew)->getCString());
        
    } while (0);
}

#pragma mark boss副本UI 战斗奖励信息

void PopUpUI::updateplayTableBossDungeonBA(CCArray * arr)
{
    setPlayTableFightResult(arr, kBossResult);
}

#pragma mark boss副本UI 购买挑战次数回调

void PopUpUI::playTableBossDungeonBuyCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));

            int nCurJew = m_pRoleByControlled->getSpriteAttribute()->getJewel(); // 宝石
            
            if (m_nUIBossDungeonEnableBuyCnt > 0)
            {
                if (nCurJew >= m_nUIBossDungeonJewCnt) // 花费宝石购买提示
                {
                    setConsumeJewelTips(kJewelConsumeForBuyChallenge, m_nUIBossDungeonJewCnt);
                }
                else // 宝石数量不足
                {
                    ((YYScene*)getParent())->showTips(g_sLackJewel);
                }
            }
            else // 宝石购买次数已用尽
            {
                ((YYScene*)getParent())->showTips(g_sPlayTableBossDungeonUnabled);
            }
        }
    }while (0);
}

#pragma mark boss副本UI 扫荡副本回调

void PopUpUI::playTableBossDungeonWipeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
            if (type == TOUCH_EVENT_ENDED)
            {
                CC_BREAK_IF(isTouchUnable(pSender,type));
                CC_BREAK_IF(!m_arrUIBossDungeonData);
                BossDungeonUIData* data = static_cast<BossDungeonUIData *>(m_arrUIBossDungeonData->objectAtIndex(m_nUIBossDungeonCurSelIdx));
                Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_BOSS_DUNGEON_WIPE,data->getID()))->getCString(),true);
                Controller::getInstance()->setDungeonType(kDungeonBoss); //设置进入副本类型
            }
        }
    }while (0);
}

#pragma mark boss副本UI 挑战副本回调

void PopUpUI::playTableBossDungeonChallengeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            CC_BREAK_IF(m_bUIBossDungeonTopTouch);    // 弹出panel 屏蔽挑战回调
            UIImageView* imageView = static_cast<UIImageView *>(pSender);
            m_nUIBossDungeonCurSelIdx = imageView->getTag();
            CC_BREAK_IF(!m_pRoleByControlled);
            int nLevel=  m_pRoleByControlled->getLevel(); // 玩家等级
            BossDungeonUIData* data =static_cast<BossDungeonUIData *>(m_arrUIBossDungeonData->objectAtIndex(m_nUIBossDungeonCurSelIdx));
            if (nLevel < data->getNeedLevel()) // 等级不够
            {
                 ((YYScene*)getParent())->showTips(g_sPlayTableBossDungeonLevel);
            }
            else
            {
                playTableBossDungeonCloseDungeonSetPopTouch(true); // 弹出页面
                playTableBossDungeonUpdatePopUI(true); // 更新弹出框数据
                
                // 判定是否可以扫荡
                bool bWipeEnable = (m_nUIBossWipeCnt > m_nUIBossDungeonCurSelIdx ? true :false);
                {
                    // UI初始化
                    CC_BREAK_IF(!m_pUITouchGroup);
                    UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
                    CC_BREAK_IF(!panel_c);

                    // 二级页面
                    UIPanel* panel_1 = static_cast<UIPanel *>(panel_c->getChildByName("Panel_1"));
                    CC_BREAK_IF(!panel_1);
                   
                    UIButton* btnWipe = static_cast<UIButton *>(panel_1->getChildByName("Button2"));
                    CC_BREAK_IF(!btnWipe);
                    btnWipe->setTouchEnabled(bWipeEnable);
                    btnWipe->setBright(bWipeEnable);
                }
            }
        }
    }while (0);
}

#pragma mark boss副本UI 进入副本按钮回调

void PopUpUI::playTableBossDungeonIntoDungeonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            if(m_nUIBossDungeonResidueCnt <= 0) // 剩余挑战次数不足
            {
                ((YYScene *)getParent())->showTips(g_sPlayTableBossLackChallenge);
            }
            else
            {
                CC_BREAK_IF(!m_arrUIBossDungeonData);
                BossDungeonUIData* data = static_cast<BossDungeonUIData *>(m_arrUIBossDungeonData->objectAtIndex(m_nUIBossDungeonCurSelIdx));
                Controller::getInstance()->sendMessage((CCString::createWithFormat(ASK_BOSS_DUNGEON_CHALLENGE,data->getID()))->getCString(),true);
                Controller::getInstance()->switchGameStatus(kStatusLoading);
                Controller::getInstance()->setDungeonType(kDungeonBoss); //设置进入副本类型
            }
        }
    }while (0);
}

#pragma mark boss副本UI 关闭副本按钮回调

void PopUpUI::playTableBossDungeonCloseDungeonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            playTableBossDungeonCloseDungeonSetPopTouch(false);
            playTableBossDungeonUpdatePopUI(false); // 隐藏弹出框
        }
    }while (0);
}

#pragma mark boss副本UI pageView滑动回调

void PopUpUI::playTableBossDungeonPageViewEvent(CCObject *pSender, PageViewEventType type)
{
    do
    {
        switch (type)
        {
            case PAGEVIEW_EVENT_TURNING:
            {
                if (Controller::getInstance()->getScene()->isNetLoading())
                {
                    return;
                }
                UIPageView* pageView = static_cast<UIPageView*>(pSender);
                CC_BREAK_IF(!pageView);
                m_nUIBossDungeonCurPage = pageView->getCurPageIndex(); // 设置当前选中页
                updatePlayTableBossDungeonCurPageUI();// 圆点标记位切换
                
            }
                break;
            default:
                break;
        }
    } while (0);
}

#pragma mark boss副本 更新弹出框数据

void PopUpUI::playTableBossDungeonUpdatePopUI(bool bVisible)
{
    do
    {
        // UI初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
        CC_BREAK_IF(!panel_c);
        
        UIPanel* panel_1 = static_cast<UIPanel *>(panel_c->getChildByName("Panel_1"));
        CC_BREAK_IF(!panel_1);
        panel_1->setVisible(bVisible);
        
        BossDungeonUIData* data =  static_cast<BossDungeonUIData *>(m_arrUIBossDungeonData->objectAtIndex(m_nUIBossDungeonCurSelIdx));
        CC_BREAK_IF(!data);
        
        // name
        UILabel * name = static_cast<UILabel *>(panel_1->getChildByName("TextArea_0"));
        CC_BREAK_IF(!name);
        name->setText(data->getName());
        
        // icon
        UIImageView* imageView = static_cast<UIImageView *>(panel_1->getChildByName("ImageView2"));
        UIImageView* icon = static_cast<UIImageView *>(imageView->getChildByTag(10));
        if (icon)
        {
            icon->removeFromParent();
        }
        icon = UIImageView::create();
        icon->setTag(10);
        imageView->addChild(icon);
        std::string path = getFilePath(data->getIcon(), kDirTypeIcon, kFileSuffixPng);
        icon->loadTexture(path.c_str());
        
        // 掉落物品
        int nItemCnt = data->getArrItemName()->count();
        for (int i = 0; i < 4; i++)
        {
            CCString* path2 = CCString::createWithFormat("TextArea_%d",i+2);
            UILabel* lal = static_cast<UILabel *>(panel_1->getChildByName(path2->getCString()));
            
            if (i < nItemCnt)
            {
                CCString* str = static_cast<CCString *>(data->getArrItemName()->objectAtIndex(i));
                lal->setText(str->getCString());
            }
            else
            {
                lal->setText(" ");
            }
        }
        
    } while (0);
}

#pragma mark boss副本 设置弹出框后 按钮是否可点击

void PopUpUI::playTableBossDungeonCloseDungeonSetPopTouch(bool bTouch)
{
    do
    {
        // UI初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
        CC_BREAK_IF(!panel_c);

        // pageView 滑动回调
        if (m_pUIBossDungeonPageView)
        {
             m_pUIBossDungeonPageView->setTouchEnabled(!bTouch);
        }
        // 购买
        UIButton* btnBuy = static_cast<UIButton *>(panel_c->getChildByName("Button_buy"));
        CC_BREAK_IF(!btnBuy);
        btnBuy->setTouchEnabled(!bTouch);
        
        // 关闭按钮
        UIPanel* panel0 = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel0"));
        CC_BREAK_IF(!panel0);
        UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
        CC_BREAK_IF(!closeButton);
        closeButton->setTouchEnabled(!bTouch);
        
        // 二级页面
        UIPanel* panel_1 = static_cast<UIPanel *>(panel_c->getChildByName("Panel_1"));
        CC_BREAK_IF(!panel_1);
        panel_1->setTouchEnabled(bTouch);
        
        // 控制挑战按钮
        m_bUIBossDungeonTopTouch = bTouch;
  
    }while (0);
}

#pragma mark 初始化世界boss UI

void PopUpUI::initPlayTableWorldBossUI()
{
    do
    {
        // UI初始化
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_f = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_f"));
        CC_BREAK_IF(!panel_f);
        
        // 祝福回调
        UIButton* btnPray = static_cast<UIButton *>(panel_f->getChildByName("Button_1"));
        CC_BREAK_IF(!btnPray);
        btnPray->setTouchEnabled(true);
        btnPray->addTouchEventListener(this, toucheventselector(PopUpUI::playTableWorldBossPrayCallBack));
        
        // 进入回调
        UIButton* btnEnter = static_cast<UIButton *>(panel_f->getChildByName("Button_0"));
        CC_BREAK_IF(!btnEnter);
        btnEnter->setTouchEnabled(true);
        btnEnter->addTouchEventListener(this, toucheventselector(PopUpUI::playTableWorldBossEnterCallBack));
        
        // 初始化数据
        m_arrUIWorldBossRankList = CCArray::create(); // 伤害排行榜
        m_arrUIWorldBossRankList->retain();
        m_sUIWorldBossAniName = ""; // boss UI动画路径
        m_nUIWorldBossPrayJew = 0; // 祝福所需宝石数
        m_nUIWorldBossQuickEnterJew = 0; // 快速进入所需宝石数
        m_nUIWorldBossCDTime = 0; // 世界boss CD时间
        m_sUIWorldBossPlayerDamagePoint = ""; // 玩家自身攻击数据
        m_pUIWorldBossPortrait = NULL;
        m_nUIWorldBossCurSecondTime = -1;
        
    } while (0);
}

#pragma mark 世界boss 接收UI列表信息

void PopUpUI::updateplayTableWorldBossWC(CCArray * arr)
{
    //返回 [剩余冷却时间(单位 秒)]_[购买祝福花费宝石数量]_[清理cd时间需要花费的宝石数量]_[BOSSICON]_[玩家前十排行(初始都为-1，如果有玩家则，玩家名,伤害总值;...]_[玩家自己对boss造成的伤害值(百分数)]
    do
    {
        CC_BREAK_IF(!arr);
        m_arrUIWorldBossRankList->removeAllObjects();
        int index = 0;
        std::string pStr = "";
        m_nUIWorldBossCDTime = static_cast<CCString *>(arr->objectAtIndex(index++))->intValue(); // 0 表示没有CD
        m_nUIWorldBossPrayJew = static_cast<CCString *>(arr->objectAtIndex(index++))->intValue(); // -1 表示不可祝福
        m_nUIWorldBossQuickEnterJew = static_cast<CCString *>(arr->objectAtIndex(index++))->intValue();
        m_sUIWorldBossAniName = static_cast<CCString *>(arr->objectAtIndex(index++))->getCString(); // boss动画路径
        pStr = static_cast<CCString *>(arr->objectAtIndex(index++))->getCString(); // 玩家排行
        CCArray* arrTmp = split_string(pStr.c_str(), ';');
        for (int i = 0; i < arrTmp->count(); i++)
        {
            pStr = static_cast<CCString *>(arrTmp->objectAtIndex(i))->getCString();
            if (pStr.compare("-1"))
            {
                m_arrUIWorldBossRankList->addObject(arrTmp->objectAtIndex(i));
            }
        }
        m_sUIWorldBossPlayerDamagePoint = static_cast<CCString *>(arr->objectAtIndex(index++))->getCString();
        updatePlayTableWorldBossUI();
    } while (0);
}

#pragma mark 更新世界boss UI

void PopUpUI::updatePlayTableWorldBossUI()
{
    do
    {
        std::string pStr = "";

        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_f = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_f"));
        CC_BREAK_IF(!panel_f);
        
        // 祝福回调
        UIButton* btnPray = static_cast<UIButton *>(panel_f->getChildByName("Button_1"));
        CC_BREAK_IF(!btnPray);
       
        if (m_nUIWorldBossPrayJew > 0)
        {
            btnPray->setTouchEnabled(true);
            btnPray->setBright(true);
        }
        else
        {
            btnPray->setTouchEnabled(false);
            btnPray->setBright(false);
        }
        
        // 进入回调
        UIButton* btnEnter = static_cast<UIButton *>(panel_f->getChildByName("Button_0"));
        CC_BREAK_IF(!btnEnter);
        UILabel* label_0 = static_cast<UILabel *>(btnEnter->getChildByName("Label_0")); // 进入
        CC_BREAK_IF(!label_0);
        UILabel* label_1 = static_cast<UILabel *>(btnEnter->getChildByName("Label_1")); // 立即复活
        CC_BREAK_IF(!label_1);
        UIPanel* panel_0 = static_cast<UIPanel *>(panel_f->getChildByName("Panel_0")); // 倒计时
        CC_BREAK_IF(!panel_0);
        
        if (m_nUIWorldBossCDTime > 0)
        {
            label_0->setVisible(false);
            label_1->setVisible(true);
            panel_0->setVisible(true);
            playTableWorldBosssSetDeleteTime(m_nUIWorldBossCDTime);
        }
        else
        {
            label_0->setVisible(true);
            label_1->setVisible(false);
            panel_0->setVisible(false);
        }
        
        // 添加boss动画
        if (m_pUIWorldBossPortrait) // 如果之前有动画 删除
        {
            m_pUIWorldBossPortrait->removeFromParent();
            m_pUIWorldBossPortrait = NULL;
        }
        
        m_pUIWorldBossPortrait = YYSprite::create();
        if (m_pUIWorldBossPortrait)
        {
            m_pUIWorldBossPortrait->initWithActionSet(m_sUIWorldBossAniName.c_str(), kDirTypeSprite);
        }
        UIImageView* imgView1 = static_cast<UIImageView *>(panel_f->getChildByName("ImageView_1"));
        CC_BREAK_IF(!imgView1);
        UIImageView* imgView0 = static_cast<UIImageView *>(imgView1->getChildByName("ImageView_0"));
        CC_BREAK_IF(!imgView0);
        imgView0->addNode(m_pUIWorldBossPortrait);
        m_pUIWorldBossPortrait->setPosition(ccpSub(imgView0->getPosition(), ccp(0,imgView0->getContentSize().height*0.3f)));
        m_pUIWorldBossPortrait->setVisible(true);
        m_pUIWorldBossPortrait->setScale(0.8f);
        m_pUIWorldBossPortrait->setFaceToRight(false);
        m_pUIWorldBossPortrait->setAction(ACTION_STAND0,true);
   
        // 排行榜
        UIPanel* panel_2 = static_cast<UIPanel *>(panel_f->getChildByName("Panel_2")); // 排行名字
        CC_BREAK_IF(!panel_2);
        UIPanel* panel_3 = static_cast<UIPanel *>(panel_f->getChildByName("Panel_3")); // 排行伤害值
        CC_BREAK_IF(!panel_3);
        
        for (int i = 0; i < 10; i++)
        {
            std::string path = CCString::createWithFormat("Label_%d",i)->getCString();
            UILabel* name = static_cast<UILabel *>(panel_2->getChildByName(path.c_str()));
            UILabel* point = static_cast<UILabel *>(panel_3->getChildByName(path.c_str()));
            if (i < m_arrUIWorldBossRankList->count())
            {
                pStr = static_cast<CCString *>(m_arrUIWorldBossRankList->objectAtIndex(i))->getCString();
                CCArray* arrTmp = split_string(pStr.c_str(), ',');
                pStr = static_cast<CCString *>(arrTmp->objectAtIndex(0))->getCString();
                name->setText(pStr.c_str());
                pStr = static_cast<CCString *>(arrTmp->objectAtIndex(1))->getCString();
                pStr+="%";
                point->setText(pStr.c_str());
            }
            else
            {
                point->setText("0.00%");
            }    
        }
        
        // 玩家自身造成的伤害量
        UILabel* label_10 = static_cast<UILabel *>(panel_3->getChildByName("Label_10"));
        m_sUIWorldBossPlayerDamagePoint+="%";
        label_10->setText(CCString::createWithFormat("%s",m_sUIWorldBossPlayerDamagePoint.c_str())->getCString());
        
    } while (0);
}

#pragma mark 世界boss 设置删除时间label

void PopUpUI::playTableWorldBosssSetDeleteTime(int deleteTime)
{
    do
    {
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_f = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_f"));
        CC_BREAK_IF(!panel_f);
        UIPanel* panel_0 = static_cast<UIPanel *>(panel_f->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel_0);
        UILabel* label_1 = static_cast<UILabel *>(panel_0->getChildByName("Label_1"));
        CC_BREAK_IF(!label_1);
        int hour = deleteTime / 3600;
        int minute = deleteTime / 60 % 60;
        int second = deleteTime % 60;
        const char *time = CCString::createWithFormat("%d:%d:%d",hour,minute,second)->getCString();
        if (minute < 10)
        {
            if (second < 10)
            {
                time = CCString::createWithFormat("%d:0%d:0%d",hour,minute,second)->getCString();
            }
            else
            {
                time = CCString::createWithFormat("%d:0%d:%d",hour,minute,second)->getCString();
            }
        }
        label_1->setText(time);
       
    } while (0);
}

#pragma mark 世界boss 重置数据

void PopUpUI::playTableWorldBossResetData()
{
    if (m_pUIWorldBossPortrait)
    {
        m_pUIWorldBossPortrait->removeFromParent();
        m_pUIWorldBossPortrait = NULL;
    }
    m_nUIWorldBossCDTime = 0;
    m_nUIWorldBossCurSecondTime = -1;
    do
    {
        CC_BREAK_IF(!m_pUITouchGroup);
        UIPanel* panel_f = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_f"));
        CC_BREAK_IF(!panel_f);
        UIPanel* panel_0 = static_cast<UIPanel *>(panel_f->getChildByName("Panel_0"));
        CC_BREAK_IF(!panel_0);
        UILabel* label_1 = static_cast<UILabel *>(panel_0->getChildByName("Label_1"));
        CC_BREAK_IF(!label_1);
        label_1->setText(" ");
    } while (0);
            
}

#pragma mark 世界boss 实时更新数据

void PopUpUI::updateWorldBossData()
{
    // Boss头像动画更新
    if (m_pUIWorldBossPortrait)
    {
        m_pUIWorldBossPortrait->updateAnimation();
    }
    // 倒计时label更新
    if (m_nUIWorldBossCDTime > 0)
    {
        if (Controller::getInstance()->getCurrentSecond() != m_nUIWorldBossCurSecondTime)
        {
           m_nUIWorldBossCurSecondTime = Controller::getInstance()->getCurrentSecond();
           playTableWorldBosssSetDeleteTime(m_nUIWorldBossCDTime);
           m_nUIWorldBossCDTime--;
           if (m_nUIWorldBossCDTime == 0) // 倒计时接收显示进入按钮
           {
               do
               {
                   CC_BREAK_IF(!m_pUITouchGroup);
                   UIPanel* panel_f = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_f"));
                   CC_BREAK_IF(!panel_f);
                   UIButton* btnEnter = static_cast<UIButton *>(panel_f->getChildByName("Button_0"));
                   CC_BREAK_IF(!btnEnter);
                   UILabel* label_0 = static_cast<UILabel *>(btnEnter->getChildByName("Label_0")); // 进入
                   CC_BREAK_IF(!label_0);
                   UILabel* label_1 = static_cast<UILabel *>(btnEnter->getChildByName("Label_1")); // 立即复活
                   CC_BREAK_IF(!label_1);
                   UIPanel* panel_0 = static_cast<UIPanel *>(panel_f->getChildByName("Panel_0")); // 倒计时
                   CC_BREAK_IF(!panel_0);
                   label_0->setVisible(true);
                   label_1->setVisible(false);
                   panel_0->setVisible(false);
               } while (0);
           }
        }
    }
}

#pragma mark 世界boss 接收消除cd时间信息

void PopUpUI::updateplayTableWorldBossWT(CCArray * arr)
{
    do
    {
        CC_BREAK_IF(!arr);
        CCString* pStr = static_cast<CCString *>(arr->objectAtIndex(0));
        if (pStr->compare("0") == 0) // 清楚CD成功 进入世界boss
        {
            const char *msg = CCString::createWithFormat(ASK_WORLD_BOSS_CHALLENGE)->getCString();
            Controller::getInstance()->sendMessage(msg,true);
            Controller::getInstance()->switchGameStatus(kStatusLoading);
            Controller::getInstance()->setDungeonType(kWorldBoss); //设置进入副本类型
        }
    } while (0);
}

#pragma mark 世界boss 接收增加祝福Buff信息

void PopUpUI::updateplayTableWorldBossWB(CCArray * arr)
{
    do
    {
        CC_BREAK_IF(!arr);
        CCString* pStr = static_cast<CCString *>(arr->objectAtIndex(0));
        if (pStr->compare("1") == 0)
        {
          ((YYScene*)getParent())->showTips(g_sWorldBossPraySuccess);
            CC_BREAK_IF(!m_pUITouchGroup);
            UIPanel* panel_f = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_f"));
            CC_BREAK_IF(!panel_f);
            UIButton* btnPray = static_cast<UIButton *>(panel_f->getChildByName("Button_1"));
            CC_BREAK_IF(!btnPray);
            btnPray->setTouchEnabled(false);
            btnPray->setBright(false);
        }
    } while (0);
}

#pragma mark 世界boss 接收击杀boss信息

void PopUpUI::updateplayTableWorldBossWK(CCArray * arr)
{
    do
    {
        CC_BREAK_IF(!arr);
        CCString* pStr1 = static_cast<CCString *>(arr->objectAtIndex(0)); // 玩家名称
        CCString* pStr2 = static_cast<CCString *>(arr->objectAtIndex(1)); // BOSS名称
        CCString* tips = CCString::createWithFormat(g_sWorldBossKilled,pStr2->getCString(),pStr1->getCString());
        ((YYScene*)getParent())->showTips(tips->getCString(),kTipPlayTableWorldBossKill);
        
        
        // 播放Boss被击杀动画 设置当前boss 死亡
        YYFightSprite* boss = ((YYScene*)getParent())->getControlUILayer()->getBoss();
        boss->die();

        
    } while (0);
}

#pragma mark 世界boss 接收是否可以击杀boss

void PopUpUI::updateplayTableWorldBossWD(CCArray * arr)
{
    do
    {
        CC_BREAK_IF(!arr);
        int nIsKillBoss = static_cast<CCString *>(arr->objectAtIndex(0))->intValue(); // 0 不可击杀 1 可以击杀
        bool bKill;
        nIsKillBoss == 0 ? bKill = false : bKill = true;
        (((YYScene*)getParent())->getControlUILayer())->setWorldBossEnableDead(bKill);
        float fRemainHP = static_cast<CCString *>(arr->objectAtIndex(1))->floatValue(); // Boss 剩余血量百分比
        (((YYScene*)getParent())->getControlUILayer())->setWorldBossRemainHP(fRemainHP);
        uint32_t nAmount = static_cast<CCString *>(arr->objectAtIndex(2))->intValue();  // Boss 总血量
        (((YYScene*)getParent())->getControlUILayer())->setWorldBossAmountHP(nAmount);
        
    } while (0);
}

#pragma mark 世界boss 祝福回调

void PopUpUI::playTableWorldBossPrayCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            int nCurJew = m_pRoleByControlled->getSpriteAttribute()->getJewel(); // 宝石
            if (nCurJew >= m_nUIWorldBossPrayJew)
            {
                setConsumeJewelTips(kJewelConsumeForBuyPray, m_nUIWorldBossPrayJew);
            }
            else // 宝石数量不足
            {
                ((YYScene*)getParent())->showTips(g_sLackJewel);
            }
        }
        
    }while (0);
}

#pragma mark 世界boss 进入回调

void PopUpUI::playTableWorldBossEnterCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            if(m_nUIWorldBossCDTime <= 0) // 无CD 直接进入
            {
                const char *msg = CCString::createWithFormat(ASK_WORLD_BOSS_CHALLENGE)->getCString();
                Controller::getInstance()->sendMessage(msg,true);
                Controller::getInstance()->switchGameStatus(kStatusLoading);
                Controller::getInstance()->setDungeonType(kWorldBoss); //设置进入副本类型
            }
            else
            {
                int nCurJew = m_pRoleByControlled->getSpriteAttribute()->getJewel(); // 宝石
                if (nCurJew >= m_nUIWorldBossQuickEnterJew)
                {
                    setConsumeJewelTips(kJewelConsumeForBuyQuickEnter, m_nUIWorldBossQuickEnterJew);
                }
                else
                {
                    ((YYScene*)getParent())->showTips(g_sLackJewel);
                }
            }
        }
    }while (0);
}

#pragma mark 初始化 活动奖励

void PopUpUI::setPlayTableFightResult(CCArray* arr, PlayTableFightResult kType)
{
    do
    {
        bool bBossResultWipe = false;  // 是否是boss副本扫荡奖励页面
        if (kType == kBossResult)
        {
            int nFlag = (static_cast<CCString *>(arr->objectAtIndex(0)))->intValue(); // 读取标识位
            if (nFlag == 1)
            {
                bBossResultWipe = true;
            }
        }
        if (bBossResultWipe)
        {
            setPlayTableWipeFightResult(arr,0);
        }
        else
        {
            // 加载UI
            Controller::getInstance()->setGameStatus(kStatusUI);
            setUIType(kUIPlayTableFightResult);
            if (m_pUITouchGroup)
            {
                removeChild(m_pUITouchGroup);
                m_pUITouchGroup = NULL;
            }
            m_pUITouchGroup = TouchGroup::create();
            CC_BREAK_IF(!m_pUITouchGroup);
            addChild(m_pUITouchGroup);
            m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
            std::string filePath = PLAYTABLE_RESULTUI_RESOURCE_FILE_NAME;
            m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
            
            CC_BREAK_IF(!arr);
            
            // 屏蔽所有panel
            Layout * panel = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel"));
            CC_BREAK_IF(!panel);
            for (int i = 0; i < panel->getChildrenCount(); i++)
            {
                Widget * widget= static_cast<Widget *>(panel->getChildren()->objectAtIndex(i));
                CC_BREAK_IF(!widget);
                widget->setEnabled(false);
                widget->setVisible(false);
            }
            
            // 显示公共页面
            UIPanel* panel0 = static_cast<UIPanel *>(panel->getChildByName("Panel0"));
            panel0->setEnabled(true);
            panel0->setVisible(true);
            
            // 确定按钮回调
            UIButton *pButton_sure = static_cast<UIButton*>(panel0->getChildByName("Button_done"));
            CC_BREAK_IF(!pButton_sure);
            pButton_sure->setTouchEnabled(true);
            pButton_sure->addTouchEventListener(this, toucheventselector(PopUpUI::PlayTableFightResultConfirmCallBack));
            m_bPlayTableBtnTouch = false;  //播放特效动画 按钮不可点击
            
            // 清除按键状态
            ((YYScene *)getParent())->getControlUILayer()->clearKeyState();
            
            switch (kType)
            {
                case kDungeonResult:   // 地下城
                {
                    // DA_[关卡]_[所得道具icon],[所得道具名字],[所得道具数量];..._[金币]_[jew]_[经验] 特殊情况 没有物品奖励 返回为0
                    int nIndex = 0;
                    std::string pStr;
                    uint8_t nItemCnt = 0;
                    float fAniTime = 0.3f;  // 动画间隔
                    std::vector<UIImageView* > vec_icon; // icon 图标容器
                    std::vector<UILabel* > vec_label;    // label 个数容器
                    
                    UIPanel* panel1 = static_cast<UIPanel *>(panel->getChildByName("Panel1"));
                    CC_BREAK_IF(!panel1);
                    panel1->setEnabled(true);
                    panel1->setVisible(true);
                    
                    // 关卡
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(nIndex++)))->getCString();
                    UILabel* pLabRound = static_cast<UILabel *>(panel1->getChildByName("Label_0_0"));
                    CC_BREAK_IF(!pLabRound);
                    pLabRound->setText(pStr.c_str());
                    
                    // 物品
                    UILabel* pLabel_3 = static_cast<UILabel *>(panel1->getChildByName("Label_3"));
                    CC_BREAK_IF(!pLabel_3);
                    
                    // 隐藏物品图标
                    for (int i = 0; i < 12; i++)
                    {
                        UIImageView* img = static_cast<UIImageView *>(pLabel_3->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
                        CC_BREAK_IF(!img);
                        img->setVisible(false);
                    }
                    
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(nIndex++)))->getCString();
                    CCArray* arrImg = split_string(pStr.c_str(), ';');
                    nItemCnt = arrImg->count();
                    nItemCnt > 12 ? nItemCnt = 12 : nItemCnt = nItemCnt;
                    
                    if(pStr.compare("0") == 0)  //没有物品奖励 返回"0"
                    {
                        nItemCnt = 0;
                    }
                    
                    for (int i = 0; i < nItemCnt; i++)
                    {
                        UIImageView* img = static_cast<UIImageView *>(pLabel_3->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
                        CC_BREAK_IF(!img);
                        CCPoint cp = img->getPosition();
                        img->setVisible(true);
                        
                        pStr = (static_cast<CCString *>(arrImg->objectAtIndex(i)))->getCString();
                        CCArray* arrContent = split_string(pStr.c_str(), ',');
                        CCString* pIconName = static_cast<CCString *>(arrContent->objectAtIndex(0));
                        CCString* pStrCnt  = static_cast<CCString *>(arrContent->objectAtIndex(2));
                        std::string pIconPath = getFilePath(pIconName->getCString(), kDirTypeIcon, kFileSuffixPng);
                        
                        UIImageView* imgIcon = UIImageView::create();
                        imgIcon->loadTexture(pIconPath.c_str());
                        CC_BREAK_IF(!imgIcon);
                        img->addChild(imgIcon,10);
                        imgIcon->setVisible(false);
                        vec_icon.push_back(imgIcon);
                        
                        UILabel* pLabelCnt = createLabel(CCSizeMake(img->getContentSize().width-5, 30), kCCTextAlignmentRight, 20, ccRED);
                        CC_BREAK_IF(!pLabelCnt);
                        img->addChild(pLabelCnt,20);
                        pLabelCnt->setVisible(false);
                        pLabelCnt->setText(CCString::createWithFormat("X%s",pStrCnt->getCString())->getCString());
                        pLabelCnt->setPosition(ccp(0.0f, -img->getSize().height * 0.5f + 15));
                        vec_label.push_back(pLabelCnt);
                    }
                    
                    // 金币
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(nIndex++)))->getCString();
                    UILabel* pLabGlod = static_cast<UILabel *>(panel1->getChildByName("Label_2_0"));
                    CC_BREAK_IF(!pLabGlod);
                    pLabGlod->setText(pStr.c_str());
                    pLabGlod->setVisible(false);
                    
                    // 宝石 未使用
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(nIndex++)))->getCString();
                    
                    // 经验
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(nIndex++)))->getCString();
                    UILabel* pLabExp = static_cast<UILabel *>(panel1->getChildByName("Label_1_0"));
                    CC_BREAK_IF(!pLabExp);
                    pLabExp->setText(pStr.c_str());
                    pLabExp->setVisible(false);
                    
                    pLabExp->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                    fAniTime+=0.3f;
                    pLabGlod->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                    fAniTime+=0.3f;
                    
                    if (vec_icon.size() > 0)
                    {
                        std::vector<UIImageView* >::iterator it;
                        std::vector<UILabel* >::iterator it2 = vec_label.begin();
                        for (it = vec_icon.begin(); it != vec_icon.end(); it++)
                        {
                            UIImageView* icon = *it;
                            UILabel* label = *it2;
                            icon->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                            label->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                            
                            // 特效
                            CCCallFunc* callFunc = CCCallFuncN::create(this,callfuncN_selector(PopUpUI::playTableFightResultPlayEffectCallBack));
                            CCSequence*  seqEffect = CCSequence::create(CCDelayTime::create(fAniTime), callFunc, NULL);
                            icon->runAction(seqEffect);
                            
                            fAniTime+=0.3f;
                            it2++;
                        }
                    }
                    
                    //播放动画完成回调
                    CCCallFunc* callFuncFinish = CCCallFuncN::create(this,callfuncN_selector(PopUpUI::playTableFightResultFinishAnimationCallBack));
                    this->runAction(CCSequence::create(CCDelayTime::create(fAniTime),callFuncFinish,NULL));
                    
                }
                    break;
                case kProtossResult: // 守卫星灵
                {
                    // TW_[关卡]_[经验]
                    int nIndex = 0;
                    std::string pStr;
                    
                    UIPanel* panel1 = static_cast<UIPanel *>(panel->getChildByName("Panel1"));
                    CC_BREAK_IF(!panel1);
                    panel1->setEnabled(true);
                    panel1->setVisible(true);
                    
                    // 关卡
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(nIndex++)))->getCString();
                    UILabel* pLabRound = static_cast<UILabel *>(panel1->getChildByName("Label_0_0"));
                    CC_BREAK_IF(!pLabRound);
                    pLabRound->setText(pStr.c_str());
                    pLabRound->setVisible(true);
                    
                    // 经验
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(nIndex++)))->getCString();
                    UILabel* pLabExp = static_cast<UILabel *>(panel1->getChildByName("Label_1_0"));
                    CC_BREAK_IF(!pLabExp);
                    pLabExp->setText(pStr.c_str());
                    pLabExp->setVisible(true);
                    
                    // 金币隐藏
                    UILabel* pLabGlod = static_cast<UILabel *>(panel1->getChildByName("Label_2"));
                    CC_BREAK_IF(!pLabGlod);
                    pLabGlod->setVisible(false);
                    
                    // 物品隐藏
                    UILabel* pLabel_3 = static_cast<UILabel *>(panel1->getChildByName("Label_3"));
                    CC_BREAK_IF(!pLabel_3);
                    pLabel_3->setVisible(false);
                    
                    m_bPlayTableBtnTouch = true; // 无动画播放 按钮可点击
                }
                    break;
                case kBossResult: // BOSS 副本
                {
                    // 胜利返回BA_[经验]_[金币]_[剩余挑战次数]_[奖励名字；icon；数量]...
                    // 失败返回BA_-1
                    std::string pStr;
                    UIPanel* panel1 = static_cast<UIPanel *>(panel->getChildByName("Panel1"));
                    CC_BREAK_IF(!panel1);
                    panel1->setEnabled(true);
                    panel1->setVisible(true);
                    
                    // 隐藏标题Label
                    UILabel* Label_0 = static_cast<UILabel *>(panel1->getChildByName("Label_0"));
                    CC_BREAK_IF(!Label_0);
                    Label_0->setVisible(false);
                    UILabel* Label_0_0 = static_cast<UILabel *>(panel1->getChildByName("Label_0_0"));
                    CC_BREAK_IF(!Label_0_0);
                    Label_0_0->setVisible(false);
                    UILabel* Label_0_1 = static_cast<UILabel *>(panel1->getChildByName("Label_0_1"));
                    CC_BREAK_IF(!Label_0_1);
                    Label_0_1->setVisible(false);
                   // int nFlag = (static_cast<CCString *>(arr->objectAtIndex(0)))->intValue(); // 读取标示位
                    int nSuccess = (static_cast<CCString *>(arr->objectAtIndex(1)))->intValue();
                    if (nSuccess > 0) // 挑战成功
                    {
                        float fAniTime = 0.0f;
                        std::vector<UIImageView *> vec_icon;
                        std::vector<UILabel *> vec_label;
                        
                        // 挑战成功
                        UILabel* Label_0_2 = static_cast<UILabel *>(panel1->getChildByName("Label_0_2"));
                        CC_BREAK_IF(!Label_0_2);
                        Label_0_2->setVisible(true);
                        Label_0_2->setText(g_sPlayTableBossDungeonChallenge[1]);
                        
                        // 物品
                        UILabel* pLabel_3 = static_cast<UILabel *>(panel1->getChildByName("Label_3"));
                        CC_BREAK_IF(!pLabel_3);
                        
                        // 隐藏物品图标
                        for (int i = 0; i < 12; i++)
                        {
                            UIImageView* img = static_cast<UIImageView *>(pLabel_3->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
                            CC_BREAK_IF(!img);
                            img->setVisible(false);
                        }
                        
                        // 经验
                        pStr = static_cast<CCString *>(CCString::createWithFormat("%d",nSuccess))->getCString();
                        UILabel* pLabExp = static_cast<UILabel *>(panel1->getChildByName("Label_1_0"));
                        CC_BREAK_IF(!pLabExp);
                        pLabExp->setText(pStr.c_str());
                        pLabExp->setVisible(false);
                        
                        // 金币
                        pStr = (static_cast<CCString *>(arr->objectAtIndex(2)))->getCString();
                        UILabel* pLabGlod = static_cast<UILabel *>(panel1->getChildByName("Label_2_0"));
                        CC_BREAK_IF(!pLabGlod);
                        pLabGlod->setText(pStr.c_str());
                        pLabGlod->setVisible(false);
                        
                        // 剩余挑战次数不解析
                        
                        // 动画
                        pLabExp->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                        fAniTime+=0.3f;
                        pLabGlod->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                        fAniTime+=0.3f;
                        
                        for (int i = 4; i < arr->count(); i++)
                        {
                            pStr = (static_cast<CCString *>(arr->objectAtIndex(i)))->getCString();
                            UIImageView* img = static_cast<UIImageView *>(pLabel_3->getChildByName(CCString::createWithFormat("ImageView_%d",i-2)->getCString()));
                            CC_BREAK_IF(!img);
                            CCPoint cp = img->getPosition();
                            img->setVisible(true);
                            
                            CCArray* arrContent = split_string(pStr.c_str(), ';');
                            CCString* pIconName = static_cast<CCString *>(arrContent->objectAtIndex(1));
                            CCString* pStrCnt  = static_cast<CCString *>(arrContent->objectAtIndex(2));
                            std::string pIconPath = getFilePath(pIconName->getCString(), kDirTypeIcon, kFileSuffixPng);
                            
                            UIImageView* imgIcon = UIImageView::create();
                            imgIcon->loadTexture(pIconPath.c_str());
                            CC_BREAK_IF(!imgIcon);
                            img->addChild(imgIcon,10);
                            imgIcon->setVisible(false);
                            vec_icon.push_back(imgIcon);
                            
                            UILabel* pLabelCnt = createLabel(CCSizeMake(img->getContentSize().width-5, 30), kCCTextAlignmentRight, 20, ccRED);
                            CC_BREAK_IF(!pLabelCnt);
                            img->addChild(pLabelCnt,20);
                            pLabelCnt->setVisible(false);
                            pLabelCnt->setText(CCString::createWithFormat("X%s",pStrCnt->getCString())->getCString());
                            pLabelCnt->setPosition(ccp(0.0f, -img->getSize().height * 0.5f + 15));
                            vec_label.push_back(pLabelCnt);
                        }
                        
                        // 动画
                        if (vec_icon.size() > 0)
                        {
                            std::vector<UIImageView* >::iterator it;
                            std::vector<UILabel* >::iterator it2 = vec_label.begin();
                            for (it = vec_icon.begin(); it != vec_icon.end(); it++)
                            {
                                UIImageView* icon = *it;
                                UILabel* label = *it2;
                                icon->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                                label->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                                
                                // 特效
                                CCCallFunc* callFunc = CCCallFuncN::create(this,callfuncN_selector(PopUpUI::playTableFightResultPlayEffectCallBack));
                                CCSequence*  seqEffect = CCSequence::create(CCDelayTime::create(fAniTime), callFunc, NULL);
                                icon->runAction(seqEffect);
                                
                                fAniTime+=0.3f;
                                it2++;
                            }
                        }
                        
                        //播放动画完成回调
                        CCCallFunc* callFuncFinish = CCCallFuncN::create(this,callfuncN_selector(PopUpUI::playTableFightResultFinishAnimationCallBack));
                        this->runAction(CCSequence::create(CCDelayTime::create(fAniTime),callFuncFinish,NULL));
                    }
                    else // 挑战失败
                    {
                        m_bPlayTableBtnTouch = true; // 无动画播放 按钮可点击
                        
                        // 挑战失败
                        UILabel* Label_0_2 = static_cast<UILabel *>(panel1->getChildByName("Label_0_2"));
                        CC_BREAK_IF(!Label_0_2);
                        Label_0_2->setVisible(true);
                        Label_0_2->setText(g_sPlayTableBossDungeonChallenge[0]);
                        
                        // 经验隐藏
                        UILabel* pLabExp = static_cast<UILabel *>(panel1->getChildByName("Label_1"));
                        CC_BREAK_IF(!pLabExp);
                        pLabExp->setVisible(false);
                        
                        // 金币隐藏
                        UILabel* pLabGlod = static_cast<UILabel *>(panel1->getChildByName("Label_2"));
                        CC_BREAK_IF(!pLabGlod);
                        pLabGlod->setVisible(false);
                        
                        // 物品隐藏
                        UILabel* pLabel_3 = static_cast<UILabel *>(panel1->getChildByName("Label_3"));
                        CC_BREAK_IF(!pLabel_3);
                        pLabel_3->setVisible(false);
                    }
                }
                    break;
                default:
                    break;
            }
        }
    } while (0);
}

#pragma mark 活动 扫荡副本战斗结果页面

void PopUpUI::setPlayTableWipeFightResult(CCArray* arr, int nType) // 0 boss副本
{
    do
    {
        switch (nType) {
            case 0: // Boss副本
            {
                // 加载UI
                if (m_pUITouchPop)
                {
                    removeChild(m_pUITouchPop);
                    m_pUITouchPop = NULL;
                }
                m_pUITouchPop = TouchGroup::create();
                CC_BREAK_IF(!m_pUITouchPop);
                addChild(m_pUITouchPop);
                m_pUITouchPop->setPosition(m_cAdjustScreenPosition);
                std::string filePath = PLAYTABLE_RESULTUI_RESOURCE_FILE_NAME;
                m_pUITouchPop->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
                
                CC_BREAK_IF(!arr);
                
                // 屏蔽所有panel
                Layout * panel = static_cast<Layout *>(m_pUITouchPop->getWidgetByName("Panel"));
                CC_BREAK_IF(!panel);
                for (int i = 0; i < panel->getChildrenCount(); i++)
                {
                    Widget * widget= static_cast<Widget *>(panel->getChildren()->objectAtIndex(i));
                    CC_BREAK_IF(!widget);
                    widget->setEnabled(false);
                    widget->setVisible(false);
                }
                
                // 显示公共页面
                UIPanel* panel0 = static_cast<UIPanel *>(panel->getChildByName("Panel0"));
                panel0->setEnabled(true);
                panel0->setVisible(true);
                
                // 确定按钮回调
                UIButton *pButton_sure = static_cast<UIButton*>(panel0->getChildByName("Button_done"));
                CC_BREAK_IF(!pButton_sure);
                pButton_sure->setTouchEnabled(true);
                pButton_sure->addTouchEventListener(this, toucheventselector(PopUpUI::PlayTableWipeFightResultConfirmCallBack));
                
                m_bPlayTableBtnTouch = false;  //播放特效动画 按钮不可点击
                
                std::string pStr;
                UIPanel* panel1 = static_cast<UIPanel *>(panel->getChildByName("Panel1"));
                CC_BREAK_IF(!panel1);
                panel1->setEnabled(true);
                panel1->setVisible(true);
                
                // 隐藏标题Label
                UILabel* Label_0 = static_cast<UILabel *>(panel1->getChildByName("Label_0"));
                CC_BREAK_IF(!Label_0);
                Label_0->setVisible(false);
                UILabel* Label_0_0 = static_cast<UILabel *>(panel1->getChildByName("Label_0_0"));
                CC_BREAK_IF(!Label_0_0);
                Label_0_0->setVisible(false);
                UILabel* Label_0_1 = static_cast<UILabel *>(panel1->getChildByName("Label_0_1"));
                CC_BREAK_IF(!Label_0_1);
                Label_0_1->setVisible(false);
                //int nFlag = (static_cast<CCString *>(arr->objectAtIndex(0)))->intValue(); // 读取标示位
                int nSuccess = (static_cast<CCString *>(arr->objectAtIndex(1)))->intValue();
                {
                    float fAniTime = 0.0f;
                    std::vector<UIImageView *> vec_icon;
                    std::vector<UILabel *> vec_label;
                    
                    // 挑战成功
                    UILabel* Label_0_2 = static_cast<UILabel *>(panel1->getChildByName("Label_0_2"));
                    CC_BREAK_IF(!Label_0_2);
                    Label_0_2->setVisible(true);
                    Label_0_2->setText(g_sPlayTableBossDungeonChallenge[1]);
                    
                    // 物品
                    UILabel* pLabel_3 = static_cast<UILabel *>(panel1->getChildByName("Label_3"));
                    CC_BREAK_IF(!pLabel_3);
                    
                    // 隐藏物品图标
                    for (int i = 0; i < 12; i++)
                    {
                        UIImageView* img = static_cast<UIImageView *>(pLabel_3->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
                        CC_BREAK_IF(!img);
                        img->setVisible(false);
                    }
                    
                    // 经验
                    pStr = static_cast<CCString *>(CCString::createWithFormat("%d",nSuccess))->getCString();
                    UILabel* pLabExp = static_cast<UILabel *>(panel1->getChildByName("Label_1_0"));
                    CC_BREAK_IF(!pLabExp);
                    pLabExp->setText(pStr.c_str());
                    pLabExp->setVisible(false);
                    
                    // 金币
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(2)))->getCString();
                    UILabel* pLabGlod = static_cast<UILabel *>(panel1->getChildByName("Label_2_0"));
                    CC_BREAK_IF(!pLabGlod);
                    pLabGlod->setText(pStr.c_str());
                    pLabGlod->setVisible(false);
                    
                    // 剩余挑战次数
                    pStr = (static_cast<CCString *>(arr->objectAtIndex(3)))->getCString();
                   
                    CC_BREAK_IF(!m_pUITouchGroup);
                    UIPanel* panel_c = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel_c"));
                    CC_BREAK_IF(!panel_c);
                    UILabel* label_0 = static_cast<UILabel *>(panel_c->getChildByName("Label_0"));
                    CC_BREAK_IF(!label_0);
                    UILabel* label_00 = static_cast<UILabel *>(label_0->getChildByName("Label_0"));
                    CC_BREAK_IF(!label_00);
                    label_00->setText(pStr.c_str());
                    
                    // 动画
                    pLabExp->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                    fAniTime+=0.3f;
                    pLabGlod->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                    fAniTime+=0.3f;
                    
                    for (int i = 4; i < arr->count(); i++)
                    {
                        pStr = (static_cast<CCString *>(arr->objectAtIndex(i)))->getCString();
                        UIImageView* img = static_cast<UIImageView *>(pLabel_3->getChildByName(CCString::createWithFormat("ImageView_%d",i-2)->getCString()));
                        CC_BREAK_IF(!img);
                        CCPoint cp = img->getPosition();
                        img->setVisible(true);
                        
                        CCArray* arrContent = split_string(pStr.c_str(), ';');
                        CCString* pIconName = static_cast<CCString *>(arrContent->objectAtIndex(1));
                        CCString* pStrCnt  = static_cast<CCString *>(arrContent->objectAtIndex(2));
                        std::string pIconPath = getFilePath(pIconName->getCString(), kDirTypeIcon, kFileSuffixPng);
                        
                        UIImageView* imgIcon = UIImageView::create();
                        imgIcon->loadTexture(pIconPath.c_str());
                        CC_BREAK_IF(!imgIcon);
                        img->addChild(imgIcon,10);
                        imgIcon->setVisible(false);
                        vec_icon.push_back(imgIcon);
                        
                        UILabel* pLabelCnt = createLabel(CCSizeMake(img->getContentSize().width-5, 30), kCCTextAlignmentRight, 20, ccRED);
                        CC_BREAK_IF(!pLabelCnt);
                        img->addChild(pLabelCnt,20);
                        pLabelCnt->setVisible(false);
                        pLabelCnt->setText(CCString::createWithFormat("X%s",pStrCnt->getCString())->getCString());
                        pLabelCnt->setPosition(ccp(0.0f, -img->getSize().height * 0.5f + 15));
                        vec_label.push_back(pLabelCnt);
                    }
                    
                    // 动画
                    if (vec_icon.size() > 0)
                    {
                        std::vector<UIImageView* >::iterator it;
                        std::vector<UILabel* >::iterator it2 = vec_label.begin();
                        for (it = vec_icon.begin(); it != vec_icon.end(); it++)
                        {
                            UIImageView* icon = *it;
                            UILabel* label = *it2;
                            icon->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                            label->runAction(CCSequence::create(CCDelayTime::create(fAniTime),CCShow::create(),NULL));
                            
                            // 特效
                            CCCallFunc* callFunc = CCCallFuncN::create(this,callfuncN_selector(PopUpUI::playTableFightResultPlayEffectCallBack));
                            CCSequence*  seqEffect = CCSequence::create(CCDelayTime::create(fAniTime), callFunc, NULL);
                            icon->runAction(seqEffect);
                            
                            fAniTime+=0.3f;
                            it2++;
                        }
                    }
                    
                    //播放动画完成回调
                    CCCallFunc* callFuncFinish = CCCallFuncN::create(this,callfuncN_selector(PopUpUI::playTableFightResultFinishAnimationCallBack));
                    this->runAction(CCSequence::create(CCDelayTime::create(fAniTime),callFuncFinish,NULL));
                }
                
                // 隐藏扫荡弹出框

                CC_BREAK_IF(!m_pUITouchGroup);
                m_pUITouchGroup->setTouchEnabled(false);
                playTableBossDungeonCloseDungeonSetPopTouch(false);
                playTableBossDungeonUpdatePopUI(false); // 隐藏弹出框
                
                // 更新剩余挑战次数
            }
                break;
            default:
                break;
        }
    } while (0);
}

#pragma mark 活动 战斗结果物品奖励特效

void PopUpUI::playTableFightResultPlayEffectCallBack(CCNode* pSender)
{
    UIImageView* imgView = static_cast<UIImageView *>(pSender);
    CCPoint position = imgView->getPosition();
    YYSprite * sprite = YYSprite::createWithActionSet("se6", "se6", kDirTypeEffect);
    sprite->setAction(0, false);
    imgView->addNode(sprite,30);
    m_pSpecialEffectArray->addObject(sprite);
    const std::string filePath = getFilePath(REWARDSTAR_EFFECT ,kDirTypeSound ,kFileSuffixWav);
    PLAY_EFFECT(filePath.c_str());
}

#pragma mark 活动 战斗结果 动画播放完成

void PopUpUI::playTableFightResultFinishAnimationCallBack()
{
    m_bPlayTableBtnTouch = true;
}

#pragma mark 活动 扫荡页面发送确认奖励

void PopUpUI::PlayTableWipeFightResultConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(!m_bPlayTableBtnTouch); //UI 播放 按钮不可点击
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            //发送离开副本消息
            {
                m_pUITouchGroup->setTouchEnabled(true);
                if (m_pUITouchPop)
                {
                    m_pUITouchPop->removeAllChildrenWithCleanup(true);
                    m_pUITouchPop->removeFromParent();
                }
            }
        }
    }while (0);
}

#pragma mark 活动 发送确认离开消息

void PopUpUI::PlayTableFightResultConfirmCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        CC_BREAK_IF(!m_bPlayTableBtnTouch); //UI 播放 按钮不可点击
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type));
            //发送离开副本消息
            {
                Controller::getInstance()->sendMessage(ASK_RETURN_CITY,true);
                Controller::getInstance()->switchGameStatus(kStatusLoading);
            }
        }
    }while (0);
}
