//
//  YYActionData.h
//  Datang
//
//  Created by fuchenhao on 13-2-4. 
//
//

#ifndef __Datang__YYActionData__
#define __Datang__YYActionData__

#include "cocos2d.h"

class ActionData : public cocos2d::CCObject{

    CC_SYNTHESIZE(uint16_t, m_u16StartFrameIndex, StartFrameIndex);// 起始帧序号 
    CC_SYNTHESIZE(uint16_t, m_u16EndFrameIndex, EndFrameIndex);// 终止帧序号 
    
public:
    
    ActionData()
    :m_u16StartFrameIndex(0)
    ,m_u16EndFrameIndex(0)
    {
    }
    
    inline bool init(){
        return true;
    }
    
    CREATE_FUNC(ActionData);
};

#endif /* defined(__Datang__YYActionData__) */
