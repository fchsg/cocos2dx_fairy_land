//
//  YYSpriteLayer.h
//  XJ
//  战斗精灵层 
//  Created by fuchenhao on 13-4-27. 
//
//

#ifndef __XJ__YYSpriteLayer__
#define __XJ__YYSpriteLayer__

#include "cocos2d.h"

#define CLIENT_ENEMY_AI 1

#define BELOW_SPRITE_LAYER -1 // 精灵层之下（放置被精灵遮挡的特效） 
#define ABOVE_SPRITE_LAYER 1 // 精灵层之上（放置遮挡精灵的特效） 

class YYPlayer;
class YYTrialPlayer;
class YYSprite;
class YYFightSprite;
class YYSkillData;
class TaskData;
class TaskTarget;

class YYSpriteLayer : public cocos2d::CCLayer
{
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pSpriteDictionary, SpriteDictionary);// 存储所有玩家 
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pSpriteWithServerIDDict, SpriteWithServerIDDict);// 存储玩家及非战斗精灵（以精灵存储在服务端的ID映射）
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pFriendlyRoleArray, FriendlyRoleArray);// 存储友方角色 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pEnemyArray, EnemyArray);// 存储敌人
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pStabArray, StabArray);// 存储地刺的容器（对敌我双方都可造成伤害）
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pMoveableEnemyArray, MoveableEnemyArray);// 可行动的敌人 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pNPCArray, NPCArray);// 存储NPC 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pMedicamentArray, MedicamentArray);// 存储NPC 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pDoorArray, DoorArray);// 存储传送门 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pDamageEffectArray, DamageEffectArray);// 处理伤害效果显示 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pDodgeEffectArray, DodgeEffectArray);// 处理闪避效果显示
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pHitEffectArray, HitEffectArray);// 打击特效 
    CC_SYNTHESIZE(cocos2d::CCArray * ,m_pGestureSkillShadeArray,GestureSkillShadeArray);// 手势技能形状数组 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pAttackingSpriteArray, AttackingSpriteArray);// 正在攻击的精灵数组 
    CC_SYNTHESIZE_READONLY(YYPlayer *, m_pRoleByControlled, RoleByControlled);// 玩家可控角色
    CC_SYNTHESIZE_READONLY(YYTrialPlayer *, m_pRoleByTrial, RoleByTrial);          // 试炼角色
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pRecoverArray, RecoverArray);
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pReplacePartPicArray, ReplacePartPicArray);// 更换的装备图片 
    CC_SYNTHESIZE(uint8_t, m_u8DropOdds, DropOdds);// 药瓶掉率 
    CC_SYNTHESIZE(int ,m_nSpriteIDCounter, SpriteIDCounter);// 战斗精灵本地ID 
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pTrapArray, TrapArray);// 机关陷阱容器
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pEffectArray, EffectArray);
    CC_SYNTHESIZE(uint32_t, m_nWorldBossDamageHP, WorldBossDamageHP); // 活动 世界Boss 打掉boss的血量

private:
	void bezierMove(int nSpriteType);			//金币做贝赛尔运动
private:
    int m_nPreGold;				// 更新前的金币数
	bool m_bFirstSetGold;		//是否是第一次设置金币数
    int m_nPreJewel;			// 更新前的宝石数
	bool m_bFirstSetJewel;		//是否是第一次设置宝石数
    bool m_bReplaceEquip;
    
public:
    
    YYSpriteLayer();
    ~YYSpriteLayer();
    
    CREATE_FUNC(YYSpriteLayer);
    void update(void);
    void sortSprite();
    
    void addElementToSpriteDictionary(YYSprite * sprite);
    void loadSprites(cocos2d::CCArray * array);// 加载战斗精灵
    void removeSprites(const char *spriteID);// 删除战斗精灵
    void processSpriteMove(cocos2d::CCArray *array);// 处理精灵位移 
    void preLoadSkillEffectResource(cocos2d::CCArray * array);// 预加载特效资源
    void preLoadTrialSkillEffectResource(cocos2d::CCArray * array);// 预加载试炼特效资源
    cocos2d::CCPoint getSpritePosition(int tileIndex,bool isRandom = true);// 通过地格信息获取精灵位置 
    void summon(int spriteIndex,cocos2d::CCPoint position,int summonedDuration,int ownerID);
    void setRoleControlled(cocos2d::CCArray * array);// 设置可控角色 
    void setPlayerAttribute(const char * spriteID,cocos2d::CCArray * array);// 设置玩家的属性
    void setTrialPlayerAttribute(const char * spriteID,cocos2d::CCArray * array);// 设置试炼玩家的属性
    void levelUpEffect(YYSprite * sprite);// 升级特效 
    void setAttributeOnUI(cocos2d::CCArray * array);// 设置在UI上显示的属性 
    void setDropOdds(cocos2d::CCArray * array);// 设置掉率 
    void dropItem(YYFightSprite * fightSprite,bool isDrop100Percent = false);// 道具掉落 
    void removeDefunctTarget(cocos2d::CCArray *array, YYFightSprite * fightSprite);// 删除已死亡的目标 
    void processEnemyDeath(YYFightSprite * fightSprite);
    void setSkillIndexData(cocos2d::CCArray * array);

    void processTask(YYFightSprite * fightSprite); // 判断是否为任务所需怪
    void processClearStageTask(); // 判断是否为任务通关
    bool isTaskStage(TaskTarget * taskTarget,int stageIndex); // 是否为任务所指定的副本 关卡
    bool isTaskTarget(TaskTarget * taskTarget); // 是否为任务所指定的副本 关卡 难度
    void openPortal(int fightSpriteIndex);
    void setAroundPlayerVisible(bool _bVisible); //是否显示周围玩家信息
    inline bool isReplaceEquip()
    {
        return m_bReplaceEquip;
    }
    inline void setReplaceEquip(bool isReplace)
    {
        m_bReplaceEquip = isReplace;
    }
};

#endif /* defined(__XJ__YYSpriteLayer__) */
