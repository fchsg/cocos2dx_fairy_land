//
//  YYTask.h
//  Fairland
//
//  Created by fuchenhao on 13-10-23. 
//
//

#ifndef __Fairland__YYTask__
#define __Fairland__YYTask__

#include "cocos2d.h"

enum TaskTargetType
{
    kTaskDefault = 0,
    kTaskTalk = 1,
    kTaskKillMonster = 2,
    kTaskCollect = 3,
    kTaskPassStage = 4,
    kTaskLevelUp = 5
};

enum TaskStatus
{
    kTaskStatusAccept = 0,// 接受
    kTaskStatusContinue ,// 继续
    kTaskStatusOver ,// 完成
    kTaskStatusFinding// 寻路
};

class TaskData: public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint32_t, m_uTaskIndex, TaskIndex);//任务编号
    CC_SYNTHESIZE(std::string, m_sTitle, Title);//任务标题
    CC_SYNTHESIZE(std::string, m_sDescription, Description);//描述
    CC_SYNTHESIZE(uint32_t, m_uNeedLevel, NeedLevel); // 需求等级
    CC_SYNTHESIZE(std::string, m_sTaskType, TaskType);//任务类型 0主线 1支线 2每日
    CC_SYNTHESIZE(std::string, m_sNPCID, NPCID);// 交接任务NPCID
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pItemAwardArray, ItemAwardArray);
    CC_SYNTHESIZE(int32_t, m_nGoldAwardNum, GoldAwardNum);
    CC_SYNTHESIZE(int32_t, m_nJewelAwardNum, JewelAwardNum);
    CC_SYNTHESIZE(int32_t, m_nExpAwardNum, ExpAwardNum);
    CC_SYNTHESIZE(uint8_t, m_u8Status, Status);// 状态
    CC_SYNTHESIZE(uint8_t, m_u8TargetType, TargetType);//目标类型 1对话 2杀怪 3收集 4通关 5升级
    CC_SYNTHESIZE(int, m_nTargetTileIndex, TargetTileIndex);//目的地所在格序号
    CC_SYNTHESIZE(std::string, m_sTargetIcon, TargetIcon); //目标图标
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pTargetArray, TargetArray);
public:
    
    TaskData()
    :m_u8Status(0)//默认为未接受
    ,m_uNeedLevel(0)
    {
        m_pItemAwardArray = cocos2d::CCArray::createWithCapacity(3);
        m_pItemAwardArray->retain();
        m_pTargetArray = cocos2d::CCArray::createWithCapacity(3);
        m_pTargetArray->retain();
    }
    
    ~TaskData()
    {
        CC_SAFE_RELEASE(m_pItemAwardArray);
        CC_SAFE_RELEASE(m_pTargetArray);
    }
    bool init()
    {
        return true;
    }
    CREATE_FUNC(TaskData);
};

#endif /* defined(__Fairland__YYTask__) */
