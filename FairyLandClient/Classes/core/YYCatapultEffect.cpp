//
//  YYCatapult.cpp
//  XJ
//
//  Created by fuchenhao on 13-6-13. 
//
//

#include "YYCatapultEffect.h"
#include "YYSpriteLayer.h"
#include "YYSkillData.h"
#include "YYSkillEffect.h"

USING_NS_CC;

YYCatapultEffect::YYCatapultEffect()
:m_nCatapultCount(0)
,m_pNextTarget(NULL)
,m_pTargetArray(NULL)
,m_bSpellAttack(false)
{
    m_pTargetArray = new CCArray(15);
}

YYCatapultEffect::~YYCatapultEffect()
{
    CC_SAFE_DELETE(m_pTargetArray);
}

void YYCatapultEffect::updateTarget(cocos2d::CCArray * targetArray)
{
    m_pTargetArray->removeAllObjects();
    m_pTargetArray->addObjectsFromArray(targetArray);
    if (m_pNextTarget)
    {
        m_pTargetArray->removeObject(m_pNextTarget);
    }
}

void YYCatapultEffect::catapultToNextTarget(YYFightSprite * currentTarget)
{
    updateTarget(((YYSpriteLayer*)getParent())->getMoveableEnemyArray());
    bool bRecord = false;
    float minDistance = 0;
    YYFightSprite * nextTarget = NULL;
    for (int i = 0 ; i < m_pTargetArray->count(); i++)
    {
        YYFightSprite * sprite = (YYFightSprite *)m_pTargetArray->objectAtIndex(i);
        if (!sprite->isVisible())
        {
            continue;
        }
        float distance = (sprite->getPositionX() - getPositionX()) * (sprite->getPositionX() - getPositionX()) +
        (sprite->getPositionY() - getPositionY()) * (sprite->getPositionY() - getPositionY());
        
        if (distance > m_pSkillData->getMaxCatapultDistance() * m_pSkillData->getMaxCatapultDistance())
        {
            continue;
        }
        
        if (!bRecord)
        {
            bRecord = true;
            minDistance = distance;
            nextTarget = sprite;
        }
        else if (distance < minDistance)
        {
            minDistance = distance;
            nextTarget = sprite;
        }
    }
    if (nextTarget)
    {
        m_pNextTarget = nextTarget;
        damageTarget(m_pNextTarget,m_pSkillData,m_pSkillEffectData->getHittedEffectResource().c_str());
        m_nCatapultCount++;
    }
    else
    {
        m_bDeleted = true;
    }
}

#pragma mark 计算当前帧位移 

void YYCatapultEffect::calculateCurrentFrameDisplacement()
{
    int radian = atan2f(m_pNextTarget->getPositionY() - getPositionY(), m_pNextTarget->getPositionX() - getPositionX());
    float offsetX = m_n16Speed * cos((float)radian);
    float offsetY = m_n16Speed * sin((float)radian);
    if (fabs(offsetX) > fabs(getPositionX() - m_pNextTarget->getPositionX()))
    {
        offsetX = m_pNextTarget->getPositionX() - getPositionX();
    }
    if (fabs(offsetY) > fabs(getPositionY() - m_pNextTarget->getPositionY()))
    {
        offsetY = m_pNextTarget->getPositionY() - getPositionY();
    }
    
    setPosition(ccpAdd(ccp(offsetX,offsetY), getPosition()));
    
    if (m_pNextTarget->getPositionY() == getPositionY() && m_pNextTarget->getPositionX() == getPositionX())
    {
        damageTarget(m_pNextTarget,m_pSkillData, m_pSkillEffectData->getHittedEffectResource().c_str());
        if (m_nCatapultCount < m_pSkillData->getCatapultAmount())
        {
            catapultToNextTarget(m_pNextTarget);
        }
        else
        {
            m_bDeleted = true;
        }
        return;
    }
}

void YYCatapultEffect::update()
{
    calculateCurrentFrameDisplacement();
}
