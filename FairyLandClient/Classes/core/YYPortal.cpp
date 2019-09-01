//
//  YYPortal.cpp
//  Fairyland
//
//  Created by fuchenhao on 13-12-20. 
//
//

#include "YYPortal.h"

YYPortal::YYPortal()
{
    
}

YYPortal::~YYPortal()
{
    
}

void YYPortal::update(void)
{
    updateAnimation();
    if (m_bActionOver) {
        switch (m_u8ActionIndex)
        {
            case ACTION_STAND0:
                m_u16CurrentFrameIndex = getCurrentActionData()->getStartFrameIndex();
                setFrame(m_u16CurrentFrameIndex);
                m_bActionOver = false;
                break;
            case ACTION_RUN:
                setAction(ACTION_STAND0, false);
                break;
            default:
                break;
        }
    }
}
