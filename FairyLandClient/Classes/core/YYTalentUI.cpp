//
//  YYTalentUI.cpp
//  Fairyland
//
//  Created by fuchenhao on 14-4-21.
//
//

#include "YYPopUpUI.h"

#pragma mark 天赋

void PopUpUI::initTalentUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUITalent);
    m_nSkillIconIndex = 0;  // 默认选中第一个技能
    m_pTalentBtn = NULL;    // 默认不选中
    m_pUITouchGroup = TouchGroup::create();
    if (m_pUITouchGroup)
    {
        addChild(m_pUITouchGroup);
        m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
        const std::string filePath = TALENT_UI_RESOURCE_FILE_NAME;
        m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
        
        // 关闭
        Layout * panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
        Button * closeButton = static_cast<Button *>(panel0->getChildByName("Button_closed"));
        if (closeButton)
        {
            closeButton->setTag(kButtonClose);
            closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
        }
        
        // 帮助
        UIButton *helpButton = static_cast<UIButton *>(panel0->getChildByName("Button_0"));
        helpButton->setZOrder(6);
        if(helpButton)
        {
            helpButton->addTouchEventListener(this, toucheventselector(PopUpUI::helpButtonCallBack));
            ImageView * image = static_cast<ImageView *>(helpButton->getChildByName("ImageView_0"));
            image->addTouchEventListener(this, toucheventselector(PopUpUI::hideHelpInterface));
            image->setTag(kButtonHelp);
            image->setEnabled(false);
            image->setZOrder(20);
        }
        
        Layout *panel1 = static_cast<Layout *>(panel0->getChildByName("Panel1"));
        Layout *panel1_2 = static_cast<Layout *>(panel1->getChildByName("Panel1_2"));
        
        // 重置按钮
        UIButton *resetButton = static_cast<UIButton *>(panel1_2->getChildByName("Button"));
        if (resetButton)
        {
            resetButton->addTouchEventListener(this, toucheventselector(PopUpUI::resetCallBack));
            resetButton->setTag(kTalentUIResetButton);
        }
        
        Layout *panel1_1 = static_cast<Layout *>(panel1->getChildByName("Panel1_1"));
        int skillCount = GlobalData::getInstance()->getUsableSkillDataArray()->count();
        for (int i = 0; i < panel1_1->getChildrenCount(); i++)
        {
            ImageView *imageView = static_cast<ImageView *>(panel1_1->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
            Label * label = static_cast<Label *>(imageView->getChildByName("TextArea0")); // 技能拥有天赋点数
            UILabel* labName = static_cast<UILabel *>(imageView->getChildByName("Label_0"));
            if (i < skillCount)
            {
                SkillData *skillData = static_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(i));
                if (skillData)
                {
                    imageView->addTouchEventListener(this, toucheventselector(PopUpUI::skillSwitchCallBack));
                    imageView->setTag(kTalentUISkillIcon1 + i);
                    
                    UIImageView* icon = UIImageView::create();
                    const std::string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                    icon->loadTexture(filePath.c_str());
                    imageView->addChild(icon);
                    if (i == m_nSkillIconIndex)
                    {
                        ImageView *frameIcon = static_cast<ImageView *>(imageView->getChildByName("ImageView")); // 默认显示第一个选中框
                        frameIcon->setVisible(true);
                    }
                    label->setText(CCString::createWithFormat("%d",skillData->getTalentPoint())->getCString()); // 技能包含天赋点数
                    labName->setZOrder(10);
                    labName->setText(skillData->getSkillName()); //技能名称
                }
            }
        }
        
        // 设置角色天赋点数 宝石数
        updatePlayerTalentPointLabel();
        
        // 隐藏金币
        ImageView *imageView0 = static_cast<ImageView *>(panel1_2->getChildByName("ImageView0"));
        imageView0->setEnabled(false);
        imageView0->setVisible(false);
        
        // 设置天赋信息
        UIPanel* panel1_0   = static_cast<UIPanel *>(panel1->getChildByName("Panel1_0"));
        UIPanel *panel1_1_0 = static_cast<UIPanel *>(panel1_0->getChildByName("Panel1_1_0"));
        setTalentInfo(panel1_1_0);//设置技能天赋信息，在此之前应先设置角色天赋点数，以判断技能天赋点是否可以加减
        
        // 隐藏天赋详细信息面板回调
        UIImageView* imageViewTmp = static_cast<UIImageView *>(panel1_0->getChildByName("ImageView0"));
        imageViewTmp->setTouchEnabled(true);
        imageViewTmp->addTouchEventListener(this, toucheventselector(PopUpUI::hideTalentDescription));
    }
}

#pragma mark 更新技能天赋UI 数据

void PopUpUI::updateTalentUI()
{
    Layout * panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    Layout *panel1 = static_cast<Layout *>(panel0->getChildByName("Panel1"));
    Layout *panel1_1 = static_cast<Layout *>(panel1->getChildByName("Panel1_1"));
    UIPanel* panel1_0   = static_cast<UIPanel *>(panel1->getChildByName("Panel1_0"));
    UIPanel *panel1_1_0 = static_cast<UIPanel *>(panel1_0->getChildByName("Panel1_1_0"));
    UIPanel* panel2 = static_cast<UIPanel *>(panel0->getChildByName("Panel2"));
    
    // 技能包含天赋数
    
    int skillCount = GlobalData::getInstance()->getUsableSkillDataArray()->count();
    for (int i = 0; i < panel1_1->getChildrenCount(); i++)
    {
        ImageView *imageView = static_cast<ImageView *>(panel1_1->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
        Label * label = static_cast<Label *>(imageView->getChildByName("TextArea0")); // 技能拥有天赋点数
        if (i < skillCount)
        {
            SkillData *skillData = static_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(i));
            if (skillData)
            {
                label->setText(CCString::createWithFormat("%d",skillData->getTalentPoint())->getCString()); // 技能包含天赋点数
               
            }
        }
    }
    
    // 天赋点数更新
    SkillData *skillData = static_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));// 当前选中的技能
        CCArray *array = static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataArrayDict()->objectForKey(skillData->getIndex()));
        for (int i = 0; i < array->count(); i++)
        {
            TalentData *talentData = static_cast<TalentData *>(array->objectAtIndex(i));
            UIButton * button = static_cast<UIButton *>(panel1_1_0->getChildByName(CCString::createWithFormat("Button%d_%d",talentData->getLayerIndex(),talentData->getPositionIndex())->getCString()));
            button->setUserData(talentData);
            // 当前点数
            Label *currentPointsLabel = static_cast<Label *>(button->getChildByName("Label0"));
            currentPointsLabel->setText(CCString::createWithFormat("%d", talentData->getLevel())->getCString());
            
            // 最大天赋点数
            UILabel* maxLabel = static_cast<UILabel *>(button->getChildByName("Label2"));
            maxLabel->setText(CCString::createWithFormat("%d", talentData->getMaxPoint())->getCString());
        }
    
    // 天赋详细信息更新
    if (panel2->isVisible() && m_pTalentBtn)
    {
        UIButton* pBtn = static_cast<UIButton *>(m_pTalentBtn);
        TalentData *talentData = static_cast<TalentData *>(pBtn->getUserData());
        UIImageView* img = static_cast<UIImageView *>(pBtn->getChildByName("ImageView_0")); // 设置选中框
        img->setVisible(true);
        
        UIButton* btnUp = static_cast<UIButton *>(panel2->getChildByName("Button_up"));
        if (isTalentPointsAddable(skillData, talentData)) // 判断天赋是否可加点
        {
            btnUp->setEnabled(true);
        }
        else
        {
            btnUp->setEnabled(false);
        }
        
        UILabel* label0 = static_cast<UILabel *>(panel2->getChildByName("Label0")); // 当前效果描述
        UILabel* label_0 = static_cast<UILabel *>(panel2->getChildByName("Label_0"));// "当前效果"
        label0->setText(talentData->getDescription());
        label0->setVisible(true);
        label_0->setVisible(true);
        if (talentData->getLevel() == 0)
        {
            label0->setVisible(false);
            label_0->setVisible(false);
        }
        
        UILabel* label1 = static_cast<UILabel *>(panel2->getChildByName("Label1")); // 下一级效果描述
        UILabel* label_1 = static_cast<UILabel *>(panel2->getChildByName("Label_1")); // "下一级效果"
        if ((talentData->getLevel() < talentData->getMaxPoint()))
        {
            label1->setVisible(true);
            label_1->setVisible(true);
            CCArray* arrTalent =static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataDict()->objectForKey(talentData->getIndex()));
            TalentData* nextTalentData = static_cast<TalentData *>(arrTalent->objectAtIndex(talentData->getLevel()));
            label1->setText(nextTalentData->getDescription());
        }
        else // 天赋加到满点 不显示下一级效果
        {
            label1->setVisible(false);
            label_1->setVisible(false);
        }
        
        UILabel* label3_0 = static_cast<UILabel *>(panel2->getChildByName("Label3_0")); // 前置总点数
        label3_0->setText(CCString::createWithFormat("%d",talentData->getAmountTalentPoints())->getCString());
        
        UILabel* label2_0 = static_cast<UILabel *>(panel2->getChildByName("Label2_0")); // 前置天赋名称
        UILabel* label2 = static_cast<UILabel *>(panel2->getChildByName("Label2")); // "前置天赋"
        if (talentData->getPreposeTalentIndex() > 0)
        {
            label2->setVisible(true);
            label2_0->setVisible(true);
            CCArray* arrTalent =static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataDict()->objectForKey(talentData->getPreposeTalentIndex()));
            TalentData* preTalentData = static_cast<TalentData *>(arrTalent->objectAtIndex(0));
            label2_0->setText(preTalentData->getName());
        }
        else
        {
            label2->setVisible(false);
            label2_0->setVisible(false);
        }
    }
}

#pragma mark 更新玩家可加天赋点 宝石

void PopUpUI::updatePlayerTalentPointLabel()
{
    //  当前可加天赋点
    Layout * panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel * panel1 = static_cast<UIPanel *>(panel0->getChildByName("Panel1"));
    Layout *panel1_2 = static_cast<Layout *>(panel1->getChildByName("Panel1_2"));
    Label *talentPointTitle = static_cast<Label *>(panel1_2->getChildByName("TextArea0"));
    Label *talentPoint = static_cast<Label *>(talentPointTitle->getChildByName("TextArea"));
    talentPoint->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getTalentPoint())->getCString());
    
    // 宝石
    ImageView *imageView1 = static_cast<ImageView *>(panel1_2->getChildByName("ImageView1"));
    Label *jewelLabel = static_cast<Label *>(imageView1->getChildByName("TextArea"));
    int jewel = m_pRoleByControlled->getSpriteAttribute()->getJewel();
    jewelLabel->setText(CCString::createWithFormat("%d",jewel)->getCString());
}

#pragma mark 设置天赋信息 图标、名称、投入点数

void PopUpUI::setTalentInfo(cocos2d::gui::Widget *widget)
{
    SkillData *skillData = static_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));// 当前选中的技能
    do{
        CC_BREAK_IF(!skillData);
        CCArray *array = static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataArrayDict()->objectForKey(skillData->getIndex()));
        for (int i = 0; i < array->count(); i++)
        {
            TalentData *talentData = static_cast<TalentData *>(array->objectAtIndex(i));
            CC_BREAK_IF(!talentData);
            UIButton * button = static_cast<UIButton *>(widget->getChildByName(CCString::createWithFormat("Button%d_%d",talentData->getLayerIndex(),talentData->getPositionIndex())->getCString()));
            CC_BREAK_IF(!button);
            UIImageView * icon = static_cast<UIImageView *>(getChildByTag(10));
            if(!icon)
            {
                icon = ImageView::create();
                CC_BREAK_IF(!icon);
                const std::string filePath = getFilePath(talentData->getIconName(),kDirTypeIcon,kFileSuffixPng);
                icon->loadTexture(filePath.c_str());
                icon->setTag(10);
                button->addChild(icon);
                button->addTouchEventListener(this, toucheventselector(PopUpUI::showDescriptionCallBack));
                button->setTag(kTalentUITalentIcon1 + i);
                button->setUserData(talentData);
            }
            Label *talentNameLabel = static_cast<Label *>(button->getChildByName("Label_name"));
            if (talentNameLabel)
            {
                talentNameLabel->setText(talentData->getName().c_str());
            }

            // 当前点数
            Label *currentPointsLabel = static_cast<Label *>(button->getChildByName("Label0"));
            currentPointsLabel->setText(CCString::createWithFormat("%d", talentData->getLevel())->getCString());
            
            // 最大天赋点数
            UILabel* maxLabel = static_cast<UILabel *>(button->getChildByName("Label2"));
            maxLabel->setText(CCString::createWithFormat("%d", talentData->getMaxPoint())->getCString());
        }
        
    }while (0);
}

#pragma mark 判断天赋是否可加点

bool PopUpUI::isTalentPointsAddable(SkillData * skillData , TalentData *talentData)
{
    if (talentData->getLevel() < talentData->getMaxPoint() && m_pRoleByControlled->getSpriteAttribute()->getTalentPoint() > 0)// 有可用天赋点, 当前投入点数小于最大可投入点数
    {
        int nNeedSkillIdx = 0;
        if (talentData->getLevel() == 0)
        {
            nNeedSkillIdx = talentData->getSkillLevel();
        }
        else
        {
            CCArray* arrTmp = static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataDict()->objectForKey(talentData->getIndex()));
            TalentData * nextTalentData = static_cast<TalentData *>(arrTmp->objectAtIndex(talentData->getLevel()));
            nNeedSkillIdx = nextTalentData->getSkillLevel();
        }
        if(skillData->getLevel() >= nNeedSkillIdx) //技能等级 >= 天赋所需技能等级
        {
            int nPreTalentIdx  = talentData->getPreposeTalentIndex(); // 前置天赋id
            CCArray * arrTalent = static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataArrayDict()->objectForKey(skillData->getIndex()));
            if (nPreTalentIdx > 0)
            {
                for (int i = 0; i < arrTalent->count(); i++)
                {
                    TalentData * preposeTalentData = static_cast<TalentData *>(arrTalent->objectAtIndex(i));
                    if (preposeTalentData->getIndex() == nPreTalentIdx)
                    {
                        if ( preposeTalentData->getLevel() < talentData->getPreposeTalentPoints()) // 前置天赋点未达到
                        {
                            return false;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
            if (skillData->getTalentPoint() >= talentData->getAmountTalentPoints()) // 前置天赋点 >= 天赋所需前置天赋总点
            {
                return true;
            }
        }
    }
    return false;
}

#pragma mark 切换技能按钮回调

void PopUpUI::skillSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        Layout * panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
        Layout *panel1 = static_cast<Layout *>(panel0->getChildByName("Panel1"));
        Layout *panel1_0 = static_cast<Layout *>(panel1->getChildByName("Panel1_0"));
        Layout *panel1_1 = static_cast<Layout *>(panel1->getChildByName("Panel1_1"));
        ImageView *skillIcon = static_cast<ImageView *>(pSender);
        int i = -1;
        if (strcmp(skillIcon->getName(), "ImageView0") == 0)
        {
            i = 0;
        }
        else if (strcmp(skillIcon->getName(), "ImageView1") == 0)
        {
            i = 1;
        }
        else if (strcmp(skillIcon->getName(), "ImageView2") == 0)
        {
            i = 2;
        }
        else if (strcmp(skillIcon->getName(), "ImageView3") == 0)
        {
            i = 3;
        }
        else if (strcmp(skillIcon->getName(), "ImageView4") == 0)
        {
            i = 4;
        }
        if (i != m_nSkillIconIndex)
        {
            // 显示当前被选中的图标光圈，隐藏之前被选中的图标光圈
            ImageView *frame = static_cast<ImageView *>(skillIcon->getChildByName("ImageView"));
            frame->setVisible(true);
            ImageView *imageView0 = static_cast<ImageView *>(panel1_0->getChildByName("ImageView0"));
            ImageView *skillIcon = static_cast<ImageView *>(panel1_1->getChildByName(CCString::createWithFormat("ImageView%d",m_nSkillIconIndex)->getCString()));
            frame = static_cast<ImageView *>(skillIcon->getChildByName("ImageView"));
            frame->setVisible(false);
            
            // 隐藏旧页签显示新页签
            ImageView *frame0 = static_cast<ImageView *>(imageView0->getChildByName(CCString::createWithFormat("ImageView%d",m_nSkillIconIndex)->getCString()));
            frame0->setVisible(false);
            m_nSkillIconIndex = i;
            frame0 = static_cast<ImageView *>(imageView0->getChildByName(CCString::createWithFormat("ImageView%d",m_nSkillIconIndex)->getCString()));
            frame0->setVisible(true);
            UILayout *panel1_1_0 = static_cast<UILayout *>(panel1_0->getChildByName("Panel1_1_0"));
            setTalentInfo(panel1_1_0);
            
            // 隐藏天赋详细信息页面
            UIPanel* panel2 = static_cast<UIPanel *>(panel0->getChildByName("Panel2"));
            panel2->setVisible(false);
            hideTalentFrame(); //隐藏天赋选中框
        }
    }
}

#pragma mark 显示天赋描述页面

void PopUpUI::showDescriptionCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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

        hideTalentFrame(); // 隐藏选中框
        
        SkillData *skillData = static_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));// 当前选中的技能
        UIButton* pBtn = static_cast<UIButton *>(pSender);
        m_pTalentBtn = pBtn;
        TalentData *talentData = static_cast<TalentData *>(pBtn->getUserData());
        UIImageView* img = static_cast<UIImageView *>(pBtn->getChildByName("ImageView_0")); // 设置选中框
        img->setVisible(true);
        UIPanel* panel0 = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel0"));
        UIPanel* panel2 = static_cast<UIPanel *>(panel0->getChildByName("Panel2"));
        panel2->setVisible(true);
        
        UIButton* btnUp = static_cast<UIButton *>(panel2->getChildByName("Button_up"));
        if (isTalentPointsAddable(skillData, talentData)) // 判断天赋是否可加点
        {
            btnUp->setEnabled(true);
            btnUp->setTag(kTalentUIAddPointButton);
            btnUp->addTouchEventListener(this, toucheventselector(PopUpUI::talentAddPointCallBack));
        }
        else
        {
             btnUp->setEnabled(false);
        }
        
        UILabel* label0 = static_cast<UILabel *>(panel2->getChildByName("Label0")); // 当前效果描述
        UILabel* label_0 = static_cast<UILabel *>(panel2->getChildByName("Label_0"));// "当前效果"
        label0->setText(talentData->getDescription());
        label0->setVisible(true);
        label_0->setVisible(true);
        if (talentData->getLevel() == 0)
        {
            label0->setVisible(false);
            label_0->setVisible(false);
        }
        
        UILabel* label1 = static_cast<UILabel *>(panel2->getChildByName("Label1")); // 下一级效果描述
        UILabel* label_1 = static_cast<UILabel *>(panel2->getChildByName("Label_1")); // "下一级效果"
        if (talentData->getLevel() < talentData->getMaxPoint())
        {
            label1->setVisible(true);
            label_1->setVisible(true);
            CCArray* arrTalent =static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataDict()->objectForKey(talentData->getIndex()));
            TalentData* nextTalentData = static_cast<TalentData *>(arrTalent->objectAtIndex(talentData->getLevel()));
            label1->setText(nextTalentData->getDescription());
        }
        else // 天赋加到满点 不显示下一级效果
        {
            label1->setVisible(false);
            label_1->setVisible(false);
        }
       
        UILabel* label3_0 = static_cast<UILabel *>(panel2->getChildByName("Label3_0")); // 前置总点数
        label3_0->setText(CCString::createWithFormat("%d",talentData->getAmountTalentPoints())->getCString());
        
        UILabel* label2_0 = static_cast<UILabel *>(panel2->getChildByName("Label2_0")); // 前置天赋名称
        UILabel* label2 = static_cast<UILabel *>(panel2->getChildByName("Label2")); // "前置天赋"
        if (talentData->getPreposeTalentIndex() > 0)
        {
            label2->setVisible(true);
            label2_0->setVisible(true);
            CCArray* arrTalent =static_cast<CCArray *>(GlobalData::getInstance()->getTalentDataDict()->objectForKey(talentData->getPreposeTalentIndex()));
            TalentData* preTalentData = static_cast<TalentData *>(arrTalent->objectAtIndex(0));
            label2_0->setText(preTalentData->getName());
        }
        else
        {
            label2->setVisible(false);
            label2_0->setVisible(false);
        }
    }
}

#pragma mark 天赋加点回调

void PopUpUI::talentAddPointCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        TalentData* talentData =static_cast<TalentData *>(m_pTalentBtn->getUserData());
        const char *msg = CCString::createWithFormat(ASK_UPDATE_TALENT,talentData->getIndex())->getCString();
        Controller::getInstance()->sendMessage(msg,true);
    }
}

#pragma mark  隐藏天赋详细信息

void PopUpUI::hideTalentDescription(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    UIPanel* panel0 = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* panel2 = static_cast<UIPanel *>(panel0->getChildByName("Panel2"));
    panel2->setVisible(false);
    hideTalentFrame();
}

#pragma mark 隐藏天赋选中框

void PopUpUI::hideTalentFrame()
{
    UIPanel* panel0 = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* panel1 = static_cast<UIPanel *>(panel0->getChildByName("Panel1"));
    UIPanel* panel1_0 = static_cast<UIPanel *>(panel1->getChildByName("Panel1_0"));
    UIPanel* panel1_1_0 = static_cast<UIPanel *>(panel1_0->getChildByName("Panel1_1_0"));
    CCArray* arr = panel1_1_0->cocos2d::CCNode::getChildren();
    for (int i = 0; i < arr->count(); i++)
    {
        UIButton* btn = static_cast<UIButton *>(arr->objectAtIndex(i));
        UIImageView* img = static_cast<UIImageView *>(btn->getChildByName("ImageView_0"));
        img->setVisible(false);
    }
    m_pTalentBtn = NULL;
}

#pragma mark 重置按钮

void PopUpUI::resetCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        UIPanel* panel0 = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel0"));
        UIPanel* panel2 = static_cast<UIPanel *>(panel0->getChildByName("Panel2"));
        if (panel2->isVisible())  //显示天赋详情时 屏蔽重置按钮
        {
            return;
        }
        
        SkillData *skillData = static_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));// 当前选中的技能
        if (skillData)
        {
            int nHadJew =  m_pRoleByControlled->getSpriteAttribute()->getJewel();
            int nNeedJew = skillData->getResetSkillTalentJew();
            if (nHadJew <= 0 || nHadJew < nNeedJew)
            {
                 ((YYScene*)getParent())->showTips(g_sLackJew);
            }
            else
            {
                 setConsumeJewelTips(kJewelConsumeForTalentReset, nNeedJew);
            }
        }
    }
}
