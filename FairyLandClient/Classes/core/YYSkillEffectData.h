//
//  YYSkillEffectData.h
//  XJ
//
//  Created by fuchenhao on 13-7-25. 
//
//

#ifndef __XJ__YYSkillEffectData__
#define __XJ__YYSkillEffectData__

#include "cocos2d.h"

//特效类型 
enum EffectType
{
    kTypeNormal = 0,
    kTypeBullet,
    kTypeSummon
};

//特效显示模式 
enum DisplayMode
{
    kDisplayNormal = 0,
    kDisplayScreenCenter,// 在屏幕中心显示 
    kDisplayFollowOwner,// 跟随主人一同位移（冲锋和旋风斩） 
    kDisplayRandom,// 随机出现在不同的位置 
    kDisplayInherit
};
//子弹类型 
enum BulletType
{
    kBulletTypeNormal = 0,// 命中目标后消失 
    kBulletTypePenetrate,// 穿透目标 
    kBulletTypeCatapult,// 弹射 
    kBulletTypeLink // 链击 
};

class SkillEffectData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint16_t, m_u16Index, Index);// 序号 
    CC_SYNTHESIZE(int16_t, m_n16Duration, Duration);// 时间
    CC_SYNTHESIZE(std::string, m_sResource, Resource);// 资源名称 
    CC_SYNTHESIZE(cocos2d::CCRect,m_CollisionRect,CollisionRect);// 攻击范围 
    CC_SYNTHESIZE(int8_t, m_n8LayerIndex, LayerIndex);// 层级序号 
    CC_SYNTHESIZE(uint8_t, m_u8Type, Type);// 类型 
    
    // 普通特效处理 
    CC_SYNTHESIZE(uint8_t, m_u8DamageAmount, DamageAmount);// 伤害次数 
    CC_SYNTHESIZE(uint8_t, m_u8DamageFrameIndex, DamageFrameIndex);// 从第几帧开始造成伤害 
    CC_SYNTHESIZE(uint8_t, m_u8DamageInterval, DamageInterval);// 间隔时间 
    
    // 子弹类特效处理 
    CC_SYNTHESIZE(uint8_t, m_u8OffsetXPerFrame, OffsetXPerFrame);// 子弹类型特效每帧的偏移量 
    CC_SYNTHESIZE(float, m_fAngleOfDeflection, AngleOfDeflection);// 子弹类型特效偏转角度 
    CC_SYNTHESIZE(uint8_t, m_u8BulletType, BulletType);// 子弹类型（命中后的处理方式） 0命中目标后删除子弹 1不删除 2 弹射 3链击 
    
    //召唤类特效处理 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pSummonedPositionArray, SummonedPositionArray);
    CC_SYNTHESIZE(std::string, m_sHittedEffectResource, HittedEffectResource);// 命中特效资源名称 
    CC_SYNTHESIZE(int8_t, m_n8DisplayMode, DisplayMode);//显示方式 
    CC_SYNTHESIZE(int16_t, m_n16FollowUpEffectIndex, FollowUpEffectIndex);// 后续特效编号 
    
private:
    
    bool m_bBullet;// 是否是子弹 
    bool m_bDeletedAfterHit;// 是否在命中后删除特效 
    
public:
    
    SkillEffectData()
    :m_u8OffsetXPerFrame(0)
    ,m_fAngleOfDeflection(0)
    ,m_u16Index(0)
    ,m_u8Type(0)
    ,m_n16Duration(0)
    ,m_n8LayerIndex(0)
    ,m_u8DamageAmount(0)
    ,m_u8DamageFrameIndex(0)
    ,m_u8DamageInterval(0)
    ,m_n8DisplayMode(0)
    ,m_bBullet(false)
    ,m_bDeletedAfterHit(false)
    ,m_pSummonedPositionArray(NULL)
    {
    }
    
    ~SkillEffectData()
    {
    }
    
    CREATE_FUNC(SkillEffectData);
    
    inline bool init()
    {
        return true;
    }
};

#endif /* defined(__XJ__YYSkillEffectData__) */
