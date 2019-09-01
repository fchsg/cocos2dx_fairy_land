//
//  YYLogo.cpp
//  XJ
//
//  Created by fuchenhao on 13-5-9. 
//
//

#include "YYLogo.h"
#include "../common/Common.h"
#include "../animation/YYSprite.h"
#include "YYController.h"

USING_NS_CC;

Logo::Logo()
:m_pLogoSprite(NULL)
{
}

Logo::~Logo()
{
}

bool Logo::init()
{
    CCLayer::init();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	ccColor4B ct0 = {255,255,255,255};
    CCLayerColor *whiteBG = CCLayerColor::create(ct0, size.width, size.height);
    addChild(whiteBG);
    m_pLogoSprite = YYSprite::createWithActionSet("studio_logo","studio_logo",kDirTypePic);
    m_pLogoSprite->setPosition(ccp(size.width/2, size.height/2));
    m_pLogoSprite->setAction(0,false);
    addChild(m_pLogoSprite ,0 ,0);
    return true;
}

void Logo::update()
{
    if(m_pLogoSprite)
    {
        m_pLogoSprite->updateAnimation();
        if (m_pLogoSprite->isActionOver())
        {
            Controller::getInstance()->switchGameStatus(kStatusCover);
        }
    }
}
