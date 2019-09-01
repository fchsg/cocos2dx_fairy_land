//
//  YYSkillAction.h
//  XJ
//
//  Created by fuchenhao on 13-7-19. 
//
//

#ifndef XJ_YYSkillAction_h
#define XJ_YYSkillAction_h

#include "YYSkillActionData.h"

class Skill;
class SkillEffectData;

class SkillAction : public cocos2d::CCObject
{
    
    CC_SYNTHESIZE(uint16_t,m_u16Counter, Counter)// 技能持续时间计数器
    CC_SYNTHESIZE(uint16_t,m_u16ActionIndex, ActionIndex)// 对应精灵的动作序号
    CC_SYNTHESIZE(SkillActionData *, m_pSkillActionData, SkillActionData);//
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pEffectArray, EffectArray);// 附加特效
    CC_SYNTHESIZE(Skill *, m_pSkill, Skill);
    
private:
    
    bool m_bSkillActionOver;// 技能动作是否结束 
    bool m_bPlaySound;
    int m_nSoundID;
    
public:
    
    SkillAction();
    
    ~SkillAction();
    
    bool init();
    
    CREATE_FUNC(SkillAction);
    
    void update();
    
    inline bool isSkillActionOver()
    {
        return m_bSkillActionOver;
    }
    
    inline void setSkillActionOver(bool isActionOver)
    {
        m_bSkillActionOver = isActionOver;
    }
    
    inline void setPlaySound(bool isPlaySound)
    {
        m_bPlaySound = isPlaySound;
    }
    
    inline bool isPlaySound()
    {
        return m_bPlaySound;
    }
    
    void addSkillEffect(int currentFrameIndex);
    void stopSkillEffect();
    
    void addSoundEffect(int currentFrameIndex);
    void stopSoundEffect();
};
#endif
