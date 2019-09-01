//
//  YYTrap.cpp
//  Fairyland
//
//  Created by fuchenhao on 14-1-6. 
//
//

#include "YYTrap.h"
#include "../common/Common.h"
#include "YYSpriteAttribute.h"
#include "YYSpriteLayer.h"
#include "YYController.h"

USING_NS_CC;
YYTrap::YYTrap()
:m_bTouchOff(false)
{
    
}

YYTrap::~YYTrap()
{
    
}

void YYTrap::initFightData(SpriteData * spriteData,SpriteAttribute * spriteAttribute)
{
    m_pSpriteData = spriteData;
    m_pSpriteAttribute = spriteAttribute;
    m_uCurrentHP = m_pSpriteAttribute->getMaxHP();
    m_u16ConfirmBehaviorCounter = m_pSpriteData->getConfirmBehaviorDuration();
    m_n16Speed = m_pSpriteData->getSpeed();
    m_nCurrentDisplacement = 0;
    m_nTotalDisplacement = m_pSpriteData->getTenacity();// 总位移
}

void YYTrap::update(void)
{
    YYFightSprite::update();

    if (!m_bPause && m_bActionOver)
    {
        switch (m_u8ActionIndex)
        {
            case ACTION_RUN:
            case ACTION_STAND0:
                m_u16CurrentFrameIndex = getCurrentActionData()->getStartFrameIndex();
                setFrame(m_u16CurrentFrameIndex);
                m_bActionOver = false;
                break;
            case ACTION_DIE:
                die();
                break;
            case ACTION_ATTACK_0:
                m_bActionOver = false;
                switchAction(ACTION_STAND0);
                setActionStatus(kActionNormal);
                break;
            default:
                m_bActionOver = false;
                switchAction(ACTION_STAND0);
                setActionStatus(kActionNormal);
                break;
        }
    }
    CCArray * arr = ((YYSpriteLayer *)getParent())->getFriendlyRoleArray();
    touchOffDetection(arr);
    if (m_bMoving)
    {
        move();
    }
}

// 触发条件检测
void YYTrap::touchOffDetection(cocos2d::CCArray * targetArray)
{
    if (!m_bTouchOff)
    {
        CCObject *object = NULL;
        CCARRAY_FOREACH(targetArray, object)
        {
            YYFightSprite * enemy = (YYFightSprite *)object;
            if (enemy->getPositionX() > getPositionX() - m_pSpriteData->getRangeOfVisibility().width
                && enemy->getPositionX() < getPositionX() + m_pSpriteData->getRangeOfVisibility().width
                && enemy->getPositionY() > getPositionY() - m_pSpriteData->getRangeOfVisibility().height
                && enemy->getPositionY() < getPositionY() + m_pSpriteData->getRangeOfVisibility().height
                && enemy->isVisible()
                && enemy->getCurrentHP() > 0)
            {
                m_pTargetSprite = enemy;
                m_bTouchOff = true;
                if (getSpriteType() == SPRITE_TYPE_ROLLING_BUCKET)
                {
                    setMoving(true);
                }
                else
                {
                    m_n8AttackIndex = 0;
                    int skillDataIndex = ((CCString *)m_pSpriteData->getSkillIndexArray()->objectAtIndex(m_n8AttackIndex))->intValue();
                    SkillData * skillData = (SkillData *)GlobalData::getInstance()->getCommonSkillData(skillDataIndex);
                    if (skillData)
                    {
                        createSkillWithData(skillData);
                        setSkillAction();
                        setActionStatus(kActionSkill);
                    }
                }
                break;
            }
        }
    }
    else if (getSpriteType() == SPRITE_TYPE_STAB)
    {
        if (getActionStatus() == kActionNormal && !m_pSkill)
        {
            m_n8AttackIndex = 0;
            int skillDataIndex = ((CCString *)m_pSpriteData->getSkillIndexArray()->objectAtIndex(m_n8AttackIndex))->intValue();
            SkillData * skillData = (SkillData *)GlobalData::getInstance()->getCommonSkillData(skillDataIndex);
            if (skillData)
            {
                createSkillWithData(skillData);
                setSkillAction();
                setActionStatus(kActionSkill);
            }

        }
    }
}

void YYTrap::move()
{
    if (getSpriteType() == SPRITE_TYPE_ROLLING_BUCKET)
    {// 滚桶 
        if (m_nCurrentDisplacement <= m_nTotalDisplacement - m_n16Speed)
        {
            m_nCurrentDisplacement += m_n16Speed;
            if (isFaceToRight())
            {
                setPosition(getPosition() + ccp(m_n16Speed, 0));
            }
            else
            {
                setPosition(getPosition() - ccp(m_n16Speed, 0));
            }
        }
        else
        {// 达到最大位移后消失 
            die();
        }
    }
}
