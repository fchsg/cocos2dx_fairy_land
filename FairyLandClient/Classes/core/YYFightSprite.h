//
//  YYFightSprite.h
//  XJ
//  战斗精灵 
//  Created by fuchenhao on 13-4-24. 
//
//

#ifndef XJ_YYFightSprite_h
#define XJ_YYFightSprite_h

#include "../animation/YYSprite.h"
#include "YYSkill.h"

#define H_DISTANCE 80
#define V_DISTANCE 20

#define BASIC_HIT_ODDS 70
#define BASIC_DODGE_ODDS 25
#define BASIC_CRITICAL_ODDS 25

#define BLOOD_RESERVOIR_X 21 // 血槽最左端 
#define BLOOD_RESERVOIR_Y 1 // 血槽相对屏幕上端的距离 
#define MANA_RESERVOIR_X 20 // 法力槽最左端 
#define MANA_RESERVOIR_Y -20 // 法力槽相对屏幕上端的距离 

struct Debuff
{
    int8_t type;// 类型 0 
    int16_t duration;// 持续时间 
    int16_t value;// 属性减少值 
};

struct DOT
{
    int8_t type;// 类型 1 灼烧 2 中毒 
    int16_t duration;// 持续时间 
    int16_t DPS;// 每秒造成的伤害 
    int8_t layerIndex;// 当前层数 
};

enum kHittedStatus
{
    kHittedNone = 0,
    kHittedFirst = 1,
    kHittedFirstOver
};
class YYScene;
class SkillData;
class YYSkillAction;
class SpriteAttribute;
class YYRect;

class YYFightSprite : public YYSprite
{
    CC_SYNTHESIZE(SpriteAttribute *, m_pSpriteAttribute, SpriteAttribute);// 精灵属性 
    CC_SYNTHESIZE(YYFightSprite *, m_pTargetSprite, TargetSprite);// 攻击目标 
    CC_SYNTHESIZE(int16_t, m_n16OwnerID, OwnerID);
    CC_SYNTHESIZE(int, m_nTempOffsetY, TempOffsetY);// 临时偏移量（受击浮空） 
    CC_SYNTHESIZE(int, m_nFloatCounter, FloatCounter);// 浮空计数 
    CC_SYNTHESIZE(int16_t, m_n16Speed, Speed);
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pHatredPointDictionary, HatredPointDictionary);// 仇恨<仇恨值，仇恨对象ID> 
    CC_SYNTHESIZE(int8_t, m_n8AttackIndex, AttackIndex);// 普通攻击索引 
    CC_SYNTHESIZE(uint8_t, m_u8AttackAmount, AttackAmount);// 普通攻击有几种攻击方式 
    CC_SYNTHESIZE(uint32_t, m_uCurrentHP, CurrentHP);// 生命值 
    CC_SYNTHESIZE(uint32_t, m_uCurrentMP, CurrentMP);// 魔力值 
    CC_SYNTHESIZE(uint16_t, m_u16Tenacity, Tenacity);// 韧性 
    CC_SYNTHESIZE_READONLY(Skill *, m_pSkill, Skill);// 在正使用的技能 
    CC_SYNTHESIZE(int16_t, m_n16OffsetXBySkill, OffsetXBySkill);// 释放技能时每帧位移 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pEffectIDArray, EffectIDArray);// 存储释放的所有技能特效(ID)，死亡时一并清除 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pFootmanIDArray, FootmanIDArray); // 存储仆从ID 
    CC_SYNTHESIZE(int, m_nDamageCounter, DamageCounter);// 已造成的伤害次数
    CC_SYNTHESIZE(bool, m_bTrialPause, TrialPause);// 试炼等待fight动画结束
    CC_SYNTHESIZE(int, m_nDamageFrameIndex, DamageFrameIndex);// 起始伤害值序号
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pSkillArray, SkillArray);// 存储释放的全部技能
    
protected:
    
    YYScene * m_pScene;
    int8_t m_n8CurrentBehaviorIndex;//当前行为序号 0 站立不动 1 追踪目标 2 随机移动 
    uint16_t *m_pAttackIntervalCounter;//间隔计数器 
    int16_t m_u16ConfirmBehaviorCounter; // 重新确定行为逻辑的计数器 
    bool *m_pAttackable;// 可攻击 
    bool m_bAttackable;// 可被攻击 
    float m_fSlope;// 随机移动模式自定义位移方向上的斜率 
    
    std::vector<Debuff> m_vDebuff;// 减益效果 
    bool m_bAkinesia;// 不可行动（不能攻击） 
    bool m_bImmovability;// 不能移动 
    bool m_bRunAround;// 乱窜 
    
    std::vector<DOT> m_vDOT;// 持续伤害特效 
    int8_t m_n8CurrentLayerNumber[2];// 当前层数
    kHittedStatus hittedStatus;
public:
    
    YYFightSprite(void);
    virtual ~YYFightSprite(void);
    virtual void switchAction(uint16_t animationID);// 切换动作 
    virtual void move(void)
    {
    }
    virtual void update(void);

    void burning(int duration);
    void poison(int duration);
    void dodge();
    void showDamage(int damagePoint,bool isCritical);
    virtual void die();
    bool isCritical();// 暴击 
    bool isDodge();// 闪避 
    bool isHit();// 命中率 
    int calcDamagePoint(YYFightSprite * target,float damageFactor1 = 1.0f,float damageFactor2 = 1.0f, int extraDamage = 0,float skillDamageFactor = 0);
    bool isInRangeOfAttack(YYFightSprite * attacker, YYRect *attackRect);//是否在攻击范围内 
    void damageTarget(YYFightSprite *target, SkillData * skillData = NULL,const char * hitEffectResource = NULL);
    //技能相关 
    void createSkillWithData(SkillData * skillData);
    void setSkillAction();// 播放技能动画及特效 
    void skillOver();
    void updateSkillAction();// 更新技能动作 
    void addDebuff(int type,int duration,int value);// 添加减益特效 
    void addDot(int type,int duration,int value);// 添加持续伤害特效 
    void updateDebuff();// 更新减益效果 
    void updateDOT();// 更新持续伤害效果
    
};

#endif
