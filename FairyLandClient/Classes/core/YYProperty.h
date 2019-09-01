//
//  YYProperty.h
//  XJ
//  道具 
//  Created by fuchenhao on 13-6-18. 
//
//

#ifndef XJ_YYProperty_h
#define XJ_YYProperty_h

#include "cocos2d.h"

class Property : public cocos2d::CCObject
{
    CC_SYNTHESIZE(std::string, m_sID, ID);// 物品ID
    CC_SYNTHESIZE(int, m_nIndex, Index);// 物品编号
    CC_SYNTHESIZE(std::string, m_sIconPath, IconPath);// 图标
    CC_SYNTHESIZE(int, m_nType, Type);// 物品类别
    CC_SYNTHESIZE(int, m_nQuality, Quality);// 品质 
    CC_SYNTHESIZE(int, m_nLevel, Level);//  等级
    CC_SYNTHESIZE(int, m_nPurchasePrice, PurchasePrice);// 买价 
    CC_SYNTHESIZE(int, m_nSalePrice, SalePrice);// 售价
    CC_SYNTHESIZE(int, m_nNumber, Number);// 数量 
    CC_SYNTHESIZE(int, m_nMaxNumber, MaxNumberPerCell);// 每格最大数量 
    CC_SYNTHESIZE(std::string ,m_sReport,Report);// 用于物品操作返回服务器指令
    CC_SYNTHESIZE(std::string ,m_sName,Name);// 物品名称
    CC_SYNTHESIZE(std::string ,m_sLabel,Label);// 物品类型标签
    CC_SYNTHESIZE(int ,m_nProfessionIndex,ProfessionIndex);// 物品职业限制
    CC_SYNTHESIZE(std::string ,m_sDescription,Description);// 物品说明
    CC_SYNTHESIZE(int ,m_nTimeLimit,TimeLimit);// 物品时限
//  CC_SYNTHESIZE(cocos2d::CCArray *, m_pDescriptionArray, DescriptionArray);
    
private:
    
    bool m_bExchangeable;// 是否可交易 
    bool m_bBind;// 是否绑定
    bool m_bUnique;// 是否唯一
    bool m_bDelete;// 是否需要移除
    bool m_bOpened;
    
public:
    
    Property()
    :m_nIndex(-1)
    ,m_nType(-1)
    ,m_nQuality(1)
    ,m_nLevel(0)
    ,m_nPurchasePrice(0)
    ,m_nSalePrice(0)
    ,m_nNumber(0)
    ,m_nMaxNumber(0)
    ,m_bDelete(false)
    ,m_bBind(false)
    ,m_bUnique(false)
    ,m_bExchangeable(false)
    ,m_bOpened(false)
    {
//        m_pDescriptionArray = cocos2d::CCArray::createWithCapacity(10);
//        m_pDescriptionArray->retain();
    }
    
    ~Property()
    {
//        CC_SAFE_RELEASE(m_pDescriptionArray);
    }
    
    bool init()
    {
        return true;
    }
    
    CREATE_FUNC(Property);
    
    inline bool isDelete()
    {
        return m_bDelete;
    }
    void setDelete(bool bDelete)
    {
        m_bDelete = bDelete;
    }
    inline bool isBind()
    {
        return m_bBind;
    }
    void setBind(bool bBind)
    {
        m_bBind = bBind;
    }
    
    inline bool isUnique()
    {
        return m_bUnique;
    }
    void setUnique(bool bUnique)
    {
        m_bUnique = bUnique;
    }
    
    inline bool isExchangeable()
    {
        return m_bExchangeable;
    }
    void setExchangeable(bool bExchangeable)
    {
        m_bExchangeable = bExchangeable;
    }
    inline bool isOpened()
    {
        return m_bOpened;
    }
    void setOpened(bool isOpened)
    {
        m_bOpened = isOpened;
    }
};

#endif
