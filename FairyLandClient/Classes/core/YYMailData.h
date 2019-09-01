//
//  YYMailUI.h
//  Fairyland
//
//  Created by yoyo on 14-6-4.
//
//

#ifndef __Fairyland__YYMailData__
#define __Fairyland__YYMailData__

#include "cocos2d.h"

class YYMailData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint16_t, m_uIndex, Index);
    CC_SYNTHESIZE(std::string, m_sPostName, PostName);
    CC_SYNTHESIZE(std::string, m_sTitle, Title);
    CC_SYNTHESIZE(uint8_t, m_uReadState, ReadState);
    CC_SYNTHESIZE(std::string, m_sPostTime, PostTime);
public:
    YYMailData()
    :m_uIndex(0)
    ,m_sPostName("")
    ,m_sTitle("")
    ,m_uReadState(0)
    ,m_sPostTime("")
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(YYMailData);
    
    ~YYMailData()
    {
    }
    
};//[发件人];[标题];[读取状态];[发送时间]]

class YYMailDetailData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint16_t, m_uIndex, Index);
    CC_SYNTHESIZE(std::string, m_sPostInx, PostInx);
    CC_SYNTHESIZE(std::string, m_sPostName, PostName);
    CC_SYNTHESIZE(std::string, m_sTitle, Title);
    CC_SYNTHESIZE(std::string, m_sContent, Content);
    CC_SYNTHESIZE(CCArray *, m_pArrayAccessory, ArrayAccessory);
    CC_SYNTHESIZE(uint8_t, m_uReadState, ReadState);
    CC_SYNTHESIZE(std::string, m_sPostTime, PostTime);
public:
    YYMailDetailData()
    :m_uIndex(0)
    ,m_sPostInx("")
    ,m_sPostName("")
    ,m_sTitle("")
    ,m_sContent("")
    ,m_pArrayAccessory(NULL)
    ,m_uReadState(0)
    ,m_sPostTime("")
    {
    }
    
    inline bool init()
    {
        m_pArrayAccessory = new CCArray();
        return true;
    }
    
    CREATE_FUNC(YYMailDetailData);
    
    ~YYMailDetailData()
    {
        if (m_pArrayAccessory)
        {
            m_pArrayAccessory->removeAllObjects();
            CC_SAFE_DELETE(m_pArrayAccessory);
        }
    }
};//[邮件index]_[发件人]_[标题]_[内容]_[附件]_[读取状态]_[发送时间]
class YYItemData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(std::string, m_sNum, Num);
    CC_SYNTHESIZE(std::string, m_sIcon, Icon);
    CC_SYNTHESIZE(int , m_ntype, Type);  //1 物品 0 其他
public:
    CREATE_FUNC(YYItemData);
    YYItemData()
    :m_sNum("")
    ,m_sIcon("")
    ,m_ntype(0)
    {
    }
    ~YYItemData()
    {
    }
    inline bool init()
    {
        return true;
    }
};
#endif
