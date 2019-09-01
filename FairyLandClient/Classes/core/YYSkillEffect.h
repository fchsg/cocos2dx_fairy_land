//
//  YYSkillEffect.h
//  XJ
//
//  Created by fuchenhao on 13-7-18. 
//
//

#ifndef __XJ__YYSkillEffect__
#define __XJ__YYSkillEffect__

#include "YYFightSprite.h"
#include "YYSkillEffectData.h"

class YYFightSprite;
class YYSkillAction;
class SkillData;

enum TargetType
{// 目标类型 
    kTargetTypeEnemy = 0,
    kTargetTypeFriend = 1,
    kTargetTypeAll = 2
};

class YYSkillEffect : public YYFightSprite
{
    CC_SYNTHESIZE(SkillEffectData *,m_pSkillEffectData, SkillEffectData)// 技能特效数据 
    CC_SYNTHESIZE(SkillData *, m_pSkillData, SkillData);
    CC_SYNTHESIZE(TargetType, m_eTargetType, TargetType);// 目标类型 0 针对敌方单位 1 针对友方单位
    
private:
    
    int m_nFrameCounter;// 游戏帧计数器（间隔50ms） 
    cocos2d::CCRect attackRect;// 攻击框 
    bool m_bSpellAttack;// 是否为法术攻击 
    
public:
    
    YYSkillEffect();
    ~YYSkillEffect();
    
    CREATE_FUNC(YYSkillEffect);
    static YYSkillEffect * createWithData(SkillEffectData *skillEffectData, SkillData *skillData,YYFightSprite *owner);
    static YYSkillEffect * createWithData(SkillEffectData *skillEffectData, YYSkillEffect *skillEffect);
    void update();
    void updateDamage();
};

#endif /* defined(__XJ__YYSkillEffect__) */
