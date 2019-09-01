//
//  YYSpriteLayer.cpp
//  XJ
//
//  Created by fuchenhao on 13-4-27. 
//
//

#include "YYSpriteLayer.h"

#include "../common/Common.h"

#include "YYController.h"
#include "YYControlUILayer.h"
#include "YYPlayer.h"
#include "YYTrialPlayer.h"
#include "YYMonster.h"
#include "YYScene.h"
#include "YYLinkEffect.h"
#include "YYCatapultEffect.h"
#include "YYSkillEffect.h"
#include "YYSpriteData.h"
#include "YYSpriteAttribute.h"
#include "YYPopUpUI.h"
#include "YYNPC.h"
#include "YYTaskTargetData.h"
#include "YYPortal.h"
#include "YYTrap.h"
#include "YYGlobalData.h"
#include "YYTaskData.h"
#include "YYConfig.h"

USING_NS_CC;

#define SKILL_SPEED 20
#define DAMAGE_DURATION 10// 伤害数字持续帧数 
#define DAMAGE_OFFSET_Y_PER_FRAME 8// 伤害数字每帧向上偏移量
#define TAG_LEVEL_UP 15 // 升级效果标签
#define TAG_TASK_LABEL  50// 任务怪物信息标签 
// 传送门开启条件 
#define TAG_CLEAN_ALL_ENEMY 1 // 清除所有敌人 
#define TAG_KILL_APPOINTED_ENEMY 2 // 杀死指定的敌人 
#define TAG_OPEN_ACCEPT_TASK 3 // 接受任务后开启 

YYSpriteLayer::YYSpriteLayer()
:m_pRoleByControlled(NULL)
,m_pRoleByTrial(NULL)
,m_pSpriteDictionary(NULL)
,m_pSpriteWithServerIDDict(NULL)
,m_pFriendlyRoleArray(NULL)
,m_pEnemyArray(NULL)
,m_pMoveableEnemyArray(NULL)
,m_pNPCArray(NULL)
,m_pDoorArray(NULL)
,m_pDamageEffectArray(NULL)
,m_pDodgeEffectArray(NULL)
,m_pHitEffectArray(NULL)
,m_pGestureSkillShadeArray(NULL)
,m_pAttackingSpriteArray(NULL)
,m_pRecoverArray(NULL)
,m_pReplacePartPicArray(NULL)
,m_u8DropOdds(0)
,m_nSpriteIDCounter(0)
,m_bReplaceEquip(false)
,m_nPreGold(0)
,m_bFirstSetGold(true)
,m_nPreJewel(0)
,m_bFirstSetJewel(true)
,m_nWorldBossDamageHP(0)
{
    m_pSpriteWithServerIDDict = new CCDictionary();
    m_pSpriteDictionary = new CCDictionary();
    m_pFriendlyRoleArray = new CCArray(15);
    m_pEnemyArray = new CCArray(25);
    m_pMoveableEnemyArray = new CCArray(20);
    m_pNPCArray = new CCArray(10);
    m_pMedicamentArray = new CCArray(10);
    m_pDamageEffectArray = new CCArray(15);
    m_pDodgeEffectArray = new CCArray(15);
    m_pHitEffectArray = new CCArray(10);
    m_pGestureSkillShadeArray = new CCArray(5);
    m_pDoorArray = new CCArray(2);
    m_pRecoverArray = new CCArray(5);
    m_pReplacePartPicArray = new CCArray(5);
    m_pTrapArray = new CCArray(15);
    m_pStabArray = new CCArray(5);
    m_pEffectArray = new CCArray(5);
}

YYSpriteLayer::~YYSpriteLayer()
{
    CC_SAFE_DELETE(m_pSpriteWithServerIDDict);
    CC_SAFE_DELETE(m_pSpriteDictionary);
    CC_SAFE_DELETE(m_pFriendlyRoleArray);
    CC_SAFE_DELETE(m_pEnemyArray);
    CC_SAFE_DELETE(m_pMoveableEnemyArray);
    CC_SAFE_DELETE(m_pNPCArray);
    CC_SAFE_DELETE(m_pMedicamentArray);
    CC_SAFE_DELETE(m_pDodgeEffectArray);
    CC_SAFE_DELETE(m_pDamageEffectArray);
    CC_SAFE_DELETE(m_pHitEffectArray);
    CC_SAFE_DELETE(m_pGestureSkillShadeArray);
    CC_SAFE_DELETE(m_pDoorArray);
    CC_SAFE_DELETE(m_pRecoverArray);
    CC_SAFE_DELETE(m_pReplacePartPicArray);
    CC_SAFE_DELETE(m_pTrapArray);
    CC_SAFE_DELETE(m_pStabArray);
    CC_SAFE_DELETE(m_pEffectArray);
}

#pragma mark 设置精灵在本地的ID并且添加其至精灵容器中 

void YYSpriteLayer::addElementToSpriteDictionary(YYSprite * sprite)
{
    sprite->setSpriteID(m_nSpriteIDCounter);
    m_pSpriteDictionary->setObject(sprite, m_nSpriteIDCounter);
    m_nSpriteIDCounter++;
}

#pragma mark 设置是否显示周围玩家

void YYSpriteLayer::setAroundPlayerVisible(bool _bVisible)
{
    if(m_pSpriteWithServerIDDict)
    {
        const char * playerID =  Controller::getInstance()->getPlayerID();
        CCArray * keyArray = m_pSpriteWithServerIDDict->allKeys();
        for (int i = 0; i < keyArray->count(); i++)
        {
            const char* key = ((CCString*)keyArray->objectAtIndex(i))->getCString();
            YYSprite* spr = (YYSprite *)m_pSpriteWithServerIDDict->objectForKey(key);
            if (spr)
            {
                SpriteData * spriteData  = spr->getSpriteData();
                if (spriteData && (spriteData->getType() == SPRITE_TYPE_PLAYER || spriteData->getType() == SPRITE_TYPE_TRIAL))
                {
                    if (strcmp(playerID, key))
                    {
                        spr->setVisible(_bVisible);
                        spr->getNameLabel()->setVisible(_bVisible);
                    }
                }
            }
        }
    }
}

#pragma mark 加载场景内的精灵 

void YYSpriteLayer::loadSprites(CCArray * array)
{
    if (array->count() < 4)
    {
        CCLOG("loadSprite error with wrong data");
    }
    int tileIndex = ((CCString *)array->objectAtIndex(0))->intValue();// 位置 
    int dir = ((CCString *)array->objectAtIndex(1))->intValue();// 朝向 
    int spriteIndex = ((CCString *)array->objectAtIndex(2))->intValue();// 精灵编号 
    SpriteData * spriteData = (SpriteData *)GlobalData::getInstance()->getSpriteDataDict()->objectForKey(spriteIndex);// 精灵基本数据 
    if (!spriteData)
    {// 跳过数据错误的精灵
        CCLOG("loadSprite failed with spriteIndex=%d",spriteIndex);
        return;
    }
    YYSprite * sprite = NULL;
    const char * spriteIDInServer = ((CCString *)array->objectAtIndex(3))->getCString();// 精灵在服务端的ID
    sprite = (YYSprite *)m_pSpriteWithServerIDDict->objectForKey(spriteIDInServer);
    if (!sprite)
    {// 未加载过该精灵 
        switch (spriteData->getType())
        {
            case SPRITE_TYPE_PLAYER:
            {// 玩家
                //精灵服务器ID中包含"trial"字段是试炼角色
                const char* isTrial = strstr(spriteIDInServer, "trial");
                if (Controller::getInstance()->isTrial() && isTrial)
                {
                    sprite = YYTrialPlayer::create();
                    sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite,true);
                    m_pEnemyArray->addObject(sprite);
                    ((YYTrialPlayer *)sprite)->setTenacity(spriteData->getTenacity());
                }
                else
                {
                    sprite = YYPlayer::create();
                    sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite,true);
                    m_pFriendlyRoleArray->addObject(sprite);
                    ((YYPlayer *)sprite)->setTenacity(spriteData->getTenacity());
                    CCArray * keyArray = sprite->getBatchNodeDictionary()->allKeys();
                    for (int i = 0; i < keyArray->count(); i++)
                    {
                        int key = ((CCInteger *)keyArray->objectAtIndex(i))->getValue();
                        CCSpriteBatchNode * batchNode = dynamic_cast<CCSpriteBatchNode*>(sprite->getBatchNodeDictionary()->objectForKey(key));
                        if (batchNode->getZOrder() == 0)
                        {// 翅膀只在主城显示
                            if (Controller::getInstance()->getSceneType() != MAIN_CITY)
                            {
                                batchNode->setVisible(false);
                            }
                            else
                            {
                                batchNode->setVisible(true);
                            }
                            break;
                        }
                    }
                }
            }
                break;
            case SPRITE_TYPE_BOSS:
            case SPRITE_TYPE_MONSTER:
            case SPRITE_TYPE_DESTRUCTIBLE_OBJECT:
            {
                sprite = YYMonster::create();
                sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite);
                m_pEnemyArray->addObject(sprite);
                if (spriteData->getType() == SPRITE_TYPE_MONSTER || spriteData->getType() == SPRITE_TYPE_BOSS)
                {
                    m_pMoveableEnemyArray->addObject(sprite);
                }
                // 精灵属性编号 
                SpriteAttribute * spriteAttribute = (SpriteAttribute * )GlobalData::getInstance()->getSpriteAttributeDict()->objectForKey(spriteData->getAttributeIndex());
                ((YYMonster *)sprite)->initFightData(spriteData, spriteAttribute);
                
                // 活动 世界boss 设置boss 血值
                if(Controller::getInstance()->getDungeonType() == kWorldBoss && spriteData->getType() == SPRITE_TYPE_BOSS)
                {
                    
                }
            }
                break;
            case SPRITE_TYPE_NPC:
            {
                sprite = YYNPC::create();
                sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite);
                m_pNPCArray->addObject(sprite);
            }
                break;
            case SPRITE_TYPE_DOOR:
            case SPRITE_TYPE_DUNGEONS:
                sprite = YYPortal::create();
                sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite);
                m_pDoorArray->addObject(sprite);
                sprite->setTileIndex(tileIndex);
                break;
            case SPRITE_TYPE_ROLLING_BUCKET:
            case SPRITE_TYPE_STAB:
            {
                sprite = YYTrap::create();
                sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite);
                sprite->setAction(ACTION_STAND0);
                // 精灵属性编号 
                SpriteAttribute * spriteAttribute = (SpriteAttribute * )GlobalData::getInstance()->getSpriteAttributeDict()->objectForKey(spriteData->getAttributeIndex());
                ((YYTrap *)sprite)->initFightData(spriteData, spriteAttribute);
                m_pTrapArray->addObject(sprite);
                if(spriteData->getType() == SPRITE_TYPE_STAB)
                {
                    m_pStabArray->addObject(sprite);
                    sprite->setZOrder(-1);
                }
            }
                break;
            default:
                sprite = YYSprite::create();
                sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite);
                break;
        }
        sprite->setFaceToRight(dir == 1);// 设置精灵朝向 
        sprite->setScale(spriteData->getScaleFactor());// 设置精灵缩放系数
        sprite->setSpriteType(spriteData->getType()); // 设置精灵类型
        if (spriteData->getType() == SPRITE_TYPE_PLAYER
            || spriteData->getType() == SPRITE_TYPE_BOSS
            || spriteData->getType() == SPRITE_TYPE_MONSTER
            || spriteData->getType() == SPRITE_TYPE_ROLLING_BUCKET
            || spriteData->getType() == SPRITE_TYPE_STAB
            || spriteData->getType() == SPRITE_TYPE_NPC
            || spriteData->getType() == SPRITE_TYPE_DESTRUCTIBLE_OBJECT
            || spriteData->getType() == SPRITE_TYPE_DUNGEONS
            || spriteData->getType() == SPRITE_TYPE_DOOR)
        {
            sprite->setAction(ACTION_STAND0);
        }
        else
        {
            sprite->setAction(0,true);
        }
        sprite->setSpriteData(spriteData);
        sprite->setSpriteIDInServer(spriteIDInServer);//精灵ID
        m_pSpriteWithServerIDDict->setObject(sprite, spriteIDInServer);
        addElementToSpriteDictionary(sprite);// 设置精灵ID并将精灵添加至精灵容器 
        // 设置精灵所在位置(传送门对齐地格中间，不随机位置) 
        CCPoint position = getSpritePosition(tileIndex,spriteData->getType() != SPRITE_TYPE_DOOR && spriteData->getType() != SPRITE_TYPE_DUNGEONS);
        sprite->setPosition(position);
        sprite->setCurrentTile(ccp((tileIndex - 10) % ((YYScene *)getParent())->getTileColumnAmount(),(tileIndex - 10)/((YYScene *)getParent())->getTileColumnAmount()));
        
        // 添加精灵 
        if (spriteData->getType() == SPRITE_TYPE_CIRCLE)
        {
            addChild(sprite,BELOW_SPRITE_LAYER);
        }
        else
        {
            addChild(sprite);
        }
        //阴影
        if (spriteData->getType() == SPRITE_TYPE_PLAYER
            || spriteData->getType() == SPRITE_TYPE_BOSS
            || spriteData->getType() == SPRITE_TYPE_NPC
            || spriteData->getType() == SPRITE_TYPE_MONSTER
            || spriteData->getType() == SPRITE_TYPE_DUNGEONS
            || spriteData->getType() == SPRITE_TYPE_DOOR)
        {
            std::string str = getFilePath("shadow", kDirTypePic, kFileSuffixPng);
            CCSprite *shadow = CCSprite::create(str.c_str());
            sprite->addChild(shadow,-1);
        }
        
        //boss脚下光圈
        if (spriteData->getType() == SPRITE_TYPE_BOSS)
        {
            YYSprite *mark = YYSprite::create();
            mark->initWithActionSet(spriteData->getBossMarkResource().c_str(), kDirTypeSprite);
            mark->setAction(0,true);
            mark->setTag(TAG_BOSS_MARK);
            sprite->addChild(mark,-1);
        }
        if (array->count() > 4)
        {// 服务器发送名字
            const char * name = ((CCString *)array->objectAtIndex(4))->getCString();
            sprite->setSpriteName(name);
            // 设置精灵名称
            sprite->setNameLabel(CCLabelTTF::create(sprite->getSpriteName()->getCString(), DEFAULT_FONT_NAME, 25));
            sprite->getNameLabel()->setPosition(sprite->getPosition() + ccp(0,spriteData->getNameOffsetHeight() * spriteData->getScaleFactor()));
            sprite->getNameLabel()->setAnchorPoint(ccp(0.5,0));
            addChild(sprite->getNameLabel(),1);
        }
        if (spriteData->getType() == SPRITE_TYPE_PLAYER)
        {
            sprite->getNameLabel()->setColor(ccc3(10,255,30));
        }
        else if (spriteData->getType() == SPRITE_TYPE_MONSTER)
        {
            ((YYMonster *)sprite)->initBloodBar(ccp(0,spriteData->getNameOffsetHeight() - 5));//初始化怪物血条
        }
    }

    if(array->count() > 5)
    {
        if (spriteData->getType() == SPRITE_TYPE_PLAYER)
        {// 玩家换装
            for(int i = 5; i < array->count(); i++)
            {
                const char* replaceEquipPic = ((CCString*)array->objectAtIndex(i))->getCString();
                if (std::string(replaceEquipPic).compare("") == 0)
                {
                    continue;
                }
                if(m_pRoleByControlled && strcmp(m_pRoleByControlled->getSpriteIDInServer()->getCString(),spriteIDInServer) == 0)
                {
                    m_bReplaceEquip = true;
                    m_pReplacePartPicArray->addObject(CCString::create(replaceEquipPic));
                }
                ((YYPlayer*)sprite)->replaceEquipment(replaceEquipPic);
            }
        }
        else if (spriteData->getType() == SPRITE_TYPE_BOSS || spriteData->getType() == SPRITE_TYPE_MONSTER)
        {// 隐藏敌人
            sprite->setVisible(false);
            sprite->getNameLabel()->setVisible(false);
            sprite->setVisibleConditionArray(split_string(((CCString *)array->objectAtIndex(5))->getCString(), ';'));
        }
        else if (spriteData->getType() == SPRITE_TYPE_DOOR || spriteData->getType() == SPRITE_TYPE_DUNGEONS)
        {
        // 隐藏传送门
            sprite->setVisible(false);
            sprite->getNameLabel()->setVisible(false);
            sprite->setTag(((CCString *)array->objectAtIndex(5))->intValue());
            if (array->count() > 6)
            {
                sprite->setVisibleConditionArray(split_string(((CCString *)array->objectAtIndex(6))->getCString(), ','));
            }
        }
        else if(spriteData->getType() == SPRITE_TYPE_NPC)
        {//NPCID
            const char * ID = static_cast<CCString *>(array->objectAtIndex(5))->getCString();
            ((YYNPC *)sprite)->setNPCID(ID);
        }
    }
}



#pragma mark 设置精灵位置

cocos2d::CCPoint YYSpriteLayer::getSpritePosition(int tileIndex, bool isRandom)
{
    int currentTileIndex = tileIndex - 10;// 当前所在地图格
    YYScene *scene = Controller::getInstance()->getScene();
    int x = currentTileIndex % scene->getTileColumnAmount() * scene->getTileWidth();
    int y = Controller::getInstance()->getSceneCellTopY() - currentTileIndex / scene->getTileColumnAmount() * scene->getTileHeight();
    int offsetX = scene->getTileWidth() / 2;
    int offsetY = scene->getTileHeight() / 2;
    if (isRandom)
    {
        offsetX = RANDOM_INT(5, scene->getTileWidth() - 5);
        offsetY = RANDOM_INT(5, scene->getTileHeight() - 5);
    }
    return ccpAdd(ccp(x,y), ccp(offsetX, -offsetY));
}

/**
 *删除离开场景内的其他玩家
 */
void YYSpriteLayer::removeSprites(const char *spriteID)
{
    if(m_pSpriteWithServerIDDict)
    {
        YYSprite * sprite = (YYSprite *)m_pSpriteWithServerIDDict->objectForKey(spriteID);
        if (sprite)
        {
            if (sprite->getSpriteType() == SPRITE_TYPE_PLAYER && sprite != m_pRoleByControlled)
            {
                sprite->setDeleted(true);
            }
        }
    }
}

void YYSpriteLayer::setSkillIndexData(cocos2d::CCArray *array)
{
    // SP_fchqwe.1_1300;1301;1302_2300;2301;2302_3000;3100;3200
    const char * playerID = ((CCString *)array->objectAtIndex(0))->getCString();
    CCArray * skillATArray = CCArray::create(); // 普通攻击
    CCArray * skillSSArray = CCArray::create(); // 快捷栏技能
    CCArray * skillSWArray = CCArray::create(); // 怒技
    for (int i = 1; i < array->count(); i++)
    {
        CCArray* tmp = split_string(static_cast<CCString *>(array->objectAtIndex(i))->getCString(),';');
        for (int j = 0; j < tmp->count(); j++)
        {
            int nIdx = (static_cast<CCString *>(tmp->objectAtIndex(j)))->intValue();   // 技能索引
            CCString* strIdx = CCString::createWithFormat("%d",nIdx);
            int nType = getFirstNBitNumber(nIdx,1);
            switch (nType)
            {
                case 1:
                    skillATArray->addObject(strIdx);
                    break;
                case 2:
                    skillSSArray->addObject(strIdx);
                    break;
                case 3:
                    skillSWArray->addObject(strIdx);
                    break;
                default:
                    break;
            }
        }
    }

    // 普通攻击技能
    GlobalData::getInstance()->getPlayerAttackIndexDict()->removeAllObjects();
    GlobalData::getInstance()->getPlayerAttackIndexDict()->setObject(skillATArray, playerID);
    if (m_pRoleByControlled)
    {
        m_pRoleByControlled->setAttackAmount(skillATArray->count());
    }

    // 主角快捷栏技能
    GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->removeAllObjects();
    GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->setObject(skillSSArray, playerID);
    if (m_pRoleByControlled)
    {
        if (Controller::getInstance()->getSceneType() == DUNGEONS || Controller::getInstance()->getSceneType() == WAR_ZONE)
        {
            ((YYScene *)getParent())->getControlUILayer()->setShortcutSkillIcon(skillSSArray);
        }
    }

    // 怒气技能
    GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->removeAllObjects();
    GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->setObject(skillSWArray, playerID);

    // 预加载技能资源
    preLoadSkillEffectResource(skillATArray);
    preLoadSkillEffectResource(skillSSArray);
    preLoadSkillEffectResource(skillSWArray);
}

#pragma mark 预加载技能特效资源

void YYSpriteLayer::preLoadSkillEffectResource(cocos2d::CCArray * array)
{
    if (Controller::getInstance()->getSceneType() == DUNGEONS
        || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {
        for (int i = 0; i < array->count(); i++)
        {
            int skillDataIndex = ((CCString *)array->objectAtIndex(i))->intValue();
            if (skillDataIndex < 1)
            {
                continue;
            }
            SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByControlled->getSpriteIDInServer()->getCString(),skillDataIndex);
            //CCLOG("preLoadSkillEffectResource %s %d",m_pRoleByControlled->getSpriteIDInServer()->getCString(),skillDataIndex);
            for (int m = 0; m < skillData->getActionIndexArray()->count(); m++)
            {
                int skillActionIndex = ((CCString *)skillData->getActionIndexArray()->objectAtIndex(m))->intValue();
                SkillActionData * skillActionData = (SkillActionData *)GlobalData::getInstance()->getSkillActionDataDict()->objectForKey(skillActionIndex);
                for (int n = 0; n < skillActionData->getEffectDataArray()->count(); n++)
                {
                    int skillEffectIndex = ((CCString *)skillActionData->getEffectDataArray()->objectAtIndex(n))->intValue();
                    SkillEffectData * skillEffectData = (SkillEffectData *)GlobalData::getInstance()->getSkillEffectDataDict()->objectForKey(skillEffectIndex);
                    CCArray* array = split_string( skillEffectData->getResource().c_str(),',');
                    for (int j = 0; j < array->count(); j++)
                    {
                        CCString * fileName = (CCString*)array->objectAtIndex(j);
                        std::string strPath = "";
                        if (j == 0)
                        {
                            strPath = getFilePath(fileName->getCString(), kDirTypeSprite, kFileSuffixAnu);
                            GlobalData::getInstance()->getActionSet(strPath);
                        }
                        else
                        {
                            strPath = getFilePath(fileName->getCString(), kDirTypeSprite, kFileSuffixPng);
                            CCTextureCache::sharedTextureCache()->addImage(strPath.c_str());
                        }
                    }
                }
            }
        }
    }
}

void YYSpriteLayer::preLoadTrialSkillEffectResource(cocos2d::CCArray * array)
{
    if (Controller::getInstance()->getSceneType() == DUNGEONS
        || Controller::getInstance()->getSceneType() == WAR_ZONE)
    {
        for (int i = 0; i < array->count(); i++)
        {
            int skillDataIndex = ((CCString *)array->objectAtIndex(i))->intValue();
            if (skillDataIndex < 1)
            {
                continue;
            }
            SkillData *skillData = GlobalData::getInstance()->getPlayerSkillData(m_pRoleByTrial->getSpriteIDInServer()->getCString(),skillDataIndex);
            for (int m = 0; m < skillData->getActionIndexArray()->count(); m++)
            {
                int skillActionIndex = ((CCString *)skillData->getActionIndexArray()->objectAtIndex(m))->intValue();
                SkillActionData * skillActionData = (SkillActionData *)GlobalData::getInstance()->getSkillActionDataDict()->objectForKey(skillActionIndex);
                for (int n = 0; n < skillActionData->getEffectDataArray()->count(); n++)
                {
                    int skillEffectIndex = ((CCString *)skillActionData->getEffectDataArray()->objectAtIndex(n))->intValue();
                    SkillEffectData * skillEffectData = (SkillEffectData *)GlobalData::getInstance()->getSkillEffectDataDict()->objectForKey(skillEffectIndex);
                    CCArray* array = split_string( skillEffectData->getResource().c_str(),',');
                    for (int j = 0; j < array->count(); j++)
                    {
                        CCString * fileName = (CCString*)array->objectAtIndex(j);
                        std::string strPath = "";
                        if (j == 0)
                        {
                            strPath = getFilePath(fileName->getCString(), kDirTypeSprite, kFileSuffixAnu);
                            GlobalData::getInstance()->getActionSet(strPath);
                        }
                        else
                        {
                            strPath = getFilePath(fileName->getCString(), kDirTypeSprite, kFileSuffixPng);
                            CCTextureCache::sharedTextureCache()->addImage(strPath.c_str());
                        }
                    }
                }
            }
        }
    }
}

#pragma mark 设置控制角色
void YYSpriteLayer::setRoleControlled(cocos2d::CCArray * array)
{
    const char * spriteID = ((CCString *)array->objectAtIndex(0))->getCString();// 精灵ID
    m_pRoleByControlled = (YYPlayer *)m_pSpriteWithServerIDDict->objectForKey(spriteID);// 通过ID获得控制角色
    if (m_pRoleByControlled)
    {
        m_pRoleByControlled->setControlledBySelf(true);
        Controller::getInstance()->setProfessionIndex(m_pRoleByControlled->getSpriteData()->getSpriteIndex());//设置所控制角色的职业 
        // 玩家控制角色脚底增加光环 
        YYSprite * sprite = YYSprite::createWithActionSet("guangquan", "guangquan",kDirTypeSprite);
        sprite->setAction(0, true);
        m_pRoleByControlled->addChild(sprite,-1);
//        sprite->setTag(TAG_CIRCLE);
        m_pRoleByControlled->getYYSpriteChildren()->addObject(sprite);
        
        ((YYScene *)getParent())->updateCameraPosition();//更新镜头位置 
//        // 设置主角普通攻击 
        CCArray * skillIndexArray = (CCArray *)GlobalData::getInstance()->getPlayerAttackIndexDict()->objectForKey(spriteID);
        if (skillIndexArray)
        {
            m_pRoleByControlled->setAttackAmount(skillIndexArray->count());
            preLoadSkillEffectResource(skillIndexArray);
        }
        
        // 设置主角快捷技能 
        skillIndexArray = (CCArray *)GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(spriteID);
        if (skillIndexArray)
        {
            preLoadSkillEffectResource(skillIndexArray);
        }
        
        // 设置主角手势技能 
        skillIndexArray = (CCArray *)GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(spriteID);
        if (skillIndexArray)
        {
            preLoadSkillEffectResource(skillIndexArray);
        }
        
        // 设置主角属性 
        CCArray *spriteAttributeArray = (CCArray *)GlobalData::getInstance()->getPlayerAttributeDict()->objectForKey(spriteID);
        if (spriteAttributeArray)
        {
            setPlayerAttribute(spriteID,spriteAttributeArray);
        }
        
        ((YYScene *)getParent())->getControlUILayer()->setRoleByControlled(m_pRoleByControlled);
        ((YYScene *)getParent())->getPopUpUI()->setRoleByControlled(m_pRoleByControlled);
        ((YYScene *)getParent())->getControlUILayer()->initElements();
        
        setAroundPlayerVisible(Config::getInstance()->isAroundPlayerShow());
    }
    else
    {
        CCLOG("m_pRoleByControlled = NULL");
    }
    //设置试炼角色
    if (Controller::getInstance()->isTrial())
    {
        //查找试炼精灵ID
        const char* strTrialID = NULL;
        CCArray* arrKey = m_pSpriteWithServerIDDict->allKeys();
        for (int i = 0; i < arrKey->count(); i++)
        {
            const char* sprID = ((CCString*)arrKey->objectAtIndex(i))->getCString();
            const char* isTrial = strstr(sprID, "trial");
            if (isTrial)
            {
                strTrialID = sprID;
                break;
            }
        }
        m_pRoleByTrial = (YYTrialPlayer *)m_pSpriteWithServerIDDict->objectForKey(strTrialID);// 通过ID获得试炼角色
        m_pRoleByTrial->setSpriteType(SPRITE_TYPE_TRIAL); //设置类型
        if (m_pRoleByTrial)
        {
            // 设置试炼普通攻击
            CCArray * skillIndexArray = (CCArray *)GlobalData::getInstance()->getPlayerAttackIndexDict()->objectForKey(strTrialID);
            if (skillIndexArray)
            {
                m_pRoleByTrial->setAttackAmount(skillIndexArray->count());
                preLoadTrialSkillEffectResource(skillIndexArray);
            }
            
            // 设置试炼快捷技能
            skillIndexArray = (CCArray *)GlobalData::getInstance()->getPlayerShortcutSkillIndexDict()->objectForKey(strTrialID);
            if (skillIndexArray)
            {
                preLoadTrialSkillEffectResource(skillIndexArray);
            }
            
            // 设置试炼手势技能
            skillIndexArray = (CCArray *)GlobalData::getInstance()->getPlayerGestureSkillIndexDict()->objectForKey(strTrialID);
            if (skillIndexArray)
            {
                preLoadTrialSkillEffectResource(skillIndexArray);
            }
            
            // 设置属性
            CCArray *spriteAttributeArray = (CCArray *)GlobalData::getInstance()->getPlayerAttributeDict()->objectForKey(strTrialID);
            setTrialPlayerAttribute(strTrialID,spriteAttributeArray);
            ((YYScene *)getParent())->getControlUILayer()->setRoleByTrial(m_pRoleByTrial);
            ((YYScene *)getParent())->getControlUILayer()->trialCommonUI();
            
            //设定攻击目标
            m_pRoleByTrial->setTargetSprite(m_pRoleByControlled);
            m_pRoleByControlled->setTargetSprite(m_pRoleByTrial);
            
            //试炼倒计时结束前暂停
            m_pRoleByTrial->setTrialPause(true);
            m_pRoleByControlled->setTrialPause(true);
            ((YYScene *)getParent())->getControlUILayer()->setPlayTrialCounter(true);
        }
    }
}

#define GOLDBEZIER_LAYER		21
/**
*金币在场景中画弧线
*/
void YYSpriteLayer::bezierMove(int nSpriteType)
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCPoint startPoint(size.width / 2, 30);
	CCPoint endPoint(Controller::getInstance()->getScene()->getControlUILayer()->m_PlayerPortraitPoint);//(50, size.height - 30);
	CCSprite *bezier = NULL;
	if (1 == nSpriteType)
		bezier = CCSprite::create("ui/300.png");
	if (2 == nSpriteType)
		bezier = CCSprite::create("ui/298.png");
	bezier->setPosition(startPoint);
	addChild(bezier, GOLDBEZIER_LAYER);
	ccBezierConfig bezierCo;
	bezierCo.controlPoint_1 = ccp(startPoint.x - 50, startPoint.y + 50);
	bezierCo.controlPoint_2 = ccp(endPoint.x + 50, endPoint.y - 50);
	bezierCo.endPosition = endPoint;
	CCCallFuncN* callFunc = CCCallFuncN::create(Controller::getInstance()->getScene(), callfuncN_selector(YYScene::funDestroy));
	CCFiniteTimeAction*  rep = CCSequence::create(CCBezierTo::create(2, bezierCo), callFunc, NULL);
	bezier->runAction(rep);
}

/**
 *设置玩家控制角色的属性
 */
void YYSpriteLayer::setPlayerAttribute(const char * spriteID,cocos2d::CCArray * array)
{
    YYPlayer * player = (YYPlayer *)m_pSpriteWithServerIDDict->objectForKey(spriteID);
    if (player)
    {
        const char * name = ((CCString *)array->objectAtIndex(1))->getCString();// 名字 
        player->setSpriteName(name);// 设置姓名 
        int level = ((CCString *)array->objectAtIndex(2))->intValue();// 等级
        if (player->getLevel() > 0 && player->getLevel()< level)
        {
            levelUpEffect(player);
        }
        player->setLevel(level);
        if(((YYScene *)getParent())->getControlUILayer()->getLevelLabel()){// 更新UI界面等级显示
            const char *temp = CCString::createWithFormat("%d",level)->getCString();
            ((YYScene *)getParent())->getControlUILayer()->getLevelLabel()->setString(temp);
        }
        SpriteAttribute *attribute = static_cast<SpriteAttribute *>(GlobalData::getInstance()->getSpriteAttributeDict()->objectForKey(player->getSpriteData()->getSpriteIndex()));
        if (!attribute)
        {
            attribute = SpriteAttribute::create();
            GlobalData::getInstance()->getSpriteAttributeDict()->setObject(attribute, player->getSpriteData()->getSpriteIndex());
        }
        player->setSpriteAttribute(attribute);

        int hp = ((CCString *)array->objectAtIndex(3))->intValue();// 最大血量 
        
        attribute->setMaxHP(hp);
        player->setCurrentHP(hp);
        
        int mp = ((CCString *)array->objectAtIndex(4))->intValue();// 当前魔法值 
        attribute->setMaxMP(mp);
        player->setCurrentMP(mp);
        
        int sp = ((CCString *)array->objectAtIndex(5))->intValue();
        player->setMaxSP(sp);
        player->setCurrentSP(sp);
        
        int currentExp = ((CCString *)array->objectAtIndex(6))->intValue();// 当前经验值 
        player->setCurrentExp(currentExp);
        int levelupExp = ((CCString *)array->objectAtIndex(7))->intValue();// 升级所需经验值
        player->setLevelUpExp(levelupExp);
        int ap = ((CCString *)array->objectAtIndex(8))->intValue();// 攻击 
        attribute->setAttackPoint(ap);
        int sap = ((CCString *)array->objectAtIndex(9))->intValue();// 魔法攻击 
        attribute->setSpellAttackPoint(sap);
        int hitPoint = ((CCString *)array->objectAtIndex(10))->intValue();// 命中等级 
        attribute->setHitLevel(hitPoint);
        int criticalPoint = ((CCString *)array->objectAtIndex(11))->intValue();// 暴击等级 
        attribute->setCriticalLevel(criticalPoint);

        int gold = ((CCString *)array->objectAtIndex(12))->intValue();// 金币 
        attribute->setGold(gold);
//		if(m_bFirstSetGold)
//		{
//			m_nPreGold = gold;
//			m_bFirstSetGold = false;
//		}
//		else
//		{
//			if (m_nPreGold < gold)
//			{
//				m_nPreGold = gold;
//				bezierMove(1);
//			}
//		}
        int jewel = ((CCString *)array->objectAtIndex(13))->intValue();// 宝石
        attribute->setJewel(jewel);
//		if(m_bFirstSetJewel)
//		{
//			m_nPreJewel = jewel;
//			m_bFirstSetJewel = false;
//		}
//		else
//		{
//			if (m_nPreJewel < jewel)
//			{
//				m_nPreJewel = jewel;
//				bezierMove(2);
//			}
//		}
        int PSPoint = ((CCString *)array->objectAtIndex(14))->intValue();// 体力值
        attribute->setPhysicalStrengthPoint(PSPoint);
        int IntelligencePoint = ((CCString *)array->objectAtIndex(15))->intValue();// 智力 
        attribute->setIntelligencePoint(IntelligencePoint);
        int StrengthPoint = ((CCString *)array->objectAtIndex(16))->intValue();// 智力 
        attribute->setStrengthPoint(StrengthPoint);
        int AgilityPoint = ((CCString *)array->objectAtIndex(17))->intValue();// 敏捷 
        attribute->setAgilityPoint(AgilityPoint);
        int StaminaPoint = ((CCString *)array->objectAtIndex(18))->intValue();// 耐力 
        attribute->setStaminaPoint(StaminaPoint);
        int defensePoint = ((CCString *)array->objectAtIndex(19))->intValue();// 物理防御
        attribute->setDefencePoint(defensePoint);
        int skillDamagePoint = ((CCString *)array->objectAtIndex(20))->intValue();// 法术防御 
        attribute->setSkillDamagePoint(skillDamagePoint);
        int FightingCapacity = ((CCString *)array->objectAtIndex(21))->intValue();// 战斗力 
        attribute->setFightingCapacity(FightingCapacity);
        int talentPoint = ((CCString *)array->objectAtIndex(22))->intValue();// 天赋点 
        attribute->setTalentPoint(talentPoint);
        attribute->setTempTalentPoint(talentPoint);
        int nVipLv = ((CCString *)array->objectAtIndex(23))->intValue();// vip等级
        attribute->setVipLevel(nVipLv);
        int nVipJew = ((CCString *)array->objectAtIndex(24))->intValue();// vip 已充值宝石
        attribute->setVipJew(nVipJew);
        attribute->setBasicHitOdds(100);
    }
}

/**
 *设置试炼玩家控制角色的属性
 */
void YYSpriteLayer::setTrialPlayerAttribute(const char * spriteID,cocos2d::CCArray * array)
{
    YYTrialPlayer * player = (YYTrialPlayer *)m_pSpriteWithServerIDDict->objectForKey(spriteID);
    if (player)
    {
        const char * name = ((CCString *)array->objectAtIndex(1))->getCString();// 名字
        player->setSpriteName(name);// 设置姓名
        
        int level = ((CCString *)array->objectAtIndex(2))->intValue();// 等级
        player->setLevel(level);
        if(((YYScene *)getParent())->getControlUILayer()->getTrialLevelLabel()){// 更新UI界面等级显示
            const char *temp = CCString::createWithFormat("%d",level)->getCString();
            ((YYScene *)getParent())->getControlUILayer()->getTrialLevelLabel()->setString(temp);
        }
        
        SpriteAttribute *attribute = static_cast<SpriteAttribute *>(GlobalData::getInstance()->getSpriteAttributeDict()->objectForKey(player->getSpriteData()->getSpriteIndex()));
        if (!attribute)
        {
            attribute = SpriteAttribute::create();
            GlobalData::getInstance()->getSpriteAttributeDict()->setObject(attribute, player->getSpriteData()->getSpriteIndex());
        }
        player->setSpriteAttribute(attribute);
        
        int hp = ((CCString *)array->objectAtIndex(3))->intValue();// 最大血量
        
        attribute->setMaxHP(hp);
        player->setCurrentHP(hp);
        
        int mp = ((CCString *)array->objectAtIndex(4))->intValue();// 当前魔法值
        attribute->setMaxMP(mp);
        player->setCurrentMP(mp);
        
        int sp = ((CCString *)array->objectAtIndex(5))->intValue();
        player->setMaxSP(sp);
        player->setCurrentSP(sp);
        
        int currentExp = ((CCString *)array->objectAtIndex(6))->intValue();// 当前经验值
        player->setCurrentExp(currentExp);
        int levelupExp = ((CCString *)array->objectAtIndex(7))->intValue();// 升级所需经验值
        player->setLevelUpExp(levelupExp);
        
        int ap = ((CCString *)array->objectAtIndex(8))->intValue();// 攻击
        attribute->setAttackPoint(ap);
        int sap = ((CCString *)array->objectAtIndex(9))->intValue();// 魔法攻击
        attribute->setSpellAttackPoint(sap);
        int hitPoint = ((CCString *)array->objectAtIndex(10))->intValue();// 命中等级
        attribute->setHitLevel(hitPoint);
        int criticalPoint = ((CCString *)array->objectAtIndex(11))->intValue();// 暴击等级
        attribute->setCriticalLevel(criticalPoint);
        int gold = ((CCString *)array->objectAtIndex(12))->intValue();// 金币
        attribute->setGold(gold);
        int jewel = ((CCString *)array->objectAtIndex(13))->intValue();// 宝石
        attribute->setJewel(jewel);
        int PSPoint = ((CCString *)array->objectAtIndex(14))->intValue();// 体力值
        attribute->setPhysicalStrengthPoint(PSPoint);
        
        int IntelligencePoint = ((CCString *)array->objectAtIndex(15))->intValue();// 智力
        attribute->setIntelligencePoint(IntelligencePoint);
        int StrengthPoint = ((CCString *)array->objectAtIndex(16))->intValue();// 智力
        attribute->setStrengthPoint(StrengthPoint);
        int AgilityPoint = ((CCString *)array->objectAtIndex(17))->intValue();// 敏捷
        attribute->setAgilityPoint(AgilityPoint);
        int StaminaPoint = ((CCString *)array->objectAtIndex(18))->intValue();// 耐力
        attribute->setStaminaPoint(StaminaPoint);
        int defensePoint = ((CCString *)array->objectAtIndex(19))->intValue();// 物理防御
        attribute->setDefencePoint(defensePoint);
        int skillDamagePoint = ((CCString *)array->objectAtIndex(20))->intValue();// 法术防御
        attribute->setSkillDamagePoint(skillDamagePoint);
        int FightingCapacity = ((CCString *)array->objectAtIndex(21))->intValue();// 战斗力
        attribute->setFightingCapacity(FightingCapacity);
        int talentPoint = ((CCString *)array->objectAtIndex(22))->intValue();// 天赋点
        attribute->setTalentPoint(talentPoint);
        attribute->setBasicHitOdds(100);
    }
}

#pragma mark 设置血瓶、魔瓶掉落率 

void YYSpriteLayer::setDropOdds(cocos2d::CCArray * array)
{
    setDropOdds(((CCString *)array->objectAtIndex(0))->intValue());
}

void YYSpriteLayer::levelUpEffect(YYSprite * sprite)
{
    YYSprite * levelupEffect = YYSprite::createWithActionSet("levelup", "levelup", kDirTypeEffect);
    levelupEffect->setAction(0, false);
//    levelupEffect->setTag(TAG_LEVEL_UP_EFFECT);
    levelupEffect->setDeleteCondition(DELETE_ACTION_OVER);
    sprite->addChild(levelupEffect,20);
    sprite->getYYSpriteChildren()->addObject(levelupEffect);
    const std::string filePath = getFilePath(UPDATE_EFFECT ,kDirTypeSound ,kFileSuffixWav);
    PLAY_EFFECT(filePath.c_str());
}

#pragma mark处理战斗精灵移动 

void YYSpriteLayer::processSpriteMove(cocos2d::CCArray *array)
{
    const char *spriteID = ((CCString *)array->objectAtIndex(2))->getCString();// 精灵ID
    YYFightSprite * fs = (YYFightSprite *)m_pSpriteWithServerIDDict->objectForKey(spriteID);// 查找是否已存在该玩家
    if (fs)
    {
        if (m_pRoleByControlled->getSpriteIDInServer()->isEqual(CCString::create(spriteID)))
        {// 不处理自身 
            return;
        }
        if (fs->getSpriteType() != SPRITE_TYPE_PLAYER)
        {
            return;
        }
        int currentTileIndex = ((CCString *)array->objectAtIndex(0))->intValue() - 10;// 当前所在地图元素格序号 
        fs->setCurrentTile(ccp(currentTileIndex%((YYScene *)getParent())->getTileColumnAmount(),currentTileIndex/((YYScene *)getParent())->getTileColumnAmount()));// 当前所在元素格行、列数 
        int tileIndex = ((CCString *)array->objectAtIndex(1))->intValue();// 即将前往的地图格索引 
        fs->setDestination(getSpritePosition(tileIndex));
        fs->setMoving(true);
        fs->setMoveType(MOVE_BY_SET_DESTINATION);
    }
}

#pragma mark 循环 

void YYSpriteLayer::update(void)
{
    sortSprite();
    if (m_pSpriteDictionary)
    {
        CCArray * array = m_pSpriteDictionary->allKeys();
        for (int i = 0; i < array->count();)
        {
            int key = ((CCInteger *)array->objectAtIndex(i))->getValue();
            YYSprite * sprite = (YYSprite *)m_pSpriteDictionary->objectForKey(key);
            if (sprite)
            {
                if (sprite->isDeleted())
                {
                    removeChild(sprite->getNameLabel());
                    switch (sprite->getSpriteType())
                    {
                        case SPRITE_TYPE_PLAYER:
                        case SPRITE_TYPE_TRIAL:
                            m_pSpriteWithServerIDDict->removeObjectForKey(sprite->getSpriteIDInServer()->getCString());
                            m_pFriendlyRoleArray->removeObject(sprite);
                            removeDefunctTarget(m_pMoveableEnemyArray,(YYPlayer *)sprite);
                            break;
                        case SPRITE_TYPE_PET:
                            m_pFriendlyRoleArray->removeObject(sprite);
                            removeDefunctTarget(m_pMoveableEnemyArray,(YYFightSprite *)sprite);
                            break;
                        case SPRITE_TYPE_MONSTER:
                        case SPRITE_TYPE_BOSS:
                        {
                            
                            if (sprite->getSpriteType() != SPRITE_TYPE_MONSTER || !((YYMonster *)sprite)->isSummoned())
                            {
                                m_pSpriteWithServerIDDict->removeObjectForKey(sprite->getSpriteIDInServer()->getCString());
                            }
                            m_pMoveableEnemyArray->removeObject(sprite);
                            m_pEnemyArray->removeObject(sprite);
                            YYFightSprite *enemy = (YYFightSprite *)sprite;
                            dropItem(enemy);
                            processEnemyDeath(enemy);
                            removeDefunctTarget(m_pFriendlyRoleArray,enemy);
                            break;
                        }
                        case SPRITE_TYPE_DESTRUCTIBLE_OBJECT:
                            dropItem((YYFightSprite *)sprite, true);
                            m_pSpriteWithServerIDDict->removeObjectForKey(sprite->getSpriteIDInServer()->getCString());
                            m_pEnemyArray->removeObject(sprite);
                            {
                             const std::string filePath = getFilePath(CUTTREE_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                             PLAY_EFFECT(filePath.c_str());
                            }
                            break;
                        case SPRITE_TYPE_NPC:
                            m_pSpriteWithServerIDDict->removeObjectForKey(sprite->getSpriteIDInServer()->getCString());
                            m_pNPCArray->removeObject(sprite);
                            break;
                        case SPRITE_TYPE_DUNGEONS:
                        case SPRITE_TYPE_DOOR:
                        case SPRITE_TYPE_CIRCLE:
                            m_pSpriteWithServerIDDict->removeObjectForKey(sprite->getSpriteIDInServer()->getCString());
                            m_pDoorArray->removeObject(sprite);
                            break;
                        case SPRITE_TYPE_ITEM:
                            m_pMedicamentArray->removeObject(sprite);
                            break;
                        case SPRITE_TYPE_STAB:
                        case SPRITE_TYPE_ROLLING_BUCKET:
                            m_pSpriteWithServerIDDict->removeObjectForKey(sprite->getSpriteIDInServer()->getCString());
                            m_pTrapArray->removeObject(sprite);
                            break;
                        default:
                            break;
                    }
                    removeChild(sprite);
                    m_pSpriteDictionary->removeObjectForKey(key);
                    array->removeObjectAtIndex(i);
                }
                else
                {
                    if (sprite->isVisible())
                    {
                        sprite->update();
                        // 如果boss 在屏幕内显示boss血条 
                        if (sprite->getSpriteType() == SPRITE_TYPE_BOSS)
                        {
                            if (sprite->isIntersectsRect(CCRectMake(((YYScene *)getParent())->getScreenDatumPoint().x, ((YYScene *)getParent())->getScreenDatumPoint().y, CCDirector::sharedDirector()->getWinSize().width, CCDirector::sharedDirector()->getWinSize().height)))
                            {
                                if (!Controller::getInstance()->getScene()->getControlUILayer()->getBoss())
                                {
                                    Controller::getInstance()->getScene()->getControlUILayer()->setBoss((YYFightSprite *)sprite);
                                }
                            }
                        }
                    }
                    i++;
                }
            }
            else
            {
                i++;
            }
        }
    }
    
    if(m_pHitEffectArray)// 打击特效 
    {
        for(int i = 0; i < m_pHitEffectArray->count();)
        {
            YYSprite *hitEffect = (YYSprite*)m_pHitEffectArray->objectAtIndex(i);
            if(hitEffect->isActionOver())
            {
                this->removeChild(hitEffect);
                m_pHitEffectArray->removeObject(hitEffect);
            }
            else
            {
                hitEffect->update();
                i++;
            }
        }
    }
    if (m_pDodgeEffectArray)
    {// 躲闪 
        for(int i = 0 ; i < m_pDodgeEffectArray->count();)
        {
            CCLabelTTF * label = (CCLabelTTF *)(m_pDodgeEffectArray->objectAtIndex(i));
            int temp = ((CCInteger *)m_pDodgeEffectArray->objectAtIndex(i+1))->getValue();
            
            if (temp < DAMAGE_DURATION)
            {
                temp ++;
                if (temp > 4)
                {
                    label->setPosition(ccpAdd(label->getPosition(),ccp(0,DAMAGE_OFFSET_Y_PER_FRAME)));
                }
                m_pDodgeEffectArray->removeObjectAtIndex(i+1,true);
                m_pDodgeEffectArray->insertObject(CCInteger::create(temp), i+1);
                i+=2;
            }
            else
            {
                removeChild(label);
                m_pDodgeEffectArray->removeObjectAtIndex(i,true);
                m_pDodgeEffectArray->removeObjectAtIndex(i,true);
            }
        }
    }
    if (m_pDamageEffectArray)
    {// 处理伤害数字及闪避 
        for(int i = 0 ; i < m_pDamageEffectArray->count();)
        {
            CCNode * node = (CCNode *)(m_pDamageEffectArray->objectAtIndex(i));
            int temp = ((CCInteger *)m_pDamageEffectArray->objectAtIndex(i + 1))->getValue();
            bool isCritical = ((CCBool *)m_pDamageEffectArray->objectAtIndex(i + 2))->getValue();
            
            if (temp < (isCritical ? DAMAGE_DURATION + node->getChildrenCount() * 3 : DAMAGE_DURATION))
            {
                if (isCritical && temp < node->getChildrenCount() * 3)
                {
                    for (int i = 0; i < node->getChildrenCount(); i ++)
                    {
                        if (i == temp % node->getChildrenCount())
                        {
                            ((CCSprite *)node->getChildren()->objectAtIndex(i))->setScale(1 + (temp / node->getChildrenCount())* 0.1);
                        }
                    }
                }
                temp++;
                if (!isCritical || temp >= node->getChildrenCount() * 3)
                {
                    node->setPosition(ccpAdd(node->getPosition(),ccp(0,DAMAGE_OFFSET_Y_PER_FRAME)));
                }
                m_pDamageEffectArray->removeObjectAtIndex(i + 1,true);
                m_pDamageEffectArray->insertObject(CCInteger::create(temp), i + 1);
                i += 3;
            }
            else
            {
                removeChild(node);
                m_pDamageEffectArray->removeObjectAtIndex(i);
                m_pDamageEffectArray->removeObjectAtIndex(i);
                m_pDamageEffectArray->removeObjectAtIndex(i);
            }
        }
    }
    
    if (m_pGestureSkillShadeArray)
    {
        if(m_pGestureSkillShadeArray->count() > 1)
        {
            CCSprite * shade = (CCSprite*)(m_pGestureSkillShadeArray->objectAtIndex(0));
            Controller::getInstance()->getScene()->getControlUILayer()->removeChild(shade);
            m_pGestureSkillShadeArray->removeObjectAtIndex(0);
        }
        for(int i = 0;i < m_pGestureSkillShadeArray->count();i++)
        {
            CCSprite * shade = (CCSprite*)(m_pGestureSkillShadeArray->objectAtIndex(i));
            if(shade->getActionByTag(TAG_GESTURE_ANIMATION) == NULL)
            {
                Controller::getInstance()->getScene()->getControlUILayer()->removeChild(shade);
                m_pGestureSkillShadeArray->removeObjectAtIndex(i);
            }
        }
    }
    if (m_pEffectArray)
    {
        for (int i = 0; i < m_pEffectArray->count();)
        {
            YYSprite * sprite = static_cast<YYSprite *>(m_pEffectArray->objectAtIndex(i));
            if (sprite->isDeleted() || sprite->isActionOver())
            {
                if(sprite->getParent())
                {
                    sprite->removeFromParent();
                }
                m_pEffectArray->removeObjectAtIndex(i);
            }
            else
            {
                sprite->update();
                i++;
            }
        }
    }
}

/**
 * 精灵排序
 * 排序只针对第0层精灵，其他层多为特效或地表装饰物（传送门等）
 */
void YYSpriteLayer::sortSprite()
{
    // 始终在最下
    for (int i = 0; i < getChildren()->count();i++)
    {
        YYFightSprite * sprite1 = (YYFightSprite *)getChildren()->objectAtIndex(i);
        if (sprite1->getZOrder() != 0 || !sprite1->isVisible())
        {
            continue;
        }
        YYFightSprite * fs = NULL;
        int x = i;
        float tempY = sprite1->getPositionY();
        for (int j = i + 1; j < getChildren()->count(); j++)
        {
            YYFightSprite * sprite2 = (YYFightSprite *)getChildren()->objectAtIndex(j);
            if (sprite2->getZOrder() != 0 || !sprite2->isVisible())
            {
                continue;
            }
            float tempY2 = sprite2->getPositionY();
            if (tempY < tempY2)
            {
                tempY = tempY2;
                fs = sprite2;
                x = j;
            }
        }
        if (fs)
        {
            getChildren()->replaceObjectAtIndex(i, fs);
            getChildren()->replaceObjectAtIndex(x, sprite1);
        }
    }
}

#pragma mark物品掉落 

void YYSpriteLayer::dropItem(YYFightSprite *fightSprite, bool isDrop100Percent)
{
    // 血瓶掉落 
    if (RANDOM_INT(0, 99) < m_u8DropOdds || isDrop100Percent)
    {
        int ran = RANDOM_INT(0, 1);
        SpriteData * spriteData = (SpriteData *)GlobalData::getInstance()->getSpriteDataDict()->objectForKey(452 + ran);// 精灵基本数据 
        YYSprite * sprite = YYSprite::create();
        sprite->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite,false);
        sprite->setAction(0, true);
        addChild(sprite);
        // 设置精灵类型 
        sprite->setSpriteType(spriteData->getType());
        
        // 设置精灵ID并将精灵添加至精灵容器 
        addElementToSpriteDictionary(sprite);
        
        m_pMedicamentArray->addObject(sprite);
        // 设置精灵所在位置 
        sprite->setCurrentTile(fightSprite->getCurrentTile());
        int offsetX = RANDOM_INT(5, ((YYScene *)getParent())->getTileWidth() - 5);
        int offsetY = RANDOM_INT(5, ((YYScene *)getParent())->getTileHeight() - 5);
        sprite->setPosition(ccpAdd(ccp(((YYScene *)getParent())->getTileWidth() *  sprite->getCurrentTile().x,Controller::getInstance()->getSceneCellTopY() - ((YYScene *)getParent())->getTileHeight() *  sprite->getCurrentTile().y),ccp(offsetX,-offsetY)));
        // 显示名称 
        sprite->setSpriteName(spriteData->getSpriteName().c_str());
        CCLabelTTF* label = CCLabelTTF::create(sprite->getSpriteName()->getCString(), DEFAULT_FONT_NAME, 20);
        label->setPosition(ccp(0,sprite->getDefaultSize().height + 10));
        sprite->addChild(label,1);
    }
}

void YYSpriteLayer::removeDefunctTarget(cocos2d::CCArray *array, YYFightSprite * fightSprite)
{
    // 去除已死亡的目标 
    CCObject *object = NULL;
    CCARRAY_FOREACH(array, object)
    {
        if (((YYFightSprite *)object)->getTargetSprite())
        {
            if (((YYFightSprite *)object)->getTargetSprite()->getSpriteID() == fightSprite->getSpriteID())
            {
                ((YYFightSprite *)object)->setTargetSprite(NULL);
            }
        }
    }
}

#pragma mark通关条件判断及传送门、boss出现条件判断(每个敌人死亡时调用) 

void YYSpriteLayer::processEnemyDeath(YYFightSprite * fightSprite)
{
    ((YYScene *)getParent())->setKillCount(((YYScene *)getParent())->getKillCount() + 1);// 击杀数增加 
    //boss死亡后取消血条显示 
    if(((YYScene *)getParent())->getControlUILayer()->getBoss())
    {
        if (fightSprite->getSpriteID() == ((YYScene *)getParent())->getControlUILayer()->getBoss()->getSpriteID())
        {
            ((YYScene *)getParent())->getControlUILayer()->setBoss(NULL);
            for (int i = 0; i < m_pMoveableEnemyArray->count(); i++)
            {
                YYMonster * monster = dynamic_cast<YYMonster *>(m_pMoveableEnemyArray->objectAtIndex(i));
                if (monster->getSpriteType() == SPRITE_TYPE_MONSTER)
                {
                    monster->setAction(ACTION_DIE);
                    monster->setActionStatus(kActionDie);
                    monster->setCurrentHP(0);
                }
            }
        }
    }
    
    // 战斗结果 
    if (m_pMoveableEnemyArray->count() == 0)
    {// 杀死全部敌人 
        if(m_pDoorArray->count() == 0)
        {// 无传送门的场景
            DungeonType type =  Controller::getInstance()->getDungeonType(); // 获取副本类型
            switch (type)
            {
                case kDungeonNormal: // 正常副本
                {
                    if (((YYScene *)getParent())->getStageDataArray() && ((YYScene *)getParent())->getStageDataArray()->count() > 0)
                    {//经由关卡选择进入副本,延时显示战斗结果
                        ((YYScene *)getParent())->setFightResultDelayCounter(0);
                        processClearStageTask();//处理通关任务
                        return;
                    }
                }
                    break;
                case kDungeonDungeon: // 活动_地下城 最后结束关卡
                {
                    CCString* msg = CCString::createWithFormat(ASK_DUNGEON_RESULT,1);
                    Controller::getInstance()->sendMessage(msg->getCString(),true);
                    return;
                }
                    break;
                case kDungeonProtoss:// 守卫星灵
                {
                    CCString* msg = CCString::createWithFormat(ASK_PROTECT_PROTOSS_RESULT,1);
                    Controller::getInstance()->sendMessage(msg->getCString(),true);
                    return;
                }
                    break;
                case kDungeonBoss:// BOSS 副本
                {
                    CCString* msg = CCString::createWithFormat(ASK_BOSS_DUNGEON_RESULT,1);
                    Controller::getInstance()->sendMessage(msg->getCString(),true);
                    return;
                }
                    break;
                default:
                    break;
            }
        }
        else if (Controller::getInstance()->getSceneType() == DUNGEONS
                 || Controller::getInstance()->getSceneType() == WAR_ZONE)
        {
            ((YYScene *)getParent())->getControlUILayer()->showTheWay();
            DungeonType type =  Controller::getInstance()->getDungeonType(); // 获取副本类型
            switch (type)
            {
                case kDungeonDungeon: // 地下城
                {
                    CCString* msg = CCString::createWithFormat(ASK_DUNGEON_RESULT,1);
                    Controller::getInstance()->sendMessage(msg->getCString(),false);
                    int spriteIndex = fightSprite->getSpriteData()->getSpriteIndex();
                    openPortal(spriteIndex);// 判断传送门是否开启
                    return;
                }
                    break;
                case kDungeonProtoss: // 守卫星灵
                {
                    CCString* msg = CCString::createWithFormat(ASK_PROTECT_PROTOSS_RESULT,1);
                    Controller::getInstance()->sendMessage(msg->getCString(),false);
                    int spriteIndex = fightSprite->getSpriteData()->getSpriteIndex();
                    openPortal(spriteIndex);// 判断传送门是否开启
                    return;
                }
                    break;
                case kDungeonBoss: // boss副本
                {
//                    CCString* msg = CCString::createWithFormat(ASK_BOSS_DUNGEON_RESULT,1);
//                    Controller::getInstance()->sendMessage(msg->getCString(),false);
//                    int spriteIndex = fightSprite->getSpriteData()->getSpriteIndex();
//                    openPortal(spriteIndex);// 判断传送门是否开启
//                    return;
                }
                    break;
                default:
                    break;
            }
        }
    }
    
    // 敌人出现条件判断(// 杀死指定敌人达到一定数量) 
    int spriteIndex = fightSprite->getSpriteData()->getSpriteIndex();
    for (int i = 0; i < m_pEnemyArray->count(); i++)
    {
        YYMonster * soldier = (YYMonster *)m_pEnemyArray->objectAtIndex(i);
        if (fightSprite->getSpriteID() == soldier->getSpriteID() || soldier->isVisible())
        {
            continue;
        }
        if (soldier->getVisibleConditionArray())
        {
            for (int j = 0; j < soldier->getVisibleConditionArray()->count();)
            {
                CCString * string = (CCString *)soldier->getVisibleConditionArray()->objectAtIndex(j);
                CCArray * array = split_string(string->getCString(), ',');
                int soldierIndex = ((CCString *)array->objectAtIndex(0))->intValue();
                if (spriteIndex == soldierIndex)
                {
                    int num = ((CCString *)array->objectAtIndex(1))->intValue() - 1;
                    if (num == 0)
                    {
                        soldier->getVisibleConditionArray()->removeObjectAtIndex(j);
                        if (soldier->getVisibleConditionArray()->count() == 0)
                        {
                            soldier->setVisible(true);
                            soldier->getNameLabel()->setVisible(true);
                        }
                    }
                    else
                    {// 更新显示条件中的需求数量 
                        CCString * newString = CCString::createWithFormat("%d,%d",soldierIndex,num);
                        soldier->getVisibleConditionArray()->replaceObjectAtIndex(j, newString);
                        j++;
                    }
                }
                else
                {
                    j++;
                }
            }
        }
    }
    openPortal(spriteIndex);// 判断传送门是否开启
    processTask(fightSprite);// 处理打怪及收集任务
}

/**
 * 判断是否开启传送门
 */
void YYSpriteLayer::openPortal(int fightSpriteIndex)
{
    // 传送门显示条件判断 
    for (int i = 0; i < m_pDoorArray->count(); i++)
    {
        YYSprite * door = (YYSprite *)m_pDoorArray->objectAtIndex(i);
        if (door->isVisible())
        {
            continue;
        }
        if (door->getTag() == TAG_CLEAN_ALL_ENEMY)
        {// 需清除所有敌人开启 
            if (m_pMoveableEnemyArray->count() == 0)
            {
                door->setVisible(true);
                door->getNameLabel()->setVisible(true);
            }
        }
        else if(door->getTag()== TAG_KILL_APPOINTED_ENEMY)
        {// 杀死指定敌人达到一定数量 
            for (int j = 0; j < door->getVisibleConditionArray()->count();)
            {
                CCString * string = (CCString *)door->getVisibleConditionArray()->objectAtIndex(j);
                CCArray * array = split_string(string->getCString(), ',');
                int soldierIndex = ((CCString *)array->objectAtIndex(0))->intValue();
                if (fightSpriteIndex == soldierIndex)
                {
                    int num = ((CCString *)array->objectAtIndex(1))->intValue() - 1;
                    if (num == 0)
                    {
                        door->getVisibleConditionArray()->removeObjectAtIndex(j);
                        if (door->getVisibleConditionArray()->count() == 0)
                        {
                            door->setVisible(true);
                            door->getNameLabel()->setVisible(true);
                        }
                    }
                    else
                    {// 更新显示条件中的需求数量 
                        CCString * newString = CCString::createWithFormat("%d,%d",soldierIndex,num);
                        door->getVisibleConditionArray()->replaceObjectAtIndex(j, newString);
                        j++;
                    }
                }
            }
        }
        else if (door->getTag()== TAG_OPEN_ACCEPT_TASK)
        { // 指定任务寻路开启
            if (door->getVisibleConditionArray() && door->getVisibleConditionArray()->count() > 0)
            {
                int taskID = ((CCString *)door->getVisibleConditionArray()->objectAtIndex(0))->intValue();
                if (door->getVisibleConditionArray()->count() > 1)
                {
                    int taskStatus = ((CCString *)door->getVisibleConditionArray()->objectAtIndex(1))->intValue();
                    if (taskID == ((YYScene*)getParent())->getPopUpUI()->getAcceptedTaskIndex()
                        && taskStatus == fightSpriteIndex)
                    {
                        door->setVisible(true);
                        door->getNameLabel()->setVisible(true);
                    }
                }
            }
        }
    }
}

/**
 * 当前关卡是否为任务指定关卡
 */
bool YYSpriteLayer::isTaskStage(TaskTarget * taskTarget,int stageIndex)
{
    CCArray* array2 = split_string(((CCString*)((YYScene *)getParent())->getStageDataArray()->objectAtIndex(stageIndex))->getCString(), ';');
    CCString *stageName = static_cast<CCString *>(array2->objectAtIndex(0));
    if(stageName->isEqual(CCString::create(taskTarget->getStageName())))
    {
        return  true;
    }
    return false;
}
/**
 * 当前关卡是否为任务目标 （所在副本、关卡及难度一致）
 */
bool YYSpriteLayer::isTaskTarget(TaskTarget * taskTarget)
{
    if(Controller::getInstance()->getSceneType() == WAR_ZONE && taskTarget->getDungeonsName().compare("0") == 0)
    {// 新手阶段杀怪任务不指定副本及关卡
        return  true;
    }
    int level = taskTarget->getStageLevel();
    CCArray* array1 = split_string(((CCString*)((YYScene *)getParent())->getDungeonsDataArray()->objectAtIndex(((YYScene *)getParent())->getDungeonsIndex()))->getCString(), ';');
    CCString *dungeonsName = static_cast<CCString *>(array1->objectAtIndex(0));
    CCArray* array2 = split_string(((CCString*)((YYScene *)getParent())->getStageDataArray()->objectAtIndex(((YYScene *)getParent())->getChoosedStageIndex()))->getCString(), ';');
    CCString *stageName = static_cast<CCString *>(array2->objectAtIndex(0));
    if(dungeonsName->isEqual(CCString::create(taskTarget->getDungeonsName())))
    {
        if(stageName->isEqual(CCString::create(taskTarget->getStageName())))
        {
            if (level == ((YYScene *)getParent())->getChoosedStageDifficultIndex())
            {
                return  true;
            }
        }
    }
    return false;
}

void YYSpriteLayer::processTask(YYFightSprite * fightSprite)
{
    CCDictionary* taskDictionary = GlobalData::getInstance()->getTaskDataDict();
    CCArray * array = taskDictionary->allKeys();
    if (!array)
    {
        return;
    }
    for (int i = 0; i < array->count(); i++)
    {
        int key = ((CCInteger *)array->objectAtIndex(i))->getValue();
        TaskData * task = (TaskData *)taskDictionary->objectForKey(key);
        if(task->getTargetType() == kTaskKillMonster || task->getTargetType() == kTaskCollect)
        {
            for (int j = 0; j < task->getTargetArray()->count(); ++j)
            {
                TaskTarget * taskTarget = (TaskTarget *)task->getTargetArray()->objectAtIndex(j);
                if(isTaskTarget(taskTarget))
                {
                    if(fightSprite->getSpriteData()->getSpriteIndex() == CCString::create(taskTarget->getTargetID())->intValue())
                    {
                        if (task->getTargetType() == kTaskKillMonster)
                        {
                            if(taskTarget->getTargetMonsterCount() < taskTarget->getTargetMonsterAmount())
                            {
                                taskTarget->setTargetMonsterCount(taskTarget->getTargetMonsterCount() + 1);
                                if (taskTarget->getTargetMonsterCount() == taskTarget->getTargetMonsterAmount())
                                {
                                    const char * message = CCString::createWithFormat(SEND_TASK_COMPLETE,task->getTaskIndex())->getCString();
                                    Controller::getInstance()->sendMessage(message,true);
                                }
                                else
                                {
                                    const char * message = CCString::createWithFormat(SEND_TASK_RESPONSE,task->getTaskIndex(),taskTarget->getTargetMonsterCount())->getCString();
                                    Controller::getInstance()->sendMessage(message,true);
                                }
                            }
                        }
                        else
                        {
                            if(RANDOM_INT(1, 100) <= (taskTarget->getDropOdds()))
                            {
                                if(taskTarget->getTargetItemCount() < taskTarget->getTargetItemAmount())
                                {
                                    taskTarget->setTargetItemCount(taskTarget->getTargetItemCount() + 1);
                                    if (taskTarget->getTargetItemCount() == taskTarget->getTargetItemAmount())
                                    {
                                        const char * message = CCString::createWithFormat(SEND_TASK_COMPLETE,task->getTaskIndex())->getCString();
                                        Controller::getInstance()->sendMessage(message,true);
                                    }
                                    else
                                    {
                                        const char * message = CCString::createWithFormat(SEND_TASK_RESPONSE,task->getTaskIndex(),taskTarget->getTargetItemCount() < taskTarget->getTargetItemAmount())->getCString();
                                        Controller::getInstance()->sendMessage(message,true);
                                    }
                                }
                            }
                        }
                        if(((YYScene*)getParent())->getControlUILayer()->getIsShowTaskInfo())
                        {
                            ((YYScene*)getParent())->getControlUILayer()->showTaskInfo();
                        }
                    }
                }
            }
        }
    }
}

void YYSpriteLayer::processClearStageTask()
{
    CCArray * array = GlobalData::getInstance()->getTaskDataArray();
    for (int i = 0; i < array->count(); ++i) {
        TaskData * task = (TaskData *)array->objectAtIndex(i);
        if(task->getTargetType() == kTaskPassStage)
        {
            CCArray * arr = task->getTargetArray();
            for (int j = 0; j < arr->count(); j++)
            {
                TaskTarget * taskTarget = static_cast<TaskTarget *>(arr->objectAtIndex(j));
                if (isTaskTarget(taskTarget))
                {
                    if (taskTarget->getClearStageCount() < taskTarget->getClearStageAmount())
                    {
                        taskTarget->setClearStageCount(taskTarget->getClearStageCount()+1);
                        if (taskTarget->getClearStageCount() == taskTarget->getClearStageAmount())
                        {
                            const char * message = CCString::createWithFormat(SEND_TASK_COMPLETE,task->getTaskIndex())->getCString();
                            Controller::getInstance()->sendMessage(message,true);
                        }
                        else
                        {
                            const char * message = CCString::createWithFormat(SEND_TASK_RESPONSE,task->getTaskIndex(),taskTarget->getTargetItemCount() < taskTarget->getTargetItemAmount())->getCString();
                            Controller::getInstance()->sendMessage(message,true);
                        }
                    }
                }
            }
        }
    }
}

#pragma mark 召唤侍从

void YYSpriteLayer::summon(int spriteIndex, cocos2d::CCPoint position,int duration, int ownerID)
{
    SpriteData * spriteData = (SpriteData *)GlobalData::getInstance()->getSpriteDataDict()->objectForKey(spriteIndex);// 精灵基本数据 
    YYMonster * footman = YYMonster::create();
    footman->initWithActionSet(spriteData->getResourceArray(), kDirTypeSprite);
    footman->setAction(ACTION_STAND0, false);
    footman->setPosition(position);
    footman->setSpriteType(spriteData->getType());
    
    footman->setSpriteName(spriteData->getSpriteName().c_str());
    addChild(footman);
    footman->setOwnerID(ownerID);
    if (ownerID == m_pRoleByControlled->getSpriteID())
    {
        footman->setFriendly(true);
        m_pFriendlyRoleArray->addObject(footman);
    }
    else
    {
        footman->setFriendly(false);
        m_pMoveableEnemyArray->addObject(footman);
        m_pEnemyArray->addObject(footman);
    }
    footman->setSummoned(true);
    footman->setSummonedDuration(duration);
    addElementToSpriteDictionary(footman);// 设置精灵ID并将精灵添加至精灵容器 
    
    int attributeIndex = spriteData->getAttributeIndex();
    SpriteAttribute * spriteAttribute = (SpriteAttribute*)GlobalData::getInstance()->getSpriteAttributeDict()->objectForKey(attributeIndex);
    footman->initFightData(spriteData, spriteAttribute);
    YYFightSprite *owner = (YYFightSprite *)m_pSpriteDictionary->objectForKey(ownerID);
    footman->setNameLabel(CCLabelTTF::create(footman->getSpriteName()->getCString(), DEFAULT_FONT_NAME, 25));
    footman->getNameLabel()->setPosition(footman->getPosition() + ccp(0,spriteData->getNameOffsetHeight() * spriteData->getScaleFactor()));
    footman->getNameLabel()->setAnchorPoint(ccp(0.5,0));
    addChild(footman->getNameLabel(),1);
    if (spriteData->getType() == SPRITE_TYPE_MONSTER || spriteData->getType() == SPRITE_TYPE_PET)
    {
        ((YYMonster *)footman)->initBloodBar(ccp(0,spriteData->getNameOffsetHeight() - 5));//初始化怪物血条
    }
    owner->getFootmanIDArray()->addObject(CCInteger::create(footman->getSpriteID()));// 添加至主人的仆从容器中
}
