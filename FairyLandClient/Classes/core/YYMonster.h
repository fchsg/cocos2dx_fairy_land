//
//  YYMonster.h
//  XJ
//  小怪 
//  Created by fuchenhao on 13-4-22. 
//
//

#ifndef __XJ__YYMonster__
#define __XJ__YYMonster__

#include "YYFightSprite.h"

#define TAG_BOSS_MARK 10

class YYRect;

class YYMonster : public YYFightSprite
{
    CC_SYNTHESIZE(int16_t, m_n16SummonedDuration, SummonedDuration);// 被召唤物持续时间 
    
protected:
    
    int m_nBloodMaxWidth;
    int m_nRecordHP;
    cocos2d::CCSpriteBatchNode * m_pBloodSpriteBatch;
    bool m_bArriveAtTheDestination;// 到达目的地 
    bool m_bBlocked;// 被阻碍 
    bool m_bSummoned;// 是否是被召唤物 
    int16_t m_n16SummonedCounter;// 被召唤物计时器 
    bool m_bFriendly;// 是否为友方生物 
    bool m_bWaitingForSwitchBehavior;// 是否等待切换动作 
    bool m_bStartPursue;// 开始追击目标 
    bool m_bWaitForAttack;// 等待攻击（执行完当前动作后攻击） 
    cocos2d::CCArray *m_pSkillCounterOfUsedArray;
    cocos2d::CCSpriteBatchNode * m_pManaSpriteBatch;
    int m_nCurrentHP,m_nCurrentMP;// 判断血、魔值是否有变化 
    int32_t m_nCurrentDisplacement;
    int32_t m_nTotalDisplacement;
    cocos2d::CCPoint m_BloodPosition;
    
public:
    
    YYMonster(void);
    ~YYMonster();
    CREATE_FUNC(YYMonster);
    virtual void initFightData(SpriteData * spriteData,SpriteAttribute * spriteAttribute);
    void switchAction(uint16_t actionIndex);
    virtual void update(void);
    virtual void countAttackInterval();// 计算攻击间隔 
    virtual void pursueAndAttackTarget();// 追击目标
    void attack();
    void move(void);
    void initBloodBar(cocos2d::CCPoint position);
    void updateStatusBar(int currentPoint,int maxPoint);
    bool isAttackable();// 判断是否可攻击 
    void determineCurrentBehavior(bool isIgnoreCounter = false/** 是否忽略当前计数直接切换行为 */);
    void randomMoveSlope(uint8_t blockedType = 0/* 受阻类型 0无阻碍 1受阻于场景上边缘 2下边缘 3左边缘 4右边缘 */);// 随机移动的斜率
    bool isEnemyInView(YYFightSprite *enemy);// 判断敌人是否在视野范围内存活
    void findTarget(cocos2d::CCArray *targetArray,bool isIgnoreVisionField = false/** 是否忽略视野*/);
    void die();
    inline void setFriendly(bool isFriendly)
    {
        m_bFriendly = isFriendly;
    }
    inline void setSummoned(bool isSummoned)
    {
        m_bSummoned = isSummoned;
    }
    inline bool isSummoned()
    {
        return m_bSummoned;
    }
    void switchBehavior(int8_t behaviorIndex);// 行为切换 
    void executeAI();// 执行AI 
};
#endif /* defined(__XJ__YYMonster__) */
