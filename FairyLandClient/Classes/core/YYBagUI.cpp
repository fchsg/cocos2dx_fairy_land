//
//  YYBagUI.cpp
//  Fairyland
//  背包界面
//  Created by fuchenhao on 14-4-17.
//
//

#include "YYPopUpUI.h"

#pragma mark 初始化背包界面

void PopUpUI::initBagUI(int nIndexBag)
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIBag);
    m_pDragRegionDictionary->removeAllObjects();
    m_u8ReinforceIndex = -1;
    m_u8ComposeIndex = -1;
    m_pRoleByControlled = Controller::getInstance()->getScene()->getControlUILayer()->getRoleByControlled();
    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    string jsonFilePath = BAG_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(jsonFilePath.c_str()));
    // 关闭
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton* close_Button = static_cast<UIButton*>(panel0->getChildByName("Button_closed"));
    close_Button->setTag(kButtonClose);
    close_Button->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    // help
    UIButton *helpButton = static_cast<UIButton *>(panel0->getChildByName("Button_0"));
    if(helpButton)
    {
        helpButton->setTag(kButtonHelp);
        helpButton->addTouchEventListener(this, toucheventselector(PopUpUI::helpButtonCallBack));
        ImageView * image = static_cast<ImageView *>(helpButton->getChildByName("ImageView_0"));
        image->addTouchEventListener(this, toucheventselector(PopUpUI::hideHelpInterface));
        image->setEnabled(false);
    }

    // 背包内 装备 强化 锻造 页面切换回调
    UIPanel* panel1 = static_cast<UIPanel*>(panel0->getChildByName("Panel1"));
    for (int i = 0 ; i < panel1->getChildrenCount(); i++)
    {
        Button * button = dynamic_cast<UIButton *>(panel1->getChildByName(CCString::createWithFormat("Button%d",i)->getCString()));
        button->setTag(kBagUIPage + i);
        button->addTouchEventListener(this, toucheventselector(PopUpUI::bagPageSwitchCallBack));
        if (i == nIndexBag)
        {// 默认选中第一个页签
            m_pBagPageButton = button;
            string str = getFilePath("206", kDirTypeUI, kFileSuffixPng);
            button->loadTextureNormal(str.c_str());
        }
    }
    
    // 左侧装备信息 强化 锻造 使用
    Layout * panel3 = static_cast<Layout *>(panel0->getChildByName("Panel3"));
    ScrollView * sv = static_cast<ScrollView*>(panel3->getChildByName("ScrollView_0"));
    for(int i = 0; i < sv->getChildrenCount(); i++)
    {
        ImageView* bagCell = static_cast<ImageView*>(sv->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
        bagCell->addTouchEventListener(this, toucheventselector(PopUpUI::bagCellCallBackEx));
        bagCell->setTag(i);
    }
    sv->setBounceEnabled(true);
    
    // 新手引导 列表不可滚动
    if(Controller::getInstance()->isTutorial())
    {
        sv->setMoved(false);
    }
    
    // 滚动条 选中背景框
    m_pBagSelImg = UIImageView::create();
    sv->addChild(m_pBagSelImg,10);
    std::string filePath = getFilePath("471", kDirTypeUI, kFileSuffixPng);
    m_pBagSelImg->loadTexture(filePath.c_str());
    
    // 右侧 ScrollView 装备 使用
    Layout * panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
    Layout * panel2_3 = static_cast<Layout*>(panel_a->getChildByName("Panel2_3"));
    ScrollView * svEquip = static_cast<ScrollView*>(panel2_3->getChildByName("ScrollView0"));
    svEquip->setBounceEnabled(true);
    for(int i = 0; i < svEquip->getChildrenCount(); i++)
    {
        ImageView* bagCell = static_cast<ImageView*>(svEquip->getChildByName(CCString::createWithFormat("ImageView0_%d_0",i)->getCString()));
        bagCell->addTouchEventListener(this, toucheventselector(PopUpUI::bagCellCallBack));
        bagCell->setTag(i);
    }
    
    // 强化详细信息回调
    {
        // 装备信息
        UILayout * panel_b = static_cast<UILayout *>(panel0->getChildByName("Panel_b"));
        UILayout * panel2_1 = static_cast<UILayout*>(panel_b->getChildByName("Panel2_1"));
        
        // 装备
        UIImageView* imageView0 = static_cast<UIImageView*>(panel2_1->getChildByName("ImageView0"));
        imageView0->setTouchEnabled(true);
        imageView0->addTouchEventListener(this, toucheventselector(PopUpUI::reinForceEquipCallBack));
        
        // 材料
        UILayout* panel2_3 = static_cast<UILayout*>(panel_b->getChildByName("Panel2_3"));
        for(int i = 0; i < 3; i++)
        {
            ImageView* bagCell = static_cast<ImageView*>(panel2_3->getChildByName(CCString::createWithFormat("ImageView%d",i+1)->getCString()));
            bagCell->setTouchEnabled(true);
            bagCell->addTouchEventListener(this, toucheventselector(PopUpUI::reinForceMeterialCallBack));
            bagCell->setTag(i);
        }
    }
    
    //锻造详细信息回调
    {
        //装备信息
        UILayout * panel_c = static_cast<UILayout *>(panel0->getChildByName("Panel_c"));
        UILayout * panel2_1 = static_cast<UILayout*>(panel_c->getChildByName("Panel2_1"));
        
        //装备
        UIImageView* imageView0 = static_cast<UIImageView*>(panel2_1->getChildByName("ImageView0"));
        imageView0->setTouchEnabled(true);
        imageView0->addTouchEventListener(this, toucheventselector(PopUpUI::composeEquipCallBack));
        
        //合成后的装备
        UIImageView* imageView1 = static_cast<UIImageView*>(panel2_1->getChildByName("ImageView1"));
        imageView1->setTouchEnabled(true);
        imageView1->addTouchEventListener(this, toucheventselector(PopUpUI::composeAfterEquipCallBack));
        
        //材料
        UILayout* panel2_2 = static_cast<UILayout*>(panel_c->getChildByName("Panel2_2"));
        for(int i = 0; i < 5; i++)
        {
            ImageView* bagCell = static_cast<ImageView*>(panel2_2->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
            bagCell->setTouchEnabled(true);
            bagCell->addTouchEventListener(this, toucheventselector(PopUpUI::composeMeterialCallBack));
            bagCell->setTag(i);
        }
    }
    
    m_pBagAllEquip = CCArray::create();
    m_pBagAllEquip->retain();
    m_pArrMeterialItem = CCArray::create();
    m_pArrMeterialItem->retain();
    updateBag();
    initPlayerAttribute();
    initTreasureContainer();
    
    // 隐藏弹出页面,装备，物品详细信息页面
    TouchGroup *touchGroup = static_cast<TouchGroup *>(getParent()->getChildByTag(TAG_POPUP_WINDOW));
    Layout *panel03 = static_cast<Layout *>(touchGroup->getWidgetByName("Panel03"));
    for (int i = 0; i < 3; i++)
    {
        CCString* strPath = CCString::createWithFormat("ImageView_%d",i);
        UIImageView* imageView_2 = static_cast<UIImageView *>(panel03->getChildByName(strPath->getCString()));
        imageView_2->setEnabled(false);
        imageView_2->setVisible(false);
    }
    
    // 隐藏列表选中框
    m_pBagSelImg->setVisible(false);
}

#pragma mark 更新背包界面

void PopUpUI::updateBag()
{
    Layout* panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* equipPanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_a"));  // 装备面板
    UIPanel* reinforcePanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_b")); // 强化面板
    UIPanel* composePanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_c")); // 锻造合成面板
    Layout * panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));  //装备 ScrollView 右侧
    Layout * panel2_3 = static_cast<Layout*>(panel_a->getChildByName("Panel2_3"));
    ScrollView * equipSView = static_cast<ScrollView*>(panel2_3->getChildByName("ScrollView0"));
    Layout * panel3 = static_cast<Layout*>(panel0->getChildByName("Panel3"));    //强化 锻造 ScrollView 左侧
    ScrollView * forceComposeSView = static_cast<ScrollView*>(panel3->getChildByName("ScrollView_0"));
    Layout * panel2 = static_cast<Layout*>(panel0->getChildByName("Panel2"));    //装备 主角 左侧
    //更新装备数组列表
    updateBagAllEquipArray();
    
    //更新装备界面背包数据
    {
        UIPanel* leftPanel= static_cast<UIPanel*>(panel0->getChildByName("Panel2"));  //装备左边角色面板
        UIPanel* roleEquipPanel= static_cast<UIPanel*>(leftPanel->getChildByName("Panel0"));
        setEquipOnTheBody(roleEquipPanel,1);// 角色身上装备
        
        UIButton* attributeButton = static_cast<UIButton*>(leftPanel->getChildByName("Button_code"));
        attributeButton->setTag(kBagUIRoleAttributeButton);
        attributeButton->addTouchEventListener(this, toucheventselector(PopUpUI::showPlayerAttributeCallBack));
        
        setRoleLabel(leftPanel,false);
    }
    
    if (strcmp(m_pBagPageButton->getName(), "Button0") == 0)
    {
        reinforcePanel->setVisible(false);
        reinforcePanel->setEnabled(false);
        composePanel->setVisible(false);
        composePanel->setEnabled(false);
        equipPanel->setVisible(true);
        equipPanel->setEnabled(true);
        panel2->setVisible(true);
        panel2->setEnabled(true);
        panel3->setVisible(false);
        panel3->setEnabled(false);
        //获取右边按钮
        UIPanel* rightPanel= static_cast<UIPanel*>(equipPanel->getChildByName("Panel2_1"));
        
        //物品
        Button* button0 = static_cast<Button*>(rightPanel->getChildByName("Button0"));
        button0->setTag(kBagUIProp);
        button0->addTouchEventListener(this, toucheventselector(PopUpUI::itemTypeSwitchCallBack));
        string str = getFilePath("209", kDirTypeUI, kFileSuffixPng);
        button0->loadTextureNormal(str.c_str());
        m_pItemTypeButton = button0;
        
        //装备
        Button* button1 = static_cast<Button*>(rightPanel->getChildByName("Button1"));
        button1->setTag(kBagUIEquip);
        button1->addTouchEventListener(this, toucheventselector(PopUpUI::itemTypeSwitchCallBack));
        str = getFilePath("210", kDirTypeUI, kFileSuffixPng);
        button1->loadTextureNormal(str.c_str());
        
        //整理
        Button *button = static_cast<Button *>(rightPanel->getChildByName("Button_clean"));
        button->setTag(kBagUIClearUpButton);
        button->addTouchEventListener(this, toucheventselector(PopUpUI::sortItem));
        
        setItemInBag(equipSView, GlobalData::getInstance()->getPropArray());
        YYSprite *player = dynamic_cast<YYSprite *>(getChildByTag(TAG_UI_PLAYER));
        if (player)
        {
            player->setVisible(true);
        }
    }
    else if (strcmp(m_pBagPageButton->getName(), "Button1") == 0)
    {
        reinforcePanel->setVisible(true);
        reinforcePanel->setEnabled(true);
        composePanel->setVisible(false);
        composePanel->setEnabled(false);
        equipPanel->setVisible(false);
        equipPanel->setEnabled(false);
        panel2->setVisible(false);
        panel2->setEnabled(false);
        panel3->setVisible(true);
        panel3->setEnabled(true);
        UIPanel* panel2_1= static_cast<UIPanel*>(reinforcePanel->getChildByName("Panel2_1"));
        
        // 100%强化
        UIButton* Button_Reinforce100 = static_cast<UIButton*>(panel2_1->getChildByName("Button_100"));
        Button_Reinforce100->setTag(kBagUIReinforceButton2);
        Button_Reinforce100->addTouchEventListener(this, toucheventselector(PopUpUI::reinforce100PercentCallBack));
        
        // 强化
        UIButton* reinforceButton = static_cast<UIButton*>(panel2_1->getChildByName("Button_strong"));
        reinforceButton->setTag(kBagUIReinforceButton1);
        reinforceButton->addTouchEventListener(this, toucheventselector(PopUpUI::reinforceCallBack));
        setItemInBagEx(forceComposeSView, m_pBagAllEquip);
        
        YYSprite *player = dynamic_cast<YYSprite *>(getChildByTag(TAG_UI_PLAYER));
        if (player)
        {
            player->setVisible(false);
        }
    }
    else if (strcmp(m_pBagPageButton->getName(), "Button2") == 0)
    {
        composePanel->setVisible(true);
        composePanel->setEnabled(true);
        reinforcePanel->setVisible(false);
        reinforcePanel->setEnabled(false);
        equipPanel->setVisible(false);
        equipPanel->setEnabled(false);
        panel2->setVisible(false);
        panel2->setEnabled(false);
        panel3->setVisible(true);
        panel3->setEnabled(true);
        UIPanel* rightPanel= static_cast<UIPanel*>(composePanel->getChildByName("Panel2_1"));
        //补全所有
        UIButton* autoCompletion = static_cast<UIButton*>(rightPanel->getChildByName("Button_code"));
        autoCompletion->setTag(kBagUIGetAllMaterialButton);
        autoCompletion->addTouchEventListener(this, toucheventselector(PopUpUI::getAllMeterialCallBack));

        //锻造
        UIButton* Button_Compose = static_cast<UIButton*>(rightPanel->getChildByName("Button_up"));
        Button_Compose->setTag(kBagUIForgeButton);
        Button_Compose->addTouchEventListener(this, toucheventselector(PopUpUI::composeCallBack));
        
        setItemInBagEx(forceComposeSView, m_pBagAllEquip);
        m_bCompose = false;
        YYSprite *player = dynamic_cast<YYSprite *>(getChildByTag(TAG_UI_PLAYER));
        if (player)
        {
            player->setVisible(false);
        }
    }
    else
    {
        return;
    }
    //金币宝石数据更新
    updateBagUIPropertyInfo();
}

#pragma mark 设置穿在身上的装备
void PopUpUI::setEquipOnTheBody(cocos2d::gui::Widget * panel, int nPage)
{
    CCArray* array = GlobalData::getInstance()->getEquipOnTheBodyArray();
    if (array)
    {
        for(int i = 0; i < panel->getChildrenCount(); i++)
        {
            const char * str = CCString::createWithFormat("%d",i)->getCString();
            ImageView* equipIcon = static_cast<ImageView *>(m_pDragRegionDictionary->objectForKey(str));
            if (!equipIcon)
            {
                if (nPage == 0)
                {
                    equipIcon = static_cast<ImageView*>(panel->getChildByName(CCString::createWithFormat("ImageView%d_0",i)->getCString()));
                    
                }
                else if(nPage == 1)
                {
                    equipIcon = static_cast<ImageView*>(panel->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
                }
                equipIcon->addTouchEventListener(this, toucheventselector(PopUpUI::bodyPartCallBack));
                equipIcon->setTag(i);
                m_pDragRegionDictionary->setObject(equipIcon, CCString::createWithFormat("%d",i)->getCString());
            }
            for(int j = 0;j < array->count(); ++j)
            {
                Equipment* equip = static_cast<Equipment*>(array->objectAtIndex(j));
                if(equip && equip->getPartIndex() == i)
                {
                    updateItemIcon(equipIcon,equip->getIconPath().c_str(),equip->getQuality(),ccp(0, 3));
                    break;
                }
            }
        }
    }
}

#pragma mark 更新所有装备数组

void PopUpUI::updateBagAllEquipArray()
{
    //更新背包内所有数据 包含已装备 和 未装备
    m_pBagAllEquip->removeAllObjects();
    m_pBagValidEquipOnBodyCount = 0;
    CCArray* arrEquipOnTheBody = GlobalData::getInstance()->getEquipOnTheBodyArray(); //已装备
    if (arrEquipOnTheBody)
    {
        for(int i = 0; i < arrEquipOnTheBody->count(); i++)
        {
          Equipment* equip = dynamic_cast<Equipment*>(arrEquipOnTheBody->objectAtIndex(i));
          if (equip)
          {
             int nComposeLv = equip->getComposeLevel();  //锻造装备所需等级
             int nReinForceMaxLevel  = equip->getMaxLevel(); //装备强化最大等级
             int nCurEquipLv = equip->getLevel();        //装备当前等级
             int nCurPlayerLv = m_pRoleByControlled->getLevel(); //玩家当前等级
             int nNeedPlayerLv = equip->getPlayerLevel();   //需求玩家等级
             if (strcmp(m_pBagPageButton->getName(), "Button1") == 0) //强化页面
             {
                if (nCurEquipLv < nReinForceMaxLevel && nCurPlayerLv > nNeedPlayerLv)  //装备当前等级 < 装备强化最大等级 当前玩家等级 > 玩家所需等级
                {
                    m_pBagAllEquip->addObject(equip);
                    m_pBagValidEquipOnBodyCount++;
                }
             }
             else if (strcmp(m_pBagPageButton->getName(), "Button2") == 0) //锻造页面
             {
                if (nCurEquipLv >= nComposeLv && nCurPlayerLv >= nNeedPlayerLv)  //装备当前等级 > 装备所需合成等级 当前玩家等级 >= 玩家所需等级
                {
                    m_pBagAllEquip->addObject(equip);
                    m_pBagValidEquipOnBodyCount++;
                }
             }
           }
         }
      }
    CCArray* arrEquip = GlobalData::getInstance()->getEquipArray(); //未装备
    if (arrEquip)
    {
        for(int i = 0; i < arrEquip->count(); i++)
        {
          Equipment* equip = dynamic_cast<Equipment*>(arrEquip->objectAtIndex(i));
          if (equip)
          {
            int nComposeLv = equip->getComposeLevel();  //锻造装备所需等级
            int nReinForceMaxLevel  = equip->getMaxLevel(); //装备强化最大等级
            int nCurEquipLv = equip->getLevel();        //装备当前等级
            int nCurPlayerLv = m_pRoleByControlled->getLevel(); //玩家当前等级
            int nNeedPlayerLv = equip->getPlayerLevel();   //需求玩家等级
            if (strcmp(m_pBagPageButton->getName(), "Button1") == 0) //强化页面
            {
              if (nCurEquipLv < nReinForceMaxLevel && nCurPlayerLv > nNeedPlayerLv)  //装备当前等级 < 装备强化最大等级
              {
                 m_pBagAllEquip->addObject(equip);
              }
            }
            else if (strcmp(m_pBagPageButton->getName(), "Button2") == 0) //锻造页面
            {
              if (nCurEquipLv >= nComposeLv && nCurPlayerLv >= nNeedPlayerLv)  //装备当前等级 > 装备所需合成等级
              {
                 m_pBagAllEquip->addObject(equip);
              }
            }
          }
        }
    }
}

#pragma mark 更新是否删除装备列表信息

void PopUpUI::updateEquipListUI()
{
    updateBagAllEquipArray();
    Layout* panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    Layout * panel3 = static_cast<Layout*>(panel0->getChildByName("Panel3"));    //强化 锻造 ScrollView 左侧
    ScrollView * forceComposeSView = static_cast<ScrollView*>(panel3->getChildByName("ScrollView_0"));
    setItemInBagEx(forceComposeSView, m_pBagAllEquip);
}

#pragma mark 初始化角色属性

void PopUpUI::initPlayerAttribute()
{
    UIPanel * panel01 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel01"));
    panel01->setEnabled(false);
    panel01->setVisible(false);
    
    UIButton * close_Button = static_cast<UIButton*>(panel01->getChildByName("Button_closed_0"));
    close_Button->addTouchEventListener(this, toucheventselector(PopUpUI::closeUI));
    
    UIPanel * panel1 = static_cast<UIPanel*>(panel01->getChildByName("Panel1_0")); // 属性界面
    for (int i = 0 ; i < panel1->getChildrenCount(); i++)
    {
        Button * button = dynamic_cast<UIButton *>(panel1->getChildByName(CCString::createWithFormat("Button%d_0",i)->getCString()));
        button->setTag(kAttributeUIPage + i);
        button->addTouchEventListener(this, toucheventselector(PopUpUI::attributePageCallBack));
        if (i == 0)
        {// 默认选中第一个页签
            m_pAttributePageButton = button;
            string str = getFilePath("206", kDirTypeUI, kFileSuffixPng);
            button->loadTextureNormal(str.c_str());
        }
    }
    
    UIPanel* leftPanel = static_cast<UIPanel*>(panel01->getChildByName("Panel3_0"));
    UIPanel* roleEquipPanel= static_cast<UIPanel*>(leftPanel->getChildByName("Panel2_0_0_0"));
    setEquipOnTheBody(roleEquipPanel,0);// 角色身上装备
    UIButton* button_Equip = static_cast<UIButton*>(leftPanel->getChildByName("Button_armor_0"));
    button_Equip->setTag(kAttributeUIBagButton);
    button_Equip->addTouchEventListener(this,toucheventselector(PopUpUI::returnBagInterfaceCallBack));
}

#pragma mark 设置背包里的物品 装备界面

void PopUpUI::setItemInBag(cocos2d::gui::Widget *panel, cocos2d::CCArray *arr)
{
    if (arr)
    {
        for(int i = 0; i < panel->getChildrenCount(); i++)
        {
            ImageView* icon = static_cast<ImageView*>(panel->getChildByName(CCString::createWithFormat("ImageView0_%d_0",i)->getCString()));
            for (int j = 0; j < icon->getChildrenCount(); )
            {//
                CCNode *node = (CCNode *)icon->getChildren()->objectAtIndex(j);
                if (dynamic_cast<Label *>(node))
                {
                    node->setVisible(false);
                    j++;
                }
                else
                {
                    icon->removeChild(node,true);
                }
            }
            
            if(i < arr->count())
            {
                Equipment *equip = dynamic_cast<Equipment*>(arr->objectAtIndex(i));
                if(equip)
                {
                    updateItemIcon(icon,equip->getIconPath().c_str(),equip->getQuality());
                    continue;
                }
                Property *prop = dynamic_cast<Property*>(arr->objectAtIndex(i));
                if(prop)
                {
                    updateItemIcon(icon ,prop->getIconPath().c_str(),prop->getQuality());
                    Label * label_0 = static_cast<Label *>(icon->getChildByName("Label_0_0"));
                    if(label_0)
                    {
                        if (prop->getNumber() > 1)
                        {
                            label_0->setText(CCString::createWithFormat("%d",prop->getNumber())->getCString());
                            label_0->setVisible(true);
                        }
                        else
                        {
                            label_0->setVisible(false);
                        }
                    }
                    if (prop->isOpened())
                    {
                        ImageView *image = ImageView::create();
                        std::string filePath = getFilePath("701", kDirTypeIcon, kFileSuffixPng);
                        image->loadTexture(filePath.c_str());
                        image->setAnchorPoint(ccp(1,1));
                        image->setPosition(ccp((image->getAnchorPoint().x - icon->getAnchorPoint().x) * icon->getContentSize().width, (image->getAnchorPoint().y - icon->getAnchorPoint().y) * icon->getContentSize().width));
                        icon->addChild(image);
                    }
                }
            }
        }
    }
}

#pragma mark  设置背包里的物品 强化 锻造 界面

void PopUpUI::setItemInBagEx(cocos2d::gui::Widget *panel, cocos2d::CCArray *arr)
{
    if (arr)
    {
        for(int i = 0; i < panel->getChildrenCount()-1; i++) // 添加选中框 个数-1
        {
            UIImageView* imageBG = static_cast<ImageView*>(panel->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
            UIImageView* icon = static_cast<ImageView*>(imageBG->getChildByName("ImageView0"));
            UILabel* name = static_cast<UILabel*>(imageBG->getChildByName("Label0"));
            UILabel* level = static_cast<UILabel*>(imageBG->getChildByName("Label1"));
            UILabel* level2 = static_cast<UILabel*>(level->getChildByName("Label0")); //所需等级
            UILabel* curLevel = static_cast<UILabel*>(imageBG->getChildByName("Label2"));
            UILabel* curLevel2 = static_cast<UILabel*>(curLevel->getChildByName("Label0")); //装备当前等级
            name->setVisible(true);
            level->setVisible(true);
            icon->setVisible(true);
            curLevel->setVisible(true);
            imageBG->setTouchEnabled(true);
            if(i < arr->count())
            {
                Equipment *equip = dynamic_cast<Equipment*>(arr->objectAtIndex(i));
                if(equip)
                {
                    //icon
                    updateItemIcon(icon,equip->getIconPath().c_str(),equip->getQuality());
                    
                    //name
                    name->setText(equip->getEquipName().c_str());
                    setTextColorByQuality(name,equip->getQuality());
                    
                    //装备当前等级
                    int nCurLv = equip->getLevel();
                    if (nCurLv == 0)
                    {
                        curLevel->setVisible(false);
                    }
                    else
                    {
                        curLevel->setVisible(true);
                        curLevel2->setText( CCString::createWithFormat("%d",equip->getLevel())->getCString());
                    }
                    
                    if (strcmp(m_pBagPageButton->getName(), "Button1") == 0) //强化页面
                    {
                        level->setText(g_sBagNeedPlayerLevel); //需求玩家等级
                        level2->setText( CCString::createWithFormat("%d",equip->getPlayerLevel())->getCString());
                    }
                    else if (strcmp(m_pBagPageButton->getName(), "Button2") == 0) //锻造页面
                    {
                        level->setText(g_sBagNeedEquipLevel);  //需求装备等级
                        level2->setText( CCString::createWithFormat("%d",equip->getComposeLevel())->getCString());
                    }  
                }
            }
            else
            {
                name->setVisible(false);
                level->setVisible(false);
                icon->setVisible(false);
                curLevel->setVisible(false);;
                imageBG->setTouchEnabled(false);
            }
        }
    }
}

#pragma mark 背包页签切换 "装备" "强化" "锻造" 切换

void PopUpUI::bagPageSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIButton* button = static_cast<UIButton*>(pSender);
        if (m_pBagPageButton != button)
        {
            //设置按键为选中状态
            string str = getFilePath("206", kDirTypeUI, kFileSuffixPng);
            button->loadTextureNormal(str.c_str());
            str = getFilePath("205", kDirTypeUI, kFileSuffixPng);
            m_pBagPageButton->loadTextureNormal(str.c_str());
            if (strcmp(m_pBagPageButton->getName(), "Button1") == 0)
            {
                resetReinforceInterface(); // 重置强化页面
            }
            else if (strcmp(m_pBagPageButton->getName(), "Button2") == 0)
            {
                resetComposeInterface();   // 重置锻造页面
            }
            m_pBagPageButton = button;
            updateBag();
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            //装备界面scrollView
            Layout * panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
            Layout * panel2_3 = static_cast<Layout*>(panel_a->getChildByName("Panel2_3"));
            ScrollView * scrollView = static_cast<ScrollView*>(panel2_3->getChildByName("ScrollView0"));
            scrollView->jumpToTop();
            //强化 锻造界面
            Layout * panel3 = static_cast<Layout*>(panel0->getChildByName("Panel3"));
            ScrollView * scrollView1 = static_cast<ScrollView*>(panel3->getChildByName("ScrollView_0"));
            scrollView1->jumpToTop();
            m_pBagSelImg->setVisible(false);
            
            hideItemPopUpInfo();
        }
    }
}

#pragma mark 背包物品类型切换(道具装备切换) 只用于装备页签

void PopUpUI::itemTypeSwitchCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened)
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        Button* button = static_cast<Button*>(pSender);
        if (button != m_pItemTypeButton)
        {
            //设置之前被选中的按键为非选中状态(在调用setBarBagData()时设置被选中按键为选中状态)
            string str = getFilePath("210", kDirTypeUI, kFileSuffixPng);
            m_pItemTypeButton->loadTextureNormal(str.c_str());
            str = getFilePath("209", kDirTypeUI, kFileSuffixPng);
            button->loadTextureNormal(str.c_str());
            m_pItemTypeButton = button;
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            Layout * panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
            Layout * panel2_3 = static_cast<Layout*>(panel_a->getChildByName("Panel2_3"));
            ScrollView * scrollView = static_cast<ScrollView*>(panel2_3->getChildByName("ScrollView0"));
            scrollView->jumpToTop();
            CCArray *arr = NULL;
            if(strcmp(m_pItemTypeButton->getName(), "Button0") == 0)
            {
               arr = GlobalData::getInstance()->getPropArray();  //物品
            }
            else
            {
               arr = GlobalData::getInstance()->getEquipArray(); //装备
            }
            setItemInBag(scrollView,arr);
            hideItemPopUpInfo();
        }
    }
}

#pragma mark 身体部位触控回调

void PopUpUI::bodyPartCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    m_pReplacedCell = static_cast<ImageView *>(pSender);
    if(type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_pReplacedCell->getChildrenCount() > 1)// 背包格内有物品图标
        {
            if(strcmp(m_pBagPageButton->getName(), "Button0") == 0)
            {// 换装界面
                Equipment *equip = getEquipOnTheBodyByPart(m_pReplacedCell->getTag());
                showEquipAttributeInterface(equip,true);
            }
        }
    }
}

#pragma mark 背包格触碰回调 装备使用

void PopUpUI::bagCellCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    m_pBagCell = static_cast<ImageView *>(pSender);
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_pBagCell->getChildrenCount() > 1)// 背包格内有物品图标
        {
            if(strcmp(m_pBagPageButton->getName(), "Button0") == 0)
            {// 换装界面
                if(strcmp(m_pItemTypeButton->getName(), "Button1") == 0)
                {// 装备栏
                    Equipment *equip = static_cast<Equipment *>(GlobalData::getInstance()->getEquipArray()->objectAtIndex(m_pBagCell->getTag()));
                    Equipment *equip1 = getEquipOnTheBodyByPart(equip->getPartIndex());
                    if (equip1)
                    {
                        showEquipAttributeInterface(equip1,true);
                    }
                    showEquipAttributeInterface(equip,false);
                }// 物品栏
                else
                {
                    Property *prop = static_cast<Property *>(GlobalData::getInstance()->getPropArray()->objectAtIndex(m_pBagCell->getTag()));
                    showPropAttribute(prop);
                }
            }
        }
    }
}

#pragma mark 背包格触碰回调 强化 锻造 使用

void PopUpUI::bagCellCallBackEx(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    m_pBagCell = static_cast<ImageView *>(pSender);
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_pBagCell->getChildrenCount() > 1)// 背包格内有物品图标
        {
            // 选中框
            m_pBagSelImg->setVisible(true);
            m_pBagSelImg->setPosition(m_pBagCell->getPosition());
            
            if(strcmp(m_pBagPageButton->getName(), "Button1") == 0)// 强化页面
            {
                string str;
                m_pBagEquip = static_cast<Equipment *>(m_pBagAllEquip->objectAtIndex(m_pBagCell->getTag()));
                if (m_pBagEquip)
                {
                    int nCount = m_pBagValidEquipOnBodyCount;
                    if (m_pBagCell->getTag() < nCount) // 已装备
                    {
                        str = CCString::createWithFormat("%d", m_pBagEquip->getPartIndex())->getCString();
                    }
                    else      // 未装备
                    {
                        str = m_pBagEquip->getID();
                    }
                    m_bReinforceConfirmed = false;
                    m_u8ReinforceIndex = m_pBagEquip->getIndex();
                    const char *msg = CCString::createWithFormat(ASK_REINFORCE_INFO,str.c_str())->getCString();
                    Controller::getInstance()->sendMessage(msg,true);
                }
            }
            else if(strcmp(m_pBagPageButton->getName(), "Button2") == 0)  // 锻造页面
            {
                CCString *str = NULL;
                m_pBagEquip = static_cast<Equipment *>(m_pBagAllEquip->objectAtIndex(m_pBagCell->getTag()));
                if (m_pBagEquip)
                {
                    int nCount = m_pBagValidEquipOnBodyCount;
                    if (m_pBagCell->getTag() < nCount) // 已装备
                    {
                        str = CCString::createWithFormat("%d",m_pBagEquip->getPartIndex());
                    }
                    else
                    {
                        str = CCString::createWithFormat("%s",m_pBagEquip->getID().c_str());
                    }
                    m_u8ComposeIndex = m_pBagEquip->getIndex();
                    const char *msg = CCString::createWithFormat(ASK_COMPOSE_INFO,str->getCString())->getCString();
                    Controller::getInstance()->sendMessage(msg,true);
                }
            }
        }
    }
}

#pragma mark 穿装备 按钮回调

void PopUpUI::putOnEquipCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        const std::string filePath = getFilePath(PUTEQUIP_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (isTouchUnable(sender, type, false))
        {
            return;
        }
        if (m_pBagCell->getTag() >= GlobalData::getInstance()->getEquipArray()->count())
        {
            return;
        }
        Equipment *equip = static_cast<Equipment *>(GlobalData::getInstance()->getEquipArray()->objectAtIndex(m_pBagCell->getTag()));
        if (equip)
        {
            m_pBagEquip = equip;
            m_pReplacedCell = static_cast<ImageView *>(m_pDragRegionDictionary->objectForKey(CCString::createWithFormat("%d",equip->getPartIndex())->getCString()));
            m_pReplacedEquip = getEquipOnTheBodyByPart(equip->getPartIndex());
            CCString *str = CCString::createWithFormat(ASK_PUT_ON_EQUIP,m_pBagEquip->getID().c_str());
            Controller::getInstance()->sendMessage(str->getCString(),true);
            hideItemPopUpInfo();
        }
    }
}

#pragma mark 穿上装备 消息回调

void PopUpUI::putOnEquip()
{
    if (m_pReplacedEquip)// 该部位有装备,与背包内的装备图标交换
    {
        GlobalData::getInstance()->getEquipArray()->insertObject(m_pReplacedEquip,m_pBagCell->getTag());
        GlobalData::getInstance()->getEquipOnTheBodyArray()->addObject(m_pBagEquip);
        ImageView * icon = ImageView::create();
        transferChildren(m_pReplacedCell, icon,CCPointZero,1);
        transferChildren(m_pBagCell, m_pReplacedCell,ccp(0,3));
        transferChildren(icon, m_pBagCell,ccp(0,-3));
    }
    GlobalData::getInstance()->getEquipOnTheBodyArray()->removeObject(m_pReplacedEquip);
    GlobalData::getInstance()->getEquipArray()->removeObject(m_pBagEquip);
    CCArray* arr = Controller::getInstance()->getScene()->getSpriteLayer()->getReplacePartPicArray();
    for(int i = 0;i < arr->count(); ++i)
    {
        const char* pic = ((CCString*)arr->objectAtIndex(i))->getCString();
        ((YYPlayer *)getChildByTag(TAG_UI_PLAYER))->replaceEquipment(pic);
    }
}


#pragma mark 将一个节点(node1)的全部子节点转移到另一个节点(node2)

void PopUpUI::transferChildren(CCNode *node1,CCNode *node2,cocos2d::CCPoint offset,int startNodeIndex)
{
    for (int i = startNodeIndex; i < node1->getChildrenCount();)
    {
        CCNode * childNode = static_cast<CCNode *>(node1->getChildren()->objectAtIndex(i));
        if (dynamic_cast<Label *>(childNode))
        {
            i++;
            continue;
        }
        childNode->retain();
        node1->removeChild(childNode,true);
        node2->addChild(childNode);
        childNode->setPosition(childNode->getPosition() + offset);
        childNode->release();
    }
}

#pragma mark 物品整理

void PopUpUI::sortItem(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened)
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (strcmp(m_pItemTypeButton->getName(), "Button0") == 0)
        {// 整理道具
            Controller::getInstance()->sendMessage(ASK_SORT_ITEM,true);
        }
        else
        {// 整理装备
            GlobalData::getInstance()->sortEquip();
            Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
            Layout * panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
            Layout * panel2_3 = static_cast<Layout*>(panel_a->getChildByName("Panel2_3"));
            ScrollView * scrollView = static_cast<ScrollView*>(panel2_3->getChildByName("ScrollView0"));
            setItemInBag(scrollView, GlobalData::getInstance()->getEquipArray());
        }
    }
}

#pragma mark 进入背包界面

void PopUpUI::returnBagInterfaceCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
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
        UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        panel0->setEnabled(true);
        panel0->setVisible(true);
        UIPanel* panel_Attribute = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel01"));
        panel_Attribute->setEnabled(false);
        panel_Attribute->setVisible(false);
        updateBag();
    }
}

#pragma mark 角色属性页面切换
void PopUpUI::attributePageCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIButton* button = static_cast<UIButton*>(pSender);
        //设置之前被选中的按键为非选中状态
        if (m_pAttributePageButton != button)
        {
            //设置按键为选中状态
            string str = getFilePath("206", kDirTypeUI, kFileSuffixPng);
            button->loadTextureNormal(str.c_str());
            str = getFilePath("205", kDirTypeUI, kFileSuffixPng);
            m_pAttributePageButton->loadTextureNormal(str.c_str());
            m_pAttributePageButton = button;
            updateAttributePage();
        }
    }
}

#pragma mark 更新角色属性页面

void PopUpUI::updateAttributePage()
{
    UIPanel* panel01 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel01"));
    UIPanel* fightPanel = static_cast<UIPanel*>(panel01->getChildByName("Panel_a_0"));  // 战斗层
    fightPanel->setVisible(false);
    UIPanel* societyPanel = static_cast<UIPanel*>(panel01->getChildByName("Panel_b_0")); // 社会层
    societyPanel->setVisible(false);
    
    if (strcmp(m_pAttributePageButton->getName(),"Button0_0") == 0)
    {
        UIPanel *panel2 = static_cast<UIPanel *>(panel01->getChildByName("Panel2_0"));
        Label *goldLabel = static_cast<Label *>(panel2->getChildByName("TextArea0_0"));
        Label *gold = static_cast<Label*>(goldLabel->getChildByName("TextArea00_0"));
        gold->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getGold())->getCString());
        Label *jewelLabel = static_cast<Label *>(panel2->getChildByName("TextArea1_0"));
        Label *jewel = static_cast<Label*>(jewelLabel->getChildByName("TextArea10_0"));
        jewel->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel())->getCString());
        fightPanel->setVisible(true);
        UIPanel* attributePanel = static_cast<UIPanel*>(fightPanel->getChildByName("Panel_a_1_0"));
        Label* textArea00 = static_cast<Label*>(attributePanel->getChildByName("TextArea00_0"));
        //战力
        Label* textArea_FightingCapacity = static_cast<Label*>(textArea00->getChildByName("TextArea_0"));
        textArea_FightingCapacity->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getFightingCapacity())->getCString());
        UILabel* textArea10 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea10_0"));
        UILabel* textArea_Hp = static_cast<UILabel*>(textArea10->getChildByName("TextArea_0"));
        textArea_Hp->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getMaxHP())->getCString());
        
        UILabel* textArea11 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea11_0"));
        UILabel* textArea_Mp = static_cast<UILabel*>(textArea11->getChildByName("TextArea_0"));
        textArea_Mp->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getMaxMP())->getCString());
        //力量
        UILabel* textArea20 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea20_0"));
        UILabel* textArea_StrengthPoint = static_cast<UILabel*>(textArea20->getChildByName("TextArea_0"));
        textArea_StrengthPoint->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getStrengthPoint())->getCString());
        //敏捷
        UILabel* textArea21 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea21_0"));
        UILabel* textArea_AgilityPoint = static_cast<UILabel*>(textArea21->getChildByName("TextArea_0"));
        textArea_AgilityPoint->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getAgilityPoint())->getCString());
        UILabel* textArea22 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea22_0"));
        UILabel* textArea_StaminaPoint = static_cast<UILabel*>(textArea22->getChildByName("TextArea_0"));
        textArea_StaminaPoint->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getStaminaPoint())->getCString());
        UILabel* textArea23 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea23_0"));
        UILabel* textArea_IntelligencePoint = static_cast<UILabel*>(textArea23->getChildByName("TextArea_0"));
        textArea_IntelligencePoint->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getIntelligencePoint())->getCString());
        
        UILabel* textArea30 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea30_0"));
        UILabel* textArea_Attack = static_cast<UILabel*>(textArea30->getChildByName("TextArea_0"));
        textArea_Attack->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getAttackPoint())->getCString());
        UILabel* textArea31 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea31_0"));
        UILabel* textArea_SpellAttack = static_cast<UILabel*>(textArea31->getChildByName("TextArea_0"));
        textArea_SpellAttack->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getSpellAttackPoint())->getCString());
        UILabel* textArea32 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea32_0"));
        UILabel* textArea_PhysicalDefensePoint = static_cast<UILabel*>(textArea32->getChildByName("TextArea_0"));
        textArea_PhysicalDefensePoint->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getDefencePoint())->getCString());
        UILabel* textArea33 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea33_0"));
        textArea33->setVisible(false);
        UILabel* textArea34 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea34_0"));
        UILabel* textArea_HitLevel = static_cast<UILabel*>(textArea34->getChildByName("TextArea_0"));
        textArea_HitLevel->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getHitLevel())->getCString());
        UILabel* textArea35 = static_cast<UILabel*>(attributePanel->getChildByName("TextArea35_0"));
        UILabel* textArea_CriticalLevel = static_cast<UILabel*>(textArea35->getChildByName("TextArea_0"));
        textArea_CriticalLevel->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getCriticalLevel())->getCString());
    }
    else
    {
        societyPanel->setVisible(true);
    }
}

#pragma mark 显示人物属性页面

void PopUpUI::showPlayerAttributeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        panel0->setEnabled(false);
        panel0->setVisible(false);
        UIPanel* panel_Attribute = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel01"));
        panel_Attribute->setEnabled(true);
        panel_Attribute->setVisible(true);
        
        UIPanel* panel_2 = static_cast<UIPanel*>(panel_Attribute->getChildByName("Panel2_0")); // 金币 钻石层
        UILabel* textArea0 = static_cast<UILabel*>(panel_2->getChildByName("TextArea0_0"));
        UILabel* text_Money = static_cast<UILabel*>(textArea0->getChildByName("TextArea00_0"));
        
        text_Money->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getGold())->getCString());
        UILabel* textArea1 = static_cast<UILabel*>(panel_2->getChildByName("TextArea1_0"));
        UILabel* text_Jewel = static_cast<UILabel*>(textArea1->getChildByName("TextArea10_0"));
        text_Jewel->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel())->getCString());
        
        UIPanel* panel_3 = static_cast<UIPanel*>(panel_Attribute->getChildByName("Panel3_0")); // 左边人物层
        setRoleLabel(panel_3,true);  //角色属性
        UIPanel* panel_RoleBody= static_cast<UIPanel*>(panel_3->getChildByName("Panel2_0_0_0"));
        for(int i=0; i < 10; ++i)
        {
            UIImageView* equipIcon = (UIImageView*)panel_RoleBody->getChildren()->objectAtIndex(i);
            
            string str = getFilePath("207",kDirTypeUI,kFileSuffixPng);
            equipIcon->loadTexture(str.c_str());
            
            CCArray *arr  = GlobalData::getInstance()->getEquipOnTheBodyArray();
            for(int j = 0; j < arr->count(); j++)
            {
                Equipment* equip = static_cast<Equipment *>(arr->objectAtIndex(j));
                if(equip && equip->getPartIndex()== i)
                {
                    updateItemIcon(equipIcon,equip->getIconPath().c_str(), equip->getQuality()/*,ccp(0,3)*/);
                    break;
                }
            }
        }
        updateAttributePage();
    }
}

#pragma mark 进入人物角色按钮

void PopUpUI::drawRoleUI()
{
    initBagUI();
    
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    panel0->setEnabled(false);
    panel0->setVisible(false);
    UIPanel* panel_Attribute = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel01"));
    panel_Attribute->setEnabled(true);
    panel_Attribute->setVisible(true);
    
    UIPanel* panel_2 = static_cast<UIPanel*>(panel_Attribute->getChildByName("Panel2_0")); // 金币 钻石层
    UILabel* textArea0 = static_cast<UILabel*>(panel_2->getChildByName("TextArea0_0"));
    UILabel* text_Money = static_cast<UILabel*>(textArea0->getChildByName("TextArea00_0"));
    
    text_Money->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getGold())->getCString());
    UILabel* textArea1 = static_cast<UILabel*>(panel_2->getChildByName("TextArea1_0"));
    UILabel* text_Jewel = static_cast<UILabel*>(textArea1->getChildByName("TextArea10_0"));
    text_Jewel->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel())->getCString());
    
    UIPanel* panel_3 = static_cast<UIPanel*>(panel_Attribute->getChildByName("Panel3_0")); // 左边人物层
    setRoleLabel(panel_3,true); //角色属性
    UIPanel* panel_RoleBody= static_cast<UIPanel*>(panel_3->getChildByName("Panel2_0_0_0"));
    for(int i=0;i < 10; ++i)
    {
        UIImageView* equipIcon = (UIImageView*)panel_RoleBody->getChildren()->objectAtIndex(i);
        
        string str = getFilePath("207", kDirTypeUI, kFileSuffixPng);
        equipIcon->loadTexture(str.c_str());
        
        CCArray *arr  = GlobalData::getInstance()->getEquipOnTheBodyArray();
        for(int j = 0; j < arr->count(); j++)
        {
            Equipment* equip = static_cast<Equipment *>(arr->objectAtIndex(j));
            if(equip && equip->getPartIndex()== i)
            {
                updateItemIcon(equipIcon,equip->getIconPath().c_str(), equip->getQuality()/*,ccp(0,3)*/);
                break;
            }
        }
    }
    updateAttributePage();
}

#pragma mark 通过装备品质更新图片

void PopUpUI::updateItemIcon(cocos2d::gui::UIWidget * widget,const char *iconPath, int quality, CCPoint point)
{
    widget->removeChildByTag(TAG_ICON_BG);
    widget->removeChildByTag(TAG_ITEM_ICON);
    CCString *str = CCString::createWithFormat("i-9-0-0-%d", quality - 1);
    std::string filePath = getFilePath(str->getCString(), kDirTypeIcon, kFileSuffixPng);
    ImageView * iconBG = ImageView::create();
    if (iconBG)
    {
        iconBG->loadTexture(filePath.c_str());
        iconBG->setPosition(iconBG->getPosition() + point);// 背景元素格略有偏移
        iconBG->setTag(TAG_ICON_BG);
        widget->addChild(iconBG);
    }
    ImageView * itemIcon = ImageView::create();
    if (itemIcon)
    {
        itemIcon->loadTexture(iconPath);
        itemIcon->setPosition(itemIcon->getPosition() + point);
        itemIcon->setTag(TAG_ITEM_ICON);
        widget->addChild(itemIcon);
    }
}

#pragma mark 强化

void PopUpUI::updateReinforceInterface(cocos2d::CCArray * array)
{
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* reinforcePanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_b"));
    UIPanel* StrongPanel = static_cast<UIPanel*>(reinforcePanel->getChildByName("Panel2_1"));     //强化
    UIPanel* attributePanel = static_cast<UIPanel*>(reinforcePanel->getChildByName("Panel2_2"));  //属性
    UIPanel* materialPanel = static_cast<UIPanel*>(reinforcePanel->getChildByName("Panel2_3"));   //材料
    
    CCArray * attributeArray = split_string(static_cast<CCString *>(array->objectAtIndex(2))->getCString(), ';');
    int odds = static_cast<CCString *>(array->objectAtIndex(3))->intValue();// 强化成功率
    int gold = static_cast<CCString *>(array->objectAtIndex(4))->intValue();// 强化需花费金钱数
    int jew = static_cast<CCString *>(array->objectAtIndex(5))->intValue();// 100%强化需花费宝石数
    int level = m_pBagEquip->getLevel();
    m_nConsumeJewelNum = jew;
    //等级
    UILabel *levelLabel = static_cast<UILabel*>(StrongPanel->getChildByName("TextArea_level"));
    UILabel *levelValue = static_cast<Label*>(levelLabel->getChildByName("TextArea_0"));
    levelValue->setText(CCString::createWithFormat("%d",level)->getCString());
    levelValue->setVisible(true);
    
    //装备图片
    UIImageView *levelImgView = static_cast<UIImageView*>(StrongPanel->getChildByName("ImageView0"));
    updateItemIcon(levelImgView, m_pBagEquip->getIconPath().c_str(), m_pBagEquip->getQuality());
    
    UIButton* btnStrong = static_cast<UIButton*>(StrongPanel->getChildByName("Button_strong"));
    Label *oddsLabel = static_cast<Label*>(btnStrong->getChildByName("TextArea_rate"));
    Label *oddsValue = static_cast<Label*>(oddsLabel->getChildByName("TextArea_0"));
    oddsValue->setText(CCString::createWithFormat("%d",odds)->getCString());
    oddsValue->setVisible(true);
    Label *goldLabel = static_cast<Label*>(btnStrong->getChildByName("TextArea_money"));
    Label *goldValue = static_cast<Label*>(goldLabel->getChildByName("TextArea_0"));
    goldValue->setText(CCString::createWithFormat("%d",gold)->getCString());
    goldValue->setVisible(true);
    UIButton* btn_100 = static_cast<UIButton*>(StrongPanel->getChildByName("Button_100"));
    Label *jewLabel = static_cast<Label*>(btn_100->getChildByName("TextArea_jew"));
    Label *jewValue = static_cast<Label*>(jewLabel->getChildByName("TextArea_0"));
    jewValue->setText(CCString::createWithFormat("%d",jew)->getCString());
    jewValue->setVisible(true);
    
    //特殊处理 查找属性值不为0的两项
    CCArray* attributeArray2 = CCArray::create();
    for (int i = 0; i < 8; i++)
    {
        CCArray * segValue = split_string(static_cast<CCString *>(attributeArray->objectAtIndex(i))->getCString(), ',');
        if ((static_cast<CCString *>(segValue->objectAtIndex(1)))->intValue() != 0)
        {
            attributeArray2->addObject(attributeArray->objectAtIndex(i));
        }
    }
    
    Label * strLabel = static_cast<Label*>(attributePanel->getChildByName("Label_1"));
    Label * valueLabel1 = static_cast<Label*>(strLabel->getChildByName("Label_0"));
    Label * signLabel = static_cast<Label*>(strLabel->getChildByName("Label_1"));
    Label * valueLabel2 = static_cast<Label*>(strLabel->getChildByName("Label_2"));
    CCArray * strengthValue = split_string(static_cast<CCString *>(attributeArray2->objectAtIndex(0))->getCString(), ',');
    int value = static_cast<CCString *>(strengthValue->objectAtIndex(1))->intValue();
    valueLabel1->setText(CCString::createWithFormat("%d",value)->getCString());
    strLabel->setText(((CCString*)strengthValue->objectAtIndex(0))->getCString());
    if(level < m_pBagEquip->getMaxLevel())
    {
        signLabel->setVisible(true);
        valueLabel2->setVisible(true);
        value = static_cast<CCString *>(strengthValue->objectAtIndex(2))->intValue();
        valueLabel2->setText(CCString::createWithFormat("%d",value)->getCString());
    }
    else
    {
        signLabel->setVisible(false);
        valueLabel2->setVisible(false);
    }
    
    Label * agiLabel = static_cast<Label*>(attributePanel->getChildByName("Label_2"));
    valueLabel1 = static_cast<Label*>(agiLabel->getChildByName("Label_0"));
    signLabel = static_cast<Label*>(agiLabel->getChildByName("Label_1"));
    valueLabel2 = static_cast<Label*>(agiLabel->getChildByName("Label_2"));
    CCArray * agilityValue = split_string(static_cast<CCString *>(attributeArray2->objectAtIndex(1))->getCString(), ',');
    value = static_cast<CCString *>(agilityValue->objectAtIndex(1))->intValue();
    valueLabel1->setText(CCString::createWithFormat("%d",value)->getCString());
    agiLabel->setText(((CCString*)agilityValue->objectAtIndex(0))->getCString());
    if(level < m_pBagEquip->getMaxLevel())
    {
        signLabel->setVisible(true);
        valueLabel2->setVisible(true);
        value = static_cast<CCString *>(agilityValue->objectAtIndex(2))->intValue();
        valueLabel2->setText(CCString::createWithFormat("%d",value)->getCString());
    }
    else
    {
        signLabel->setVisible(false);
        valueLabel2->setVisible(false);
    }
    
    //删除之前的材料
    m_pArrMeterialItem->removeAllObjects();
    
    // 强化材料
    for (int i = 0; i < 3; i++)
    {
        ImageView *material = static_cast<ImageView *>(materialPanel->getChildByName(CCString::createWithFormat("ImageView%d",i + 1)->getCString()));
        material->removeChildByTag(100);
        Label *label_1 = static_cast<Label*>(material->getChildByName("Label_1"));
        label_1->setVisible(false);
        Label * numLabel1 = static_cast<Label*>(material->getChildByName("Label_2"));
        numLabel1->setVisible(false);
        Label * numLabel2 = static_cast<Label*>(material->getChildByName("Label_0_1"));
        numLabel2->setVisible(false);
        Label * numLabel3 = static_cast<Label*>(material->getChildByName("Label_0_0"));
        numLabel3->setVisible(false);
        if (array->count() > 6 + i)
        {
            CCArray *materialArray1 = split_string(static_cast<CCString *>(array->objectAtIndex(6+i))->getCString(),',');
            ImageView *icon = ImageView::create();
            std::string str = getFilePath(static_cast<CCString *>(materialArray1->objectAtIndex(1))->getCString(), kDirTypeIcon, kFileSuffixPng);
            icon->loadTexture(str.c_str());
            icon->setTag(100);
            material->addChild(icon);
            // 需求数量
            int num1 = static_cast<CCString *>(materialArray1->objectAtIndex(4))->intValue();
            label_1->setVisible(true);
            numLabel1->setVisible(true);
            numLabel1->setText(CCString::createWithFormat("%d",num1)->getCString());
            int num2 = static_cast<CCString *>(materialArray1->objectAtIndex(3))->intValue();
            if (num2 >= num1)
            {
                numLabel3->setVisible(false);
                numLabel2->setVisible(true);
                numLabel2->setText(CCString::createWithFormat("%d",num2)->getCString());
            }
            else
            {
                numLabel2->setVisible(false);
                numLabel3->setVisible(true);
                numLabel3->setText(CCString::createWithFormat("%d",num2)->getCString());
            }
            m_pArrMeterialItem->addObject(materialArray1->objectAtIndex(0));  // 添加到item
        }
    }
    
    //强化特效
    if(m_bReinforceConfirmed)  // 区分为点击强化按钮(与查看属性区分)
    {
        UIImageView* imageView3 = static_cast<UIImageView*>(materialPanel->getChildByName("ImageView3"));
        m_bReinforceConfirmed = false;
        if(!m_bReinforceSuccess) // 强化失败特效
        {
            YYSprite *sprite = YYSprite::createWithActionSet("se3", "se3", kDirTypeEffect);
            sprite->setAction(0, false);
            levelImgView->addNode(sprite);
            m_pSpecialEffectArray->addObject(sprite);
            
            sprite = YYSprite::createWithActionSet("se22", "se23", kDirTypeEffect);
            sprite->setAction(0, false);
            imageView3->addNode(sprite,10);
            sprite->setPosition(ccp(sprite->getPositionX()-imageView3->getContentSize().width-20,sprite->getPositionY()));
            m_pSpecialEffectArray->addObject(sprite);
            const std::string filePath = getFilePath(EQUIPFAILED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
        else
        {
            m_bReinforceSuccess = false;
            YYSprite *sprite = YYSprite::createWithActionSet("se2", "se2", kDirTypeEffect);
            sprite->setAction(0, false);
            levelImgView->addNode(sprite);
            m_pSpecialEffectArray->addObject(sprite);
            
            sprite = YYSprite::createWithActionSet("se9", "se10", kDirTypeEffect);
            sprite->setAction(0, false);
            imageView3->addNode(sprite,10);
            sprite->setPosition(ccp(sprite->getPositionX()-imageView3->getContentSize().width-20,sprite->getPositionY()));
            m_pSpecialEffectArray->addObject(sprite);
            const std::string filePath = getFilePath(EQUIPSUCCESS_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
        }
    }
    //强化成功 更新左边列表栏内的数据
    updateReinforceScrollCell();
    
    //强化到最高等级 删除列表数据
    {
        Equipment* equip = m_pBagEquip;
        int nReinForceMaxLevel  = equip->getMaxLevel(); //装备强化最大等级
        int nCurEquipLv = equip->getLevel();        //装备当前等级
        int nCurPlayerLv = m_pRoleByControlled->getLevel(); //玩家当前等级
        int nNeedPlayerLv = equip->getPlayerLevel();   //需求玩家等级

        if (nCurPlayerLv <= nNeedPlayerLv || nCurEquipLv >= nReinForceMaxLevel)  //玩家等级 <= 装备所需玩家等级 装备当前等级 <= 装备强化最大等级
        {
            updateEquipListUI();
            m_pBagSelImg->setVisible(false);  //选中图片隐藏
            resetReinforceInterface();        //重置强化页面
        }
    }
}

void PopUpUI::updateReinforceScrollCell()
{
    //当前选中装备m_pBagEquip
    UIImageView* icon = static_cast<ImageView*>(m_pBagCell->getChildByName("ImageView0"));
    UILabel* name = static_cast<UILabel*>(m_pBagCell->getChildByName("Label0"));
    UILabel* level = static_cast<UILabel*>(m_pBagCell->getChildByName("Label1"));
    UILabel* level2 = static_cast<UILabel*>(level->getChildByName("Label0")); //所需等级
    UILabel* curLevel = static_cast<UILabel*>(m_pBagCell->getChildByName("Label2"));
    UILabel* curLevel2 = static_cast<UILabel*>(curLevel->getChildByName("Label0")); //装备当前等级
    if(m_pBagEquip)
    {
        //icon
        updateItemIcon(icon,m_pBagEquip->getIconPath().c_str(),m_pBagEquip->getQuality());
            
        //name
        name->setText(m_pBagEquip->getEquipName().c_str());
        setTextColorByQuality(name,m_pBagEquip->getQuality());
            
        //装备当前等级
        
        int nCurLv = m_pBagEquip->getLevel();
        if (nCurLv == 0)
        {
            curLevel->setVisible(false);
        }
        else
        {
            curLevel->setVisible(true);
            curLevel2->setText( CCString::createWithFormat("%d",m_pBagEquip->getLevel())->getCString());
        }
        
        //所需玩家等级
        level2->setText( CCString::createWithFormat("%d",m_pBagEquip->getPlayerLevel())->getCString());
    }
}


void PopUpUI::resetReinforceInterface()
{
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* reinforcePanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_b"));
    UIPanel* StrongPanel = static_cast<UIPanel*>(reinforcePanel->getChildByName("Panel2_1"));     //强化
    UIPanel* attributePanel = static_cast<UIPanel*>(reinforcePanel->getChildByName("Panel2_2"));  //属性
    UIPanel* materialPanel = static_cast<UIPanel*>(reinforcePanel->getChildByName("Panel2_3"));   //材料
    
    //装备图片
    UIImageView *levelImgView = static_cast<UIImageView*>(StrongPanel->getChildByName("ImageView0"));
    if (levelImgView)
    {
        levelImgView->removeAllChildren();
    }
    
    //等级信息
    UILabel *levelLabel = static_cast<UILabel*>(StrongPanel->getChildByName("TextArea_level"));
    UILabel *levelValue = static_cast<Label*>(levelLabel->getChildByName("TextArea_0"));
    levelValue->setVisible(false);
    
    //成功率
    UIButton* btnStrong = static_cast<UIButton*>(StrongPanel->getChildByName("Button_strong"));
    Label *oddsLabel = static_cast<Label*>(btnStrong->getChildByName("TextArea_rate"));
    Label *oddsValue = static_cast<Label*>(oddsLabel->getChildByName("TextArea_0"));
    oddsValue->setVisible(false);
    
    //金币
    Label *goldLabel = static_cast<Label*>(btnStrong->getChildByName("TextArea_money"));
    Label *goldValue = static_cast<Label*>(goldLabel->getChildByName("TextArea_0"));
    goldValue->setVisible(false);
    
    //宝石
    UIButton* btn_100 = static_cast<UIButton*>(StrongPanel->getChildByName("Button_100"));
    Label *jewLabel = static_cast<Label*>(btn_100->getChildByName("TextArea_jew"));
    Label *jewValue = static_cast<Label*>(jewLabel->getChildByName("TextArea_0"));
    jewValue->setVisible(false);
    
    //力量
    Label * strLabel = static_cast<Label*>(attributePanel->getChildByName("Label_1"));
    Label * valueLabel1 = static_cast<Label*>(strLabel->getChildByName("Label_0"));
    Label * signLabel = static_cast<Label*>(strLabel->getChildByName("Label_1"));
    Label * valueLabel2 = static_cast<Label*>(strLabel->getChildByName("Label_2"));
    strLabel->setVisible(false);
    valueLabel1->setVisible(false);
    signLabel->setVisible(false);
    valueLabel2->setVisible(false);
    
    //敏捷
    Label * agiLabel = static_cast<Label*>(attributePanel->getChildByName("Label_2"));
    valueLabel1 = static_cast<Label*>(agiLabel->getChildByName("Label_0"));
    signLabel = static_cast<Label*>(agiLabel->getChildByName("Label_1"));
    valueLabel2 = static_cast<Label*>(agiLabel->getChildByName("Label_2"));
    agiLabel->setVisible(false);
    valueLabel1->setVisible(false);
    signLabel->setVisible(false);
    valueLabel2->setVisible(false);
    
    //材料
    for (int j = 1; j < 4; j++)
    {
        ImageView *material = static_cast<ImageView *>(materialPanel->getChildByName(CCString::createWithFormat("ImageView%d",j)->getCString()));
        material->removeChildByTag(100);
        Label *label_1 = static_cast<Label*>(material->getChildByName("Label_1"));
        label_1->setVisible(false);
        Label * numLabel1 = static_cast<Label*>(material->getChildByName("Label_2"));
        numLabel1->setVisible(false);
        Label * numLabel2 = static_cast<Label*>(material->getChildByName("Label_0_1"));
        Label * numLabel3 = static_cast<Label*>(material->getChildByName("Label_0_0"));
        numLabel2->setVisible(false);
        numLabel3->setVisible(false);
    }
    
    m_pReplacedCell = NULL;
    m_pBagCell = NULL;
    m_pBagEquip = NULL;
    m_pReplacedEquip = NULL;
}

void PopUpUI::reinforceCallBack(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || !m_pBagEquip || m_bEquipDescription)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        m_bReinforceConfirmed = true;
        CCString *str = NULL;
        m_pBagEquip = static_cast<Equipment *>(m_pBagAllEquip->objectAtIndex(m_pBagCell->getTag()));
        int nCount = m_pBagValidEquipOnBodyCount;
        if (m_pBagCell && m_pBagCell->getTag() < nCount) //已装备
        {
            str = CCString::createWithFormat("%d",m_pBagEquip->getPartIndex());
        }
        else
        {
            str = CCString::createWithFormat("%s",m_pBagEquip->getID().c_str());
        }
        CCString *str1 = CCString::createWithFormat(ASK_REINFORCE_MONEY,str->getCString());
        Controller::getInstance()->sendMessage(str1->getCString(),true);
    }
}

void PopUpUI::reinforce100PercentCallBack(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || !m_pBagEquip || m_bEquipDescription)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        setConsumeJewelTips(kJewelConsumeForReinforce, m_nConsumeJewelNum);
    }
}
void PopUpUI::reinforceConsumeJewelLogic()
{
    m_pRoleByControlled = Controller::getInstance()->getScene()->getSpriteLayer()->getRoleByControlled();
    int nJew = m_pRoleByControlled->getSpriteAttribute()->getJewel();
    if (nJew <= 0 || nJew < m_nConsumeJewelNum)
    {
        ((YYScene*)getParent())->showTips(g_sLackJew, kTipJewCharge);
        return;
    }
    m_bReinforceConfirmed = true;
    CCString *str = NULL;
    m_pBagEquip = static_cast<Equipment *>(m_pBagAllEquip->objectAtIndex(m_pBagCell->getTag()));
    int nCount = m_pBagValidEquipOnBodyCount;
    if (m_pBagCell && m_pBagCell->getTag() < nCount) //已装备
    {
        str = CCString::createWithFormat("%d",m_pBagEquip->getPartIndex());
    }
    else
    {
        str = CCString::createWithFormat("%s",m_pBagEquip->getID().c_str());
    }
    const char *msg = CCString::createWithFormat(ASK_REINFORCE_JEWEL,str->getCString())->getCString();
    Controller::getInstance()->sendMessage(msg, true);
}
//强化失败提示
void PopUpUI::showReinforceFailedTips(int tipsType)
{
    if (tipsType == 0)  //强化类型不对（坐骑、宠物）
    {
        ((YYScene*)getParent())->showTips(g_sCanNotType);
    }
    else if (tipsType == 1)           //强化钱的类型不对
    {
        ((YYScene*)getParent())->showTips(g_sCanNotType);
    }
    else if (tipsType == 2)           //强化装备不存在
    {
        ((YYScene*)getParent())->showTips(g_sCanNotMiss);
    }
    else if (tipsType == 3)           //装备达到最大强化等级
    {
        ((YYScene*)getParent())->showTips(g_sCanNotReinforce);
    }
    else if (tipsType == 4)           //强化所需金币不够
    {
        ((YYScene*)getParent())->showTips(g_sLackGold);
    }
    else if (tipsType == 5)          //强化所需宝石不够
    {
        ((YYScene*)getParent())->showTips(g_sLackJew);
    }
    else if (tipsType == 6)         //玩家等级不够
    {
        ((YYScene*)getParent())->showTips(g_sCanNotLevel);
    }
    else if (tipsType == 7)         //强化材料不够
    {
        ((YYScene*)getParent())->showTips(g_sMissingMaterialForReinforce);
    }
    else if (tipsType == 8)         //一键补全宝石不够
    {
        ((YYScene*)getParent())->showTips(g_sLackJew);
    }
}

#pragma mark 锻造

void PopUpUI::updateComposeInterface(cocos2d::CCArray * array)
{
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* composePanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_c"));
    UIPanel* rightPanel = static_cast<UIPanel*>(composePanel->getChildByName("Panel2_1"));
    UIPanel* materialPanel = static_cast<UIPanel*>(composePanel->getChildByName("Panel2_2"));
    
    m_pArrMeterialItem->removeAllObjects();
    
    //材料
    for (int i = 0; i < array->count(); i++)
    {
        ImageView *icon = static_cast<ImageView *>(materialPanel->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
        icon->setVisible(true);
        for (int j = 0; j < icon->getChildrenCount(); j++)
        {
            ImageView *imageView = dynamic_cast<ImageView *>(icon->getChildren()->objectAtIndex(j));
            if (imageView)
            {
                icon->removeChild(imageView,true);
            }
        }
        if(i < array->count()-1)
        {
            CCArray *materialArray = split_string(static_cast<CCString *>(array->objectAtIndex(i+1))->getCString(),',');
            string str = getFilePath(static_cast<CCString *>(materialArray->objectAtIndex(1))->getCString(), kDirTypeIcon, kFileSuffixPng);
            ImageView *icon1 = ImageView::create();
            icon1->loadTexture(str.c_str());
            icon->addChild(icon1);
            // 需求数量
            int num1 = static_cast<CCString *>(materialArray->objectAtIndex(4))->intValue();
            Label * numLabel1 = static_cast<Label*>(icon->getChildByName("Label_2"));
            numLabel1->setText(CCString::createWithFormat("%d",num1)->getCString());
            numLabel1->setVisible(true);
            
            Label * sign = static_cast<Label*>(icon->getChildByName("Label_1"));
            sign->setVisible(true);
            // 已有数量
            int num2 = static_cast<CCString *>(materialArray->objectAtIndex(3))->intValue();
            if (num2 >= num1)
            {
                Label * numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_1"));
                numLabel2->setText(CCString::createWithFormat("%d",num2)->getCString());
                numLabel2->setVisible(true);
                Label * numLabel0 = static_cast<Label*>(icon->getChildByName("Label_0_0"));
                numLabel0->setVisible(false);
            }
            else
            {
                Label * numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_0"));
                numLabel2->setText(CCString::createWithFormat("%d",num2)->getCString());
                numLabel2->setVisible(true);
                Label * numLabel1 = static_cast<Label*>(icon->getChildByName("Label_0_1"));
                numLabel1->setVisible(false);
            }
            
            m_pArrMeterialItem->addObject(materialArray->objectAtIndex(0));
        }
        else
        {
            Label * sign = static_cast<Label*>(icon->getChildByName("Label_1"));
            sign->setVisible(false);
            Label * numLabel1 = static_cast<Label*>(icon->getChildByName("Label_2"));
            numLabel1->setVisible(false);
            Label * numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_1"));
            numLabel2->setVisible(false);
            numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_0"));
            numLabel2->setVisible(false);
        }
    }
    
    //装备图标
    ImageView *equipImgView = dynamic_cast<ImageView *>(rightPanel->getChildByName("ImageView0"));
    updateItemIcon(equipImgView, m_pBagEquip->getIconPath().c_str(), m_pBagEquip->getQuality());
    
    //删除之前预览图标
    UIImageView *preViewImgView = dynamic_cast<UIImageView *>(rightPanel->getChildByName("ImageView1"));
    ImageView *preEquipImageView = dynamic_cast<UIImageView *>(preViewImgView->getChildByTag(20));
    if (preEquipImageView)
    {
        preViewImgView->removeChild(preEquipImageView,true);
    }
    
    //预览图标
    string str = getFilePath(static_cast<CCString *>(array->objectAtIndex(0))->getCString(),kDirTypeIcon,kFileSuffixPng);    UIImageView * newIcon = UIImageView::create();
    newIcon->loadTexture(str.c_str());
    newIcon->setTag(20);
    preViewImgView->addChild(newIcon);
}

void PopUpUI::processEquipComposeSucceed()
{
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* composePanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_c"));
    UIPanel* rightPanel = static_cast<UIPanel*>(composePanel->getChildByName("Panel2_1"));
    UIPanel* materialPanel = static_cast<UIPanel*>(composePanel->getChildByName("Panel2_2"));
    
    for (int i = 0; i < 6; i ++)
    {
        ImageView *icon = static_cast<ImageView *>(materialPanel->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
        icon->setVisible(true);
        for (int j = 0; j < icon->getChildrenCount(); j++)
        {
            ImageView *imageView = dynamic_cast<ImageView *>(icon->getChildren()->objectAtIndex(j));
            if (imageView)
            {
                icon->removeChild(imageView,true);
            }
        }
        Label * sign = static_cast<Label*>(icon->getChildByName("Label_1"));
        sign->setVisible(false);
        Label * numLabel1 = static_cast<Label*>(icon->getChildByName("Label_2"));
        numLabel1->setVisible(false);
        Label * numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_1"));
        numLabel2->setVisible(false);
        numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_0"));
        numLabel2->setVisible(false);
    }
    
    //删除装备图标
    ImageView *equipImgView = dynamic_cast<ImageView *>(rightPanel->getChildByName("ImageView0"));
    equipImgView->removeChildByTag(TAG_ICON_BG);
    equipImgView->removeChildByTag(TAG_ITEM_ICON);
    
    //删除预览图标
    UIImageView *preViewImgView = dynamic_cast<UIImageView *>(rightPanel->getChildByName("ImageView1"));
    ImageView *preEquipImageView = dynamic_cast<UIImageView *>(preViewImgView->getChildByTag(20));
    
    //添加预览图标移动特效
    CCActionInterval* action1 = CCFadeOut::create(2.0f);
    preEquipImageView->runAction(action1);
    CCActionInterval* action2 = CCMoveTo::create(2.0f, ccp(-360.0f,preEquipImageView->getPositionY()));
    preEquipImageView->runAction(action2);
    preEquipImageView->runAction(CCSequence::create(CCDelayTime::create(2.0f),CCHide::create(),NULL));
    
    //添加锻造成功特效
    UIImageView* ImageView5 = static_cast<ImageView *>(materialPanel->getChildByName("ImageView5"));
    YYSprite * spriteEffect = YYSprite::createWithActionSet("se9", "se11", kDirTypeEffect);
    spriteEffect->setAction(0, false);
    ImageView5->addNode(spriteEffect,10);
    spriteEffect->setPosition(ccp(spriteEffect->getPositionX()-ImageView5->getContentSize().width-35,spriteEffect->getPositionY()+ImageView5->getContentSize().height));
    m_pSpecialEffectArray->addObject(spriteEffect);
    const std::string filePath = getFilePath(EQUIPSUCCESS_EFFECT ,kDirTypeSound ,kFileSuffixWav);
    PLAY_EFFECT(filePath.c_str());
    //删除锻造后的cell
    updateEquipListUI();
    m_pBagSelImg->setVisible(false);
    
    // 还原 合成槽位置
    m_pReplacedCell = NULL;
    m_pBagCell = NULL;
    m_pReplacedEquip = NULL;
    m_pBagEquip = NULL;
    
    //锻造成功后更新背包内人物身上装备
    CCArray* arr = Controller::getInstance()->getScene()->getSpriteLayer()->getReplacePartPicArray();
    for(int i = 0;i < arr->count(); ++i)
    {
        const char* pic = ((CCString*)arr->objectAtIndex(i))->getCString();
        ((YYPlayer *)getChildByTag(TAG_UI_PLAYER))->replaceEquipment(pic);
    }
}

void PopUpUI::updateComposeScrollCell()
{
    //当前选中装备m_pBagEquip
    UIImageView* icon = static_cast<ImageView*>(m_pBagCell->getChildByName("ImageView0"));
    UILabel* name = static_cast<UILabel*>(m_pBagCell->getChildByName("Label0"));
    UILabel* level = static_cast<UILabel*>(m_pBagCell->getChildByName("Label1"));
    UILabel* level2 = static_cast<UILabel*>(level->getChildByName("Label0")); //所需等级
    UILabel* curLevel = static_cast<UILabel*>(m_pBagCell->getChildByName("Label2"));
    UILabel* curLevel2 = static_cast<UILabel*>(curLevel->getChildByName("Label0")); //装备当前等级
    if(m_pBagEquip)
    {
        //icon
        updateItemIcon(icon,m_pBagEquip->getIconPath().c_str(),m_pBagEquip->getQuality());
        
        //name
        name->setText(m_pBagEquip->getEquipName().c_str());
        setTextColorByQuality(name,m_pBagEquip->getQuality());
        
        //装备当前等级
        curLevel2->setText( CCString::createWithFormat("%d",m_pBagEquip->getLevel())->getCString());
        level2->setText( CCString::createWithFormat("%d",m_pBagEquip->getComposeLevel())->getCString());
    }
}

//补全所有材料
void PopUpUI::getAllMeterialCallBack(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || !m_pBagEquip || m_bEquipDescription)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        CCString *str = NULL;
        int nCount = m_pBagValidEquipOnBodyCount;
        if (m_pBagCell && m_pBagCell->getTag() < nCount) //已装备
        {
            str = CCString::createWithFormat("%d",m_pBagEquip->getPartIndex());
        }
        else
        {
            str = CCString::createWithFormat("%s",m_pBagEquip->getID().c_str());
        }
        CCString *str1 = CCString::createWithFormat(ASK_MATERIAL,str->getCString());
        Controller::getInstance()->sendMessage(str1->getCString(),true);
    }
}
void PopUpUI::getMeterialConsumeJewlLogic()
{
    m_pRoleByControlled = Controller::getInstance()->getScene()->getSpriteLayer()->getRoleByControlled();
    int nJew = m_pRoleByControlled->getSpriteAttribute()->getJewel();
    if (nJew <= 0 || nJew < m_nConsumeJewelNum)
    {
        ((YYScene*)getParent())->showTips(g_sLackJew, kTipJewCharge);
        return;
    }

    CCString *str = NULL;
    if (m_pBagCell && m_pBagCell->getTag() < m_pBagValidEquipOnBodyCount)
    {
        str = CCString::createWithFormat("%d",m_pBagEquip->getPartIndex());
    }
    else
    {
        str = CCString::createWithFormat("%s",m_pBagEquip->getID().c_str());
    }
    CCString *str1 = CCString::createWithFormat(ASK_COMPOSE_JEWEL,str->getCString());
    Controller::getInstance()->sendMessage(str1->getCString(),true);
}
void PopUpUI::composeCallBack(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || !m_pBagEquip || m_bEquipDescription)
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        CCString *str = NULL;
        int nCount = m_pBagValidEquipOnBodyCount;
        if (m_pBagCell && m_pBagCell->getTag() < nCount) //已装备
        {
            str = CCString::createWithFormat("%d",m_pBagEquip->getPartIndex());
        }
        else
        {
            str = CCString::createWithFormat("%s",m_pBagEquip->getID().c_str());
        }
        CCString *str1 = CCString::createWithFormat(ASK_COMPOSE_MONEY,str->getCString());
        Controller::getInstance()->sendMessage(str1->getCString(),true);
    }
}

void PopUpUI::showComposeFailedTips(int tipType)
{
    switch (tipType)
    {
        case 0:
            ((YYScene*)getParent())->showTips(g_sComposeType);
            break;
        case 1:
            ((YYScene*)getParent())->showTips(g_sComposeMiss);
            break;
        case 2:
            ((YYScene*)getParent())->showTips(g_sCanNotCompose);
            break;
        case 3:
            ((YYScene*)getParent())->showTips(g_sMissingMaterialForCompose);
            break;
        case 4:
            ((YYScene*)getParent())->showTips(g_sLackJewForCompose);
            break;
        default:
            break;
    }
}
/**
 * 还原锻造界面
 */
void PopUpUI::resetComposeInterface()
{
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIPanel* composePanel = static_cast<UIPanel*>(panel0->getChildByName("Panel_c"));
    UIPanel* rightPanel = static_cast<UIPanel*>(composePanel->getChildByName("Panel2_1"));
    UIPanel* materialPanel = static_cast<UIPanel*>(composePanel->getChildByName("Panel2_2"));
    
    for (int i = 0; i < 6; i ++)
    {
        ImageView *icon = static_cast<ImageView *>(materialPanel->getChildByName(CCString::createWithFormat("ImageView%d",i)->getCString()));
        icon->setVisible(true);
        for (int j = 0; j < icon->getChildrenCount(); j++)
        {
            ImageView *imageView = dynamic_cast<ImageView *>(icon->getChildren()->objectAtIndex(j));
            if (imageView)
            {
                icon->removeChild(imageView,true);
            }
        }
        Label * sign = static_cast<Label*>(icon->getChildByName("Label_1"));
        sign->setVisible(false);
        Label * numLabel1 = static_cast<Label*>(icon->getChildByName("Label_2"));
        numLabel1->setVisible(false);
        Label * numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_1"));
        numLabel2->setVisible(false);
        numLabel2 = static_cast<Label*>(icon->getChildByName("Label_0_0"));
        numLabel2->setVisible(false);
    }
    
    //删除装备图标
    ImageView *equipImgView = dynamic_cast<ImageView *>(rightPanel->getChildByName("ImageView0"));
    equipImgView->removeChildByTag(TAG_ICON_BG);
    equipImgView->removeChildByTag(TAG_ITEM_ICON);
    
    //删除预览图标
    UIImageView *preViewImgView = dynamic_cast<UIImageView *>(rightPanel->getChildByName("ImageView1"));
    ImageView *preEquipImageView = dynamic_cast<UIImageView *>(preViewImgView->getChildByTag(20));
    if (preEquipImageView)
    {
        preViewImgView->removeChild(preEquipImageView,true);
    }
    
    // 还原 合成槽位置
    m_pReplacedCell = NULL;
    m_pBagCell = NULL;
    m_pReplacedEquip = NULL;
    m_pBagEquip = NULL;
}

#pragma mark 强化 锻造 详细信息回调

//
void PopUpUI::reinForceEquipCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    UIImageView* imgageView = static_cast<ImageView *>(pSender);
    
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (imgageView->getChildrenCount() > 1)
        {
            if (m_pBagCell && m_pBagCell->getTag() < m_pBagValidEquipOnBodyCount)
            {
                 showEquipAttributeInterface(m_pBagEquip,true);  //已装备
            }
            else
            {
                 showEquipAttributeInterface(m_pBagEquip,false);   //未装备
            }
        }
    }
}

void PopUpUI::reinForceMeterialCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    UIImageView* imgageView = static_cast<ImageView *>(pSender);
    
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (imgageView->getChildrenCount() > 4)
        {
            CCString* str = CCString::createWithFormat(ASK_MATERIAL_INFO,static_cast<CCString *>(m_pArrMeterialItem->objectAtIndex(imgageView->getTag()))->getCString());
            Controller::getInstance()->sendMessage(str->getCString(),true);
        }
    }
}

void PopUpUI::composeEquipCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    UIImageView* imgageView = static_cast<ImageView *>(pSender);
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (imgageView->getChildrenCount() > 1)
        {
            if (m_pBagCell && m_pBagCell->getTag() < m_pBagValidEquipOnBodyCount)
            {
                showEquipAttributeInterface(m_pBagEquip,true);  //已装备
            }
            else
            {
                showEquipAttributeInterface(m_pBagEquip,false);   //未装备
            }
        }
    }
}


void PopUpUI::composeAfterEquipCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    UIImageView* imgageView = static_cast<ImageView *>(pSender);
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (imgageView->getChildrenCount() > 0)
        {
            const char *msg = CCString::createWithFormat(ASK_MATERIAL_INFO2,m_pBagEquip->getIsCompose())->getCString();
            Controller::getInstance()->sendMessage(msg,true);
        }
    }
}


void PopUpUI::composeMeterialCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading() || m_bReadingTreasureData || m_bTreasureOpened || m_bEquipDescription)
    {
        return;
    }
    UIImageView* imgageView = static_cast<ImageView *>(pSender);
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (imgageView->getChildrenCount() > 4)
        {
            const char *msg = CCString::createWithFormat(ASK_MATERIAL_INFO,static_cast<CCString *>(m_pArrMeterialItem->objectAtIndex(imgageView->getTag()))->getCString())->getCString();
            Controller::getInstance()->sendMessage(msg,true);
        }
    }
}

void PopUpUI::updateEquipDetailInfo(cocos2d::CCArray * array)
{
    if(!array)
        return;
    
    CCString *baseInfo = dynamic_cast<CCString *>(array->objectAtIndex(0));
    CCArray *baseInfoArray = split_string(baseInfo->getCString(), ',');
    int index = 0;
    Equipment *equip = Equipment::create();
    
    // 编号(排序用)
    int equipIndex = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
    equip->setIndex(equipIndex);
    
    int curLevel = equipIndex % 100;
    equip->setLevel(curLevel);
    
    //body part
    int part = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
    equip->setPartIndex(part);
    
    //icon path
    std::string iconPath = getFilePath(dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->getCString(), kDirTypeIcon, kFileSuffixPng);
    equip->setIconPath(iconPath.c_str());
    
    //quality
    int quality = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
    equip->setQuality(quality);
    
    // 锻造需求等级
    int level = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
    equip->setComposeLevel(level);
    
    // 最大可强化等级
    int maxLevel = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
    equip->setMaxLevel(maxLevel);
    
    CCString *detail0 = dynamic_cast<CCString *>(array->objectAtIndex(1));
    CCArray *arr0 = split_string(detail0->getCString(), ';');
    if (arr0)
    {
        int i = 0;
        const char * name = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
        equip->setEquipName(name);
        const char * type = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
        equip->setType(type);
        int pro = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
        equip->setProfessionIndex(pro);
        int level = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
        equip->setPlayerLevel(level);
        int combat = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
        equip->setCombat(combat);
        int nIsCompose = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
        equip->setIsCompose(nIsCompose);
        const char * title = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
        equip->setTitle(title);
    }
    
    CCString *detail1 = dynamic_cast<CCString *>(array->objectAtIndex(2));
    CCArray *arr1 = split_string(detail1->getCString(), ';');
    if (arr1)
    {
        int i = 0;
        int point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
        equip->setStrenghPoint(point);
        point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
        equip->setAgilityPoint(point);
        point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
        equip->setIntelligencePoint(point);
        point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
        equip->setEndurancePoint(point);
    }
    CCString *detail2 = dynamic_cast<CCString *>(array->objectAtIndex(3));
    CCArray *arr2 = split_string(detail2->getCString(), ';');
    if (arr2)
    {
        int i = 0;
        int point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setAP(point);
        point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setMAP(point);
        point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setSAP(point);
        point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setDP(point);
        point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setHP(point);
        point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setMP(point);
        point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setHitOdds(point);
        point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
        equip->setCriticalOdds(point);
    }
    CCString *detail3 = dynamic_cast<CCString *>(array->objectAtIndex(4));
    equip->setSalePrice(detail3->intValue());
    CCString *detail4 = dynamic_cast<CCString *>(array->objectAtIndex(5));
    equip->setDescription(detail4->getCString());
    showEquipAttributeInterface(equip,false);  //未装备
}

void PopUpUI::updateItemDetailInfo(cocos2d::CCArray * array)
{
    if(!array)
        return;
    int index = 0;
    Property *prop = Property::create();
    
    // 图标
    const char * iconName = dynamic_cast<CCString *>(array->objectAtIndex(index++))->getCString();
    std::string path = getFilePath(iconName, kDirTypeIcon, kFileSuffixPng);
    prop->setIconPath(path.c_str());
    // 品质
    int quality = dynamic_cast<CCString *>(array->objectAtIndex(index++))->intValue();
    prop->setQuality(quality);
    // 数量
    int num = dynamic_cast<CCString *>(array->objectAtIndex(index++))->intValue();
    prop->setNumber(num);
    // 编号
    int propIndex = dynamic_cast<CCString *>(array->objectAtIndex(index++))->intValue();
    prop->setIndex(propIndex);
    // 名称
    CCString * tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setName(tempstr->getCString());
    // 类型标签
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setLabel(tempstr->getCString());
    // 职业
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setProfessionIndex(tempstr->intValue());
    // 是否绑定
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setBind(tempstr->intValue());
    // 是否唯一
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setUnique(tempstr->intValue());
    // 等级
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setLevel(tempstr->intValue());
    // 描述
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setDescription(tempstr->getCString());
    // 售价
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setSalePrice(tempstr->intValue());
    // 限时
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setTimeLimit(tempstr->intValue());
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index));
    
    if (getFirstNBitNumber(prop->getIndex(),3) == 501 && tempstr->intValue() == 1)
    {
        prop->setOpened(true);
    }
    
    showPropAttribute(prop,false);
}

#pragma mark 金币宝石更新

void PopUpUI::updateBagUIPropertyInfo()
{
    UILabel* money_Text = NULL;
    UILabel* jewel_Text = NULL;
    if (strcmp(m_pBagPageButton->getName(), "Button0") == 0) //装备
    {
        UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        UIPanel* panela = static_cast<UIPanel*>(panel0->getChildByName("Panel_a"));
        UIPanel* panel2_2 = static_cast<UIPanel*>(panela->getChildByName("Panel2_2"));
        UILabel* money_Label = static_cast<UILabel*>(panel2_2->getChildByName("TextArea0"));
        money_Text = static_cast<UILabel*>(money_Label->getChildByName("TextArea00"));
        UILabel* jewel_Label = static_cast<UILabel*>(panel2_2->getChildByName("TextArea1"));
        jewel_Text = static_cast<UILabel*>(jewel_Label->getChildByName("TextArea10"));
    }
    else if (strcmp(m_pBagPageButton->getName(), "Button1") == 0) //强化
    {
        UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        UIPanel* panelb = static_cast<UIPanel*>(panel0->getChildByName("Panel_b"));
        UIPanel* panel2_4 = static_cast<UIPanel*>(panelb->getChildByName("Panel2_4"));
        UIImageView* money_ImgView = static_cast<UIImageView*>(panel2_4->getChildByName("ImageView_0"));
        money_Text = static_cast<UILabel*>(money_ImgView->getChildByName("TextArea00"));
        UIImageView* jewel_ImgView = static_cast<UIImageView*>(panel2_4->getChildByName("ImageView_1"));
        jewel_Text = static_cast<UILabel*>(jewel_ImgView->getChildByName("TextArea00"));
    }
    else if (strcmp(m_pBagPageButton->getName(), "Button2") == 0) //锻造
    {
        UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        UIPanel* panelc = static_cast<UIPanel*>(panel0->getChildByName("Panel_c"));
        UIPanel* panel2_3 = static_cast<UIPanel*>(panelc->getChildByName("Panel2_3"));
        UIImageView* money_ImgView = static_cast<UIImageView*>(panel2_3->getChildByName("ImageView_0"));
        money_Text = static_cast<UILabel*>(money_ImgView->getChildByName("TextArea00"));
        UIImageView* jewel_ImgView = static_cast<UIImageView*>(panel2_3->getChildByName("ImageView_1"));
        jewel_Text = static_cast<UILabel*>(jewel_ImgView->getChildByName("TextArea00"));
    }
    
    money_Text->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getGold())->getCString());
    int color = 0xded3bd;
    ccColor3B color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
    money_Text->setColor(color3B);
    jewel_Text->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel())->getCString());
    color = 0xffbc2d;
    color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
    jewel_Text->setColor(color3B);
}

#pragma mark 设置属性页面主角信息

void PopUpUI::setRoleLabel(UIPanel* panel,bool bArrOrBag)
{
    UILabel* professtion_Text = NULL;
    UILabel* level_Text = NULL;
    UILabel* name_Text = NULL;
    
    if (bArrOrBag)    // 角色属性
    {
        professtion_Text = static_cast<UILabel*>(panel->getChildByName("TextArea0_0"));
        level_Text = static_cast<UILabel*>(panel->getChildByName("TextArea1_0"));
        name_Text = static_cast<UILabel*>(panel->getChildByName("TextArea2_0"));
    }
    else             // 背包中角色属性
    {
        professtion_Text = static_cast<UILabel*>(panel->getChildByName("TextArea0"));
        level_Text = static_cast<UILabel*>(panel->getChildByName("TextArea1"));
        name_Text = static_cast<UILabel*>(panel->getChildByName("TextArea2"));
    }
    
    // 名字
    name_Text->setText(m_pRoleByControlled->getSpriteName()->getCString());
    int color = 0xf3e8d9;
    ccColor3B color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
    name_Text->setColor(color3B);
    // 职业
    int professtionIndex = m_pRoleByControlled->getSpriteData()->getSpriteIndex();
    professtion_Text->setText(g_sProfessionStr[professtionIndex - 1]);
    color = 0xfff600;
    color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
    name_Text->setColor(color3B);
    // 等级
    int currentLevel = m_pRoleByControlled->getLevel();
    const char *buffer = CCString::createWithFormat(g_sStageFmt,currentLevel)->getCString();
    level_Text->setText(buffer);
    color = 0xa7180c;
    color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
    level_Text->setColor(color3B);
    
    YYSprite * player = dynamic_cast<YYSprite *>(getChildByTag(TAG_UI_PLAYER));
    if(!player)
    {
        player = (YYPlayer *)YYSprite::copy(m_pRoleByControlled);
        player->setAction(ACTION_STAND0, true);
        player->setPosition(ccp(m_cAdjustScreenPosition.x + panel->getPositionX() + panel->getContentSize().width / 2,m_cAdjustScreenPosition.y + name_Text->getPosition().y - 250));
        player->setTag(TAG_UI_PLAYER);
        addChild(player, 1);
    }
}

#pragma mark 通过装备部位标签获取装备

Equipment * PopUpUI::getEquipOnTheBodyByPart(int part)
{
    CCArray *array = GlobalData::getInstance()->getEquipOnTheBodyArray();
    for (int i = 0; array->count(); i++)
    {
        Equipment * equip = static_cast<Equipment *>(array->objectAtIndex(i));
        if (equip && equip->getPartIndex() == part)
        {
            return equip;
        }
    }
    return NULL;
}

#pragma mark 设置物品属性

void PopUpUI::showPropAttribute(Property * prop, bool bShowBtn)
{
    m_bEquipDescription = true;
    TouchGroup *touchGroup = static_cast<TouchGroup *>(getParent()->getChildByTag(TAG_POPUP_WINDOW));
    Layout *panel03 = static_cast<Layout *>(touchGroup->getWidgetByName("Panel03"));
    panel03->setEnabled(true);
    panel03->setVisible(true);
    ImageView *imageView_0 = static_cast<ImageView *>(panel03->getChildByName("ImageView_0"));
    imageView_0->setEnabled(false);
    ImageView *imageView_1 = static_cast<ImageView *>(panel03->getChildByName("ImageView_1"));
    imageView_1->setEnabled(false);
    ImageView *imageView_2 = static_cast<ImageView *>(panel03->getChildByName("ImageView_2"));
    imageView_2->setEnabled(true);
    imageView_2->setVisible(true);
    // 名称
    Layout *title = static_cast<Layout *>(imageView_2->getChildByName("Panel0"));
    Label * label = static_cast<Label *>(title->getChildByName("Label_name"));
    label->setText(prop->getName().c_str());
    // 图标
    Layout *panel1 = static_cast<Layout *>(imageView_2->getChildByName("Panel1"));
    ImageView *icon = static_cast<ImageView *>(panel1->getChildByName("ImageView_icon"));
    icon->removeAllChildren();
    ImageView *icon1 = ImageView::create();
    icon1->loadTexture(prop->getIconPath().c_str());
    icon->addChild(icon1);
    // 是否绑定
    Label * label_0 = static_cast<Label *>(panel1->getChildByName("Label_0"));
    Label * bindLabel = static_cast<Label *>(label_0->getChildByName("Label_0"));
    bindLabel->setText(prop->isBind()?g_sYes:g_sNo);
    // 描述
    Layout *panel2 = static_cast<Layout *>(imageView_2->getChildByName("Panel2"));
    label_0 = static_cast<Label *>(panel2->getChildByName("Label_0"));
    label_0->setText(prop->getDescription().c_str());
    // 价格
    Layout *panel3 = static_cast<Layout *>(imageView_2->getChildByName("Panel3"));
    label_0 = static_cast<Label *>(panel3->getChildByName("Label_0"));
    Label * priceLabel = static_cast<Label *>(label_0->getChildByName("Label_0"));
    priceLabel->setText(CCString::createWithFormat("%d",prop->getSalePrice())->getCString());
    Layout *panel4 = static_cast<Layout *>(imageView_2->getChildByName("Panel4"));
    Button *button0 = static_cast<Button *>(panel4->getChildByName("Button_0"));
    Button *button1 = static_cast<Button *>(panel4->getChildByName("Button_1"));
    Button *button2 = static_cast<Button *>(panel4->getChildByName("Button_2"));
    button2->setTag(kPopUpUICloseButton);
    button2->addTouchEventListener(this, toucheventselector(PopUpUI::hideItemInfo));
    if (bShowBtn)
    {
        button0->setBright(true);
        button0->setEnabled(true);
        button0->setTag(kPopUpUISaleButton);
        button0->addTouchEventListener(this, toucheventselector(PopUpUI::saleItemCallBack));
        
        if(prop->getLevel() == 0)
        {
            button1->setBright(false);
            button1->setTouchEnabled(false);
        }
        else
        {
            button1->setBright(true);
            button1->setEnabled(true);
            button1->setTouchEnabled(true);
            button1->setTag(kPopUpUIUseButton);
            button1->addTouchEventListener(this, toucheventselector(PopUpUI::useProp));
        }
    }
    else
    {
        button0->setBright(false);
        button0->setEnabled(false);
        button1->setBright(false);
        button1->setEnabled(false);
    }
}

#pragma mark 使用物品

void PopUpUI::useProp(CCObject *pSender, cocos2d::gui::TouchEventType type)
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
        if (m_pBagCell)
        {
            if (m_pBagCell->getTag() >= GlobalData::getInstance()->getPropArray()->count())
            {
                CCLOG("m_pBagCell->getTag() out of range");
                return;
            }
            Property *prop = static_cast<Property *>(GlobalData::getInstance()->getPropArray()->objectAtIndex(m_pBagCell->getTag()));
            if (prop)
            {
                const char *str = NULL;
                int index = getFirstNBitNumber(prop->getIndex(),3);
                if (index == 501)
                {
                    str = CCString::createWithFormat(ASK_OPEN_TREASURE,prop->getID().c_str())->getCString();
                    addTreasureOpeningAction();
                    Controller::getInstance()->sendMessage(str,false);
                }
                else
                {
                    str = CCString::createWithFormat(ASK_USE_PROP,prop->getID().c_str())->getCString();
                    Controller::getInstance()->sendMessage(str,true);
                }
                hideItemPopUpInfo();
            }
            else
            {
                CCLOG("prop == NULL");
            }
        }
        else
        {
            CCLOG("m_pBagCell == NULL");
        }
    }
}

#pragma mark 显示装备属性

void PopUpUI::showEquipAttributeInterface(Equipment* equipment,bool isEqupOnBody)
{
    m_bEquipDescription = true;
    TouchGroup *touchGroup = static_cast<TouchGroup *>(getParent()->getChildByTag(TAG_POPUP_WINDOW));
    Layout *panel03 = static_cast<Layout *>(touchGroup->getWidgetByName("Panel03"));
    panel03->setEnabled(true);
    panel03->setVisible(true);
    
    if (!isEqupOnBody) //未装备
    {
        ImageView *imageView_0 = static_cast<ImageView *>(panel03->getChildByName("ImageView_0"));
        imageView_0->setEnabled(true);
        imageView_0->setVisible(true);
        ImageView *imageView_2 = static_cast<ImageView *>(panel03->getChildByName("ImageView_2"));
        imageView_2->setEnabled(false);
        imageView_2->setVisible(false);
        setEquipAttribute(equipment,imageView_0);
        Layout *panel6 = static_cast<Layout *>(imageView_0->getChildByName("Panel6"));
        Button *button0 = static_cast<Button *>(panel6->getChildByName("Button_0"));
        Button *button1 = static_cast<Button *>(panel6->getChildByName("Button_1"));
        if (!m_bTreasureOpened && strcmp(m_pBagPageButton->getName(), "Button0") == 0)
        {
            button0->setTag(kPopUpUISaleButton);
            button1->setTag(kPopUpUIPutOnButton);
            button0->addTouchEventListener(this, toucheventselector(PopUpUI::saleItemCallBack));
            button1->addTouchEventListener(this, toucheventselector(PopUpUI::putOnEquipCallBack));
            button0->setBright(true);
            button0->setTouchEnabled(true);
            button1->setBright(true);
            button1->setTouchEnabled(true);
        }
        else
        {
            button0->setBright(false);// 按键调用禁用状态图片
            button0->setTouchEnabled(false);
            button1->setBright(false);// 按键调用禁用状态图片
            button1->setTouchEnabled(false);
        }
    }
    else  //已装备
    {
        ImageView *imageView_1 = static_cast<ImageView *>(panel03->getChildByName("ImageView_1"));
        imageView_1->setEnabled(true);
        imageView_1->setVisible(true);
        setEquipAttribute(equipment,imageView_1);
    }
}


#pragma mark 设置装备属性

void PopUpUI::setEquipAttribute(Equipment* equipment,UIWidget *widget)
{
    // 名称
    Layout *title = static_cast<Layout *>(widget->getChildByName("Panel0"));
    Label * label = static_cast<Label *>(title->getChildByName("Label_name"));
    label->setText(equipment->getEquipName().c_str());
    setTextColorByQuality(label, equipment->getQuality());
    
    Label * level_0 = static_cast<Label *>(label->getChildByName("Label_0"));
    Label * levelLabel = static_cast<Label *>(label->getChildByName("Label_lvl"));
    if (equipment->getLevel() > 0)
    {
        level_0->setVisible(true);
        levelLabel->setVisible(true);
        levelLabel->setText(CCString::createWithFormat("%d",equipment->getLevel())->getCString());
    }
    else
    {
        level_0->setVisible(false);
        levelLabel->setVisible(false);
    }
    label = static_cast<Label *>(title->getChildByName("Label_num"));
    Label *combatLabel = static_cast<Label *>(label->getChildByName("Label_0"));
    combatLabel->setText(CCString::createWithFormat("%d",equipment->getCombat())->getCString());
    
    // 图标
    Layout *panel1 = static_cast<Layout *>(widget->getChildByName("Panel1"));
    ImageView *icon = static_cast<ImageView *>(panel1->getChildByName("ImageView_icon"));
    icon->removeAllChildren();
    updateItemIcon(icon,equipment->getIconPath().c_str(),equipment->getQuality());
    // 部位
    Label *label_0 = static_cast<Label *>(panel1->getChildByName("Label_0"));
    Label *value = static_cast<Label *>(label_0->getChildByName("Label_0"));
    value->setText(equipment->getType());
    // 职业
    Label *label_1 = static_cast<Label *>(panel1->getChildByName("Label_1"));
    value = static_cast<Label *>(label_1->getChildByName("Label_0"));
    value->setText(g_sProfessionStr[equipment->getProfessionIndex()-1]);
    // 品质
    Label *label_2 = static_cast<Label *>(panel1->getChildByName("Label_2"));
    value = static_cast<Label *>(label_2->getChildByName("Label_0"));
    value->setText(g_Quality[equipment->getQuality()-1]);
    setTextColorByQuality(value, equipment->getQuality());
    // 需求等级
    Label *label_3 = static_cast<Label *>(panel1->getChildByName("Label_3"));
    value = static_cast<Label *>(label_3->getChildByName("Label_0"));
    int nComposeLv = equipment->getComposeLevel();  //锻造装备所需等级
    int nNeedPlayerLv = equipment->getPlayerLevel();   //需求玩家等级
    if (strcmp(m_pBagPageButton->getName(), "Button2") == 0) //锻造页面
    {
        value->setText((CCString::createWithFormat("%d",nComposeLv))->getCString());
    }
    else
    {
        value->setText((CCString::createWithFormat("%d",nNeedPlayerLv))->getCString());
    }
    // 属性
    Layout *panel2 = static_cast<Layout *>(widget->getChildByName("Panel2"));
    
    int arrProPerty[8] = { equipment->getStrenghPoint(),   // 力量
                           equipment->getAgilityPoint(),   // 敏捷 
                           equipment->getIntelligencePoint(), // 智力
                           equipment->getEndurancePoint(), // 耐力
                           equipment->getAP(), // 物理攻击
                           equipment->getMAP(), // 魔法攻击
                           equipment->getSAP(), // 绝技攻击
                           equipment->getDP()// 防御
                          };
    int k = 0;
    for (int i = 0; i < 8; i++)
    {
        if (arrProPerty[i] > 0)
        {
            Label * strLabel = static_cast<Label *>(panel2->getChildByName(CCString::createWithFormat("Label_%d",k)->getCString()));
            strLabel->setText(g_sEquipPropertyLabel[i]);
            value = static_cast<Label *>(strLabel->getChildByName("Label_0"));
            value->setText(CCString::createWithFormat("%d",arrProPerty[i])->getCString());
            k++;
            if (k >= 2)
            {
                break;
            }
        }
    }
  
    // 装备描述
    Layout *panel3 = static_cast<Layout *>(widget->getChildByName("Panel3"));
    label_0 = static_cast<Label *>(panel3->getChildByName("Label_0"));
    label_0->setText(equipment->getDescription().c_str());
    
    // 价格
    Layout *panel5 = static_cast<Layout *>(widget->getChildByName("Panel5"));
    label_0 = static_cast<Label *>(panel5->getChildByName("Label_0"));
    Label * priceLabel = static_cast<Label *>(label_0->getChildByName("Label_0"));
    priceLabel->setText(CCString::createWithFormat("%d",equipment->getSalePrice())->getCString());
    
    // 关闭装备信息
    Layout *panel6 = static_cast<Layout *>(widget->getChildByName("Panel6"));
    Button *button2 = static_cast<Button *>(panel6->getChildByName("Button_2"));
    button2->setTag(kPopUpUICloseButton);
    button2->addTouchEventListener(this, toucheventselector(PopUpUI::hideItemInfo));
}

#pragma mark 隐藏物品装备信息

void PopUpUI::hideItemInfo(CCObject *pSender, cocos2d::gui::TouchEventType type)
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
        hideItemPopUpInfo();
    }
}

#pragma mark 隐藏物品装备弹出信息

void PopUpUI::hideItemPopUpInfo()
{
    TouchGroup *touchGroup = static_cast<TouchGroup *>(getParent()->getChildByTag(TAG_POPUP_WINDOW));
    Layout *panel03 = static_cast<Layout *>(touchGroup->getWidgetByName("Panel03"));
    panel03->setEnabled(false);
    panel03->setVisible(false);
    
    ImageView *imageView_2 = static_cast<ImageView *>(panel03->getChildByName("ImageView_2"));
    imageView_2->setEnabled(false);
    imageView_2->setVisible(false);
    ImageView *imageView_0 = static_cast<ImageView *>(panel03->getChildByName("ImageView_0"));
    imageView_0->setEnabled(false);
    imageView_0->setVisible(false);
    ImageView *imageView_1 = static_cast<ImageView *>(panel03->getChildByName("ImageView_1"));
    imageView_1->setEnabled(false);
    imageView_1->setVisible(false);
    m_bEquipDescription = false;
}

#pragma mark 出售物品装备

void PopUpUI::saleItemCallBack(CCObject *sender ,cocos2d::gui::TouchEventType type)
{
    if (Controller::getInstance()->getScene()->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(sender, type))
        {
            return;
        }
        if(strcmp(m_pItemTypeButton->getName(), "Button0") == 0)
        {// 物品
            if (m_pBagCell->getTag() >=  GlobalData::getInstance()->getPropArray()->count())
            {
                return;
            }
            Property *prop = static_cast<Property *>(GlobalData::getInstance()->getPropArray()->objectAtIndex(m_pBagCell->getTag()));
            if (prop)
            {
                const char *msg = CCString::createWithFormat(ASK_SALE_PROP,prop->getID().c_str())->getCString();
                Controller::getInstance()->sendMessage(msg,true);
                hideItemPopUpInfo();
            }
        }
        else
        {// 装备
            if (m_pBagCell->getTag() >=  GlobalData::getInstance()->getEquipArray()->count())
            {
                return;
            }
            Equipment *equip = static_cast<Equipment *>(GlobalData::getInstance()->getEquipArray()->objectAtIndex(m_pBagCell->getTag()));
            if (equip)
            {
                const char *msg = CCString::createWithFormat(ASK_SALE_EQUIP,equip->getID().c_str())->getCString();
                Controller::getInstance()->sendMessage(msg,true);
                hideItemPopUpInfo();
            }
        }
    }
}

#pragma mark 物品消耗及装备删除

void PopUpUI::updateItemIconInBag(bool isEquip)
{
    if(m_pBagPageButton && strcmp(m_pBagPageButton->getName(),"Button0") == 0)
    {
        Layout *panel0 = static_cast<Layout *>(m_pUITouchGroup->getWidgetByName("Panel0"));
        Layout * panel_a = static_cast<Layout *>(panel0->getChildByName("Panel_a"));
        Layout * panel2_3 = static_cast<Layout*>(panel_a->getChildByName("Panel2_3"));
        ScrollView * scrollView = static_cast<ScrollView*>(panel2_3->getChildByName("ScrollView0"));
        if (isEquip)
        {
            if(m_pItemTypeButton && strcmp(m_pItemTypeButton->getName(),"Button1") == 0) //装备
            {
                setItemInBag(scrollView, GlobalData::getInstance()->getEquipArray());
            }
        }
        else
        {
            if(m_pItemTypeButton && strcmp(m_pItemTypeButton->getName(),"Button0") == 0) //物品
            {
                setItemInBag(scrollView, GlobalData::getInstance()->getPropArray());
            }
        }
    }
}

#pragma mark 宝箱
/**
 * 初始化宝箱
 */
void PopUpUI::initTreasureContainer()
{
    m_u16RefreshAmount = 0;
    m_u16RefreshCount = 0;
    m_bTreasureOpened = false;
    m_bReadingTreasureData = false;
    UIPanel* panel_Treasure = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel02"));
    panel_Treasure->setEnabled(false);
    panel_Treasure->setZOrder(100);
    UIButton* button0 = static_cast<UIButton*>(panel_Treasure->getChildByName("Button0_0"));
    button0->setTag(kTreasureUIRefreshButton);
    button0->addTouchEventListener(this, toucheventselector(PopUpUI::treasureButtonCallBack));
    UIButton* button1 = static_cast<UIButton*>(panel_Treasure->getChildByName("Button1_0"));
    button1->setTag(kTreasureUIConfirmButton);
    button1->addTouchEventListener(this, toucheventselector(PopUpUI::treasureButtonCallBack));
    UIButton* button2 = static_cast<UIButton*>(panel_Treasure->getChildByName("Button2_0"));
    button2->setTag(kTreasureUIConfirmButton);
    button2->addTouchEventListener(this, toucheventselector(PopUpUI::treasureButtonCallBack));
    
}

/**
 * 设置宝物数据
 */
void PopUpUI::setTreasureData(cocos2d::CCArray * array)
{
    if (array)
    {
        int index = 0;
        m_sTreasureID = static_cast<CCString *>(array->objectAtIndex(index++))->getCString();
        
        if (array->count() > index)
        {
            m_u16RefreshCount = static_cast<CCString *>(array->objectAtIndex(index++))->intValue();
        }
        if (array->count() > index)
        {
            m_u16RefreshAmount = static_cast<CCString *>(array->objectAtIndex(index++))->intValue();
        }
        if (array->count() > index)
        {
            m_u16RefreshConsume = static_cast<CCString *>(array->objectAtIndex(index++))->intValue();
        }
    }
}

void PopUpUI::showTreasureInterface(bool isRefresh)
{
    UIPanel* panel_Treasure = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel02"));
    panel_Treasure->setEnabled(true);
    panel_Treasure->setVisible(true);
    UILabel* text_Name = static_cast<UILabel*>(panel_Treasure->getChildByName("TextArea0_0"));
    // 根据宝箱ID获得宝箱名称
    Property *treasure = static_cast<Property *>(GlobalData::getInstance()->getPropertyDict()->objectForKey(m_sTreasureID));
    text_Name->setText(treasure->getName().c_str());
    UIButton* button0 = static_cast<UIButton*>(panel_Treasure->getChildByName("Button0_0"));
    UIButton* button1 = static_cast<UIButton*>(panel_Treasure->getChildByName("Button1_0"));
    UIButton* button2 = static_cast<UIButton*>(panel_Treasure->getChildByName("Button2_0"));
    UILabel* text = static_cast<UILabel*>(panel_Treasure->getChildByName("TextArea1_0"));
    if(isRefresh) // 两种表现形式(可刷新与不可刷新)
    {
        button0->setVisible(true);
        button0->setTouchEnabled(true);
        button1->setVisible(true);
        button1->setTouchEnabled(true);
        button2->setEnabled(false);
        button2->setVisible(false);
        button2->setTouchEnabled(false);
        text->setVisible(true);
    }
    else
    {
        button0->setVisible(false);
        button0->setTouchEnabled(false);
        button1->setVisible(false);
        button1->setTouchEnabled(false);
        button2->setEnabled(true);
        button2->setVisible(true);
        button2->setTouchEnabled(true);
        text->setVisible(false);
    }
    Label * textArea1 = static_cast<UILabel*>(panel_Treasure->getChildByName("TextArea1_0"));
    UILabel* text_CurrentCount = static_cast<UILabel*>(textArea1->getChildByName("TextArea10_0"));
    UILabel* text_MaxCount = static_cast<UILabel*>(textArea1->getChildByName("TextArea12_0"));
    text_MaxCount->setText(CCString::createWithFormat("%d",m_u16RefreshAmount)->getCString());
    text_CurrentCount->setText(CCString::createWithFormat("%d",m_u16RefreshCount)->getCString());
    
    UIButton* button_Refresh = static_cast<UIButton*>(panel_Treasure->getChildByName("Button0_0"));
    UILabel* text_Refresh = static_cast<UILabel*>(button_Refresh->getChildByName("TextArea_0"));
    if(m_u16RefreshAmount > 0) // 可刷新次数显示
    {
        text_Refresh->setText(CCString::createWithFormat("%d%s",m_u16RefreshConsume,g_sRefreshable)->getCString());
    }
    else
    {
        text_Refresh->setText(CCString::createWithFormat("%s",g_sNotRefreshable)->getCString());
    }
    
    UIPanel* panel_Widget = static_cast<UIPanel*>(panel_Treasure->getChildByName("Panel1_0"));
    CCArray *arr = GlobalData::getInstance()->getTreasureItemArray();
    for(int i = 0;i < arr->count(); i++)
    {
        UIImageView* imageView = static_cast<UIImageView*>(panel_Widget->getChildByName(CCString::createWithFormat("ImageView%d_0",i)->getCString()));
        imageView->setTouchEnabled(true);
        imageView->addTouchEventListener(this, toucheventselector(PopUpUI::treasureCellCallBack));
        Label *label_0 = static_cast<Label *>(imageView->getChildByName("Label_0_0"));
        CCObject *object = arr->objectAtIndex(i);
        ImageView * icon = ImageView::create();
        if (dynamic_cast<Property *>(object))
        {
            icon->loadTexture(dynamic_cast<Property *>(object)->getIconPath().c_str());
            label_0->setVisible(true);
            label_0->setText(CCString::createWithFormat("%d",dynamic_cast<Property *>(object)->getNumber())->getCString());
        }
        else if(dynamic_cast<Equipment *>(object))
        {
            icon->loadTexture(dynamic_cast<Equipment *>(object)->getIconPath().c_str());
            label_0->setVisible(false);
        }
        imageView->addChild(icon);
    }
}

void PopUpUI::treasureCellCallBack(CCObject *object, TouchEventType type)
{
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (isTouchUnable(object, type))
        {
            return;
        }
        CCArray *arr = GlobalData::getInstance()->getTreasureItemArray();
        for (int i = 0; i < arr->count(); i++)
        {
            if (strcmp(static_cast<ImageView *>(object)->getName(),CCString::createWithFormat("ImageView%d_0",i)->getCString()) == 0)
            {
                CCObject *object = arr->objectAtIndex(i);
                if (dynamic_cast<Property *>(object))
                {
                    showPropAttribute(dynamic_cast<Property *>(object),false);
                }
                else if(dynamic_cast<Equipment *>(object))
                {
                    showEquipAttributeInterface(dynamic_cast<Equipment *>(object),false);
                }
                break;
            }
        }
    }
}

void PopUpUI::treasureButtonCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIButton* widget = static_cast<UIButton*>(pSender);
        if(widget->getTag() == kTreasureUIRefreshButton)
        {
            if(m_u16RefreshConsume > m_pRoleByControlled->getSpriteAttribute()->getJewel())
            {
                ((YYScene*)getParent())->showTips(g_sLackJewel);
            }
            else
            {
                const char* message = CCString::createWithFormat(ASK_REFRESH_TREASURE,m_sTreasureID.c_str())->getCString();
                Controller::getInstance()->sendMessage(message,false);
                closeTreasureContainer();
                addTreasureOpeningAction();
            }
        }
        else if (widget->getTag() == kTreasureUIConfirmButton)
        {
            const char* message = CCString::createWithFormat(ASK_GET_ITEM,m_sTreasureID.c_str())->getCString();
            Controller::getInstance()->sendMessage(message,false);
            closeTreasureContainer();
        }
    }
}

void PopUpUI::updateTreasureData()
{
    YYSprite *sprite = static_cast<YYSprite*>(getChildByTag(TAG_BAG_TREASURE));
    if (sprite)
    {
        sprite->update();
        if(!m_bReadingTreasureData)
        {
            if (m_bTreasureOpened)
            {
                if( sprite->isActionOver())
                {
                    showTreasureInterface(true);
                    removeChildByTag(TAG_BAG_TREASURE);
                }
            }
            else
            {
                removeChildByTag(TAG_BAG_TREASURE);
            }
        }
    }
}

void PopUpUI::addTreasureOpeningAction()
{
    m_bReadingTreasureData = true;
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    YYSprite *sprite = YYSprite::createWithActionSet("362", "362", kDirTypeEffect);
    sprite->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    sprite->setTag(TAG_BAG_TREASURE);
    sprite->setAction(0,true);
    addChild(sprite,1);
}

void PopUpUI::showTreasureErrorTip(std::string& message)
{
    char errorBit1 = message.at(0);
    char errorBit2 = message.at(1);
    if (errorBit1 == '0')
    {
        if (errorBit2 == '0')
        {
            
        }
        else if (errorBit2 == '1')
        {
            ((YYScene*)getParent())->showTips(g_sLackKey);
        }
        else if (errorBit2 == '2')
        {
            setTreasureOpened(false);
        }
    }
    else if (errorBit1 == '1')
    {
        if (errorBit2 == '0')
        {
            
        }
        else if (errorBit2 == '1')
        {
            
        }
        else if (errorBit2 == '2')
        {
            
        }
        else if (errorBit2 == '3')
        {
            
        }
        else if (errorBit2 == '4')
        {
            
        }
    }
    else if (errorBit1 == '2')
    {
        if (errorBit2 == '0')
        {
            
        }
        else if (errorBit2 == '1')
        {
            
        }
        else if (errorBit2 == '2')
        {
            
        }
        else if (errorBit2 == '3')
        {
            
        }
    }
    setReadingTreasureData(false);
}

void PopUpUI::closeTreasureContainer()
{
    m_bTreasureOpened = false;
    UIPanel* panel_Treasure = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel02"));
    panel_Treasure->setEnabled(false);
    panel_Treasure->setVisible(false);
}

#pragma mark 退出背包
void PopUpUI::exitBagUI()
{
    m_pBagPageButton = NULL;
    m_pItemTypeButton = NULL;
    m_pAttributePageButton = NULL;// 当前选中的属性页签按键
    m_pBagEquip = NULL;// 背包内被拖动的装备
    m_pReplacedEquip = NULL;// 被替换的装备
    m_pBagCell = NULL;// 操作的装备所属背包格
    m_pReplacedCell = NULL;// 被替换装备所在格
    removeChildByTag(TAG_UI_PLAYER);
    CC_SAFE_RELEASE_NULL(m_pBagAllEquip);
}
