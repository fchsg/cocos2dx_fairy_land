//
//  YYRect.cpp
//  Datang
//
//  Created by fuchenhao on 13-2-4. 
//
//

#include "YYRect.h"

USING_NS_CC;

YYRect::YYRect()
:m_nX(0)
,m_nY(0)
,m_nWidth(0)
,m_nHeight(0)
,m_u8ImageIndex(0)
{
}

YYRect::~YYRect()
{
    
}
YYRect * YYRect::createRect(cocos2d::CCRect rect)
{
    YYRect * newRect = new YYRect();
    newRect->autorelease();
    newRect->m_nX = rect.origin.x;
    newRect->m_nY = rect.origin.y;
    newRect->m_nWidth = rect.size.width;
    newRect->m_nHeight = rect.size.height;
    return newRect;
}
YYRect * YYRect::createRect(YYRect *rect)
{
    YYRect * newRect = new YYRect();
    newRect->autorelease();
    newRect->m_nX = rect->m_nX;
    newRect->m_nY = rect->m_nY;
    newRect->m_nWidth = rect->m_nWidth;
    newRect->m_nHeight = rect->m_nHeight;
    newRect->m_u8ImageIndex = rect->m_u8ImageIndex;
    return newRect;
}
YYRect * YYRect::createRect(int x,int y,int w,int h,uint8_t imageIndex)
{
    YYRect * rect = new YYRect();
    rect->autorelease();
    rect->m_nX = x;
    rect->m_nY = y;
    rect->m_nWidth = w;
    rect->m_nHeight = h;
    rect->m_u8ImageIndex = imageIndex;
    return rect;
}

#pragma mark 判断两个矩形是否相交(YYRect *与 CCRect) 
bool YYRect::isIntersectWitchCCRect(cocos2d::CCRect rect,cocos2d::CCPoint position1,cocos2d::CCPoint position2)
{
    if (position1.x + getX() + getWidth() < position2.x + rect.getMinX() || position1.x + getX() > position2.x + rect.getMaxX() || position1.y + getY() + getHeight() < position2.y + rect.getMinY() || position1.y + getY() > position2.y + rect.getMaxY()) {
        return false;
    }
    return true;
}

#pragma mark 判断两个矩形是否相交(YYRect *与 YYRect *) 
bool YYRect::isIntersectWitchYYRect(YYRect *rect,cocos2d::CCPoint position1,cocos2d::CCPoint position2)
{
    if (position1.x + getX() + getWidth() < position2.x + rect->getX() || position1.x + getX() > position2.x + rect->getX() + rect->getWidth() || position1.y + getY() + getHeight() < position2.y + rect->getY() || position1.y + getY() > position2.y + rect->getY() + rect->getHeight()) {
        return false;
    }
    return true;
}

