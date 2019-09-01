//
//  YYSprite.h
//  Datang
//
//  Created by fuchenhao on 13-2-5. 
//
//

#ifndef __Datang__YYSprite__
#define __Datang__YYSprite__

#include "cocos2d.h"
#include "YYActionSet.h"
#include "YYActionData.h"
#include "YYFrameData.h"
#include "../core/YYSpriteData.h"
#include "../core/YYGlobalData.h"

#define SPRITE_TYPE_PLAYER 1// 玩家 
#define SPRITE_TYPE_NPC 2// NPC
#define SPRITE_TYPE_MONSTER 3// 小怪 
#define SPRITE_TYPE_BOSS 4// BOSS
#define SPRITE_TYPE_ITEM 5// 道具 
#define SPRITE_TYPE_PET 6// 召唤兽 
#define SPRITE_TYPE_DESTRUCTIBLE_OBJECT 7// 可被破坏的物体 
#define SPRITE_TYPE_DOOR 9// 传送门 
#define SPRITE_TYPE_DUNGEONS 10// 进入副本选择 
#define SPRITE_TYPE_CIRCLE 11// 出怪之前显示的法阵 
#define SPRITE_TYPE_FACE 12 // 表情 
#define SPRITE_TYPE_DEBUFF 13 // debuff特效 
#define SPRITE_TYPE_DEBUFF_OVER 14 // debuff结束特效 
#define SPRITE_TYPE_DOT 15// 持续伤害 
#define SPRITE_TYPE_SKILL_EFFECT 16// 技能特效 
#define SPRITE_TYPE_CATAPULT_EFFECT 17// 弹射特效 
#define SPRITE_TYPE_LINK_EFFECT 18// 链击特效 
#define SPRITE_TYPE_ROLLING_BUCKET 19 // 滚桶 
#define SPRITE_TYPE_STAB 20 // 刺 
#define SPRITE_TYPE_GUIDE 21// 指引
#define SPRITE_TYPE_TRIAL 22// 试炼

#define ACTION_RUN 0
#define ACTION_STAND0 1
#define ACTION_STAND1 2
#define ACTION_HITTED 3
#define ACTION_DIE 4
#define ACTION_RUN1 5
#define ACTION_STAND2 6
#define ACTION_RUN2 7
#define ACTION_SING 8
#define ACTION_SHOT 9
#define ACTION_ATTACK_0 10
#define ACTION_ATTACK_1 11
#define ACTION_ATTACK_2 12
#define ACTION_HURTED 19

// 位移方式 
#define MOVE_BY_CONTROLLED 0// 方向键控制 
#define MOVE_BY_SET_DESTINATION 1// 设置目的地 
#define MOVE_BY_SKILL 2// 施放技能时的位置偏移 
#define MOVE_BY_CLICK_NPC 3// 点击NPC的偏移 
#define MOVE_BY_RANDOM 4// 随机位移 
#define MOVE_BY_SCRIPT 5// 随机位移 

#define DELETE_ACTION_OVER 1 // 动作结束后删除 
#define DELETE_COUNTDOWN_TO_THE_END 2// 倒计时结束后删除 

//————————动作状态———————————— 
enum
{
    kActionNormal = 0,
    kActionAttack,
    kActionSkill,
    kActionInjured,
    kActionDie
};

class YYSprite : public cocos2d::CCSprite
{
    CC_SYNTHESIZE(uint8_t, m_u8FrameDuration, FrameDuration);// 动作帧持续时间 
    CC_SYNTHESIZE(int, m_nActionFrameIndex, ActionFrameIndex);// 当前帧在当前动作中的序号 
    CC_SYNTHESIZE(uint16_t, m_u16CurrentFrameIndex,CurrentFrameIndex);// 在帧列表中的序号 
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pBatchNodeDictionary, BatchNodeDictionary);//
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pPngFileNameDictionary, PngFileNameDictionary);// 图片文件名称集合 
    CC_SYNTHESIZE_RETAIN(ActionSet *, m_pActionSet, ActionSet);// 精灵动作集
    CC_SYNTHESIZE(ActionData *, m_pCurrentActionData, CurrentActionData);// 当前动作数 
    CC_SYNTHESIZE(cocos2d::CCSize, m_size, Size);// 当前帧图形对应的矩形大小 
    CC_SYNTHESIZE(cocos2d::CCSize, m_DefaultSize, DefaultSize);// 默认图形对应的矩形大小 
    CC_SYNTHESIZE(uint16_t, m_u16SpriteType, SpriteType);// 精灵类型 
    CC_SYNTHESIZE(cocos2d::CCPoint, m_CurrentTile, CurrentTile);// 玩家当前所在元素格(行、列数) 
    CC_SYNTHESIZE(cocos2d::CCPoint, m_Destination, Destination);// 目的地 
    CC_SYNTHESIZE(uint8_t, m_u8ActionIndex, ActionIndex);// 动作索引 
    CC_SYNTHESIZE(cocos2d::CCLabelTTF *, m_pTalkLabel, TalkLabel);
    CC_SYNTHESIZE(int16_t, m_n16SpriteID, SpriteID);// 精灵ID(本地) 
    CC_SYNTHESIZE(SpriteData *, m_pSpriteData, SpriteData);// 精灵基础数据 
    CC_SYNTHESIZE_RETAIN(cocos2d::CCArray *, m_pVisibleConditionArray, VisibleConditionArray);// 显示条件 
    CC_SYNTHESIZE(int, m_nScriptActionDuration, ScriptActionDuration);// 动作播放持续时间 
    CC_SYNTHESIZE(int, m_nScriptActionCounter, ScriptActionCounter);// 动作播放持续时间计数器 
    CC_SYNTHESIZE(int, m_nScriptSpeed, ScriptSpeed);// 脚本控制精灵移动的速率 
    CC_SYNTHESIZE(uint8_t ,m_u8MoveType, MoveType);// 位移类型 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pYYSpriteChildren, YYSpriteChildren);// 存贮YYSprite *类型的子节点 
    CC_SYNTHESIZE(int8_t ,m_n8DeleteCondition, DeleteCondition);// YYSprite *类型子节点的删除条件  1到达设定的时间 2当前动作执行完毕 
    CC_SYNTHESIZE(int, m_nTileIndex, TileIndex);
    CC_SYNTHESIZE(cocos2d::CCLabelTTF *, m_pNameLabel, NameLabel);//精灵名称
    
protected:
    
    cocos2d::CCString *m_pSpriteID;// 精灵在服务器的ID 
    cocos2d::CCString *m_pSpriteName;// 精灵名称 
    bool m_bSetDefaultSize;// 是否设置过精灵默认大小 
    uint8_t m_u8ActionStatus;// 动作状态 
    int8_t m_n8FrameIntervalCount;// 帧间隔计数器 
    bool m_bFaceToRight;// 精灵朝向 
    bool m_bClipMirror;// 切片是否需要镜像 
    bool m_bSpriteMirror; // 精灵是否镜像 
    bool m_bRepeatAction,m_bActionOver;// 是否重复动作，动作是否结束 
    bool m_bDeleted;// 被移除 
    uint16_t m_u16SpriteIndex;// 精灵序号（类别） 
    bool m_bLocked;// 锁定的精灵只受脚本指令控制 
    bool m_bKeepCurrentFrame;// 脚本动作结束后是否停顿在当前帧 
    bool m_bPause;// 暂停动作 
    bool m_bMoving;// 是否正在位移 
    int m_nDuration;// 生命周期 
    int m_nCounter;// 生命周期计时 
    
public:
    
    YYSprite();
    virtual ~YYSprite();
    CREATE_FUNC(YYSprite);
    static YYSprite * createWithActionSet(const std::string& aniFile,const std::string& pngFileName,DirectoryType directoryType);
    void initWithActionSet(const std::string& aniFile,const std::string& pngFileName,DirectoryType directoryType);
    void initWithActionSet(cocos2d::CCArray * resourceNames,const DirectoryType directoryType,bool isSetLayerIndex = false/** 是否解析图片名设置层级序列 */);
    void initWithActionSet(const char * resourceFileName,DirectoryType directoryType);
    static YYSprite * copy(YYSprite * sprite);
    void setMoudle(int imgID, cocos2d::CCRect rect ,cocos2d::CCPoint position ,unsigned char transform);
    void setFrame(FrameData * frameData);//通过帧数据设置当前帧 
    void setFrame(uint16_t frameIndex);//通过当前帧ID设置当前帧 
    void setAction(uint16_t animationID ,bool isRepeat = false);//设置当前动作 
    void setActionByScript(uint16_t animationIndex ,int duration , bool isKeepLastFrame);// 设置动作持续时间 
    
    void setMoudle(int imgID,cocos2d::CCRect rect,cocos2d::CCPoint position,unsigned char transform,cocos2d::CCRect clipRect);
    void setFrame(FrameData * frameData,cocos2d::CCRect rect);//通过帧数据设置当前帧
    void setFrame(uint16_t frameIndex,cocos2d::CCRect rect);//通过当前帧ID设置当前帧 
    void setAction(uint16_t animationID ,bool isRepeat,cocos2d::CCRect rect);// 设置当前动作 

    virtual void cleanupClipRect(void);
    bool isActionOver();
    void setFaceToRight(bool isFaceToRight);
    bool isFaceToRight();
    virtual void updateAnimation(void);
    virtual void update(void);
    int8_t getActionStatus();
    void setActionStatus(uint8_t actionStatus);
    void setDuration(int duration);
    inline void setDeleted(bool isDeleted)
    {
        m_bDeleted = isDeleted;
    }
    
    inline bool isDeleted()
    {
        return m_bDeleted;
    }
    
    inline void setSpriteMirror(bool isMirror)
    {
        m_bSpriteMirror = isMirror;
    }
    
    inline bool isSpriteMirror()
    {
        return m_bSpriteMirror;
    }
    
    inline bool isRepeatAction()
    {
        return m_bRepeatAction;
    };
    
    inline void setRepeatAction(bool isRepeatAction)
    {
        m_bRepeatAction = isRepeatAction;
    };
    
    inline void setSpriteIDInServer( const char * spriteID)
    {
        CC_SAFE_DELETE(m_pSpriteID);
        m_pSpriteID = new cocos2d::CCString(spriteID);
    }

    /** 返回精灵在服务端的ID */
    inline cocos2d::CCString * getSpriteIDInServer()
    {
        return m_pSpriteID;
    }
    
    inline void setSpriteName(const char * spriteName)
    {
        CC_SAFE_DELETE(m_pSpriteName);
        m_pSpriteName = new cocos2d::CCString(spriteName);
    }
    
    /** 返回精灵名称 */
    inline cocos2d::CCString * getSpriteName()
    {
        return m_pSpriteName;
    }
    
    inline void setLocked(bool isLocked)
    {
        m_bLocked = isLocked;
    }
    
    inline bool isLocked()
    {
        return m_bLocked;
    }
    
    inline bool isMoving()
    {
        return m_bMoving;
    }
    inline void setMoving(bool isMove)
    {
        m_bMoving = isMove;
    }
    bool isIntersectsRect(cocos2d::CCRect rect);// 是否有矩形与精灵发生碰撞 
    bool isIntersectsSprite(YYSprite * sprite);// 是否有矩形与精灵发生碰撞 
    bool isTouched(cocos2d::CCPoint point);// 是否被点选中 
};
#endif /* defined(__Datang__YYSprite__) */
