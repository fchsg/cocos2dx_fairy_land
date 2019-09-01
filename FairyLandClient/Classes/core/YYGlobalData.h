//
//  YYGlobalData.h
//  Fairyland
//
//  Created by fuchenhao on 14-2-13. 
//
//

#ifndef __Fairyland__YYGlobalData__
#define __Fairyland__YYGlobalData__

#include <cocos2d.h>
#include "platform/CCFileUtils.h"
#include "../common/Common.h"
#include "YYProperty.h"
#include "YYEquipment.h"
#include "YYTalentData.h"
#include "YYProtocol.h"
#include "YYSkillData.h"
#include "YYSkillActionData.h"
#include "YYSkillEffectData.h"
#include "YYSpriteData.h"
#include "YYSpriteAttribute.h"
#include "YYTaskData.h"
#include "YYTaskTargetData.h"
#include "YYProtossData.h"
#include "YYAchievementData.h"
#include "YYLoginAwardData.h"
#include "YYActivityData.h"
#include "YYVipData.h"

class ActionSet;

class GlobalData : public cocos2d::CCObject
{
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pActionSetDict, ActionSetDict);// 缓存动作文件数据
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pSkillDataDict, SkillDataDict);// 技能表格数据 key 技能id value 等级数组
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pTalentDataDict, TalentDataDict);// 天赋表格数据 key 天赋id value 等级数组
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pSkillActionDataDict, SkillActionDataDict);// 技能动作数据
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pSkillEffectDataDict, SkillEffectDataDict);// 技能特效
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pTalentDataArrayDict, TalentDataArrayDict);// 存储每个技能分别对应的一组天赋数据 key 技能索引 value 天赋对象
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pUsableSkillDataArray, UsableSkillDataArray);// 玩家基础技能数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pUsableGestureSkillDataArray, UsableGestureSkillDataArray);// 玩家怒气技数据
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pPlayerShortcutSkillIndexDict, PlayerShortcutSkillIndexDict);// value:玩家快捷技能对应的技能编号数组，key:玩家ID
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pPlayerGestureSkillIndexDict, PlayerGestureSkillIndexDict);// value:玩家手势技能对应的技能编号数组，key:玩家ID
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pPlayerSkillDataArray, PlayerSkillDataArray);// 玩家技能数据 value:技能编号 key:playerID
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pSpriteDataDict, SpriteDataDict);// 玩家对应的编号、资源等数据
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pSpriteAttributeDict, SpriteAttributeDict);// 存储精灵属性
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pPlayerAttributeDict, PlayerAttributeDict);// value:主角队玩家属性，key:玩家ID
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pPlayerAttackIndexDict, PlayerAttackIndexDict);// value:玩家普通攻击对应的技能编号数组，key:玩家ID
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pEquipmentDict, EquipmentDict);
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pEquipOnTheBodyArray, EquipOnTheBodyArray);//角色身上装备
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pEquipArray, EquipArray);//背包排序后的装备
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pPropArray, PropArray);// 背包排序后的物品
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pPropertyDict, PropertyDict);
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pTaskDataDict, TaskDataDict);// 任务基础数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pTaskDataArray, TaskDataArray);// 排序后的任务数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pTreasureItemArray, TreasureItemArray);
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pNickNameArray,NickNameArray);//读取敏感词数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pSetLoadingMessageArray, SetLoadingMessageArray);//存储设定进入loading状态的信息；
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pProtossDataArray, ProtossDataArray);// 星灵数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pConstellationDataArray, ConstellationDataArray);// 星座数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pScrapDataArray, ScrapDataArray);// 星灵对应的碎片数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pAchievementClassDataArray, AchievementClassDataArray);  // 成就类型
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pAchievementGroupDataArray, AchievementGroupDataArray);  // 成就分组
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pAchievementItemDataArray, AchievementItemDataArray);    // 成就单项
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pAchievementAllDataDict, AchievementAllDataDict);         // 成就所有数据
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pSignUpDataArray, SignUpDataArray);  // 签到
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pSignUpStaticAwardArray, SignUpStaticAwardArray);  // 签到
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pSigUpBoxAwardDataArray, SigUpBoxAwardDataArray);  // 宝箱
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pLoginAwardDict, LoginAwardDict);  // 七天登陆奖励
    CC_SYNTHESIZE(cocos2d::CCArray *, m_pActivityTaskArray, ActivityTask);           // 活跃度任务
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pActivityWishTreeDict, ActivityWishTreeDict);   // 活跃度奖励
    CC_SYNTHESIZE(cocos2d::CCDictionary *, m_pVipDataDict, VipDataDict);                     // vip 数据
    
private:
    
    cocos2d::CCArray * m_pGestureSkillDataArray[3];// 对应三种不同的手势技能
    
public:
    
    GlobalData();
    ~GlobalData();
    
    void init();
    
    static GlobalData *getInstance();
    
    void readData();
    
    ActionSet * getActionSet(const std::string& aniFileName);
    
    /**
     * 读取技能数据
     */
    void readSkillData();
    /**
     * 读取技能对应的动作数据
     */
    void readSkillActionData();
    /**
     * 读取技能特效数据
     */
    void readSkillEffectData();
    /**
     * 读取玩家数据
     */
    void readPlayerData();
    /**
     * 读取可战斗精灵的数据
     */
    void readFightSpriteData();
    /**
     * 读取NFU(非战斗单位)数据 Non-Fight-Unit
     */
    void readNFSData();
    /**
     * 读取精灵属性
     */
    void readNickNameData();
    /**
     * 读取敏感词数据
     */
    void readSpriteAttribute();
    /**
     * 读取星灵数据
     */
    void readProtossData();
    /**
     * 读取星座数据
     */
    void readConstellationData();
    /**
     * 读取碎片数据
     */
    void readScrapData();
    /**
     * 读取活跃度数据
     */
    void readActivityData();
    /**
     * 读取天赋数据
     */
    void readTalentData();
    /**
     * 读取vip数据
     */
    void readVipData();
    //----------星灵相关----------
    YYProtossData *getProtossData(int nID);
    //------------成就-----------
    void readAchievementClassData(); //读取成就分类信息
    void readAchievementGroupData(); //读取成就分组信息
    void readAchievementItemData();  //读取成就单项信息
    void setAchievementAllData();    //存储成就全部信息
    AwardData* setAchievementAwardData(int nIndex, int nNum, std::string sIcon);
    AchievementClassData *getAchievementClassData(int nClassIdx);
    AchievementGroupData *getAchievementGroupData(int nClassIdx, int nGroupIdx);
    AchievementItemData  *getAchievementItemData(int nClassIdx, int nGroupIdx, int nItemIdx);
    //------------签到----------
    void readSignUpData();
    void readSignUpBoxAwardData();
    void readLoginAwardData();
    YYSignUpData *getSignUpData(int nDay);
    YYSignUpBoxAwardData *getSignUpBoxAwardData(int nIndex);
    //----------技能相关-------------
    SkillData *getPlayerSkillData(const char * playerID, int skillIndex);
    cocos2d::CCArray *getGestureSkillDataArrayByType(int gestureType);
    
    SkillData *getCommonSkillData(int skillIndex, int skillLevel = 1); //通过技能id 获取技能 默认等级为一级

    void receiveTalentData(cocos2d::CCArray *array);// 天赋
    void replaceEquip(cocos2d::CCArray *array);//用新装备替换老装备
    void addProperty(cocos2d::CCArray * array, bool isInTreasure = false);// 物品
    void addEquipment(cocos2d::CCArray * array, bool isInTreasure = false);// 装备
    void itemConsume(cocos2d::CCArray *array,uint8_t itemType);// 物品消耗，类型0 道具 1 背包内装备 2 身上装备
    void sortEquip();// 排序装备
    void sortProp();// 排序物品
    void addTaskData(cocos2d::CCArray *array);// 添加任务
    void sortTaskData();// 任务排序
    
    void receiveSkillSQ(cocos2d::CCArray *arr, const char * userID);
    
    void clean();
    
private:

    // 清空技能相关缓存数据
    void removeSkillCacheData();
    // 创建技能天赋缓存数据
    void createSkillTalentArrData();
};

#endif /* defined(__Fairyland__YYGlobalData__) */
