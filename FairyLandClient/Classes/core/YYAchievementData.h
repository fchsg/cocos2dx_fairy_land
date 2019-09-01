//
//  YYAchievementData.h
//  Fairyland
//
//  Created by yoyo on 14-4-11.
//
//

#ifndef Fairyland_YYAchievementData_h
#define Fairyland_YYAchievementData_h

#include "cocos2d.h"

class AwardData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint32_t, m_nType, Type);
    CC_SYNTHESIZE(uint32_t, m_nIndex, Index);
    CC_SYNTHESIZE(std::string, m_sName, Name);
    CC_SYNTHESIZE(uint16_t, m_nNum, Num);
    CC_SYNTHESIZE(std::string, m_sIcon, Icon);
public:
    CREATE_FUNC(AwardData);
    AwardData()
    :m_nType(0)
    ,m_nIndex(0)
    ,m_sName("")
    ,m_nNum(0)
    ,m_sIcon("")
    {
    }
    void inline clone(AwardData *pData)
    {
        m_nType  = pData->m_nType;
        m_nIndex = pData->m_nIndex;
        m_sName  = pData->m_sName;
        m_nNum   = pData->m_nNum;
        m_sIcon  = pData->m_sIcon;
    }
    ~AwardData()
    {
    }
    inline bool init()
    {
        return true;
    }
};

class AchievementClassData: public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint32_t, m_u32Index, Index);
    CC_SYNTHESIZE(std::string, m_sName, Name);
    CC_SYNTHESIZE(uint32_t, m_u8FinishNum, FinishNum);
    CC_SYNTHESIZE(uint32_t, m_u8TotalNum, TotalNum);
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pChildDic, ChildDic); //存储分组信息
public:
    CREATE_FUNC(AchievementClassData);
    AchievementClassData()
    :m_u32Index(0)
    ,m_sName("")
    ,m_u8FinishNum(0)
    ,m_u8TotalNum(0)
    ,m_pChildDic(NULL)
    {
    }
    ~AchievementClassData()
    {
        if(m_pChildDic)
        {
            m_pChildDic->removeAllObjects();
        }
        CC_SAFE_DELETE(m_pChildDic);
    }
    inline bool init()
    {
        m_pChildDic = new cocos2d::CCDictionary();
        return true;
    }
};

class AchievementGroupData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint32_t, m_u32Index, Index);
    CC_SYNTHESIZE(std::string, m_sName, Name);
    CC_SYNTHESIZE(uint32_t, m_u8FinishNum, FinishNum);
    CC_SYNTHESIZE(uint32_t, m_u8TotalNum, TotalNum);
    CC_SYNTHESIZE(uint8_t, m_u8GetStatus, GetStatus);
    CC_SYNTHESIZE(std::string, m_sDesc, Desc);
    CC_SYNTHESIZE(AwardData *, m_pAwardData, AwardData);
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pChildDic, ChildDic); //存储分项信息
public:
    CREATE_FUNC(AchievementGroupData);
    AchievementGroupData()
    :m_u32Index(0)
    ,m_sName("")
    ,m_u8FinishNum(0)
    ,m_u8TotalNum(0)
    ,m_u8GetStatus(0)
    ,m_sDesc("")
    ,m_pAwardData(NULL)
    ,m_pChildDic(NULL)
    {
    }
    ~AchievementGroupData()
    {
        if(m_pChildDic)
        {
          m_pChildDic->removeAllObjects();
        }
        CC_SAFE_DELETE(m_pChildDic);
        CC_SAFE_RELEASE_NULL(m_pAwardData);
    }
    inline bool init()
    {
        m_pAwardData = AwardData::create();
        m_pAwardData->retain();
        m_pChildDic = new cocos2d::CCDictionary();
        return true;
    }
};

class  AchievementItemData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint32_t, m_u32Index, Index);
    CC_SYNTHESIZE(std::string, m_sName, Name);
    CC_SYNTHESIZE(uint32_t, m_nCurNum, CurNum);
    CC_SYNTHESIZE(uint32_t, m_nNum, Num);
    CC_SYNTHESIZE(uint8_t , m_u8GetStatus, GetStatus);// state : 0未完成，1完成可领奖励，2完成已经领奖励
    CC_SYNTHESIZE(std::string, m_sTime, Time);
    CC_SYNTHESIZE(std::string, m_sDesc, Desc);
    CC_SYNTHESIZE(AwardData *, m_pAwardData, AwardData);
public:
    CREATE_FUNC(AchievementItemData);
    AchievementItemData()
    :m_u32Index(0)
    ,m_sName("")
    ,m_nCurNum(0)
    ,m_nNum(0)
    ,m_u8GetStatus(0)
    ,m_sTime("")
    ,m_sDesc("")
    ,m_pAwardData(NULL)
    {
    }
    ~AchievementItemData()
    {
        CC_SAFE_RELEASE_NULL(m_pAwardData);
    }
    inline bool init()
    {
        m_pAwardData = AwardData::create();
        m_pAwardData->retain();
        return true;
    }
};
#endif
