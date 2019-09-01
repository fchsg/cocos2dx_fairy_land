//
//  YYTrialPlayer.cpp
//  Fairyland
//
//  Created by 付辰昊 on 4/25/14.
//
//

#include "YYTrialPlayer.h"
#include "../animation/YYRect.h"
#include "../common/Common.h"
#include "YYScene.h"
#include "YYController.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYSpriteAttribute.h"
#include "YYSkillEffect.h"
#include "YYControlUILayer.h"
#include "YYPopUpUI.h"

USING_NS_CC;

static const uint8_t s_u8EnergyConsume[] ={30,60,90};

YYTrialPlayer::YYTrialPlayer()
:m_bArriveAtTheDestination(false)
,m_bBlocked(false)
,m_bStartPursue(false)
,m_fEnegyPercentage(0.0f)
{
    m_u8MoveType = MOVE_BY_SET_DESTINATION;
    m_bMoving = true;
    
    //快捷栏技能计数
    m_pAttackable = new bool[SKILL_SHORTCUT_KEY_AMOUNT];
    m_bHadAttack  = new bool[SKILL_SHORTCUT_KEY_AMOUNT];
    m_pAttackIntervalCounter = new uint16_t[SKILL_SHORTCUT_KEY_AMOUNT];
    for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i ++)
    {
        m_pAttackable[i] = true;
        m_bHadAttack[i] = false;
        m_pAttackIntervalCounter[i] = 0;
    }
    //怒气技能
    m_bEnegyHadAttack = new bool[3];
    for (int i= 0; i < 3; i++)
    {
        m_bEnegyHadAttack[i] = false;
    }
}

YYTrialPlayer::~YYTrialPlayer()
{
    CC_SAFE_DELETE_ARRAY(m_pAttackable);
    CC_SAFE_DELETE_ARRAY(m_bHadAttack);
    CC_SAFE_DELETE_ARRAY(m_pAttackIntervalCounter);
    CC_SAFE_DELETE_ARRAY(m_bEnegyHadAttack);
}

#pragma mark 循环

void YYTrialPlayer::update(void)
{
    YYFightSprite::update();
    if(m_bTrialPause)
        return;
    if(!m_bPause && m_bActionOver && !m_bRepeatAction)
    {
        if (m_pSkill && (getActionStatus() == kActionAttack || getActionStatus() == kActionSkill))
        {
            if (m_pSkill->getSkillAction()->getSkillActionData()->getDuration() == 0)
            {
                m_pSkill->getSkillAction()->stopSoundEffect();// 停止对应的音效
                m_pSkill->setNextSkillAction();
                if (m_pSkill->isSkillOver())
                {
                    skillOver();
                }
                else
                {
                    setSkillAction();
                }
                return;
            }
        }

       switch (m_u8ActionIndex)
        {
            case ACTION_STAND0:
            case ACTION_STAND2:
            case ACTION_RUN:
            case ACTION_RUN1:
                m_u16CurrentFrameIndex = getCurrentActionData()->getStartFrameIndex();
                setFrame(m_u16CurrentFrameIndex);
                m_bActionOver = false;
                break;
            case ACTION_HITTED:
            case ACTION_HURTED:
                setPosition(ccpAdd(getPosition(), ccp(0,- getTempOffsetY())));
                setTempOffsetY(0);
                if (getCurrentHP() <= 0)
                {
                    die();
                }
                else
                {
                    switchAction(ACTION_STAND0);
                    setActionStatus(kActionNormal);
                }
                break;
            case ACTION_DIE:
                break;
            case ACTION_STAND1:
                switchAction(ACTION_STAND0);
                setActionStatus(kActionNormal);
                break;
            default:
                switchAction(ACTION_STAND0);
                setActionStatus(kActionNormal);
                break;
        }
     }
     if (m_uCurrentHP > 0)
     {
        executeAI();
        if (!m_bImmovability && m_bMoving)
        {
            move();
            if(m_bArriveAtTheDestination || m_bBlocked)
            { // 到达目的地或受阻碍停止位移
                m_bArriveAtTheDestination = false;
                m_bBlocked = false;
                switchAction(ACTION_STAND0);
                setActionStatus(kActionNormal);
            }
        }
     }
}

#pragma mark 普通攻击

void YYTrialPlayer::attack()
{
    if(!m_pSkill)
    {
        m_bMoving = false;
        // 获取普通攻击对应的技能编号
        CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerAttackIndexDict()->objectForKey(getSpriteIDInServer()->getCString()));
        int skillIndex = static_cast<CCString *>(array->objectAtIndex(m_n8AttackIndex))->intValue();
        SkillData * skillData = getSkillData(skillIndex);
        if (skillData)
        {
            createSkillWithData(skillData);
            setSkillAction();
            m_n8AttackIndex++;
            if (m_n8AttackIndex == m_u8AttackAmount)
            {
                m_n8AttackIndex = 0;
            }
            setActionStatus(kActionAttack);
        }
    }
}

#pragma mark 执行AI

void YYTrialPlayer::executeAI()
{
    countAttackInterval();//攻击间隔计算(冷却倒计时)
    determineCurrentBehavior();
    if (m_bStartPursue)
    {
        pursueAndAttackTarget();
    }
}

#pragma mark 死亡

void YYTrialPlayer::die()
{
    if (getActionStatus() != kActionDie)
    {
        m_bMoving = false;
        m_bTrialPause = true;
        switchAction(ACTION_DIE);
        setActionStatus(kActionDie);
        if (m_pSkill)
        {
            if (m_pSkill->getSkillAction())
            {
                m_pSkill->getSkillAction()->stopSkillEffect();
            }
            CC_SAFE_RELEASE_NULL(m_pSkill);
        }
        m_pScene->getPopUpUI()->initTrialResult(true);
        m_pScene->getControlUILayer()->trialOver();  //试炼结束
        m_pScene->getControlUILayer()->clearKeyState();//清除按键状态
    }
}

#pragma mark 是否可攻击

bool YYTrialPlayer::isAttackable()
{
    //眩晕状态不可移动
    if (m_bImmovability)
    {// 不可行动
        m_bMoving = false;
        setActionStatus(kActionNormal);
        switchAction(ACTION_STAND0);
        return true;
    }
    if ((m_pSkill && m_pSkill->isSkillOver()) || !m_pSkill)
    {
        //快捷栏技能
        for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i++)
        {
            if (m_pAttackable[i] && !m_bHadAttack[i])
            {
                CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(getSpriteIDInServer()->getCString()));
                CCString * skillID = (CCString *)array->objectAtIndex(i);
                if (!skillID->isEqual(CCString::create("")))
                {
                    int skillIndex = ((CCString *)skillID)->intValue();
                    SkillData * pSkillData = getSkillData(skillIndex);
                    CCArray* arrAction = pSkillData->getActionIndexArray();
                    if (arrAction)
                    {
                        for (int j = 0; j < arrAction->count(); j++) //action
                        {
                            const char* strActionIndex = (((CCString*)arrAction->objectAtIndex(j))->getCString());
                            //动作
                            SkillActionData*  actionData = (SkillActionData*)((CCDictionary*)GlobalData::getInstance()->getSkillActionDataDict())->objectForKey(atoi(strActionIndex));
                            if (actionData)
                            {
                                CCArray* arrActionEffect = actionData->getEffectDataArray();
                                if (arrActionEffect)
                                {
                                    for (int k = 0; k < arrActionEffect->count(); k++) //effect
                                    {
                                        const char* strEffectIndex = ((CCString*)arrActionEffect->objectAtIndex(k))->getCString();
                                        //特效
                                        SkillEffectData*  effectData = (SkillEffectData*)((CCDictionary*)GlobalData::getInstance()->getSkillEffectDataDict())->objectForKey(atoi(strEffectIndex));
                                        CCRect effectRect = effectData->getCollisionRect();
                                        YYRect * yyEffectRect = YYRect::createRect(effectRect);
                                        if (m_pTargetSprite->isInRangeOfAttack(this,yyEffectRect)) //目标可以攻击
                                        {
                                            if( getCurrentMP() >= pSkillData->getConsumePoint())
                                            {
                                                setCurrentMP(getCurrentMP() - pSkillData->getConsumePoint());
                                                createSkillWithData(pSkillData);
                                                setSkillAction();
                                                setActionStatus(kActionSkill);
                                                //技能可移动攻击
                                                if(getSkill()->getSkillAction()->getSkillActionData()->isMovable())
                                                {
                                                    m_bMoving = true;
                                                }
                                                else
                                                {
                                                    m_bMoving = false;
                                                }
                                                m_pAttackable[i] = false;
                                                m_bHadAttack[i] = true;       //每个技能只发送一次
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        //怒气技
        {
            if(m_fEnegyPercentage >= s_u8EnergyConsume[2] && !m_bEnegyHadAttack[2])
            {
                CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(getSpriteIDInServer()->getCString()));
                CCString * object = (CCString *)array->objectAtIndex(2);// 如果第一个手势技能是圆
                if (!object->isEqual(CCString::create("")))
                {
                    int skillIndex = ((CCString *)object)->intValue();
                    SkillData * pSkillData = getSkillData(skillIndex);
                    createSkillWithData(pSkillData);
                    if (getActionIndex() == ACTION_RUN)
                    {
                        setMoving(false);
                    }
                    setSkillAction();
                    setActionStatus(kActionSkill);
                    m_fEnegyPercentage -= s_u8EnergyConsume[2];
                    m_bEnegyHadAttack[2] = true;
                    return true;
                }
            }
            else if(m_fEnegyPercentage >= s_u8EnergyConsume[1] && !m_bEnegyHadAttack[1])
            {
                CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(getSpriteIDInServer()->getCString()));
                CCString * object = (CCString *)array->objectAtIndex(1);// 如果第一个手势技能是圆
                if (!object->isEqual(CCString::create("")))
                {
                    int skillIndex = ((CCString *)object)->intValue();
                    SkillData * pSkillData = getSkillData(skillIndex);
                    createSkillWithData(pSkillData);
                    if (getActionIndex() == ACTION_RUN)
                    {
                        setMoving(false);
                    }
                    setSkillAction();
                    setActionStatus(kActionSkill);
                    m_fEnegyPercentage -= s_u8EnergyConsume[1];
                    m_bEnegyHadAttack[1] = true;
                    return true;
                }
            }
            else if(m_fEnegyPercentage >= s_u8EnergyConsume[0] && !m_bEnegyHadAttack[0])
            {
                CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(getSpriteIDInServer()->getCString()));
                CCString * object = (CCString *)array->objectAtIndex(0);// 如果第一个手势技能是圆
                if (!object->isEqual(CCString::create("")))
                {
                    int skillIndex = ((CCString *)object)->intValue();
                    SkillData * pSkillData = getSkillData(skillIndex);
                    createSkillWithData(pSkillData);
                    if (getActionIndex() == ACTION_RUN)
                    {
                        setMoving(false);
                    }
                    setSkillAction();
                    setActionStatus(kActionSkill);
                    m_fEnegyPercentage -= s_u8EnergyConsume[0];
                    m_bEnegyHadAttack[0] = true;
                    return true;
                }
            }
        }
        
        //普通攻击
        {
            int attackIndex = getAttackIndex();// 正在执行第几段攻击
            // 获取普通攻击对应的技能编号
            CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerAttackIndexDict()->objectForKey(getSpriteIDInServer()->getCString()));
            int skillIndex = static_cast<CCString *>(array->objectAtIndex(attackIndex))->intValue();
            SkillData * pSkillData = getSkillData(skillIndex);
            YYRect * rect = dynamic_cast<YYRect *>(m_pSpriteData->getAttackRectArray()->objectAtIndex(0));
            if (!rect || m_pTargetSprite->isInRangeOfAttack(this,rect))
            {
                createSkillWithData(pSkillData);
                if (getActionIndex() == ACTION_RUN)
                {
                    m_bMoving = false;
                }
                setSkillAction();
                setActionStatus(kActionAttack);
                attackIndex++;
                if (attackIndex == getAttackAmount())
                {
                    setAttackIndex(0);
                }
                else
                {
                    setAttackIndex(attackIndex);
                }
                return true;
            }
        }
    }
    return false;
}

#pragma mark 主动攻击敌人

void YYTrialPlayer::pursueAndAttackTarget()
{
    if (m_pTargetSprite)
    {
        if (getActionStatus() == kActionNormal)
        {
            setFaceToRight(m_pTargetSprite->getPositionX() > getPositionX());// 根据目标位置设置朝向
            if(isAttackable()) //通过距离判定技能是否可触发并且发动攻击
            {

            }
            else // 不可攻击则靠近目标
            {
                YYRect * rect = NULL;
                rect = (YYRect *)m_pSpriteData->getAttackRectArray()->objectAtIndex(0);
                // 刚好进入攻击范围的横向间距（同向间距与对向间距可能会不同）
                int intervalH = rect->getX() + rect->getWidth() + (m_pTargetSprite->isFaceToRight() == isFaceToRight() ? -m_pTargetSprite->getSpriteData()->getRangeOfBody().getMinX() : m_pTargetSprite->getSpriteData()->getRangeOfBody().size.width + m_pTargetSprite->getSpriteData()->getRangeOfBody().getMinX());
                setMoving(true);
                m_u8MoveType = MOVE_BY_SET_DESTINATION;
                setDestination(ccp((m_pTargetSprite->getPositionX() + (isFaceToRight() ? -intervalH : intervalH)),m_pTargetSprite->getPositionY()));
            }
        }
    }
    else
    {
        setMoving(false);
        switchAction(ACTION_STAND0);
        setActionStatus(kActionNormal);
    }
}

#pragma mark 确定当前的行为

void YYTrialPlayer::determineCurrentBehavior()
{
    if (m_bAkinesia && m_bImmovability)
    {// 不可攻击，不可移动(眩晕）
        if (getActionStatus() != kActionDie && getActionStatus() != kActionInjured)
        {
            m_bMoving = false;
            switchAction(ACTION_STAND0);
            setActionStatus(kActionNormal);
            m_bStartPursue = false;
        }
    }
    else
    {
        m_bStartPursue = true;
    }
    return;
}

#pragma mark 计算攻击间隔

void YYTrialPlayer::countAttackInterval()
{
    for (int i = 0; i < SKILL_SHORTCUT_KEY_AMOUNT; i++)   //计算快捷栏技能CD
    {
        CCArray * array = static_cast<CCArray *>(GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(getSpriteIDInServer()->getCString()));
            CCString * skillID = (CCString *)array->objectAtIndex(i);
            if (!skillID->isEqual(CCString::create("")))
            {
                int skillIndex = ((CCString *)skillID)->intValue();
                SkillData * skillData = getSkillData(skillIndex);
     
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

#pragma mark 移动

void YYTrialPlayer::move()
{
    float offsetX = 0,offsetY = 0;
    switch(m_u8MoveType)
    {
       case MOVE_BY_SET_DESTINATION:
            {
              float k = atan2f(getDestination().y-getPositionY() ,getDestination().x - getPositionX());
              offsetX = m_pSpriteData->getSpeed() * cos(k);
              offsetY = m_pSpriteData->getSpeed() * sin(k);
              if (fabsf(offsetX) >= fabsf(getDestination().x - getPositionX()))
              {// 目的地与当前位置横坐标间距小于1帧的偏移量
                offsetX = getDestination().x - getPositionX();
              }
              if (fabsf(offsetY) >= fabsf(getDestination().y - getPositionY()))
              {// 目的地与当前位置纵坐标间距小于1帧的偏移量
                offsetY = getDestination().y - getPositionY();
              }
            }
            break;
        case MOVE_BY_SKILL:
        {
            m_bMoving = false;
            offsetX = (isFaceToRight() ? m_n16OffsetXBySkill : -m_n16OffsetXBySkill);
            offsetY = 0;
        }
            break;
        default:
            break;
    }
    // 无位移，返回
    if (offsetX == 0 && offsetY == 0)
    {
        m_bMoving = false;
        switchAction(ACTION_STAND0);
        return;
    }
    if (getActionStatus() != kActionSkill)
    {
        if (offsetX > 0)
        {
            setFaceToRight(true);
        }
        else if(offsetX < 0)
        {
            setFaceToRight(false);
        }
        switchAction(ACTION_RUN);
    }
    CCPoint tempPosition = ccp(offsetX, offsetY) + getPosition();// 移动后的位置
    if(tempPosition.x < 0)
    {
        tempPosition.x = 0;
    }
    else if(tempPosition.x >= m_pScene->getSceneSize().width)
    {
        tempPosition.x = m_pScene->getSceneSize().width - 1;
    }
    // 地格碰撞
    int tempTileColCount,tempTileRowCount;// 移动后所在元素格列数、行数
    tempTileColCount =  tempPosition.x / m_pScene->getTileWidth();
    tempTileRowCount = (Controller::getInstance()->getSceneCellTopY() - tempPosition.y) / m_pScene->getTileHeight();
    if (m_pScene->isBlockTile(tempTileColCount, tempTileRowCount))
    {// 移动后所在单元格有障碍
        if (!m_pScene->isBlockTile(tempTileColCount, m_CurrentTile.y) && fabsf(offsetX) > 0.1)
        {// 若横向位移无阻碍
            float newPositionX = offsetX + getPositionX();
            if (newPositionX < 0)
            {// 移动至最左端
                setPosition(ccp(0,getPositionY()));
            }
            else if(newPositionX >= m_pScene->getSceneSize().width)
            {// 移动至最右端
                setPosition(ccp(m_pScene->getSceneSize().width - 1,getPositionY()));
            }
            else
            {
                setPosition(ccp(newPositionX,getPositionY()));
            }
            getNameLabel()->setPosition(ccp(newPositionX,0) + getNameLabel()->getPosition());
            m_CurrentTile.x = tempTileColCount;
        }
        else if(!m_pScene->isBlockTile(m_CurrentTile.x,tempTileRowCount) && fabsf(offsetY) > 0.1)
        {// 若纵向位移无阻碍
            setPosition(ccpAdd(ccp(0,offsetY), getPosition()));// 纵向位移
            getNameLabel()->setPosition(ccp(0,offsetY) + getNameLabel()->getPosition());
            m_CurrentTile.y = tempTileRowCount;
        }
        else
        {
            m_bMoving = false;
            switchAction(ACTION_STAND0);
        }
        m_bBlocked = true;
    }
    else
    {// 无障碍
        setPosition(tempPosition);
        getNameLabel()->setPosition(ccp(offsetX,offsetY) + getNameLabel()->getPosition());
        m_CurrentTile = ccp(tempTileColCount,tempTileRowCount);
    }
    
    if (m_u8MoveType == MOVE_BY_SET_DESTINATION || m_u8MoveType == MOVE_BY_SCRIPT)
    {
        if (getDestination().x == getPositionX() && getDestination().y == getPositionY())
        {// 到达目的地
            m_bMoving = false;
            switchAction(ACTION_STAND0);
            m_bArriveAtTheDestination = true;
        }
    }
    else if (getMoveType() == MOVE_BY_SKILL)
    {
        if(getActionStatus() != kActionSkill)// 技能释放结束后
        {
            if(getCurrentHP() > 0)
            {
                switchAction(ACTION_STAND0);
                setActionStatus(kActionNormal);
            }
            else
            {
                die();
            }
        }
    }
}

#pragma mark 技能相关(包含普通攻击)

SkillData * YYTrialPlayer::getSkillData(int skillIndex)
{
    SkillData *skillData = (SkillData *)GlobalData::getInstance()->getPlayerSkillData(getSpriteIDInServer()->getCString(), skillIndex);
    return skillData;
}

