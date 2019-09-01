//
//  YYLinkEffect.cpp
//  XJ
//
//  Created by fuchenhao on 13-6-9. 
//
//

#include "YYLinkEffect.h"
#include "YYSpriteLayer.h"

USING_NS_CC;

#define LINK_EFFECT_LENGTH 40// 闪电链特效长度 

YYLinkEffect::YYLinkEffect()
:m_nLinkAmount(4)
,m_nLinkCount(0)
,m_nLinkInterval(1)
,m_nLinkIntervalCount(0)
,m_nMaxLinkDistance(450)
,m_bLinkPause(false)
,m_pCurrentTarget(NULL)
,m_pTargetArray(NULL)
,m_pLinkedArray(NULL)
{
    m_pTargetArray = new CCArray(15);
    m_pLinkedArray = new CCArray(m_nLinkAmount);
}

YYLinkEffect::~YYLinkEffect()
{
    CC_SAFE_DELETE(m_pLinkedArray);
    CC_SAFE_DELETE(m_pTargetArray);
}

void YYLinkEffect::update()
{
    updateAnimation();
    for (int i = 0; i < getChildren()->count(); i++) {
        YYSprite * sprite = dynamic_cast<YYSprite*>(getChildren()->objectAtIndex(i));
        if (sprite)
        {
            sprite->updateAnimation();
        }
    }
    if (m_bLinkPause)
    {
        if (m_nLinkIntervalCount < m_nLinkInterval)
        {
            m_nLinkIntervalCount++;
        }
        else
        {
            m_bLinkPause = false;
            m_nLinkIntervalCount = 0;
        }
    }else{
        if (m_nLinkCount < m_nLinkAmount)
        {
            linkNextTarget(m_pCurrentTarget);
        }
        else
        {
            m_bDeleted = true;
        }
    }
}

void YYLinkEffect::updateTarget(YYFightSprite * fightSprite)
{
    m_pLinkedArray->addObject(fightSprite);
    
    CCArray * array = ((YYSpriteLayer *)getParent())->getMoveableEnemyArray();
    m_pTargetArray->removeAllObjects();
    m_pTargetArray->addObjectsFromArray(array);
    
    for (int i = 0; i < m_pLinkedArray->count(); i++)
    {
        YYFightSprite *linkedSprite = (YYFightSprite *)m_pLinkedArray->objectAtIndex(i);
        for (int j = 0; j < m_pTargetArray->count(); j ++)
        {
            YYFightSprite * target = (YYFightSprite *)m_pTargetArray->objectAtIndex(j);
            if (target == linkedSprite)
            {
                m_pTargetArray->removeObjectAtIndex(j);
            }
        }
    }
}

void YYLinkEffect::linkNextTarget(YYFightSprite * currentTarget)
{
    updateTarget(currentTarget);
    int linkDistance = 0;
    YYFightSprite * nextTarget = NULL;
    bool bRecord = false;
    for (int i = 0; i < m_pTargetArray->count(); i++)
    {
        YYFightSprite * fightSprite = (YYFightSprite *)m_pTargetArray->objectAtIndex(i);
        int distance = (currentTarget->getPositionX()-fightSprite->getPositionX())*(currentTarget->getPositionX()-fightSprite->getPositionX()) + (currentTarget->getPositionY()-fightSprite->getPositionY())*(currentTarget->getPositionY()-fightSprite->getPositionY());
        if (distance > m_nMaxLinkDistance * m_nMaxLinkDistance) {
            continue;
        }
        if (!bRecord)
        {
            bRecord = true;
            linkDistance = distance;
            nextTarget = fightSprite;
        }
        else if (distance < linkDistance)
        {
            linkDistance = distance;
            nextTarget = fightSprite;
        }
    }
    if (nextTarget)
    {
        m_pCurrentTarget = nextTarget;
        float radian = atan2f(nextTarget->getPositionY() - currentTarget->getPositionY(), nextTarget->getPositionX() - currentTarget->getPositionX());
        CCPoint initPosition = ccp(currentTarget->getPositionX(),currentTarget->getPositionY() + currentTarget->getSize().height / 2);
        setLinkEffect(initPosition,sqrt((float)linkDistance),radian);
//        m_pCurrentTarget->hitted();
        m_bLinkPause = true;
        m_nLinkCount ++;
    }
    else
    {
        m_bDeleted = true;
    }
}

void YYLinkEffect::setLinkEffect(cocos2d::CCPoint initPosition,int linkDistance ,float radian)
{
    int count = linkDistance / LINK_EFFECT_LENGTH;
    for(int i = 0;i < count+1;i++)
    {
        YYSprite * lightning = YYSprite::createWithActionSet("light", "lightning", kDirTypeSprite);
        lightning->setAnchorPoint(ccp(0,0.5));
        lightning->setPosition(ccp(initPosition.x + cos(radian) * LINK_EFFECT_LENGTH * i,initPosition.y + sin(radian) * LINK_EFFECT_LENGTH * i));
        lightning->setRotation(-radian * 180.0f / M_PI);
        lightning->setAction(0, true);
        addChild(lightning,-1);
    }
}
