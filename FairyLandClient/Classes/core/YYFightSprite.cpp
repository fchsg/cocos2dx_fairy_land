//
//  YYFightSprite.cpp
//  XJ
//
//  Created by fuchenhao on 13-4-26. 
//
//

#include "YYFightSprite.h"
#include "../common/Common.h"
#include "YYConfig.h"
#include "SimpleAudioEngine.h"
#include "YYSpriteLayer.h"
#include "YYScene.h"
#include "YYController.h"
#include "YYPlayer.h"
#include "YYControlUILayer.h"
#include "YYSkill.h"
#include "YYSkillData.h"
#include "YYSpriteData.h"
#include "YYSpriteAttribute.h"
#include "../animation/YYRect.h"

USING_NS_CC;

YYFightSprite::YYFightSprite(void)
:m_pAttackable(false)
,m_pAttackIntervalCounter(NULL)
,m_nDamageCounter(0)
,m_nDamageFrameIndex(0)
,m_u16Tenacity(0)
,m_n8AttackIndex(0)
,m_nTempOffsetY(0)
,m_nFloatCounter(0)
,m_u16ConfirmBehaviorCounter(0)
,m_n8CurrentBehaviorIndex(0)
,m_u8AttackAmount(0)
,m_uCurrentHP(0)
,m_uCurrentMP(0)
,m_n16OffsetXBySkill(0)// 施放技能的动作附加的位移 
,m_fSlope(0)
,m_n16OwnerID(-1)
,m_bAkinesia(false)// 可行动（可发起攻击） 
,m_bImmovability(false)// 不能移动 
,m_bRunAround(false)// 乱窜
,hittedStatus(kHittedNone)
,m_pTargetSprite(NULL)
,m_pSkill(NULL)
,m_pSpriteAttribute(NULL)
,m_pHatredPointDictionary(NULL)
,m_bTrialPause(false)
{
    m_pHatredPointDictionary = new CCDictionary();
    m_pEffectIDArray = new CCArray(10);
    m_pFootmanIDArray = new CCArray(10);
    m_pSkillArray = new CCArray(3);
    m_pScene = Controller::getInstance()->getScene();
    m_bAttackable = true;
    for (int i = 0; i < sizeof(m_n8CurrentLayerNumber) / sizeof(m_n8CurrentLayerNumber[0]); i++)
    {
        m_n8CurrentLayerNumber[i] = 0;
    }
}

YYFightSprite::~YYFightSprite(void)
{
    CC_SAFE_DELETE(m_pSkillArray);
    CC_SAFE_DELETE(m_pHatredPointDictionary);
    CC_SAFE_DELETE(m_pAttackable);
    CC_SAFE_DELETE(m_pAttackIntervalCounter);
    CC_SAFE_DELETE(m_pFootmanIDArray);
    CC_SAFE_DELETE(m_pEffectIDArray);
}

void YYFightSprite::update(void)
{
    YYSprite::update();
    if (!m_bPause)
    {
        updateSkillAction();
    }
    updateDebuff();
    updateDOT();
}

#define DOT_MAX_LAYERS  5 // dot最大层数 
#define TAG_DEBUFF_INITIAL_VALUE 100// 不良效果起始标签位 
#define TAG_DOT_INITIAL_VALUE 100// 持续伤害特效起始标签位 

void YYFightSprite::addDebuff(int type,int duration,int value)
{
    // 替换相同的debuff 
    for(vector<Debuff>::iterator it = m_vDebuff.begin(); it != m_vDebuff.end(); it++)
    {
        if (it->type == type)
        {// 已有不良状态与新加不良状态相同 
            it->duration = duration;// 更新持续时间 
            if (type == kDebuffTieDown)
            {// 更新属性的变更 
                m_n16Speed += it->value;
                m_n16Speed -= value;
            }
            YYSprite * sprite = (YYSprite *)getChildByTag(TAG_DEBUFF_INITIAL_VALUE + type);
            CC_ASSERT(sprite);
            sprite->setDuration(duration);
            return;
        }
    }
    
    // 添加新的不良状态 
    Debuff debuff;
    debuff.type = type;
    debuff.duration = duration;
    debuff.value = value;
    switch (type)
    {
        case kDebuffStun:// 无法移动且无法行动 
            m_bImmovability = true;
            m_bAkinesia = true;
            break;
        case kDebuffFrozen:// 无法移动 
            m_bImmovability = true;
            break;
        case kDebuffFear:// 乱窜 
            m_bRunAround = true;
            break;
        case kDebuffCilence:// 无法行动 
            m_bAkinesia = true;
            break;
        case kDebuffTieDown:// 减速 
            m_n16Speed -= value;
            break;
    }
    m_vDebuff.push_back(debuff);
    
    CCString *fileName = CCString::createWithFormat("debuff%d",type);
    YYSprite *sprite = YYSprite::createWithActionSet(fileName->getCString(), fileName->getCString(), kDirTypeEffect);
    sprite->setAction(0,true);
    sprite->setTag(TAG_DEBUFF_INITIAL_VALUE + type);
    sprite->setDuration(duration);
    sprite->setDeleteCondition(DELETE_COUNTDOWN_TO_THE_END);
    sprite->setSpriteType(SPRITE_TYPE_DEBUFF);
    if (type == kDebuffStun || type == kDebuffFear || type == kDebuffCilence)
    {// 眩晕，恐惧，沉默的特效在头上 
        sprite->setPosition(ccp(0,getSize().height + 50));
    }
    addChild(sprite,15);
    getYYSpriteChildren()->addObject(sprite);
}

void YYFightSprite::addDot(int type,int duration,int value)
{
    if (m_n8CurrentLayerNumber[type - 1] > 0)
    {
        YYSprite * sprite = (YYSprite *)getChildByTag(TAG_DOT_INITIAL_VALUE + type);
        CC_ASSERT(sprite);
        sprite->setDuration(duration);
    }
    else
    {
        CCString *fileName = CCString::createWithFormat("dot%d",type);
        YYSprite *sprite = YYSprite::createWithActionSet(fileName->getCString(), fileName->getCString(), kDirTypeEffect);
        sprite->setAction(0,true);
        sprite->setTag(TAG_DOT_INITIAL_VALUE + type);
        sprite->setDuration(duration);
        sprite->setDeleteCondition(DELETE_COUNTDOWN_TO_THE_END);
        sprite->setSpriteType(SPRITE_TYPE_DOT);
        sprite->setPosition(ccp(0,getSize().height / 2));
        addChild(sprite,15);
        getYYSpriteChildren()->addObject(sprite);
    }
    
    if (m_n8CurrentLayerNumber[type - 1] < DOT_MAX_LAYERS)
    {// 添加新的持续伤害特效 
        m_n8CurrentLayerNumber[type - 1]++;
        DOT dot;
        dot.type = type;
        dot.duration = duration;
        dot.DPS = value;
        dot.layerIndex = m_n8CurrentLayerNumber[type - 1];
        m_vDOT.push_back(dot);
    }
    else
    {// 替换最早的持续伤害特效 
        vector<DOT>::iterator dot;
        for(vector<DOT>::iterator it = m_vDOT.begin(); it != m_vDOT.end(); it++)
        {
            if (it->type == type)
            {// 已有持续伤害特效与新加伤害特效相同 
                it->layerIndex --;
                if (it->layerIndex == 0)
                {
                    dot = it;
                }
            }
        }
        dot->duration = duration;
        dot->layerIndex = DOT_MAX_LAYERS;
    }
}

void YYFightSprite::updateDebuff()
{
    m_bImmovability = false;
    m_bAkinesia = false;
    m_bRunAround = false;
	for(vector<Debuff>::iterator it = m_vDebuff.begin();it != m_vDebuff.end();)
    {
        if (it->duration > 0)
        {
            it->duration--;
            switch (it->type)
            {
                case kDebuffStun:// 无法移动且无法行动 
                    m_bImmovability = true;
                    m_bAkinesia = true;
                    break;
                case kDebuffFrozen:// 无法移动 
                    m_bImmovability = true;
                    break;
                case kDebuffFear:// 乱窜 
                    m_bRunAround = true;
                    break;
                case kDebuffCilence:// 无法行动 
                    m_bAkinesia = true;
                    break;
            }
            it++;
        }
        else
        {
            if (it->type == kDebuffFrozen)
            {// 冰冻结束特效 
                CCString *fileName = CCString::createWithFormat("debuff%d",it->type);
                YYSprite *sprite = YYSprite::createWithActionSet(fileName->getCString(), fileName->getCString(), kDirTypeEffect);
                sprite->setAction(1);
                sprite->setSpriteType(SPRITE_TYPE_DEBUFF_OVER);
                sprite->setDeleteCondition(DELETE_ACTION_OVER);
                addChild(sprite,-1);
                getYYSpriteChildren()->addObject(sprite);
            }
            else if (it->type == kDebuffTieDown)
            {
                m_n16Speed += it->value;
            }
            it = m_vDebuff.erase(it);
        }
    }
}

void YYFightSprite::updateDOT()
{
    for(vector<DOT>::iterator it = m_vDOT.begin(); it != m_vDOT.end();)
    {
        if (it->duration > 0)
        {
            if (it->duration % FRAME_PER_SECOND == 0)
            {
                if (m_uCurrentHP > it->DPS)
                {
                    showDamage(it->DPS, false);
                    m_uCurrentHP -= it->DPS;
                }
                else
                {
                    showDamage(m_uCurrentHP, false);
                    m_uCurrentHP = 0;
                    switchAction(ACTION_HITTED);
                    m_n8CurrentLayerNumber[it->type-1]--;
                    it = m_vDOT.erase(it);
                    break;
                }
            }
            it->duration--;
            it++;
        }
        else
        {
            m_n8CurrentLayerNumber[it->type-1]--;
            it = m_vDOT.erase(it);
        }
    }
}

void YYFightSprite::switchAction(uint16_t actionIndex)
{
    if (getActionIndex() == actionIndex && getActionStatus() != kActionAttack)
    {
        return;
    }
    setAction(actionIndex, false);
}

/**
 *  闪避
 */
void YYFightSprite::dodge()
{
    CCSprite * label = CCLabelTTF::create(g_sAvoid, "Helvetica", 30);
    ccColor3B color = {10,245,10};
    label->setColor(color);
    label->setPosition(ccpAdd(ccp(RANDOM_INT(-5, 5),this->getSize().height * fabs(this->getScaleX()) + 10),getPosition()));
    getParent()->addChild(label,ABOVE_SPRITE_LAYER);
    ((YYSpriteLayer *)getParent())->getDodgeEffectArray()->addObject(label);
    ((YYSpriteLayer *)getParent())->getDodgeEffectArray()->addObject(CCInteger::create(0));/* 显示计数 */
}

/**
 *  显示伤害数值
 */
void YYFightSprite::showDamage(int damagePoint,bool isCritical)
{
    CCPoint position = ccpAdd(ccp(RANDOM_INT(-5, 5),100),getPosition());
//    const char * temp = CCString::createWithFormat("%d",damagePoint)->getCString();
//    CCLabelAtlas * node;
    std::string filePath;
    if (getSpriteID() == m_pScene->getSpriteLayer()->getRoleByControlled()->getSpriteID())
    {
        filePath = getFilePath("num_5",kDirTypePic,kFileSuffixPng);
//        node = CCLabelAtlas::create(temp,filePath.c_str(),35,38,'0');

    }
    else
    {
        if (isCritical)
        {
            filePath = getFilePath("num_1",kDirTypePic,kFileSuffixPng);
//            node = CCLabelAtlas::create(temp,filePath.c_str(),44,50,'0');/* 按ascii排列顺序排在9后面的是:;对应图片里的-+ */
        }
        else
        {
            filePath = getFilePath("num_0",kDirTypePic,kFileSuffixPng);
//            node = CCLabelAtlas::create(temp,filePath.c_str(),33,43,'0');/* 按ascii排列顺序排在9后面的是:;对应图片里的-+ */
        }
    }
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage(filePath.c_str());
    CCSpriteBatchNode * node = Controller::getInstance()->drawNumber(damagePoint, texture, position, ccp(0.5,0.5), 0, ((YYSpriteLayer *)getParent()), 1);
//   node->setPosition(position);
//   ((YYSpriteLayer *)getParent())->addChild(node,1);
    ((YYSpriteLayer *)getParent())->getDamageEffectArray()->addObject(node);
    ((YYSpriteLayer *)getParent())->getDamageEffectArray()->addObject(CCInteger::create(0));/* 显示计数 */
    ((YYSpriteLayer *)getParent())->getDamageEffectArray()->addObject(CCBool::create(isCritical));
}

int YYFightSprite::calcDamagePoint(YYFightSprite * target,float damageFactor1/* 物理攻击系数 */, float damageFactor2/* 魔法攻击系数 */,int extraDamage,float skillDamageFactor)
{
    int dp = MAX(getSpriteAttribute()->getSpellAttackPoint() * damageFactor2 + getSpriteAttribute()->getAttackPoint()* damageFactor1  + extraDamage +
                 getSpriteAttribute()->getSkillDamagePoint() * skillDamageFactor - target->getSpriteAttribute()->getDefencePoint(), 1);
    dp = dp * RANDOM_INT(95, 105) / 100;
    return dp;
}

void YYFightSprite::die()
{
    m_bDeleted = true;
}

bool YYFightSprite::isCritical()
{
    float criticalOdds = MIN((float)getSpriteAttribute()->getCriticalLevel() / (getSpriteAttribute()->getCriticalLevel() + getSpriteAttribute()->getCriticalConstant()) + BASIC_CRITICAL_ODDS, 45);
    int odds = RANDOM_INT(0, 100);
    if (odds < criticalOdds) {
        return true;
    }
    return false;
}

bool YYFightSprite::isDodge()
{
    float dodgeOdds = MIN((float)getSpriteAttribute()->getDodgeLevel() / (getSpriteAttribute()->getDodgeLevel() + getSpriteAttribute()->getDodgeConstant()) + getSpriteAttribute()->getBasicDodgeOdds(), 35);
    int odds = RANDOM_INT(0, 100);
    if (odds < dodgeOdds) {
        return true;
    }
    return false;
}

bool YYFightSprite::isHit()
{
    float hitOdds = (float)getSpriteAttribute()->getHitLevel() / (getSpriteAttribute()->getHitLevel() + getSpriteAttribute()->getHitConstant()) + getSpriteAttribute()->getBasicHitOdds();
    hitOdds = hitOdds > 100 ? 100 : hitOdds;
    
    int odds = RANDOM_INT(0, 100);
    if (odds < hitOdds) {
        return true;
    }
    return false;
}

#pragma mark 是否在攻击范围内 

bool YYFightSprite::isInRangeOfAttack(YYFightSprite * attacker,YYRect * rect)
{
    CCRect attackRect = CCRectMake(attacker->getPositionX() + (attacker->isFaceToRight() ? rect->getX() : (-rect->getX() - rect->getWidth())), attacker->getPositionY() + rect->getY(), rect->getWidth(), rect->getHeight());
    CCRect bodyRect = CCRectMake(getPositionX() + getSpriteData()->getRangeOfBody().getMinX(), getPositionY() + getSpriteData()->getRangeOfBody().getMinY(), getSpriteData()->getRangeOfBody().size.width, getSpriteData()->getRangeOfBody().size.height);
    return attackRect.intersectsRect(bodyRect);
}

#pragma mark 处理对目标造成的伤害及不良状态 

void YYFightSprite::damageTarget(YYFightSprite * target,SkillData * skillData ,const char *hittedEffectSource)
{
    // 设置bossUI 
    if (getSpriteType() == SPRITE_TYPE_BOSS)
    {
        m_pScene->getControlUILayer()->setBoss(this);
    }
    else if(target->getSpriteType() == SPRITE_TYPE_BOSS)
    {
        m_pScene->getControlUILayer()->setBoss(target);
    }
    
    if (!target->m_bAttackable)// 目标不可被攻击 
    {
        return;
    }
    if (target->hittedStatus == kHittedNone)
    {// 标识第一次受击
        target->hittedStatus = kHittedFirst;
    }
    if(!isHit())
    {
        target->dodge();
        return;
    }
    if(target->getSpriteType() == SPRITE_TYPE_PLAYER)
    {
        const std::string filePath = getFilePath(ATTACKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
    }
    // 计算并显示伤害值
    int damagePoint = calcDamagePoint(target,skillData ?  skillData->getAPCoefficient() : 1,skillData ? skillData->getSAPCoefficient() : 1,skillData ? skillData->getAddedDamage() : 0,skillData ? skillData->getSkillDamageCoefficient() : 0);
    //试炼
    if (Controller::getInstance()->isTrial())
    {
        damagePoint = (int)damagePoint*0.1f;
    }
    if (damagePoint < 1)
    {
        damagePoint = 1;
    }
    bool bCritical = isCritical();
    if (bCritical)
    {
        damagePoint = damagePoint << 1;
    }
    if ( Controller::getInstance()->getDungeonType() == kWorldBoss && target->getSpriteType() == SPRITE_TYPE_BOSS) // 活动 世界boss BOSS伤害显示"命中"
    {
        
    }
    else
    {
        target->showDamage(damagePoint, bCritical);
    }
    
    if(Controller::getInstance()->getDungeonType() == kWorldBoss && target->getSpriteType() == SPRITE_TYPE_BOSS) // 活动 世界boss 记录BOSS被攻击值
    {
        m_pScene->getSpriteLayer()->setWorldBossDamageHP(m_pScene->getSpriteLayer()->getWorldBossDamageHP() + damagePoint);
        CCLOG("-------- m_nWorldBossDamageHP %d",m_pScene->getSpriteLayer()->getWorldBossDamageHP());
    }
    
    target->setCurrentHP(target->getCurrentHP() >= damagePoint ? target->getCurrentHP() - damagePoint : 0);// 设置被攻击目标当前血量 
    
    // 攻击者记录活着的被攻击者对自己的仇恨值 
    if (target->getCurrentHP() > 0)
    {
        YYFightSprite * fs = NULL;
        if(getSpriteType() == SPRITE_TYPE_SKILL_EFFECT
           || getSpriteType() == SPRITE_TYPE_CATAPULT_EFFECT
           || getSpriteType() == SPRITE_TYPE_LINK_EFFECT)
        {
            YYFightSprite *owner = (YYFightSprite *)((YYSpriteLayer *)getParent())->getSpriteDictionary()->objectForKey(m_n16OwnerID);
            if (owner && owner->getCurrentHP() > 0)
            {
                fs = owner;
            }
        }
        else if(getSpriteType() == SPRITE_TYPE_BOSS
                || getSpriteType() == SPRITE_TYPE_MONSTER
                || getSpriteType() == SPRITE_TYPE_PLAYER
                || getSpriteType() == SPRITE_TYPE_PET
                || getSpriteType() == SPRITE_TYPE_TRIAL)
        {
            fs = this;
        }
        if (fs)
        {
            int hatredPoint = 0;
            CCInteger * hatred = (CCInteger *)fs->m_pHatredPointDictionary->objectForKey(target->getSpriteID());
            if (hatred)
            {
                hatredPoint = hatred->getValue();
            }
            fs->m_pHatredPointDictionary->setObject(CCInteger::create(hatredPoint + damagePoint + (skillData != NULL ? skillData->getHatredPoint() : 0)), target->getSpriteID());
        }
    }
    
    // 被攻击对象是玩家控制角色,统计关卡内被击次数及失血量 
    if (target->getSpriteID() == m_pScene->getSpriteLayer()->getRoleByControlled()->getSpriteID())
    {
        m_pScene->setHPDecrement(m_pScene->getHPDecrement() + damagePoint);
        m_pScene->setHittedAmount(m_pScene->getHittedAmount() + 1);
    }
    
    // 攻击者为自身 
    if (m_n16OwnerID == m_pScene->getSpriteLayer()->getRoleByControlled()->getSpriteID())
    {
        m_pScene->setStageDamage(m_pScene->getStageDamage() + damagePoint);// 统计关卡内造成的总伤害 
        m_pScene->getControlUILayer()->calcCombo(1);// 增加连击计数 
    }
    
    // 受击特效 
    YYSprite * sprite = YYSprite::create();
    if (hittedEffectSource)
    {
        sprite->initWithActionSet(hittedEffectSource, kDirTypeSprite);
    }
    else
    {
        sprite->initWithActionSet("p0-s11", "p0-s11", kDirTypeSprite);
    }
    sprite->setAction(0, false);
    sprite->setPosition(target->getPosition());
    getParent()->addChild(sprite,ABOVE_SPRITE_LAYER);
    m_pScene->getSpriteLayer()->getHitEffectArray()->addObject(sprite);
    
    // 是否可切换受伤动作 hp减为0可直接切换受击动作之后切换死亡动作，hp大于0需判断韧性值以及当前动作是否可以被打断 
    bool isSwitchInjureAction = true;
    if (target->getActionStatus() == kActionDie || target->getActionStatus() == kActionInjured)
    {// 死亡状态、受伤状态不可更改 
        isSwitchInjureAction = false;
    }
    else
    {
        if (target->getCurrentHP() > 0)
        {
            int tenacityConsume = 0;// 计算受攻击目标的韧性值 
            if (skillData)
            {
                tenacityConsume = skillData->getTargetReducedTenacity();
            }
            else
            {
                tenacityConsume = ((CCString *)getSpriteData()->getTargetReducedTenacityArray()->objectAtIndex(m_n8AttackIndex))->intValue();
            }
            target->setTenacity(target->getTenacity() < tenacityConsume ? 0 :(target->getTenacity() - tenacityConsume));
            if (target->getTenacity() > 0)
            {// 韧性大于0不可切换受伤动作 
                isSwitchInjureAction = false;
            }
            else
            {// 被攻击对象为玩家或boss时,判断是否打断其技能 
                if ((target->getSpriteType() == SPRITE_TYPE_PLAYER || target->getSpriteType() == SPRITE_TYPE_BOSS || target->getSpriteType() == SPRITE_TYPE_TRIAL)
                    && (target->getActionStatus() == kActionSkill || target->getActionStatus() == kActionAttack))
                {
                    if (!target->getSkill()->getSkillData()->isBreaded())
                    {// 技能不可被打断
                        isSwitchInjureAction = false;
                    }
                    else
                    {// 打断技能切换受伤动作 
                        target->getSkill()->getSkillAction()->stopSkillEffect();
                        target->setTenacity(target->getSpriteData()->getTenacity());
                    }
                }
                else
                {
                    target->setTenacity(target->getSpriteData()->getTenacity());
                }
            }
        }
    }
    // 不良状态
    if (target->getSpriteType() != SPRITE_TYPE_BOSS && target->getSpriteType() != SPRITE_TYPE_DESTRUCTIBLE_OBJECT)
    {
        if (skillData->getDebuffType() > 0)
        {
            target->addDebuff(skillData->getDebuffType() ,skillData->getDebuffDuration() ,skillData->getDebuffAddedValue());
        }
        if (skillData->getDOTType() > 0)
        {
            target->addDot(skillData->getDOTType(),skillData->getDOTDuration(),skillData->getDOTDamagePoint());
        }
    }
    // 附加特殊效果
    if (isSwitchInjureAction)
    {
        if (target->m_pSkill)
        {//清除目标正在释放的技能
            CC_SAFE_RELEASE_NULL(target->m_pSkill);
        }
        target->setMoving(false);
        if (skillData->isBeatedBack())
        {// 击退

            if (target->getSpriteData()->getSpriteIndex() == 3)
            {// 战士被击退调用特殊受伤动作 
                target->setAction(ACTION_HURTED,false);
            }
            else
            {
                target->setAction(ACTION_HITTED,false);
            }
            target->setMoving(false);
            target->setActionStatus(kActionInjured);
            if (target->getSpriteType() != SPRITE_TYPE_DESTRUCTIBLE_OBJECT)
            {
                target->setPosition(ccpAdd(target->getPosition(), ccp(skillData->getBeatedBackDistance() * (target->isFaceToRight() ? -1 : 1),0)));// 后退 
            }
        }
        else
        {
            target->setAction(ACTION_HITTED,false);
            target->setMoving(false);
            target->setActionStatus(kActionInjured);
        }
    }
}

void YYFightSprite::createSkillWithData(SkillData *skillData)
{
    m_pSkill = Skill::create();
    m_pSkill->retain();
    m_pSkill->initWithSkillData(skillData);
    m_pSkill->setOwner(this);
}

#pragma mark 设置技能对应的动作 

void YYFightSprite::setSkillAction()
{
    SkillAction * skillAction = m_pSkill->getSkillAction();
    int index = 0;
    for (int i = 0; i < skillAction->getSkillActionData()->getSpriteIndexArray()->count(); i++)
    {
        int spriteIndex = ((CCString *)skillAction->getSkillActionData()->getSpriteIndexArray()->objectAtIndex(i))->intValue();
        if (spriteIndex == m_pSpriteData->getSpriteIndex())
        {
            index = i;
            break;
        }
    }
    int actionIndex = ((CCString *)skillAction->getSkillActionData()->getActionIndexArray()->objectAtIndex(index))->intValue();
    skillAction->setActionIndex(actionIndex);
    bool isLoop = skillAction->getSkillActionData()->getDuration() > 0;
    if ((getSpriteType() == SPRITE_TYPE_PLAYER || getSpriteType() == SPRITE_TYPE_TRIAL) && (actionIndex == ACTION_ATTACK_0 || actionIndex == ACTION_ATTACK_1 || actionIndex == ACTION_ATTACK_2))
    {
        uint8_t ProfessionIndex = Controller::getInstance()->getProfessionIndex();
        switch (ProfessionIndex)
        {
            case 2: //法师
            {
                const std::string filePath = getFilePath(ATTACK2_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                PLAY_EFFECT(filePath.c_str());
            }
                break;
            case 3: //战士
            {
                const std::string filePath = getFilePath(ATTACK3_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                PLAY_EFFECT(filePath.c_str());
            }
                break;
            default:
                break;
        }
    }
    setAction(actionIndex,isLoop);
}

void YYFightSprite::skillOver()
{
    CC_SAFE_RELEASE_NULL(m_pSkill);
    if (m_uCurrentHP > 0)  //boss被打死 monster为die的状态不改变
    {
        setActionStatus(kActionNormal);
        switchAction(ACTION_STAND0);
    }
    m_bMoving = false;
}

void YYFightSprite::updateSkillAction()
{
    if (m_pSkill)
    {
        SkillAction * skillAction = m_pSkill->getSkillAction();
        if (skillAction)
        {
            int currentFrameIndex = m_u16CurrentFrameIndex - getCurrentActionData()->getStartFrameIndex();
            // 设置技能动作对应的位移 
            if (skillAction->getSkillActionData()->getOffsetXPerFrame() > 0)
            {
                m_n16OffsetXBySkill = skillAction->getSkillActionData()->getOffsetXPerFrame();
                m_bMoving = true;
                m_u8MoveType = MOVE_BY_SKILL;
            }
            else if(skillAction->getSkillActionData()->getOffsetFrameIndexArray()->count() > 0)
            {
                for (int i = 0; i < skillAction->getSkillActionData()->getOffsetFrameIndexArray()->count(); i++)
                {
                    if(((CCString *)skillAction->getSkillActionData()->getOffsetFrameIndexArray()->objectAtIndex(i))->intValue() == m_nActionFrameIndex + 1)
                    {
                        m_n16OffsetXBySkill = ((CCString *)skillAction->getSkillActionData()->getOffsetXCurrentFrameArray()->objectAtIndex(i))->intValue();
                        m_bMoving = true;
                        m_u8MoveType = MOVE_BY_SKILL;
                        break;
                    }
                }
            }
            
            skillAction->addSkillEffect(currentFrameIndex);// 添加技能特效 
            skillAction->addSoundEffect(currentFrameIndex);// 添加技能动作对应的音效 
            // 设置技能动作对应的震屏特效 
            if (!m_pScene->isVibrate())
            {
                if (currentFrameIndex == skillAction->getSkillActionData()->getVibrateFrameIndex())
                {
                    m_pScene->vibrate(skillAction->getSkillActionData()->getVibrateDuration());
                }
            }
            if (m_bRepeatAction)// 动作循环播放
            {
                skillAction->update();
            }
            else
            {
                skillAction->setSkillActionOver(m_bActionOver);
            }
            
            if (skillAction->isSkillActionOver())
            {// 动作结束
                skillAction->stopSoundEffect();
                m_pSkill->setNextSkillAction();
                if (m_pSkill->isSkillOver())
                {
                    skillOver();
                }
                else
                {
                    setSkillAction();
                }
            }
        }
    }
}
