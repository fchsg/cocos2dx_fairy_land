//
//  YYSkillUI.cpp
//  Fairyland
//
//  Created by fuchenhao on 14-4-21.
//
//

#include "YYPopUpUI.h"

#pragma mark 技能

void PopUpUI::initSkillUI()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUISkill);
    m_pRoleByControlled = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled();
    m_nSkillIconIndex = -1;
    m_nSkillDataIndex = -1;
    
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = SKILL_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    
    // 在TouchGroup之上添加一层，供拖拽图标使用
    m_pTopLayer = CCLayer::create();
    addChild(m_pTopLayer,2);
    
    // 关闭
    Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(closeButton);
    closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    // help
    UIButton *helpButton = static_cast<UIButton *>(panel0->getChildByName("Button_0"));
    if(helpButton)
    {
        helpButton->addTouchEventListener(this, toucheventselector(PopUpUI::helpButtonCallBack));
        helpButton->setZOrder(20);
        ImageView * image = static_cast<ImageView *>(helpButton->getChildByName("ImageView_0"));
        image->addTouchEventListener(this, toucheventselector(PopUpUI::hideHelpInterface));
        image->setEnabled(false);
    }
    Layout *panel1 = static_cast<Layout*>(panel0->getChildByName("Panel1"));
    CC_ASSERT(panel1);
    
    // 屏蔽作废按键
    Button * oldButton = static_cast<UIButton *>(panel1->getChildByName("Button3"));
    oldButton->setEnabled(false);
    
    // 顶部3个页签切换
    UIButton *button[3];
    for (int i = 0; i < 3; i++)
    {
        button[i] = static_cast<UIButton *>(panel1->getChildByName(CCString::createWithFormat("Button%d",i)->getCString()));
        CC_ASSERT(button[i]);
        button[i]->addTouchEventListener(this, toucheventselector(PopUpUI::skillPageButtonCallBack));
    }
    filePath = getFilePath("206",kDirTypeUI,kFileSuffixPng);
    button[0]->loadTextureNormal(filePath.c_str()); // 设置第一个页签为选中状态
    m_pSkillPageButton = button[0];
    
    Layout *panel2 = static_cast<Layout *>(panel0->getChildByName("Panel2"));
    
    // 初始化怒技pageView 包含2页 默认不可用
    UIPageView* pageView = static_cast<UIPageView *>(panel0->getChildByName("PageView_0"));
    for(int i = 0; i < 2; i++)
    {
        Layout* page = static_cast<Layout *>(panel2->clone());
        pageView->addPage(page);
    }
    pageView->setEnabled(false);
    
    // 技能设置顶部icon
    Layout *panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
    panel_a->setVisible(true);
    setSkillIcon(panel2,GlobalData::getInstance()->getUsableSkillDataArray());
    
    // 技能设置底部icon
    setShortcutsUIInfo(panel_a);
    
    // 设置怒气技技能切换回调
    UIPanel* panel_a_2_0 = static_cast<UIPanel *>(panel_a->getChildByName("Panel2_0"));
    UIPanel* panel_a_2_0_1 = static_cast<UIPanel *>(panel_a_2_0->getChildByName("Panel2_0_1"));
    for (int i = 0; i < 3; i++)
    {
        CCString* strPath = CCString::createWithFormat("ImageView0%d",i);
        UIImageView* skillImg = static_cast<UIImageView *>(panel_a_2_0_1->getChildByName(strPath->getCString()));
        skillImg->setTag(i);
        skillImg->addTouchEventListener(this, toucheventselector(PopUpUI::switchGestureSkillCallBack));
    }
    
    // 更新个人金币信息
    Layout *panel_b = static_cast<Layout *>(panel0->getChildByName("Panel_b"));
    panel_b->setVisible(false);
    Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
    Layout * panel2_0_1 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_1"));
    updateSkillUIPropertyInfo(panel2_0_1);
    
    // 技能升级详细信息
    Layout * panel2_0_2 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_2"));
    UIScrollView* pSV = static_cast<UIScrollView *>(panel2_0_2->getChildByName("ScrollView_0"));
    pSV->setBounceEnabled(true);
    ImageView *book = static_cast<ImageView *>(pSV->getChildByName("ImageView_book"));
    book->setVisible(false);
    
    // 升级按钮
    Button *levelUpButton = static_cast<Button *>(panel2_0_1->getChildByName("Button"));
    levelUpButton->addTouchEventListener(this,toucheventselector(PopUpUI::skillLevelUp));
    
    // 默认屏蔽详情页面
    panel_b->setVisible(false);
    panel_b->setEnabled(false);

}

#pragma mark 设置技能顶部图标 技能设置 技能升级

void PopUpUI::setSkillIcon(Widget * widget, cocos2d::CCArray * skillDataArray,bool isDefault)
{
    if(skillDataArray)
    {
        for (int i = 0 ; i < widget->getChildrenCount(); i++)
        {
            ImageView *skillIcon = static_cast<ImageView *>(widget->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
            if (!skillIcon)
            {
                break;
            }
            skillIcon->removeChildByTag(5);
            skillIcon->setEnabled(false);
            skillIcon->setVisible(false);
            if (i < skillDataArray->count())
            {
                skillIcon->setTag(i);
                if (isDefault)
                {
                    if (i == 0) //设置默认选中项
                    {
                        ImageView *iconFrame = dynamic_cast<ImageView *>(skillIcon->getChildByName("ImageView"));
                        iconFrame->setVisible(true);
                        m_nSkillIconIndex = 0;
                    }
                }
                SkillData *skillData =  dynamic_cast<SkillData *>(skillDataArray->objectAtIndex(i));
                if (skillData)
                {
                    string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                    ImageView *skillIcon1 = ImageView::create();
                    skillIcon1->loadTexture(filePath.c_str());
                    skillIcon1->setZOrder(5);
                    skillIcon1->setTag(5);
                    skillIcon->addChild(skillIcon1);
                    Label *label = static_cast<Label*>(skillIcon->getChildByName("TextArea0"));
                    label->setText(CCString::createWithFormat("%d",skillData->getLevel())->getCString());
                    skillIcon->setEnabled(true);
                    skillIcon->setVisible(true);
                    UILabel* name = static_cast<UILabel *>(skillIcon->getChildByName("Label_0"));
                    name->setText(skillData->getSkillName());
                    name->setZOrder(6);
                    
                    // 将技能图标设置为按键
                    skillIcon->addTouchEventListener(this, toucheventselector(PopUpUI::skillIconCallBack));
                }
            }
        }
    }
}

#pragma mark 设置技能顶部图标 怒技升级

void PopUpUI::setGesturePageViewIcon(Widget * widget, cocos2d::CCArray * skillDataArray, bool isDefault)
{
    UIPageView* pageView = static_cast<UIPageView *>(widget);
    for (int i = 0; i < 2; i++)
    {
        UILayout* page = static_cast<UILayout *>(pageView->getPage(i));
        for (int j = 0; j < page->getChildrenCount(); j++)
        {
            ImageView *skillIcon = static_cast<ImageView *>(page->getChildByName(CCString::createWithFormat("ImageView%d",j)->getCString()));
            if (!skillIcon)
            {
                break;
            }
            skillIcon->removeChildByTag(5);
            skillIcon->setEnabled(false);
            skillIcon->setVisible(false);
            int index = i*page->getChildrenCount() + j;
            skillIcon->setTag(index);
            if (index < skillDataArray->count())
            {
                if (isDefault)
                {
                    if (index == 0) //设置默认选中项
                    {
                        ImageView *iconFrame = dynamic_cast<ImageView *>(skillIcon->getChildByName("ImageView"));
                        iconFrame->setVisible(true);
                        m_nSkillIconIndex = 0;
                        pageView->scrollToPage(0);
                    }
                }

                SkillData *skillData =  dynamic_cast<SkillData *>(skillDataArray->objectAtIndex(index));
                if (skillData)
                {
                    string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                    ImageView *skillIcon1 = ImageView::create();
                    skillIcon1->loadTexture(filePath.c_str());
                    skillIcon1->setZOrder(5);
                    skillIcon1->setTag(5);
                    skillIcon->addChild(skillIcon1);
                    Label *label = static_cast<Label*>(skillIcon->getChildByName("TextArea0"));
                    label->setText(CCString::createWithFormat("%d",skillData->getLevel())->getCString());
                    skillIcon->setEnabled(true);
                    skillIcon->setVisible(true);
                    UILabel* name = static_cast<UILabel *>(skillIcon->getChildByName("Label_0"));
                    name->setText(skillData->getSkillName());
                    name->setZOrder(6);
                    
                    // 将技能图标设置为按键
                    skillIcon->addTouchEventListener(this, toucheventselector(PopUpUI::skillIconCallBack));
                }
            }
        }
    }
}

#pragma mark 设置对应手势的技能图片

void PopUpUI::setGestureSkillIcon(Widget * widget,int gestureType)
{
    Layout *panel2_0 = static_cast<Layout* >(widget->getChildByName("Panel2_0"));
    Layout *panel2_0_1 = static_cast<Layout* >(panel2_0->getChildByName("Panel2_0_1"));
    Layout *panel2_0_1_0 = static_cast<Layout* >(panel2_0_1->getChildByName("Panel2_0_1_0"));
    CCArray * skillDataArray = GlobalData::getInstance()->getGestureSkillDataArrayByType(gestureType);
    for (int i = 0; i < 3; i++)
    {
        const char *name = CCString::createWithFormat("ImageView1%d",i)->getCString();
        ImageView *skillIcon = static_cast<ImageView *>(panel2_0_1_0->getChildByName(name));
        CC_ASSERT(skillIcon);
        skillIcon->setEnabled(false);
        skillIcon->setVisible(false);
        if (i < skillDataArray->count())
        {
            SkillData *skillUIData = dynamic_cast<SkillData *>(skillDataArray->objectAtIndex(i));
            if (skillUIData && skillUIData->getLevel() && skillUIData->getGestureType() == gestureType)
            {// 已学习且与对应的手势快捷栏类型一致
                string filePath = getFilePath(skillUIData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                ImageView *skillIcon1 = ImageView::create();
                skillIcon1->loadTexture(filePath.c_str());
                skillIcon->addChild(skillIcon1);
                Label *label = static_cast<Label*>(skillIcon->getChildByName("TextArea0"));
                label->setText(CCString::createWithFormat("%d",skillUIData->getLevel())->getCString());
                UILabel* label_0 = static_cast<UILabel *>(skillIcon->getChildByName("Label_0"));
                label_0->setText(skillUIData->getSkillName());
                label_0->setZOrder(10);
                skillIcon->setEnabled(true);
                skillIcon->setVisible(true);
                skillIcon->addTouchEventListener(this, toucheventselector(PopUpUI::angrySkillIconCallBack));
            }
        }
    }
}

#pragma mark 设置技能快捷栏信息

void PopUpUI::setShortcutsUIInfo(Widget * widget)
{
    m_pDragRegionDictionary->removeAllObjects();
    // 技能快捷栏三个快捷技能拖拽区域
    Layout *panel2_0 = static_cast<Layout* >(widget->getChildByName("Panel2_0"));
    Layout *panel2_0_2 = static_cast<Layout* >(panel2_0->getChildByName("Panel2_0_2"));
    ImageView *shortcutsBar = static_cast<ImageView* >(panel2_0_2->getChildByName("ImageView"));
    ImageView *atkIcon = ImageView::create();
    string filePath = getFilePath("0",kDirTypeUI,kFileSuffixPng);
    atkIcon->loadTexture(filePath.c_str());
    shortcutsBar->addChild(atkIcon);
    
    ImageView *skillRegion1[3];
    skillRegion1[0] =  static_cast<ImageView*>(shortcutsBar->getChildByName("ImageView0"));
    skillRegion1[1] =  static_cast<ImageView*>(shortcutsBar->getChildByName("ImageView2"));
    skillRegion1[2] =  static_cast<ImageView*>(shortcutsBar->getChildByName("ImageView1"));
    
    CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
    for (int i = 0; i < array->count(); i++)
    {
        CCString *pStr = CCString::createWithFormat("%i", i);
        m_pDragRegionDictionary->setObject(skillRegion1[i], pStr->getCString());
        CCString * skillID = (CCString *)array->objectAtIndex(i);
        if (skillID && !skillID->isEqual(CCString::create("")))
        {
            SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(), skillID->intValue());
            if(skillData)
            {
                string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                ImageView *skillIcon = ImageView::create();
                skillIcon->loadTexture(filePath.c_str());
                skillIcon->setTag(10);
                skillRegion1[i]->addChild(skillIcon);
            }
        }
    }
    
    // 怒气技能图标面板
    Layout *panel2_0_1 = static_cast<Layout*>(panel2_0->getChildByName("Panel2_0_1"));
    
    // 怒气技能三个拖拽区域
    ImageView *skillRegion[3];
    skillRegion[0] =  static_cast<ImageView*>(panel2_0_1->getChildByName("ImageView00"));
    skillRegion[1] =  static_cast<ImageView*>(panel2_0_1->getChildByName("ImageView01"));
    skillRegion[2] =  static_cast<ImageView*>(panel2_0_1->getChildByName("ImageView02"));

    array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
    if (array)
    {
        for (int i = 0; i < array->count(); i++)
        {
            CCString *pStr = CCString::createWithFormat("%i", i + 3);
            m_pDragRegionDictionary->setObject(skillRegion[i], pStr->getCString());
            CCString * skillID = (CCString *)array->objectAtIndex(i);
            if (skillID && !skillID->isEqual(CCString::create("")))
            {
                SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(), skillID->intValue());
                if (skillData)
                {
                    string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                    ImageView *skillIcon = ImageView::create();
                    skillIcon->loadTexture(filePath.c_str());
                    skillRegion[i]->addChild(skillIcon);
                    skillIcon->setTag(10);
                    UILabel *lalName =static_cast<UILabel*>(skillRegion[i]->getChildByName("Label_0"));
                    lalName->setText(skillData->getSkillName());
                    ImageView *imageView =static_cast<ImageView*>(skillRegion[i]->getChildByName("ImageView"));
                    imageView->setVisible(false);
                    lalName->setZOrder(10);
                }
                else
                {
                    CCLOG("%d",skillID->intValue());
                }
            }
        }
    }
    
    // 设置第一个怒气技快捷栏为选中状态
    ImageView *imageView =static_cast<ImageView*>(skillRegion[0]->getChildByName("ImageView"));
    imageView->setVisible(true);
    m_nGestureType = 0;
    setGestureSkillIcon(widget,0);
}

#pragma mark 切换手势技能

void PopUpUI::switchGestureSkillCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        ImageView *button = dynamic_cast<ImageView *>(pSender);
        if (m_nGestureType != button->getTag())
        {
            //隐藏选中框
            UIPanel* panel = static_cast<UIPanel *>(button->getParent());
            UIImageView* imageView;
            for (int i = 0; i < 3; i++)
            {
                CCString* path = CCString::createWithFormat("ImageView0%d",i);
                UIImageView* imageView1 = dynamic_cast<ImageView *>(panel->getChildByName(path->getCString()));
                imageView = dynamic_cast<ImageView *>(imageView1->getChildByName("ImageView"));
                imageView->setVisible(false);
            }
            imageView = dynamic_cast<ImageView *>(button->getChildByName("ImageView"));
            imageView->setVisible(true);
            m_nGestureType = button->getTag();
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            Layout *panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
            setGestureSkillIcon(panel_a,button->getTag());
        }
    }
}

#pragma mark 技能页签切换

void PopUpUI::skillPageButtonCallBack(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
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
        UIButton *button = dynamic_cast<UIButton *>(pSender);
        if(m_pSkillPageButton != button) // 未重复按下相同页签
        {
            //切换按键被选中效果
            string filePath1 = getFilePath("205",kDirTypeUI,kFileSuffixPng);
            string filePath2 = getFilePath("206",kDirTypeUI,kFileSuffixPng);
            m_pSkillPageButton->loadTextureNormal(filePath1.c_str());
            button->loadTextureNormal(filePath2.c_str());
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            Layout *panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
            Layout *panel_b = static_cast<Layout *>(panel0->getChildByName("Panel_b"));
            Layout *panel2 = static_cast<Layout *>(panel0->getChildByName("Panel2"));// 普通技能图标面板
            UIPageView* pageView = static_cast<UIPageView *>(panel0->getChildByName("PageView_0"));  //怒技pageView
            
             // 清除之前的图标被选中效果
            if ((strcmp(m_pSkillPageButton->getName(), "Button0") == 0) ||(strcmp(m_pSkillPageButton->getName(), "Button1") == 0)) // 技能设置 技能升级
            {
                for (int i = 0; i < 5; i++)
                {
                    ImageView *preIcon = dynamic_cast<ImageView *>(panel2->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
                    ImageView *iconFrame = dynamic_cast<ImageView *>(preIcon->getChildByName("ImageView"));
                    iconFrame->setVisible(false);
                }
            }
            else if (strcmp(m_pSkillPageButton->getName(), "Button2") == 0) //怒技升级
            {
                for (int i = 0; i < 10; i++)
                {
                    UIPanel* panel;
                    if (i < 5)
                    {
                         panel = static_cast<UIPanel *>(pageView->getPage(0));
                    }
                    else
                    {
                         panel = static_cast<UIPanel *>(pageView->getPage(1));
                    }
                    int index = i % 5;
                    ImageView *preIcon = dynamic_cast<ImageView *>(panel->getChildByName(CCString::createWithFormat("ImageView%d",index)->getCString()));
                    ImageView *iconFrame = dynamic_cast<ImageView *>(preIcon->getChildByName("ImageView"));
                    iconFrame->setVisible(false);
                }
            }
  
            m_pSkillPageButton = button;
  
            if (strcmp(button->getName(), "Button0") == 0)// 技能设置
            {
                pageView->setEnabled(false);
                panel2->setEnabled(true);
                panel_a->setVisible(true);
                panel_a->setEnabled(true);
                panel_b->setVisible(false);
                panel_b->setEnabled(false);
                setSkillIcon(panel2,GlobalData::getInstance()->getUsableSkillDataArray());
                setShortcutsUIInfo(panel_a);
            }
            else if (strcmp(button->getName(), "Button1") == 0)// 技能升级
            {
                pageView->setEnabled(false);
                panel2->setEnabled(true);
                panel_a->setVisible(false);
                panel_a->setEnabled(false);
                panel_b->setVisible(true);
                panel_b->setEnabled(true);
             
                // 隐藏技能书图标栏
                Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
                Layout * panel2_0_2 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_2"));
                UIScrollView* pSV = static_cast<UIScrollView *>(panel2_0_2->getChildByName("ScrollView_0"));
                pSV->jumpToTop();
                ImageView *book = static_cast<ImageView *>(pSV->getChildByName("ImageView_book"));
                book->setVisible(false);
                
                setSkillIcon(panel2,GlobalData::getInstance()->getUsableSkillDataArray());
                if (GlobalData::getInstance()->getUsableSkillDataArray()->count() < 1)
                {
                    return;
                }
                // 技能描述
                SkillData *skillData = dynamic_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));
                if(skillData)
                {
                    m_nSkillDataIndex = skillData->getIndex();
                    
                    //当前技能描述
                    Label *label1 = dynamic_cast<Label *>(pSV->getChildByName("Label_0_0"));
                    if (label1)
                    {
                        label1->setVisible(true);
                        label1->setText(skillData->getCurrentLevelDescription());
                    }
                    
                    //下一级技能描述
                    Label *label2 = dynamic_cast<Label *>(pSV->getChildByName("Label_1_0"));
                    if (label2)
                    {
                        label2->setVisible(true);
                        label2->setText(skillData->getNextLevelDescription());
                    }

                    // 金币
                    UIImageView* pImg0 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView0"));
                    if (pImg0)
                    {
                        UILabel* label3 = dynamic_cast<Label *>(pImg0->getChildByName("TextArea"));
                        if (label3)
                        {
                            label3->setVisible(true);
                            label3->setText(CCString::createWithFormat("%d",skillData->getMoney())->getCString());
                        }
                    }
                    
                    // 宝石
                    UIImageView* pImg1 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView1"));
                    if (pImg1)
                    {
                        UILabel* label4 = dynamic_cast<Label *>(pImg1->getChildByName("TextArea"));
                        if (label4)
                        {
                            label4->setVisible(true);
                            label4->setText(CCString::createWithFormat("%d",skillData->getJew())->getCString());
                        }
                    }
                    
                    // 升级所需角色等级
                    UILabel* label5 = dynamic_cast<UILabel *>(pSV->getChildByName("Label_3"));
                    if (label5)
                    {
                        UILabel* label6 = dynamic_cast<Label *>(label5->getChildByName("Label_1_0"));
                        if (label6)
                        {
                            label6->setVisible(true);
                            label6->setText(CCString::createWithFormat("%d",skillData->getRoleLevel())->getCString());
                        }
                    }
                }
            }
            else if (strcmp(button->getName(), "Button2") == 0)// 怒技升级
            {
                pageView->setEnabled(true);
                panel2->setEnabled(false);
                panel_a->setVisible(false);
                panel_a->setEnabled(false);
                panel_b->setVisible(true);
                panel_b->setEnabled(true);
               
                Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
                setGesturePageViewIcon(pageView,GlobalData::getInstance()->getUsableGestureSkillDataArray());
                Layout *panel2_0_2 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_2"));
                if (GlobalData::getInstance()->getUsableGestureSkillDataArray()->count() < 1)
                {
                    return;
                }
                // 技能描述
                
                SkillData *skillData = dynamic_cast<SkillData *>(GlobalData::getInstance()->getUsableGestureSkillDataArray()->objectAtIndex(m_nSkillIconIndex));
                if(skillData)
                {
                    m_nSkillDataIndex = skillData->getIndex();
                    UIScrollView* pSV = static_cast<UIScrollView *>(panel2_0_2->getChildByName("ScrollView_0"));
                    pSV->jumpToTop();
                    Label *label1 = dynamic_cast<Label *>(pSV->getChildByName("Label_0_0"));
                    if (label1)
                    {
                        label1->setVisible(true);
                        label1->setText(skillData->getCurrentLevelDescription());
                    }
                    Label *label2 = dynamic_cast<Label *>(pSV->getChildByName("Label_1_0"));
                    if (label2)
                    {
                        label2->setVisible(true);
                        label2->setText(skillData->getNextLevelDescription());
                    }
                    
                    // 金币
                    UIImageView* pImg0 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView0"));
                    if (pImg0)
                    {
                        UILabel* label3 = dynamic_cast<Label *>(pImg0->getChildByName("TextArea"));
                        if (label3)
                        {
                            label3->setVisible(true);
                            label3->setText(CCString::createWithFormat("%d",skillData->getMoney())->getCString());
                        }
                    }
                    
                    // 宝石
                    UIImageView* pImg1 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView1"));
                    if (pImg1)
                    {
                        UILabel* label4 = dynamic_cast<Label *>(pImg1->getChildByName("TextArea"));
                        if (label4)
                        {
                            label4->setVisible(true);
                            label4->setText(CCString::createWithFormat("%d",skillData->getJew())->getCString());
                        }
                    }
                    
                    // 升级所需角色等级
                    UILabel* label5 = dynamic_cast<UILabel *>(pSV->getChildByName("Label_3"));
                    if (label5)
                    {
                        UILabel* label6 = dynamic_cast<Label *>(label5->getChildByName("Label_1_0"));
                        if (label6)
                        {
                            label6->setVisible(true);
                            label6->setText(CCString::createWithFormat("%d",skillData->getRoleLevel())->getCString());
                        }
                    }
                    
                    // 显示技能书图标栏
                    ImageView * book = static_cast<ImageView *>(pSV->getChildByName("ImageView_book"));
                    book->setVisible(true);
                    ImageView *icon = ImageView::create();
                    string filePath = getFilePath(skillData->getItemIcon(),kDirTypeIcon,kFileSuffixPng);
                    icon->loadTexture(filePath.c_str());
                    book->addChild(icon);
                    Label *label02 = static_cast<Label *>(book->getChildByName("TextArea2_0"));
                    label02->setText(CCString::createWithFormat("%d",skillData->getItemNumber())->getCString());
                    Label *label00 = static_cast<Label *>(book->getChildByName("TextArea0_0"));
                    int num = getItemNumber(skillData->getItemID());
                    label00->setText(CCString::createWithFormat("%d",num)->getCString());
                }
            }
        }
    }
}

#pragma mark 获取item索引

int PopUpUI::getItemNumber(int index)
{
    int num = 0;
    CCArray *array = GlobalData::getInstance()->getPropArray();
    for (int i = 0; i < array->count(); i++)
    {
        Property *prop = static_cast<Property *>(array->objectAtIndex(i));
        if (prop->getIndex() == index)
        {
            num += prop->getNumber();
        }
    }
    return num;
}

#pragma mark 设置手势技能时排除其它类型手势技能的快捷栏

void PopUpUI::excludeDifferentTypeIcon(int gestureType, bool isEnable)
{
    Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
    Layout *panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
    Layout *panel2_0 = static_cast<Layout *>(panel_a->getChildByName("Panel2_0"));
    Layout *panel2_0_1 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_1"));
    for (int j = 0; j < 3; j ++)
    {
        if (j != gestureType)
        {
            ImageView * icon = static_cast<ImageView *>(panel2_0_1->getChildByName(CCString::createWithFormat("ImageView0%d",j)->getCString()));
            icon->setEnabled(isEnable);
        }
    }
}

#pragma mark 移动技能图标按钮回调

void PopUpUI::skillIconCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    ImageView *icon = dynamic_cast<ImageView *>(pSender);
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
        Layout *layout = dynamic_cast<Layout *>(icon->getParent());
        CC_ASSERT(layout);
        // 清除之前的选中效果
        if((strcmp(m_pSkillPageButton->getName(), "Button0") == 0) || (strcmp(m_pSkillPageButton->getName(), "Button1") == 0))
        {
            ImageView *preIcon = NULL;
            if (m_nSkillIconIndex > -1)
            {
                preIcon = dynamic_cast<ImageView *>(layout->getChildByName(CCString::createWithFormat("ImageView%d",m_nSkillIconIndex)->getCString()));
            }
            if (preIcon)
            {
                ImageView *iconFrame = dynamic_cast<ImageView *>(preIcon->getChildByName("ImageView"));
                iconFrame->setVisible(false);
            }
        }
        else if(strcmp(m_pSkillPageButton->getName(), "Button2") == 0)
        {
            Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
            UIPageView* pageView = static_cast<UIPageView *>(panel0->getChildByName("PageView_0"));
            for (int i = 0; i < 10; i++)
            {
                UIPanel* panel;
                if (i < 5)
                {
                    panel = static_cast<UIPanel *>(pageView->getPage(0));
                }
                else
                {
                    panel = static_cast<UIPanel *>(pageView->getPage(1));
                }
                int index = i % 5;
                ImageView *preIcon = dynamic_cast<ImageView *>(panel->getChildByName(CCString::createWithFormat("ImageView%d",index)->getCString()));
                ImageView *iconFrame = dynamic_cast<ImageView *>(preIcon->getChildByName("ImageView"));
                iconFrame->setVisible(false);
            }
        }
        
        // 设置图标被选中效果
        ImageView *iconFrame = dynamic_cast<ImageView *>(icon->getChildByName("ImageView"));
        iconFrame->setVisible(true);
        m_nSkillIconIndex = icon->getTag();
        
        if(strcmp(m_pSkillPageButton->getName(), "Button0") == 0)
        {// 技能设置
            CCArray *array = GlobalData::getInstance()->getUsableSkillDataArray();
            SkillData *skillData = static_cast<SkillData *>(array->objectAtIndex(m_nSkillIconIndex));
            if (skillData)
            {
                m_nSkillDataIndex = skillData->getIndex();
                string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                CCSprite *sprite = CCSprite::create(filePath.c_str());
                if (sprite)
                {
                    m_pTopLayer->addChild(sprite);
                    sprite->setPosition(icon->getTouchStartPos());
                }
            }
        }
        else if(strcmp(m_pSkillPageButton->getName(), "Button1") == 0) // 技能描述
        {
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            Layout *panel_b = static_cast<Layout *>(panel0->getChildByName("Panel_b"));
            Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
            Layout *panel2_0_2 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_2"));
            SkillData *skillData = dynamic_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));
            if(skillData)
            {
                m_nSkillDataIndex = skillData->getIndex();
                UIScrollView* pSV = static_cast<UIScrollView *>(panel2_0_2->getChildByName("ScrollView_0"));
                pSV->jumpToTop();
                
                //当前技能描述
                Label *label1 = dynamic_cast<Label *>(pSV->getChildByName("Label_0_0"));
                if (label1)
                {
                    label1->setVisible(true);
                    label1->setText(skillData->getCurrentLevelDescription());
                }
                
                //下一级技能描述
                Label *label2 = dynamic_cast<Label *>(pSV->getChildByName("Label_1_0"));
                if (label2)
                {
                    label2->setVisible(true);
                    label2->setText(skillData->getNextLevelDescription());
                }
                
                // 金币
                UIImageView* pImg0 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView0"));
                if (pImg0)
                {
                    UILabel* label3 = dynamic_cast<Label *>(pImg0->getChildByName("TextArea"));
                    if (label3)
                    {
                        label3->setVisible(true);
                        label3->setText(CCString::createWithFormat("%d",skillData->getMoney())->getCString());
                    }
                }
                
                // 宝石
                UIImageView* pImg1 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView1"));
                if (pImg1)
                {
                    UILabel* label4 = dynamic_cast<Label *>(pImg1->getChildByName("TextArea"));
                    if (label4)
                    {
                        label4->setVisible(true);
                        label4->setText(CCString::createWithFormat("%d",skillData->getJew())->getCString());
                    }
                }
                
                // 升级所需角色等级
                UILabel* label5 = dynamic_cast<UILabel *>(pSV->getChildByName("Label_3"));
                if (label5)
                {
                    UILabel* label6 = dynamic_cast<Label *>(label5->getChildByName("Label_1_0"));
                    if (label6)
                    {
                        label6->setVisible(true);
                        label6->setText(CCString::createWithFormat("%d",skillData->getRoleLevel())->getCString());
                    }
                }
            }
        }
        else if(strcmp(m_pSkillPageButton->getName(), "Button2") == 0) // 怒气描述
        {
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            Layout *panel_b = static_cast<Layout *>(panel0->getChildByName("Panel_b"));
            Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
            Layout *panel2_0_2 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_2"));
            SkillData *skillData = dynamic_cast<SkillData *>(GlobalData::getInstance()->getUsableGestureSkillDataArray()->objectAtIndex(m_nSkillIconIndex));
            if(skillData)
            {
                m_nSkillDataIndex = skillData->getIndex();
                UIScrollView* pSV = static_cast<UIScrollView *>(panel2_0_2->getChildByName("ScrollView_0"));
                pSV->jumpToTop();
                
                //当前技能描述
                Label *label1 = dynamic_cast<Label *>(pSV->getChildByName("Label_0_0"));
                if (label1)
                {
                    label1->setVisible(true);
                    label1->setText(skillData->getCurrentLevelDescription());
                }
                
                //下一级技能描述
                Label *label2 = dynamic_cast<Label *>(pSV->getChildByName("Label_1_0"));
                if (label2)
                {
                    label2->setVisible(true);
                    label2->setText(skillData->getNextLevelDescription());
                }
                
                // 金币
                UIImageView* pImg0 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView0"));
                if (pImg0)
                {
                    UILabel* label3 = dynamic_cast<Label *>(pImg0->getChildByName("TextArea"));
                    if (label3)
                    {
                        label3->setVisible(true);
                        label3->setText(CCString::createWithFormat("%d",skillData->getMoney())->getCString());
                    }
                }
                
                // 宝石
                UIImageView* pImg1 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView1"));
                if (pImg1)
                {
                    UILabel* label4 = dynamic_cast<Label *>(pImg1->getChildByName("TextArea"));
                    if (label4)
                    {
                        label4->setVisible(true);
                        label4->setText(CCString::createWithFormat("%d",skillData->getJew())->getCString());
                    }
                }
                
                // 升级所需角色等级
                UILabel* label5 = dynamic_cast<UILabel *>(pSV->getChildByName("Label_3"));
                if (label5)
                {
                    UILabel* label6 = dynamic_cast<Label *>(label5->getChildByName("Label_1_0"));
                    if (label6)
                    {
                        label6->setVisible(true);
                        label6->setText(CCString::createWithFormat("%d",skillData->getRoleLevel())->getCString());
                    }
                }
            }
        }
    }
    else if(type == TOUCH_EVENT_MOVED)
    {
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        if (m_pTopLayer->getChildrenCount() > 0)
        {
            CCSprite *sprite = dynamic_cast<CCSprite *>(m_pTopLayer->getChildren()->objectAtIndex(0));
            if (sprite)
            {
                sprite->setPosition(icon->getTouchMovePos());
            }
        }
    }
    else if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        m_pTopLayer->removeAllChildren();
    }
    else if(type == TOUCH_EVENT_CANCELED)
    {
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        if (m_pTopLayer->getChildrenCount() > 0)
        {
            CCSprite *sprite = dynamic_cast<CCSprite *>(m_pTopLayer->getChildren()->objectAtIndex(0));
            if (sprite)
            {
                // 检测是否拖拽至快捷栏
                for (int i = 0; i < 3; i++)
                {
                    CCString *pStr = CCString::createWithFormat("%i", i);
                    ImageView * skillRegion = dynamic_cast<ImageView *>(m_pDragRegionDictionary->objectForKey(pStr->getCString()));
                    if (skillRegion)
                    {
                        CCRect rect = CCRectMake(skillRegion->getWorldPosition().x - skillRegion->getAnchorPoint().x * skillRegion->getContentSize().width, skillRegion->getWorldPosition().y - skillRegion->getAnchorPoint().y * skillRegion->getContentSize().height, skillRegion->getContentSize().width, skillRegion->getContentSize().height);
                        bool bSameSkill = false;
                        
                        CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
                        if (array)
                        {
                            // 判断是否包含相同技能id
                            for (int j = 0; j < array->count();j++)
                            {
                                if (dynamic_cast<CCString *>(array->objectAtIndex(j))->intValue() == m_nSkillDataIndex)
                                {
                                    bSameSkill = true;
                                    break;
                                }
                            }
                        }
 
                        if (rect.containsPoint(icon->getTouchMovePos()) && !bSameSkill)
                        {
                            SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(), m_nSkillDataIndex);
                            if (skillData)
                            {
                                array->removeObjectAtIndex(i);// 删除该位置之前的技能序号
                                array->insertObject(CCString::createWithFormat("%d",m_nSkillDataIndex), i);// 添加新的技能序号
                                skillRegion->removeChildByTag(10);
                                string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                                ImageView *skillIcon = ImageView::create();
                                skillIcon->loadTexture(filePath.c_str());
                                skillIcon->setTag(10);
                                skillRegion->addChild(skillIcon);
                                m_pTopLayer->removeAllChildren();
                                
                                // 向服务器发送快捷方式修改后的结果
                                CCString* msg = CCString::createWithFormat(ASK_SKILL_SHORT_CUT,m_nSkillDataIndex,i);
                                Controller::getInstance()->sendMessage(msg->getCString(),true);
                                break;
                            }
                        }
                    }
                }
            }
            m_pTopLayer->removeAllChildren();
        }
    }
}

#pragma mark 移动怒技图标按钮回调

void PopUpUI::angrySkillIconCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    Widget *widget = static_cast<Widget*>(pSender);
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type))
        {
            return;
        }
        if(strcmp(m_pSkillPageButton->getName(), "Button0") == 0)
        {
            int i = -1;
            if (strcmp(widget->getName(), "ImageView10") == 0)
            {
                i = 0;
            }
            else if (strcmp(widget->getName(), "ImageView11") == 0)
            {
                i = 1;
            }
            else if (strcmp(widget->getName(), "ImageView12") == 0)
            {
                i = 2;
            }
            if (i != -1)
            {
                CCArray *array = GlobalData::getInstance()->getGestureSkillDataArrayByType(m_nGestureType);
                SkillData *skillData = static_cast<SkillData *>(array->objectAtIndex(i));
                if (skillData)
                {
                    m_nSkillDataIndex = skillData->getIndex();
                    string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                    CCSprite *sprite = CCSprite::create(filePath.c_str());
                    if (sprite)
                    {
                        m_pTopLayer->addChild(sprite);
                        sprite->setPosition(widget->getTouchStartPos());
                        excludeDifferentTypeIcon(m_nGestureType,false);
                    }
                }
            }
        }
    }
    else if(type == TOUCH_EVENT_MOVED)
    {
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        if (m_pTopLayer->getChildrenCount() > 0)
        {
            CCSprite *sprite = dynamic_cast<CCSprite *>(m_pTopLayer->getChildren()->objectAtIndex(0));
            if (sprite)
            {
                sprite->setPosition(widget->getTouchMovePos());
            }
        }
    }
    else if(type == TOUCH_EVENT_ENDED)
    {
        m_pTopLayer->removeAllChildren();
        excludeDifferentTypeIcon(m_nGestureType, true);
    }
    else if(type == TOUCH_EVENT_CANCELED)
    {
        if (m_pTopLayer->getChildrenCount() > 0)
        {
            CCSprite *sprite = dynamic_cast<CCSprite *>(m_pTopLayer->getChildren()->objectAtIndex(0));
            if (sprite)
            {
                // 检测是否拖拽至快捷栏
                CCString *pStr = CCString::createWithFormat("%i", m_nGestureType + 3);
                ImageView * skillRegion = dynamic_cast<ImageView *>(m_pDragRegionDictionary->objectForKey(pStr->getCString()));
                if (skillRegion)
                {
                    CCRect rect = CCRectMake(skillRegion->getWorldPosition().x - skillRegion->getAnchorPoint().x * skillRegion->getContentSize().width, skillRegion->getWorldPosition().y - skillRegion->getAnchorPoint().y * skillRegion->getContentSize().height, skillRegion->getContentSize().width, skillRegion->getContentSize().height);
                    if (rect.containsPoint(widget->getTouchMovePos()))
                    {
                        
                        SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(),m_nSkillDataIndex);
                        if (skillData)
                        {
                            CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(m_pRoleByControlled->getSpriteIDInServer()->getCString()));
                            
                            array->removeObjectAtIndex(m_nGestureType);// 删除该位置之前的技能序号
                            array->insertObject(CCString::createWithFormat("%d",m_nSkillDataIndex), m_nGestureType);// 添加新的技能序号
                            skillRegion->removeChildByTag(10);
                            string filePath = getFilePath(skillData->getSkillIcon(),kDirTypeIcon,kFileSuffixPng);
                            ImageView *skillIcon = ImageView::create();
                            skillIcon->loadTexture(filePath.c_str());
                            skillIcon->setTag(10);
                            skillRegion->addChild(skillIcon);
                            m_pTopLayer->removeAllChildren();
                            static_cast<Widget*>(widget->getParent())->setTouchEnabled(true);
                            
                            // 向服务器发送快捷方式修改后的结果
                            CCString* msg = CCString::createWithFormat(ASK_SKILL_SHORT_CUT,m_nSkillDataIndex,m_nGestureType);
                            Controller::getInstance()->sendMessage(msg->getCString(),true);
                        }
                    }
                }
            }
            m_pTopLayer->removeAllChildren();
            excludeDifferentTypeIcon(m_nGestureType,true);
        }
    }
}


#pragma mark 更新技能UI的财产信息

void PopUpUI::updateSkillUIPropertyInfo(cocos2d::gui::Widget * panel)
{
    // 更新金钱宝石显示
    int goldNum = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getGold();
    ImageView *gold = static_cast<ImageView *>(panel->getChildByName("ImageView0"));
    Label * text1 = static_cast<Label *>(gold->getChildByName("TextArea"));
    text1->setText(CCString::createWithFormat("%d",goldNum)->getCString());
    
    int jewelNum =Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled()->getSpriteAttribute()->getJewel();
    ImageView *jewel = static_cast<ImageView *>(panel->getChildByName("ImageView1"));
    Label * text2 = static_cast<Label *>(jewel->getChildByName("TextArea"));
    text2->setText(CCString::createWithFormat("%d",jewelNum)->getCString());
}

#pragma mark 技能升级更新技能描述

void PopUpUI::updateSkillDescription()
{
    if (getUIType() == kUISkill)
    {
        // 更新技能描述
        if (m_nSkillIconIndex >= 0)
        {
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            Layout *panel2 = static_cast<Layout *>(panel0->getChildByName("Panel2"));
            UIPageView* pageView = static_cast<UIPageView *>(panel0->getChildByName("PageView_0"));
            if(strcmp(m_pSkillPageButton->getName(), "Button1") == 0)  // 技能
            {
                setSkillIcon(panel2,GlobalData::getInstance()->getUsableSkillDataArray(),false);
                SkillData *skillData = dynamic_cast<SkillData *>(GlobalData::getInstance()->getUsableSkillDataArray()->objectAtIndex(m_nSkillIconIndex));
                if (skillData)
                {
                    Layout *panel_b = static_cast<Layout *>(panel0->getChildByName("Panel_b"));
                    Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
                    Layout *panel2_0_2 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_2"));

                    UIScrollView* pSV = static_cast<UIScrollView *>(panel2_0_2->getChildByName("ScrollView_0"));
                    pSV->jumpToTop();
                    Label *label1 = dynamic_cast<Label *>(pSV->getChildByName("Label_0_0"));
                    if (label1)
                    {
                        label1->setVisible(true);
                        label1->setText(skillData->getCurrentLevelDescription());
                    }
                    Label *label2 = dynamic_cast<Label *>(pSV->getChildByName("Label_1_0"));
                    if (label2)
                    {
                        label2->setVisible(true);
                        label2->setText(skillData->getNextLevelDescription());
                    }
                    
                    // 金币
                    UIImageView* pImg0 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView0"));
                    if (pImg0)
                    {
                        UILabel* label3 = dynamic_cast<Label *>(pImg0->getChildByName("TextArea"));
                        if (label3)
                        {
                            label3->setVisible(true);
                            label3->setText(CCString::createWithFormat("%d",skillData->getMoney())->getCString());
                        }
                    }
                    
                    // 宝石
                    UIImageView* pImg1 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView1"));
                    if (pImg1)
                    {
                        UILabel* label4 = dynamic_cast<Label *>(pImg1->getChildByName("TextArea"));
                        if (label4)
                        {
                            label4->setVisible(true);
                            label4->setText(CCString::createWithFormat("%d",skillData->getJew())->getCString());
                        }
                    }
                    
                    // 升级所需角色等级
                    UILabel* label5 = dynamic_cast<UILabel *>(pSV->getChildByName("Label_3"));
                    if (label5)
                    {
                        UILabel* label6 = dynamic_cast<Label *>(label5->getChildByName("Label_1_0"));
                        if (label6)
                        {
                            label6->setVisible(true);
                            label6->setText(CCString::createWithFormat("%d",skillData->getRoleLevel())->getCString());
                        }
                    }
                }
            }
            else if(strcmp(m_pSkillPageButton->getName(), "Button2") == 0) // 怒技
            {
                setGesturePageViewIcon(pageView,GlobalData::getInstance()->getUsableGestureSkillDataArray(),false);
                SkillData *skillData = dynamic_cast<SkillData *>(GlobalData::getInstance()->getUsableGestureSkillDataArray()->objectAtIndex(m_nSkillIconIndex));
                if (skillData)
                {
                    Layout *panel_b = static_cast<Layout *>(panel0->getChildByName("Panel_b"));
                    Layout *panel2_0 = static_cast<Layout *>(panel_b->getChildByName("Panel2_0"));
                    Layout *panel2_0_2 = static_cast<Layout *>(panel2_0->getChildByName("Panel2_0_2"));

                    UIScrollView* pSV = static_cast<UIScrollView *>(panel2_0_2->getChildByName("ScrollView_0"));
                    pSV->jumpToTop();
                    Label *label1 = dynamic_cast<Label *>(pSV->getChildByName("Label_0_0"));
                    if (label1)
                    {
                        label1->setVisible(true);
                        label1->setText(skillData->getCurrentLevelDescription());
                    }
                    Label *label2 = dynamic_cast<Label *>(pSV->getChildByName("Label_1_0"));
                    if (label2)
                    {
                        label2->setVisible(true);
                        label2->setText(skillData->getNextLevelDescription());
                    }
                    
                    // 金币
                    UIImageView* pImg0 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView0"));
                    if (pImg0)
                    {
                        UILabel* label3 = dynamic_cast<Label *>(pImg0->getChildByName("TextArea"));
                        if (label3)
                        {
                            label3->setVisible(true);
                            label3->setText(CCString::createWithFormat("%d",skillData->getMoney())->getCString());
                        }
                    }
                    
                    // 宝石
                    UIImageView* pImg1 = dynamic_cast<UIImageView *>(pSV->getChildByName("ImageView1"));
                    if (pImg1)
                    {
                        UILabel* label4 = dynamic_cast<Label *>(pImg1->getChildByName("TextArea"));
                        if (label4)
                        {
                            label4->setVisible(true);
                            label4->setText(CCString::createWithFormat("%d",skillData->getJew())->getCString());
                        }
                    }
                    
                    // 升级所需角色等级
                    UILabel* label5 = dynamic_cast<UILabel *>(pSV->getChildByName("Label_3"));
                    if (label5)
                    {
                        UILabel* label6 = dynamic_cast<Label *>(label5->getChildByName("Label_1_0"));
                        if (label6)
                        {
                            label6->setVisible(true);
                            label6->setText(CCString::createWithFormat("%d",skillData->getRoleLevel())->getCString());
                        }
                    }
                    
                    // 显示技能书图标栏
                    ImageView * book = static_cast<ImageView *>(pSV->getChildByName("ImageView_book"));
                    book->setVisible(true);
                    ImageView *icon = ImageView::create();
                    string filePath = getFilePath(skillData->getItemIcon(),kDirTypeIcon,kFileSuffixPng);
                    icon->loadTexture(filePath.c_str());
                    book->addChild(icon);
                    Label *label02 = static_cast<Label *>(book->getChildByName("TextArea2_0"));
                    label02->setText(CCString::createWithFormat("%d",skillData->getItemNumber())->getCString());
                    Label *label00 = static_cast<Label *>(book->getChildByName("TextArea0_0"));
                    int num = getItemNumber(skillData->getItemID());
                    label00->setText(CCString::createWithFormat("%d",num)->getCString());
                }
            }

        }
    }
}

#pragma mark 技能升级按钮回调

void PopUpUI::skillLevelUp(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender,type,false))
        {
            return;
        }
        if (m_nSkillIconIndex > -1)
        {
            SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(),m_nSkillDataIndex);
            if(skillData)
            {
                if (m_pRoleByControlled->getLevel() < skillData->getRoleLevel())
                {
                    CCString *pStrTip = CCString::createWithFormat(g_sRoleLevelDificiency, skillData->getRoleLevel());
                    ((YYScene*)getParent())->showTips(pStrTip->getCString(), kTipNormal);
                    return;
                }
                int nGold = m_pRoleByControlled->getSpriteAttribute()->getGold();
                int nNeedGold = skillData->getMoney();
                int nNeedJewel = skillData->getJew();
                if (nNeedJewel > 0)
                {
                    if (!skillData->isAngrySkill())
                        setConsumeJewelTips(kJewelConsumeForUpgradeSkill, nNeedJewel);
                    else
                        setConsumeJewelTips(kJewelConsumeForUpgradeAngry, nNeedJewel);
                    return;
                }
                if (nNeedGold > 0 && nGold < nNeedGold)
                {
                    ((YYScene*)getParent())->showTips(g_sLackGold, kTipGoldCharge);
                    return;
                }
                CCString* msg = CCString::createWithFormat(ASK_SKILL_LEVEL_UP,skillData->getIndex());
                Controller::getInstance()->sendMessage(msg->getCString(), true);
            }
        }
    }
}

#pragma mark 技能升级花费宝石逻辑
    
void PopUpUI::skillUpgradeConsumeJewelLogic()
{
    if (m_nSkillIconIndex <= -1)
        return;
    SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(),m_nSkillDataIndex);
    int nGold = m_pRoleByControlled->getSpriteAttribute()->getGold();
    int nJewel = m_pRoleByControlled->getSpriteAttribute()->getJewel();
    int nNeedGold = skillData->getMoney();
    int nNeedJewel = skillData->getJew();
    if (nNeedJewel > 0 && nJewel < nNeedJewel)
    {
        ((YYScene*)getParent())->showTips(g_sLackJew, kTipJewCharge);
        return;
    }
    if (nNeedGold > 0 && nGold < nNeedGold)
    {
        ((YYScene*)getParent())->showTips(g_sLackGold, kTipGoldCharge);
        return;
    }
    CCString* msg = CCString::createWithFormat(ASK_SKILL_LEVEL_UP,skillData->getIndex());
    Controller::getInstance()->sendMessage(msg->getCString(), true);
}
