//
//  YYTaskUI.cpp
//  Fairyland
//
//  Created by fuchenhao on 14-4-21.
//
//

#include "YYPopUpUI.h"
#include "YYNPC.h"

#pragma mark 任务

void PopUpUI::initTaskUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUITask);
    m_pRoleByControlled = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled();
    
    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    string jsonFilePath = TASK_UI_RESOURCE_FILE_NAME;
    Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(jsonFilePath.c_str());
    m_pUITouchGroup->addWidget(mainWidget);
    
    Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    ScrollView* taskScrollview = static_cast<ScrollView*>(panel1->getChildByName("ScrollView"));
    taskScrollview->addTouchEventListener(this,toucheventselector(PopUpUI::taskCloseTipsCallback));
    int current_MoveSize = taskScrollview->getInnerContainer()->getContentSize().height;
    int view_Width = taskScrollview->getSize().width;
    int view_Height = taskScrollview->getSize().height;
    
    int adjust_MoveSize = 0;
    for (int i = 0; i < taskScrollview->getChildren()->count(); ++i)
    {
        UIButton* button_job = static_cast<UIButton*>(taskScrollview->getChildByName(CCString::createWithFormat("Button_%d",i)->getCString()));
        button_job->setTag(TAG_JOB_ITEM + i);
        button_job->setVisible(false);
        button_job->addTouchEventListener(this, toucheventselector(PopUpUI::menuTaskInfoCallback));
    }
    
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    panel0->setTouchEnabled(true);
    panel0->addTouchEventListener(this,toucheventselector(PopUpUI::taskCloseTipsCallback));
    
    Button* close_button = static_cast<Button*>(panel0->getChildByName("Button_closed"));
    close_button->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    Button* button_finish = static_cast<Button*>(panel2->getChildByName("Button_done"));
    button_finish->addTouchEventListener(this, toucheventselector(PopUpUI::menuFinishCallback));
    
    
    CCArray* arr = GlobalData::getInstance()->getTaskDataArray();

    int maxVisibleNum = arr->count() > taskScrollview->getChildren()->count() ? taskScrollview->getChildren()->count() : arr->count();
    for(int i = 0; i < maxVisibleNum; ++i)  // cocostudio设置任务列表默认为15个 大于15个没有显示 所以以默认为参照
    {
        TaskData * task = (TaskData *)arr->objectAtIndex(i);
        UIButton* taskButton = static_cast<UIButton*>(taskScrollview->getChildByName(CCString::createWithFormat("Button_%d",i)->getCString()));
        taskButton->setTag(task->getTaskIndex());
        taskButton->setVisible(true);
        UILabel* title_Text = static_cast<UILabel*>(taskButton->getChildByName(CCString::createWithFormat("Label_%d",i)->getCString()));
        title_Text->setText(CCString::createWithFormat("%s: %s",task->getTaskType().c_str(),task->getTitle().c_str())->getCString());
        adjust_MoveSize = current_MoveSize - taskButton->getPosition().y + taskButton->getContentSize().height/2;
    }
    if(adjust_MoveSize < view_Height )
    {
        adjust_MoveSize = view_Height;
    }
    taskScrollview->setInnerContainerSize(CCSize(view_Width,adjust_MoveSize)); // 调整scroll拖拽区域
    
    for(int i = 0;i < taskScrollview->getInnerContainer()->getChildren()->count(); ++i)
    {
        UIWidget* child = (UIWidget*)taskScrollview->getInnerContainer()->getChildren()->objectAtIndex(i);
        child->setPosition(ccpSub(child->getPosition(), ccp(0,current_MoveSize - adjust_MoveSize)));
    }
    if(arr->count() > 0)
    {
        TaskData * task = (TaskData *)arr->objectAtIndex(0); // 默认第一个任务
        if(task)
        {
            updateTaskInfo(task->getTaskIndex());
        }
    }
}

void PopUpUI::taskCloseTipsCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        updateTaskTipsUI(false);
    }
}

void PopUpUI::updateTaskTipsUI(bool bShow, int nItem)
{
    UIPanel *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    UIPanel * panel2_1 = static_cast<UIPanel *>(panel2->getChildByName("Panel2_1"));
    panel2_1->setVisible(bShow);
    if (bShow)
    {
        TaskData * task = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(m_nChooseTaskIndex);
        
        UILabel* label_0 = static_cast<UILabel *>(panel2_1->getChildByName("Label_0"));
        UILabel* label_1 = static_cast<UILabel *>(panel2_1->getChildByName("Label_1"));
        
        CCArray* array1 = split_string(((CCString*)task->getItemAwardArray()->objectAtIndex(nItem))->getCString(),',');
        UIImageView* image = UIImageView::create();
       
    }
}

void PopUpUI::popUpDialogInterface(YYNPC *npc)
{
    int count = npc->getTaskDataArray()->count();
    if(count == 1)
    {
        m_nAcceptedTaskIndex = static_cast<CCString *>(npc->getTaskDataArray()->objectAtIndex(0))->intValue();
        showTaskInfoInDialogInterface(m_nAcceptedTaskIndex);
    }
    else
    {
        Controller::getInstance()->setGameStatus(kStatusUI);
        setUIType(kUITask);
        m_pUITouchGroup = TouchGroup::create();
        addChild(m_pUITouchGroup);
        m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
        string str = getFilePath("NewProject2_22",kDirTypeUI,kFileSuffixJson);
        Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(str.c_str());
        m_pUITouchGroup->addWidget(mainWidget);
        UIImageView* npcIcon =static_cast<UIImageView*>(m_pUITouchGroup->getWidgetByName("ImageView_npc"));
        std::string path = getFilePath(npc->getSpriteData()->getSpriteImageName(), kDirTypeIcon, kFileSuffixPng);
        npcIcon->loadTexture(path.c_str());
        Layout* panel_Right = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel_1"));
        Layout* panel_RightDown = static_cast<Layout*>(panel_Right->getChildByName("Panel_2"));
        UIImageView* title = static_cast<UIImageView*>(panel_Right->getChildByName("Image_title"));
        UILabel *npc_Name = static_cast<UILabel*>(title->getChildByName("Label_npcname_Clone"));
        npc_Name->setText(npc->getSpriteName()->getCString());
        UIButton *close_button = static_cast<UIButton*>(panel_Right->getChildByName("Button_closed"));
        close_button->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
        if(count == 0)
        {
            panel_RightDown->removeFromParent();
            UILabel* description = static_cast<UILabel*>(panel_Right->getChildByName("TextArea"));
            description->setAnchorPoint(ccp(0,1));
            description->setText(npc->getSpriteData()->getSpriteTalkContent().c_str());
        }
        else
        {
            setTaskUIType(kNPCTaskList);
            UIScrollView* scrollView = static_cast<UIScrollView*>(panel_RightDown->getChildByName("ScrollView"));
            float view_Width = scrollView->getSize().width;
            float view_Height = scrollView->getSize().height;
            for (int i = 1; i < scrollView->getChildren()->count(); ++i)
            {
                UIButton* button_job = static_cast<UIButton*>(scrollView->getChildren()->objectAtIndex(i));
                button_job->setTag(TAG_SHOULD_ACCEPT_JOB + i);
                button_job->setVisible(false);
                button_job->addTouchEventListener(this, toucheventselector(PopUpUI::menuTaskInfoCallback2));
            }
            
            float current_MoveSize = scrollView->getInnerContainer()->getContentSize().height;
            float adjust_MoveSize = 0;
            int max_ShowCount = count > scrollView->getChildren()->count() - 1 ? scrollView->getChildren()->count() - 1 : count;
            for(int i = 0; i < max_ShowCount; ++ i)
            { // 默认最大显示数为5
                int taskIndex = static_cast<CCString *>(npc->getTaskDataArray()->objectAtIndex(i))->intValue();
                TaskData * task = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
                UIButton* button_job = static_cast<UIButton*>(scrollView->getChildren()->objectAtIndex(i+1)); // 第一个为NPC功能
                button_job->setTag(task->getTaskIndex());
                button_job->setVisible(true);
                UILabel* title_Text = static_cast<UILabel*>(button_job->getChildByName("TextArea"));
                title_Text->setText(CCString::createWithFormat("%s%s",task->getTaskType().c_str(),task->getTitle().c_str())->getCString());
                
                adjust_MoveSize = current_MoveSize - button_job->getPosition().y + button_job->getContentSize().height/2;
            }
            if(adjust_MoveSize < view_Height )
            {
                adjust_MoveSize = view_Height;
            }
            scrollView->setInnerContainerSize(CCSize(view_Width,adjust_MoveSize)); // 调整scroll拖拽区域
            
            for(int i = 0;i < scrollView->getInnerContainer()->getChildren()->count(); ++i)
            {
                UIWidget* child = (UIWidget*)scrollView->getInnerContainer()->getChildren()->objectAtIndex(i);
                child->setPosition(ccpSub(child->getPosition(), ccp(0,current_MoveSize - adjust_MoveSize)));
            }
        }
    }
}

void PopUpUI::menuAcceptCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        updateTaskProcess(m_nAcceptedTaskIndex);
    }
}

void PopUpUI::updateTaskProcess(int taskIndex)
{
    TaskData * task = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
    if (!task)
    {
        CCLOG("taskIndex error:%d",taskIndex);
        return;
    }
    TaskTarget * taskTarget = (TaskTarget *)task->getTargetArray()->objectAtIndex(0);
    m_pTaskNPC = (YYNPC *)Controller::getInstance()->getScene()->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(taskTarget->getTargetID());
    if(task->getStatus() == kTaskStatusOver || task->getStatus() == kTaskStatusFinding)
    {
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        if(getChildByTag(TAG_UI_NPC_TEMPLAYER))
        {
            removeChildByTag(TAG_UI_NPC_TEMPLAYER);
        }
        Controller::getInstance()->setGameStatus(kStatusScene);
        if(task->getStatus() == kTaskStatusFinding)
        {
            m_bWaitingForFindTarget = true;
        }
        else if (task->getStatus() == kTaskStatusOver)
        {
            m_bWaitingForUpdateTask = true;
            playTaskOverEffect();
            
        }
    }
    const char *message = CCString::createWithFormat(ASK_TASK_FROM_NPC,m_pTaskNPC->getNPCID().c_str(),taskIndex)->getCString();
    Controller::getInstance()->sendMessage(message,true);
}

void PopUpUI::playTaskOverEffect()
{
    YYSprite *sprite = YYSprite::createWithActionSet("se9", "se21", kDirTypeEffect);
    sprite->setAction(0, false);
    sprite->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2,CCDirector::sharedDirector()->getWinSize().height / 2));
    addChild(sprite,1);
    Controller::getInstance()->getScene()->getSpriteLayer()->getEffectArray()->addObject(sprite);
}

void PopUpUI::updateTaskResult(int taskIndex)
{
    TaskData * taskData = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
    if(taskData)
    {
        switch (taskData->getTargetType())
        {
            case kTaskTalk:
            {
                m_pRoleByControlled->taskAutoFindingWay(taskData,true);
                if(Controller::getInstance()->getGameStatus() == kStatusUI)
                {
                    removeChild(m_pUITouchGroup);
                    m_pUITouchGroup = NULL;
                    Controller::getInstance()->setGameStatus(kStatusScene);
                }
            }
                break;
            case kTaskKillMonster:
            {
                for (int i = 0 ; i < taskData->getTargetArray()->count(); i ++)
                {
                    TaskTarget *taskTarget = (TaskTarget *)taskData->getTargetArray()->objectAtIndex(i);
                    if(taskTarget->getTargetMonsterCount() < taskTarget->getTargetMonsterAmount())
                    {
                        m_pRoleByControlled->taskAutoFindingWay(taskData,false);
                        if(Controller::getInstance()->getGameStatus() == kStatusUI)  // 判断是否在查看任务界面点击的右上角
                        {
                            removeChild(m_pUITouchGroup);
                            m_pUITouchGroup = NULL;
                            Controller::getInstance()->setGameStatus(kStatusScene);
                        }
                        break;
                    }
                    else
                    {
                        if (i == taskData->getTargetArray()->count() - 1)
                        {
                            const char * message = CCString::createWithFormat(SEND_TASK_COMPLETE,m_nAcceptedTaskIndex)->getCString();
                            Controller::getInstance()->sendMessage(message,true);
                            if(taskData->getStatus() == kTaskStatusOver || taskData->getStatus() == kTaskStatusFinding)
                            {
                                if(Controller::getInstance()->getGameStatus() == kStatusUI)
                                {
                                    removeChild(m_pUITouchGroup);
                                    m_pUITouchGroup = NULL;
                                    Controller::getInstance()->setGameStatus(kStatusScene);
                                }
                                playTaskOverEffect();
                            }
                        }
                    }
                }
            }
                break;
            case kTaskCollect:
            {
                for (int i = 0 ; i < taskData->getTargetArray()->count(); i ++) {
                    TaskTarget *taskTarget = (TaskTarget *)taskData->getTargetArray()->objectAtIndex(i);
                    if(taskTarget->getTargetItemCount() < taskTarget->getTargetItemAmount())
                    {
                        m_pRoleByControlled->taskAutoFindingWay(taskData,false);
                        if(Controller::getInstance()->getGameStatus() == kStatusUI)
                        {
                            removeChild(m_pUITouchGroup);
                            m_pUITouchGroup = NULL;
                            Controller::getInstance()->setGameStatus(kStatusScene);
                        }
                        break;
                    }else{
                        if (i == taskData->getTargetArray()->count() - 1)
                        {
                            const char *message = CCString::createWithFormat(SEND_TASK_COMPLETE,m_nAcceptedTaskIndex)->getCString();
                            Controller::getInstance()->sendMessage(message,true);
                            if(taskData->getStatus() == kTaskStatusOver || taskData->getStatus() == kTaskStatusFinding){
                                if(Controller::getInstance()->getGameStatus() == kStatusUI)
                                {
                                    removeChild(m_pUITouchGroup);
                                    m_pUITouchGroup = NULL;
                                    Controller::getInstance()->setGameStatus(kStatusScene);
                                }
                                playTaskOverEffect();
                            }
                        }
                    }
                }
            }
                break;
            case kTaskPassStage:
            {
                for (int i = 0 ; i < taskData->getTargetArray()->count(); i++)
                {
                    TaskTarget *taskTarget = (TaskTarget *)taskData->getTargetArray()->objectAtIndex(i);
                    if(taskTarget->getClearStageCount() < taskTarget->getClearStageAmount())
                    {
                        m_pRoleByControlled->taskAutoFindingWay(taskData,false);
                        if(Controller::getInstance()->getGameStatus() == kStatusUI)
                        {
                            removeChild(m_pUITouchGroup);
                            m_pUITouchGroup = NULL;
                            Controller::getInstance()->setGameStatus(kStatusScene);
                        }
                        break;
                    }else{
                        if (i == taskData->getTargetArray()->count() - 1)
                        {
                            const char *message =CCString::createWithFormat(SEND_TASK_COMPLETE,m_nAcceptedTaskIndex)->getCString();
                            Controller::getInstance()->sendMessage(message,true);
                            if(taskData->getStatus() == kTaskStatusOver || taskData->getStatus() == kTaskStatusFinding)
                            {
                                if(Controller::getInstance()->getGameStatus() == kStatusUI)
                                {
                                    removeChild(m_pUITouchGroup);
                                    m_pUITouchGroup = NULL;
                                    Controller::getInstance()->setGameStatus(kStatusScene);
                                }
                                playTaskOverEffect();
                            }
                        }
                    }
                }
            }
                break;
            case kTaskLevelUp:
            {
                int roleLevel = m_pRoleByControlled->getLevel();
                TaskTarget *taskTarget = (TaskTarget *)taskData->getTargetArray()->objectAtIndex(0);
                if(roleLevel < taskTarget->getPlayerLevel())
                {
                    CCMessageBox(g_sLevelLow, g_sPowerNeedUpgrade);
                }
                else
                {
                    //                const char * message = CCString::createWithFormat("mi ND%d_%d",m_nAcceptedTaskIndex)->getCString();
                    //                Controller::getInstance()->sendMessage(message);
                    //                Controller::getInstance()->getScene()->setNetLoading(true);
                    //                if(task->getStatus() == kTaskStatusOver
                    //                   || task->getStatus() == kTaskStatusFinding)
                    //                {
                    //                    if(Controller::getInstance()->getGameStatus() == kStatusUI)
                    //                    {
                    //                        removeChild(m_pUITouchGroup);
                    //                        Controller::getInstance()->setGameStatus(kStatusScene);
                    //                    }
                    //                    handleTaskOverEffect();
                    //                }
                }
            }
                break;
            default:
                break;
        }
        ((YYScene*)getParent())->getSpriteLayer()->openPortal(taskData->getStatus());
    }
}

void PopUpUI::menuFinishCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        if(m_bIsOpenTask)
        {
            m_nAcceptedTaskIndex = m_nChooseTaskIndex;
            updateTaskResult(m_nAcceptedTaskIndex);
        }
        else
        {
            CCMessageBox("", g_sSelectTask);
            return;
        }
    }
}

void PopUpUI::updateTaskUI()
{
    if (m_bTouchBegan)
    {
        m_bTouchBegan = false;
        YYButton* guideButton = Controller::getInstance()->getScene()->getControlUILayer()->getGuideButton();
        if(guideButton)
        {
            if(Controller::isButtonTouched(guideButton,m_CurTouchPoint))
            {
                if(guideButton->getChildByTag(1))
                {
                    const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                    PLAY_EFFECT(filePath.c_str());
                    if(getTaskUIType() == kDialogInterface)
                    {
                        updateTaskProcess(guideButton->getIndex());
                    }
                    else if(getTaskUIType() == kNPCTaskList)
                    {
                        exitUI();
                        showTaskInfoInDialogInterface(guideButton->getIndex());
                    }
                    else
                    {
                        m_nAcceptedTaskIndex = guideButton->getIndex();
                        updateTaskResult(guideButton->getIndex());
                    }
                }
            }
        }
    }
}

void PopUpUI::exitTaskUI()
{

    if(getChildByTag(TAG_UI_NPC_TEMPLAYER))
    {
        removeChildByTag(TAG_UI_NPC_TEMPLAYER);
    }
    setTaskUIType(kTaskUIDefault);
    m_bIsOpenTask = false;
}

void PopUpUI::menuTaskInfoCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if ( Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        UIWidget* widget = static_cast<UIWidget*>(pSender);
        if(widget->isVisible())
        {
            updateTaskInfo(widget->getTag());
        }
    }
}

void PopUpUI::menuTaskInfoCallback2(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        exitUI();
        UIWidget* widget = static_cast<UIWidget*>(pSender);
        showTaskInfoInDialogInterface(widget->getTag());
    }
}

void PopUpUI::showTaskInfoInDialogInterface(int taskIndex)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_eUIType == kUITask && m_eTaskUIType == kDialogInterface)
    {
        return;
    }
    Controller::getInstance()->setGameStatus(kStatusUI);
    m_eUIType = kUITask;
    m_eTaskUIType = kDialogInterface;
    TaskData * task = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
    m_nAcceptedTaskIndex = taskIndex;
    // 对话任务只有一个任务目标（与xxNPC对话）
    TaskTarget *taskTarget = static_cast<TaskTarget *>(task->getTargetArray()->objectAtIndex(0));
    YYSprite* targetSprite = (YYSprite*)Controller::getInstance()->getScene()->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(taskTarget->getTargetID());
    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    string str = getFilePath("NewProject2_23",kDirTypeUI,kFileSuffixJson);
    Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(str.c_str());
    m_pUITouchGroup->addWidget(mainWidget);
    UIImageView* npcIcon =static_cast<UIImageView*>(m_pUITouchGroup->getWidgetByName("ImageView_npc"));
    std::string path = getFilePath(targetSprite->getSpriteData()->getSpriteImageName(), kDirTypeIcon, kFileSuffixPng);
    npcIcon->loadTexture(path.c_str());
    Layout* panel= static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel_1"));

    UILabel* task_Definition = static_cast<UILabel*>(panel->getChildByName("TextArea"));
    task_Definition->setText(taskTarget->getDialogContent());
    task_Definition->setAnchorPoint(ccp(0,1));
    m_pTaskDialogueInfo = task_Definition;
    
    UIImageView* title = static_cast<UIImageView*>(panel->getChildByName("Image_title"));
    UILabel* npc_Name = static_cast<UILabel*>(title->getChildByName("Label_npcname_Clone_Clone"));
    npc_Name->setText(targetSprite->getSpriteName()->getCString());
    Layout* panel_Award = static_cast<Layout*>(panel->getChildByName("Panel_2"));
    for(int i = 0;i < 3; i++)
    {
        UIWidget* icon_job = static_cast<UIWidget*>(panel_Award->getChildByName(CCString::createWithFormat("Image_icon%d",i)->getCString()));
        icon_job->setVisible(false);
        if (i < task->getItemAwardArray()->count())
        {
            CCArray* array1 = split_string(((CCString*)task->getItemAwardArray()->objectAtIndex(i))->getCString(),',');
            icon_job->setVisible(true);
            UIImageView* icon_Image = static_cast<UIImageView*>(icon_job->getChildByName("ImageView00"));
            string str = getFilePath(((CCString*)array1->objectAtIndex(1))->getCString(), kDirTypeIcon, kFileSuffixPng);
            icon_Image->loadTexture(str.c_str());
            int num = ((CCString*)array1->objectAtIndex(2))->intValue();
            UILabel* ptemp = static_cast<Label *>(icon_job->getChildByName("Label_0"));
            ptemp->setText(CCString::createWithFormat(" %d",num)->getCString());
        }
    }
    for(int i = 0;i < 3; ++i)
    {
        UIWidget* icon_job = ((UIWidget*)panel_Award->getChildren()->objectAtIndex(i + 1));
        UILabel* icon_Text = static_cast<UILabel*>(icon_job->getChildByName("TextArea0"));
        if (i == 0)
        {
            icon_Text->setText(CCString::createWithFormat("%d",task->getGoldAwardNum())->getCString());
        }
        if (i == 1)
        {
            icon_Text->setText(CCString::createWithFormat("%d",task->getExpAwardNum())->getCString());
        }
        if (i == 2)
        {
            icon_Text->setText(CCString::createWithFormat("%d",task->getJewelAwardNum())->getCString());
        }
    }
    
    UIButton* close_button = static_cast<UIButton*>(m_pUITouchGroup->getWidgetByName("Button_closed"));
    close_button->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    UIButton* accept_button = static_cast<UIButton*>(m_pUITouchGroup->getWidgetByName("Button0"));
    accept_button->addTouchEventListener(this, toucheventselector(PopUpUI::menuAcceptCallback));
    UILabel* button_Text = static_cast<UILabel*>(accept_button->getChildByName("TextArea_ui"));
    
    switch (task->getStatus()) {
        case kTaskStatusAccept:
        {
            button_Text->setText(g_sAccept);
        }
            break;
        case kTaskStatusContinue:
        {
            button_Text->setText(g_sContinue);
        }
            break;
        case kTaskStatusOver:
        {
            button_Text->setText(g_sDone);
        }
            break;
        case kTaskStatusFinding:
        {
            button_Text->setText(g_sSeek);
        }
            break;
            
        default:
            break;
    }
    m_pTaskDialogueButton = button_Text;
}

void PopUpUI::updateTaskInfo(int taskIndex)
{
    setTaskUIType(kTaskList);
    TaskData * task = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
    m_nChooseTaskIndex = taskIndex;
    m_bIsOpenTask = true;
    
    Layout* panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    Layout* panel2_0 = static_cast<Layout*>(panel2->getChildByName("Panel2_0"));
    
    Label *desLabel = static_cast<Label *>(panel2_0->getChildByName("Label0"));
    Label *desContent = static_cast<Label *>(desLabel->getChildByName("Label"));
    desContent->setText(task->getDescription());
    Label *targetLabel = static_cast<Label *>(panel2_0->getChildByName("Label1"));
    Label *targetContent = static_cast<Label *>(targetLabel->getChildByName("Label"));
    
    switch (task->getTargetType())
    {
        case kTaskTalk:
        {
            TaskTarget *taskTarget = (TaskTarget *)task->getTargetArray()->objectAtIndex(0);
            YYSprite *targetSprite = (YYSprite *)Controller::getInstance()->getScene()->getSpriteLayer()->getSpriteWithServerIDDict()->objectForKey(taskTarget->getTargetID());
            if(targetSprite)
            {
                targetContent->setText(targetSprite->getSpriteName()->getCString());
            }
        }
            break;
        case kTaskKillMonster:
        {
            std::string content = "";
            for (int i = 0 ; i < task->getTargetArray()->count(); i ++)
            {
                TaskTarget *taskTarget = (TaskTarget *)task->getTargetArray()->objectAtIndex(i);
                content.append(taskTarget->getTargetName());
                content.append(CCString::createWithFormat(": %d",taskTarget->getTargetMonsterCount())->getCString());
                content.append(CCString::createWithFormat(" / %d",taskTarget->getTargetMonsterAmount())->getCString());
                content.append("\n");
            }
            targetContent->setText(content);
        }
            break;
        case kTaskCollect:
        {
            std::string content = "";
            for (int i = 0 ; i < task->getTargetArray()->count(); i ++)
            {
                TaskTarget *taskTarget = (TaskTarget *)task->getTargetArray()->objectAtIndex(i);
                ;
                content.append(taskTarget->getTargetName());
                content.append(CCString::createWithFormat(": %d",taskTarget->getTargetItemCount())->getCString());
                content.append(CCString::createWithFormat(" / %d",taskTarget->getTargetItemAmount())->getCString());
                content.append("\n");
            }
            targetContent->setText(content);
        }
            break;
        case kTaskPassStage:
        {
            std::string content = "";
            for (int i = 0 ; i < task->getTargetArray()->count(); i ++)
            {
                TaskTarget *taskTarget = (TaskTarget *)task->getTargetArray()->objectAtIndex(i);
                content.append(taskTarget->getDungeonsName());
                content.append(" ");
                content.append(taskTarget->getStageName());
                content.append(g_sDiffculty[taskTarget->getStageLevel()]);
                content.append(CCString::createWithFormat(": %d",taskTarget->getClearStageCount())->getCString());
                content.append(CCString::createWithFormat(" / %d",taskTarget->getClearStageAmount())->getCString());
                content.append("\n");
            }
            targetContent->setText(content);
        }
            break;
        case kTaskLevelUp:
        {
            TaskTarget *taskTarget = (TaskTarget *)task->getTargetArray()->objectAtIndex(0);
            targetContent->setText(CCString::createWithFormat(g_sUpgradeFmt,taskTarget->getPlayerLevel())->getCString());
        }
            break;
        default:
            break;
    }
    
    UIButton* button_finish = static_cast<UIButton*>(panel2->getChildByName("Button_done"));
    switch (task->getStatus())
    {
        case kTaskStatusAccept:
        {
            button_finish->setTitleText(g_sAccept);
        }
            break;
        case kTaskStatusContinue:
        {
            button_finish->setTitleText(g_sContinue);
        }
            break;
        case kTaskStatusOver:
        {
            button_finish->setTitleText(g_sDone);
        }
            break;
        case kTaskStatusFinding:
        {
            button_finish->setTitleText(g_sSeek);
        }
            break;
            
        default:
            break;
    }
    Layout* label2 = static_cast<Layout*>(panel2_0->getChildByName("Label2"));
    
    // 金币
    UIImageView* goldIcon = static_cast<UIImageView *>(panel2_0->getChildByName("ImageView_0"));
    Label *goldLabel = static_cast<Label *>(label2->getChildByName("Label_money"));
    Label *goldValue = static_cast<Label *>(goldLabel->getChildByName("Label"));
    if (task->getGoldAwardNum() > 0)
    {
        goldValue->setText(CCString::createWithFormat("%d",task->getGoldAwardNum())->getCString());
        goldIcon->setVisible(true);
    }
    else
    {
        goldValue->setVisible(false);
        goldIcon->setVisible(false);
    }
    
    // 经验
    UIImageView* expIcon = static_cast<UIImageView *>(panel2_0->getChildByName("ImageView_1"));
    Label *expLabel = static_cast<Label *>(label2->getChildByName("Label_exp"));
    Label *expValue = static_cast<Label *>(expLabel->getChildByName("Label"));
    if (task->getExpAwardNum() > 0)
    {
        expValue->setText(CCString::createWithFormat("%d",task->getExpAwardNum())->getCString());
        expIcon->setVisible(true);
    }
    else
    {
        expValue->setVisible(false);
        expIcon->setVisible(false);
    }
    
    // 宝石
    UIImageView* jewIcon = static_cast<UIImageView *>(panel2_0->getChildByName("ImageView_2"));
    Label *jewLabel = static_cast<Label *>(label2->getChildByName("Label_jew"));
    Label *jewValue = static_cast<Label *>(jewLabel->getChildByName("Label"));
    if (task->getJewelAwardNum() > 0)
    {
        jewValue->setText(CCString::createWithFormat("%d",task->getJewelAwardNum())->getCString());
        jewIcon->setVisible(true);
    }
    else
    {
        jewValue->setVisible(false);
        jewIcon->setVisible(false);
    }
    
    // 物品
    for(int i = 0; i < 3; i++)
    {
        ImageView *icon = static_cast<ImageView *>(label2->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
        icon->setVisible(false);
        if(i < task->getItemAwardArray()->count())
        {
            icon->setVisible(true);
            icon->removeChildByTag(10);
            CCArray* array1 = split_string(((CCString*)task->getItemAwardArray()->objectAtIndex(i))->getCString(),',');
            UIImageView* image = UIImageView::create();
            if (array1->count() > 1)
            {
                const char *iconName = ((CCString*)array1->objectAtIndex(1))->getCString();
                string str = getFilePath(iconName, kDirTypeIcon, kFileSuffixPng);
                image->loadTexture(str.c_str());
                image->setTag(10);
                icon->addChild(image);
                Label *label1= static_cast<Label *>(icon->getChildByName("Label_0"));
                if (array1->count() > 2)
                {
                    CCString * num = CCString::createWithFormat("x%s",((CCString*)array1->objectAtIndex(2))->getCString());
                    label1->setVisible(true);
                    label1->setText(num->getCString());
                }
            }
        }
    }
    
    // 等级
    UILabel* label3 = static_cast<UILabel *>(panel2_0->getChildByName("Label3"));
    UILabel* levLabel =static_cast<UILabel *>(label3->getChildByName("Label"));
    char tmp[10] = {0};
    sprintf(tmp, "%d",task->getNeedLevel());
    levLabel->setText(tmp);
}
