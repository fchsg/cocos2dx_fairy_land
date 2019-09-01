//
//  YYSkillActionData.h
//  XJ
//
//  Created by fuchenhao on 13-7-25. 
//
//

#ifndef XJ_YYSkillActionData_h
#define XJ_YYSkillActionData_h

#include "cocos2d.h"

class SkillActionData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(uint16_t ,m_u16Index, Index);// 序号 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pSpriteIndexArray, SpriteIndexArray);// 可调用该技能动作的精灵编号 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pActionIndexArray, ActionIndexArray);// 对应不同职业动作文件中的动作ID
    CC_SYNTHESIZE(uint16_t ,m_u16Duration, Duration);// 动作持续n秒动作循环播放 为0时只播放一遍 
    CC_SYNTHESIZE(std::string ,m_sSoundEffectPath, SoundEffectPath);// 技能对应音效资源名 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pEffectDataArray,EffectDataArray);// 对应的特效数据 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pEffectStartFrameArray,EffectStartFrameArray);// 特效启用时对应人物动作的第几帧 
    CC_SYNTHESIZE(int16_t, m_n16OffsetXPerFrame, OffsetXPerFrame);// 每帧横向偏移 
    CC_SYNTHESIZE(uint8_t, m_u8SoundFrameIndex, SoundFrameIndex);// 从第几帧开始播放音效 
    CC_SYNTHESIZE(int8_t, m_n8VibrateFrameIndex, VibrateFrameIndex);// 从第几帧开始震屏 
    CC_SYNTHESIZE(uint8_t, m_u8VibrateDuration, VibrateDuration);// 震屏持续时间 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pOffsetFrameIndexArray, OffsetFrameIndexArray);// 记录有位置偏移的帧的序号 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pOffsetXCurrentFrameArray, OffsetXCurrentFrameArray);// 记录有位置偏移的帧的偏移量 

private:
    
    bool m_bSoundEffectLoop;
    bool m_bMovable;// 调用该动作时是否可移动
    
public:
    
    SkillActionData()
    :m_pSpriteIndexArray(NULL)
    ,m_pActionIndexArray(NULL)
    ,m_pEffectDataArray(NULL)
    ,m_pEffectStartFrameArray(NULL)
    ,m_u16Index(0)
    ,m_bMovable(false)
    ,m_u16Duration(0)
    ,m_n16OffsetXPerFrame(0)
    ,m_u8SoundFrameIndex(0)
    ,m_n8VibrateFrameIndex(0)
    ,m_u8VibrateDuration(0)
    ,m_bSoundEffectLoop(false)
    {
        m_pOffsetFrameIndexArray = new cocos2d::CCArray();
        m_pOffsetXCurrentFrameArray = new cocos2d::CCArray();
    }
    ~SkillActionData()
    {
        CC_SAFE_RELEASE(m_pEffectDataArray);
    }
    CREATE_FUNC(SkillActionData);
    inline bool init()
    {
        return true;
    }
    inline bool isSoundEffectLoop()
    {
        return m_bSoundEffectLoop;
    }
    inline void setSoundEffectLoop(bool isLoop)
    {
        m_bSoundEffectLoop = isLoop;
    }
    inline bool isMovable()
    {
        return m_bMovable;
    }
    inline void setMovable(bool bMovable)
    {
        m_bMovable = bMovable;
    }
};

#endif
