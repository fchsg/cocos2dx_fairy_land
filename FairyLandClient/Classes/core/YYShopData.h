//
//  YYShopData.h
//  SXL
//
//  Created by 商小亮 on 14-1-10. 
//
//

#ifndef SXL_YYShopData_h
#define SXL_YYShopData_h

#include "cocos2d.h"
#include "YYPopUpUI.h"

//代币类型 
enum kMoneyType
{
    kMoneyTypeYoudian = 0,     //幽点 
    kMoneyTypePoint,           //积分 
    kMoneyTypeGold,            //金币 
    kMoneyTypeHonor,           //荣誉 
};

//时间限制类型 
enum kTimeLimitType
{
    kTimeLimitTypeWait = -2,
    kTimeLimitTypePass = -1,
    kTimeLimitTypeNormal = 0,
};

class YYShopData : public cocos2d::CCObject
{
public:
    YYShopData()
    :m_u32Index(0)
    ,m_sIconName("")
    ,m_sName("")
    ,m_eShopItemType(kShopHotPage)
    ,m_u16Num(0)
    ,m_sDesc("")
    ,m_u16Amount(0)
    ,m_u32PriceJew(0)
    ,m_u32PriceMoney(0)
    ,m_eDateType(kTimeLimitTypeNormal)
    ,m_eTimeType(kTimeLimitTypeNormal)
    {
    }
    
    inline bool init()
    {
        return true;
    }
    inline bool isPurchase()
    {
       // struct tm *tm = Controller::getInstance()->getSystemTime();
       // CCLOG("%d, %d, %d, %d-%d-%d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
        return true;
    }
    CREATE_FUNC(YYShopData);
    
    ~YYShopData()
    {
    }

    CC_SYNTHESIZE(uint32_t ,m_u32Index, Index);           // 商品index 
    CC_SYNTHESIZE(std::string, m_sIconName, IconName);    //icon
    CC_SYNTHESIZE(std::string, m_sName, Name);            //name
    CC_SYNTHESIZE(ShopPageType, m_eShopItemType, ShopItemType);    // 商品所属类型
    CC_SYNTHESIZE(uint16_t, m_u16Num, Num);               //数量
    CC_SYNTHESIZE(std::string, m_sDesc, Desc);            //描述 
    CC_SYNTHESIZE(uint16_t, m_u16Amount, Amount);         //数量限制
    CC_SYNTHESIZE(uint32_t, m_u32PriceJew, PriceJew);     //价格（宝石）
    CC_SYNTHESIZE(uint32_t, m_u32PriceMoney, PriceMoney); //价格（金币）
    CC_SYNTHESIZE(kTimeLimitType, m_eDateType, DateType);
    CC_SYNTHESIZE(kTimeLimitType, m_eTimeType, TimeType);
    CC_SYNTHESIZE(std::string, m_sDateContentStart, DateContentStart);
    CC_SYNTHESIZE(std::string, m_sTimeContentStart, TimeContentStart);
    CC_SYNTHESIZE(std::string, m_sDateContentEnd, DateContentEnd);
    CC_SYNTHESIZE(std::string, m_sTimeContentEnd, TimeContentEnd);
};

#endif
