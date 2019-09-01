//
//  YYVipUI.cpp
//  Fairyland
//
//  Created by 付辰昊 on 9/1/14.
//
//

#include "YYPopUpUI.h"
#include "YYGlobalData.h"

#pragma mark 初始化UI

void PopUpUI::initVipUI()
{
    do {
        ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
        Controller::getInstance()->setGameStatus(kStatusUI);
        setUIType(kUIVip);
        m_pUITouchGroup = TouchGroup::create();
        CC_BREAK_IF(!m_pUITouchGroup);
        addChild(m_pUITouchGroup);
        m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
        std::string filePath = VIP_UI_RESOURCE_FILE_NAME;
        m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
        
        // 关闭
        Layout *panel0 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        CC_BREAK_IF(!panel0);
        UIButton *closeButton = static_cast<UIButton *>(panel0->getChildByName("Button_closed"));
        CC_BREAK_IF(!closeButton);
        closeButton->addTouchEventListener(this, toucheventselector(PopUpUI::vipCloseCallBack));
        
        Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_BREAK_IF(!panel2);
        
        UIScrollView* sc = static_cast<UIScrollView *>(panel2->getChildByName("ScrollView_0"));
        CC_BREAK_IF(!sc);
        sc->setBounceEnabled(true);
        
        // 上一页
        UIButton* btnPageUp = static_cast<UIButton *>(panel2->getChildByName("Button_0"));
        CC_BREAK_IF(!btnPageUp);
        btnPageUp->addTouchEventListener(this, toucheventselector(PopUpUI::vipPageUpCallBack));
        btnPageUp->setTouchEnabled(true);
        
         // 下一页
        UIButton* btnPageDown = static_cast<UIButton *>(panel2->getChildByName("Button_1"));
        CC_BREAK_IF(!btnPageDown);
        btnPageDown->addTouchEventListener(this, toucheventselector(PopUpUI::vipPageDownCallBack));
        btnPageDown->setTouchEnabled(true);
        
        // 充值
        UIPanel* panel1 = static_cast<UIPanel *>(m_pUITouchGroup->getWidgetByName("Panel1"));
        CC_BREAK_IF(!panel1);
        UIButton* btnVip = static_cast<UIButton *>(panel1->getChildByName("Button_vip"));
        CC_BREAK_IF(!btnVip);
        btnVip->addTouchEventListener(this, toucheventselector(PopUpUI::vipChargeCallBack));
        btnVip->setTouchEnabled(true);
        
        // 数据初始化
        m_nVipCurPageID = 0;
        m_nVipPageCnt = GlobalData::getInstance()->getVipDataDict()->allKeys()->count();
        
        updateVipUI();
        updateVipDescription();
       
    } while (0);
}


#pragma mark 关闭

void PopUpUI::vipCloseCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type,false));
            exitUI();
        }
    } while (0);
}

#pragma mark 上一页

void PopUpUI::vipPageUpCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type,false));
            if(m_nVipCurPageID > 0)
            {
                --m_nVipCurPageID;
                updateVipDescription();
            }
        }
    } while (0);
}

#pragma mark 下一页

void PopUpUI::vipPageDownCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type,false));
            if(m_nVipCurPageID < m_nVipPageCnt-1) // vip等级从0开始
            {
                ++m_nVipCurPageID;
                updateVipDescription();
            }
        }
    } while (0);
}

#pragma mark 更新vipUI

void PopUpUI::updateVipUI()
{
    do
    {
        int vipLevel = m_pRoleByControlled->getSpriteAttribute()->getVipLevel(); // vip等级
        int vipJew = m_pRoleByControlled->getSpriteAttribute()->getVipJew(); // vip已充值宝石
        Layout *panel1 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel1"));
        CC_BREAK_IF(!panel1);
        
        VipData* data = static_cast<VipData *>(GlobalData::getInstance()->getVipDataDict()->objectForKey(vipLevel));
        CC_BREAK_IF(!data);
        
        // 等级
        
        UILabel *label_0 = static_cast<UILabel *>(panel1->getChildByName("Label_0"));
        CC_BREAK_IF(!label_0);
        UILabel *label_0_0 = static_cast<UILabel *>(label_0->getChildByName("Label_0"));
        label_0_0->setText(CCString::createWithFormat("%d",vipLevel)->getCString());
        
        // 更新进度条
        
        int amount = data->getJewAmount();
        UIImageView * imageView_0 = static_cast<UIImageView *>(panel1->getChildByName("ImageView_0"));
        CC_BREAK_IF(!imageView_0);
        UIImageView * imageView_00 = static_cast<UIImageView *>(imageView_0->getChildByName("ImageView_0"));
        CC_BREAK_IF(!imageView_00);
        
        // 已充值宝石数
        UILabel* label_jew = static_cast<UILabel *>(imageView_0->getChildByName("Label_0"));
        CC_BREAK_IF(!label_jew);
        label_jew->setText(CCString::createWithFormat("%d",vipJew)->getCString());
        
        // "/"
        UILabel* label_line = static_cast<UILabel *>(imageView_0->getChildByName("Label_1"));
        CC_BREAK_IF(!label_line);
        
        // 总共宝石数
        UILabel* label_jew_2 = static_cast<UILabel *>(imageView_0->getChildByName("Label_2"));
        CC_BREAK_IF(!label_jew_2);
        label_jew_2->setText(CCString::createWithFormat("%d",data->getJewAmount())->getCString());
        
        // 再充值
        UILabel *label_1 = static_cast<UILabel *>(panel1->getChildByName("Label_1"));
        CC_BREAK_IF(!label_1);
        UILabel *label_0_1 = static_cast<UILabel *>(label_1->getChildByName("Label_0"));
        label_0_1->setText(CCString::createWithFormat("%d",(data->getJewAmount()-vipJew))->getCString());
        
        // 即可成为vip
        UILabel *label_0_2 = static_cast<UILabel *>(label_1->getChildByName("Label_2"));
        label_0_2->setText(CCString::createWithFormat("%d",++vipLevel)->getCString());
        
        // 达到顶级label
        UILabel *label_2 = static_cast<UILabel *>(panel1->getChildByName("Label_2"));
        CC_BREAK_IF(!label_2);
        
        if (amount > 0) // 升级状态
        {
            float percengage = vipJew / amount;
            imageView_00->removeFromParent();
            UIImageView *pImage = (UIImageView *)imageView_0->getChildByTag(10);
            if (!pImage)
            {
                pImage = UIImageView::create();
                std::string filePath = getFilePath("383-2", kDirTypeUI, kFileSuffixPng);
                pImage->loadTexture(filePath.c_str());
                pImage->setPosition(imageView_00->getPosition());
                pImage->setAnchorPoint(imageView_00->getAnchorPoint());
                pImage->setTextureRect(CCRectMake(0, 0, percengage * pImage->getSize().width, pImage->getSize().height));
                pImage->setScaleX(1.78f);
                pImage->setScaleY(1.625f);
                imageView_0->addChild(pImage, 0, 10);
            }
            else
            {
                pImage->setTextureRect(CCRectMake(0, 0, percengage * pImage->getSize().width, pImage->getSize().height));
            }
            label_2->setVisible(false);
        }
        else // 已达到顶级
        {
            label_1->setVisible(false);
            label_jew->setVisible(false);
            label_jew_2->setVisible(false);
            label_line->setVisible(false);
            label_2->setVisible(true);
        }

    } while (0);
}

#pragma mark 更新Vip描述

void PopUpUI::updateVipDescription()
{
    do
    {
        Layout *panel2 = static_cast<Layout*>(m_pUITouchGroup->getWidgetByName("Panel2"));
        CC_BREAK_IF(!panel2);
        
        UIImageView* imageView0 = static_cast<UIImageView *>(panel2->getChildByName("ImageView_0"));
        CC_BREAK_IF(!imageView0);
        UILabel* label_1 = static_cast<UILabel *>(imageView0->getChildByName("Label_1"));
        CC_BREAK_IF(!label_1);
        label_1->setText(CCString::createWithFormat("%d",m_nVipCurPageID)->getCString());
        
        // 查看左
        UILabel *label_0 = static_cast<UILabel *>(panel2->getChildByName("Label_0"));
        CC_BREAK_IF(!label_0);
        label_0->setVisible(true);
        UILabel *label_0_1 = static_cast<UILabel *>(label_0->getChildByName("Label_1"));
        label_0_1->setText(CCString::createWithFormat("%d",m_nVipCurPageID-1)->getCString());
        
        // 查看右
        UILabel *label_1_0 = static_cast<UILabel *>(panel2->getChildByName("Label_1"));
        label_1_0->setVisible(true);
        CC_BREAK_IF(!label_1_0);
        UILabel *label_1_1 = static_cast<UILabel *>(label_1_0->getChildByName("Label_1"));
        label_1_1->setText(CCString::createWithFormat("%d",m_nVipCurPageID+1)->getCString());
        
        // 描述
        VipData* data = static_cast<VipData *>(GlobalData::getInstance()->getVipDataDict()->objectForKey(m_nVipCurPageID));
        CC_BREAK_IF(!data);
        UIScrollView* sc = static_cast<UIScrollView *>(panel2->getChildByName("ScrollView_0"));
        CC_BREAK_IF(!sc);
        sc->jumpToTop();
        UILabel* label_2 = static_cast<UILabel *>(sc->getChildByName("Label_2"));
        CC_BREAK_IF(!label_2);
        label_2->setText(data->getDesc());
        
        // 上一页
        UIButton* btnPageUp = static_cast<UIButton *>(panel2->getChildByName("Button_0"));
        CC_BREAK_IF(!btnPageUp);
        btnPageUp->setVisible(true);
        btnPageUp->setTouchEnabled(true);
        
        // 下一页
        UIButton* btnPageDown = static_cast<UIButton *>(panel2->getChildByName("Button_1"));
        CC_BREAK_IF(!btnPageDown);
        btnPageDown->setVisible(true);
        btnPageDown->setTouchEnabled(true);
        
        
        // 临界值处理 隐藏左右选项
        if (m_nVipCurPageID <= 0)
        {
            label_0->setVisible(false);
            btnPageUp->setVisible(false);
            btnPageUp->setTouchEnabled(false);
        }
        else if(m_nVipCurPageID >= m_nVipPageCnt-1)
        {
            label_1_0->setVisible(false);
            btnPageDown->setVisible(false);
            btnPageDown->setTouchEnabled(false);
        }
        
    } while (0);
}

#pragma mark 充值

void PopUpUI::vipChargeCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    do
    {
        CC_BREAK_IF(Controller::getInstance()->getScene()->isNetLoading());
        if (type == TOUCH_EVENT_ENDED)
        {
            CC_BREAK_IF(isTouchUnable(pSender,type,false));
            // 调用充值页面
        }
    } while (0);
}
