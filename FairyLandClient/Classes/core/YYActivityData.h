//
//  YYActivityData.h
//  Fairyland
//
//  Created by yoyo on 14-3-17.
//
//

#ifndef Fairyland_YYActivityData_h
#define Fairyland_YYActivityData_h

#include "cocos2d.h"

class  YYActivityTaskData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint8_t, m_sID, ID);// 任务ID
    CC_SYNTHESIZE(uint32_t, m_sAwardScore, AwardScore);// 奖励分数
    CC_SYNTHESIZE(uint8_t, m_sType, Type);// 任务类型
    CC_SYNTHESIZE(uint8_t, m_sTotalNum, TotalNum);// 需要完成总数
    CC_SYNTHESIZE(int32_t, m_sEnterIndex, EnterIndex);// 任务入口 -1 程序填 306 传送门
    CC_SYNTHESIZE(std::string, m_sDescription, Description);// 任务描述
    CC_SYNTHESIZE(uint8_t, m_sFinishNum, FinishNum);// 已完成数

public:
    CREATE_FUNC(YYActivityTaskData);
    
    YYActivityTaskData()
    {
    }
    
    inline bool init()
    {
        return true;
    }
    ~YYActivityTaskData()
    {
    }
};
class  YYActivityWishTreeData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint8_t, m_nIndex, Index);// 索引
    CC_SYNTHESIZE(uint8_t , m_nNeedScore, NeedScore);// 领取宝箱需要分数
    CC_SYNTHESIZE(uint8_t , m_nType, Type);// 类型
    CC_SYNTHESIZE(uint8_t , m_nNum, Num); // 奖励数量
    CC_SYNTHESIZE(uint32_t, m_nItemIdx, ItemIdx); //道具索引
    CC_SYNTHESIZE(std::string, m_sIconName, IconName); // icon
    CC_SYNTHESIZE(uint8_t , m_nGetStatus, GetStatus); // 0 不可领取 1 未领取 2 已领取

public:
    CREATE_FUNC(YYActivityWishTreeData);
    
    YYActivityWishTreeData()
    :m_nIndex(0)
    ,m_sIconName("")
    ,m_nGetStatus(0)
    ,m_nNeedScore(0)
    ,m_nType(0)
    ,m_nNum(0)
    ,m_nItemIdx(0)
    {
    }
    inline bool init()
    {
        return true;
    }
    ~YYActivityWishTreeData()
    {
    }
};
class YYActivityBoxContentData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(int, m_nIndex, Index);// 索引
    CC_SYNTHESIZE(int, m_nType, Type);// 类型 -1:空， 0：moeny， 1：jew， 2：其他物品
    CC_SYNTHESIZE(int, m_nNum, Num);// 数量
    CC_SYNTHESIZE(std::string, m_sName, Name);// 名字
    CC_SYNTHESIZE(std::string, m_sIcon, Icon);// 图片
public:
    CREATE_FUNC(YYActivityBoxContentData);
    
    YYActivityBoxContentData()
    :m_nIndex(-1)
    ,m_nType(-1)
    ,m_nNum(0)
    ,m_sName("")
    ,m_sIcon("")
    {
    }
    inline bool init()
    {
        return true;
    }
    ~YYActivityBoxContentData()
    {
    }
};
#endif
