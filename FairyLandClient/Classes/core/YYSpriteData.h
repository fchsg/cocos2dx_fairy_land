//
//  YYSpriteData.h
//  FairyLand II
//  资源名称及AI移动、攻击判定相关数据 
//  Created by fuchenhao on 13-9-13. 
//
//

#include "cocos2d.h"

#ifndef FairyLand_II_YYSpriteData_h
#define FairyLand_II_YYSpriteData_h

class SpriteData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint16_t, m_u16SpriteIndex, SpriteIndex);// 精灵编号 
    CC_SYNTHESIZE(std::string ,m_sSpriteName,SpriteName);// 精灵名称 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pResourceArray, ResourceArray);// 资源 
    CC_SYNTHESIZE(uint8_t,m_u8Type,Type);// 类型 
    CC_SYNTHESIZE(uint16_t, m_u16Tenacity, Tenacity);// 韧性
    CC_SYNTHESIZE(uint8_t, m_u8Speed, Speed);// 行走速度 
    CC_SYNTHESIZE(cocos2d::CCSize, m_RangeOfVisibility, RangeOfVisibility);// 视距 
    CC_SYNTHESIZE(cocos2d::CCRect, m_RangeOfBody, RangeOfBody);// 受击范围 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pAttackRectArray, AttackRectArray);// 攻击范围 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pAttackOddsArray, AttackOddsArray);// 攻击的几率 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pAttackActionIndexArray, AttackActionIndexArray);// 动作序号 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pAttackIntervalArray, AttackIntervalArray);// 攻击间隔 
    CC_SYNTHESIZE(uint16_t, m_u16ConfirmBehaviorDuration, ConfirmBehaviorDuration);// 重新确认行为逻辑的周期 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pBehaviorArray, BehaviorArray);// 不同的行为逻辑对应的几率 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pDamageAmountArray, DamageAmountArray);// 伤害次数 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pDamageFrameIndexArray, DamageFrameIndexArray);// 从第几帧开始造成伤害 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pDamageIntervalArray, DamageIntervalArray);// 间隔时间 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pDebuffArray, DebuffArray);// 造成的不良状态 0无效果 1击晕 2击退 3浮空 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pDOTArray, DOTArray);// 造成的不良状态 0无效果 1击晕 2击退 3浮空 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pResultArray, ResultArray);// 不良状态对应值 0无效果 1眩晕持续时间 2击退 向后的偏移量 3浮空 向上的偏移量 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pTargetReducedTenacityArray, TargetReducedTenacityArray);// 造成被攻击对象韧性值的消耗量 
    CC_SYNTHESIZE(float, m_f16ScaleFactor, ScaleFactor);// 缩放系数 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pHPRangeArray, HPRangeArray);// 血值范围（boss技能发动条件之一） 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pNumOfUsedArray, NumOfUsedArray);// 满足条件后技能可被使用的次数 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pSkillIndexArray, SkillIndexArray);// 每种攻击方式对应的技能序号 
    CC_SYNTHESIZE(std::string ,m_sSpriteTalkContent,SpriteTalkContent);// 精灵对话内容 
    CC_SYNTHESIZE(std::string ,m_sSpriteImageName,SpriteImageName);// 精灵半身像图片名 
    CC_SYNTHESIZE(int16_t, m_n16RangeIndex, RangeIndex);// 与目标之间的距离按哪个攻击范围设定 
    CC_SYNTHESIZE(int, m_nAttributeIndex, AttributeIndex);//对应的属性编号 
    CC_SYNTHESIZE(std::string, m_pPortraitFileName, PortraitFileName);// boss头像 
    CC_SYNTHESIZE(int8_t, m_n8BloodBarLayerNum, BloodBarLayerNum);// 血条层数 
    CC_SYNTHESIZE(std::string, m_n8BossMarkResource, BossMarkResource);//boss脚下增加光圈作为标记 
    CC_SYNTHESIZE(int16_t, m_n16NameOffsetHeight, NameOffsetHeight);
    
private:
    
    bool m_bBeatedBack;
    
public:
    
    SpriteData()
    :m_pAttackRectArray(NULL)
    ,m_pAttackOddsArray(NULL)
    ,m_pAttackActionIndexArray(NULL)
    ,m_pAttackIntervalArray(NULL)
    ,m_pResourceArray(NULL)
    ,m_pDamageAmountArray(NULL)
    ,m_pDamageFrameIndexArray(NULL)
    ,m_pDamageIntervalArray(NULL)
    ,m_pDebuffArray(NULL)
    ,m_pDOTArray(NULL)
    ,m_pResultArray(NULL)
    ,m_pTargetReducedTenacityArray(NULL)
    ,m_pHPRangeArray(NULL)
    ,m_pNumOfUsedArray(NULL)
    ,m_pSkillIndexArray(NULL)
    ,m_pBehaviorArray(NULL)
    ,m_u16SpriteIndex(0)
    ,m_u8Type(0)
    ,m_u8Speed(0)
    ,m_nAttributeIndex(0)
    ,m_u16ConfirmBehaviorDuration(0)
    ,m_n16RangeIndex(0)
    ,m_n8BloodBarLayerNum(0)
    {
    }
    
    ~SpriteData()
    {
        CC_SAFE_RELEASE(m_pHPRangeArray);
        CC_SAFE_RELEASE(m_pNumOfUsedArray);
        CC_SAFE_RELEASE(m_pBehaviorArray);
        CC_SAFE_RELEASE(m_pSkillIndexArray);
        CC_SAFE_RELEASE(m_pResourceArray);
        CC_SAFE_RELEASE(m_pAttackRectArray);
        CC_SAFE_RELEASE(m_pAttackOddsArray);
        CC_SAFE_RELEASE(m_pAttackActionIndexArray);
        CC_SAFE_RELEASE(m_pAttackIntervalArray);
        CC_SAFE_RELEASE(m_pDamageAmountArray);
        CC_SAFE_RELEASE(m_pDamageFrameIndexArray);
        CC_SAFE_RELEASE(m_pDamageIntervalArray);
        CC_SAFE_RELEASE(m_pDebuffArray);
        CC_SAFE_RELEASE(m_pDOTArray);
        CC_SAFE_RELEASE(m_pResultArray);
        CC_SAFE_RELEASE(m_pTargetReducedTenacityArray);
    }
    
    inline bool init()
    {
        return true;
    }
    
    inline bool isBeatedBack()
    {
        return m_bBeatedBack;
    }
    
    inline void setBeatedBack(bool isBeatedBack)
    {
        m_bBeatedBack = isBeatedBack;
    }
    
    CREATE_FUNC(SpriteData);
};

#endif
