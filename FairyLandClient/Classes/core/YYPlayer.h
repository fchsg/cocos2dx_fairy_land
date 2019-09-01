//
//  YYPlayer.h
//  XJ
//  玩家可控角色 
//  Created by fuchenhao on 13-4-22. 
//
//

#ifndef __XJ__YYPlayer__
#define __XJ__YYPlayer__

#include "YYFightSprite.h"
#include "YYSkill.h"

#define TAG_HORSE 40// 坐骑标签 

class YYScene;
class TaskData;

enum RunningStatus
{
    kRunningStatusNormal = 0,
    kRunningStatusAuto ,
    kRunningStatusDefault
};

class YYPlayer : public YYFightSprite
{
    
    CC_SYNTHESIZE(uint32_t, m_uLevel, Level);// 等级; 
    CC_SYNTHESIZE(uint32_t, m_uCurrentSP, CurrentSP);// 怒气值; 
    CC_SYNTHESIZE(uint32_t, m_uMaxSP, MaxSP);// 怒气值; 
    CC_SYNTHESIZE(uint32_t, m_uCurrentExp, CurrentExp);// 当前经验; 
    CC_SYNTHESIZE(uint32_t, m_uLevelUpExp, LevelUpExp);// 升级所需经验; 
    CC_SYNTHESIZE(cocos2d::CCPoint, m_TargetTile, TargetTile);// 任务目标格
    CC_SYNTHESIZE(RunningStatus, m_eRunningStatus,RunningStatus);
    
private:
    
    bool m_bTransform;// 进入传送门 
    bool m_bControlledBySelf;// 是否是自己控制的角色 
    bool m_bFindingTaskNPC;// 正在寻找任务NPC
    int m_nEquipmentIndex;
    bool m_bStandBy;
    int m_nStandByCounter;// 待机计数器 
    int m_nStandByDuration;// 待机持续时间 
    
public:
    
    YYPlayer();
    ~YYPlayer();
    CREATE_FUNC(YYPlayer);
    virtual void update(void);

    void recover();// 恢复血、蓝 
    void die();
    void switchAction(uint16_t animationID);/*切换动作 */

    void move(void);
    void processMoveEvent(int columnCount,int rowCount);// 处理行走时触发的事件
    void taskAutoFindingWay(TaskData *taskData, bool isFindingTaskNPC);// 任务自动寻路
    void transform(int columnCount,int rowCount);
    void ride();// 骑乘 
    void unRide();// 解除骑乘 
    void replaceEquipment(const char * equipmentResourceName);// 换装 
    void relive();
    // 技能相关 
    void attack();
    void useSkill(SkillData *skillData);
    SkillData * getSkillData(int index);// 按技能图标序号获取相应的技能数据
    
    inline void setControlledBySelf(bool isSelf)
    {// 设置为自己控制的角色 
        m_bControlledBySelf = isSelf;
    }
    
    inline bool isControlledBySelf() const
    {// 是否是自己控制的角色 
        return m_bControlledBySelf;
    }

    inline void setFindingTaskNPC(bool isTrue)
    {
        m_bFindingTaskNPC = isTrue;
    }
};

#endif /* defined(__XJ__YYPlayer__) */
