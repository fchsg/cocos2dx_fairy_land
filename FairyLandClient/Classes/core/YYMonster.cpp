//
//  YYMonster.cpp
//  XJ
//
//  Created by fuchenhao on 13-4-22. 
//
//

#include "../animation/YYRect.h"
#include "../common/Common.h"
#include "YYMonster.h"
#include "YYScene.h"
#include "YYController.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYSpriteAttribute.h"
#include "YYSkillEffect.h"

USING_NS_CC;

#define HORIZONTAL_SPEED 10
#define VERTICAL_SPEED 8

#define BLOOD_BAR_BG getFilePath("37", kDirTypePic, kFileSuffixPng)
#define BLOOD_BAR getFilePath("35", kDirTypePic, kFileSuffixPng)

#define OBLIGATE_WIDTH 12 // 血条预留宽度 

YYMonster::YYMonster(void)
:m_pBloodSpriteBatch(NULL)
,m_nBloodMaxWidth(0)
,m_nRecordHP(0)
,m_bArriveAtTheDestination(false)
,m_bBlocked(false)
,m_bFriendly(false)
,m_bSummoned(false)
,m_bWaitingForSwitchBehavior(false)
,m_bStartPursue(false)
,m_n16SummonedDuration(0)
,m_n16SummonedCounter(0)
,m_bWaitForAttack(false)
,m_pSkillCounterOfUsedArray(NULL)
{
}

YYMonster::~YYMonster()
{
    CC_SAFE_DELETE(m_pSkillCounterOfUsedArray);
}

void YYMonster::initFightData(SpriteData * spriteData,SpriteAttribute * spriteAttribute)
{
    m_pSpriteData = spriteData;
    m_pSpriteAttribute = spriteAttribute;
    m_uCurrentHP = m_pSpriteAttribute->getMaxHP();
    m_uCurrentMP = m_pSpriteAttribute->getMaxMP();
    m_u16Tenacity = m_pSpriteData->getTenacity();
    int count = spriteData->getAttackRectArray()->count();
    m_pAttackable = new bool[count];
    m_pAttackIntervalCounter = new uint16_t[count];
    for (int i = 0; i < count; i ++) {
        m_pAttackable[i] = true;
    }
    m_pSkillCounterOfUsedArray = new CCArray(m_pSpriteData->getNumOfUsedArray()->count());
    m_pSkillCounterOfUsedArray->addObjectsFromArray(m_pSpriteData->getNumOfUsedArray());
    m_u16ConfirmBehaviorCounter = m_pSpriteData->getConfirmBehaviorDuration();
    m_n16Speed = m_pSpriteData->getSpeed();
    if (getSpriteType() == SPRITE_TYPE_ROLLING_BUCKET)
    {
        m_nCurrentDisplacement = 0;
        m_nTotalDisplacement = m_pSpriteData->getTenacity();// 总位移
    }
}

void YYMonster::switchAction(uint16_t actionIndex)
{
    if (getActionIndex() == actionIndex && getActionStatus() != kActionAttack)
    {
        return;
    }
    setAction(actionIndex, false);
    if (m_pSpriteData->getSpriteIndex() == 301)
    {// 沙虫 
        if (actionIndex == ACTION_RUN)
        {
            m_bAttackable = false;
        }
        else
        {
            m_bAttackable = true;
        }
    }
}

void YYMonster::initBloodBar(CCPoint position)
{
    m_BloodPosition = position;
    std::string filePath = BLOOD_BAR_BG;
    CCSprite * sprite = CCSprite::create(filePath.c_str());
    m_nBloodMaxWidth = sprite->getContentSize().width;
    sprite->setPosition(m_BloodPosition);
    addChild(sprite,1);
    filePath = BLOOD_BAR;
    m_pBloodSpriteBatch = CCSpriteBatchNode::create(filePath.c_str());
    addChild(m_pBloodSpriteBatch,1);
    m_nRecordHP = -1;
}

void YYMonster::update(void)
{
    YYFightSprite::update();
    
    if (!m_bPause && m_bActionOver && !m_bRepeatAction)
    {
        switch (m_u8ActionIndex)
        {
            case ACTION_STAND0:
            case ACTION_RUN:
                m_u16CurrentFrameIndex = getCurrentActionData()->getStartFrameIndex();
                setFrame(m_u16CurrentFrameIndex);
                m_bActionOver = false;
                break;
            case ACTION_HITTED:
                setPosition(ccpAdd(getPosition(), ccp(0,-getTempOffsetY())));
                setTempOffsetY(0);
                if (getCurrentHP() <= 0)
                {
                    switchAction(ACTION_DIE);
                    setActionStatus(kActionDie);
                }
                else
                {  
                    switchAction(ACTION_STAND0);
                    setActionStatus(kActionNormal);
                }
                break;
            case ACTION_RUN1:// 沙虫钻地 
                setMoving(true);
                m_u8MoveType = MOVE_BY_SET_DESTINATION;
                switchAction(ACTION_RUN);
                break;
            case ACTION_RUN2:// 沙虫钻出 
                if (m_bWaitForAttack)
                {
                    m_bWaitForAttack = false;
                    attack();
                }
                else
                {
                    switchAction(ACTION_STAND0);
                }
                break;
            case ACTION_DIE:
                die();
                break;
            default:
                switchAction(ACTION_STAND0);
                setActionStatus(kActionNormal);
                break;
        }
    }
    if (m_uCurrentHP > 0)
    {// 活着 
        // 被召唤物 
        if (m_bSummoned)
        {
            if(m_n16SummonedDuration > 0)
            {
                if (m_n16SummonedCounter < m_n16SummonedDuration)//被召唤物存留时间计时 
                {
                    m_n16SummonedCounter ++;
                }
                else
                {
                    m_bDeleted = true;
                    return;
                }
            }
            YYFightSprite * owner = (YYFightSprite *)((YYSpriteLayer *)getParent())->getSpriteDictionary()->objectForKey(m_n16OwnerID);
            if (owner)
            {
                //出屏幕传送至主人身前 
                if (getPositionX() + getDefaultSize().width / 2 < m_pScene->getScreenDatumPoint().x)
                {
                    setPosition(owner->getPosition());
                }
            }
        }
        if (getSpriteType() != SPRITE_TYPE_DESTRUCTIBLE_OBJECT)// 宝箱、木桩等可破坏物品无行动AI 
        {
            if (!isLocked())
            {
                executeAI();
            }
            if (!m_bImmovability)
            {// 未限制移动 
                if (m_bMoving)
                {
                    move();
                    if (m_bArriveAtTheDestination || m_bBlocked)
                    {// 到达目的地或受阻碍停止位移 
                        m_bArriveAtTheDestination = false;
                        m_bBlocked = false;
                        if (m_pSpriteData->getSpriteIndex() == 301)
                        {// 沙虫钻出 
                            switchAction(ACTION_RUN2);
                        }
                        else
                        {
                            switchAction(ACTION_STAND0);
                            setActionStatus(kActionNormal);
                        }
                    }
                }
            }
        }
//        updateDamage(m_bFriendly ? ((YYSpriteLayer*)getParent())->getMoveableEnemyArray() : ((YYSpriteLayer*)getParent())->getFriendlyRoleArray());
    }
    if (getSpriteType() == SPRITE_TYPE_MONSTER && getChildByTag(TAG_BOSS_MARK))
    {
        ((YYSprite *)getChildByTag(TAG_BOSS_MARK))->updateAnimation();
    }
    if (getSpriteType() == SPRITE_TYPE_MONSTER || getSpriteType() == SPRITE_TYPE_PET)
    {
        updateStatusBar(getCurrentHP(), m_pSpriteAttribute->getMaxHP());
    }
}

void YYMonster::executeAI()
{
    countAttackInterval();//攻击间隔计算(冷却倒计时) 
    determineCurrentBehavior(hittedStatus == kHittedFirst);// 确定当前的行为 0站立原地 1追击目标敌人 2随机移动
    if (hittedStatus == kHittedFirst)
    {
        hittedStatus = kHittedNone;
    }
    
    if (m_bStartPursue)
    {// 开始追击 
        pursueAndAttackTarget();
    }
}

void YYMonster::move(void)
{
    if (getSpriteType() == SPRITE_TYPE_ROLLING_BUCKET)
    {// 滚桶
        if (m_nCurrentDisplacement <= m_nTotalDisplacement - m_n16Speed)
        {
            m_nCurrentDisplacement += m_n16Speed;
            if (isFaceToRight())
            {
                setPosition(getPosition() + ccp(m_n16Speed, 0));
                getNameLabel()->setPosition(getNameLabel()->getPosition() + ccp(m_n16Speed, 0));
            }
            else
            {
                setPosition(getPosition() - ccp(m_n16Speed, 0));
                getNameLabel()->setPosition(getNameLabel()->getPosition() - ccp(m_n16Speed, 0));
            }
        }
        else
        {// 达到最大位移后消失
            die();
        }
    }
    else if(getSpriteType() == SPRITE_TYPE_STAB)
    {
        return;
    }
    else
    {
        float offsetX = 0;
        float offsetY = 0;
        if (m_u8MoveType == MOVE_BY_RANDOM)
        {
            offsetX = m_n16Speed * cos(m_fSlope);
            offsetY = m_n16Speed * sin(m_fSlope);
        }
        else
        {
            float k = atan2f(getDestination().y - getPositionY(),getDestination().x - getPositionX());
            if(m_u8MoveType == MOVE_BY_SCRIPT)
            {
                offsetX = m_nScriptSpeed * cos(k);
                offsetY = m_nScriptSpeed * sin(k);
            }
            else
            {
                offsetX = m_n16Speed * cos(k);
                offsetY = m_n16Speed * sin(k);
            }
            if (fabsf(offsetX) >= fabsf(getDestination().x - getPositionX()))
            {// 目的地与当前位置横坐标间距小于1帧的偏移量
                offsetX = getDestination().x - getPositionX();
            }
            if (fabsf(offsetY) >= fabsf(getDestination().y - getPositionY()))
            {// 目的地与当前位置纵坐标间距小于1帧的偏移量
                offsetY = getDestination().y - getPositionY();
            }
        }
        
        if (offsetX > 0)
        {
            setFaceToRight(true);
        }
        else if(offsetX < 0)
        {
            setFaceToRight(false);
        }
        CCPoint tempPosition = getPosition() + ccp(offsetX,offsetY);// 移动后的位置
        if(tempPosition.x < 0)
        {
            tempPosition.x = 0;
            if (m_n8CurrentBehaviorIndex == 2)
            {
                randomMoveSlope(3);
            }
            else
            {
                m_bBlocked = true;
                m_bMoving = false;
                return;
            }
        }
        else if(tempPosition.x >= m_pScene->getSceneSize().width)
        {
            tempPosition.x = m_pScene->getSceneSize().width - 1;
            if (m_n8CurrentBehaviorIndex == 2)
            {
                randomMoveSlope(4);
            }
            else
            {
                m_bBlocked = true;
                m_bMoving = false;
                return;
            }
        }
        offsetX = tempPosition.x - getPositionX();
        switchAction(ACTION_RUN);
        // 地格碰撞
        int tempTileColCount,tempTileRowCount;// 移动后所在元素格列数/行数
        tempTileColCount =  tempPosition.x / m_pScene->getTileWidth();
        tempTileRowCount = (Controller::getInstance()->getSceneCellTopY() - tempPosition.y) / m_pScene->getTileHeight();
        if (m_pScene->isBlockTile(tempTileColCount, tempTileRowCount))
        {// 移动后所在单元格有障碍
            if (!m_pScene->isBlockTile(tempTileColCount, m_CurrentTile.y))
            {// 若只横向位移无阻碍
                setPosition(ccpAdd(ccp(offsetX,0), getPosition()));// 横向位移
                getNameLabel()->setPosition(ccpAdd(ccp(offsetX,0), getNameLabel()->getPosition()));
                m_CurrentTile.x = tempTileColCount;
            }
            else if(!m_pScene->isBlockTile(m_CurrentTile.x,tempTileRowCount))
            {// 若只纵向位移无阻碍
                setPosition(ccpAdd(ccp(0,offsetY), getPosition()));// 纵向位移
                getNameLabel()->setPosition(ccpAdd(ccp(0,offsetY), getNameLabel()->getPosition()));
                m_CurrentTile.y = tempTileRowCount;
            }
            else
            {//遇障碍无法位移
                if (m_n8CurrentBehaviorIndex == 2)
                {
                    //纵向受阻碍
                    if (m_pScene->isBlockTile(m_CurrentTile.x,tempTileRowCount))
                    {
                        if (tempPosition.y > getPositionY())
                        {//无法向下位移
                            randomMoveSlope(2);
                        }
                        else
                        {//无法向上位移
                            randomMoveSlope(1);
                        }
                    }
                }
                else
                {
                    m_bBlocked = true;
                    m_bMoving = false;
                    return;
                }
            }
        }
        else
        {// 无障碍 
            setPosition(tempPosition);
            getNameLabel()->setPosition(ccpAdd(ccp(offsetX,offsetY), getNameLabel()->getPosition()));
            setCurrentTile(ccp(tempTileColCount,tempTileRowCount));
        }
        if (getDestination().x == getPositionX() && getDestination().y == getPositionY())
        {// 到达目的地 
            m_bArriveAtTheDestination = true;
            m_bMoving = false;
        }
    }
}

#pragma mark 计算攻击间隔

void YYMonster::countAttackInterval()
{
    for (int i = 0 ; i < m_pSpriteData->getSkillIndexArray()->count(); i++)
    {
        int skillIndex = ((CCString *)m_pSpriteData->getSkillIndexArray()->objectAtIndex(i))->intValue();
        SkillData *skillData = (SkillData *)GlobalData::getInstance()->getCommonSkillData(skillIndex);
        if (skillData)
        {
            if (!m_pAttackable[i])
            {
                if (m_pAttackIntervalCounter[i] < skillData->getCoolingDuration() * FRAME_PER_SECOND)
                {
                    m_pAttackIntervalCounter[i] ++;
                }
                else
                {
                    m_pAttackIntervalCounter[i] = 0;
                    m_pAttackable[i] = true;
                }
            }
        }
    }
}

#pragma mark 是否可攻击 

bool YYMonster::isAttackable()
{
    for (int i = 0; i < m_pSpriteData->getHPRangeArray()->count(); i++)
    {
        CCArray * array = split_string(((CCString *)m_pSpriteData->getHPRangeArray()->objectAtIndex(i))->getCString(), ',');
        float hpPercent = (float)m_uCurrentHP / m_pSpriteAttribute->getMaxHP();
        float max = ((CCString *)array->objectAtIndex(0))->intValue() / 100.0f;
        float min = ((CCString *)array->objectAtIndex(1))->intValue() / 100.0f;
        if (hpPercent <= max && hpPercent >= min)
        {//施放该技能的血值范围 
            int value = ((CCString *)m_pSkillCounterOfUsedArray->objectAtIndex(i))->intValue();
            if (value != 0)
            {
                if (value > 0)
                {
                    value--;
                    m_pSkillCounterOfUsedArray->removeObjectAtIndex(i);
                    m_pSkillCounterOfUsedArray->insertObject(CCString::createWithFormat("%d",value), i);
                }
                if(m_pAttackable[i])
                {// 可攻击 
                    YYRect * rect = dynamic_cast<YYRect *>(m_pSpriteData->getAttackRectArray()->objectAtIndex(i));
                    if (!rect || m_pTargetSprite->isInRangeOfAttack(this,rect))
                    {// 目标在攻击范围内 
                        int odds = ((CCString *)m_pSpriteData->getAttackOddsArray()->objectAtIndex(i))->intValue();
                        int rand = RANDOM_INT(0, 99);
                        if(rand < odds)
                        {
                            // 满足攻击条件，停止位移 
                            m_bMoving = false;
                            m_n8AttackIndex = i;// 攻击方式的索引 
                            m_nDamageCounter = 0;// 伤害计数器重置 
                            m_pAttackable[i] = false;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

void YYMonster::attack()
{
    if (!m_pSkill)
    {
        m_bMoving = false;
        int skillDataIndex = ((CCString *)m_pSpriteData->getSkillIndexArray()->objectAtIndex(m_n8AttackIndex))->intValue();
        SkillData * skillData = (SkillData *)GlobalData::getInstance()->getCommonSkillData(skillDataIndex);
        if(skillData)
        {
            createSkillWithData(skillData);
            setSkillAction();
            setActionStatus(kActionSkill);
        }
    }
}
/**
 * 追击目标
 */

void YYMonster::pursueAndAttackTarget()
{
    if (m_pTargetSprite)
    {
        if (getActionStatus() == kActionNormal)
        {
            setFaceToRight(m_pTargetSprite->getPositionX() > getPositionX());// 根据目标位置设置朝向 
            if(isAttackable())
            {
                if (m_bAkinesia)
                {// 不可行动 
                    m_bMoving = false;
                    setActionStatus(kActionNormal);
                    switchAction(ACTION_STAND0);
                }
                else
                {
                    if (m_pSpriteData->getSpriteIndex() == 301 &&
                        (getActionIndex() == ACTION_RUN || getActionIndex() == ACTION_RUN1 || getActionIndex() == ACTION_RUN2))// 如果是沙虫且正在移动或钻地 
                    {
                        switchAction(ACTION_RUN2);//沙虫钻出 
                        m_bWaitForAttack = true;
                    }
                    else
                    {
                        attack();
                    }
                }
            }
            else
            {// 不可攻击则靠近目标 
                YYRect * rect = NULL;
                rect = (YYRect *)m_pSpriteData->getAttackRectArray()->objectAtIndex(m_pSpriteData->getRangeIndex() - 1);
                // 刚好进入攻击范围的横向间距（同向间距与对向间距可能会不同） 
                int intervalH = rect->getX() + rect->getWidth() + (m_pTargetSprite->isFaceToRight() == isFaceToRight() ? -m_pTargetSprite->getSpriteData()->getRangeOfBody().getMinX() : m_pTargetSprite->getSpriteData()->getRangeOfBody().size.width + m_pTargetSprite->getSpriteData()->getRangeOfBody().getMinX());
                if (!m_pTargetSprite->isInRangeOfAttack(this,rect))
                {// 脱离攻击范围 
                    if (m_pSpriteData->getSpriteIndex() == 301)
                    {// 沙虫特殊处理 
                        if (getActionIndex() == ACTION_STAND0)
                        {
                            switchAction(ACTION_RUN1);//先钻地再移动 
                        }
                    }
                    else
                    {
                        setMoving(true);
                        m_u8MoveType = MOVE_BY_SET_DESTINATION;
                    }
                    setDestination(ccp((m_pTargetSprite->getPositionX() + (isFaceToRight() ? -intervalH : intervalH)),m_pTargetSprite->getPositionY()));
                }
                else
                {
                    if (m_pSpriteData->getSpriteIndex() == 301 && getActionIndex() == ACTION_RUN)
                    {
                        switchAction(ACTION_RUN2);//沙虫钻出 
                    }
                    else
                    {
                        setMoving(false);
                        switchAction(ACTION_STAND0);
                        setActionStatus(kActionNormal);
                    }
                }
            }
        }
    }
    else
    {// 无目标 站立原地 
        
        if (m_pSpriteData->getSpriteIndex() == 301)
        {
            if (getActionIndex() == ACTION_RUN)
            {
                switchAction(ACTION_RUN2);//沙虫钻出 
            }
        }
        else
        {
            setActionStatus(kActionNormal);
            if (m_bSummoned && m_n16OwnerID == ((YYSpriteLayer *)getParent())->getRoleByControlled()->getSpriteID())
            {
                YYPlayer * owner = ((YYSpriteLayer *)getParent())->getRoleByControlled();
                int intervalH = getDefaultSize().width / 2 + owner->getDefaultSize().width;// 横方向间距 
                if (fabsf(getPositionX() - owner->getPositionX()) > intervalH // 水平间距过大 
                    || getPositionY() < owner->getPositionY() - getDefaultSize().height // 召唤物头顶低于主人脚底 
                    || getPositionY() > owner->getPositionY() + owner->getDefaultSize().height)// 召唤物脚底高于主人头顶 
                {// 追随主人 
                    setFaceToRight(owner->getPositionX() > getPositionX());
                    setMoving(true);
                    setDestination(ccp((owner->getPositionX() + (isFaceToRight() ? -intervalH : intervalH)),owner->getPositionY()));
                    m_u8MoveType = MOVE_BY_SET_DESTINATION;
                    
                }
                else
                {
                    switchAction(ACTION_STAND0);
                }
            }
            else
            {
                switchAction(ACTION_STAND0);
            }
        }
    }
}

#pragma mark 确定当前的行为 

void YYMonster::determineCurrentBehavior(bool isIgnoreCounter)
{
    if (m_bAkinesia && m_bImmovability)
    {// 不可攻击，不可移动(眩晕） 
        if (getActionStatus() != kActionDie && getActionStatus() != kActionInjured)
        {
            switchBehavior(0);
        }
        return;
    }
    if (m_bRunAround && !m_bImmovability)
    {// 乱窜，可移动 
        if (getActionStatus() == kActionNormal)
        {
            switchBehavior(2);
        }
        return;
    }
    
    // 周期性改变行为 
    if (m_u16ConfirmBehaviorCounter < m_pSpriteData->getConfirmBehaviorDuration() && !isIgnoreCounter)
    {
        m_u16ConfirmBehaviorCounter ++;
    }
    else
    {
        m_u16ConfirmBehaviorCounter = 0;
        findTarget(m_bFriendly ? ((YYSpriteLayer *)getParent())->getMoveableEnemyArray() : ((YYSpriteLayer *)getParent())->getFriendlyRoleArray(),isIgnoreCounter);
        if (!isIgnoreCounter)
        {
            if (!m_pTargetSprite)
            {// 视野范围内无目标，站立原地
                switchBehavior(0);
                return;
            }
        }
        CCArray * array = split_string(((CCString *)m_pSpriteData->getBehaviorArray()->objectAtIndex(m_n8CurrentBehaviorIndex))->getCString(),',');
        int odds;
        for (int i = 0; i < array->count(); i++)
        {
//            if (i == 0)
//            {
//                odds = ((CCString *)array->objectAtIndex(0))->intValue();
//            }
//            else
//            {
//                odds += ((CCString *)array->objectAtIndex(i))->intValue();
//            }
//            if (RANDOM_INT(0, 99) < odds)
//            {
//                m_n8CurrentBehaviorIndex = i;
//                break;
//            }
            int temp = 99;
            odds = ((CCString *)array->objectAtIndex(i))->intValue();
            if (i > 0)
            {
                temp -= ((CCString *)array->objectAtIndex(i-1))->intValue();
            }
            if (RANDOM_INT(0, temp) < odds)
            {
                m_n8CurrentBehaviorIndex = i;
                break;
            }
        }
        
        // 处于攻击或挨打等不可打断的状态，等待以一次循环调用时再执行不同的行为 
        if (getActionStatus() == kActionNormal)
        {
            switchBehavior(m_n8CurrentBehaviorIndex);
        }
        else
        {
            m_bWaitingForSwitchBehavior = true;
        }
    }
    
    if (m_bWaitingForSwitchBehavior && getActionStatus() == kActionNormal)
    {
        switchBehavior(m_n8CurrentBehaviorIndex);
        m_bWaitingForSwitchBehavior = false;
    }
}

/**
 * 行为切换
 */
void YYMonster::switchBehavior(int8_t behaviorIndex)
{
    m_bStartPursue = false;
    if (behaviorIndex == 0)
    {
        m_bMoving = false;
        switchAction(ACTION_STAND0);
        setActionStatus(kActionNormal);
    }
    else if (behaviorIndex == 1)
    {
        findTarget(m_bFriendly ? ((YYSpriteLayer *)getParent())->getMoveableEnemyArray() : ((YYSpriteLayer *)getParent())->getFriendlyRoleArray());
        pursueAndAttackTarget();
        m_bStartPursue = true;
    }
    else
    {
        m_bMoving = true;
        m_u8MoveType = MOVE_BY_RANDOM;
        randomMoveSlope();
    }
}

#pragma mark 设定随机移动的斜                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               率
void YYMonster::randomMoveSlope(uint8_t blockedType)
{
    float slope[4] = {atan2f(1,2)/* 右上 */,atan2f(-1,2)/* 右下 */,atan2f(1,-2)/* 左上 */,atan2f(-1, -2)/* 左下 */};
    int rand = RANDOM_INT(0, 3);
    switch (blockedType)
    {
        case 1://受阻于上边缘 
            if (rand == 0 || rand == 2)
            {
                rand++;
            }
            break;
        case 2://受阻于下边缘 
            if (rand == 1 || rand == 3)
            {
                rand++;
            }
            break;
        case 3://受阻于左边缘 
            if (rand == 2 || rand == 3)
            {
                rand+=2;
            }
            break;
        case 4://受阻于右边缘 
            if (rand == 0 || rand == 1)
            {
                rand+=2;
            }
            break;
        default:
            break;
    }
    if (rand > 3)
    {
        rand -= 3;
    }
    m_fSlope = slope[rand];
}
/* 
 * 判断敌人是否在视野范围内存活
 */
bool YYMonster::isEnemyInView(YYFightSprite *enemy)
{
    if (enemy->getPositionX() > getPositionX() - m_pSpriteData->getRangeOfVisibility().width
        && enemy->getPositionX() < getPositionX() + m_pSpriteData->getRangeOfVisibility().width
        && enemy->getPositionY() > getPositionY() - m_pSpriteData->getRangeOfVisibility().height
        && enemy->getPositionY() < getPositionY() + m_pSpriteData->getRangeOfVisibility().height
        && enemy->isVisible()
        && enemy->getCurrentHP() > 0)
    {
        return true;
    }
    return false;
}
#pragma mark 搜寻视野范围内的目标

/**
 * 搜寻视野范围内的目标
 * @isIgnore
 */
void YYMonster::findTarget(cocos2d::CCArray * targetArray,bool isIgnoreVisionField)
{
    if (targetArray->count() == 0)
    {
        m_pTargetSprite = NULL;
        return;
    }
    
    // 可搜寻目标数量仅有一个 
    if (targetArray->count() == 1)
    {
        YYFightSprite *enemy = (YYFightSprite *)targetArray->objectAtIndex(0);
        if(isIgnoreVisionField || isEnemyInView(enemy))
        {/* 搜寻视野范围内活着的敌人 */
            m_pTargetSprite = enemy;// 目标为自身控制角色 
        }
        return;
    }
    
    /* 锁定生命值全队最低并且不足最大生命值10%的目标 */
    CCObject * object = NULL;
    int targetID = -1;
    bool isRecordMarkHP = false;
    int enemyMarkHP = 0;
    CCARRAY_FOREACH(targetArray, object)
    {
        YYFightSprite * enemy = (YYFightSprite *)object;
        if(isIgnoreVisionField || isEnemyInView(enemy))
        {/* 搜寻视野范围内活着的敌人 */
            if (!isRecordMarkHP)
            {
                isRecordMarkHP = true;
                enemyMarkHP = enemy->getCurrentHP();
                targetID = enemy->getSpriteID();
            }
            else
            {
                if (enemy->getCurrentHP() < enemyMarkHP)
                {
                    enemyMarkHP = enemy->getCurrentHP();
                    targetID = enemy->getSpriteID();
                }
            }
        }
    }
    if (targetID >= 0)
    {
        YYFightSprite * targetSprite = (YYFightSprite *)((YYSpriteLayer *)getParent())->getSpriteDictionary()->objectForKey(targetID);
        if (targetSprite->getCurrentHP() < targetSprite->getSpriteAttribute()->getMaxHP() / 10.0)
        {
            m_pTargetSprite = targetSprite;
            return;
        }
    }
    
    // 比较仇恨值 锁定仇恨值最大的目标 
    object = NULL;
    int hatedPoint = -1;
    targetID = -1;
    CCARRAY_FOREACH(targetArray, object)
    {
        YYFightSprite * enemy = (YYFightSprite *)object;
        if(isIgnoreVisionField || isEnemyInView(enemy))
        {// 搜寻视野范围内活着的敌人 
            int tempPoint = 0;
            if(enemy->getHatredPointDictionary()->objectForKey(getSpriteID()))
            {
                tempPoint = ((CCInteger *)enemy->getHatredPointDictionary()->objectForKey(getSpriteID()))->getValue();
            }
            if (tempPoint > hatedPoint)
            {
                hatedPoint = tempPoint;
                m_pTargetSprite = enemy;
            }
        }
    }
    if (m_pTargetSprite)
    {
        return;
    }
    
    // 寻找最近目标 
    object = NULL;
    targetID = -1;
    bool isInit = false;
    int distance = 0;
    CCARRAY_FOREACH(targetArray, object)
    {
        YYFightSprite * enemy = (YYFightSprite *)object;
        if(isIgnoreVisionField || isEnemyInView(enemy))
        {// 搜寻视野范围内活着的敌人 
            if (!isInit)
            {
                isInit = true;
                distance = (enemy->getPositionX() - getPositionX()) * (enemy->getPositionX() - getPositionX()) +  (enemy->getPositionY() - getPositionY()) * (enemy->getPositionY() - getPositionY());
                m_pTargetSprite = enemy;
            }
            else
            {
                int dis = (enemy->getPositionX() - getPositionX()) * (enemy->getPositionX() - getPositionX()) +  (enemy->getPositionY() - getPositionY()) * (enemy->getPositionY() - getPositionY());
                if (dis < distance)
                {
                    distance = dis;
                    m_pTargetSprite = enemy;
                }
            }
        }
    }
    if (m_pTargetSprite)
    {
        return;
    }
    m_pTargetSprite = NULL;
}

#pragma mark 更新血条显示 

void YYMonster::updateStatusBar(int currentPoint,int maxPoint)
{
    if (m_nRecordHP == currentPoint)
    {
        return;
    }
    m_nRecordHP = currentPoint;
    m_pBloodSpriteBatch->removeAllChildren();
    int width = currentPoint * ( m_nBloodMaxWidth - 4) / maxPoint;
    int texWidth = m_pBloodSpriteBatch->getTexture()->getContentSize().width;
    int texHeight = m_pBloodSpriteBatch->getTexture()->getContentSize().height;
    if (width < OBLIGATE_WIDTH)
    {// 宽度小于圆角部分 
        CCSprite * sprite = CCSprite::createWithTexture(m_pBloodSpriteBatch->getTexture(),CCRectMake(0, 0, width, texHeight));
        sprite->setAnchorPoint(ccp(0,0.5));
        sprite->setPosition(ccp(m_BloodPosition.x - m_nBloodMaxWidth / 2 + 2,m_BloodPosition.y));
        m_pBloodSpriteBatch->addChild(sprite);
    }
    else
    {
        // 血条左端的圆角 
        CCSprite * sprite = CCSprite::createWithTexture(m_pBloodSpriteBatch->getTexture(),CCRectMake(0, 0, OBLIGATE_WIDTH, texHeight));
        sprite->setAnchorPoint(ccp(0,0.5));
        sprite->setPosition(ccp(m_BloodPosition.x - m_nBloodMaxWidth / 2 + 2,m_BloodPosition.y));
        m_pBloodSpriteBatch->addChild(sprite);
        if (width < OBLIGATE_WIDTH * 2)
        {
            sprite = CCSprite::createWithTexture(m_pBloodSpriteBatch->getTexture(),CCRectMake(OBLIGATE_WIDTH, 0, width - OBLIGATE_WIDTH, texHeight));
            sprite->setAnchorPoint(ccp(0,0.5));
            sprite->setPosition(ccp(m_BloodPosition.x + OBLIGATE_WIDTH - m_nBloodMaxWidth / 2 + 2,m_BloodPosition.y));
            m_pBloodSpriteBatch->addChild(sprite);
            return;
        }
        else
        {// 血条右端的圆角 
            CCSprite * sprite = CCSprite::createWithTexture(m_pBloodSpriteBatch->getTexture(),CCRectMake(0,0, OBLIGATE_WIDTH, texHeight));
            sprite->setScaleX(-1);
            sprite->setPosition(ccp(m_BloodPosition.x + width - m_nBloodMaxWidth / 2 - OBLIGATE_WIDTH / 2,m_BloodPosition.y));
            m_pBloodSpriteBatch->addChild(sprite);
            int num = (width - OBLIGATE_WIDTH * 2) / (texWidth - OBLIGATE_WIDTH);
            int remainder = (width - OBLIGATE_WIDTH * 2) - num * (texWidth - OBLIGATE_WIDTH);
            for (int i = 0; i < num; i++)
            {
                CCSprite * sprite = CCSprite::createWithTexture(m_pBloodSpriteBatch->getTexture(),CCRectMake(OBLIGATE_WIDTH, 0, texWidth - OBLIGATE_WIDTH, texHeight));
                sprite->setAnchorPoint(ccp(0,0.5));
                sprite->setPosition(ccp(m_BloodPosition.x + 2 + i * (texWidth - OBLIGATE_WIDTH) + OBLIGATE_WIDTH - m_nBloodMaxWidth / 2,m_BloodPosition.y));
                m_pBloodSpriteBatch->addChild(sprite);
            }
            if(remainder > 0)
            {
                CCSprite * sprite = CCSprite::createWithTexture(m_pBloodSpriteBatch->getTexture(),CCRectMake(OBLIGATE_WIDTH, 0, remainder, texHeight));
                sprite->setAnchorPoint(ccp(0,0.5));
                sprite->setPosition(ccp(m_BloodPosition.x + 2 + num * (texWidth - OBLIGATE_WIDTH)  + OBLIGATE_WIDTH - m_nBloodMaxWidth / 2,m_BloodPosition.y));
                m_pBloodSpriteBatch->addChild(sprite);
            }
        }
    }
}

void YYMonster::die()
{
    YYFightSprite::die();
    for (int i = 0; i < m_pFootmanIDArray->count(); i++)
    {
        int footmanID = ((CCInteger *)m_pFootmanIDArray->objectAtIndex(i))->getValue();
        YYMonster *footman = (YYMonster *)((YYSpriteLayer *)getParent())->getSpriteDictionary()->objectForKey(footmanID);
        if (footman)
        {
            footman->setDeleted(true);
        }
    }
    for (int i = 0; i < m_pEffectIDArray->count(); i++)
    {
        int effectID = ((CCInteger *)m_pEffectIDArray->objectAtIndex(i))->getValue();
        YYSkillEffect *effect = (YYSkillEffect *)((YYSpriteLayer *)getParent())->getSpriteDictionary()->objectForKey(effectID);
        if (effect)
        {
            effect->setDeleted(true);
        }
    }
}
