//
//  YYTrap.h
//  Fairyland
//
//  Created by fuchenhao on 14-1-6. 
//
//

#ifndef __Fairyland__YYTrap__
#define __Fairyland__YYTrap__

#include "YYFightSprite.h"

class YYTrap : public YYFightSprite
{
    
private:
    
    int32_t m_nCurrentDisplacement;
    int32_t m_nTotalDisplacement;
    bool m_bTouchOff;
    
public:
   
    YYTrap();
    ~YYTrap();
    CREATE_FUNC(YYTrap);
    void move();
    void update(void);
    void initFightData(SpriteData * spriteData,SpriteAttribute * spriteAttribute);
    void touchOffDetection(cocos2d::CCArray *targetArray);
    
};

#endif /* defined(__Fairyland__YYTrap__) */
