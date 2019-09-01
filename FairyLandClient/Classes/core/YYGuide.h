//
//  YYGuide.h
//  Fairyland
//
//  Created by fuchenhao on 13-12-24. 
//
//

#ifndef Fairyland_YYGuide_h
#define Fairyland_YYGuide_h

#include "../animation/YYSprite.h"

class YYGuide : public YYSprite
{
    CC_SYNTHESIZE(int16_t, m_n16TouchBeganValue, TouchBeganValue);
    CC_SYNTHESIZE(int16_t, m_n16TouchEndedValue, TouchEndedValue);
    CC_SYNTHESIZE(CCRect, m_rTouchRegion, TouchRegion);// 需点击的区域
  
private:
    bool m_bTouchMoved;
    
public:
    
    YYGuide(void)
    :m_n16TouchBeganValue(-1)
    ,m_n16TouchEndedValue(-1)
    {
        setSpriteType(SPRITE_TYPE_GUIDE);
    }
    
    ~YYGuide(void)
    {
    }
    
    CREATE_FUNC(YYGuide);
    
    inline void setTouchMoved(bool isMoved)
    {
        m_bTouchMoved = isMoved;
    }
    
    inline bool isTouchMoved()
    {
        return m_bTouchMoved;
    }
};

#endif
