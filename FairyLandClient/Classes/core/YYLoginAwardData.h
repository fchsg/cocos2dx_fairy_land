//
//  YYLoginAwardData.h
//  SXL
//
//  Created by 商小亮 on 14-1-28. 
//
//

#ifndef SXL_YYLoginAwardData_h
#define SXL_YYLoginAwardData_h

#include "cocos2d.h"
#include "YYAchievementData.h"

//奖励类型
enum LoginAwardStatus
{
    kLoginAwardStatusGetable = 1,   //领取
    kLoginAwardStatusGotten = 2,    //已领取
};

//签到状态类型 
enum kSignUpStatus
{
    kStatusCanNotSignUp,
    kStatusCanSignUp
};
enum kSignuPAwardStatus
{
    kStatusAwardHaveGot,
    kStatusAwardCanGet,
    kStatusAwardCanNotGet,
};


class LoginAwardData : public cocos2d::CCObject
{
public:
    LoginAwardData()
    :m_u8Index(-1)
    ,m_eStatus(kLoginAwardStatusGetable)
    ,m_bChanged(false)
    ,m_pItemInfoArray(NULL)
    ,m_pDescriptionArray(NULL)
    {
    }
    
    inline bool init()
    {
        return true;
    }
    
    CREATE_FUNC(LoginAwardData);
    
    ~LoginAwardData()
    {
        CC_SAFE_RELEASE(m_pItemInfoArray);
        CC_SAFE_RELEASE(m_pDescriptionArray);
    }

    CC_SYNTHESIZE(uint8_t ,m_u8Index, Index);                                           // index
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pItemInfoArray, ItemInfoArray);          //物品图标、数量、ID
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pDescriptionArray, DescriptionArray);    //物品数量
    CC_SYNTHESIZE(LoginAwardStatus, m_eStatus, Status);                                 //status
    
private:
    bool m_bChanged;
    
public:
    inline bool isChanged()
    {
        return m_bChanged;
    }
    inline void setChanged(bool isChanged)
    {
        m_bChanged = isChanged;
    }
    
};

class YYSignUpData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint8_t, m_u8Index, Index);
    CC_SYNTHESIZE(kSignUpStatus, m_bSignUpFinished, SignUpFinished); // 签到成功
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pArrayAwardData, ArrayAwardData); // 奖励
public:
    YYSignUpData()
    :m_u8Index(1)
    ,m_bSignUpFinished(kStatusCanNotSignUp)
    ,m_pArrayAwardData(NULL)
    {
    }
    
    inline bool init()
    {
        m_pArrayAwardData = new cocos2d::CCArray();
        return true;
    }
    
    CREATE_FUNC(YYSignUpData);
    
    ~YYSignUpData()
    {
        m_pArrayAwardData->removeAllObjects();
        CC_SAFE_DELETE(m_pArrayAwardData);
    }
};

class YYSignUpBoxAwardData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint8_t, m_u8Index, Index);
    CC_SYNTHESIZE(kSignuPAwardStatus, m_kSignuPAwardStatus, SignuPAwardStatus);
    CC_SYNTHESIZE(AwardData *, m_pAwardData, AwardData);
public:
    YYSignUpBoxAwardData()
    :m_u8Index(0)
    ,m_kSignuPAwardStatus(kStatusAwardCanNotGet)
    ,m_pAwardData(NULL)
    {
    }
    
    inline bool init()
    {
        m_pAwardData = AwardData::create();
        m_pAwardData->retain();
        return true;
    }
    
    CREATE_FUNC(YYSignUpBoxAwardData);
    
    ~YYSignUpBoxAwardData()
    {
        CC_SAFE_RELEASE_NULL(m_pAwardData);
    }
};
#endif
