//
//  YYLogo.h
//  XJ
//
//  Created by fuchenhao on 13-5-9. 
//
//

#ifndef __XJ__YYLogo__
#define __XJ__YYLogo__

#include <cocos2d.h>

class YYSprite;

class Logo : public cocos2d::CCLayer{
    
private:
    
    YYSprite * m_pLogoSprite;
    
public:
    
    Logo();
    ~Logo();
    
    CREATE_FUNC(Logo);
    virtual bool init();
    void update();
    
};
#endif /* defined(__XJ__YYLogo__) */
