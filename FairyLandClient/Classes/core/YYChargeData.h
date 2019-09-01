//
//  YYChargeUI.h
//  Fairyland
//
//  Created by yoyo on 14-5-22.
//
//

#ifndef __Fairyland__YYChargeUI__
#define __Fairyland__YYChargeUI__

#include "cocos2d.h"

class YYChargeData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint8_t ,m_uID, ID);
    CC_SYNTHESIZE(uint32_t, m_uJewelNum, JewelNum);
    CC_SYNTHESIZE(uint32_t, m_uRMBNum, RMBNum);
public:
    YYChargeData()
    :m_uID(0)
    ,m_uJewelNum(0)
    ,m_uRMBNum(0)
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(YYChargeData);
    
    ~YYChargeData()
    {
    }
    
};
#endif
