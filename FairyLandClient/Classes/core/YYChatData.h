//
//  YYChatClass.h
//  Fairyland
//
//  Created by 章敏 on 1/10/14. 
//
//

#ifndef __Fairyland__YYChatClass__
#define __Fairyland__YYChatClass__

#include <iostream>

#include "cocos2d.h"
#include "cocos-ext.h"

struct FontInfo
{
    std::string fontName;
    std::string name;
    cocos2d::ccColor3B color;
    cocos2d::CCPoint point;
    int type;    // 1label 2sprite
    int fontSize;
};

class YYChatData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(int, m_nLineAmount, LineAmount); // 每段话总行数 
    CC_SYNTHESIZE(float, m_fLineHeight, LineHeight); // 每行行高 
    
public:
    
    YYChatData()
    :m_nLineAmount(0)
    ,m_fLineHeight(0)
    {
    }
    
    ~YYChatData()
    {
    }
    bool init()
    {
        return true;
    }
    CREATE_FUNC(YYChatData);
    std::vector<FontInfo>  ChatArray;
};

#endif /* defined(__Fairyland__YYChatClass__) */
