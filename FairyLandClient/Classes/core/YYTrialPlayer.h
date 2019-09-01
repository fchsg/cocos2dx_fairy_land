//
//  YYTrialPlayer.h
//  Fairyland
//
//  Created by 付辰昊 on 4/25/14.
//
//

#ifndef __Fairyland__YYTrialPlayer__
#define __Fairyland__YYTrialPlayer__

#include "YYFightSprite.h"
#include "YYSkill.h"

USING_NS_CC;

class YYRect;
class YYTrialPlayer : public YYFightSprite
{
    CC_SYNTHESIZE(uint32_t, m_uLevel, Level);// 等级;
    CC_SYNTHESIZE(uint32_t, m_uCurrentSP, CurrentSP);// 怒气值;
    CC_SYNTHESIZE(uint32_t, m_uMaxSP, MaxSP);// 怒气值;
    CC_SYNTHESIZE(uint32_t, m_uCurrentExp, CurrentExp);// 当前经验;
    CC_SYNTHESIZE(uint32_t, m_uLevelUpExp, LevelUpExp);// 升级所需经验;
    CC_SYNTHESIZE(float, m_fEnegyPercentage, EnegyPercentage);// 怒气能量值;
    
protected:
    bool m_bArriveAtTheDestination;// 到达目的地
    bool m_bBlocked;               // 被阻碍
    bool m_bStartPursue;           // 开始追击目标
    uint16_t *m_pAttackIntervalCounter;  // 快捷键技能间隔计数器
    bool *m_bHadAttack;                  // 快捷键技能是否已触发一次
    bool *m_bEnegyHadAttack;             // 怒气技能是否已触发一次
    
public:
    YYTrialPlayer(void);
    ~YYTrialPlayer();
    CREATE_FUNC(YYTrialPlayer);
    void update(void);
    void executeAI();
    void determineCurrentBehavior();
    void pursueAndAttackTarget();
    bool isAttackable();
    SkillData * getSkillData(int skillIndex);
    void countAttackInterval();
    void move(void);
    void die();
    void attack();
    
};

#endif /* defined(__Fairyland__YYTrialPlayer__) */
