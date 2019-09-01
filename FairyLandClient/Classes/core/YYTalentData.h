//
//  YYTalentData.h
//  Fairyland
//
//  Created by fuchenhao on 14-2-25. 
//
//

#ifndef Fairyland_YYTalentData_h
#define Fairyland_YYTalentData_h

enum TYPE
{
    kTypeReduceCD = 0,
    kTypeReduceMPConsume = 1,
    kTypeAddDamage = 2,
    kTypeSetBreaked = 3,// 设置是否能被打断 
    kTypeChangeActionIndex = 4,// 更改动作序号 
    kTypeAddHurtedNumber = 5,// 增加伤害个数 
    kTypeDebuff = 6,// 触发debuff 
    kTypeDOT = 7,// 触发DOT 
    kTypeBeatBack = 8// 击退 
};

#include "cocos2d.h"

class TalentData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(int32_t, m_nIndex, Index);// 天赋序号
    CC_SYNTHESIZE(uint8_t, m_nLevel, Level) // 当前天赋点数
    CC_SYNTHESIZE(int32_t, m_nSkillIndex, SkillIndex);// 对应技能序号 
    CC_SYNTHESIZE(std::string, m_sName, Name);// 名称 
    CC_SYNTHESIZE(std::string, m_sIconName, IconName);// 图标文件名
    CC_SYNTHESIZE(int32_t, m_nMaxPoint, MaxPoint);// 最大可投入点数 
    CC_SYNTHESIZE(uint8_t, m_nSkillLevel, SkillLevel);// 升级天赋所需技能等级
    CC_SYNTHESIZE(int32_t, m_nPreposeTalentIndex, PreposeTalentIndex);// 前置天赋序号 
    CC_SYNTHESIZE(int32_t, m_nPreposeTalentPoints, PreposeTalentPoints);// 前置天赋点数
    CC_SYNTHESIZE(int32_t, m_nAmountTalentPoints, AmountTalentPoints);// 需求天赋总点数点数
    CC_SYNTHESIZE(int32_t, m_nLayerIndex,LayerIndex);// 层级 
    CC_SYNTHESIZE(int32_t, m_nPositionIndex,PositionIndex);// 位置 
    CC_SYNTHESIZE(int32_t, m_nType, Type);// 类型(可组合) 0 1 2 3 4 5 
    CC_SYNTHESIZE(std::string, m_sCDAddedValue, CDAddedValue); // 技能冷却时间附加值（负值减少冷却时间）
    CC_SYNTHESIZE(std::string, m_sConsumeMPAddedValue, ConsumeMPAddedValue); // 法力消耗的附加值（负值减少法力消耗）
    CC_SYNTHESIZE(std::string, m_sDamageAddedValue, DamageAddedValue);// 技能附加伤害增加值
    CC_SYNTHESIZE(std::string, m_sHurtedNumberAddedValue, HurtedNumberAddedValue);//伤害个数附加值
    CC_SYNTHESIZE(std::string, m_sActionIndexGroup, ActionIndexGroup);// 更改技能调用的动作序号(以,分割的字符串）
    
    CC_SYNTHESIZE(uint8_t, m_u8DebuffType, DebuffType);// 减益效果类型 0无 1晕眩 2冻结 3沉默 4恐惧 5减速 
    CC_SYNTHESIZE(uint16_t, m_nDebuffDuration, DebuffDuration);// 减益效果持续时间
    CC_SYNTHESIZE(uint16_t, m_nDebuffAddedValue, DebuffAddedValue);// 减益效果其他附加值(如减速效果的速度减少值)
    CC_SYNTHESIZE(uint8_t, m_u8DOTType, DOTType);// dot类型 0无 1灼烧 2中毒 
    CC_SYNTHESIZE(uint16_t, m_nDOTDuration, DOTDuration);// 持续时间
    CC_SYNTHESIZE(uint16_t, m_nDOTDamagePoint, DOTDamagePoint);// 每秒伤害
    CC_SYNTHESIZE(std::string, m_sBeatedBackDistance, BeatedBackDistance);// 被击退的距离
    CC_SYNTHESIZE(std::string, m_sDescription, Description);// 描述
    CC_SYNTHESIZE(bool, m_bBreaked, Breaked);  // 是否可以被打断

public:
    TalentData()
    :m_nIndex(0)
    ,m_nAmountTalentPoints(0)
    ,m_nLevel(0)
    ,m_nSkillIndex(0)
    ,m_nMaxPoint(0)
    ,m_nPreposeTalentIndex(0)
    ,m_nPreposeTalentPoints(0)
    ,m_nLayerIndex(0)
    ,m_nPositionIndex(0)
    ,m_u8DebuffType(0)
    ,m_u8DOTType(0)
    ,m_nType(0)
    ,m_sBeatedBackDistance("")
    ,m_sName("")
    ,m_sIconName("")
    ,m_sCDAddedValue("")
    ,m_sConsumeMPAddedValue("")
    ,m_sDamageAddedValue("")
    ,m_sHurtedNumberAddedValue("")
    ,m_nDebuffDuration(0)
    ,m_nDebuffAddedValue(0)
    ,m_nDOTDuration(0)
    ,m_nDOTDamagePoint(0)
    ,m_nSkillLevel(0)
    ,m_sActionIndexGroup("")
    ,m_sDescription("")
    ,m_bBreaked(false)
    {
    }
    
    ~TalentData()
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(TalentData);
};

#endif
