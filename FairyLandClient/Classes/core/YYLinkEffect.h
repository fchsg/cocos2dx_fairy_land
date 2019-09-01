//
//  YYLinkEffect.h
//   XJ
//  闪电链特效 
//  Created by fuchenhao on 13-6-9. 
//
//

#ifndef __XJ__YYLinkEffect__
#define __XJ__YYLinkEffect__

#include "YYFightSprite.h"
class YYLinkEffect : public YYFightSprite
{
    CC_SYNTHESIZE(int, m_nLinkAmount, LinkAmount);// 链接总数 
    CC_SYNTHESIZE(int, m_nLinkCount, LinkCount);// 链接计数 
    CC_SYNTHESIZE(int, m_nLinkInterval, LinkCountInterval);// 链接间隔 
    CC_SYNTHESIZE(int, m_nLinkIntervalCount, LinkIntervalCount);// 链接间隔计数 
    CC_SYNTHESIZE(int,m_nMaxLinkDistance,MaxLinkDistance);
    
private:
    
    cocos2d::CCArray * m_pTargetArray;// 存储可被链接的目标对象 
    cocos2d::CCArray * m_pLinkedArray;// 存储已被链接的对象 
    YYFightSprite * m_pCurrentTarget;
    bool m_bLinkPause;
    
public:
    
    YYLinkEffect();
    ~YYLinkEffect();
    CREATE_FUNC(YYLinkEffect);
    void update();
    void linkNextTarget(YYFightSprite * fightSprite);
    void updateTarget(YYFightSprite * fightSprite);
    void setLinkEffect(cocos2d::CCPoint initPosition,int linkDistance , float redian);
    
};

#endif /* defined(__XJ__YYLinkEffect__) */
