//
//  YYTaskTargetData.h
//  Fairland
//
//  Created by fuchenhao on 13-10-22. 
//
//

#ifndef Fairland_YYTaskTargetData_h
#define Fairland_YYTaskTargetData_h

#include "cocos2d.h"


class TaskTarget: public cocos2d::CCObject
{
    CC_SYNTHESIZE(std::string, m_sDialogContent, DialogContent);
    CC_SYNTHESIZE(std::string,m_sDungeonsName,DungeonsName); //副本名称
    CC_SYNTHESIZE(std::string,m_sStageName,StageName); //关卡名称
    CC_SYNTHESIZE(uint8_t,m_u8StageLevel,StageLevel); //副本等级
    CC_SYNTHESIZE(uint8_t,m_u8ClearStageAmount,ClearStageAmount); //达成目标所需通关次数
    CC_SYNTHESIZE(uint8_t,m_u8ClearStageCount,ClearStageCount); //已通关次数
    CC_SYNTHESIZE(uint16_t,m_u16TargetMonsterAmount,TargetMonsterAmount); //需杀死的目标的总数
    CC_SYNTHESIZE(uint16_t,m_u16TargetMonsterCount,TargetMonsterCount); //已杀死的目标计数
    CC_SYNTHESIZE(uint8_t,m_u8TargetItemAmount,TargetItemAmount); //达成目标所需通关次数
    CC_SYNTHESIZE(uint8_t,m_u8TargetItemCount,TargetItemCount); //已通关次数
    CC_SYNTHESIZE(float,m_fDropOdds,DropOdds); //掉落几率
    CC_SYNTHESIZE(std::string,m_sTargetID,TargetID); // 目标ID
    CC_SYNTHESIZE(std::string,m_sTargetName,TargetName); //目标名称
    CC_SYNTHESIZE(uint16_t, m_u16PlayerLevel, PlayerLevel);// 达成目标需求的玩家等级

public:
    
    TaskTarget()
    {
    }
    
    ~TaskTarget()
    {
    }
    bool init()
    {
        return true;
    }
    CREATE_FUNC(TaskTarget);
};

#endif
