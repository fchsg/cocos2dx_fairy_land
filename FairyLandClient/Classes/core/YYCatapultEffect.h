//
//  YYCatapult.h
//  XJ
//
//  Created by fuchenhao on 13-6-13. 
//
//

#ifndef __XJ__YYCatapult__
#define __XJ__YYCatapult__

#include "YYFightSprite.h"

class SkillEffectData;

class YYCatapultEffect : public YYFightSprite
{
    CC_SYNTHESIZE(int, m_nCatapultCount, CatapultCount);// 弹射计数
//    CC_SYNTHESIZE(uint8_t, m_u8Speed,Speed);// 每帧位移速率 
    CC_SYNTHESIZE(SkillData *, m_pSkillData, SkillData);
    CC_SYNTHESIZE(SkillEffectData *, m_pSkillEffectData, SkillEffectData);
    
private:
    cocos2d::CCArray * m_pTargetArray;// 目标数组（存储可被弹射的对象） 
    YYFightSprite * m_pNextTarget;
    bool m_bSpellAttack;
    
public:
    YYCatapultEffect();
    ~YYCatapultEffect();
    CREATE_FUNC(YYCatapultEffect);
    void catapultToNextTarget(YYFightSprite * currentTarget);
    void calculateCurrentFrameDisplacement();
    void update();
    void updateTarget(cocos2d::CCArray * targetArray);
    inline void setSpellAttack(bool isSpellAttack)
    {
        m_bSpellAttack = isSpellAttack;
    }
};

#endif /* defined(__XJ__YYCatapult__) */
