//
//  YYAnimation.h
//  Datang
//
//  Created by fuchenhao on 13-2-4. 
//
//

#ifndef __Datang__YYAnimation__
#define __Datang__YYAnimation__

#include <cocos2d.h>
#include "YYActionData.h"
#include "YYFrameData.h"
#include "YYRect.h"

class ActionSet : public cocos2d::CCObject
{
    /** 图片矩形块数 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pImageRectCountArray, ImageRectCountArray);
    /** 矩形块数据 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pImageRectDataArray, ImageRectDataArray);
    /** 动作数据 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *,m_pActionDataArray, ActionDataArray);
    /** 帧数据 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *,m_pFrameDataArray, FrameDataArray);
    /** 帧ID数据 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *,m_pFrameIndexArray, FrameIDArray);
    /** 延迟的帧数 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pDelayArray, DelayArray);
    /** 每帧在x轴方向的偏移量 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pOffsetXArray, OffsetXArray);
    /** 每帧在y轴方向的偏移量 */
    CC_SYNTHESIZE_READONLY(cocos2d::CCArray *, m_pOffsetYArray, OffsetYArray);
    /** 图片数量 */
    CC_SYNTHESIZE_READONLY(uint8_t ,m_u8ImageAmount,ImageAmount);
    /** 总切片数 */
    CC_SYNTHESIZE_READONLY(uint16_t, m_u16ClipAmount, ClipAmount);
    /** 总帧数 */
    CC_SYNTHESIZE_READONLY(uint16_t, m_u16FrameAmount, FrameAmount);
    /** 动作数 */
    CC_SYNTHESIZE_READONLY(uint16_t, m_u16ActionAmount, ActionAmount);
public:
    ActionSet();
    ~ActionSet();
    bool init();
    CREATE_FUNC(ActionSet);
    static ActionSet* createActionSetWithFile(const std::string& aniFileName);
};
#endif /* defined(__Datang__YYAnimation__) */
