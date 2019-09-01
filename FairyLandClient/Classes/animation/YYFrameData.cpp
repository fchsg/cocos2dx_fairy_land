//
//  YYFrameData.cpp
//  Datang
//
//  Created by fuchenhao on 13-2-4. 
//
//

#include "YYFrameData.h"

USING_NS_CC;

FrameData::FrameData()
:m_u16RectAmount(0)
,m_pRectIndexArray(NULL)
,m_pTransformTypeArray(NULL)
,m_pRectXArray(NULL)
,m_pRectYArray(NULL)
{
}

FrameData::~FrameData()
{
    CC_SAFE_DELETE(m_pRectIndexArray);
    CC_SAFE_DELETE(m_pTransformTypeArray);
    CC_SAFE_DELETE(m_pRectXArray);
    CC_SAFE_DELETE(m_pRectYArray);
}

FrameData *FrameData::createFrameData(unsigned short rectAmount)
{
    FrameData* frameData = new FrameData();
    frameData->autorelease();
    frameData->m_u16RectAmount = rectAmount;
    frameData->m_pRectIndexArray = new CCArray(rectAmount);
    frameData->m_pTransformTypeArray = new CCArray(rectAmount);
    frameData->m_pRectXArray = new CCArray(rectAmount);
    frameData->m_pRectYArray = new CCArray(rectAmount);
    return frameData;
}
