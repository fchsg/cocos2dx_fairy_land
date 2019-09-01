//
//  YYLoading.cpp
//  XJ
//
//  Created by fuchenhao on 13-5-23. 
//
//

#include "YYLoading.h"
#include "../animation/YYSprite.h"
#include "../common/Common.h"
#include "YYController.h"
#include "YYScene.h"

USING_NS_CC;

Loading::Loading()
:m_bResumeAction(false)
,m_nCount(0)
,m_nRepeatScene(0)
,m_pLabel(NULL)
,m_pLabelMsg(NULL)
{
}

Loading::~Loading()
{
}

bool Loading::init()
{
    CCLayer::init();
    //loading动画
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //黑背景
    ccColor4B ct0 = {0,0,0,255};
    CCLayerColor *blackBG = CCLayerColor::create(ct0, winSize.width, winSize.height);
    addChild(blackBG);

    m_nCount = 0;
    m_nRepeatScene = 0;
    m_bResumeAction = false;
    m_pLabel = CCLabelTTF::create("0", DEFAULT_FONT_NAME, 24);
    m_pLabel->setPosition(ccp(winSize.width / 2, 50));
    addChild(m_pLabel,2);
    
    //loading 提示信息
    uint8_t u8SelID = RANDOM_INT(0,MSG_LABEL_COUNT-1);
    m_pLabelMsg = CCLabelTTF::create(g_sLoadingMsgStr[u8SelID], DEFAULT_FONT_NAME, 24);
    m_pLabelMsg->setAnchorPoint(ccp(0.5f,0.5f));
    m_pLabelMsg->setPosition(ccp(winSize.width / 2, winSize.height / 2));
    addChild(m_pLabelMsg,2);
   
    std::string str = getFilePath("38", kDirTypePic, kFileSuffixPng);
    CCSprite* sprite = CCSprite::create(str.c_str());
    sprite->setPosition(ccp(winSize.width / 2, 50));
    addChild(sprite);
    
    str = getFilePath("39", kDirTypePic, kFileSuffixPng);
    CCProgressTimer* progressTimer = CCProgressTimer::create(CCSprite::create(str.c_str()));
    progressTimer->setAnchorPoint(ccp(0.5,0.5));
    progressTimer->setPosition(ccp(winSize.width / 2,50));
    addChild(progressTimer,1,CUTSCENE_STATE);
    
    cocos2d::CCProgressFromTo * progress = CCProgressFromTo::create(2, 0, 100);
    progressTimer->setMidpoint(ccp(0,0));
    progressTimer->setBarChangeRate(ccp(1, 0));
    progressTimer->setType( kCCProgressTimerTypeBar );
    progressTimer->runAction(progress);
    progressTimer->pauseSchedulerAndActions();
    progress->setTag(CUTSCENE_ANIMATION);

    YYSprite * loading = YYSprite::create();
    loading->initWithActionSet("bigloading","bigloading",kDirTypePic);
    loading->setAction(0, true);
    loading->setPosition(ccp(progressTimer->getPositionX() - progressTimer->getContentSize().width / 2 , progressTimer->getPositionY()));
    loading->setTag(TAG_LOADING_SE);
    addChild(loading,1);
    
    m_u16FrameCounter = 0;
    m_u8SwitchFrame = 0;
    return true;
}

void Loading::update()
{
    CCProgressTimer *timer = static_cast<CCProgressTimer*>(getChildByTag(CUTSCENE_STATE));
    float percent = timer->getPercentage();
    const char * progress = CCString::createWithFormat("%.2f%%",percent)->getCString();
    uint16_t u16FrameIdx = 0;
    m_pLabel->setString(progress);
    YYSprite* sprite = ((YYSprite*)getChildByTag(TAG_LOADING_SE));
    if(sprite)
    {
        if (0 <= percent && percent < 15 )
        {
            u16FrameIdx = 0;
            sprite->setFrame(u16FrameIdx);
        }
        else if (15 <= percent && percent < 20 )
        {
            u16FrameIdx = 1;
            sprite->setFrame(u16FrameIdx);
        }
        else if (20 <= percent && percent < 30 )
        {
            u16FrameIdx = 2;
            sprite->setFrame(u16FrameIdx);
        }
        else if (30 <= percent && percent < 40 )
        {
            u16FrameIdx = 3;
            sprite->setFrame(u16FrameIdx);
        }
        else if (40 <= percent && percent < 50 )
        {
            u16FrameIdx = 4;
            sprite->setFrame(u16FrameIdx);
        }
        else if (50 <= percent && percent < 60 )
        {
            u16FrameIdx = 5;
            sprite->setFrame(u16FrameIdx);
        }
        else if (60 <= percent && percent < 70 )
        {
            u16FrameIdx = 6;
            sprite->setFrame(u16FrameIdx);
        }
        else if (70 <= percent && percent < 90 )
        {
            u16FrameIdx = 7;
            sprite->setFrame(u16FrameIdx);
        }
        else
        {
            m_u16FrameCounter++;
           if (m_u16FrameCounter >= 3)
            {
              m_u16FrameCounter = 0;
              switch (m_u8SwitchFrame)
                {
                    case 0:
                        u16FrameIdx = 8;
                        break;
                    case 1:
                        u16FrameIdx = 9;
                        break;
                    case 2:
                        u16FrameIdx = 10;
                        break;
                    case 3:
                        u16FrameIdx = 11;
                        break;
                    default:
                        u16FrameIdx = 0;
                        break;
                }
                m_u8SwitchFrame++;
                if (m_u8SwitchFrame > 4)
                {
                    m_u8SwitchFrame = 0;
                }
                if (u16FrameIdx == 0)
                {
                    u16FrameIdx = 9;
                }
                sprite->setFrame(u16FrameIdx);
            }
        }
        int x = timer->getPositionX() - timer->getContentSize().width / 2 + timer->getContentSize().width * percent / 100;
        if (x < timer->getPositionX() - (timer->getContentSize().width - sprite->getDefaultSize().width) / 2 + 10)
        {
            x = timer->getPositionX() - (timer->getContentSize().width - sprite->getDefaultSize().width) / 2 + 10;
        }
        if (x > timer->getPositionX() + (timer->getContentSize().width - sprite->getDefaultSize().width) / 2 - 10)
        {
            x = timer->getPositionX() + (timer->getContentSize().width - sprite->getDefaultSize().width) / 2 - 10;
        }
        sprite->setPosition(ccp(x,timer->getPositionY()));
    }
    if (percent < 100)
    {
        if(m_bResumeAction)
        {
            ((CCProgressTimer*)getChildByTag(CUTSCENE_STATE))->resumeSchedulerAndActions();
        }
    }
    else
    {
        m_bProgressCompleted = true;
        if(Controller::getInstance()->isLoadingEnd())
        {
            if (m_nCount < 1)
            {
                m_nCount ++;
            }
            else
            {
                m_nCount = 0;
                Controller::getInstance()->loadOver();
            }
        }
    }
}


