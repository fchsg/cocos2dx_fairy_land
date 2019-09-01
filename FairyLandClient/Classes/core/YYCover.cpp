//
//  Cover.cpp
//  XJ
//
//  Created by fuchenhao on 13-5-9.
//
//

#include "YYCover.h"
#include "../common/Common.h"
#include "../common/ODSocket.h"
#include "../animation/YYSprite.h"
#include "YYController.h"
#include "YYConfig.h"
#include "YYScene.h"
USING_NS_CC;

Cover::Cover()
:m_pLightningSprite(NULL)
,m_pCoverSprite(NULL)
,m_pRoleSprite(NULL)
,m_pSmokeSprite1(NULL)
,m_pSmokeSprite2(NULL)
,m_pTipSprite(NULL)
,m_pProgressLabel(NULL)
,m_bShowConnectFailed(false)
,m_CoverStatus(kCoverStatusDefault)
{
}

Cover::~Cover()
{
}

void Cover::onEnter()
{
    cocos2d::CCLayer::onEnter();
    //const char* message = CCString::createWithFormat(VERIFY_VERSION,Config::getInstance()->getVersion().c_str())->getCString();
    const char* message = CCString::createWithFormat(ASK_SERVER_LIST,Config::getInstance()->getChannel().c_str())->getCString();
    if (message)
    {
        if (ODSocket::getInstance()->getNetStatus() == kNetStatusConnected)
        {
            Controller::getInstance()->sendMessage(message, true);
        }
        else
        {
            Controller::getInstance()->setConnectMessage(message);
            Controller::getInstance()->connectRegister();
        }
    }
}
bool Cover::init()
{
    CCLayer::init();
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //黑背景
    ccColor4B ct0 = {0,0,0,255};
    CCLayerColor *blackBG = CCLayerColor::create(ct0, winSize.width, winSize.height);
    addChild(blackBG);

    // 封面背景
    std::string filePath = getFilePath("bg", kDirTypeCover, kFileSuffixPng);
    m_pCoverSprite = CCSprite::create(filePath.c_str());
    m_pCoverSprite->setAnchorPoint(ccp(0, 0));
    m_pCoverSprite->setPosition(ccp(0,0));
    addChild(m_pCoverSprite);
    
    //标题
    filePath = getFilePath("312", kDirTypeUI, kFileSuffixPng);
    CCSprite *title = CCSprite::create(filePath.c_str()
                                       );
    title->setPosition(ccp(winSize.width / 2,winSize.height / 2 + 150));
    addChild(title,1);
    
    // 角色动画
    m_pRoleSprite = YYSprite::createWithActionSet("role","role",kDirTypeCover);
    m_pRoleSprite->setPosition(ccp(0,0));
    m_pRoleSprite->setAction(0,true);
    addChild(m_pRoleSprite,1);
    
//    //提示信息
    m_pTipSprite = YYSprite::createWithActionSet("tip", "tip0,tip1", kDirTypeCover);
    m_pTipSprite->setPosition(ccp(winSize.width / 2,winSize.height / 2 - 80));
    m_pTipSprite->setAction(0,true);
    addChild(m_pTipSprite,1);
    m_pTipSprite->setVisible(false);
    
    //闪电
    m_pLightningSprite = YYSprite::createWithActionSet("lightning","lightning0,lightning1,lightning2,lightning3,lightning4,lightning5,lightning6,lightning7,lightning8,lightning9,lightning10,lightning11,lightning12,lightning13",kDirTypeCover);
    m_pLightningSprite->setPosition(ccp(258, STANDARD_SCENE_HEIGHT - 85));
    m_pLightningSprite->setAction(0,true);
    m_pCoverSprite->addChild(m_pLightningSprite ,1);
    
    //烟雾1
    m_pSmokeSprite1 = YYSprite::createWithActionSet("smoke","smoke0,smoke1,smoke2,smoke3,smoke4",kDirTypeCover);
    m_pSmokeSprite1->setPosition(ccp(920, STANDARD_SCENE_HEIGHT - 307));
    m_pSmokeSprite1->setAction(0,true);
    m_pCoverSprite->addChild(m_pSmokeSprite1, 1);
    
    //烟雾2
    m_pSmokeSprite2 = YYSprite::copy(m_pSmokeSprite1);
    m_pSmokeSprite2->setPosition(ccp(1130, STANDARD_SCENE_HEIGHT - 277));
    m_pSmokeSprite2->setAction(0,true);
    m_pCoverSprite->addChild(m_pSmokeSprite2, 1);
    
    m_pProgressLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 25);
    m_pProgressLabel->setAnchorPoint(ccp(1, 0));
    m_pProgressLabel->setPosition(ccp(winSize.width - 10, 5));
    addChild(m_pProgressLabel,1);
    
    m_pVersionLabel = CCLabelTTF::create("", DEFAULT_FONT_NAME, 25);
    m_pVersionLabel->setAnchorPoint(ccp(0, 0));
    m_pVersionLabel->setPosition(ccp(10, 5));
    m_pVersionLabel->setString(Config::getInstance()->getVersion().c_str());
    addChild(m_pVersionLabel,1);
    
    return true;
}

void Cover::update()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (m_pCoverSprite)
    {
        if (m_pCoverSprite->getPosition().x > winSize.width - m_pCoverSprite->getContentSize().width)
        {
            m_pCoverSprite->setPosition(ccpAdd(m_pCoverSprite->getPosition(),ccp(-0.5f, 0)));
        }
    }
    if (m_pRoleSprite)
    {
        m_pRoleSprite->updateAnimation();
    }
    if (m_pLightningSprite)
    {
        m_pLightningSprite->updateAnimation();
    }
    if (m_pSmokeSprite1)
    {
        m_pSmokeSprite1->updateAnimation();
    }
    if (m_pSmokeSprite2)
    {
        m_pSmokeSprite2->updateAnimation();
    }
    if (m_pTipSprite && m_pTipSprite->isVisible())
    {
        m_pTipSprite->updateAnimation();
    }
    if(!m_bShowConnectFailed && Controller::getInstance()->getRegServerFailed()) // 注册服务器联网失败 退出应用
    {
        m_bShowConnectFailed = true;
        ((YYScene*)getParent())->showTips(g_sNetworkError, kTipCoverConnectFailed);
    }
}

void Cover::touchBegan(cocos2d::CCTouch * touch)
{
    if (m_CoverStatus == kCoverStatusNormal)
    {
        Controller::getInstance()->setLoadingEnd(true);
        Controller::getInstance()->setLoadingOverStatus(kLOSLogin);
        Controller::getInstance()->switchGameStatus(kStatusLoading);
    }
    
}

void Cover::onError(cocos2d::extension::AssetsManager::ErrorCode errorCode)
{
    if (errorCode == cocos2d::extension::AssetsManager::kNoNewVersion)
    {
        m_pProgressLabel->setString("no new version");
    }

    if (errorCode == cocos2d::extension::AssetsManager::kNetwork)
    {
        m_pProgressLabel->setString("network error");
    }
}

void Cover::onProgress(int percent)
{
    char progress[20];
    snprintf(progress, 20, "downloading %d%%", percent);
    m_pProgressLabel->setString(progress);
}

void Cover::onSuccess()
{
    CCLog("Cover::onSuccess");
    m_pProgressLabel->setString("download ok");
}
