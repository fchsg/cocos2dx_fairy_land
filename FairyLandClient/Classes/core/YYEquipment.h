// 
//  YYEquipment.h
//  XJ
//  装备 
//  Created by fuchenhao on 13-6-18. 
//
//

#ifndef XJ_YYEquipment_h
#define XJ_YYEquipment_h

#include "cocos2d.h"

class Equipment : public cocos2d::CCObject
{
    CC_SYNTHESIZE(std::string, m_sID, ID);// 物品ID
    CC_SYNTHESIZE(int, m_nIndex, Index);// 编号
    CC_SYNTHESIZE(std::string, m_sType, Type);// 物品类别
    CC_SYNTHESIZE(int, m_nQuality, Quality);// 品质 
    CC_SYNTHESIZE(std::string, m_sEquipName, EquipName);// 装备名称
    CC_SYNTHESIZE(int,m_nPartIndex,PartIndex);
    CC_SYNTHESIZE(int, m_nLevel, Level);// 装备等级
    CC_SYNTHESIZE(int, m_nMaxLevel, MaxLevel);// 装备可强化至MaxLevel等级
    CC_SYNTHESIZE(int, m_nPlayerLevel, PlayerLevel);// 装备需求玩家的等级
    CC_SYNTHESIZE(int, m_nComposeLevel, ComposeLevel);// 装备合成所需等级
    CC_SYNTHESIZE(int, m_nProfessionIndex, ProfessionIndex);// 装备对应职业
    CC_SYNTHESIZE(int, m_nPurchasePrice, PurchasePrice);// 买价 
    CC_SYNTHESIZE(int, m_nSalePrice, SalePrice);// 售价
    CC_SYNTHESIZE(int, m_nStrenghPoint, StrenghPoint);// 力量 
    CC_SYNTHESIZE(int, m_nAgilityPoint, AgilityPoint);// 敏捷 
    CC_SYNTHESIZE(int, m_nIntelligencePoint, IntelligencePoint);// 智力
    CC_SYNTHESIZE(int, m_nEndurancePoint, EndurancePoint);// 耐力
    CC_SYNTHESIZE(int, m_nCriticalOdds,CriticalOdds);// 暴击
    CC_SYNTHESIZE(int, m_nHitOdds,HitOdds);// 命中
    CC_SYNTHESIZE(std::string, m_pIconPath, IconPath);// 对应图标路径
    CC_SYNTHESIZE(std::string, m_sTitle, Title);// 特殊称号
    CC_SYNTHESIZE(int, m_nCombat, Combat);  //战斗力
    CC_SYNTHESIZE(int, m_nAP, AP);// 攻击力
    CC_SYNTHESIZE(int, m_nSAP, SAP);// 绝技攻击力
    CC_SYNTHESIZE(int, m_nDP, DP);// 防御
    CC_SYNTHESIZE(int, m_nMAP, MAP);// 魔法攻击力
    CC_SYNTHESIZE(int, m_nHP, HP);// 血
    CC_SYNTHESIZE(int, m_nMP, MP);// 魔
    CC_SYNTHESIZE(int, m_nIsCompose, IsCompose); //是否可锻造
    CC_SYNTHESIZE(std::string, m_sDescription , Description);// 装备描述
private:
    
    bool m_bExchangeable;// 是否可交易
    bool m_bPutOn;// 是否绑定
    bool m_bDelete;// 是否需要移除
    
public:
    
    Equipment()
    :m_nIndex(-1)
    ,m_nQuality(-1)
    ,m_nLevel(0)
    ,m_nPlayerLevel(0)
    ,m_nComposeLevel(0)
    ,m_nProfessionIndex(0)
    ,m_nPurchasePrice(0)
    ,m_nSalePrice(0)
    ,m_nStrenghPoint(0)
    ,m_nIntelligencePoint(0)
    ,m_nAgilityPoint(0)
    ,m_nEndurancePoint(0)
    ,m_nCriticalOdds(0)
    ,m_nHitOdds(0)
    ,m_bExchangeable(false)
    ,m_bPutOn(false)
    ,m_bDelete(false)
    ,m_nIsCompose(0)
    {
    }
    
    ~Equipment()
    {
    }
    
    bool init()
    {
        return true;
    }
    
    CREATE_FUNC(Equipment);
    
    inline bool isDelete()
    {
        return m_bDelete;
    }
    void setDelete(bool bDelete)
    {
        m_bDelete = bDelete;
    }
    
    inline bool isPutOn()
    {
        return m_bPutOn;
    }
    
    void setPutOn(bool bPutOn)
    {
        m_bPutOn = bPutOn;
    }

    inline bool isExchangeable()
    {
        return m_bExchangeable;
    }
    
    void setExchangeable(bool bExchangeable)
    {
        m_bExchangeable = bExchangeable;
    }
};

#endif
