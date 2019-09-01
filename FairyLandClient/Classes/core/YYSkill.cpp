//
//  Skill.cpp
//  XJ
//
//  Created by fuchenhao on 13-7-22. 
//
//

#include "cocos2d.h"
#include "YYSkill.h"
#include "YYSkillData.h"
#include "YYSkillAction.h"
#include "YYController.h"
#include "YYGlobalData.h"

USING_NS_CC;

Skill::Skill()
:m_u8ActionDataIndex(0)
,m_pSkillData(NULL)
,m_pSkillAction(NULL)
,m_bSkillOver(false)
{
}

Skill::~Skill()
{
    CC_SAFE_DELETE(m_pSkillAction);
}

void Skill::initWithSkillData(SkillData * skillData)
{
    m_pSkillData = skillData;
    CC_SAFE_DELETE(m_pSkillAction);
    m_pSkillAction = new SkillAction();
    int actionDataIndex = ((CCString *)skillData->getActionIndexArray()->objectAtIndex(m_u8ActionDataIndex))->intValue();
    SkillActionData *actionData = (SkillActionData *)GlobalData::getInstance()->getSkillActionDataDict()->objectForKey(actionDataIndex);
    m_pSkillAction->setSkillActionData(actionData);
    m_pSkillAction->setSkill(this);// 设置技能动作的父节点 
}

void Skill::setNextSkillAction()
{
    if (m_u8ActionDataIndex < m_pSkillData->getActionIndexArray()->count() - 1)
    {
        m_u8ActionDataIndex ++;
        CC_SAFE_DELETE(m_pSkillAction);
        m_pSkillAction = new SkillAction();
        int actionDataIndex = ((CCString *)m_pSkillData->getActionIndexArray()->objectAtIndex(m_u8ActionDataIndex))->intValue();
        SkillActionData *actionData = (SkillActionData *)GlobalData::getInstance()->getSkillActionDataDict()->objectForKey(actionDataIndex);
        m_pSkillAction->setSkillActionData(actionData);
        m_pSkillAction->setSkill(this);// 设置技能动作的父节点 
    }
    else
    {
        m_bSkillOver = true;
    }
}
