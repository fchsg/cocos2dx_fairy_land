//
//  YYSkillEffect.cpp
//  XJ
//
//  Created by fuchenhao on 13-7-19. 
//
//

#include "YYSkillEffect.h"
#include "../common/Common.h"

#include "YYSkillEffectData.h"
#include "YYSkillData.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYMonster.h"
#include "YYSpriteAttribute.h"
#include "YYController.h"
#include "YYScene.h"
#include "YYCatapultEffect.h"
#include "YYSpriteData.h"
#include "../animation/YYRect.h"
#include "YYGlobalData.h"

USING_NS_CC;

YYSkillEffect::YYSkillEffect()
:m_nFrameCounter(0)
,m_pSkillEffectData(NULL)
,m_pSkillData(NULL)
,m_bSpellAttack(false)
{
}

YYSkillEffect::~YYSkillEffect()
{
}

YYSkillEffect * YYSkillEffect::createWithData(SkillEffectData *skillEffectData, SkillData * skillData,YYFightSprite * owner)
{
    YYSkillEffect * skillEffect = YYSkillEffect::create();
    skillEffect->m_pSkillEffectData = skillEffectData;
    skillEffect->initWithActionSet(skillEffectData->getResource().c_str(), kDirTypeSprite);
    skillEffect->setSpriteType(SPRITE_TYPE_SKILL_EFFECT);
    skillEffect->setOwnerID(owner->getSpriteID());// 特效的主人 
    if (owner->getSpriteData()->getSpriteIndex() == 2)// 法师
    {
        skillEffect->m_bSpellAttack = true;
    }
    skillEffect->setSpriteAttribute(owner->getSpriteAttribute());// 继承主人的属性,以便进行伤害运算 
    skillEffect->setFaceToRight(owner->isFaceToRight());// 朝向 
    if (owner->getSpriteType() == SPRITE_TYPE_PLAYER)
    {
        skillEffect->setTargetType(kTargetTypeEnemy);// 针对敌方目标 
    }
//    else if(owner->getSpriteType() == SPRITE_TYPE_STAB)
//    {
//        skillEffect->setTargetType(kTargetTypeAll);// 地刺针对所有目标
//    }
    else
    {
        skillEffect->setTargetType(kTargetTypeFriend);// 针对友方目标 
    }
    skillEffect->setSkillData(skillData);// 特效对应的技能数据 
    //子弹类型 旋转角度的处理 
    if (skillEffectData->getType() == kTypeBullet)
    {
        if (skillEffect->isFaceToRight())
        {
            skillEffect->setRotation(-skillEffectData->getAngleOfDeflection());
        }
        else
        {
            skillEffect->setRotation(skillEffectData->getAngleOfDeflection());
        }
    }
    else
    {
        if (skillEffectData->getDuration() != 0)
        {
            skillEffect->setDuration(skillEffectData->getDuration());
            skillEffect->setDeleteCondition(DELETE_COUNTDOWN_TO_THE_END);
        }
        else
        {
            skillEffect->setDeleteCondition(DELETE_ACTION_OVER);
        }
    }
    //特效位置相关 
    if(skillEffectData->getDisplayMode() == kDisplayNormal
       || skillEffectData->getDisplayMode() == kDisplayFollowOwner)// 正常类型，设置在主人的当前位置上
    {
        if (owner)
        {
            skillEffect->setPosition(owner->getPosition());
        }
    }
    else if (skillEffectData->getDisplayMode() == kDisplayScreenCenter)// 居中显示 
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        skillEffect->setPosition(ccpAdd(Controller::getInstance()->getScene()->getScreenDatumPoint(),ccp(winSize.width / 2,winSize.height / 2 - 200)));
    }
    else if(skillEffectData->getDisplayMode() == kDisplayRandom)//随机位置 
    {
        int bottom = 100;
        int rand = RANDOM_INT(0, 8);
        int W = STANDARD_SCENE_WIDTH * 2 / 3;// 在亮屏幕宽的场景内随机 
        int H = (LOGIC_LAYER_HEIGHT - bottom) / 3;
        int x = rand % 3 * W + RANDOM_INT(50, W - 50);
        int y = bottom + rand / 3 * H + RANDOM_INT(10, H - 50);
        skillEffect->setPosition(ccp(x,y));
    }
    skillEffect->setAction(0, skillEffectData->getDuration()!=0 || skillEffectData->getType() == kTypeBullet);// 设置持续播放的特效或子弹类特效循环播放特效动画
    return skillEffect;
}

YYSkillEffect * YYSkillEffect::createWithData(SkillEffectData *skillEffectData, YYSkillEffect *skillEffect)
{
    YYSkillEffect * nextSkillEffect = YYSkillEffect::create();
    nextSkillEffect->m_pSkillEffectData = skillEffectData;
    nextSkillEffect->initWithActionSet(skillEffectData->getResource().c_str(), kDirTypeSprite);
    nextSkillEffect->setSpriteType(SPRITE_TYPE_SKILL_EFFECT);
    nextSkillEffect->setSpriteAttribute(skillEffect->getSpriteAttribute());// 继承主人的属性,以便进行伤害运算 
    nextSkillEffect->setFaceToRight(skillEffect->isFaceToRight());// 朝向 
    YYFightSprite *owner = (YYFightSprite *)((YYSpriteLayer *)skillEffect->getParent())->getSpriteDictionary()->objectForKey(skillEffect->getOwnerID());
    if (owner)
    {
        nextSkillEffect->setOwnerID(skillEffect->getOwnerID());// 特效的主人 
    }
    nextSkillEffect->setTargetType(skillEffect->getTargetType());
    nextSkillEffect->setSkillData(skillEffect->getSkillData());// 特效对应的技能数据 
    //子弹类型 旋转角度的处理 
    if (skillEffectData->getType() == kTypeBullet)
    {
        if (nextSkillEffect->isFaceToRight())
        {
            nextSkillEffect->setRotation(-skillEffectData->getAngleOfDeflection());
        }
        else
        {
            nextSkillEffect->setRotation(skillEffectData->getAngleOfDeflection());
        }
    }
    else
    {
        if (skillEffectData->getDuration() > 0)
        {
            nextSkillEffect->setDuration(skillEffectData->getDuration());
            nextSkillEffect->setDeleteCondition(DELETE_COUNTDOWN_TO_THE_END);
        }
        else
        {
            nextSkillEffect->setDeleteCondition(DELETE_ACTION_OVER);
        }
    }
    nextSkillEffect->setPosition(skillEffect->getPosition());
    nextSkillEffect->setAction(0, skillEffectData->getDuration() || skillEffectData->getType() == kTypeBullet);// 设置持续播放的特效或子弹类特效循环播放特效动画 
    return nextSkillEffect;
}

void YYSkillEffect::update()
{
    YYSprite::update();
    // 设置特效攻击范围 
    attackRect = CCRectMake(getPositionX() + (isFaceToRight() ? m_pSkillEffectData->getCollisionRect().origin.x : (-m_pSkillEffectData->getCollisionRect().origin.x - m_pSkillEffectData->getCollisionRect().size.width)), getPositionY() + m_pSkillEffectData->getCollisionRect().origin.y, m_pSkillEffectData->getCollisionRect().size.width, m_pSkillEffectData->getCollisionRect().size.height);
    
    if (m_pSkillEffectData->getType() == kTypeBullet)
    {// 子弹类飞行轨迹 
        float radian = m_pSkillEffectData->getAngleOfDeflection() != 0 ? M_PI * m_pSkillEffectData->getAngleOfDeflection() / 180 : 0;// 弧度 
        float offsetX = (isFaceToRight() ? m_pSkillEffectData->getOffsetXPerFrame(): -m_pSkillEffectData->getOffsetXPerFrame()) * cos(radian);
        float offsetY = m_pSkillEffectData->getOffsetXPerFrame() * sin(radian);
        setPosition(ccpAdd(getPosition(),ccp(offsetX, offsetY)));
        // 飞出屏幕，删除 
        CCRect rect = CCRectMake(Controller::getInstance()->getScene() ->getScreenDatumPoint().x, Controller::getInstance()->getScene()->getScreenDatumPoint().y, CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height);
        if (!rect.intersectsRect(attackRect))
        {
            m_bDeleted = true;
            return;
        }
    }
    else
    {
        if (m_pSkillEffectData->getDisplayMode() == kDisplayFollowOwner)
        {// 跟随主人 
            YYFightSprite * owner = (YYFightSprite *)((YYSpriteLayer*)getParent())->getSpriteDictionary()->objectForKey(m_n16OwnerID);
            if (owner)
            {
                setPosition(owner->getPosition());
            }
        }
        
        if (m_bDeleted && m_n8DeleteCondition == DELETE_ACTION_OVER)
        {// 非持续性特效,动画播放完毕后删除 
            YYFightSprite * owner = (YYFightSprite *)((YYSpriteLayer*)getParent())->getSpriteDictionary()->objectForKey(m_n16OwnerID);
            if (owner)
            {
                if (m_pSkillEffectData->getType() == kTypeSummon && owner)
                {//召唤 
                    for (int i = 0; i < m_pSkillData->getSummonedSpriteNum(); i++)
                    {
                        CCArray *array = split_string(((CCString *)m_pSkillEffectData->getSummonedPositionArray()->objectAtIndex(i))->getCString(), ',');
                        int offsetX = ((CCString *)array->objectAtIndex(0))->intValue();
                        int offsetY = ((CCString *)array->objectAtIndex(1))->intValue();
                        CCPoint position = ccp(owner->getPositionX() + (owner->isFaceToRight() ? offsetX : - offsetX),owner->getPositionY() + offsetY);
                        ((YYSpriteLayer *)getParent())->summon(m_pSkillData->getSummonedSpriteIndex(),position,m_pSkillData->getSummonedSpriteDuration(),m_n16OwnerID);
                        
                    }
                }
                if (m_pSkillEffectData->getFollowUpEffectIndex() > 0)// 有后续特效 
                {
                    SkillEffectData * skillEffectData = (SkillEffectData *)GlobalData::getInstance()->getSkillEffectDataDict()->objectForKey(m_pSkillEffectData->getFollowUpEffectIndex());
                    YYSkillEffect *skillEffect = YYSkillEffect::createWithData(skillEffectData, this);
                    int layerIndex = skillEffect->getSkillEffectData()->getLayerIndex();/* 1遮挡人 -1被人遮挡 */
                    Controller::getInstance()->getScene()->getSpriteLayer()->addChild(skillEffect,layerIndex);
                    Controller::getInstance()->getScene()->getSpriteLayer()->addElementToSpriteDictionary(skillEffect);// 设置精灵ID并将精灵添加至精灵容器 
                    owner->getEffectIDArray()->addObject(CCInteger::create(skillEffect->getSpriteID()));
                }
            }
            return;
        }
    }
    updateDamage();
}

void YYSkillEffect::updateDamage()
{
    bool isDamage = false;// 判断是否可造成伤害 
    if (m_pSkillEffectData->getType() == kTypeBullet)
    {
        isDamage = true;
    }
    else if(m_pSkillEffectData->getType() == kTypeNormal)
    {
        int damageAmount = m_pSkillEffectData->getDamageAmount(); // 共造成几次伤害 
        if (damageAmount != 0)// 该特效可造成伤害 
        {
            if(damageAmount == -1
               || (damageAmount > 0 && m_nDamageCounter < damageAmount))// 伤害计数小于可造成伤害的总次数 
            {
                if (getCurrentFrameIndex() - m_pCurrentActionData->getStartFrameIndex() >= m_pSkillEffectData->getDamageFrameIndex() - 1)
                {// 从技能特效的第几帧起开始造成伤害(动作列表里的帧) 
                    if ( m_nDamageFrameIndex == 0)
                    {// 刚刚切换的帧为起始伤害帧 
                        m_nDamageFrameIndex = m_nFrameCounter;// 记录当前实际帧数 
                        m_nDamageCounter ++;
                        isDamage = true;
                    }
                    else
                    {
                        int interval = m_pSkillEffectData->getDamageInterval();// 每间隔几帧造成一次伤害(游戏运行的帧间隔50ms) 
                        if (interval == 0 || (m_nFrameCounter - m_nDamageFrameIndex) % interval == 0)
                        {
                            m_nDamageCounter ++;
                            isDamage = true;
                        }
                    }
                }
            }
        }
    }
    if (isDamage)
    {
        int hurtCounter = 0;// 伤害计数 
        cocos2d::CCArray * targetArray = NULL;
        if(m_eTargetType == kTargetTypeEnemy)
        {
            targetArray = Controller::getInstance()->getScene()->getSpriteLayer()->getEnemyArray();
        }
        else if(m_eTargetType == kTargetTypeFriend)
        {
            targetArray = Controller::getInstance()->getScene()->getSpriteLayer()->getFriendlyRoleArray();
        }
        else if(m_eTargetType == kTargetTypeAll)
        {
            targetArray = Controller::getInstance()->getScene()->getSpriteLayer()->getFriendlyRoleArray();
            targetArray->addObjectsFromArray(Controller::getInstance()->getScene()->getSpriteLayer()->getEnemyArray());
        }
        CCObject *targetSprite = NULL;
        CCARRAY_FOREACH(targetArray,targetSprite)
        {
            if (targetSprite
                && ((YYFightSprite *)targetSprite)->isVisible()
                && ((YYFightSprite *)targetSprite)->getCurrentHP() > 0)
            {// 目标可见且生命值大于0 
                CCRect bodyRect = CCRectMake(((YYFightSprite *)targetSprite)->getPositionX() + ((YYFightSprite *)targetSprite)->getSpriteData()->getRangeOfBody().getMinX(), ((YYFightSprite *)targetSprite)->getPositionY() + ((YYFightSprite *)targetSprite)->getSpriteData()->getRangeOfBody().getMinY(), ((YYFightSprite *)targetSprite)->getSpriteData()->getRangeOfBody().size.width, ((YYFightSprite *)targetSprite)->getSpriteData()->getRangeOfBody().size.height);
                if (attackRect.intersectsRect(bodyRect))
                {// 技能特效命中目标且技能命中目标次数未达上限
                    damageTarget((YYFightSprite *)targetSprite,m_pSkillData, m_pSkillEffectData->getHittedEffectResource().c_str());
                    if (m_pSkillEffectData->getDuration() < 0)
                    {// 滚桶特殊处理
                        setDeleted(true);
                        YYFightSprite *owner = (YYFightSprite *)((YYSpriteLayer *)this->getParent())->getSpriteDictionary()->objectForKey(getOwnerID());
                        if (owner && owner->getSpriteType() == SPRITE_TYPE_ROLLING_BUCKET)
                        {
                            owner->setAction(ACTION_DIE);
                            owner->setMoving(false);
                        }
                    }
                    if (m_pSkillEffectData->getType() == kTypeBullet && m_pSkillEffectData->getBulletType() != kBulletTypePenetrate)
                    {// 非穿透的子弹类型特效，命中后消失 
                        setDeleted(true);
                        if (getSkillEffectData()->getBulletType() == kBulletTypeCatapult)
                        {// 弹射 
                            setDeleted(true);
                            YYCatapultEffect *catapultEffect = YYCatapultEffect::create();
                            ((YYSpriteLayer *)getParent())->addChild(catapultEffect,ABOVE_SPRITE_LAYER);
                            catapultEffect->initWithActionSet(m_pSkillEffectData->getResource().c_str(), kDirTypeSprite);
                            catapultEffect->setPosition(((YYFightSprite *)targetSprite)->getPosition());
                            catapultEffect->setSkillData(m_pSkillData);
                            catapultEffect->setOwnerID(m_n16OwnerID);
                            catapultEffect->setSkillEffectData(m_pSkillEffectData);
                            catapultEffect->setSpriteAttribute(m_pSpriteAttribute);
                            catapultEffect->setSpeed(m_pSkillEffectData->getOffsetXPerFrame());
                            catapultEffect->catapultToNextTarget((YYFightSprite *)targetSprite);
                            catapultEffect->setSpellAttack(m_bSpellAttack);
                            catapultEffect->setSpriteType(SPRITE_TYPE_CATAPULT_EFFECT);
                            ((YYSpriteLayer *)getParent())->addElementToSpriteDictionary(catapultEffect);
                            return;
                        }
                    }
                    hurtCounter ++;
                    if (hurtCounter >= m_pSkillData->getHurtNumber())
                    {
                        break;
                    }
                }
            }
        }
        if(m_eTargetType == kTargetTypeAll)
        {
            targetArray->removeObjectsInArray(Controller::getInstance()->getScene()->getSpriteLayer()->getEnemyArray());
        }
    }
    m_nFrameCounter++;
}
