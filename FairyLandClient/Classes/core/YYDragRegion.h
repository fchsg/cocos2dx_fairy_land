//
//  YYDragRegion.h
//  XJ
//
//  Created by fuchenhao on 13-6-21. 
//
//

#ifndef XJ_YYDragRegion_h
#define XJ_YYDragRegion_h

#include "cocos2d.h"

class YYDragRegion : public cocos2d::CCSprite
{
    CC_SYNTHESIZE(int,m_nIndex,Index);
    CC_SYNTHESIZE(int,m_nItemID,ItemID);
    CC_SYNTHESIZE(int,m_nItemTag,ItemTag);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCString*,m_sPart,Part);
    CC_SYNTHESIZE_RETAIN(cocos2d::CCString*,m_sType,Type);
    CC_SYNTHESIZE(cocos2d::CCRect,m_DragRegionRect,DragRegionRect);
    
public:
    
    YYDragRegion()
    :m_nItemID(-1)
    ,m_nIndex(-1)
    ,m_nItemTag(-1)
    ,m_sType(NULL)
    ,m_sPart(NULL)
    {}
    ~YYDragRegion(){
        CC_SAFE_RELEASE_NULL(m_sType);
        CC_SAFE_RELEASE_NULL(m_sPart);
    }
    CREATE_FUNC(YYDragRegion);
};

#endif
