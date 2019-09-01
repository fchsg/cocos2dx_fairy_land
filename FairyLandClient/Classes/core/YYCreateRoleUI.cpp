////
////  CreateRoleUI.cpp
////  Fairland
////
////  Created by 章敏 on 11/8/13. 
////
////
//#include "YYController.h"
//#include "../common/Common.h"
//#include "../animation/YYSprite.h"
//#include "YYCreateRoleUI.h"
//#include "SimpleAudioEngine.h"
//#include "YYSpriteData.h"
//#include "YYPlayer.h"
//#include "YYConfig.h"
//#include "YYFightSprite.h"
//#include "../common/ccUtil.h"
//
//USING_NS_CC;
//USING_NS_CC_EXT;
//using namespace CocosDenshion;
//using namespace gui;
//#define TAG_ROOT 100
//#define TAG_ROLEPLAYER 150
//
//CreateRoleUI::CreateRoleUI()
//:m_pLoginTG(NULL)
//,input(NULL)
//,m_bTouchBegan(false)
//,enter_button(NULL)
//,m_pInputName(NULL)
//,m_u8LoginStatus(0)
//{
//}
//
//CreateRoleUI::~CreateRoleUI()
//{
//}
//
//CCScene* CreateRoleUI::scene(CreateRoleUI * start)
//{
//    // 'scene' is an autorelease object
//    CCScene *scene = CCScene::create();
//    CCSize size = CCDirector::sharedDirector()->getWinSize();
//	ccColor4B ct0 = {255,255,255,255};
//    CCLayerColor *whiteBG = CCLayerColor::create(ct0, size.width, size.height);
//    scene->addChild(whiteBG);
//    // 'layer' is an autorelease object
//    scene->addChild(start);
//    
//    CCLayer * layer = CCLayer::create();
//    layer->setTag(10);
//    scene->addChild(layer,1);
//    YYSprite * sprite = YYSprite::createWithActionSet("loading","loading0,loading1,loading2,loading3,loading4,loading5",PIC_DIR);
//    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
//    sprite->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
//    sprite->setAction(0,true);
//    layer->addChild(sprite,0,1);
////    layer->setVisible(false);
//    // return the scene
//    return scene;
//}
//
//bool CreateRoleUI::init()
//{
//    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//    m_UIInitPosition = ccp((winSize.width - 960) / 2,(winSize.height - 640) / 2);
////  m_UIInitPosition = ccp(0,0);
//    createRole();
//    return true;
//}
//
//
//
//void CreateRoleUI::touchBegan(cocos2d::CCSet *pTouches){
//    CCTouch * pTouch = (CCTouch *)pTouches->anyObject();
//    m_TouchPoint = pTouch->getLocation();
//    m_bTouchBegan = true;
//}
//
//void CreateRoleUI::setRoleName(const char *pText)
//{
//    m_pInputName->setText(pText);
//}
//
//void CreateRoleUI::updateRoleAction(int idx)
//{
//    if (m_pCreateRoleSprite[idx]->isActionOver())
//    {
//        m_pRoleAnimation[idx].curActionIdx++;
//
//        if (m_pRoleAnimation[idx].curActionIdx >= m_pRoleAnimation[idx].count)
//        {
//             m_pRoleAnimation[idx].curActionIdx = 0;
//        }
//        m_pCreateRoleSprite[idx]->setAction(m_pRoleAnimation[idx].actionIdx[m_pRoleAnimation[idx].curActionIdx]);
//        
//        for (int i = 0; i < m_pRoleAnimation[idx].count; i++)
//        {
//            if(idx == 1 && i == 0)
//                continue;
//            m_pRoleAnimation[idx].effect[i]->setAction(0,true);
//            if (i == m_pRoleAnimation[idx].curActionIdx)
//            {
//               m_pRoleAnimation[idx].effect[i]->setVisible(true);
//            }
//            else
//            {
//               m_pRoleAnimation[idx].effect[i]->setVisible(false);
//            }
//          
//        }
//    }
//    if (m_pRoleAnimation[idx].effect[m_pRoleAnimation[idx].curActionIdx])
//    {
//       m_pRoleAnimation[idx].effect[m_pRoleAnimation[idx].curActionIdx]->updateAnimation();
//    }
//    m_pCreateRoleSprite[idx]->updateAnimation();
//
//}
//
//void CreateRoleUI::update()
//{
//    if (m_bNetLoading)
//    {
//        ((YYSprite *)((CCScene *)getParent())->getChildByTag(10)->getChildByTag(1))->updateAnimation();
//    }
//    for (int i = 0; i < 2; i++)
//    {
//        if (m_bCreatRolePlaying[i])
//        {
//            updateRoleAction(i);
//        }
//    }
//    if(m_bTouchBegan)
//    {
//        m_bTouchBegan = false;
//        if(getLoginStatus() == kLoginChooseRole)
//        {
//            Layout* panel1 = static_cast<Layout*>(m_pLoginTG->getWidgetByTag(TAG_ROOT)->getChildByName("Panel_1"));
//            for(int i=0;i < panel1->getChildren()->count();++i)
//            {
//                Layout* panel_temp = static_cast<Layout*>(panel1->getChildren()->objectAtIndex(i));
//                CCRect rect = CCRectMake(panel_temp->getPosition().x + m_UIInitPosition.x, panel_temp->getPosition().y + m_UIInitPosition.y, panel_temp->getContentSize().width, panel_temp->getContentSize().height);
//                if(rect.containsPoint(m_TouchPoint))
//                {
//                    UILabel *title_Text = static_cast<UILabel*>(enter_button->getChildByName("TextArea_Clone"));
//                    title_Text->setText(g_sCreateRole);
//                }
//            }
//        }
//        else if (getLoginStatus() == kLoginCreateRole)
//        {
//            
//        }
//    }
//}
//
//void CreateRoleUI::menuEnterCallback(CCObject* pSender, TouchEventType type)
//{
//    if(m_bNetLoading)
//        return;
//    if (type == TOUCH_EVENT_ENDED)
//    {
//        UILabel* title_Text = static_cast<UILabel*>(enter_button->getChildByName("TextArea_Clone"));
//        if(strcmp(title_Text->getStringValue(), g_sGameBegin) == 0)
//        {
//            
//        }
//        else if (strcmp(title_Text->getStringValue(), g_sCreateRole) == 0)
//        {
//            removeChild(m_pLoginTG);
//            createRole();
//        }
//    }
//}
//
//void CreateRoleUI::menuReturnCallback(CCObject* pSender, TouchEventType type)
//{
//    if(m_bNetLoading)
//        return;
//    if (type == TOUCH_EVENT_ENDED)
//    {
//        removeChild(m_pLoginTG);
//        Controller::getInstance()->switchGameStatus(kStatusChooseRole);
//    }
//}
//
//void CreateRoleUI::menuChooseRoleCallback(CCObject* pSender, TouchEventType type)
//{
//    if(m_bNetLoading)
//        return;
//    if (type == TOUCH_EVENT_ENDED)
//    {
//        int tag = static_cast<UIWidget*>(pSender)->getTag();
//        updateRole(tag);
//    }
//}
//
//
//extern std::string trim(std::string& str) ;
//void CreateRoleUI::menuStartCallback(CCObject* pSender, TouchEventType type)
//{
//    if(m_bNetLoading)
//        return;
//    if (type == TOUCH_EVENT_ENDED)
//    {
//		std::string roleName = m_pInputName->getText();;
//		if(roleName.empty())
//			roleName = m_pInputName->getPlaceHolder();
//		roleName = trim(roleName);
//        Controller::getInstance()->createRole(m_iProfessionIndex, roleName);	//login();
//    }
//}
//
//void CreateRoleUI::createRole()
//{
//
//}
//
//void CreateRoleUI::btnGenRoleName(CCObject* pSender, TouchEventType type)
//{
//    if(m_bNetLoading)
//        return;
//    if (type == TOUCH_EVENT_ENDED)
//    {
//        CCString *msg = CCString::createWithFormat(ASK_RANDOM_NAME,m_iProfessionIndex);
//        Controller::getInstance()->sendMessage(msg->getCString(),true);
//	}
//}
//
//void CreateRoleUI::setNetLoading(bool isNetLoading)
//{
//    m_bNetLoading = isNetLoading;
//    CCLayer * layer = (CCLayer *) ((CCScene *)getParent())->getChildByTag(10);
//    layer->setVisible(isNetLoading);
//}
