//
//  YYProtossData.h
//  SXL
//
//  Created by 商小亮 on 14-1-20. 
//
//

#ifndef SXL_YYProtossData_h
#define SXL_YYProtossData_h

#include "cocos2d.h"

class YYProtossData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(int, m_nPosID, PosID);                        // 星灵所在的位置ID
    CC_SYNTHESIZE(int, m_nType1, Type1);                        // 星灵类型1
    CC_SYNTHESIZE(int, m_nType2, Type2);                        // 星灵类型2
    CC_SYNTHESIZE(int ,m_nID, ID);                              // 星灵编号
    CC_SYNTHESIZE(std::string, m_sName, Name);                  // 星灵名称
    CC_SYNTHESIZE(int, m_nExp, Exp);                            // 星灵初始经验
    CC_SYNTHESIZE(int, m_nCurrentExp, CurrentExp);              // 星灵当前经验
    CC_SYNTHESIZE(int, m_nPhagocytic, Phagocytic);              // 星灵吞噬初始经验
    CC_SYNTHESIZE(int, m_nNext, Next);                          // 下一级星灵
    CC_SYNTHESIZE(int, m_nLevel, Level);                        // 星灵等级
    CC_SYNTHESIZE(int, m_nQuality, Quality);                    // 星灵品级
    CC_SYNTHESIZE(int, m_nNeedLevel, NeedLevel);                // 星灵所需等级
    CC_SYNTHESIZE(std::string, m_sIcon, Icon);                  // 星灵图标
    CC_SYNTHESIZE(std::string, m_sDesc, Desc);                  // 描述
    CC_SYNTHESIZE(int, m_nPrice, Price);                        // 出售价格
    CC_SYNTHESIZE(int, m_nStrength, Strength);                  // 力量
    CC_SYNTHESIZE(int, m_nAgility, Agility);                    // 敏捷
    CC_SYNTHESIZE(int, m_nStamina, Stamina);                    // 耐力
    CC_SYNTHESIZE(int, m_nInte, Inte);                          // 智力
    CC_SYNTHESIZE(int, m_nAttack, Attack);                      // 物攻
    CC_SYNTHESIZE(int, m_nPDefense, PDefense);                  // 物防
    CC_SYNTHESIZE(int, m_nMagic, Magic);                        // 魔攻
    CC_SYNTHESIZE(int, m_nMDefense, MDefense);                  // 魔防
    CC_SYNTHESIZE(int, m_nHP, HP);                              // 生命值
    CC_SYNTHESIZE(int, m_nMP, MP);                              // 法力值
    CC_SYNTHESIZE(int, m_nHit, Hit);                            // 命中等级
    CC_SYNTHESIZE(int, m_nCrit, Crit);                          // 暴击等级
    CC_SYNTHESIZE(std::string, m_nTitle, Title);
    CC_SYNTHESIZE(uint8_t, m_nColorType, ColorType)              // 文字颜色类型
    CC_SYNTHESIZE(int, m_nExchangeNum, ExchangeNum);          // 兑换需要碎片数量
    CC_SYNTHESIZE(int, m_nExchangePieceID, ExchangePieceID);  // 需要兑换的碎片的类型的id
    CC_SYNTHESIZE(bool, m_bIsSwallowed, IsSwallowed);  // 是否被选中将吞噬
    CC_SYNTHESIZE(int, m_nUnLockLevel, UnLockLevel); // 星灵解锁等级
public:
    YYProtossData()
    :m_nPosID(0)
    ,m_nID(0)
    ,m_sName("")
    ,m_nType1(-1)
    ,m_nType2(-1)
    ,m_nExp(0)
    ,m_nPhagocytic(0)
    ,m_nNext(0)
    ,m_nLevel(1)
    ,m_nQuality(0)
    ,m_nNeedLevel(0)
    ,m_sIcon("")
    ,m_sDesc("")
    ,m_nPrice(0)
    ,m_nStrength(0)
    ,m_nAgility(0)
    ,m_nStamina(0)
    ,m_nInte(0)
    ,m_nAttack(0)
    ,m_nPDefense(0)
    ,m_nMagic(0)
    ,m_nMDefense(0)
    ,m_nHP(0)
    ,m_nMP(0)
    ,m_nHit(0)
    ,m_nCrit(0)
    ,m_nTitle("")
    ,m_nColorType(0)
    ,m_nCurrentExp(0)
    ,m_bIsSwallowed(false)
    ,m_nUnLockLevel(0)
    {
    }
    CREATE_FUNC(YYProtossData);
    inline bool init()
    {
        return true;
    }
    inline void setValue(std::string str, int nIndex)
    {
        switch (nIndex)
        {
            case 0:
                m_nPosID = atoi(str.c_str());
                break;
            case 1:
                m_nID = atoi(str.c_str());
                break;
            case 2:
                m_sName = str;
                break;
            case 3:
                m_nType1 = atoi(str.c_str());
                break;
            case 4:
                m_nType2 = atoi(str.c_str());
                break;
            case 5:
                m_nExp = atoi(str.c_str());
                break;
            case 6:
                m_nPhagocytic = atoi(str.c_str());
                break;
            case 7:
                m_nNext = atoi(str.c_str());
                break;
            case 8:
                m_nLevel = atoi(str.c_str());
                break;
            case 9:
                m_nQuality = atoi(str.c_str());
                break;
            case 10:
                m_nNeedLevel = atoi(str.c_str());
                break;
            case 11:
                m_sIcon = str;
                break;
            case 12:
                m_sDesc = str;
                break;
            case 13:
                m_nPrice = atoi(str.c_str());
                break;
            case 14:
                m_nStrength = atoi(str.c_str());
                break;
            case 15:
                m_nAgility = atoi(str.c_str());
                break;
            case 16:
                m_nStamina = atoi(str.c_str());
                break;
            case 17:
                m_nInte = atoi(str.c_str());
                break;
            case 18:
                m_nAttack = atoi(str.c_str());
                break;
            case 19:
                m_nPDefense = atoi(str.c_str());
                break;
            case 20:
                m_nMagic = atoi(str.c_str());
                break;
            case 21:
                m_nMDefense = atoi(str.c_str());
                break;
            case 22:
                m_nHP = atoi(str.c_str());
                break;
            case 23:
                m_nMP = atoi(str.c_str());
                break;
            case 24:
                m_nHit = atoi(str.c_str());
                break;
            case 25:
                m_nCrit = atoi(str.c_str());
                break;
            case 26:
                m_nTitle = str;
                break;
            case 27:
                m_nCurrentExp = atoi(str.c_str());
                break;
            default:
                break;
        }
    }
    inline void clone(YYProtossData *pSurData)
    {
        m_nID = pSurData->getID();
        m_sName = pSurData->getName();
        m_nType1 = pSurData->getType1();
        m_nType2 = pSurData->getType2();
        m_nExp = pSurData->getExp();
        m_nPhagocytic = pSurData->getPhagocytic();
        m_nNext = pSurData->getNext();
        m_nLevel = pSurData->getLevel();
        m_nQuality = pSurData->getQuality();
        m_nNeedLevel = pSurData->getNeedLevel();
        m_sIcon = pSurData->getIcon();
        m_sDesc = pSurData->getDesc();
        m_nPrice = pSurData->getPrice();
        m_nTitle = pSurData->getTitle();
        m_nColorType = pSurData->getColorType();
        m_nCurrentExp = pSurData->getCurrentExp();
        m_nUnLockLevel = pSurData->getUnLockLevel();
    }
    ~YYProtossData()
    {
    }
};

class YYProtossPieceData : public cocos2d::CCObject
{
public:
    YYProtossPieceData()
    :m_nProtossPieceID(0)
    ,m_sIconName("")
    ,m_nNum(0)
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(YYProtossPieceData);
    
    ~YYProtossPieceData()
    {
    }
    
    CC_SYNTHESIZE(int ,m_nProtossPieceID, ProtossPieceID);      // 星灵碎片编号
    CC_SYNTHESIZE(std::string, m_sIconName, IconName);          // 星灵碎片图标
    CC_SYNTHESIZE(int, m_nNum, Num);                            // 数量
};
class YYConstellationData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint8_t ,m_u8ConstellationID, ConstellationID);      // 星座编号
    CC_SYNTHESIZE(float, m_f8NextRate, NextRate);
    CC_SYNTHESIZE(float, m_f8GrayRate, GrayRate);
    CC_SYNTHESIZE(float, m_f8WhiteRate, WhiteRate);
    CC_SYNTHESIZE(float, m_f8GreenRate, GreenRate);
    CC_SYNTHESIZE(float, m_f8BlueRate, BlueRate);
    CC_SYNTHESIZE(float, m_f8PurpleRate, PurpleRate);
    CC_SYNTHESIZE(float, m_f8ScrapRate, ScrapRate);
    CC_SYNTHESIZE(int ,m_u8ProtossMoeny, ProtossMoeny);      // 猎命花费金币
    CC_SYNTHESIZE(int ,m_u8LightConstellationJew, LightConstellationJew);     // 点亮星座
public:
    YYConstellationData()
    :m_u8ConstellationID(0)
    ,m_f8NextRate(0)
    ,m_f8GrayRate(0)
    ,m_f8WhiteRate(0)
    ,m_f8GreenRate(0)
    ,m_f8BlueRate(0)
    ,m_f8PurpleRate(0)
    ,m_f8ScrapRate(0)
    ,m_u8ProtossMoeny(0)
    ,m_u8LightConstellationJew(0)
    
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(YYConstellationData);
    
    ~YYConstellationData()
    {
    }

};
#endif
