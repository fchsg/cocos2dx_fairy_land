//
//  YYSpriteAttribute.h
//  FairyLand II
//
//  Created by fuchenhao on 13-9-18. 
//
//

#ifndef FairyLand_II_YYSpriteAttribute_h
#define FairyLand_II_YYSpriteAttribute_h

class SpriteAttribute : public cocos2d::CCObject
{

    CC_SYNTHESIZE(int, m_nID, ID);
    CC_SYNTHESIZE(uint32_t, m_uLevel, Level);// 等级; 
    CC_SYNTHESIZE(uint32_t, m_uMaxHP, MaxHP);// 生命值 
    CC_SYNTHESIZE(uint32_t, m_uMaxMP, MaxMP);// 魔力值 
    CC_SYNTHESIZE(uint32_t, m_uGold, Gold);// 金币数 
    CC_SYNTHESIZE(uint32_t, m_uJewel, Jewel);// 宝石数 
    CC_SYNTHESIZE(uint32_t, m_uTalentPoint, TalentPoint);// 天赋点 
    CC_SYNTHESIZE(uint32_t, m_uTempTalentPoint, TempTalentPoint);// 未得到服务端确认前本地显示的值 
    CC_SYNTHESIZE(uint32_t, m_uPhysicalStrengthPoint, PhysicalStrengthPoint);// 体力值; 
    CC_SYNTHESIZE(uint32_t, m_uIntelligencePoint, IntelligencePoint);// 智力值; 
    CC_SYNTHESIZE(uint32_t, m_uStrengthPoint, StrengthPoint);// 力量值; 
    CC_SYNTHESIZE(uint32_t, m_uAgilityPoint, AgilityPoint);// 敏捷值; 
    CC_SYNTHESIZE(uint32_t, m_uStaminaPoint, StaminaPoint);// 耐力值;
    CC_SYNTHESIZE(uint32_t, m_uSkillDamagePoint, SkillDamagePoint);// 法术防御; 
    CC_SYNTHESIZE(uint32_t, m_uFightingCapacity, FightingCapacity);// 战斗力; 
    
    CC_SYNTHESIZE(uint32_t, m_uAttackPoint, AttackPoint);// 攻击力 
    CC_SYNTHESIZE(uint32_t, m_uDefencePoint, DefencePoint);// 防御力 
    CC_SYNTHESIZE(uint32_t, m_uSpellAttackPoint, SpellAttackPoint);// 法术攻击 


    CC_SYNTHESIZE(uint32_t, m_uBasicCriticalOdds, BasicCriticalOdds);// 基础暴击率 
    CC_SYNTHESIZE(uint32_t, m_uCriticalLevel, CriticalLevel);// 暴击等级 
    CC_SYNTHESIZE(uint32_t, m_uCriticalConstant, CriticalConstant);// 暴击常数 
    CC_SYNTHESIZE(uint32_t, m_uBasicDodgeOdds, BasicDodgeOdds);// 基础闪避率 
    CC_SYNTHESIZE(uint32_t, m_uDodgeLevel, DodgeLevel);// 闪避等级 
    CC_SYNTHESIZE(uint32_t, m_uDodgeConstant, DodgeConstant);// 闪避常数 
    CC_SYNTHESIZE(uint32_t, m_uBasicHitOdds, BasicHitOdds);// 命中闪避率 
    CC_SYNTHESIZE(uint32_t, m_uHitLevel, HitLevel);// 命中等级 
    CC_SYNTHESIZE(uint32_t, m_uHitConstant, HitConstant);// 命中常数
    
    CC_SYNTHESIZE(uint32_t, m_uVipLevel, VipLevel);// vip等级
    CC_SYNTHESIZE(uint32_t, m_uVipJew, VipJew);// 已充值宝石数
    
public:
    SpriteAttribute()
    :m_uLevel(0)
    ,m_uMaxHP(0)
    ,m_uMaxMP(0)
    ,m_uGold(0)
    ,m_uJewel(0)
    ,m_uAttackPoint(0)
    ,m_uDefencePoint(0)
    ,m_uPhysicalStrengthPoint(0)
    ,m_uSpellAttackPoint(0)
    ,m_uIntelligencePoint(0)
    ,m_uStrengthPoint(0)
    ,m_uAgilityPoint(0)
    ,m_uStaminaPoint(0)
    ,m_uSkillDamagePoint(0)
    ,m_uFightingCapacity(0)
    ,m_uBasicCriticalOdds(0)
    ,m_uCriticalLevel(0)
    ,m_uCriticalConstant(0)
    ,m_uBasicHitOdds(70)
    ,m_uHitLevel(0)
    ,m_uBasicDodgeOdds(0)
    ,m_uDodgeLevel(0)
    ,m_uHitConstant(0)
    ,m_uDodgeConstant(0)
    ,m_uTalentPoint(0)
    ,m_uTempTalentPoint(0)
    ,m_uVipLevel(0)
    ,m_uVipJew(0)
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(SpriteAttribute);
};

#endif
