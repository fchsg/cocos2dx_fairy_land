//
//  YYButton.h
//  XJ
//
//  Created by fuchenhao on 13-6-19. 
//
//

#ifndef XJ_YYButton_h
#define XJ_YYButton_h

#include "cocos2d.h"
class TaskData;
class YYSkillData;
class YYButton : public cocos2d::CCSprite
{
    CC_SYNTHESIZE(int,m_nIndex,Index);
    CC_SYNTHESIZE(bool,m_bIsUsed,IsUsed);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCString*,m_pType,Type);
    CC_SYNTHESIZE(cocos2d::CCRect,m_ButtonRect,ButtonRect);
    
public:
    
    YYButton(void)
    :m_pType(NULL)
    ,m_nIndex(0)
    ,m_bIsUsed(false)
    {
    }
    
    ~YYButton(void)
    {
        CC_SAFE_RELEASE_NULL(m_pType);
    }
    
    CREATE_FUNC(YYButton);
};

#endif
