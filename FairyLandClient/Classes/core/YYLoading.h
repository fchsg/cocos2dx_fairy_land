//
//  YYLoading.h
//  XJ
//
//  Created by fuchenhao on 13-5-23. 
//
//

#ifndef __XJ__YYLoading__
#define __XJ__YYLoading__

#include <cocos2d.h>

#define  MSG_LABEL_COUNT 8   //信息个数

#define CUTSCENE_STATE 10
#define TAG_LOADING_SE 20
#define CUTSCENE_ANIMATION 30

class YYSprite;

class Loading : public cocos2d::CCLayer
{

    CC_SYNTHESIZE(bool, m_bResumeAction, ResumeAction);
    
private:
    int m_nCount;
    int m_nRepeatScene;
    uint16_t m_u16FrameCounter;
    uint8_t m_u8SwitchFrame;
    cocos2d::CCLabelTTF * m_pLabel;
    cocos2d::CCLabelTTF * m_pLabelMsg;
    bool m_bProgressCompleted;
public:
    Loading();
    ~Loading();
    
    CREATE_FUNC(Loading);
    void update();
    virtual bool init();
    inline bool isProgressCompleted()
    {
        return m_bProgressCompleted;
    }
    inline void setProgressCompleted(bool isCompleted)
    {
        m_bProgressCompleted = isCompleted;
    }
};

#endif /* defined(__XJ__YYLoading__) */
