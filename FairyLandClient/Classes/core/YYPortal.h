//
//  YYPortal.h
//  Fairyland
//
//  Created by fuchenhao on 13-12-20. 
//
//

#ifndef __Fairyland__YYPortal__
#define __Fairyland__YYPortal__

#include "../animation/YYSprite.h"

class YYPortal : public YYSprite
{
    
public:
    YYPortal();
    ~YYPortal();
    CREATE_FUNC(YYPortal);
    virtual void update(void);
};

#endif /* defined(__Fairyland__YYPortal__) */
