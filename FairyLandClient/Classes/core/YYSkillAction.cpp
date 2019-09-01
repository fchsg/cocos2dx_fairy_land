//
//  YYSkillData.cpp
//  XJ
//
//  Created by fuchenhao on 13-7-19. 
//
//

#include "YYSkillAction.h"
#include "SimpleAudioEngine.h"
#include "YYSkillEffect.h"
#include "YYController.h"
#include "YYSpriteLayer.h"
#include "YYSkillData.h"
#include "YYSkill.h"
#include "YYScene.h"
#include "YYPlayer.h"
#include "YYGlobalData.h"

USING_NS_CC;

using namespace CocosDenshion;

SkillAction::SkillAction()
:m_u16Counter(0)
,m_nSoundID(-1)
,m_bSkillActionOver(false)
,m_pSkillActionData(NULL)
,m_pEffectArray(NULL)
,m_pSkill(NULL)
,m_bPlaySound(false)
{
    m_pEffectArray = new CCArray(2);
}

SkillAction::~SkillAction()
{
    stopSoundEffect();
    CC_SAFE_DELETE(m_pEffectArray);
}

bool SkillAction::init()
{
    return true;
}

#pragma mark 添加技能特效 

void SkillAction::addSkillEffect(int currentFrameIndex)
{
    for (int i = 0; i < m_pSkillActionData->getEffectDataArray()->count(); i ++)
    {
        int effectIndex = ((CCString *)m_pSkillActionData->getEffectDataArray()->objectAtIndex(i))->intValue();// 特效编号 
        // 判断待添加的特效是否已经被添加过 
        bool isAdded = false;
        for (int j = 0; j < m_pEffectArray->count(); j++)
        {
            YYSkillEffect * skillEffect = (YYSkillEffect *)m_pEffectArray->objectAtIndex(j);
            if (skillEffect->getSkillEffectData()->getIndex() == effectIndex)
            {
                isAdded = true;
                break;
            }
        }
        if (isAdded)
        {
            continue;
        }
        /* 未被添加过则判断当前帧是否为待添加特效的启动帧 */
        int startFrameIndex =((CCString *)m_pSkillActionData->getEffectStartFrameArray()->objectAtIndex(i))->intValue() - 1;
        if (currentFrameIndex == startFrameIndex)
        {
            SkillEffectData * skillEffectData = (SkillEffectData *)GlobalData::getInstance()->getSkillEffectDataDict()->objectForKey(effectIndex);
            int num = 1;
            bool isDrop = false;
            if (m_pSkill->getSkillData()->getSpecialType() == kSkillTypeDropDamage && skillEffectData->getDisplayMode() == kDisplayRandom) //落酸雨 
            {
                num = m_pSkill->getSkillData()->getTrapNum();
                isDrop = true;
            }
            for (int j = 0; j < num; j++)
            {
                YYSkillEffect * skillEffect = YYSkillEffect::createWithData(skillEffectData,m_pSkill->getSkillData(),m_pSkill->getOwner());
                int layerIndex = skillEffect->getSkillEffectData()->getLayerIndex();/* 1遮挡人 -1被人遮挡 */
                Controller::getInstance()->getScene()->getSpriteLayer()->addChild(skillEffect,layerIndex);
                Controller::getInstance()->getScene()->getSpriteLayer()->addElementToSpriteDictionary(skillEffect);// 设置精灵ID并将精灵添加至精灵容器 
                m_pEffectArray->addObject(skillEffect);
                m_pSkill->getOwner()->getEffectIDArray()->addObject(CCInteger::create(skillEffect->getSpriteID()));
            }
        }
    }
}

#pragma mark 技能动作被打断时删除非子弹类特效 

void SkillAction::stopSkillEffect()
{
    for (int i = 0; i < m_pEffectArray->count(); )
    {
        YYSkillEffect * skillEffect = (YYSkillEffect *)m_pEffectArray->objectAtIndex(i);
        if (skillEffect->getSkillEffectData()->getType() != kTypeBullet)
        {
            skillEffect->setDeleted(true);
            m_pEffectArray->removeObjectAtIndex(i);
        }
        else
        {
            i++;
        }
    }
}

void SkillAction::addSoundEffect(int currentFrameIndex)
{
    if (!m_pSkillActionData || m_pSkillActionData->getSoundEffectPath().compare("0") == 0)
    {
        return;
    }
    if (!m_bPlaySound)
    {
        std::string path = getFilePath(m_pSkillActionData->getSoundEffectPath(), kDirTypeSound, kFileSuffixWav);
        if(currentFrameIndex == m_pSkillActionData->getSoundFrameIndex() - 1)
        {
            m_bPlaySound = true;
            bool isLoop = m_pSkillActionData->isSoundEffectLoop();
            if(Config::getInstance()->isEffectPlay())
            {
               m_nSoundID = SimpleAudioEngine::sharedEngine()->playEffect(path.c_str(), isLoop);
            }
        }
    }
}

void SkillAction::stopSoundEffect()
{
    if (m_nSoundID > -1)
    {
        SimpleAudioEngine::sharedEngine()->stopEffect(m_nSoundID);
    }
}

void SkillAction::update()
{
    if(m_u16Counter < m_pSkillActionData->getDuration())
    {
        m_u16Counter++;
    }
    else
    {
        m_u16Counter = 0;
        m_bSkillActionOver = true;
        stopSoundEffect();
    }
}
