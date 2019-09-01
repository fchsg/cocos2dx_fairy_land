//
//  YYGlobalData.cpp
//  Fairyland
//
//  Created by fuchenhao on 14-2-13.
//
//

#include "YYGlobalData.h"

#include "../common/Common.h"
#include "../common/FileReadManager.h"
#include "../animation/YYRect.h"
#include "../animation/YYActionSet.h"
USING_NS_CC;

GlobalData::GlobalData()
{
    init();
}

GlobalData::~GlobalData()
{
    CC_SAFE_DELETE(m_pActionSetDict);
    CC_SAFE_DELETE(m_pTalentDataArrayDict);
    CC_SAFE_DELETE(m_pSkillActionDataDict);
    CC_SAFE_DELETE(m_pSkillEffectDataDict);
    CC_SAFE_DELETE(m_pSpriteDataDict);
    CC_SAFE_DELETE(m_pSpriteAttributeDict);
    CC_SAFE_DELETE(m_pUsableSkillDataArray);
    CC_SAFE_DELETE(m_pUsableGestureSkillDataArray);
    CC_SAFE_DELETE(m_pPlayerAttributeDict);
    CC_SAFE_DELETE(m_pPlayerAttackIndexDict);
    CC_SAFE_DELETE(m_pPlayerShortcutSkillIndexDict);
    CC_SAFE_DELETE(m_pPlayerGestureSkillIndexDict);
    CC_SAFE_DELETE(m_pPlayerSkillDataArray);
    CC_SAFE_DELETE(m_pPropArray);
    CC_SAFE_DELETE(m_pEquipOnTheBodyArray);
    CC_SAFE_DELETE(m_pEquipArray);
    CC_SAFE_DELETE(m_pTaskDataDict);
    CC_SAFE_DELETE(m_pTaskDataArray);
    CC_SAFE_DELETE(m_pTreasureItemArray);
    CC_SAFE_DELETE(m_pProtossDataArray);
    CC_SAFE_DELETE(m_pConstellationDataArray);
    CC_SAFE_DELETE(m_pScrapDataArray);
    CC_SAFE_DELETE(m_pAchievementClassDataArray);
    CC_SAFE_DELETE(m_pAchievementGroupDataArray);
    CC_SAFE_DELETE(m_pAchievementItemDataArray);
    CC_SAFE_DELETE(m_pSignUpDataArray);
    CC_SAFE_DELETE(m_pSignUpStaticAwardArray);
    CC_SAFE_DELETE(m_pSigUpBoxAwardDataArray);
    CC_SAFE_DELETE(m_pLoginAwardDict);
    CC_SAFE_DELETE(m_pSetLoadingMessageArray);
    CC_SAFE_RELEASE_NULL(m_pNickNameArray);
    for (int i = 0; i < 3; i++)
    {
        CC_SAFE_DELETE(m_pGestureSkillDataArray[i]);
    }
    CC_SAFE_DELETE(m_pActivityTaskArray);
    CC_SAFE_DELETE(m_pActivityWishTreeDict);
    if (m_pAchievementAllDataDict)
    {
        m_pAchievementAllDataDict->removeAllObjects();
    }
    CC_SAFE_DELETE(m_pAchievementAllDataDict);
    if (m_pTalentDataArrayDict)
    {
        m_pTalentDataArrayDict->removeAllObjects();
    }
    CC_SAFE_DELETE(m_pTalentDataArrayDict);
    if (m_pSkillDataDict)
    {
        m_pSkillDataDict->removeAllObjects();
    }
    CC_SAFE_DELETE(m_pSkillDataDict);
    if (m_pVipDataDict)
    {
        m_pVipDataDict->removeAllObjects();
    }
    CC_SAFE_DELETE(m_pVipDataDict);
    
}

void GlobalData::init()
{
    m_pActionSetDict = new CCDictionary();
    m_pSkillDataDict = new CCDictionary();
    m_pPlayerSkillDataArray = new CCArray(20);
    m_pTalentDataArrayDict = new CCDictionary();
    m_pTalentDataDict = new CCDictionary();
    m_pSkillActionDataDict = new CCDictionary();
    m_pSkillEffectDataDict = new CCDictionary();
    m_pSpriteDataDict = new CCDictionary();
    m_pSpriteAttributeDict = new CCDictionary();
    m_pUsableSkillDataArray = new CCArray(5);
    m_pUsableGestureSkillDataArray = new CCArray(5);
    for (int i = 0; i < 3; i++)
    {
        m_pGestureSkillDataArray[i] = new CCArray(3);
    }
    m_pPlayerAttackIndexDict = new CCDictionary();
    m_pPlayerShortcutSkillIndexDict = new CCDictionary();
    m_pPlayerGestureSkillIndexDict = new CCDictionary();
    m_pPlayerAttributeDict= new CCDictionary();
    m_pEquipmentDict = new CCDictionary();
    m_pPropertyDict = new CCDictionary();
    m_pEquipOnTheBodyArray = new CCArray();
    m_pEquipArray = new CCArray();
    m_pPropArray = new CCArray();
    m_pTaskDataDict = new CCDictionary();
    m_pTaskDataArray = new CCArray(10);
    m_pTreasureItemArray = new CCArray(9);
    m_pProtossDataArray = new CCArray();
    m_pConstellationDataArray = new CCArray();
    m_pScrapDataArray = new CCArray();
    m_pAchievementClassDataArray = new CCArray();
    m_pAchievementGroupDataArray = new CCArray();
    m_pAchievementItemDataArray = new CCArray();
    m_pSetLoadingMessageArray = new CCArray(20);
    m_pSignUpDataArray = new CCArray();
    m_pSignUpStaticAwardArray = new CCArray();
    m_pSigUpBoxAwardDataArray = new CCArray();
    m_pLoginAwardDict = new CCDictionary();
    m_pActivityTaskArray = new CCArray();
    m_pActivityWishTreeDict = new CCDictionary();
    m_pAchievementAllDataDict = new CCDictionary();
    m_pVipDataDict = new CCDictionary();
}

static GlobalData *s_GlobalData = NULL;

GlobalData *GlobalData::getInstance()
{
    if (!s_GlobalData)
    {
        s_GlobalData = new GlobalData();
    }
    return s_GlobalData;
}

void GlobalData::readData()
{
    readSkillData();
    readSkillActionData();
    readSkillEffectData();
    readPlayerData();
    readNFSData();
    readFightSpriteData();
    readSpriteAttribute();
    readNickNameData();
    readProtossData();
    readConstellationData();
    readScrapData();
    readAchievementClassData();
    readAchievementGroupData();
    readAchievementItemData();
    setAchievementAllData();
    readSignUpData();
    readSignUpBoxAwardData();
    readLoginAwardData();
    readActivityData();
    readTalentData();
    readVipData();
}

ActionSet * GlobalData::getActionSet(const std::string& aniFileName)
{
    ActionSet *actionSet = (ActionSet *)m_pActionSetDict->objectForKey(aniFileName);
    if (actionSet)
    {
        return actionSet;
    }
    actionSet = ActionSet::createActionSetWithFile(aniFileName);
    m_pActionSetDict->setObject(actionSet,aniFileName);
    return actionSet;
}

#pragma mark 读取二进制文件数据

void GlobalData::readSkillData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("skilldata1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        SkillData * skillData = SkillData::create();
        skillData->setIndex(fileReader->readInt());// 编号
        skillData->setLevel(fileReader->readInt());// 等级
        CCArray* arr = static_cast<CCArray *>(m_pSkillDataDict->objectForKey(skillData->getIndex()));
        if (!arr)
        {
            arr = CCArray::create();
            m_pSkillDataDict->setObject(arr, skillData->getIndex());
        }
        arr->addObject(skillData);
        
        // 类型 1,普通攻击 2,技能攻击 3,怒技攻击 4,怪物技能
        int skillType =  getFirstNBitNumber(skillData->getIndex(),1);
        skillData->setSkillType(skillType);
        
        skillData->setSkillName(fileReader->readUTF(true));// 名称
        skillData->setCoolingDuration(fileReader->readInt());// 冷却时间
        // 对应动作的编号
        CCArray *array = split_string(fileReader->readUTF(true), ',');
        CCAssert(array, "failed to setActionIndex");
        skillData->setActionIndexArray(array);
        
        skillData->setAddedDamage(fileReader->readInt());// 附加伤害
        skillData->setAPCoefficient(CCString::create(fileReader->readUTF(true))->floatValue());// 物理伤害系数
        skillData->setSAPCoefficient(CCString::create(fileReader->readUTF(true))->floatValue());// 魔法伤害系数
        skillData->setSkillDamageCoefficient(CCString::create(fileReader->readUTF(true))->floatValue());// 绝技伤害系数
        skillData->setBreaked(fileReader->readInt());// 是否能被打断
        
        // 是否被击退
        array = split_string(fileReader->readUTF(true), ',');
        CCAssert(array, "failed to setBeatedBack");
        skillData->setBeatedBack(static_cast<CCString *>(array->objectAtIndex(0))->intValue());
        if (skillData->isBeatedBack())
        {
            skillData->setBeatedBackDistance(static_cast<CCString *>(array->objectAtIndex(1))->intValue());// 击退距离
        }
        
        //  不良状态 0无效果 1眩晕 2冻结 3沉默 4恐惧 5减速
        array = split_string(fileReader->readUTF(true), ',');
        CCAssert(array, "failed to readDebuff parameter");
        skillData->setDebuffType(static_cast<CCString *>(array->objectAtIndex(0))->intValue());
        if (skillData->getDebuffType())
        {
            skillData->setDebuffDuration(static_cast<CCString *>(array->objectAtIndex(1))->intValue());
            if (skillData->getDebuffType() == kDebuffTieDown)// 减速
            {
                skillData->setDebuffAddedValue(static_cast<CCString *>(array->objectAtIndex(2))->intValue());
            }
        }
        
        //  dot类型、持续时间、秒伤
        array = split_string(fileReader->readUTF(true), ';');
        skillData->setDOTType(static_cast<CCString *>(array->objectAtIndex(0))->intValue());
        if (skillData->getDOTType())
        {
            skillData->setDOTDuration(static_cast<CCString *>(array->objectAtIndex(1))->intValue());
            skillData->setDOTDamagePoint(static_cast<CCString *>(array->objectAtIndex(2))->intValue());
        }
        
        skillData->setTargetReducedTenacity(fileReader->readInt());//目标受击后,韧性的减少值
        skillData->setSpecialType(fileReader->readInt());//  特殊类型 1弹射 4召唤
        
        // 针对不同类型的技能的特殊处理
        array = split_string(fileReader->readUTF(true), ',');
        if (skillData->getSpecialType() == 1)
        {
            skillData->setCatapultAmount(((CCString *)array->objectAtIndex(0))->intValue());// 弹射总次数
            skillData->setMaxCatapultDistance(((CCString *)array->objectAtIndex(1))->intValue());// 最大弹射距离
        }
        else if(skillData->getSpecialType() == 4)
        {
            skillData->setSummonedSpriteIndex(((CCString *)array->objectAtIndex(0))->intValue());// 被召唤的精灵编号
            skillData->setSummonedSpriteNum(((CCString *)array->objectAtIndex(1))->intValue());// 被召唤的精灵数量
            skillData->setSummonedSpriteDuration(((CCString *)array->objectAtIndex(2))->intValue() * FRAME_PER_SECOND);// 召唤物持续时间
        }
        else if(skillData->getSpecialType() == 5)
        {
            skillData->setTrapNum(((CCString *)array->objectAtIndex(0))->intValue());
        }
        skillData->setHurtNumber(fileReader->readInt()); //伤害个数
        skillData->setSkillIcon(fileReader->readUTF(true)); // 技能图标
        skillData->setSkillUnableIcon(fileReader->readUTF(true)); // 技能无法释放时的图标
        skillData->setRoleLevel(fileReader->readInt()); // 升级所需角色等级
        skillData->setJew(fileReader->readInt()); // 升级宝石消耗
        skillData->setMoney(fileReader->readInt()); // 升级金钱消耗
        skillData->setCurrentLevelDescription(fileReader->readUTF(true)); // 当前技能描述
        skillData->setNextLevelDescription(fileReader->readUTF(true)); // 技能下一个等级描述
        skillData->setConsumePoint(fileReader->readInt()); // 消耗值
        skillData->setGestureType(fileReader->readInt()); // 手势类型 0线 1V 2圆
        skillData->setItemID(fileReader->readInt()); // 升级所需道具ID
        skillData->setItemIcon(fileReader->readUTF(true)); // 升级所需道具icon
        skillData->setItemNumber(fileReader->readInt()); // 升级所需道具数量
        skillData->setResetSkillTalentJew(fileReader->readInt());// 重置技能天赋所需宝石数量
    }
    CC_SAFE_DELETE(fileReader);
}

/**
 * 读取技能对应的动作数据
 */
void GlobalData::readSkillActionData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("skilldata2", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        SkillActionData * skillActionData = SkillActionData::create();
        skillActionData->setIndex(fileReader->readInt());// 技能动作编号
        // 可调用该技能的精灵编号
        CCArray * array = split_string(fileReader->readUTF(true), ',');
        skillActionData->setSpriteIndexArray(array);
        
        //  需调用该职业对应动作文件里的动作的序号
        array = split_string(fileReader->readUTF(true), ',');
        skillActionData->setActionIndexArray(array);
        
        skillActionData->setMovable(fileReader->readInt() == 1);//是否可被方向键控制进行位移 0不可以 1可以
        //  按设置的持续时间循环播放 0不循环
        int duration = fileReader->readInt();
        skillActionData->setDuration(duration * FRAME_PER_SECOND);
        if (duration > 0)
        {//音效随动作循环
            skillActionData->setSoundEffectLoop(true);
        }
        
        //  调用该动作时的位移相关参数
        array = split_string(fileReader->readUTF(true), ';');
        for (int m = 0; m < array->count(); m++)
        {
            CCArray * array1 = split_string(((CCString *)array->objectAtIndex(m))->getCString(), ',');
            if (array->count() == 1 && array1->count() == 1)
            {// 每帧具有相同的位移
                skillActionData->setOffsetXPerFrame(((CCString *)array1->objectAtIndex(0))->intValue());
            }
            else
            {// 在指定的帧偏移指定的像素数
                skillActionData->getOffsetFrameIndexArray()->addObject((CCString *)array1->objectAtIndex(0));
                skillActionData->getOffsetXCurrentFrameArray()->addObject((CCString *)array1->objectAtIndex(1));
            }
        }
        
        skillActionData->setSoundEffectPath(fileReader->readUTF(true));//  音效路径
        skillActionData->setSoundFrameIndex(fileReader->readInt());//  设置在动作的哪一帧开始播放音效
        skillActionData->setVibrateFrameIndex(fileReader->readInt());//  震屏对应的起始帧
        skillActionData->setVibrateDuration(fileReader->readInt() * FRAME_PER_SECOND);//  震屏持续时间
        //  需调用的特效数据
        array = split_string(fileReader->readUTF(true), ',');
        skillActionData->setEffectDataArray(array);
        
        //  在人物动作的第几帧启用特效
        array = split_string(fileReader->readUTF(true), ',');
        skillActionData->setEffectStartFrameArray(array);
        if (skillActionData->getIndex() == 154)
        {
            int  a = 0;
            a = a +1;
            a++;
        }
        getSkillActionDataDict()->setObject(skillActionData, skillActionData->getIndex());
    }
    delete fileReader;
}

/**
 * 读取技能特效数据
 */
void GlobalData::readSkillEffectData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("skilldata3", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        SkillEffectData * skillEffectData = SkillEffectData::create();
        skillEffectData->setIndex(fileReader->readInt());//  特效编号
        skillEffectData->setResource(fileReader->readUTF(true));//  资源路径
        skillEffectData->setLayerIndex(fileReader->readInt());//  层级
        skillEffectData->setDuration(fileReader->readInt() * FRAME_PER_SECOND);//  持续时间
        //  特效显示模式 0无特殊处理 1显示在屏幕中心 2跟随主人 3随机位置
        skillEffectData->setDisplayMode(fileReader->readInt());
        
        //  攻击范围
        CCArray * array = split_string(fileReader->readUTF(true), ',');
        if (array->count() == 4)
        {
            int x = ((CCString *)array->objectAtIndex(0))->intValue();
            int y = ((CCString *)array->objectAtIndex(1))->intValue();
            int w = ((CCString *)array->objectAtIndex(2))->intValue();
            int h = ((CCString *)array->objectAtIndex(3))->intValue();
            CCRect rect = CCRectMake(x, y, w, h);
            skillEffectData->setCollisionRect(rect);
        }
        
        // 特效类型及相关细节参数
        skillEffectData->setType(fileReader->readInt());
        if(skillEffectData->getType() == 0)// 普通技能特效
        {
            array = split_string(fileReader->readUTF(true), ',');
            skillEffectData->setDamageAmount(((CCString *)array->objectAtIndex(0))->intValue());//  伤害次数
            skillEffectData->setDamageFrameIndex(((CCString *)array->objectAtIndex(1))->intValue());//  起始伤害对应帧
            skillEffectData->setDamageInterval(((CCString *)array->objectAtIndex(2))->intValue());//  伤害间隔
        }
        else if (skillEffectData->getType() == 1)// 子弹类特效
        {
            array = split_string(fileReader->readUTF(true), ',');
            skillEffectData->setOffsetXPerFrame(((CCString *)array->objectAtIndex(0))->intValue());
            skillEffectData->setAngleOfDeflection(((CCString *)array->objectAtIndex(1))->floatValue());
            skillEffectData->setBulletType(((CCString *)array->objectAtIndex(2))->intValue());//  0命中后删除特效 1不删除 2弹射 3闪电链
        }
        else if(skillEffectData->getType() == 2)// 召唤类特效
        {
            array = split_string(fileReader->readUTF(true), ';');
            skillEffectData->setSummonedPositionArray(array);//召唤物坐标
        }
        
        skillEffectData->setHittedEffectResource(fileReader->readUTF(true));//  命中后调用的打击特效资源名称
        skillEffectData->setFollowUpEffectIndex(fileReader->readInt());// 后续特效编号
        getSkillEffectDataDict()->setObject(skillEffectData, skillEffectData->getIndex());
    }
    delete fileReader;
}

/**
 * 读取天赋数据
 */
void GlobalData::readTalentData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("talent1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        TalentData * talentData = TalentData::create();
        talentData->setIndex(fileReader->readInt());// 天赋id
        CCArray* arr = static_cast<CCArray *>(m_pTalentDataDict->objectForKey(talentData->getIndex()));
        if (!arr)
        {
            arr = CCArray::create();
            m_pTalentDataDict->setObject(arr, talentData->getIndex());
        }
        arr->addObject(talentData);
        talentData->setName(fileReader->readUTF(true)); // 天赋名称
        talentData->setIconName(fileReader->readUTF(true)); // 天赋图标
        talentData->setSkillIndex(fileReader->readInt());// 技能序号
        talentData->setLayerIndex(fileReader->readInt());// 层级
        talentData->setPositionIndex(fileReader->readInt());// 位置
        talentData->setMaxPoint(fileReader->readInt());// 最大可投入天赋点
        talentData->setPreposeTalentIndex(fileReader->readInt()); // 前置天赋编号
        talentData->setPreposeTalentPoints(fileReader->readInt());// 所需前置天赋点数
        talentData->setAmountTalentPoints(fileReader->readInt()); // 需求总点数
        int nType = fileReader->readInt();
        talentData->setType(nType);// 天赋类型
        std::string valueStr = fileReader->readUTF(true);// 产生效果数据
        talentData->setSkillLevel(fileReader->readInt());// 升级天赋所需技能等级
        std::string  description = fileReader->readUTF(true);// 天赋描述
        int debuffType = fileReader->readInt(); // debuff 类型
        int dotType = fileReader->readInt();// dot 类型
        if (valueStr!= "")
        {
            CCArray *tempArray = split_string(valueStr.c_str(),',');
            CCString *str = NULL;
            for (int j = 0; j < tempArray->count(); j++)
            {
                if (tempArray->count() == 1)
                {
                        str = CCString::createWithFormat(description.c_str(),static_cast<CCString *>(tempArray->objectAtIndex(0))->getCString());
                }
                else if (tempArray->count() == 2)
                {
                    if (nType == 4 || nType == 8) // 天赋类型为 4 替换技能动作 8 延长击退距离 特殊处理
                    {
                        str = CCString::createWithFormat(description.c_str(),static_cast<CCString *>(tempArray->objectAtIndex(1))->getCString());
                    }
                    else
                    {
                       str = CCString::createWithFormat(description.c_str(),static_cast<CCString *>(tempArray->objectAtIndex(0))->getCString(),static_cast<CCString *>(tempArray->objectAtIndex(1))->getCString());
                    }

                }
                else if(tempArray->count() == 3)
                {
                    str = CCString::createWithFormat(description.c_str(),static_cast<CCString *>(tempArray->objectAtIndex(0))->getCString(),static_cast<CCString *>(tempArray->objectAtIndex(1))->getCString(),static_cast<CCString *>(tempArray->objectAtIndex(2))->getCString());
                }
                else if(tempArray->count() == 4)
                {
                    str = CCString::createWithFormat(description.c_str(),static_cast<CCString *>(tempArray->objectAtIndex(0))->getCString(),static_cast<CCString *>(tempArray->objectAtIndex(1))->getCString(),static_cast<CCString *>(tempArray->objectAtIndex(2))->getCString(),static_cast<CCString *>(tempArray->objectAtIndex(3))->getCString());
                }
            }
            talentData->setDescription(str->getCString());//天赋描述数组
 
            switch (nType)
            {
                case kTypeReduceCD:
                    talentData->setCDAddedValue(valueStr);
                    break;
                case kTypeReduceMPConsume:
                    talentData->setConsumeMPAddedValue(valueStr);
                    break;
                case kTypeAddDamage:
                    talentData->setDamageAddedValue(valueStr);
                    break;
                case kTypeSetBreaked:
                    talentData->setBreaked(true);
                    break;
                case kTypeChangeActionIndex:
                    talentData->setActionIndexGroup(valueStr);
                    break;
                case kTypeAddHurtedNumber:
                    talentData->setHurtedNumberAddedValue(valueStr);
                    break;
                case kTypeDebuff:
                {
                    talentData->setDebuffType(debuffType);
                    CCArray *valueArray2 = split_string(valueStr.c_str(),',');
                    CCString *value2 = static_cast<CCString *>(valueArray2->objectAtIndex(0));
                    talentData->setDebuffDuration(atoi(value2->getCString()));
                    if (talentData->getDebuffType() == kDebuffTieDown)
                    {
                        CCString *value3 = static_cast<CCString *>(valueArray2->objectAtIndex(1));
                        talentData->setDebuffAddedValue(atoi(value3->getCString()));
                    }
                    
                }
                    break;
                case kTypeDOT:
                {
                    talentData->setDOTType(dotType);
                    CCArray *valueArray2 = split_string(valueStr.c_str(),',');
                    CCString *value2 = static_cast<CCString *>(valueArray2->objectAtIndex(0));
                    talentData->setDOTDuration(atoi(value2->getCString()));
                    CCString *value3 = static_cast<CCString *>(valueArray2->objectAtIndex(1));
                    talentData->setDOTDamagePoint(atoi(value3->getCString()));
                }
                    break;
                case kTypeBeatBack:
                    talentData->setBeatedBackDistance(valueStr);
                    break;
            }
        }
    }
    createSkillTalentArrData();
    CC_SAFE_DELETE(fileReader);
}
/**
 *创建技能天赋数据
 */
void GlobalData::createSkillTalentArrData()
{
    if (m_pTalentDataArrayDict)
    {
        m_pTalentDataArrayDict->removeAllObjects();
    }
    CCArray* allKeys = m_pTalentDataDict->allKeys();
    // 生成技能对应的天赋数组
    for (int i = 0; i < allKeys->count(); i++)
    {
        int nKey = static_cast<CCInteger *>(allKeys->objectAtIndex(i))->getValue(); // 天赋id
        CCArray* arr = static_cast<CCArray *>(m_pTalentDataDict->objectForKey(nKey)); // 天赋数组
        TalentData * talentData = static_cast<TalentData *>(arr->objectAtIndex(0));  // 天赋数据
        talentData->setLevel(0);// 默认天赋点为0
        int nSkillID = talentData->getSkillIndex();
        CCArray* arrTalentData = static_cast<CCArray *>(m_pTalentDataArrayDict->objectForKey(nSkillID));
        if(!arrTalentData)
        {
            arrTalentData = CCArray::create();
            m_pTalentDataArrayDict->setObject(arrTalentData, nSkillID);
        }
        arrTalentData->addObject(talentData);
    }
}
/**
 * 读取玩家数据
 */
void GlobalData::readPlayerData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("spritedata1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        SpriteData * spriteData = SpriteData ::create();
        spriteData->setSpriteIndex(fileReader->readInt());          //  设置精灵编号
        spriteData->setSpriteName(fileReader->readUTF(true));        //  设置精灵名字
        spriteData->setResourceArray(split_string(fileReader->readUTF(true), ','));        //  资源文件名
        spriteData->setType(fileReader->readInt());        //  设置精灵类型
        spriteData->setScaleFactor(CCString::create(fileReader->readUTF(true))->floatValue());        // 设置缩放比例
        spriteData->setNameOffsetHeight(fileReader->readInt());// 设置名字偏移的高度
        spriteData->setTenacity(fileReader->readInt());        //  设置精灵韧性
        //  身体矩形（受击范围）
        CCArray * array = split_string(fileReader->readUTF(true), ',');
        int x = ((CCString *)array->objectAtIndex(0))->intValue();
        int y = ((CCString *)array->objectAtIndex(1))->intValue();
        int w = ((CCString *)array->objectAtIndex(2))->intValue();
        int h = ((CCString *)array->objectAtIndex(3))->intValue();
        spriteData->setRangeOfBody(CCRectMake(x, y, w, h));
        
        spriteData->setSpeed(fileReader->readInt());        // 位移速率
        // 视距
        array = split_string(fileReader->readUTF(true), ',');
        w = ((CCString *)array->objectAtIndex(0))->intValue();
        h = ((CCString *)array->objectAtIndex(1))->intValue();
        spriteData->setRangeOfVisibility(CCSizeMake(w, h));
        
        // 攻击范围判定
        CCArray * attackRectArray = CCArray::createWithCapacity(1);
        array = split_string(fileReader->readUTF(true), ',');
        int offsetX = ((CCString *)array->objectAtIndex(0))->intValue();
        if (array->count() > 1)
        {
            int offsetY = ((CCString *)array->objectAtIndex(1))->intValue();
            int w = ((CCString *)array->objectAtIndex(2))->intValue();
            int h = ((CCString *)array->objectAtIndex(3))->intValue();
            YYRect * atkRect = YYRect::createRect(CCRectMake(offsetX, offsetY, w, h));
            attackRectArray->addObject(atkRect);
            spriteData->setAttackRectArray(attackRectArray);
        }
        
        m_pSpriteDataDict->setObject(spriteData, spriteData->getSpriteIndex());
    }
    CC_SAFE_DELETE(fileReader);
}

/**
 * 读取非战斗精灵数据
 */
void GlobalData::readNFSData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("spritedata3", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        SpriteData * spriteData = SpriteData ::create();
        spriteData->setSpriteIndex(fileReader->readInt());        //  设置精灵编号
        spriteData->setSpriteName(fileReader->readUTF(true));        //  设置精灵名称
        spriteData->setResourceArray(split_string(fileReader->readUTF(true), ','));        //  资源文件名
        spriteData->setType(fileReader->readInt());        //  设置精灵类型
        spriteData->setScaleFactor(CCString::create(fileReader->readUTF(true))->floatValue());        // 设置缩放比例
        spriteData->setNameOffsetHeight(fileReader->readInt());// 设置名字偏移的高度
        spriteData->setSpriteImageName(fileReader->readUTF(true));
        spriteData->setSpriteTalkContent(fileReader->readUTF(true));
        getSpriteDataDict()->setObject(spriteData, spriteData->getSpriteIndex());
    }
    delete fileReader;
}

/**
 * 读取可战斗精灵的数据
 */
void GlobalData::readFightSpriteData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("spritedata2", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        SpriteData * spriteData = SpriteData::create();
        spriteData->setSpriteIndex(fileReader->readInt());        //  设置精灵编号
        spriteData->setSpriteName(fileReader->readUTF(true));        //  设置精灵名称
        spriteData->setResourceArray(split_string(fileReader->readUTF(true), ','));        //  资源文件名
        spriteData->setType(fileReader->readInt());        //  设置精灵类型
        spriteData->setScaleFactor(CCString::create(fileReader->readUTF(true))->floatValue());        // 设置缩放比例
        spriteData->setNameOffsetHeight(fileReader->readInt());// 设置名字偏移的高度
        spriteData->setTenacity(fileReader->readInt());        //  设置精灵韧性
        //  身体矩形（受击范围）
        CCArray * array = split_string(fileReader->readUTF(true), ',');
        int x = ((CCString *)array->objectAtIndex(0))->intValue();
        int y = ((CCString *)array->objectAtIndex(1))->intValue();
        int w = ((CCString *)array->objectAtIndex(2))->intValue();
        int h = ((CCString *)array->objectAtIndex(3))->intValue();
        spriteData->setRangeOfBody(CCRectMake(x, y, w, h));
        
        spriteData->setSpeed(fileReader->readInt());// 移动速度
        // 视距
        array = split_string(fileReader->readUTF(true), ',');
        w = ((CCString *)array->objectAtIndex(0))->intValue();
        h = ((CCString *)array->objectAtIndex(1))->intValue();
        spriteData->setRangeOfVisibility(CCSizeMake(w, h));
        
        spriteData->setRangeIndex(fileReader->readInt());//按哪个攻击范围设定与目标之间的最小距离
        // 攻击范围
        array = split_string(fileReader->readUTF(true), ';');
        CCArray * attackRectArray = CCArray::createWithCapacity(array->count());
        for (int j = 0; j < array->count(); j++) {
            CCArray * array1 = split_string(((CCString *)array->objectAtIndex(j))->getCString(), ',');
            int x = ((CCString *)array1->objectAtIndex(0))->intValue();
            if (array1->count() > 1)
            {
                int y = ((CCString *)array1->objectAtIndex(1))->intValue();
                int w = ((CCString *)array1->objectAtIndex(2))->intValue();
                int h = ((CCString *)array1->objectAtIndex(3))->intValue();
                YYRect * atkRect = YYRect::createRect(CCRectMake(x, y, w, h));
                attackRectArray->addObject(atkRect);
            }
            else
            {
                attackRectArray->addObject(CCInteger::create(x));
            }
        }
        spriteData->setAttackRectArray(attackRectArray);
        
        //  攻击几率
        array = split_string(fileReader->readUTF(true), ',');
        spriteData->setAttackOddsArray(array);
        
        // 不同血值范围对应可发动的攻击方式
        array = split_string(fileReader->readUTF(true), ';');
        spriteData->setHPRangeArray(array);
        
        //  满足施放技能条件的情况下可施放的次数
        array = split_string(fileReader->readUTF(true), ',');
        spriteData->setNumOfUsedArray(array);

        //  每种攻击方式对应的技能编号
        array = split_string(fileReader->readUTF(true), ',');
        spriteData->setSkillIndexArray(array);
        
        //  多久重新确定一个行为(站立，追踪目标，随机移动)
        spriteData->setConfirmBehaviorDuration(fileReader->readInt() * FRAME_PER_SECOND);
        
        //  执行不同行为对应的几率(第一组为执行完站立原地的行为后或目标刚刚进入视野范围内执行三种不同行为的几率，第二组为追踪目标后再执行三种不同行为逻辑的几率，第三组为随机移动后再执行三种不同行为逻辑的几率)
        array = split_string(fileReader->readUTF(true), ';');
        spriteData->setBehaviorArray(array);
        
        spriteData->setAttributeIndex(fileReader->readInt());//对应的属性编号
        spriteData->setPortraitFileName(fileReader->readUTF(true));
        spriteData->setBloodBarLayerNum(fileReader->readInt());// boss血条层数
        spriteData->setBossMarkResource(fileReader->readUTF(true));// boss标记
        getSpriteDataDict()->setObject(spriteData, spriteData->getSpriteIndex());
    }
    CC_SAFE_DELETE(fileReader);
}

/**
 * 读取精灵属性
 */
void GlobalData::readSpriteAttribute()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("monsterdata1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        SpriteAttribute * spriteAttribute = SpriteAttribute::create();
        
        spriteAttribute->setID(fileReader->readInt());
        fileReader->readUTF(true);// 名称
        spriteAttribute->setMaxHP(fileReader->readInt());
        spriteAttribute->setAttackPoint(fileReader->readInt());
        spriteAttribute->setDefencePoint(fileReader->readInt());
        spriteAttribute->setSpellAttackPoint(fileReader->readInt());
        spriteAttribute->setSkillDamagePoint(fileReader->readInt());
        spriteAttribute->setBasicHitOdds(fileReader->readInt());
        spriteAttribute->setHitLevel(fileReader->readInt());
        spriteAttribute->setHitConstant(fileReader->readInt());
        spriteAttribute->setBasicCriticalOdds(fileReader->readInt());
        spriteAttribute->setCriticalLevel(fileReader->readInt());
        spriteAttribute->setCriticalConstant(fileReader->readInt());
        getSpriteAttributeDict()->setObject(spriteAttribute,  spriteAttribute->getID());
    }
    delete fileReader;
}

void GlobalData::readNickNameData()
{
    const std::string filePath = getFilePath("nickname", kDirTypeData, kFileSuffixPlist);
    m_pNickNameArray = CCArray::createWithContentsOfFile(filePath.c_str());
    m_pNickNameArray->retain();
}
// 星灵相关
void GlobalData::readProtossData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("protossData1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        YYProtossData *pProtossData = YYProtossData::create();
        m_pProtossDataArray->addObject(pProtossData);
        pProtossData->setID(fileReader->readInt());
        pProtossData->setName(fileReader->readUTF(true));
        pProtossData->setType1(fileReader->readInt());
        pProtossData->setType2(fileReader->readInt());
        pProtossData->setExp(fileReader->readInt());
        pProtossData->setPhagocytic(fileReader->readInt());
        pProtossData->setNext(fileReader->readInt());
        pProtossData->setLevel(fileReader->readInt());
        pProtossData->setQuality(fileReader->readInt());
        pProtossData->setNeedLevel(fileReader->readInt());
        pProtossData->setIcon(fileReader->readUTF(true));
        pProtossData->setDesc(fileReader->readUTF(true));
        pProtossData->setPrice(fileReader->readInt());
        pProtossData->setStrength(fileReader->readInt());
        pProtossData->setAgility(fileReader->readInt());
        pProtossData->setStamina(fileReader->readInt());
        pProtossData->setInte(fileReader->readInt());
        pProtossData->setAttack(fileReader->readInt());
        pProtossData->setPDefense(fileReader->readInt());
        pProtossData->setMagic(fileReader->readInt());
        pProtossData->setMDefense(fileReader->readInt());
        pProtossData->setHP(fileReader->readInt());                                                                                                                                                                                                                       
        pProtossData->setMP(fileReader->readInt());
        pProtossData->setHit(fileReader->readInt());
        pProtossData->setCrit(fileReader->readInt());
        pProtossData->setTitle(fileReader->readUTF(true));
        pProtossData->setColorType(fileReader->readInt());
    }
    delete fileReader;
}
YYProtossData *GlobalData::getProtossData(int nID)
{
    if (!m_pProtossDataArray)
        return NULL;
    
    for (int i = 0; i < m_pProtossDataArray->count(); i++)
    {
        YYProtossData *pProtossData = (YYProtossData *)m_pProtossDataArray->objectAtIndex(i);
        if (nID == pProtossData->getID())
        {
            return pProtossData;
        }
    }
    return NULL;
}
void GlobalData::readConstellationData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("protossPray1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        YYConstellationData *pConstellationData = YYConstellationData::create();
        m_pConstellationDataArray->addObject(pConstellationData);
        pConstellationData->setConstellationID(fileReader->readInt());
        pConstellationData->setNextRate(CCString::create(fileReader->readUTF(true))->floatValue());
        pConstellationData->setGrayRate(CCString::create(fileReader->readUTF(true))->floatValue());
        pConstellationData->setWhiteRate(CCString::create(fileReader->readUTF(true))->floatValue());
        pConstellationData->setGreenRate(CCString::create(fileReader->readUTF(true))->floatValue());
        pConstellationData->setBlueRate(CCString::create(fileReader->readUTF(true))->floatValue());
        pConstellationData->setPurpleRate(CCString::create(fileReader->readUTF(true))->floatValue());
        pConstellationData->setScrapRate(CCString::create(fileReader->readUTF(true))->floatValue());
        pConstellationData->setProtossMoeny(fileReader->readInt());
        pConstellationData->setLightConstellationJew(fileReader->readInt());
    }
    delete fileReader;
}
void GlobalData::readScrapData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("scrap1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        YYProtossData *pProtossData = YYProtossData::create();
        m_pScrapDataArray->addObject(pProtossData);
        pProtossData->setID(fileReader->readInt());
        pProtossData->setExchangeNum(fileReader->readInt());
    }
    delete fileReader;
}
void GlobalData::readActivityData()
{
    //读取活跃度任务
    {
        FileReadManager * fileReader =  new FileReadManager();
        const std::string filePath = getFilePath("activity1", kDirTypeData, kFileSuffixYbd);
        fileReader->readFileData(filePath.c_str(), "rb",false);
        int rowAmount = fileReader->readInt();
        for (int i = 0; i < rowAmount; i++)
        {
            YYActivityTaskData* taskData = YYActivityTaskData::create();
            taskData->setID(fileReader->readInt()); // 任务ID
            taskData->setAwardScore(fileReader->readInt()); // 奖励分数
            taskData->setType(fileReader->readInt()); // 任务类型
            taskData->setTotalNum(fileReader->readInt()); // 需要完成总数
            taskData->setEnterIndex(fileReader->readInt()); // 任务入口
            taskData->setDescription(fileReader->readUTF(true)); // 任务描述
            m_pActivityTaskArray->addObject(taskData);
        }
        CC_SAFE_DELETE(fileReader);
    }

    //读取活跃度奖励
    for (int i = 0; i < 4 ; i++)
    {
        CCString* fp = CCString::createWithFormat("activity%d",i+2);
        const std::string filePath = getFilePath(fp->getCString(), kDirTypeData, kFileSuffixYbd);
        FileReadManager * fileReader =  new FileReadManager();
        fileReader->readFileData(filePath.c_str(), "rb",false);
        int rowAmount = fileReader->readInt();
        CCArray * arrTreeData = CCArray::create();
        for (int i = 0; i < rowAmount; i++)
        {
            
            YYActivityWishTreeData* treeData = YYActivityWishTreeData::create();
            treeData->setIndex(fileReader->readInt()); // 索引
            treeData->setNeedScore(fileReader->readInt()); // 领取宝箱需要分数
            int nType = fileReader->readInt();
            treeData->setType(nType); // 类型
            CCString* data = CCString::createWithFormat("%s",fileReader->readUTF(true));
            switch (nType)
            {
                case 0: //道具
                {
                    CCArray* array = split_string(data->getCString(), ':');
                    CCString* idx = (CCString*)array->objectAtIndex(0);
                    CCString* num = (CCString*)array->objectAtIndex(1);
                    treeData->setItemIdx(idx->intValue());
                    treeData->setNum(num->intValue());
                }
                    break;
                case 1: //宝石
                case 2: //金币
                case 3: //经验
                {
                    treeData->setNum(data->intValue());
                }
                    break;
                default:
                    break;
            }
            arrTreeData->addObject(treeData);
        }
       m_pActivityWishTreeDict->setObject(arrTreeData, i);
       CC_SAFE_DELETE(fileReader);
    }
}
void GlobalData::readAchievementClassData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("achi1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        AchievementClassData *pAchieveData = AchievementClassData::create();
        m_pAchievementClassDataArray->addObject(pAchieveData);
        pAchieveData->setIndex(fileReader->readInt());
        pAchieveData->setName(fileReader->readUTF(true));
    }
    CC_SAFE_DELETE(fileReader);
}
void GlobalData::readAchievementGroupData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("achi2", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    int nId = 0, nNum = 0;
    std::string sIcon = "";
    for (int i = 0; i < rowAmount; i++)
    {
        AchievementGroupData *pAchieveData = AchievementGroupData::create();
        m_pAchievementGroupDataArray->addObject(pAchieveData);
        pAchieveData->setIndex(fileReader->readInt());
        pAchieveData->setName(fileReader->readUTF(true));
        fileReader->readInt();      //成就类型 0 线性 1 并列
        std::string strItem = fileReader->readUTF(true);
        if (strItem != std::string("0"))
        {
            CCArray *pArrayItem = split_string(strItem.c_str(), ':');
            nId = ((CCString *)pArrayItem->objectAtIndex(0))->intValue();
            nNum = ((CCString *)pArrayItem->objectAtIndex(1))->intValue();
            sIcon = ((CCString *)pArrayItem->objectAtIndex(2))->getCString();
        }
        int nGoldNum = fileReader->readInt();
        if (nGoldNum > 0)
        {
            nId = 0;
            nNum = nGoldNum;
            sIcon = "i-5-0-4-0";
        }
        int nJewelNum = fileReader->readInt();
        if (nJewelNum > 0)
        {
            nId = 1;
            nNum = nJewelNum;
            sIcon = "i-5-0-3-0";
        }
        int nExpNum = fileReader->readInt();
        if (nExpNum > 0)
        {
            nId = 2;
            nNum = nExpNum;
            sIcon = "i-5-0-2-0";
        }
        AwardData *pAward = pAchieveData->getAwardData();
        pAward->setIndex(nId);
        pAward->setNum(nNum);
        pAward->setIcon(sIcon);
    }
    CC_SAFE_DELETE(fileReader);
}

void GlobalData::readAchievementItemData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("achi3", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    int nId = 0, nNum = 0;
    std::string sIcon = "";
    for (int i = 0; i < rowAmount; i++)
    {
        AchievementItemData *pAchieveData = AchievementItemData::create();
        m_pAchievementItemDataArray->addObject(pAchieveData);
        pAchieveData->setIndex(fileReader->readInt());
        pAchieveData->setNum(fileReader->readInt());
        pAchieveData->setDesc(fileReader->readUTF(true));
        std::string strItem = fileReader->readUTF(true);
        if (strItem != std::string("0"))
        {
            CCArray *pArrayItem = split_string(strItem.c_str(), ':');
            nId = ((CCString *)pArrayItem->objectAtIndex(0))->intValue();
            nNum = ((CCString *)pArrayItem->objectAtIndex(1))->intValue();
            sIcon = ((CCString *)pArrayItem->objectAtIndex(2))->getCString();
        }
        int nGoldNum = fileReader->readInt();
        if (nGoldNum > 0)
        {
            nId = 0;
            nNum = nGoldNum;
            sIcon = "i-5-0-4-0";
        }
        int nJewelNum = fileReader->readInt();
        if (nJewelNum > 0)
        {
            nId = 1;
            nNum = nJewelNum;
            sIcon = "i-5-0-3-0";
        }
        int nExpNum = fileReader->readInt();
        if (nExpNum > 0)
        {
            nId = 2;
            nNum = nExpNum;
            sIcon = "i-5-0-2-0";
        }
        AwardData *pAward = pAchieveData->getAwardData();
        pAward->setIndex(nId);
        pAward->setNum(nNum);
        pAward->setIcon(sIcon);
    }
    CC_SAFE_DELETE(fileReader);
}

void GlobalData::setAchievementAllData()
{
    do
    {
        CC_BREAK_IF(!m_pAchievementClassDataArray);
        for (int i = 0; i < m_pAchievementClassDataArray->count(); i++)
        {
            AchievementClassData * pClassData = (AchievementClassData*)m_pAchievementClassDataArray->objectAtIndex(i);
            uint32_t nClassIdx = pClassData->getIndex();
            m_pAchievementAllDataDict->setObject(pClassData, nClassIdx);
            CCDictionary* pClassChildDic = pClassData->getChildDic();
            int nGroupCount = 0;
            CC_BREAK_IF(!m_pAchievementGroupDataArray);
            for (int j = 0; j < m_pAchievementGroupDataArray->count(); j++)
            {
                AchievementGroupData * pGroupData = (AchievementGroupData*)m_pAchievementGroupDataArray->objectAtIndex(j);
                uint32_t nGroupIdx = pGroupData->getIndex();
                CCDictionary* pGroupChildDic = pGroupData->getChildDic();
                int nCmpGroupIdx = getFirstNBitNumber(nGroupIdx,2); //分组索引前2位未分类索引
                int nItemCount = 0;
                if (nCmpGroupIdx == nClassIdx)
                {
                    pClassChildDic->setObject(pGroupData, nGroupIdx);
                    nGroupCount++;
                    CC_BREAK_IF(!m_pAchievementItemDataArray);
                    for (int k = 0; k < m_pAchievementItemDataArray->count(); k++)
                    {
                        AchievementItemData* pItemData = (AchievementItemData*)m_pAchievementItemDataArray->objectAtIndex(k);
                        uint32_t nItemIdx = pItemData->getIndex();
                        int nCmpItemIdx = getFirstNBitNumber(nItemIdx, 4); //分项索引前4位未分组索引
                        if (nCmpItemIdx == nGroupIdx)
                        {
                            pGroupChildDic->setObject(pItemData, nItemIdx);
                            nItemCount++;
                        }
                    }
                    pGroupData->setTotalNum(nItemCount);  //分组内各项总数
                }
            }
            pClassData->setTotalNum(nGroupCount);  //分类内分组总数
        }
    } while (0);

}

AwardData* GlobalData::setAchievementAwardData(int nIndex, int nNum, std::string sIcon)
{
    AwardData *pAwardData = AwardData::create();
    pAwardData->setIndex(nIndex);
    pAwardData->setNum(nNum);
    pAwardData->setIcon(sIcon);
    return pAwardData;
}

AchievementClassData *GlobalData::getAchievementClassData(int nClassIdx)
{
    if (m_pAchievementAllDataDict)
    {
        AchievementClassData * pClassData = (AchievementClassData *)m_pAchievementAllDataDict->objectForKey(nClassIdx);
        return pClassData;
    }
    return NULL;
}

AchievementGroupData *GlobalData::getAchievementGroupData(int nClassIdx, int nGroupIdx)
{
    if (m_pAchievementAllDataDict)
    {
        AchievementClassData * pClassData = getAchievementClassData(nClassIdx);
        if (pClassData)
        {
            AchievementGroupData* pGroupData = (AchievementGroupData*)(pClassData->getChildDic()->objectForKey(nGroupIdx));
            return pGroupData;
        }
    }
    return NULL;
}

AchievementItemData *GlobalData::getAchievementItemData(int nClassIdx, int nGroupIdx, int nItemIdx)
{
    AchievementGroupData* pGroupData = getAchievementGroupData(nClassIdx, nGroupIdx);
    if (pGroupData)
    {
        AchievementItemData* pItemData = (AchievementItemData* )(pGroupData->getChildDic()->objectForKey(nItemIdx));
        return pItemData;
    }
    return NULL;
}

void GlobalData::readSignUpData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("sign-301", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb", false);
    int rowAmount = fileReader->readInt();
    int nId = 0, nNum = 0;
    std::string sIcon = "";
    for (int i = 0; i < rowAmount; i++)
    {
        if (i == 0)
        {
            fileReader->readInt();
            std::string strItem = fileReader->readUTF(true);
            if (strItem != std::string("0"))
            {
                CCArray *pArrayItem = split_string(strItem.c_str(), ':');
                nId = ((CCString *)pArrayItem->objectAtIndex(0))->intValue();
                nNum = ((CCString *)pArrayItem->objectAtIndex(1))->intValue();
                sIcon = ((CCString *)pArrayItem->objectAtIndex(2))->getCString();
                m_pSignUpStaticAwardArray->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
            int nGoldNum = fileReader->readInt();
            if (nGoldNum > 0)
            {
                nId = 0;
                nNum = nGoldNum;
                sIcon = "i-5-0-4-0";
                m_pSignUpStaticAwardArray->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
            int nJewelNum = fileReader->readInt();
            if (nJewelNum > 0)
            {
                nId = 1;
                nNum = nJewelNum;
                sIcon = "i-5-0-3-0";
                m_pSignUpStaticAwardArray->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
            int nExpNum = fileReader->readInt();
            if (nExpNum > 0)
            {
                nId = 2;
                nNum = nExpNum;
                sIcon = "i-5-0-2-0";
                m_pSignUpStaticAwardArray->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
        }
        else
        {
            YYSignUpData *pSignUpData = YYSignUpData::create();
            m_pSignUpDataArray->addObject(pSignUpData);
            pSignUpData->setIndex(fileReader->readInt());
            CCArray *pArrayAward = pSignUpData->getArrayAwardData();
            std::string strItem = fileReader->readUTF(true);
            if (strItem != std::string("0"))
            {
                CCArray *pArrayItem = split_string(strItem.c_str(), ':');
                nId = ((CCString *)pArrayItem->objectAtIndex(0))->intValue();
                nNum = ((CCString *)pArrayItem->objectAtIndex(1))->intValue();
                sIcon = ((CCString *)pArrayItem->objectAtIndex(2))->getCString();
                pArrayAward->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
            int nGoldNum = fileReader->readInt();
            if (nGoldNum > 0)
            {
                nId = 0;
                nNum = nGoldNum;
                sIcon = "i-5-0-4-0";
                pArrayAward->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
            int nJewelNum = fileReader->readInt();
            if (nJewelNum > 0)
            {
                nId = 1;
                nNum = nJewelNum;
                sIcon = "i-5-0-3-0";
                pArrayAward->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
            int nExpNum = fileReader->readInt();
            if (nExpNum > 0)
            {
                nId = 2;
                nNum = nExpNum;
                sIcon = "i-5-0-2-0";
                pArrayAward->addObject(setAchievementAwardData(nId, nNum, sIcon));
            }
        }
    }
    CC_SAFE_DELETE(fileReader);
}

void GlobalData::readSignUpBoxAwardData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("sign-302", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb", false);
    int rowAmount = fileReader->readInt();
    int nId = 0, nNum = 0;
    std::string sIcon = "";
    for (int i = 0; i < rowAmount; i++)
    {
        YYSignUpBoxAwardData *pSignUpBoxAwardData = YYSignUpBoxAwardData::create();
        m_pSigUpBoxAwardDataArray->addObject(pSignUpBoxAwardData);
        pSignUpBoxAwardData->setIndex(fileReader->readInt());
        std::string strItem = fileReader->readUTF(true);
        if (strItem != std::string("0"))
        {
            CCArray *pArrayItem = split_string(strItem.c_str(), ':');
            nId = ((CCString *)pArrayItem->objectAtIndex(0))->intValue();
            nNum = ((CCString *)pArrayItem->objectAtIndex(1))->intValue();
            sIcon = ((CCString *)pArrayItem->objectAtIndex(2))->getCString();
        }
        int nGoldNum = fileReader->readInt();
        if (nGoldNum > 0)
        {
            nId = 0;
            nNum = nGoldNum;
            sIcon = "i-5-0-4-0";
        }
        int nJewelNum = fileReader->readInt();
        if (nJewelNum > 0)
        {
            nId = 1;
            nNum = nJewelNum;
            sIcon = "i-5-0-3-0";
        }
        int nExpNum = fileReader->readInt();
        if (nExpNum > 0)
        {
            nId = 2;
            nNum = nExpNum;
            sIcon = "i-5-0-2-0";
        }
        AwardData *pAward = pSignUpBoxAwardData->getAwardData();
        pAward->setIndex(nId);
        pAward->setNum(nNum);
        pAward->setIcon(sIcon);
    }
    CC_SAFE_DELETE(fileReader);
}

YYSignUpData * GlobalData::getSignUpData(int nDay)
{
    for (int i = 0; i < m_pSignUpDataArray->count(); i++)
    {
        YYSignUpData *pSignUpData = (YYSignUpData *)m_pSignUpDataArray->objectAtIndex(i);
        if (nDay == pSignUpData->getIndex())
        {
            return pSignUpData;
        }
    }
    return NULL;
}

YYSignUpBoxAwardData *GlobalData::getSignUpBoxAwardData(int nIndex)
{
    if (m_pSigUpBoxAwardDataArray->count() > nIndex)
    {
        YYSignUpBoxAwardData *pSignUpBoxAwardData = (YYSignUpBoxAwardData *)m_pSigUpBoxAwardDataArray->objectAtIndex(nIndex);
        return pSignUpBoxAwardData;
    }
    return NULL;
}

void GlobalData::readLoginAwardData()
{
    for (int i = 1; i <= 4; i++)
    {
        FileReadManager * fileReader =  new FileReadManager();
        CCString *fileName = CCString::createWithFormat("sign-7%d", i);
        const std::string filePath = getFilePath(fileName->getCString(), kDirTypeData, kFileSuffixYbd);
        fileReader->readFileData(filePath.c_str(), "rb", false);
        int rowAmount = fileReader->readInt();
        CCArray *array = CCArray::createWithCapacity(7);
        m_pLoginAwardDict->setObject(array, i);
        for (int j = 0; j < rowAmount; j++)
        {
            LoginAwardData *pLoginAwardData = LoginAwardData::create();
            array->addObject(pLoginAwardData);
            pLoginAwardData->setIndex(fileReader->readInt());
            const char *itemInfo = fileReader->readUTF(true);
            if (itemInfo)
            {
                CCArray * arr = split_string(itemInfo, ';');
                if (arr)
                {
                    pLoginAwardData->setItemInfoArray(arr);
                }
            }
            const char *description = fileReader->readUTF(true);
            if (description)
            {
                CCArray * arr = split_string(description, ';');
                if (arr)
                {
                    pLoginAwardData->setDescriptionArray(arr);
                }
            }
        }
        CC_SAFE_DELETE(fileReader);
    }
}

#pragma mark 读取Vip数据

void GlobalData::readVipData()
{
    FileReadManager * fileReader =  new FileReadManager();
    const std::string filePath = getFilePath("vip1", kDirTypeData, kFileSuffixYbd);
    fileReader->readFileData(filePath.c_str(), "rb",false);
    int rowAmount = fileReader->readInt();
    for (int i = 0; i < rowAmount; i++)
    {
        VipData * vipData = VipData::create();
        int nLevel = fileReader->readInt();    // vip 等级
        vipData->setLevel(nLevel);
        m_pVipDataDict->setObject(vipData, nLevel);
        vipData->setJewAmount(fileReader->readInt()); // 升级所需充值宝石数
        vipData->setDesc(fileReader->readUTF(true)); // 描述
        
    }
    CC_SAFE_DELETE(fileReader);
}

#pragma mark 任务相关

void GlobalData::addTaskData(cocos2d::CCArray *array)
{
    int i = 0;
    int taskIndex = ((CCString *)array->objectAtIndex(i++))->intValue();
    TaskData *taskData = dynamic_cast<TaskData *>(m_pTaskDataDict->objectForKey(taskIndex));
    if (!taskData)
    {
        taskData = TaskData::create();
        m_pTaskDataDict->setObject(taskData, taskIndex);
        m_pTaskDataArray->addObject(taskData);
    }
    taskData->setTaskIndex(taskIndex);
    taskData->setTitle(((CCString *)array->objectAtIndex(i++))->getCString());
    taskData->setDescription(((CCString *)array->objectAtIndex(i++))->getCString());
    taskData->setNeedLevel(((CCString *)array->objectAtIndex(i++))->intValue());
    //任务奖励物品最多三组，以；分割（每组物品以,分割图标和数量）、金钱和奖励
    CCArray *arr = split_string(((CCString *)array->objectAtIndex(i++))->getCString(), ';');
    if (arr)
    {
        for (int i = 0; i < arr->count(); i++)
        {
            CCString * str = static_cast<CCString *>(arr->objectAtIndex(i));
            if (str->length() > 0)
            {
                taskData->getItemAwardArray()->addObject(str);
            }
        }
    }
    arr = split_string(((CCString *)array->objectAtIndex(i++))->getCString(), ';');
    if (arr)
    {
        for (int i = 0; i < arr->count(); i++)
        {
            CCString * str = static_cast<CCString *>(arr->objectAtIndex(i));
            int value = str->intValue();
            if (i == 0)
            {
                taskData->setGoldAwardNum(value);
            }
            else if (i == 1)
            {
                taskData->setExpAwardNum(value);
            }
            else if (i == 2)
            {
                taskData->setJewelAwardNum(value);
            }
            
        }
    }
    taskData->setTaskType(((CCString *)array->objectAtIndex(i++))->getCString());
    sortTaskData();
}

void GlobalData::sortTaskData()
{
    if (m_pTaskDataArray->count() > 1)
    {
        for (int i = 0; i < m_pTaskDataArray->count(); i++)
        {
            TaskData *taskData1 = static_cast<TaskData *>(m_pTaskDataArray->objectAtIndex(i));
            int type1 = taskData1->getTaskIndex();
            for (int j = i + 1; j < m_pTaskDataArray->count(); j++)
            {
                TaskData *taskData2 = static_cast<TaskData *>(m_pTaskDataArray->objectAtIndex(i));
                int type2 = taskData2->getTaskIndex();
                if (type1 > type2)
                {
                    m_pTaskDataArray->exchangeObject(taskData1, taskData2);
                }
            }
        }
    }
}

#pragma mark 通过玩家ID和技能ID 获取技能

SkillData *GlobalData::getPlayerSkillData(const char * playerID, int skillIndex)
{
    for (int i = 0; i < m_pPlayerSkillDataArray->count(); i++)
    {
        SkillData *skillData = static_cast<SkillData *>(m_pPlayerSkillDataArray->objectAtIndex(i));
        if (skillData->getPlayerID().compare(playerID) == 0 && skillData->getIndex() == skillIndex)
        {
            return skillData;
        }
    }
    return NULL;
}

#pragma mark 通过技能ID 等级默认为1 获取技能 获取怪物技能

SkillData *GlobalData::getCommonSkillData(int skillIndex, int skillLevel)
{
    do
    {
        CC_BREAK_IF(!m_pSkillDataDict);
        CCArray* arr = static_cast<CCArray *>(m_pSkillDataDict->objectForKey(skillIndex));
        CC_BREAK_IF(!arr);
        SkillData* skillData = static_cast<SkillData *>(arr->objectAtIndex(skillLevel - 1));
        if (skillData)
        {
            return skillData;
        }
    } while (0);
    return NULL;
}

void GlobalData::receiveSkillSQ(cocos2d::CCArray *arr, const char * userID)
{
    // SQ_fchqwe.1_1300;1_1301;1_1302;1_2300;1_2301;1_2302;1_2303;1_2304;1_3000;1_3100;1_3200;1
    do
    {
        CC_BREAK_IF(!arr);
        removeSkillCacheData(); // 清除技能相关缓存数据
        const char * playerID = ((CCString*)arr->objectAtIndex(0))->getCString(); // 技能玩家ID
        for (int i = 1; i < arr->count(); i++)
        {
            CCArray* tmpIdx = split_string((static_cast<CCString *>(arr->objectAtIndex(i)))->getCString(), ';');
            int nIdx = atoi(static_cast<CCString *>(tmpIdx->objectAtIndex(0))->getCString());   // 技能id
            int nLevel = atoi(static_cast<CCString *>(tmpIdx->objectAtIndex(1))->getCString()); // 技能等级
            CCArray* tmpSkill = static_cast<CCArray *>(m_pSkillDataDict->objectForKey(nIdx));
            SkillData* skillDataTable = static_cast<SkillData *>(tmpSkill->objectAtIndex(nLevel-1));// 获取技能表格数据
            SkillData* skillData =skillDataTable->copyWithZone(); // 技能数据copy
            skillData->setPlayerID(playerID);
            skillData->getSkillType() == 3 ? skillData->setAngrySkill(true) :  skillData->setAngrySkill(false);
            if (!m_pPlayerSkillDataArray->containsObject(skillData))
            {
                m_pPlayerSkillDataArray->addObject(skillData);
            }
            if (strcmp(userID, playerID) == 0 && !m_pUsableSkillDataArray->containsObject(skillData) && skillData->getSkillType() == 2) // 玩家使用的5个基础技能
            {
                m_pUsableSkillDataArray->addObject(skillData);
            }
            if (strcmp(userID, playerID) == 0 && !m_pUsableGestureSkillDataArray->containsObject(skillData) && skillData->getSkillType() == 3) // 玩家使用怒技

            {
                int gestureType = skillData->getGestureType();
                m_pUsableGestureSkillDataArray->addObject(skillData);
                m_pGestureSkillDataArray[gestureType]->addObject(skillData);
            }
        }
    } while (0);
}

void GlobalData::removeSkillCacheData()
{
    if (m_pPlayerSkillDataArray)
    {
        m_pPlayerSkillDataArray->removeAllObjects();
    }
    if (m_pUsableSkillDataArray)
    {
        m_pUsableSkillDataArray->removeAllObjects();
    }
    if (m_pUsableGestureSkillDataArray)
    {
        m_pUsableGestureSkillDataArray->removeAllObjects();
    }
    for (int i = 0; i < 3; i++)
    {
        if (m_pGestureSkillDataArray[i])
        {
            m_pGestureSkillDataArray[i]->removeAllObjects();
        }
    }
}

/**
 * 获得对应类型的手势技能
 */
CCArray * GlobalData::getGestureSkillDataArrayByType(int gestureType)
{
    return m_pGestureSkillDataArray[gestureType];
}

#pragma mark 天赋
/**
 * 接收天赋数据
 */
void GlobalData::receiveTalentData(cocos2d::CCArray *array)
{
    // SV_fchqwe.1_2300;2301;2302;2303;2304_230000,1;230001,1_230100,1_230200,1_230300,1_230400,1
    do
    {
        // 接收天赋数据
        CC_BREAK_IF(!array);
        createSkillTalentArrData(); //重新创建技能天赋缓存数据
        std::string playerID = (static_cast<CCString *>(array->objectAtIndex(0)))->getCString();
        CCArray* arrSikllID = split_string((static_cast<CCString *>(array->objectAtIndex(1)))->getCString(),';');
        for (int i = 0; i < arrSikllID->count(); i++)
        {
            int nSkillID = static_cast<CCString *>(arrSikllID->objectAtIndex(i))->intValue();// 技能ID
            CCArray* arrSumTalent = static_cast<CCArray *>(m_pTalentDataArrayDict->objectForKey(nSkillID)); // 技能包含的总天赋
            CCArray* arrHadTalent = split_string(static_cast<CCString *>(array->objectAtIndex(i+2))->getCString(),';'); // 技能已经学习的天赋
            for (int j = 0; j < arrSumTalent->count(); j++)
            {
                TalentData* talentData = static_cast<TalentData *>(arrSumTalent->objectAtIndex(j));
                for (int k = 0; k < arrHadTalent->count(); k++)
                {
                    CCArray* arrTmp = split_string(static_cast<CCString *>(arrHadTalent->objectAtIndex(k))->getCString(),',');
                    int nTalentIdx= static_cast<CCString *>(arrTmp->objectAtIndex(0))->intValue(); // 天赋等级
                    int nTalentLevel= static_cast<CCString *>(arrTmp->objectAtIndex(1))->intValue(); // 已学习天赋点
                    if (talentData->getIndex() == nTalentIdx) // 技能学到该天赋 替换技能默认天赋
                    {
                        CCArray* arrTalentData = static_cast<CCArray *>(m_pTalentDataDict->objectForKey(nTalentIdx));
                        TalentData* replaceData = static_cast<TalentData *>(arrTalentData->objectAtIndex(nTalentLevel-1)); // 新学习到的天赋
                        if (!arrSumTalent->containsObject(replaceData)) // 1点天赋向上升
                        {
                            replaceData->setLevel(nTalentLevel);
                            arrSumTalent->replaceObjectAtIndex(j, replaceData);
                        }
                        else // 0点 升到 1点
                        {
                            TalentData* pData = static_cast<TalentData *>(arrSumTalent->objectAtIndex(j));
                            pData->setLevel(nTalentLevel);
                        }
                    }
                }
            }
        }
        
        // 通过天赋属性更新技能属性值
        for (int i = 0; i < arrSikllID->count(); i++)
        {
            int nSkillID = static_cast<CCString *>(arrSikllID->objectAtIndex(i))->intValue();// 技能ID
            SkillData* skillData =  getPlayerSkillData(playerID.c_str(), nSkillID);
            int nSumTalentPoint = 0;            //技能所加天赋总点数
            CC_BREAK_IF(!skillData);
            CCArray* arrSumTalent = static_cast<CCArray *>(m_pTalentDataArrayDict->objectForKey(nSkillID)); // 技能包含的总天赋
            for (int j = 0; j < arrSumTalent->count(); j++)
            {
                TalentData* talentData = static_cast<TalentData *>(arrSumTalent->objectAtIndex(j));
                if (talentData && talentData->getLevel() > 0) // 学习到天赋
                {
                    nSumTalentPoint+=(talentData->getLevel());
                    int nType = talentData->getType();
                    switch (nType)
                    {
                        case kTypeReduceCD: // 减少CD时间
                        {
                            CCArray* tmp = split_string(talentData->getCDAddedValue().c_str(),',');
                            float fCD = static_cast<CCString *>(tmp->objectAtIndex(0))->floatValue();
                            fCD = skillData->getCoolingDuration() - fCD;
                            skillData->setCoolingDuration(fCD);
                        }
                            break;
                        case kTypeReduceMPConsume: // 减少魔法消耗
                        {
                            CCArray* tmp = split_string(talentData->getConsumeMPAddedValue().c_str(),',');
                            int nConsumePoint = static_cast<CCString *>(tmp->objectAtIndex(0))->intValue();
                            nConsumePoint = skillData->getConsumePoint() - nConsumePoint;
                            if(nConsumePoint < 0) // 特殊处理 魔法消耗小于0时 降至0
                            {
                                nConsumePoint = 0;
                            }
                            skillData->setConsumePoint(nConsumePoint);
                        }
                            break;
                        case kTypeAddDamage: // 增加攻击力
                        {
                            CCArray* tmp = split_string(talentData->getDamageAddedValue().c_str(),',');
                            int nDamageAdd = static_cast<CCString *>(tmp->objectAtIndex(0))->intValue();
                            nDamageAdd = skillData->getAddedDamage() + nDamageAdd;
                            skillData->setAddedDamage(nDamageAdd);
                        }
                            break;
                        case kTypeSetBreaked: // 发动技能攻击时不可被打断
                        {
                            skillData->setBreaked(false);
                        }
                            break;
                        case kTypeChangeActionIndex: // 切换动作序列
                        {
                            CCArray* tmp = split_string(talentData->getActionIndexGroup().c_str(),',');
                            CCArray* arrActionIdx = CCArray::create();
                            for (int m = 0; m < arrActionIdx->count() - 1; m++)
                            {
                                arrActionIdx->addObject(tmp->objectAtIndex(m));
                            }
                            skillData->setActionIndexArray(arrActionIdx);
                        }
                            break;
                        case kTypeAddHurtedNumber: // 伤害个数
                        {
                            CCArray* tmp = split_string(talentData->getHurtedNumberAddedValue().c_str(),',');
                            int nHurtedNumber = static_cast<CCString *>(tmp->objectAtIndex(0))->intValue();
                            skillData->setHurtNumber(nHurtedNumber);
                        }
                            break;
                        case kTypeDebuff:
                        {
                            skillData->setDebuffType(talentData->getDebuffType());
                            skillData->setDebuffDuration(talentData->getDebuffDuration());
                            skillData->setDebuffAddedValue(talentData->getDebuffAddedValue());
                        }
                            break;
                        case kTypeDOT:
                        {
                            skillData->setDOTType(talentData->getDOTType());
                            skillData->setDOTDuration(talentData->getDOTDuration());
                            skillData->setDOTDamagePoint(talentData->getDOTDamagePoint());
                        }
                            break;
                        case kTypeBeatBack: // 延长击退距离
                        {
                            CCArray* tmp = split_string(talentData->getHurtedNumberAddedValue().c_str(),',');
                            int nDistance = static_cast<CCString *>(tmp->objectAtIndex(0))->intValue();
                            nDistance = skillData->getBeatedBackDistance() + nDistance;
                            skillData->setBeatedBack(true);
                            skillData->setBeatedBackDistance(nDistance);
                        }
                            break;
                        default:
                            break;
                    }

                }
            }
            skillData->setTalentPoint(nSumTalentPoint); //设置技能所加天赋点数
        }
 
    } while (0);
}

#pragma mark 背包内物品装备

/**
 * 添加道具
 */
void GlobalData::addProperty(cocos2d::CCArray * array,bool isInTreasure)
{
    if(!array)
        return;
    int index = 0;
    Property *prop = NULL;
    if (isInTreasure)
    {
        prop = Property::create();
        m_pTreasureItemArray->addObject(prop);
    }
    else
    {
        // ID
        const char * ID = dynamic_cast<CCString *>(array->objectAtIndex(index++))->getCString();
        prop = static_cast<Property *>(m_pPropertyDict->objectForKey(ID));
        if (!prop)
        {
            prop = Property::create();
            prop->setID(ID);
            m_pPropertyDict->setObject(prop, ID);
            m_pPropArray->addObject(prop);
        }
        else
        {
//            prop->getDescriptionArray()->removeAllObjects();
        }
    }
    // 图标
    const char * iconName = dynamic_cast<CCString *>(array->objectAtIndex(index++))->getCString();
    std::string filePath = getFilePath(iconName,kDirTypeIcon,kFileSuffixPng);
    prop->setIconPath(filePath);
    // 品质
    int quality = dynamic_cast<CCString *>(array->objectAtIndex(index++))->intValue();
    prop->setQuality(quality);
    // 数量
    int num = dynamic_cast<CCString *>(array->objectAtIndex(index++))->intValue();
    prop->setNumber(num);
    // 编号
    int propIndex = dynamic_cast<CCString *>(array->objectAtIndex(index++))->intValue();
    prop->setIndex(propIndex);
    // 名称
    CCString * tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setName(tempstr->getCString());
    // 类型标签
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setLabel(tempstr->getCString());
    // 职业
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setProfessionIndex(tempstr->intValue());
    // 是否绑定
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setBind(tempstr->intValue());
    // 是否唯一
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setUnique(tempstr->intValue());
    // 等级
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setLevel(tempstr->intValue());
    // 描述
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setDescription(tempstr->getCString());
    // 售价
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setSalePrice(tempstr->intValue());
    // 限时
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index++));
    prop->setTimeLimit(tempstr->intValue());
    tempstr = dynamic_cast<CCString *>(array->objectAtIndex(index));

    if (getFirstNBitNumber(prop->getIndex(),3) == 501 && tempstr->intValue() == 1)
    {
        prop->setOpened(true);
    }
}


/**
 * 添加装备
 */
void GlobalData::addEquipment(cocos2d::CCArray * array,bool isInTreasure)
{
    do{
        CC_BREAK_IF(!array);
        //base information ---------
        CCString *baseInfo = dynamic_cast<CCString *>(array->objectAtIndex(0));
        CCArray *baseInfoArray = split_string(baseInfo->getCString(), ',');
        int index = 0;
        Equipment *equip = NULL;
        if (!isInTreasure)
        {
            // 判断是否是背包里的装备
            int pos = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
            // 装备ID
            const char * ID = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->getCString();
            equip = static_cast<Equipment *>(m_pEquipmentDict->objectForKey(ID));
            if (!equip)
            {
                equip = Equipment::create();
                equip->setID(ID);
                m_pEquipmentDict->setObject(equip, ID);
                if (pos == 1)
                {// 身上装备
                    m_pEquipOnTheBodyArray->addObject(equip);
                    equip->setPutOn(true);
                }
                else
                {
                    m_pEquipArray->addObject(equip);
                    equip->setPutOn(false);
                }
            }
        }else
        {
            equip = Equipment::create();
            m_pTreasureItemArray->addObject(equip);
        }
        // 编号(排序用)
        int equipIndex = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setIndex(equipIndex);
        
        int curLevel = equipIndex % 100;
        equip->setLevel(curLevel);
        
        //body part
        int part = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setPartIndex(part);
        
        //icon path
        std::string filePath = getFilePath(dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->getCString(),kDirTypeIcon,kFileSuffixPng);
        equip->setIconPath(filePath);
        
        //quality
        int quality = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setQuality(quality);
        
        // 锻造需求等级
        int level = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setComposeLevel(level);
        
        // 最大可强化等级
        int maxLevel = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setMaxLevel(maxLevel);
        
        CCString *detail0 = dynamic_cast<CCString *>(array->objectAtIndex(1));
        CCArray *arr0 = split_string(detail0->getCString(), ';');
        if (arr0)
        {
            int i = 0;
            const char * name = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
            equip->setEquipName(name);
            const char * type = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
            equip->setType(type);
            int pro = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setProfessionIndex(pro);
            int level = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setPlayerLevel(level);
            int combat = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setCombat(combat);
            int nIsCompose = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setIsCompose(nIsCompose);
            const char * title = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
            equip->setTitle(title);
        }
        
        CCString *detail1 = dynamic_cast<CCString *>(array->objectAtIndex(2));
        CCArray *arr1 = split_string(detail1->getCString(), ';');
        if (arr1)
        {
            int i = 0;
            int point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setStrenghPoint(point);
            point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setAgilityPoint(point);
            point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setIntelligencePoint(point);
            point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setEndurancePoint(point);
        }
        CCString *detail2 = dynamic_cast<CCString *>(array->objectAtIndex(3));
        CCArray *arr2 = split_string(detail2->getCString(), ';');
        if (arr2)
        {
            int i = 0;
            int point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setAP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setMAP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setSAP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setDP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setHP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setMP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setHitOdds(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setCriticalOdds(point);
        }
        CCString *detail3 = dynamic_cast<CCString *>(array->objectAtIndex(4));
        equip->setSalePrice(detail3->intValue());
        CCString *detail4 = dynamic_cast<CCString *>(array->objectAtIndex(5));
        equip->setDescription(detail4->getCString());
    }while (0);
}

/**
 * 背包内装备排序
 */
void GlobalData::sortEquip()
{
    if (m_pEquipArray->count() < 2)
    {
        return;
    }
    for (int i = 0; i < m_pEquipArray->count() - 1;i++)
    {
        Equipment *equip1 = (Equipment *)m_pEquipArray->objectAtIndex(i);
        if(!equip1)
        {
            continue;
        }
        for (int j = i + 1; j < m_pEquipArray->count();j++)
        {
            Equipment *equip2 = (Equipment *)m_pEquipArray->objectAtIndex(j);
            if(!equip2)
            {
                continue;
            }
            else if (equip2->getIndex() < equip1->getIndex())
            {
                m_pEquipArray->exchangeObject(equip1, equip2);
            }
        }
    }
}

void GlobalData::replaceEquip(cocos2d::CCArray *array)
{
    int elementIndex = 0;
    const char *decrepitID = dynamic_cast<CCString *>(array->objectAtIndex(elementIndex++))->getCString();
    Equipment *equip = static_cast<Equipment *>(m_pEquipmentDict->objectForKey(decrepitID));
    if (equip)
    {
        m_pEquipmentDict->removeObjectForKey(decrepitID);
        CCString *baseInfo = dynamic_cast<CCString *>(array->objectAtIndex(elementIndex++));
        CCArray *baseInfoArray = split_string(baseInfo->getCString(), ',');
        int index = 0;
        CCString *newID = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++));
        equip->setID(newID->getCString());
        m_pEquipmentDict->setObject(equip, newID->getCString());
        // 编号(排序用)
        int equipIndex = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setIndex(equipIndex);
        
        int curLevel = equipIndex % 100;
        equip->setLevel(curLevel);
        
        //body part
        int part = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setPartIndex(part);
        
        //icon name
        std::string filePath = getFilePath(dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->getCString(),kDirTypeIcon,kFileSuffixPng);
        equip->setIconPath(filePath);
        
        //quality
        int quality = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setQuality(quality);
        
        // 锻造需求等级
        int level = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setComposeLevel(level);
        
        // 最大可强化等级
        int maxLevel = dynamic_cast<CCString *>(baseInfoArray->objectAtIndex(index++))->intValue();
        equip->setMaxLevel(maxLevel);
        
        CCString *detail0 = dynamic_cast<CCString *>(array->objectAtIndex(elementIndex++));
        CCArray *arr0 = split_string(detail0->getCString(), ';');
        if (arr0)
        {
            int i = 0;
            const char * name = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
            equip->setEquipName(name);
            const char * type = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
            equip->setType(type);
            int pro = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setProfessionIndex(pro);
            int level = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setPlayerLevel(level);
            int combat = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setCombat(combat);
            int nIsCompose = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->intValue();
            equip->setIsCompose(nIsCompose);
            const char * title = dynamic_cast<CCString *>(arr0->objectAtIndex(i++))->getCString();
            equip->setTitle(title);
        }
        
        CCString *detail1 = dynamic_cast<CCString *>(array->objectAtIndex(elementIndex++));
        CCArray *arr1 = split_string(detail1->getCString(), ';');
        if (arr1)
        {
            int i = 0;
            int point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setStrenghPoint(point);
            point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setAgilityPoint(point);
            point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setIntelligencePoint(point);
            point = dynamic_cast<CCString *>(arr1->objectAtIndex(i++))->intValue();
            equip->setEndurancePoint(point);
        }
        CCString *detail2 = dynamic_cast<CCString *>(array->objectAtIndex(elementIndex++));
        CCArray *arr2 = split_string(detail2->getCString(), ';');
        if (arr2)
        {
            int i = 0;
            int point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setAP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setMAP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setSAP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setDP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setHP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setMP(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setHitOdds(point);
            point = dynamic_cast<CCString *>(arr2->objectAtIndex(i++))->intValue();
            equip->setCriticalOdds(point);
        }
        CCString *detail3 = dynamic_cast<CCString *>(array->objectAtIndex(elementIndex++));
        equip->setSalePrice(detail3->intValue());
        CCString *detail4 = dynamic_cast<CCString *>(array->objectAtIndex(elementIndex));
        equip->setDescription(detail4->getCString());
    }
}

/**
 * 背包内物品排序
 */
void GlobalData::sortProp()
{
    if (m_pPropArray->count() < 2)
    {
        return;
    }
    for (int i = 0; i < m_pPropArray->count() - 1;i++)
    {
        Property *prop1 = NULL;
        if (m_pPropArray->count() > i) {
            prop1 = (Property *)m_pPropArray->objectAtIndex(i);
        }
        if(!prop1)
        {
            continue;
        }
        for (int j = i + 1; j < m_pPropArray->count();j++)
        {
            Property *prop2 = (Property *)m_pPropArray->objectAtIndex(j);
            if(!prop2)
            {
                continue;
            }
            else if (prop2->getIndex() < prop1->getIndex())
            {
                m_pPropArray->exchangeObject(prop1, prop2);
            }
        }
    }
}

void GlobalData::itemConsume(cocos2d::CCArray *array, uint8_t itemType)
{
    CCArray * arr = split_string(static_cast<CCString *>(array->objectAtIndex(0))->getCString(), ',');
    const char * ID = static_cast<CCString *>(arr->objectAtIndex(0))->getCString();
    switch (itemType) {
        case 2:
            for (int i = 0; i < m_pEquipOnTheBodyArray->count(); i++)
            {
                Equipment *equip = (Equipment *)m_pEquipOnTheBodyArray->objectAtIndex(i);
                if (equip->getID().compare(ID) == 0)
                {
                    m_pEquipOnTheBodyArray->removeObject(equip);
                    m_pEquipmentDict->removeObjectForKey(equip->getID());
                    break;
                }
            }
            break;
        case 1:
            for (int i = 0; i < m_pEquipArray->count(); i++)
            {
                Equipment *equip = (Equipment *)m_pEquipArray->objectAtIndex(i);
                if (equip->getID().compare(ID) == 0)
                {
                    m_pEquipArray->removeObject(equip);
                    m_pEquipmentDict->removeObjectForKey(equip->getID());
                    break;
                }
            }
            break;
        case 0:
        {
            int num = static_cast<CCString *>(arr->objectAtIndex(1))->intValue();
            for (int i = 0; i < m_pPropArray->count(); i++)
            {
                Property *prop = (Property *)m_pPropArray->objectAtIndex(i);
                if (prop->getID().compare(ID) == 0)
                {
                    if (num > 0)
                    {
                        prop->setNumber(num);
                    }
                    else
                    {
                        m_pPropArray->removeObject(prop);
                        m_pPropertyDict->removeObjectForKey(prop->getID());
                        sortProp();
                    }
                    break;
                }
            }
        }
            break;
        default:
            break;
    }
}

void GlobalData::clean()
{
    if(m_pEquipOnTheBodyArray)
    {
        m_pEquipOnTheBodyArray->removeAllObjects();
    }
    if(m_pEquipArray)
    {
        m_pEquipArray->removeAllObjects();
    }
    if(m_pPropArray)
    {
        m_pPropArray->removeAllObjects();
    }
    if(m_pTaskDataDict)
    {
        m_pTaskDataDict->removeAllObjects();
    }
    if (m_pTaskDataArray)
    {
        m_pTaskDataArray->removeAllObjects();
    }
    if(m_pEquipmentDict)
    {
        m_pEquipmentDict->removeAllObjects();
    }
    
    if (m_pUsableGestureSkillDataArray)
    {
        m_pUsableGestureSkillDataArray->removeAllObjects();
    }
    for (int i = 0; i < 3; i++)
    {
        if (m_pGestureSkillDataArray[i])
        {
            m_pGestureSkillDataArray[i]->removeAllObjects();
        }
    }
    if(m_pPlayerSkillDataArray)
    {
        m_pPlayerSkillDataArray->removeAllObjects();
    }
    if (m_pTalentDataArrayDict) //切换到封面重新创建技能天赋数据
    {
        m_pTalentDataArrayDict->removeAllObjects();
        createSkillTalentArrData();
    }
    
    
}
