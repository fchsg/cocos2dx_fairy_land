//
//  YYLayer.cpp
//  XJ
//
//  Created by fuchenhao on 13-6-26. 
//
//

#include "YYLayer.h"
YYLayer::YYLayer()
:m_bClip(false)
,m_pClipArray(NULL)
,m_ClipOffset(0,0)
{
    m_pClipArray = new cocos2d::CCArray(3);
}

YYLayer::~YYLayer()
{
    CC_SAFE_DELETE(m_pClipArray);
}
void YYLayer::setClip(bool isClip)
{
    m_bClip = isClip;
}

bool YYLayer::isClip()
{
    return m_bClip;
}

void YYLayer::draw()
{
    if (m_vLineVertex.size() < 1)
    {
        return;
    }
    cocos2d::ccDrawColor4B(255,255,255,255);
    for(int i = 0; i < m_vLineVertex.size();i += 2)
    {
        cocos2d::CCPoint point1 = m_vLineVertex.at(i);
        cocos2d::CCPoint point2 = m_vLineVertex.at(i+1);
        cocos2d::ccDrawLine(point1,point2);
    }
}

void YYLayer::visit()
{
    if (m_bClip)
    {
        // 剪裁区域按屏幕实际像素点计算 
        for (int i = 0; i < m_pClipArray->count();i++){
            YYRect * rect = (YYRect *)m_pClipArray->objectAtIndex(i);
            glEnable(GL_SCISSOR_TEST);
            cocos2d::CCEGLView::sharedOpenGLView()->setScissorInPoints(rect->getX() + getPositionX() + m_ClipOffset.width, rect->getY() + getPositionY()+ m_ClipOffset.height, rect->getWidth() -  m_ClipOffset.width * 2, rect->getHeight() - m_ClipOffset.height * 2);
            CCLayer::visit();
            glDisable(GL_SCISSOR_TEST);
        }
    }
    else
    {
        CCLayer::visit();
    }
}

void YYLayer::addLine(cocos2d::CCPoint point1,cocos2d::CCPoint point2)
{
    m_vLineVertex.push_back(point1);
    m_vLineVertex.push_back(point2);
}

void YYLayer::clearVertex()
{
    m_vLineVertex.clear();
}
