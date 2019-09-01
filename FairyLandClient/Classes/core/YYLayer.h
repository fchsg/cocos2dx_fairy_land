//
//  YYLayer.h
//  XJ
//
//  Created by fuchenhao on 13-6-17. 
//
//

#ifndef XJ_YYLayer_h
#define XJ_YYLayer_h

#include "cocos2d.h"
#include "../animation/YYRect.h"


class YYLayer : public cocos2d::CCLayer
{
    
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pClipArray, ClipArray)
    CC_SYNTHESIZE(cocos2d::CCSize, m_ClipOffset, ClipOffset);
    
private:
    
    bool m_bClip;// 是否剪裁 
    std::vector<cocos2d::CCPoint> m_vLineVertex;
    
public:
    
    CREATE_FUNC(YYLayer);
    YYLayer();
    ~YYLayer();
    
    void setClip(bool isClip);
    bool isClip();
    void draw();
    void visit();
    void addLine(cocos2d::CCPoint point1,cocos2d::CCPoint point2);
    void clearVertex();
};

#endif
