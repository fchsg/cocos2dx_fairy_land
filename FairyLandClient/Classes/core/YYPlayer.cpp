//
//  YYPlayer.cpp
//  XJ
//
//  Created by fuchenhao on 13-4-22. 
//
//

#include "SimpleAudioEngine.h"

#include "../common/Common.h"
#include "YYPlayer.h"
#include "YYTrialPlayer.h"
#include "YYController.h"
#include "YYScene.h"
#include "YYMonster.h"
#include "YYControlUILayer.h"
#include "YYSpriteLayer.h"
#include "YYSkillEffect.h"
#include "YYSkillAction.h"
#include "YYSkill.h"
#include "YYSpriteAttribute.h"
#include "YYPopUpUI.h"
#include "YYTaskData.h"
#include "YYGlobalData.h"

USING_NS_CC;
using namespace CocosDenshion;

#define HORSE_RUN_SPEED 36// 坐骑冲刺速率 

#define HEALTH_RECOVER_VOLUME 50 // 体力回复量 
#define MANA_RECOVER_VOLUME 50  // 魔法回复量 

YYPlayer::YYPlayer()
:m_bControlledBySelf(false)
,m_bTransform(false)
,m_bFindingTaskNPC(false)
,m_bStandBy(false)
,m_uLevel(0)
,m_uCurrentSP(0)
,m_uMaxSP(0)
,m_uCurrentExp(0)
,m_uLevelUpExp(0)
,m_nEquipmentIndex(1)
,m_nStandByCounter(0)
,m_nStandByDuration(200)// 待机动作出现周期
,m_eRunningStatus(kRunningStatusDefault)
{
}

YYPlayer::~YYPlayer()
{
}

void YYPlayer::switchAction(uint16_t actionID)
{
    if (m_u8ActionIndex == actionID)
    {
        return;
    }
    if (actionID == ACTION_STAND0)
    {// 切换普通站立动作时重新计算待机动作出现时间 
        m_bStandBy = true;
        m_nStandByCounter = 0;
    }
    else
    {
        m_bStandBy = false;
    }
    setAction(actionID, false);
}

#pragma mark 技能相关(包含普通攻击) 

SkillData * YYPlayer::getSkillData(int skillIndex)
{
    SkillData *skillData = (SkillData *)GlobalData::getInstance()->getPlayerSkillData(getSpriteIDInServer()->getCString(), skillIndex);
    return skillData;
}

/**
 * 攻击
 */
void YYPlayer::attack()
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

void YYPlayer::useSkill(SkillData *skillData)
{
    if (m_pSkill && getActionStatus() == kActionAttack)
    {
        CC_SAFE_RELEASE_NULL(m_pSkill);
    }
    createSkillWithData(skillData);
    m_bMoving = false;
    setSkillAction();
    setActionStatus(kActionSkill);
}

#pragma mark 循环

void YYPlayer::update(void)
{
    YYFightSprite::update();
    if(m_bTrialPause)
        return;
    if (!m_bPause)
    {
        if (m_bStandBy && m_u8ActionIndex == ACTION_STAND0)
        {// 普通站立动作下可切换待机动作
            if (m_nStandByCounter < m_nStandByDuration)
            {
                m_nStandByCounter ++;
            }
            else
            {
                m_bStandBy = false;
                switchAction(ACTION_STAND1);
            }
        }
        
        if (m_bActionOver && !m_bRepeatAction)
        {
            if (getActionStatus() == kActionNormal)
            {
                if (isControlledBySelf() && m_pScene->getControlUILayer()->isHoldAttackKey())
                {// 连续攻击（自控角色调用）
                    attack();
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
        if(Controller::getInstance()->isTrial()) //试炼
        {
            if (!m_bImmovability && m_bMoving)
            {
                move();
            }
        }
        else
        {
            if (m_bMoving)
            {
                move();
            }
        }
    }
}

void YYPlayer::die()
{
    if (getActionStatus() != kActionDie)
    {
        m_bMoving = false;
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
        if (this == m_pScene->getSpriteLayer()->getRoleByControlled())
        {
            if(Controller::getInstance()->isTrial())  //试炼
            {
                m_pScene->getPopUpUI()->initTrialResult(false);
                m_pScene->getControlUILayer()->trialOver();  //试炼结束
                m_pScene->getControlUILayer()->clearKeyState();//  清除按键状态
                m_pScene->getControlUILayer()->getRoleByTrial()->setTrialPause(true);
            }
            else  //副本
            {
                DungeonType type =  Controller::getInstance()->getDungeonType(); // 获取副本类型
                switch (type)
                {
                    case kDungeonNormal: // 正常副本
                    {
                        Controller::getInstance()->setGameStatus(kStatusDead);
                        m_pScene->showTips(string(g_sIf) + string(g_sReliveConfirm),kTipDeadInfo);
                        m_pScene->getControlUILayer()->clearKeyState();//  清除按键状态
                    }
                    break;
                    case kDungeonDungeon: // 活动_地下城
                    {
                        m_pScene->getControlUILayer()->clearKeyState();//  清除按键状态
                        CCString* msg = CCString::createWithFormat(ASK_DUNGEON_RESULT,0);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                        
                    }
                    break;
                    case kDungeonProtoss: // 活动_守卫星灵
                    {
                        m_pScene->getControlUILayer()->clearKeyState();//  清除按键状态
                        CCString* msg = CCString::createWithFormat(ASK_PROTECT_PROTOSS_RESULT,0);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                    }
                    break;
                    case kDungeonBoss: // 活动_Boss
                    {
                        m_pScene->getControlUILayer()->clearKeyState();//  清除按键状态
                        CCString* msg = CCString::createWithFormat(ASK_BOSS_DUNGEON_RESULT,0);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                    }
                        break;
                    case kWorldBoss:
                    {
                        m_pScene->getControlUILayer()->clearKeyState();//  清除按键状态
                        int damagePoint = m_pScene->getSpriteLayer()->getWorldBossDamageHP();
                        CCString* msg = CCString::createWithFormat(ASK_WORLD_BOSS_RESULT,damagePoint);
                        Controller::getInstance()->sendMessage(msg->getCString(),true);
                    }
                        break;
                    default:
                    {
                        Controller::getInstance()->setGameStatus(kStatusDead);
                        m_pScene->showTips(string(g_sIf) + string(g_sReliveConfirm),kTipDeadInfo);
                        m_pScene->getControlUILayer()->clearKeyState();//  清除按键状态
                    }
                    break;
                }
            }
        }
    }
}

#pragma mark 移动 

void YYPlayer::move()
{
    float offsetX = 0,offsetY = 0;
    switch(m_u8MoveType)
    {
        case MOVE_BY_CONTROLLED:
        {
            float k = atan2f(m_pScene->getControlUILayer()->getCurrentTouchPoint().y - DIRECT_VIRTUAL_KEY_POSITION.y,m_pScene->getControlUILayer()->getCurrentTouchPoint().x - DIRECT_VIRTUAL_KEY_POSITION.x);
            offsetX = m_pSpriteData->getSpeed() * cos(k);
            offsetY = m_pSpriteData->getSpeed() * sin(k);
            break;
        }
        case  MOVE_BY_SKILL:
            m_bMoving = false;
            offsetX = (isFaceToRight() ? m_n16OffsetXBySkill : -m_n16OffsetXBySkill);
            offsetY = 0;
            break;
        case MOVE_BY_SET_DESTINATION:
        case MOVE_BY_CLICK_NPC:
        case MOVE_BY_SCRIPT:
        {
            float k = atan2f(getDestination().y - getPositionY(),getDestination().x - getPositionX());
            int speed = getMoveType() == MOVE_BY_SCRIPT ? m_nScriptSpeed : m_pSpriteData->getSpeed();
            offsetX = speed * cos(k);
            offsetY = speed * sin(k);
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
    offsetX = tempPosition.x - getPositionX();
    offsetY = tempPosition.y - getPositionY();

    // 地格碰撞
    int tempTileColCount,tempTileRowCount;// 移动后所在元素格列数、行数 
    tempTileColCount =  tempPosition.x / m_pScene->getTileWidth();
    tempTileRowCount = (Controller::getInstance()->getSceneCellTopY() - tempPosition.y) / m_pScene->getTileHeight();
    if (m_pScene->isBlockTile(tempTileColCount, tempTileRowCount))
    {// 移动后所在单元格有障碍 
        if (!m_pScene->isBlockTile(tempTileColCount, m_CurrentTile.y) && fabsf(offsetX) > 0.1)
        {// 若横向位移无阻碍
            setPosition(ccp(offsetX,0) + getPosition());
            getNameLabel()->setPosition( ccp(offsetX,0) + getNameLabel()->getPosition());
            if (m_bControlledBySelf)
            {
                processMoveEvent(tempTileColCount,m_CurrentTile.y);
            }
            m_CurrentTile.x = tempTileColCount;
        }
        else if(!m_pScene->isBlockTile(m_CurrentTile.x,tempTileRowCount) && fabsf(offsetY) > 0.1)
        {// 若纵向位移无阻碍 
            setPosition(ccp(0,offsetY) + getPosition());// 纵向位移
            getNameLabel()->setPosition(ccpAdd(ccp(0,offsetY), getNameLabel()->getPosition()));
            if (m_bControlledBySelf)
            {
                processMoveEvent(m_CurrentTile.x,tempTileRowCount);
            }
            m_CurrentTile.y = tempTileRowCount;
        }
        else
        {
            if ((fabsf(offsetX) < 0.1 && fabsf(offsetY) < 0.1)
                && getMoveType() != MOVE_BY_CONTROLLED)
            {
                m_bMoving = false;
                switchAction(ACTION_STAND0);
            }
        }
    }
    else
    {// 无障碍
        setPosition(tempPosition);
        getNameLabel()->setPosition(ccp(offsetX,offsetY) + getNameLabel()->getPosition());
        if (m_bControlledBySelf)
        {
            processMoveEvent(tempTileColCount,tempTileRowCount);
        }
        m_CurrentTile = ccp(tempTileColCount,tempTileRowCount);
        if ((fabsf(offsetX) < 0.1 && fabsf(offsetY) < 0.1)
            && getMoveType() != MOVE_BY_CONTROLLED)
        {
            m_bMoving = false;
            switchAction(ACTION_STAND0);
        }
    }

    if (m_u8MoveType == MOVE_BY_SET_DESTINATION || m_u8MoveType == MOVE_BY_SCRIPT)
    {
        if(m_bFindingTaskNPC)
        {
            if (m_CurrentTile.x <= m_TargetTile.x + 1
                && m_CurrentTile.x >= m_TargetTile.x - 1
                && m_CurrentTile.y <= m_TargetTile.y + 1
                && m_CurrentTile.y >= m_TargetTile.y - 1)
            {
                m_bMoving = false;
                switchAction(ACTION_STAND0);
                m_bFindingTaskNPC = false;
                setRunningStatus(kRunningStatusDefault);
                if(Controller::getInstance()->getGameStatus() == kStatusUI)
                {
                    m_pScene->getPopUpUI()->exitUI();
                }
                if (!m_pScene->getControlUILayer()->getChoosedNPC())
                {
                    int taskIndex = m_pScene->getControlUILayer()->getGuideButton()->getIndex();
                    TaskData * task = (TaskData *)GlobalData::getInstance()->getTaskDataDict()->objectForKey(taskIndex);
                    if(task)
                    {
                        m_pScene->getPopUpUI()->showTaskInfoInDialogInterface(taskIndex);
                    }
                }
                else
                {
                    const char *msg = CCString::createWithFormat(ASK_FUNCTION_FROM_NPC,m_pScene->getControlUILayer()->getChoosedNPC()->getNPCID().c_str())->getCString();
                    m_pScene->getControlUILayer()->setChoosedNPC(NULL);
                    Controller::getInstance()->sendMessage(msg,true);
                }
            }
        }

        else if (getDestination().x == getPositionX() && getDestination().y == getPositionY())
        {// 到达目的地 
            m_bMoving = false;
            switchAction(ACTION_STAND0);
            if (getMoveType() == MOVE_BY_SCRIPT)
            {
                setSpriteMirror(false);
            }

        }
    }

}

void YYPlayer::recover()
{
    bool isRecoverHP = false;
    bool isRecoverMP = false;
    for (int i = 0; i < ((YYSpriteLayer*)getParent())->getMedicamentArray()->count(); i++)
    {
        YYSprite * sprite = (YYSprite *)((YYSpriteLayer*)getParent())->getMedicamentArray()->objectAtIndex(i);
        if (!sprite->isDeleted())
        {
            if (m_CurrentTile.x <= sprite->getCurrentTile().x + 1
                && m_CurrentTile.x >= sprite->getCurrentTile().x - 1
                && m_CurrentTile.y <= sprite->getCurrentTile().y + 1
                && m_CurrentTile.y >= sprite->getCurrentTile().y - 1)
            {
                sprite->setDeleted(true);
                if (!isRecoverHP)
                {
                    if (sprite->getSpriteName()->isEqual(CCString::create(g_sBloodBottle)))
                    {
                        isRecoverHP = true;
                        if (m_pSpriteAttribute->getMaxHP() > HEALTH_RECOVER_VOLUME && m_uCurrentHP < m_pSpriteAttribute->getMaxHP() -   HEALTH_RECOVER_VOLUME)
                        {
                            m_uCurrentHP += HEALTH_RECOVER_VOLUME;
                        }
                        else
                        {
                            m_uCurrentHP = m_pSpriteAttribute->getMaxHP();
                        }
                        // 回血特效 
                        YYSprite * sprite = YYSprite::createWithActionSet("recover-1", "recover-1", kDirTypeSprite);
                        sprite->setAction(0, false);
                        sprite->setPosition(ccp(0, getDefaultSize().height));
                        sprite->setDeleteCondition(DELETE_ACTION_OVER);
                        addChild(sprite,15);
                        getYYSpriteChildren()->addObject(sprite);
                        
                        //回血音效
                        const std::string filePath = getFilePath(RECOVERHP_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                        PLAY_EFFECT(filePath.c_str());
                        
                        if (!isRecoverMP)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
                if (!isRecoverMP)
                {
                    if (sprite->getSpriteName()->isEqual(CCString::create(g_sMagicBottle)))
                    {
                        isRecoverMP = true;
                        if (m_pSpriteAttribute->getMaxMP() > MANA_RECOVER_VOLUME && m_uCurrentMP < m_pSpriteAttribute->getMaxMP() - MANA_RECOVER_VOLUME)
                        {
                            m_uCurrentMP += m_pSpriteAttribute->getMaxMP() - MANA_RECOVER_VOLUME;
                        }
                        else
                        {
                            m_uCurrentMP = m_pSpriteAttribute->getMaxMP();
                        }
                        // 回蓝特效 
                        YYSprite * sprite = YYSprite::createWithActionSet("recover-1", "recover-2", kDirTypeSprite);
                        sprite->setAction(0, false);
                        sprite->setPosition(ccp(0, getDefaultSize().height));
                        sprite->setDeleteCondition(DELETE_ACTION_OVER);
                        addChild(sprite,15);
                        getYYSpriteChildren()->addObject(sprite);
                        
                        if (!isRecoverHP)
                        {
                            continue;
                        }
                        else
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

#pragma mark 替换装备 

void YYPlayer::replaceEquipment(const char * equipmentResourceName)
{
    CCArray * array = split_string(equipmentResourceName, '-');
    if (array && array->count() > 0)
    {
        int part = ((CCString *)array->objectAtIndex(1))->intValue();
        const std::string path = "sprite/"+std::string(equipmentResourceName)+".png";
        int count = getPngFileNameDictionary()->count();
        for (int i = 0; i < count; i++)
        {
            const char* pic = ((CCString *)getPngFileNameDictionary()->objectForKey(i))->getCString();
            CCArray * array1 = split_string(pic, '-');
            int part1 = ((CCString *)array1->objectAtIndex(1))->intValue();
            if(part == part1)
            {
                getPngFileNameDictionary()->removeObjectForKey(i);
                getPngFileNameDictionary()->setObject(CCString::create(path), i);
                CCSpriteBatchNode * batchNode = ((CCSpriteBatchNode * )m_pBatchNodeDictionary->objectForKey(i));
                int capacity = batchNode->getTextureAtlas()->getCapacity();
                removeChild(batchNode,true);
                batchNode = CCSpriteBatchNode::create(path.c_str(),capacity);
                m_pBatchNodeDictionary->removeObjectForKey(i);
                m_pBatchNodeDictionary->setObject(batchNode, i);
                addChild(batchNode,part);
                return;
            }
        }
        getPngFileNameDictionary()->setObject(CCString::create(path), count + 1);
        CCSpriteBatchNode * batchNode = CCSpriteBatchNode::create(path.c_str());
        m_pBatchNodeDictionary->setObject(batchNode, count + 1);
        addChild(batchNode,part);
    }
}

#pragma mark 处理位移触发的事件
void YYPlayer::processMoveEvent(int columnCount,int rowCount)
{
    // 所在位置格子序号未改变 
    if ((m_CurrentTile.x == columnCount && m_CurrentTile.y == rowCount))
    {
        return;
    }
    
    for (int i = 0; i < ((YYSpriteLayer*)getParent())->getDoorArray()->count(); i++)
    {
        YYSprite * door = (YYSprite *)((YYSpriteLayer*)getParent())->getDoorArray()->objectAtIndex(i);
        if (door->isVisible()){
            if (columnCount < door->getCurrentTile().x + 2
                && columnCount > door->getCurrentTile().x - 2
                && rowCount < door->getCurrentTile().y + 2
                && rowCount > door->getCurrentTile().y - 2)
            {// 碰撞传送法阵 
                if (door->getSpriteData()->getType() == SPRITE_TYPE_DUNGEONS)
                {// 弹出副本选择界面的传送门 
                    if (!m_bTransform)
                    {
                        m_bTransform = true;
                        setMoving(false);
                        switchAction(ACTION_STAND0);
                        Controller::getInstance()->sendMessage(ASK_DUNGEONS_PORTAL,true);
                        const std::string filePath = getFilePath(PORTAL_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                        PLAY_EFFECT(filePath.c_str());
                        return;
                    }
                }
                else
                {// 进入战斗场景的传送门 
                    if (!m_bTransform)
                    {
                        m_bTransform = true;
                        if(Controller::getInstance()->getSceneType() == NEWBIE_TASK_ZONE)
                        {
                            if (m_pScene->getPopUpUI())
                            {
                                TaskData *taskData = static_cast<TaskData *>(GlobalData::getInstance()->getTaskDataDict()->objectForKey(m_pScene->getPopUpUI()->getAcceptedTaskIndex()));
                                if (taskData && (taskData->getTargetType() == kTaskKillMonster || taskData->getTargetType() == kTaskCollect))
                                {
                                    const char * msg = CCString::createWithFormat(SEND_TASK_COMPLETE,taskData->getTaskIndex())->getCString();
                                    Controller::getInstance()->sendMessage(msg,false);
                                    const std::string filePath = getFilePath(PORTAL_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                                    PLAY_EFFECT(filePath.c_str());
                                }
                            }
                        }
                        setMoving(false);
                        switchAction(ACTION_STAND0);
                        int currentTileIndex = (int)door->getCurrentTile().x + (int)door->getCurrentTile().y * m_pScene->getTileColumnAmount() + 10;
                        Controller::getInstance()->sendMessage(CCString::createWithFormat(SEND_CURRENT_POSITION,currentTileIndex)->getCString(),false);
                        Controller::getInstance()->switchGameStatus(kStatusLoading);
                        int currentTime = Controller::getInstance()->getCurrentSecond();
                        m_pScene->setStageDuration(m_pScene->getStageDuration()+ currentTime - m_pScene->getCurrentTime());
                        return;
                    }
                }
            }
            else
            {
                m_bTransform = false;
            }
        }
    }
    
    if (Controller::getInstance()->getSceneType() == DUNGEONS
        || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {// 战斗场景 
        recover();
    }
    else if (Controller::getInstance()->getSceneType() == MAIN_CITY && (Controller::getInstance()->getScriptStatus() == kScriptStatusNull || Controller::getInstance()->getScriptStatus() == kScriptStatusOver))
    {
        int currentTileIndex = columnCount + rowCount * m_pScene->getTileColumnAmount() + 10;
        const char *message = CCString::createWithFormat(SEND_CURRENT_POSITION,currentTileIndex)->getCString();
        Controller::getInstance()->sendMessage(message,false);

    }
}

void YYPlayer::taskAutoFindingWay(TaskData *taskData, bool isFindingTaskNPC)
{
    setMoving(true);
    setMoveType(MOVE_BY_SET_DESTINATION);
    setRunningStatus(kRunningStatusAuto);
    setFindingTaskNPC(isFindingTaskNPC);
    if (isFindingTaskNPC)
    {        
        int tileIndex = taskData->getTargetTileIndex() - 10;//即将前往的地图格索引
        setTargetTile(ccp(tileIndex % m_pScene->getTileColumnAmount(),tileIndex / m_pScene->getTileColumnAmount()));
    }
    CCPoint position = ((YYSpriteLayer *)getParent())->getSpritePosition(taskData->getTargetTileIndex());
    setDestination(position);
}

void YYPlayer::relive()
{
    switchAction(ACTION_STAND0);
    setActionStatus(kActionNormal);
    setCurrentHP(getSpriteAttribute()->getMaxHP());
    setCurrentMP(getSpriteAttribute()->getMaxMP());
}
