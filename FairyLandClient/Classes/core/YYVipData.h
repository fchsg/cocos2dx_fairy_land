//
//  vipData.h
//  Fairyland
//
//  Created by 付辰昊 on 9/1/14.
//
//

#ifndef Fairyland_YYVipData_h
#define Fairyland_YYVipData_h

#include "cocos2d.h"

class VipData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint16_t ,m_u16Level, Level);// 等级
    CC_SYNTHESIZE(uint32_t ,m_u16JewAmount, JewAmount);// 当前VIP等级,宝石充值总数
    CC_SYNTHESIZE(std::string ,m_sDesc, Desc);// 描述
    
public:
    
    VipData()
    :m_u16Level(0)
    ,m_u16JewAmount(0)
    ,m_sDesc("")
    {
    }
    ~VipData()
    {
    }
    CREATE_FUNC(VipData);
    inline bool init()
    {
        return true;
    }

};

#endif