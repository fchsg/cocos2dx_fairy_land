//
//  YYRect.h
//  Datang
//
//  Created by fuchenhao on 13-2-4. 
//
//

#ifndef __Datang__YYRect__
#define __Datang__YYRect__

#include "cocos2d.h"

class YYRect : public cocos2d::CCObject
{
    CC_SYNTHESIZE(float, m_nX, X);
    CC_SYNTHESIZE(float, m_nY, Y);
    CC_SYNTHESIZE(float, m_nWidth, Width);
    CC_SYNTHESIZE(float, m_nHeight, Height);
    CC_SYNTHESIZE(uint8_t, m_u8ImageIndex, ImageIndex);
    
public:
    
    YYRect();
    ~YYRect();
    static YYRect * createRect(cocos2d::CCRect rect);
    static YYRect * createRect(YYRect * rect);
    static YYRect * createRect(int x,int y, int w,int h,uint8_t imageIndex);
    bool isIntersectWitchCCRect(cocos2d::CCRect rect,cocos2d::CCPoint position1,cocos2d::CCPoint position2);
    bool isIntersectWitchYYRect(YYRect * rect,cocos2d::CCPoint position1,cocos2d::CCPoint position2);
};
#endif /* defined(__Datang__YYRect__) */
