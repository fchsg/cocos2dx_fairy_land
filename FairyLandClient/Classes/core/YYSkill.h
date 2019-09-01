//
//  YYSkill.h
//  XJ
//
//  Created by fuchenhao on 13-7-22. 
//
//

#ifndef __XJ__YYSkill__
#define __XJ__YYSkill__

#include "YYSkillData.h"
#include "YYSkillAction.h"

class YYFightSprite;

class Skill : public cocos2d::CCObject
{
    
    CC_SYNTHESIZE(SkillData *, m_pSkillData, SkillData);
    CC_SYNTHESIZE(SkillAction *, m_pSkillAction, SkillAction);// 技能对应的动作
    CC_SYNTHESIZE(YYFightSprite *, m_pOwner, Owner);
    
private:
    
    uint8_t m_u8ActionDataIndex;
    bool m_bSkillOver;
    
public:
    
    Skill();
    ~Skill();
    
    inline bool init(){
        return true;
    }
    
    CREATE_FUNC(Skill);
    
    void initWithSkillData(SkillData * skillData);
    
    void setNextSkillAction();
    
    inline bool isSkillOver(){
        return m_bSkillOver;
    }
    
};

#endif /* defined(__XJ__YYSkill__) */
