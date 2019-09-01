//
//  YYSkillData.h
//  XJ
//
//  Created by fuchenhao on 13-7-25. 
//
//

#ifndef XJ_YYSkillData_h
#define XJ_YYSkillData_h

#include "cocos2d.h"

enum DebuffType
{
    kDebuffTypeNone = 0,
    kDebuffStun = 1,// 眩晕 
    kDebuffFrozen = 2,// 冰冻 
    kDebuffCilence = 3,// 沉默 
    kDebuffFear = 4,// 恐惧 
    kDebuffTieDown// 束缚(减速) 
};

enum DOTType
{
    kDOTBurning = 1,// 灼烧 
    kDOTPoison = 2// 中毒 
};

enum SkillType
{
    kSkillTypeAT = 1,// 普通攻击
    kSkillTypeSS = 2,// 技能攻击
    kSkillTypeSW = 3,// 怒技攻击
    kSkillTypeMT = 4,// 怪物技能
};

enum SkillSpecialType
{
    kSkillTypeNormal = 0,
    kSkillTypeCatapult = 1,
    kSkillTypeLink = 2,
    kSkillTypeTelesport = 3,
    kSkillTypeSummon = 4,
    kSkillTypeDropDamage = 5
};

class SkillData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(std::string, m_sPlayerID, PlayerID);
    CC_SYNTHESIZE(uint32_t ,m_u8Index, Index);// 序号
    CC_SYNTHESIZE(uint8_t, m_nLevel, Level);// 技能等级
    CC_SYNTHESIZE(uint8_t, m_nSkillType, SkillType);// 技能类型
    CC_SYNTHESIZE(std::string, m_sSkillName, SkillName);// 技能名称 
    CC_SYNTHESIZE(std::string, m_sSkillIcon, SkillIcon);// 技能图标 
    CC_SYNTHESIZE(std::string, m_sSkillUnableIcon,SkillUnableIcon);// 技能无法释放时的图标
    CC_SYNTHESIZE(float, m_fCoolingDuration, CoolingDuration);// 技能冷却时间 
    CC_SYNTHESIZE(uint16_t, m_u16ConsumePoint, ConsumePoint);// 消耗值 
    CC_SYNTHESIZE(uint16_t, m_u16AddedDamage, AddedDamage);// 附加伤害值 
    CC_SYNTHESIZE(float, m_fAPCoefficient, APCoefficient);// 攻击力倍数 
    CC_SYNTHESIZE(float, m_fSAPCoefficient, SAPCoefficient);// 法术攻击力倍数 
    CC_SYNTHESIZE(float, m_fSkillDamageCoefficient, SkillDamageCoefficient);// 技能伤害系数 
    CC_SYNTHESIZE(int, m_nRoleLevel, RoleLevel);// 升级所需角色等级 
    CC_SYNTHESIZE(int, m_nMoney, Money);// 金钱消耗 
    CC_SYNTHESIZE(int, m_nJew, Jew);// 宝石消耗 
    CC_SYNTHESIZE(std::string, m_sCurrentLevelDescription, CurrentLevelDescription);// 当前技能描述 
    CC_SYNTHESIZE(std::string, m_sNextLevelDescription, NextLevelDescription);// 技能下一个等级描述 
    CC_SYNTHESIZE(std::string, m_sItemIcon, ItemIcon);// 技能所需道具图标 
    CC_SYNTHESIZE(int, m_nItemID, ItemID);// 学习技能所需物品的ID 
    CC_SYNTHESIZE(int, m_nItemNumber, ItemNumber);// 学习技能所需道具数量
    CC_SYNTHESIZE(int, m_nTalentPoint, TalentPoint);// 已投入天赋点
    CC_SYNTHESIZE(int, m_nCurrentTalentPoint, CurrentTalentPoint);// 当前投入天赋点
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pActionIndexArray, ActionIndexArray);// 对应动作的序号 
    CC_SYNTHESIZE(uint8_t, m_u8DebuffType, DebuffType);// 减益效果类型 0无 1晕眩 2冻结 3沉默 4恐惧 5减速 
    CC_SYNTHESIZE(uint16_t, m_u16DebuffDuration, DebuffDuration);// 减益效果持续时间 
    CC_SYNTHESIZE(uint8_t, m_u8DebuffAddedValue, DebuffAddedValue);// 减益效果其他附加值(如减速效果的速度减少值) 
    CC_SYNTHESIZE(uint8_t, m_u8DOTType, DOTType);// dot类型 0无 1灼烧 2中毒 
    CC_SYNTHESIZE(uint16_t, m_u16DOTDuration, DOTDuration);// 持续时间 
    CC_SYNTHESIZE(uint16_t, m_u16DOTDamagePoint, DOTDamagePoint);// 每秒伤害 
    CC_SYNTHESIZE(uint16_t, m_u16HatredPoint, HatredPoint);// 技能造成的额外仇恨值 
    CC_SYNTHESIZE(uint16_t, m_u16TargetReducedTenacity, TargetReducedTenacity);// 造成被攻击对象韧性值的消耗量 
    CC_SYNTHESIZE(uint16_t, m_u8SpecialType, SpecialType);// 技能特殊类型 0 无特殊处理 1弹射 2 链击 3瞬移 4 召唤 5 随机位置掉落伤害 
    CC_SYNTHESIZE(int, m_nCatapultAmount, CatapultAmount);// 弹射总数 
    CC_SYNTHESIZE(int, m_nMaxCatapultDistance, MaxCatapultDistance);// 弹射范围 
    CC_SYNTHESIZE(int, m_nSummonedSpriteIndex, SummonedSpriteIndex);// 被召唤的精灵编号 
    CC_SYNTHESIZE(int, m_nSummonedSpriteNum, SummonedSpriteNum);// 召唤物数量 
    CC_SYNTHESIZE(int, m_nSummonedSpriteDuration, SummonedSpriteDuration);// 召唤物持续时间 
    CC_SYNTHESIZE(int, m_nTrapNum, TrapNum);// 机关陷阱类出现的数量 
    CC_SYNTHESIZE(int32_t, m_nHurtNumber, HurtNumber);// 伤害个数 
    CC_SYNTHESIZE(int8_t, m_u8BeatedBackDistance, BeatedBackDistance);// 被击退的距离
    CC_SYNTHESIZE(int8_t, m_u8GestureType, GestureType);// 手势类型 0线 1V 2圆
    CC_SYNTHESIZE(uint8_t, m_u8ResetSkillTalentJew, ResetSkillTalentJew); // 重置技能天赋所需宝石消耗
 
private:
    
    bool m_bAngrySkill;// 是否是怒气技 
    bool m_bBreaked;// 是否可被打断 
    bool m_bBeatedBack; // 是否被击退 
    
public:
    
    SkillData()
    :m_pActionIndexArray(NULL)
    ,m_u8ResetSkillTalentJew(0)
    ,m_sPlayerID("")
    ,m_sItemIcon("")
    ,m_nItemID(0)
    ,m_nTrapNum(0)
    ,m_nHurtNumber(0)
    ,m_nSummonedSpriteIndex(0)
    ,m_nMaxCatapultDistance(0)
    ,m_nCatapultAmount(0)
    ,m_nSummonedSpriteNum(0)
    ,m_u16TargetReducedTenacity(0)
    ,m_bBeatedBack(false)
    ,m_nSummonedSpriteDuration(0)
    ,m_sSkillName("")
    ,m_sSkillIcon("")
    ,m_sSkillUnableIcon("")
    ,m_sCurrentLevelDescription("")
    ,m_sNextLevelDescription("")
    ,m_u8BeatedBackDistance(0)
    ,m_u8DebuffType(0)
    ,m_u16DebuffDuration(0)
    ,m_u8DebuffAddedValue(0)
    ,m_u8DOTType(0)
    ,m_u16DOTDuration(0)
    ,m_u16DOTDamagePoint(0)
    ,m_u8Index(0)
    ,m_u8SpecialType(0)
    ,m_u16HatredPoint(0)
    ,m_nLevel(0)
    ,m_fCoolingDuration(0)
    ,m_nMoney(0)
    ,m_nJew(0)
    ,m_nItemNumber(0)
    ,m_u16ConsumePoint(0)
    ,m_u16AddedDamage(0)
    ,m_nRoleLevel(0)
    ,m_nTalentPoint(0)
    ,m_fAPCoefficient(0)
    ,m_fSAPCoefficient(0)
    ,m_fSkillDamageCoefficient(0)
    ,m_nCurrentTalentPoint(0)
    ,m_bBreaked(false)
    ,m_bAngrySkill(false)
    ,m_nSkillType(0)
    ,m_u8GestureType(0)
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(SkillData);
    
    ~SkillData()
    {
        CC_SAFE_RELEASE(m_pActionIndexArray);
    }
    
    inline void setBreaked(bool isBreaked)
    {
        m_bBreaked = isBreaked;
    }
    
    inline bool isBreaded()
    {
        return  m_bBreaked;
    }
    
    inline void setAngrySkill(bool isAngrySkill)
    {
        m_bAngrySkill = isAngrySkill;
    }
    
    inline bool isAngrySkill()
    {
        return  m_bAngrySkill;
    }
    
    inline bool isBeatedBack()
    {
        return m_bBeatedBack;
    }
    
    inline void setBeatedBack(bool isBeatedBack)
    {
        m_bBeatedBack = isBeatedBack;
    }
    SkillData *copyWithZone()
    {
        SkillData *skillData = SkillData::create();
        skillData->setPlayerID(getPlayerID());
        skillData->setIndex(getIndex());
        skillData->setLevel(getLevel());
        skillData->setSkillType(getSkillType());
        skillData->setSkillName(getSkillName());
        skillData->setSkillIcon(getSkillIcon());
        skillData->setSkillUnableIcon(getSkillUnableIcon());
        skillData->setLevel(getLevel());
        skillData->setCoolingDuration(getCoolingDuration());
        skillData->setConsumePoint(getConsumePoint());
        skillData->setAddedDamage(getAddedDamage());
        skillData->setAPCoefficient(getAPCoefficient());
        skillData->setSAPCoefficient(getSAPCoefficient());
        skillData->setSkillDamageCoefficient(getSkillDamageCoefficient());
        skillData->setRoleLevel(getRoleLevel());
        skillData->setMoney(getMoney());
        skillData->setJew(getJew());
        skillData->setCurrentLevelDescription(getCurrentLevelDescription());
        skillData->setNextLevelDescription(getNextLevelDescription());
        skillData->setItemIcon(getItemIcon());
        skillData->setItemID(getItemID());
        skillData->setItemNumber(getItemNumber());
        skillData->setTalentPoint(getTalentPoint());
        skillData->setCurrentTalentPoint(getCurrentTalentPoint());
        skillData->setActionIndexArray(getActionIndexArray());
        skillData->setDebuffType(getDebuffType());
        skillData->setDebuffDuration(getDebuffDuration());
        skillData->setDebuffAddedValue(getDebuffAddedValue());
        skillData->setDOTType(getDOTType());
        skillData->setDOTDuration(getDOTDuration());
        skillData->setDOTDamagePoint(getDOTDamagePoint());
        skillData->setHatredPoint(getHatredPoint());
        skillData->setTargetReducedTenacity(getTargetReducedTenacity());
        skillData->setSpecialType(getSpecialType());
        skillData->setCatapultAmount(getCatapultAmount());
        skillData->setMaxCatapultDistance(getMaxCatapultDistance());
        skillData->setSummonedSpriteIndex(getSummonedSpriteIndex());
        skillData->setSummonedSpriteNum(getSummonedSpriteNum());
        skillData->setSummonedSpriteDuration(getSummonedSpriteDuration());
        skillData->setTrapNum(getTrapNum());
        skillData->setHurtNumber(getHurtNumber());
        skillData->setBeatedBackDistance(getBeatedBackDistance());
        skillData->setGestureType(getGestureType());
        skillData->setBreaked(isBreaded());
        skillData->setAngrySkill(isAngrySkill());
        skillData->setBeatedBack(isBeatedBack());
        skillData->setResetSkillTalentJew(getResetSkillTalentJew());
        return skillData;
    }
};

#endif
