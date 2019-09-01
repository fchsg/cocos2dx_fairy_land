//
//  YYFrameData.h
//  Datang
//  帧数据 
//  Created by fuchenhao on 13-2-4. 
//
//

#ifndef __Datang__YYFrameData__
#define __Datang__YYFrameData__

#include "cocos2d.h"

class FrameData : public cocos2d::CCObject
{
    
    CC_SYNTHESIZE(uint16_t,m_u16RectAmount,RectAmount);// 矩形图块数量 
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pRectIndexArray,RectIndexArray);// 矩形图块索引 
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pTransformTypeArray,TransformTypeArray);// 矩形图块翻转类型 
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pRectXArray,RectXArray);// 矩形图块左上角X坐标 
    CC_SYNTHESIZE(cocos2d::CCArray *,m_pRectYArray,RectYArray);// 矩形图块左上角Y坐标 

public:
    
    FrameData();
    ~FrameData();
    static FrameData * createFrameData(unsigned short rectAmount);
    
};

#endif /* defined(__Datang__YYFrameData__) */
