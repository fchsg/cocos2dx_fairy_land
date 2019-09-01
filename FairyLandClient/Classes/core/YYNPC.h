//
//  YYNPC.h
//  Fairland
//
//  Created by fuchenhao on 13-10-28. 
//
//

#ifndef __Fairland__YYNPC__
#define __Fairland__YYNPC__

#include "../animation/YYSprite.h"
class YYTaskTargetData;
class YYScene;

enum MarkType
{
    kMarkTypeNull = 0,
    kMarkTypeExcalmatory = 1,
    kMarkTypeQuestion = 2
};

class YYNPC : public YYSprite
{
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pTaskDataDict, TaskDataDict); //存储与NPC相关的任务
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pTaskDataArray, TaskDataArray); //存储与NPC相关的任务
    CC_SYNTHESIZE(std::string, m_sNPCID, NPCID);// 向服务器请求NPC任务列表或功能列表需发送的ID
public:
    
    YYNPC();
    ~YYNPC();
    CREATE_FUNC(YYNPC);
    virtual void update(void);
    void move();
    void updateTaskStatus();
    
private:
    
    YYScene *m_pScene;
    bool m_bStandBy;
    int m_nStandByCounter;// 待机计数器 
    int m_nStandByDuration;// 待机持续时间 
};

#endif /* defined(__Fairland__YYNPC__) */
