//
//  YYDungeonsUI.cpp
//  Fairyland
//
//  Created by yoyo on 14-4-16.
//
//

#include "SimpleAudioEngine.h"
#include "YYPopUpUI.h"
#include "YYController.h"
#include "../common/Common.h"
#include "YYControlUILayer.h"
#include "YYScene.h"
#include "YYLayer.h"
#include "YYButton.h"
#include "YYDragRegion.h"
#include "YYSpriteLayer.h"
#include "YYPlayer.h"
#include "YYSpriteAttribute.h"
#include "YYTaskTargetData.h"
#include "YYShopData.h"
#include "YYProtossData.h"
#include "YYLoginAwardData.h"
#include "YYTalentData.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CocosDenshion;
using namespace gui;

#define TAG_RETURN_CITY 300 //回城
#define TAG_ENTER_STAGE_BUTTON 301// 确定进入关卡按键
#define TAG_CLOSE_STAGE_UI 302// 关闭场景UI的图标

#define DUNGEONS_FLAG_ANU getFilePath("282",kDirTypePic,kFileSuffixAnu)
#define DUNGEONS_FLAG_PNG getFilePath("282",kDirTypePic,kFileSuffixPng)
#define DUNGEONS_NAME_BOX getFilePath("43", kDirTypePic, kFileSuffixPng)// 副本名称框
#define TURNTABLE getFilePath("233", kDirTypePic, kFileSuffixPng)//转盘

#pragma mark 副本界面

const static int flagPosition[9][2]={/*{170,106},{312,147},*/ {350,280}, {483,355},{581,494},{730,482},{886,408},{1041,481},{1192,493},{1290,355},{1431,288}/*,{1459,145},{1602,105}*/};//转盘上旗子的相对位置 

// 副本图片资源切块
#define CLIP_FLAG_GRAY 0//灰旗
#define CLIP_FLAG_ORANGE 1// 橙色旗子
#define CLIP_FLAG_BLUE 2
#define CLIP_FLAG_VIOLET 3
#define CLIP_FLAG_RED 4
#define CLIP_FLAG_GREEN 5
#define CLIP_START_GOLDEN_BIG 6
#define CLIP_START_GRAY_BIG 7
#define CLIP_START_GOLDEN_SMALL 8
#define CLIP_START_GRAY_SMALL 9
#define CLIP_FLAG_GRAY_WITHOUT_BASE 10
#define CLIP_FLAG_RED_WITHOUT_BASE 11
#define CLIP_GOLDEN_EGG_SHINY 12
#define CLIP_FLAG_BASE 13// 旗子底座

// 战斗结果动画UI Tag
#define TAG_FIRST_ACTIVE_ITEM 1

void PopUpUI::initDungeonUI(cocos2d::CCArray * array)
{
    CCLOG("initDungeonUI");
    if(Controller::getInstance()->getGameStatus() == kStatusUI && getUIType() != kUIDungeons)
    {
        exitUI();
    }
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIDungeons);
    m_u8DungeonAmount = ((CCString *)array->objectAtIndex(0))->intValue();
    Controller::getInstance()->setLoadingOverStatus(kLOSDefault);
    m_pDungeonsDataArray = ((YYScene *)getParent())->getDungeonsDataArray();
    if (!m_pDungeonsDataArray)
    {
        m_pDungeonsDataArray = new CCArray();
        ((YYScene *)getParent())->setDungeonsDataArray(m_pDungeonsDataArray);
    }
    else
    {
        m_pDungeonsDataArray->removeAllObjects();
    }
    m_pDungeonsDataArray->addObjectsFromArray(array);
    m_pDungeonsDataArray->removeObjectAtIndex(0);//第一个数据为副本总数，其他为已解锁的副本数据
    
    m_pIsChoosedDungeon = ((YYScene *)getParent())->getIsChoosedDungeon();
    if (!m_pIsChoosedDungeon)
    {
        m_pIsChoosedDungeon = new bool[m_u8DungeonAmount];
        ((YYScene *)getParent())->setIsChoosedDungeon(m_pIsChoosedDungeon);
    }
    for (int i = 1; i < m_u8DungeonAmount; i++)
    {
        m_pIsChoosedDungeon[i] = false;
    }
    //默认选中第一个副本
    m_u8DungeonsIndex = 0;
    ((YYScene *)getParent())->setDungeonsIndex(m_u8DungeonsIndex);
    m_pIsChoosedDungeon[0] = true;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    if (winSize.width / winSize.height > (float)STANDARD_SCENE_WIDTH / STANDARD_SCENE_HEIGHT)
    {
        if (winSize.width / winSize.height > 1.7)
        {
            m_fMaxAngle = 24;
        }
        else
        {
            m_fMaxAngle = 25;
        }
    }
    else
    {
        m_fMaxAngle = 26.5;
    }
    
    m_fRotateAngle = m_fMaxAngle;//转盘转至最左边
    string str = getFilePath("42", kDirTypePic, kFileSuffixPng);
    CCSprite * bgSprite_0 = CCSprite::create(str.c_str());
    bgSprite_0->setPosition(ccp(winSize.width / 2,winSize.height / 2));
    bgSprite_0->setScaleX(winSize.width / bgSprite_0->getContentSize().width);
    bgSprite_0->setScaleY(winSize.height / bgSprite_0->getContentSize().height);
    addChild(bgSprite_0);
    
    m_pTurntable = CCSprite::create(TURNTABLE.c_str());
    addChild(m_pTurntable);
    
    m_nTurntableOffsetY = 145;//转盘图片中心点相对于屏幕底边的偏移量
    CCPoint initPosition = ccp(winSize.width / 2, m_nTurntableOffsetY);
    m_pTurntable->setRotation(m_fRotateAngle);
    m_pTurntable->setPosition(initPosition);
    
    //副本名称框
    m_pDungeonsTitleFrame = cocos2d::extension::CCScale9Sprite::create(DUNGEONS_NAME_BOX.c_str());
    m_pDungeonsTitleFrame->setPosition(ccp(10, winSize.height - 10));
    m_pDungeonsTitleFrame->setAnchorPoint(ccp(0, 1));
    addChild(m_pDungeonsTitleFrame);
    
    //图片资源切片
    ActionSet * actionSet = ActionSet::createActionSetWithFile(DUNGEONS_FLAG_ANU);
    m_pTurntableBatchNode = CCSpriteBatchNode::create(DUNGEONS_FLAG_PNG.c_str(), 70);
    addChild(m_pTurntableBatchNode);
    
    for (int i = 0; i < m_u8DungeonAmount; i++)
    {
        //转盘上每个地标相对于转盘图片左下角的坐标
        CCPoint point = ccp(flagPosition[i][0],flagPosition[i][1]);
        CCPoint point1 = ccpAdd(point, ccp((winSize.width - m_pTurntable->getContentSize().width) / 2,m_nTurntableOffsetY - m_pTurntable->getContentSize().height / 2 ));
        if (i < m_pDungeonsDataArray->count())
        {//已解锁的副本
            CCArray * array1 = split_string(((CCString *)m_pDungeonsDataArray->objectAtIndex(i))->getCString(), ';');
            //彩旗
            int8_t flagIndex = ((CCString *)array1->objectAtIndex(1))->intValue();
            YYRect *rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(flagIndex);
            CCRect rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
            YYButton * flag = YYButton::create();
            flag->initWithFile(DUNGEONS_FLAG_PNG.c_str(), rect1);
            flag->setAnchorPoint(ccp(0.5,0));
            flag->setPosition(point1 + ccp(0, -25));
            flag->setIndex(i);
            flag->setButtonRect(CCRectMake(point1.x - rect->getWidth() / 2, point1.y - 25, rect->getWidth(), rect->getHeight()));
            m_pDungeonsButtonDictionary->setObject(flag, i);
            m_pTurntableBatchNode->addChild(flag,0,i);
            
            //底座
            rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_FLAG_BASE);
            rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
            CCSprite * flagBase = CCSprite::create();
            flagBase->initWithFile(DUNGEONS_FLAG_PNG.c_str(), rect1);
            flagBase->setAnchorPoint(ccp(0.5,1));
            flagBase->setPosition(point1);
            m_pTurntableBatchNode->addChild(flagBase,0,m_u8DungeonAmount + i);
            
            CCSprite * sprite = NULL;
            
            //选中副本
            if(m_pIsChoosedDungeon[i])
            {
                //显示副本名
                m_pDungeonsTitelLabel = CCLabelTTF::create(((CCString *)array1->objectAtIndex(0))->getCString(), DEFAULT_FONT_NAME, 40);
                m_pDungeonsTitleFrame->setPreferredSize(CCSize(m_pDungeonsTitelLabel->getContentSize().width + 40,m_pDungeonsTitelLabel->getContentSize().height + 30));
                m_pDungeonsTitelLabel->setPosition(ccp(10 + m_pDungeonsTitleFrame->getContentSize().width / 2,winSize.height - 10 - m_pDungeonsTitleFrame->getContentSize().height / 2));
                addChild(m_pDungeonsTitelLabel);
                
                //发光效果
                rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_GOLDEN_EGG_SHINY);
                rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
                sprite = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(), rect1);
                sprite->setAnchorPoint(ccp(0.5,0));
                sprite->setPosition(point1 - ccp(0, 20));
                m_pTurntableBatchNode->addChild(sprite, 1, m_u8DungeonAmount * 2 + i);
            }
            
            //得分
            int score = ((CCString *)array1->objectAtIndex(2))->intValue();
            for (int j = 0; j < 3; j++)
            {
                if (j < score)
                {// 小金星
                    rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GOLDEN_SMALL);
                    rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
                    sprite = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(), rect1);
                }
                else
                {// 小灰星
                    rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GRAY_SMALL);
                    rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
                    sprite = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(), rect1);
                }
                m_pTurntableBatchNode->addChild(sprite,0,m_u8DungeonAmount * 3 + i * 3 + j);
                sprite->setPosition(ccpAdd(point1,ccp(-25 + j * 25, 95)));
            }
        }
        else
        {//未解锁的副本
            //灰旗
            YYRect * rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_FLAG_GRAY);
            CCRect rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
            CCSprite * flag = YYButton::create();
            flag->initWithFile(DUNGEONS_FLAG_PNG.c_str(), rect1);
            flag->setAnchorPoint(ccp(0.5,0));
            flag->setPosition(point1);
            m_pTurntableBatchNode->addChild(flag,0,i);
            
            //底座
            rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_FLAG_BASE);
            rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
            CCSprite * flagBase = YYSprite::create();
            flagBase->initWithFile(DUNGEONS_FLAG_PNG.c_str(), rect1);
            flagBase->setAnchorPoint(ccp(0.5,1));
            flagBase->setPosition(point1);
            m_pTurntableBatchNode->addChild(flagBase,0,m_u8DungeonAmount + i);
            
            //小灰星星
            for (int j = 0; j < 3; j++)
            {
                rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GRAY_SMALL);
                rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
                CCSprite * sprite = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(), rect1);
                m_pTurntableBatchNode->addChild(sprite,0,m_u8DungeonAmount * 3 + i * 3 + j);
                sprite->setPosition(ccpAdd(point1,ccp(-25 + j * 25, 95)));
            }
        }
    }
    updateDungeonsUIElements();
    
    //返回
    str = getFilePath("286", kDirTypeUI, kFileSuffixPng);
    CCSprite *sprite = CCSprite::create(str.c_str());
    sprite->setPosition(ccp(winSize.width / 2,winSize.height - 60));
    sprite->setTag(TAG_RETURN_CITY);
    addChild(sprite);
}

void PopUpUI::updateDungeonsUIElements()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint initPosition = ccp(winSize.width / 2, m_nTurntableOffsetY);
    float w = m_pTurntable->getContentSize().width / 2;
    float h = m_pTurntable->getContentSize().height;
    float radius = (w * w + h * h)/(2 * h);//转盘半径
    m_pTurntable->setRotation(m_fRotateAngle);
    float radian = m_fRotateAngle != 0 ? M_PI * m_fRotateAngle / 180 : 0;//旋转弧度
    float distance = radius - h / 2;//图片中心点到圆心的距离
    CCPoint offset = ccp((distance) * sin(radian),distance * cos(radian) - distance);//转盘旋转后图片中心点的偏移量
    m_pTurntable->setPosition(ccpAdd(initPosition,offset));
    //圆心相对图片左下角坐标
    float centerX = w;
    float centerY = h - radius;
    
    for (int i = 0; i < m_u8DungeonAmount; i++)
    {
        //转盘上每个地标相对于转盘图片左下角的坐标
        float x = flagPosition[i][0];
        float y = flagPosition[i][1];
        CCPoint point = ccp(x,y);
        if (m_fRotateAngle != 0)
        {
            float dis = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));//每个地标到圆心的距离
            float currentRadian = asinf((y - centerY) / dis);
            if (centerX > x)
            {
                currentRadian = M_PI - currentRadian;
            }
            float x1 = cos(currentRadian - radian) * dis;
            float y1 = sin(currentRadian - radian) * dis;
            point = ccp((centerX + x1),(centerY + y1));//旋转后每个地标相对于转盘图片左下角的坐标
        }
        CCPoint point1 = ccpAdd(point, ccp((winSize.width - m_pTurntable->getContentSize().width) / 2,m_nTurntableOffsetY - m_pTurntable->getContentSize().height / 2 ));
        
        //旗
        CCSprite * flag = (CCSprite *)m_pTurntableBatchNode->getChildByTag(i);
        YYButton * button = dynamic_cast<YYButton *>(flag);
        if (button)
        {
            float x = point1.x - button->getButtonRect().size.width / 2;
            float y = point1.y - 25;
            button->setButtonRect(CCRectMake(x, y, button->getButtonRect().size.width, button->getButtonRect().size.height));
            button->setPosition(point1 + ccp(0,-25));
        }
        
        //旗底座
        CCSprite * flagBase = (CCSprite *)m_pTurntableBatchNode->getChildByTag(m_u8DungeonAmount + i);
        if (flagBase)
        {
            flagBase->setPosition(point1);
        }
        
        
        //发光效果
        CCSprite *sprite = (CCSprite *)m_pTurntableBatchNode->getChildByTag(m_u8DungeonAmount * 2 + i);
        if (sprite)
        {
            if(m_pIsChoosedDungeon[i])
            {
                sprite->setPosition(point1 - ccp(0, 20));
            }
        }
        
        //小星星
        for (int j = 0; j < 3; j++)
        {
            sprite = (CCSprite *)m_pTurntableBatchNode->getChildByTag(m_u8DungeonAmount * 3 + i * 3 + j);
            if (sprite)
            {
                sprite->setPosition(ccpAdd(point1,ccp(-25 + j * 25, 95)));
            }
        }
    }
}

void PopUpUI::updateDungeonsUI()
{
    if(m_pRendDungeonActionArray)
    {
        for(int i=0; i < m_pRendDungeonActionArray->count();++i)
        {
            YYSprite* pSprite = (YYSprite*)m_pRendDungeonActionArray->objectAtIndex(i);
            pSprite->update();
        }
    }
}

void PopUpUI::dungeonsUITouchHandle()
{
    if (m_bTouchBegan)
    {
        m_bTouchBegan = false;
        if (getUIType() == kUIDungeons)
        {
            /* 返回主城 */
            YYSprite * returnButton = (YYSprite *)getChildByTag(TAG_RETURN_CITY);
            if (Controller::getInstance()->isButtonTouched(returnButton, m_CurTouchPoint))
            {
                this->removeAllChildren();
                setUIType(kUIDefaultStatus);
                Controller::getInstance()->setGameStatus(kStatusScene);
                return;
            }
            CCArray * array = m_pDungeonsButtonDictionary->allKeys();
            for (int i = 0; i < array->count(); i++)
            {
                int key = ((CCInteger *)array->objectAtIndex(i))->getValue();
                YYButton * button = (YYButton *)m_pDungeonsButtonDictionary->objectForKey(key);
                if (button->getButtonRect().containsPoint(m_CurTouchPoint))
                {
                    if (!m_pIsChoosedDungeon[button->getIndex()])
                    {
                        ((YYScene *)getParent())->setDungeonsIndex(button->getIndex());
                        setChoosedEffect();
                    }
                    sendChoosedDungeonsIndexMessage();
                    break;
                }
            }
        }
        else if(getUIType() == kUIStageAwardInfo)
        {
            CCSprite * closeButton = (CCSprite *)getChildByTag(TAG_CLOSE_STAGE_UI);
            if (Controller::getInstance()->isButtonTouched(closeButton, m_CurTouchPoint))
            {
                m_bCloseChoosed = true;// 点中退出
                return;
            }
            for (int i = 0; i < m_u8UnlockDifficultOptions; i++)
            {
                if (m_pDifficultButton[i]->getButtonRect().containsPoint(m_CurTouchPoint))
                {
                    m_u8ChoosedDifficultOptionIndex = ((YYScene *)getParent())->getChoosedStageDifficultIndex();
                    m_pDifficultLabel[m_u8ChoosedDifficultOptionIndex]->setColor(ccWHITE);
                    m_u8ChoosedDifficultOptionIndex = i;
                    ((YYScene *)getParent())->setChoosedDifficultOptionIndex(m_u8ChoosedDifficultOptionIndex);
                    m_pDifficultLabel[i]->setColor(ccYELLOW);
                    //                    updateStageAward();
                    break;
                }
            }
            YYSprite * enterButton = (YYSprite *)getChildByTag(TAG_ENTER_STAGE_BUTTON);
            if (Controller::getInstance()->isButtonTouched(enterButton, m_CurTouchPoint))
            {
                m_bEnter = true;
                string str = getFilePath("65", kDirTypePic, kFileSuffixPng);
                enterButton->setTexture(CCTextureCache::sharedTextureCache()->addImage(str.c_str()));
            }
        }
    }
    if (m_bTouchMoved)
    {
        m_bTouchMoved = false;
        m_fRotateAngle += (m_CurTouchPoint.x - m_PreTouchPoint.x) / 4;
        if (m_fRotateAngle > m_fMaxAngle)
        {
            m_fRotateAngle = m_fMaxAngle;
        }
        if (m_fRotateAngle < -m_fMaxAngle)
        {
            m_fRotateAngle = -m_fMaxAngle;
        }
        updateDungeonsUIElements();
    }
    if (m_bTouchEnded)
    {
        m_bTouchEnded = false;
        if (m_bCloseChoosed)
        {
            m_bCloseChoosed = false;
        }
        if (m_bEnter && getChildByTag(TAG_ENTER_STAGE_BUTTON))
        {
            m_bEnter = false;
            YYSprite * enterButton = (YYSprite *)getChildByTag(TAG_ENTER_STAGE_BUTTON);
            if (Controller::getInstance()->isButtonTouched(enterButton, m_CurTouchPoint))
            {
                sendChoosedStageConfirmMessage();
                ((YYScene *)getParent())->setStageDamage(0);
                ((YYScene *)getParent())->setStageDuration(0);
                ((YYScene *)getParent())->setHPDecrement(0);
                ((YYScene *)getParent())->setMaxComboCount(0);
                ((YYScene *)getParent())->setHittedAmount(0);
                ((YYScene *)getParent())->setKillCount(0);
            }
            string str = getFilePath("64", kDirTypePic, kFileSuffixPng);
            enterButton->setTexture(CCTextureCache::sharedTextureCache()->addImage(str.c_str()));
        }
    }
}
void PopUpUI::setChoosedEffect()
{
    m_u8DungeonsIndex = ((YYScene *)getParent())->getDungeonsIndex();
    //更新发光效果
    for (int i = 0; i < m_pDungeonsDataArray->count(); i++)
    {
        if (m_pIsChoosedDungeon[i])
        {
            m_pIsChoosedDungeon[i] = false;
            //删除之前的选中光效
            m_pTurntableBatchNode->removeChildByTag(m_u8DungeonAmount * 2 + i);
            //获取被选中的旗子底座的位置
            CCPoint position = m_pTurntableBatchNode->getChildByTag(m_u8DungeonAmount + m_u8DungeonsIndex)->getPosition();//底盘
            ActionSet * actionSet = ActionSet::createActionSetWithFile(DUNGEONS_FLAG_ANU);
            YYRect *rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_GOLDEN_EGG_SHINY);
            CCRect rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
            CCSprite *sprite = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(), rect1);
            sprite->setAnchorPoint(ccp(0.5,0));
            sprite->setPosition(position - ccp(0, 20));
            m_pTurntableBatchNode->addChild(sprite,1,m_u8DungeonAmount * 2 + m_u8DungeonsIndex);
            break;
        }
    }

    m_pIsChoosedDungeon[m_u8DungeonsIndex] = true;
    //更新显示的副本名称
    CCArray * array1 = split_string(((CCString *)m_pDungeonsDataArray->objectAtIndex(m_u8DungeonsIndex))->getCString(), ';');
    m_pDungeonsTitelLabel->setString(((CCString *)array1->objectAtIndex(0))->getCString());
    m_pDungeonsTitleFrame->setPreferredSize(CCSize(m_pDungeonsTitelLabel->getContentSize().width + 40,m_pDungeonsTitelLabel->getContentSize().height + 30));
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    m_pDungeonsTitelLabel->setPosition(ccp(10 + m_pDungeonsTitleFrame->getContentSize().width / 2,winSize.height - 10 - m_pDungeonsTitleFrame->getContentSize().height / 2));
}

void PopUpUI::sendChoosedDungeonsIndexMessage()
{
    m_u8DungeonsIndex = ((YYScene *)getParent())->getDungeonsIndex();
    const char *temp = CCString::createWithFormat(SEND_DUNGEONS_INDEX,m_u8DungeonsIndex)->getCString();
    Controller::getInstance()->sendMessage(temp,false);
}

void PopUpUI::sendChoosedStageConfirmMessage()
{
    m_u8DungeonsIndex = ((YYScene *)getParent())->getDungeonsIndex();
    m_u8ChoosedStageIndex = ((YYScene *)getParent())->getChoosedStageIndex();
    m_u8ChoosedDifficultOptionIndex = ((YYScene *)getParent())->getChoosedStageDifficultIndex();
    const char *temp = CCString::createWithFormat(SEND_DIFFCULTY,m_u8DungeonsIndex,m_u8ChoosedStageIndex,m_u8ChoosedDifficultOptionIndex)->getCString();
    Controller::getInstance()->sendMessage(temp,false);
    Controller::getInstance()->setDungeonType(kDungeonNormal); //设置进入副本类型
}

#pragma mark  副本关卡选择界面
void PopUpUI::initStageUI(cocos2d::CCArray * array)
{
    do{
        Controller::getInstance()->setGameStatus(kStatusUI);
        setUIType(kUIStage);
        m_pRoleByControlled = ((YYScene *)getParent())->getControlUILayer()->getRoleByControlled();
        if (m_pUITouchGroup)
        {
            removeChild(m_pUITouchGroup);
            m_pUITouchGroup = NULL;
        }
        m_pUITouchGroup = TouchGroup::create();
        CC_BREAK_IF(!m_pUITouchGroup);
        addChild(m_pUITouchGroup);
        m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
        std::string filePath = STAGE_UI_RESOURCE_FILE_NAME;
        m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
        m_pStageLevel = NULL;
        m_nStageNum = 0;
        //退出按键
        UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        CC_BREAK_IF(!panel0);
        UIButton* close_button = static_cast<UIButton*>(panel0->getChildByName("Button_closed"));
        CC_BREAK_IF(!close_button);
        close_button->addTouchEventListener(this, toucheventselector(PopUpUI::exitStageCallBack));
        
        
        ImageView *imageView0 = static_cast<ImageView *>(panel0->getChildByName("ImageView0"));
        CC_BREAK_IF(!imageView0);
        Label *titleLabel = static_cast<Label *>(imageView0->getChildByName("TextArea"));
        CC_BREAK_IF(!titleLabel);
        titleLabel->setText(static_cast<CCString *>(array->objectAtIndex(0))->getCString());
        
        int stageNum = static_cast<CCString *>(array->objectAtIndex(1))->intValue();
        UIPanel* panel1 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel1"));
        CC_BREAK_IF(!panel1);
        ScrollView *scrollView = static_cast<ScrollView *>(panel1->getChildByName("ScrollView"));
        CC_BREAK_IF(!scrollView);
        
        m_pStageDataArray = ((YYScene *)getParent())->getStageDataArray();
        if (!m_pStageDataArray)
        {
            m_pStageDataArray = new CCArray();
            ((YYScene *)getParent())->setStageDataArray(m_pStageDataArray);
        }
        else
        {
            m_pStageDataArray->removeAllObjects();
        }
        m_pStageDataArray->addObjectsFromArray(array);
        m_pStageDataArray->removeObjectAtIndex(0);//第一个数据为副本名称
        m_pStageDataArray->removeObjectAtIndex(0);//第二个数据为关卡总数，其他为已解锁的关卡数据
        
        //ImageView *imageViewPic = static_cast<ImageView *>(scrollView->getChildByName(CCString::createWithFormat("ImageView_0")->getCString()));
        //scrollView->setInnerContainerSize(CCSizeMake(2 * imageViewPic->getContentSize().width, stageNum / 2 + stageNum % 2 * imageViewPic->getContentSize().height));
        for (int i = 0; i < scrollView->getChildrenCount(); i++)
        {
            ImageView *imageView = static_cast<ImageView *>(scrollView->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
            if (!imageView) {
                continue;
            }
            if(i < array->count() - 2)
            {
                if (i == 0)
                {
                    m_nStageNum = array->count() - 2;
                    m_pStageLevel = (int *)malloc(sizeof(int) * m_nStageNum);
                }
                imageView->setTag(i);
                imageView->addTouchEventListener(this, toucheventselector(PopUpUI::stageCallBack));
                CCArray *stageDataArray = split_string(static_cast<CCString *>(array->objectAtIndex(2 + i))->getCString(), ';');
                imageView->setEnabled(true);
                const char * stage = static_cast<CCString *>(stageDataArray->objectAtIndex(0))->getCString();
                const char * level = static_cast<CCString *>(stageDataArray->objectAtIndex(1))->getCString();
                m_pStageLevel[i] = atoi(level);
                Label *stageLabel = static_cast<Label *>(imageView->getChildByName("Label0"));
                stageLabel->setText(stage);
                Label *levelLabel = static_cast<Label *>(imageView->getChildByName("Label1"));
                Label *levelValue = static_cast<Label *>(levelLabel->getChildByName("Label_0"));
                levelValue->setText(level);
                
                //怪物资源
                std::string filePath = getFilePath("601-1", kDirTypeUI, kFileSuffixPng);
                CCSprite *pSriteMosterBg = CCSprite::create(filePath.c_str());
                pSriteMosterBg->setAnchorPoint(ccp(0.5, 0.5));
                pSriteMosterBg->setPosition(ccp(imageView->getContentSize().width / 4 - 5,  -10));
                imageView->addNode(pSriteMosterBg);
                
                const char * resourceFileName = ((CCString *)stageDataArray->objectAtIndex(2))->getCString();
                YYSprite * sprite = YYSprite::create();
                CCString *pStrIcon = CCString::createWithFormat("%s", resourceFileName);
                std::string strIcon = getFilePath(pStrIcon->m_sString, kDirTypeIcon, kFileSuffixPng);
                sprite->initWithFile(strIcon.c_str());
                sprite->setAnchorPoint(ccp(0.5, 0.5));
                sprite->setPosition(ccp(pSriteMosterBg->getContentSize().width / 2,  pSriteMosterBg->getContentSize().height / 2));
                pSriteMosterBg->addChild(sprite);
                
                int score = ((CCString *)stageDataArray->lastObject())->intValue();            //得分
                //图片资源切片
                ActionSet * actionSet = ActionSet::createActionSetWithFile(DUNGEONS_FLAG_ANU);
                YYRect * rect = NULL;
                for (int m = 0 ; m < 3; m++)
                {
                    if (m < score)
                    {
                        rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GOLDEN_BIG);//大星星(黄)
                    }
                    else
                    {
                        rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GRAY_BIG);//大星星(灰)
                    }
                    
                    CCSprite * sprite = CCSprite::create(DUNGEONS_FLAG_PNG.c_str() , CCRect(rect->getX(),rect->getY(),rect->getWidth(),rect->getHeight()));
                    sprite->setPosition(ccp(sprite->getContentSize().width * m - 3 * imageView->getContentSize().height / 4 + 20, -imageView->getContentSize().height / 4));
                    sprite->setAnchorPoint(ccp(0,0.5));
                    imageView->addNode(sprite);
                }
                CCArray * array = GlobalData::getInstance()->getTaskDataArray();
                for (int n = 0; n < array->count(); n++)
                {
                    TaskData * taskData = ((TaskData *)array->objectAtIndex(n));
                    if(taskData->getTargetType() == kTaskKillMonster || taskData->getTargetType() == kTaskCollect || taskData->getTargetType() == kTaskPassStage)
                    {
                        for (int j = 0; j < taskData->getTargetArray()->count(); j++)
                        {
                            TaskTarget *taskTarget = (TaskTarget *)taskData->getTargetArray()->objectAtIndex(j);
                            if(((YYScene *)getParent())->getSpriteLayer()->isTaskStage(taskTarget,i))
                            {
                                YYSprite *sprite = YYSprite::createWithActionSet("se25", "se25", kDirTypeEffect);
                                sprite->setAction(0, true);
                                sprite->setAnchorPoint(ccp(0.5, 0));
                                sprite->setPosition(ccp(0, 30));
                                imageView->addNode(sprite);
                                getRendDungeonActionArray()->addObject(sprite);
                                break;
                            }
                        }
                    }
                }
                ImageView *lockImage = static_cast<ImageView *>(imageView->getChildByName("ImageView0"));
                lockImage->setVisible(false);
            }
            else if (i < stageNum)
            {
                ImageView *lockImage = static_cast<ImageView *>(imageView->getChildByName("ImageView0"));
                lockImage->setVisible(true);
            }
            else
            {
                imageView->setEnabled(false);
            }
        }
    }while (0);
}

void PopUpUI::stageCallBack(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if(type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIImageView *pImageView = (UIImageView *)pSender;
        if (m_nStageNum > pImageView->getTag())
        {
            int nLevel = ((YYScene *)getParent())->getControlUILayer()->getRoleByControlled()->getLevel();
            int nState = nLevel - m_pStageLevel[pImageView->getTag()];
            if (nState < 0)
            {
                ((YYScene*)getParent())->showTips(g_sStateUILevelTip);
                return;
            }
        }
        ImageView *stageOption = static_cast<ImageView *>(pSender);
        ScrollView *scrollView = static_cast<ScrollView *>(stageOption->getParent());
        for (int i = 0; i < scrollView->getChildrenCount(); i++)
        {
            ImageView *imageView = static_cast<ImageView *>(scrollView->getChildByName(CCString::createWithFormat("ImageView_%d",i)->getCString()));
            if (strcmp(stageOption->getName(),imageView->getName()) == 0)
            {
                m_u8ChoosedStageIndex = i;
                ((YYScene *)getParent())->setChoosedStageIndex(m_u8ChoosedStageIndex);
                sendChoosedStageIndexMessage(((YYScene *)getParent())->getDungeonsIndex(),i);
                CC_SAFE_FREE(m_pStageLevel);
                break;
            }
        }
    }
}
void PopUpUI::exitStageCallBack(cocos2d::CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
        m_pRendDungeonActionArray->removeAllObjects();
        CC_SAFE_FREE(m_pStageLevel);
        setUIType(kUIDungeons);
    }
}
void PopUpUI::sendChoosedStageIndexMessage(int dungeonsIndex,int stageIndex)
{
    const char *temp = CCString::createWithFormat(SEND_STAGE_INDEX,dungeonsIndex,stageIndex)->getCString();
    Controller::getInstance()->sendMessage(temp,false);
}

#pragma mark 关卡奖励界面

#define difficultLabel g_sDiffculty //"普通","困难","噩梦"};

#define TAG_AWARD_ICON 1
#define TAG_AWARD_NUM 10

void PopUpUI::initStageAwardUI(cocos2d::CCArray * array)
{
    if (!array)
        return;
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIStageAwardInfo);
    m_pRoleByControlled = ((YYScene *)getParent())->getControlUILayer()->getRoleByControlled();
    if (m_pUITouchGroup)
    {
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
    }
    m_pUITouchGroup = TouchGroup::create();
    CC_ASSERT(m_pUITouchGroup);
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = STAGE_AWARD_UI_RESOURCE_FILE_NAME;
    m_pUITouchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    //退出按键
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *pButton_close = static_cast<UIButton*>(panel0->getChildByName("Button_closed"));
    CC_ASSERT(pButton_close);
    pButton_close->addTouchEventListener(this, toucheventselector(PopUpUI::exitStageCallBack));
    
    UIImageView *pImageView0 = static_cast<UIImageView *>(panel0->getChildByName("ImageView0"));
    CC_ASSERT(pImageView0);
    UILabel *pLabel_name = static_cast<UILabel *>(pImageView0->getChildByName("TextArea"));
    CC_ASSERT(pLabel_name);
    // 关卡名字
    m_u8ChoosedStageIndex = ((YYScene *)getParent())->getChoosedStageIndex();
    CCArray *tempArray = split_string(((CCString *)m_pStageDataArray->objectAtIndex(m_u8ChoosedStageIndex))->getCString(),';');
    const char * stageName = ((CCString *)tempArray->objectAtIndex(0))->getCString();
    pLabel_name->setText(stageName);
    
    UIPanel* panel1 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    CC_ASSERT(panel1);
    
    // 立即进入
    UIButton *pButton_enter = static_cast<UIButton*>(panel1->getChildByName("Button_0"));
    CC_ASSERT(pButton_enter);
    pButton_enter->addTouchEventListener(this, toucheventselector(PopUpUI::enterDungeonCallBack));
    
    // 扫荡
    UIButton *pButton_wipe = static_cast<UIButton *>(panel1->getChildByName("Button_1"));
    CC_ASSERT(pButton_wipe);
    pButton_wipe->addTouchEventListener(this, toucheventselector(PopUpUI::wipeDungeonCallBack));
    
    UILabel *pLabel_0 = static_cast<UILabel *>(panel1->getChildByName("Label_0"));
    CC_ASSERT(pLabel_0);
    UILabel *pLabel_1 = static_cast<UILabel *>(panel1->getChildByName("Label_1"));
    CC_ASSERT(pLabel_1);
    UILabel *pLabel_2 = static_cast<UILabel *>(panel1->getChildByName("Label_2"));
    CC_ASSERT(pLabel_2);
    UILabel *pLabel_3 = static_cast<UILabel *>(panel1->getChildByName("Label_3"));
    CC_ASSERT(pLabel_3);
    
    UILabel *pLabel_curentPhyS = static_cast<UILabel *>(pLabel_0->getChildByName("Label_0"));
    CC_ASSERT(pLabel_curentPhyS);
    
    std::string str1 = ((CCString *)array->objectAtIndex(1))->getCString();//当前体力
    CCArray *pArray = (CCArray *)split_string(str1.c_str(), ';');
    std::string str_currentPhysicalStrength = ((CCString *)pArray->objectAtIndex(1))->getCString();//当前体力
    pLabel_curentPhyS->setText(str_currentPhysicalStrength);

    UILabel *pLabel_consumePhyS = static_cast<UILabel *>(pLabel_1->getChildByName("Label_0"));
    CC_ASSERT(pLabel_consumePhyS);
    std::string str_consumePhysicalStrength = ((CCString *)pArray->objectAtIndex(0))->getCString();//需消耗体力
    pLabel_consumePhyS->setText(str_consumePhysicalStrength);
    
    m_u32CurrentPhysicalStrength = atoi(str_currentPhysicalStrength.c_str());
    m_u32ConsumePhysicalStrength = atoi(str_consumePhysicalStrength.c_str());
    
    
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
        {
            m_DifficultInfo[i][j] = -1;
        }
    for (int i = 0; i < 3; i++)
    {
        std::string str = ((CCString *)array->objectAtIndex(i + 2))->getCString();//难度
        CCArray *pArray = (CCArray *)split_string(str.c_str(), ';');
        m_DifficultInfo[i][0] = ((CCString *)pArray->objectAtIndex(0))->intValue();
        if ( m_DifficultInfo[i][0] != -1)
        {
            m_DifficultInfo[i][1] = ((CCString *)pArray->objectAtIndex(1))->intValue();
            m_DifficultInfo[i][2] = ((CCString *)pArray->objectAtIndex(2))->intValue();
        }
    }

    UILabel *pLabel_refreshNum = static_cast<UILabel *>(pLabel_2->getChildByName("Label_0"));
    CC_ASSERT(pLabel_refreshNum);
    m_pLabelRefreshNum = pLabel_refreshNum;
    CCString *pStrreFreshNum = CCString::createWithFormat("%d", m_DifficultInfo[0][2] - m_DifficultInfo[0][1]);
    std::string str_refreshNum = pStrreFreshNum->getCString();//可刷次数
    pLabel_refreshNum->setText(str_refreshNum);
    //描述
    const char *discription = ((CCString *)array->objectAtIndex(0))->getCString();
    UILabel *pLabel_desc = static_cast<UILabel *>(pLabel_3->getChildByName("Label_0"));
    CC_ASSERT(pLabel_desc);
    pLabel_desc->setText(discription);
    
    //难度
    m_u8ChoosedDifficultOptionIndex = 0;
    m_u8UnlockDifficultOptions = 0;
    ((YYScene *)getParent())->setChoosedDifficultOptionIndex(m_u8ChoosedDifficultOptionIndex);
    
    CCSize winSize;
    winSize.width = 960;
    winSize.height = 640;
    //CCDirector::sharedDirector()->getWinSize();
    const int bgWidth = 860,bgHeight = 540;
    CCPoint initPosition = ccp((winSize.width - bgWidth) / 2 - 10, (winSize.height - bgHeight) / 2 - 220);
    int bgHeight2 = 300,bgWidth3 = 340, bgHeight3 = 200;//bgWidth2 = 450,
    int offsetX1 = 30,offsetY1 = 100;
    ActionSet * actionSet = ActionSet::createActionSetWithFile(DUNGEONS_FLAG_ANU);
    YYRect * rect = NULL;
    
    CCMenu *pMenu = CCMenu::create();
    pMenu->setPosition(ccp(0, 0));
    m_pUITouchGroup->addChild(pMenu);
    for (int i = 0; i < 3; i++)
    {
        int difficult = m_DifficultInfo[i][2];
        //旗
        m_pDifficultButton[i] = YYButton::create();
        
        if (difficult < 0)
        {//灰旗
            rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_FLAG_GRAY_WITHOUT_BASE);
        }
        else
        {//红旗
            m_u8UnlockDifficultOptions ++;
            rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_FLAG_RED_WITHOUT_BASE);
        }
        m_pDifficultButton[i]->initWithFile(DUNGEONS_FLAG_PNG.c_str(),CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight()));
        
        m_pDifficultButton[i]->setButtonRect(CCRectMake(m_pDifficultButton[i]->getPosition().x - m_pDifficultButton[i]->getContentSize().width / 2, m_pDifficultButton[i]->getPosition().y - m_pDifficultButton[i]->getContentSize().height / 2,rect->getWidth(), rect->getHeight()));
        
        CCMenuItemSprite* spriteItemDifficult = CCMenuItemSprite::create(m_pDifficultButton[i], m_pDifficultButton[i], this, menu_selector(PopUpUI::difficultBtnCallBack));
        spriteItemDifficult->setTag(i);
        spriteItemDifficult->setPosition(ccp(initPosition.x + bgWidth - bgWidth3 - offsetX1 + 128 * i + m_pDifficultButton[i]->getContentSize().width / 2, initPosition.y + offsetY1 + bgHeight3 + 10 + m_pDifficultButton[i]->getContentSize().height / 2));
        pMenu->addChild(spriteItemDifficult);
        
        //难度标签
        m_pDifficultLabel[i] = CCLabelTTF::create(difficultLabel[i], DEFAULT_FONT_NAME, 20);
        if (m_u8ChoosedDifficultOptionIndex != i)
        {
            m_pDifficultLabel[i]->setColor(ccWHITE);
        }
        else
        {
            m_pDifficultLabel[i]->setColor(ccYELLOW);
        }
        m_pDifficultLabel[i]->setPosition(spriteItemDifficult->getPosition());
        m_pUITouchGroup->addChild(m_pDifficultLabel[i]);
        
        //星级
        CCSprite *start = NULL;
        for (int j = 0; j < 3; j ++)
        {
            if (j < m_DifficultInfo[i][0])
            {
                rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GOLDEN_SMALL);
                start = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(),CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight()));
            }
            else
            {
                rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GRAY_SMALL);
                start = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(),CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight()));
            }
            start->setPosition(ccp(spriteItemDifficult->getPositionX() + (j - 1) * (start->getContentSize().width), spriteItemDifficult->getPositionY() + 20));
            m_pUITouchGroup->addChild(start,1);
        }
    }
    
    //奖励物品
    int offsetX2 = 30,offsetY2 = 120;
    int intervalH = 110,intervalV = 170;
    CCLabelTTF *label = CCLabelTTF::create(g_sFixedAward, DEFAULT_FONT_NAME, 30);
    label->setPosition(ccp(initPosition.x + offsetX1 + offsetX2 + 10, initPosition.y + offsetY1 + bgHeight2 - offsetY2 + 80));
    label->setColor(ccRED);
    m_pUITouchGroup->addChild(label);
    
    //固定奖励
    CCString *string = (CCString *)array->objectAtIndex(5);
    CC_SAFE_DELETE(m_pAwardString);
    m_pAwardString = new CCString(string->getCString());
    ((YYScene *)getParent())->setAwardString(m_pAwardString);
    
    CCArray * awardArray = CCArray::createWithArray(split_string(string->getCString(), ';'));
    
    //随机奖励
    m_pAwardDictionary = ((YYScene *)getParent())->getAwardDictionary();
    if (!m_pAwardDictionary)
    {
        m_pAwardDictionary = new CCDictionary();
        ((YYScene *)getParent())->setAwardDictionary(m_pAwardDictionary);
    }
    else
    {
        m_pAwardDictionary->removeAllObjects();
    }
    for (int i = 6; i < array->count(); i++)
    {
        CCString *string = (CCString *)array->objectAtIndex(i);
        awardArray->addObjectsFromArray(split_string(string->getCString(), ';'));
        m_pAwardDictionary->setObject(string, i - 6);
    }
    std::string pngFilePath = getFilePath("208", kDirTypeUI, kFileSuffixPng);
    for (int i = 0; i < 4; i++)
    {
        CCArray * array1 = split_string(((CCString *)awardArray->objectAtIndex(i))->getCString(), ',');
        const char * iconName = ((CCString *)array1->objectAtIndex(0))->getCString();//物品图标
        int num = ((CCString *)array1->objectAtIndex(1))->intValue();//数量
        
        //物品栏
        CCSprite * sprite = CCSprite::create(pngFilePath.c_str());
        float tempWidth = sprite->getContentSize().width;
        float tempHeight = sprite->getContentSize().height;
        float tempX = initPosition.x + offsetX1 + offsetX2 + i % 4 * intervalH + tempWidth / 2 - 58;
        float tempY = initPosition.y + offsetY1 + bgHeight2 - offsetY2 - i / 4 * intervalV - tempHeight / 2 + 25;
        
        //星级
        int startLevel = 1;
        if (i % 4 > 1)
        {
            startLevel = i % 4;
        }
        for (int j = 0; j < startLevel; j++)
        {
            rect = (YYRect *)actionSet->getImageRectDataArray()->objectAtIndex(CLIP_START_GOLDEN_SMALL);
            sprite = CCSprite::create(DUNGEONS_FLAG_PNG.c_str(),CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight()));
            sprite->setPosition(ccp(tempX - (startLevel - 1) * sprite->getContentSize().width / 2 + j * sprite->getContentSize().width,tempY + tempHeight / 2 + sprite->getContentSize().height / 2 + 10));
            m_pUITouchGroup->addChild(sprite);
        }
        
        //物品图标
        std::string str = getFilePath(iconName, kDirTypeIcon, kFileSuffixPng);
        sprite = CCSprite::create(str.c_str());
        sprite->setPosition(ccp(tempX,tempY));
        m_pUITouchGroup->addChild(sprite);
        sprite->setTag(TAG_AWARD_ICON + i);
        
        //奖励物品数量
        const char *temp = CCString::createWithFormat("X%d",num)->getCString();
        CCLabelTTF *label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 20);
        label->setColor(ccRED);
        label->setPosition(ccp(initPosition.x + offsetX1 + offsetX2 + i % 4 * intervalH + tempWidth / 2 + label->getContentSize().width/2 - 15, initPosition.y + offsetY1 + bgHeight2 - offsetY2 - i / 4 *intervalV - 10));
        m_pUITouchGroup->addChild(label);
        label->setTag(TAG_AWARD_NUM + i);
    }
}

#pragma mark 扫荡UI界面

void PopUpUI::wipeDungeonCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_u32CurrentPhysicalStrength < m_u32ConsumePhysicalStrength) // 体力值不足
        {
            ((YYScene*)getParent())->showTips(g_sPhysicalStrengthInsuffence, kTipNormal);
            return;
        }
        if(m_DifficultInfo[m_u8ChoosedDifficultOptionIndex][0] < 3) // 星级不够
        {
            ((YYScene*)getParent())->showTips(g_sStarPointLack, kTipNormal);
            return;
        }
        sendWipeDungeonConfirmMessage(); // 发送扫荡信息
        m_pUITouchPop = NULL;
        initWipeDungeonAwardUI();
    }
}

void PopUpUI::wipeDungeonCloseCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_u8CurrentActiveItemIndex < m_u8ShowItemAmount)
        {
            return;
        }
        closeWipeDungeonAwardUI();
    }
}
void PopUpUI::wipeDungeonContinueCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_u8CurrentActiveItemIndex < m_u8ShowItemAmount)
        {
            return;
        }
        m_pRoleByControlled = ((YYScene *)getParent())->getControlUILayer()->getRoleByControlled();
        SpriteAttribute* arrtibute = m_pRoleByControlled->getSpriteAttribute();
        int nPhys = arrtibute->getPhysicalStrengthPoint();
        if (nPhys < m_u32ConsumePhysicalStrength)
        {
            ((YYScene*)getParent())->showTips(g_sPhysicalStrengthInsuffence, kTipWipeDungeon);
            return;
        }
        sendWipeDungeonConfirmMessage(); // 发送扫荡信息
        initWipeDungeonAwardUI();
    }
}

void PopUpUI::closeWipeDungeonAwardUI() // 关闭扫荡页面
{
    m_pUITouchPop->removeAllChildrenWithCleanup(true);
    m_pUITouchPop->removeFromParent();
    m_pUITouchPop = NULL;
    m_pUITouchGroup->setTouchEnabled(true);
    setUIType(kUIStageAwardInfo); // 切换至关卡选择页面
    
    // 刷新体力值
    m_pRoleByControlled = ((YYScene *)getParent())->getControlUILayer()->getRoleByControlled();
    SpriteAttribute* arrtibute = m_pRoleByControlled->getSpriteAttribute();
    int nPhys = arrtibute->getPhysicalStrengthPoint();
    CC_ASSERT(m_pUITouchGroup);
    UIPanel* panel1 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel1"));
    CC_ASSERT(panel1);
    UILabel *pLabel_0 = static_cast<UILabel *>(panel1->getChildByName("Label_0"));
    CC_ASSERT(pLabel_0);
    UILabel *pLabel_curentPhyS = static_cast<UILabel *>(pLabel_0->getChildByName("Label_0"));
    CC_ASSERT(pLabel_curentPhyS);
    CCString* strPhys = CCString::createWithFormat("%d",nPhys);
    pLabel_curentPhyS->setText(strPhys->getCString());
}

void PopUpUI::updateWipeDungeonResult() // 更新奖励动画
{
    CCNode * node = m_pUITouchPop->getChildByTag(TAG_FIRST_ACTIVE_ITEM + m_u8CurrentActiveItemIndex);
    if (node)
    {
        node->setVisible(true);
        if (dynamic_cast<YYSprite *>(node))
        {
            if (((YYSprite * )node)->isActionOver())
            {
                m_u8CurrentActiveItemIndex++;
            }
            else
            {
                ((YYSprite * )node)->updateAnimation();
            }
        }
        else
        {
            m_u8CurrentActiveItemIndex++;
        }
        if(m_u8CurrentActiveItemIndex > 10)
        {
            if (m_u8CurrentActiveItemIndex % 2 == 1)
            {
                CCPoint position = m_pUITouchPop->getChildByTag(TAG_FIRST_ACTIVE_ITEM + m_u8CurrentActiveItemIndex - 2)->getPosition();
                position.x = position.x - 3;
                YYSprite * sprite = YYSprite::createWithActionSet("se6", "se6", kDirTypeEffect);
                sprite->setAction(0, false);
                sprite->setPosition(position);
                m_pUITouchPop->addChild(sprite);
                m_pBonusEffectArray->addObject(sprite);
                const std::string filePath = getFilePath(REWARDSTAR_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                PLAY_EFFECT(filePath.c_str());
            }
        }
    }
    for (int i = 0; i < m_pBonusEffectArray->count();)
    {
        YYSprite * sprite = (YYSprite *)m_pBonusEffectArray->objectAtIndex(i);
        if (sprite->isActionOver())
        {
            m_pBonusEffectArray->removeObjectAtIndex(i);
        }
        else
        {
            sprite->update();
            i++;
        }
    }
}

void PopUpUI::initWipeDungeonAwardUI()
{
    do
    {
        if (m_pUITouchPop)
        {
            m_pUITouchPop->removeAllChildrenWithCleanup(true);
            m_pUITouchPop->removeFromParentAndCleanup(true);
        }
        CCSize winSize2 = CCDirector::sharedDirector()->getWinSize();
        m_pUITouchPop = TouchGroup::create();
        m_pUITouchPop->setPosition(ccp((winSize2.width - STANDARD_SCENE_WIDTH) / 2,(winSize2.height - STANDARD_SCENE_HEIGHT) / 2));
        addChild(m_pUITouchPop,15);
        setUIType(kUIWipeDungeon);
        std::string filePath = FIGHT_EVALUATION_UI_RESOURCE_FILE_NAME;
        m_pUITouchPop->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
        Layout * panel = static_cast<Layout *>(m_pUITouchPop->getWidgetByName("Panel"));
        UIPanel* panel0 = static_cast<UIPanel *>(panel->getChildByName("Panel0"));
        CC_BREAK_IF(!panel0);
        
        // 继续扫荡
        UIButton* btnWipe = static_cast<UIButton *>(panel0->getChildByName("Button_goon"));
        CC_BREAK_IF(!btnWipe);
        btnWipe->setTouchEnabled(true);
        btnWipe->setVisible(true);
        btnWipe->addTouchEventListener(this, toucheventselector(PopUpUI::wipeDungeonContinueCallBack));
        
        // 关闭
        UIButton* btnClose = static_cast<UIButton *>(panel0->getChildByName("Button_done"));
        CC_BREAK_IF(!btnClose);
        btnClose->setTouchEnabled(true);
        btnClose->setVisible(true);
        btnClose->addTouchEventListener(this, toucheventselector(PopUpUI::wipeDungeonCloseCallBack));
        
        // 数据初始化
        m_pAwardString = ((YYScene *)getParent())->getAwardString();
        CCArray * array = split_string(m_pAwardString->getCString(), ';'); // 固定奖励
        
        m_u8ChoosedDifficultOptionIndex = ((YYScene *)getParent())->getChoosedStageDifficultIndex();
        
        if (!m_pAwardArray)
        {
            m_pAwardArray = new CCArray(5);
        }
        else
        {
            m_pAwardArray->removeAllObjects();
        }
        m_u8ShowItemAmount = 4;//初始化待显示项的数量为星级（5个数字3颗星星）
        for (int i = 0; i < 4; i++)
        {
            m_pAwardArray->addObject(array->objectAtIndex(i));
            m_u8ShowItemAmount += 2;//每增加一个奖励，待显示项增加图标和数量两项
        }
        if (!m_pBonusEffectArray)
        {
            m_pBonusEffectArray = new CCArray(5);
        }
        else
        {
            m_pBonusEffectArray->removeAllObjects();
        }
        CCString* pString = (CCString*)m_pAwardDictionary->objectForKey(m_u8ChoosedDifficultOptionIndex);
        CCArray* pSeparateArray = split_string(pString->getCString(), ';'); // 分离最后两个属性
        m_pAwardArray->addObject(pSeparateArray->objectAtIndex(pSeparateArray->count() - 1));// 经验
        m_pAwardArray->addObject(pSeparateArray->objectAtIndex(pSeparateArray->count() - 2));// 金钱
 
        m_u8CurrentActiveItemIndex = 0;
        // 动画数据初始化
    
        CCSize winSize;
        winSize.width = 960;
        winSize.height = 640;
        const int bgWidth = 860,bgHeight = 540;
        int bgWidth2 = 780,bgHeight2 = 380;
        CCPoint initPosition = ccp((winSize.width - bgWidth) / 2, (winSize.height - bgHeight) / 2);
        int offsetX = initPosition.x + (bgWidth - bgWidth2) / 2.0f,offsetY = initPosition.y + (bgHeight - bgHeight2) / 2.0f;
        
        // "胜利"动画
        YYSprite * aniSprite = YYSprite::createWithActionSet("96", "96", kDirTypeEffect);
        aniSprite->setPosition(ccp(winSize.width / 2, offsetY + bgHeight2 - 70));
        m_pUITouchPop->addChild(aniSprite);
        aniSprite->setVisible(false);
        int tag = TAG_FIRST_ACTIVE_ITEM;
        aniSprite->setTag(tag++);
        aniSprite->setAction(0, false);
        
        const char *temp = "---";
        // 通关时间
        CCLabelTTF *label = CCLabelTTF::create(g_sFightResult, DEFAULT_FONT_NAME, 30);
        float nameLabelWidth = label->getContentSize().width;
        label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
        label->setPosition(ccp(offsetX + 30 + (bgWidth2 / 2 - 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 90));
        label->setAnchorPoint(ccp(0.5,1));
        m_pUITouchPop->addChild(label);
        label->setTag(tag++);
        label->setVisible(false);
        
        // 最大连击
        label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
        label->setPosition(ccp(offsetX + 30 + (bgWidth2 / 2 - 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 150));
        label->setAnchorPoint(ccp(0.5,1));
        m_pUITouchPop->addChild(label);
        label->setTag(tag++);
        label->setVisible(false);
        
        // 击杀个数
        label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
        label->setPosition(ccp(offsetX + 30 + (bgWidth2 / 2 - 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 210));
        label->setAnchorPoint(ccp(0.5,1));
        m_pUITouchPop->addChild(label);
        label->setTag(tag++);
        label->setVisible(false);
        
        // 通关评价"三星"
        for (int i = 0; i < 3; i++)
        {
            aniSprite = YYSprite::createWithActionSet("96", "96", kDirTypeEffect);
            aniSprite->setPosition(ccp((winSize.width + bgWidth2 / 2 + 30 + nameLabelWidth) / 2 - 78 + 78 * i, offsetY + bgHeight2 - 170));
            m_pUITouchPop->addChild(aniSprite);
            aniSprite->setTag(tag++);
            aniSprite->setVisible(false);
            aniSprite->setAction(2, false);
        }
        int expAward = ((CCString *)m_pAwardArray->objectAtIndex(m_pAwardArray->count() - 2))->intValue();//经验奖励
        temp = CCString::createWithFormat("%d",expAward)->getCString();
        label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
        label->setPosition(ccp((winSize.width + bgWidth2 / 2 + 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 150));
        label->setAnchorPoint(ccp(0.5,1));
        m_pUITouchPop->addChild(label);
        label->setTag(tag++);
        label->setVisible(false);
        
        int bonus = ((CCString *)m_pAwardArray->objectAtIndex(m_pAwardArray->count() - 1))->intValue();//奖金
        temp = CCString::createWithFormat("%d",bonus)->getCString();
        label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
        label->setPosition(ccp((winSize.width + bgWidth2 / 2 + 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 210));
        label->setAnchorPoint(ccp(0.5,1));
        m_pUITouchPop->addChild(label);
        label->setTag(tag++);
        label->setVisible(false);
        
        for (int i = 0; i < 5; i++)
        {
            // 物品栏
            string str = getFilePath("208", kDirTypeUI, kFileSuffixPng);
            CCSprite * sprite = CCSprite::create(str.c_str());
            
            float tempWidth = sprite->getContentSize().width;
            int interval = 26;
            float tempX = offsetX + 172 + label->getContentSize().width + tempWidth / 2 + i * (tempWidth + interval);
            sprite->setPosition(ccp(tempX,offsetY + bgHeight2 - 300));
            
            if (i < m_pAwardArray->count() - 2) {//m_pAwardArray最后两位是金钱、经验
                CCArray *array = split_string(((CCString *)m_pAwardArray->objectAtIndex(i))->getCString(), ',');
                // 物品图标
                const char *iconName = ((CCString *)array->objectAtIndex(0))->getCString();
                string str = getFilePath(iconName, kDirTypeIcon, kFileSuffixPng);
                sprite = CCSprite::create(str.c_str());
                sprite->setPosition(ccp(tempX,offsetY + bgHeight2 - 300));
                m_pUITouchPop->addChild(sprite);
                sprite->setTag(tag++);
                sprite->setVisible(false);
                
                // 奖励物品数量
                int num = ((CCString *)array->objectAtIndex(1))->intValue();
                temp = CCString::createWithFormat("X%d",num)->getCString();
                CCLabelTTF *label1 = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 20);
                label1->setColor(ccRED);
                label1->setPosition(ccp(tempX + (tempWidth + interval) / 2, offsetY + bgHeight2 - 300));
                m_pUITouchPop->addChild(label1);
                label1->setTag(tag++);
                label1->setVisible(false);
            }
        }
        
        m_pUITouchGroup->setTouchEnabled(false);
    } while (0);

   
}

void PopUpUI::sendWipeDungeonConfirmMessage()
{
    m_u8DungeonsIndex = ((YYScene *)getParent())->getDungeonsIndex();
    m_u8ChoosedStageIndex = ((YYScene *)getParent())->getChoosedStageIndex();
    m_u8ChoosedDifficultOptionIndex = ((YYScene *)getParent())->getChoosedStageDifficultIndex();
    const char *temp = CCString::createWithFormat(SEND_WIPE_DUNGEON,m_u8DungeonsIndex,m_u8ChoosedStageIndex,m_u8ChoosedDifficultOptionIndex)->getCString();
    Controller::getInstance()->sendMessage(temp,false);
}

void PopUpUI::enterDungeonCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if (m_u32CurrentPhysicalStrength < m_u32ConsumePhysicalStrength)
        {
            ((YYScene*)getParent())->showTips(g_sPhysicalStrengthInsuffence, kTipNormal);
            return;
        }
        sendChoosedStageConfirmMessage();
        Controller::getInstance()->switchGameStatus(kStatusLoading);
        ((YYScene *)getParent())->setStageDamage(0);
        ((YYScene *)getParent())->setStageDuration(0);
        ((YYScene *)getParent())->setHPDecrement(0);
        ((YYScene *)getParent())->setMaxComboCount(0);
        ((YYScene *)getParent())->setHittedAmount(0);
        ((YYScene *)getParent())->setKillCount(0);
    }
}
void PopUpUI::difficultBtnCallBack(CCObject *pSender)
{
    CCNode *node = (CCNode *)pSender;
    int nTag = node->getTag();
    if (nTag < m_u8UnlockDifficultOptions)
    {
        m_u8ChoosedDifficultOptionIndex = ((YYScene *)getParent())->getChoosedStageDifficultIndex();
        m_pDifficultLabel[m_u8ChoosedDifficultOptionIndex]->setColor(ccWHITE);
        m_u8ChoosedDifficultOptionIndex = nTag;
        m_pDifficultLabel[nTag]->setColor(ccYELLOW);
        ((YYScene *)getParent())->setChoosedDifficultOptionIndex(m_u8ChoosedDifficultOptionIndex);
        CCString *pStrreFreshNum = CCString::createWithFormat("%d", m_DifficultInfo[m_u8ChoosedDifficultOptionIndex][2] - m_DifficultInfo[m_u8ChoosedDifficultOptionIndex][1]);
        std::string str_refreshNum = pStrreFreshNum->getCString();//可刷次数
        m_pLabelRefreshNum->setText(str_refreshNum);

    }
}
#pragma mark 翻牌

void PopUpUI::showFlopAward()
{
    ((YYScene*)getParent())->getControlUILayer()->clearKeyState();
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIAward);
    setAwardStatus(kAwardChooseStatus);
    setAwardActionStatus(kAwardActionDefaultStatus);
    m_nCurrentExcludeCount = 2;
    m_nMaxExcludeCount = 2;
    m_nMaxAwardCount = 2;
    m_nCurrentAwardCount = 2;
    m_nMaxZorder = 0;
    m_nStartFrame = 0; ;
    m_nIntervalFrame = INTERVAL_FRAME;
    m_nRandomCount = 0;
    m_nRandomIndex = -1;
    m_nNeedJewelCount = -1;
    
    m_pAwardButtonArray = CCArray::createWithCapacity(10);
    m_pAwardButtonArray->retain();
    m_pAwardPositionArray = CCArray::createWithCapacity(10);
    m_pAwardPositionArray->retain();
    
    m_pChooseAwardArray = CCArray::createWithCapacity(10);
    m_pChooseAwardArray->retain();

    m_pUITouchGroup = TouchGroup::create();
    addChild(m_pUITouchGroup);
    m_pUITouchGroup->setPosition(m_cAdjustScreenPosition);
    std::string filePath = getFilePath("NewProject_1", kDirTypeUI, kFileSuffixJson);
    Widget *mainWidget = GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str());
    m_pUITouchGroup->addWidget(mainWidget);
    
    UIPanel* panel2 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    UILabel* text_Count = static_cast<UILabel*>(panel2->getChildByName("TextArea1"));
    m_pUILabel = text_Count;
    
    UIPanel* panel = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton* shuffle_Button = static_cast<UIButton*>(panel->getChildByName("button0"));
    UILabel* text_Lottery =static_cast<UILabel*>(shuffle_Button->getChildByName("TextArea"));
    text_Lottery->setText(g_sLottery);
    shuffle_Button->addTouchEventListener(this, toucheventselector(PopUpUI::menuExcludeCallback)); // 洗牌
    UIButton* accept_Button = static_cast<UIButton*>(panel->getChildByName("button1"));
    accept_Button->addTouchEventListener(this, toucheventselector(PopUpUI::menuGoCallback)); // 获得
    accept_Button->setZOrder(shuffle_Button->getZOrder() - 1);
    UIButton* random_Button = static_cast<UIButton*>(panel->getChildByName("button2"));
    random_Button->addTouchEventListener(this, toucheventselector(PopUpUI::menuRandomCallback)); // 随机获得
    random_Button->setVisible(false);
    
    UIPanel* panel_Award = static_cast<UIPanel*>(mainWidget->getChildByName("Panel1"));
    panel_Award->setZOrder(1);
    CCArray* pButtonArray = panel_Award->getChildren();
    CCObject* object = NULL;
    CCARRAY_FOREACH(pButtonArray, object)
    {
        UIButton* pButton = (UIButton*)object;
        pButton->setVisible(false);
    }
    m_pAwardDictionary = ((YYScene *)getParent())->getAwardDictionary();
    if(!m_pAwardDictionary)
    {
        return;
    }
    CCString *awardStr = (CCString*)m_pAwardDictionary->objectForKey(((YYScene*)getParent())->getChoosedStageDifficultIndex());
    if (!awardStr)
        return;
    CCArray *arr = split_string(awardStr->getCString(), ';');
    arr->removeLastObject();
    arr->removeLastObject();
    
//    time_t t;
//    std::srand((unsigned int)time(&t));// 随机数种子
//    while (arr->count() > 0)
//    {
//        int temp = rand()% arr->count();
//        m_pChooseAwardArray->addObject(arr->objectAtIndex(temp));
//        arr->removeObjectAtIndex(temp);
//    }
    m_pChooseAwardArray->addObjectsFromArray(arr);
    for(int i = 0; i < m_pChooseAwardArray->count(); ++i)
    {
        CCString *awardStr1 = (CCString*)m_pChooseAwardArray->objectAtIndex(i);
        CCArray *arr = split_string(awardStr1->getCString(), ',');
        UIButton* button_Award = static_cast<UIButton*>(pButtonArray->objectAtIndex(i));
        button_Award->addTouchEventListener(this, toucheventselector(PopUpUI::menuLockCallback));
        UIImageView* image_Award = static_cast<UIImageView*>(button_Award->getChildByName("ImageView0"));
        std::string path = getFilePath(((CCString *)arr->objectAtIndex(0))->getCString(), kDirTypeIcon, kFileSuffixPng);
        image_Award->loadTexture(path.c_str());
        button_Award->setVisible(true);
        button_Award->setTag(TAG_FRONT_FACE);
        m_pAwardButtonArray->addObject(button_Award);
        
        UIButton* pPosition = UIButton::create();
        pPosition->setPosition(button_Award->getPosition());
        m_pAwardPositionArray->addObject(pPosition);
        Label * label = static_cast<Label*>(button_Award->getChildByName("TextArea0"));
        std::string name = ((CCString*)arr->objectAtIndex(3))->getCString();
        label->setText(name);
        int quality = ((CCString*)arr->objectAtIndex(4))->intValue();
        setTextColorByQuality(label, quality);
    }
    changeFlopCount();
    updateAwardUIPropertyInfo();
}

void PopUpUI::updateAwardUIPropertyInfo()
{
    UIPanel* panel = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel"));
    UIPanel* panel_Wealth = static_cast<UIPanel*>(panel->getChildByName("Panel4"));
    UILabel* textArea_Jewel = static_cast<UILabel*>(panel_Wealth->getChildByName("TextArea0"));
    UILabel* textArea_Money = static_cast<UILabel*>(panel_Wealth->getChildByName("TextArea1"));
    UILabel* jewel_Text = static_cast<UILabel*>(textArea_Jewel->getChildByName("TextArea00"));
    UILabel* money_Text = static_cast<UILabel*>(textArea_Money->getChildByName("TextArea10"));
    
    money_Text->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getGold())->getCString());
    int color = 0xded3bd;
    ccColor3B color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
    money_Text->setColor(color3B);
    jewel_Text->setText(CCString::createWithFormat("%d",m_pRoleByControlled->getSpriteAttribute()->getJewel())->getCString());
    color = 0xffbc2d;
    color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
    jewel_Text->setColor(color3B);
}

void PopUpUI::changeFlopCount()
{
    m_pUILabel->setText(CCString::createWithFormat("%d / %d",m_nCurrentExcludeCount,m_nMaxExcludeCount)->getCString());
}

void PopUpUI::changeReceiveCount()
{
    m_pUILabel->setText(CCString::createWithFormat("%d / %d",m_nCurrentAwardCount,m_nMaxAwardCount)->getCString());
}

void PopUpUI::menuLockCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        UIButton* pButton = (UIButton*)pSender;
        switch (getAwardStatus())
        {
            case kAwardChooseStatus:
            {
                if(pButton->getTag() == TAG_FRONT_FACE)
                {
                    if(m_nCurrentExcludeCount == 0)
                    {
                        ((YYScene*)getParent())->showTips(g_sNotCouldExclude, kTipNormal);
                        return;
                    }
                    UIImageView* image_Award = static_cast<UIImageView*>(pButton->getChildByName("ImageView0"));
                    image_Award->setVisible(false);
                    UILabel* text_Description =static_cast<UILabel*>(pButton->getChildByName("TextArea0"));
                    text_Description->setVisible(false);
                    UIImageView* image_BackFace = static_cast<UIImageView*>(pButton->getChildByName("ImageView1"));
                    image_BackFace->setVisible(true);
                    UIImageView* image_Lock = static_cast<UIImageView*>(pButton->getChildByName("ImageView2"));
                    image_Lock->setVisible(true);
                    UIImageView* image_Exclude = static_cast<UIImageView*>(pButton->getChildByName("ImageView3"));
                    image_Exclude->setVisible(false);
                    UIImageView* image_Box = static_cast<UIImageView*>(pButton->getChildByName("ImageView5"));
                    image_Box->setVisible(false);
                    Label *label = static_cast<Label *>(pButton->getChildByName("TextArea0"));
                    label->setVisible(false);
                    if(pButton->getChildByTag(TAG_AWARD_STROKE))
                    {
                        pButton->getChildByTag(TAG_AWARD_STROKE)->setVisible(false);
                    }
                    if(pButton->getChildByTag(TAG_AWARD_NAME))
                    {
                        pButton->getChildByTag(TAG_AWARD_NAME)->setVisible(false);
                    }
                    --m_nCurrentExcludeCount;
                    changeFlopCount();
                    pButton->setTag(TAG_BACK_FACE);
                }
                else if (pButton->getTag() == TAG_BACK_FACE)
                {
                    UIImageView* image_Award = static_cast<UIImageView*>(pButton->getChildByName("ImageView0"));
                    image_Award->setVisible(true);
                    
                    UIImageView* image_BackFace = static_cast<UIImageView*>(pButton->getChildByName("ImageView1"));
                    image_BackFace->setVisible(false);
                    UIImageView* image_Lock = static_cast<UIImageView*>(pButton->getChildByName("ImageView2"));
                    image_Lock->setVisible(false);
                    UIImageView* image_Exclude = static_cast<UIImageView*>(pButton->getChildByName("ImageView3"));
                    image_Exclude->setVisible(true);
                    UIImageView* image_Box = static_cast<UIImageView*>(pButton->getChildByName("ImageView5"));
                    image_Box->setVisible(true);
                    Label *label = static_cast<Label *>(pButton->getChildByName("TextArea0"));
                    label->setVisible(true);
                    if(pButton->getChildByTag(TAG_AWARD_STROKE))
                    {
                        pButton->getChildByTag(TAG_AWARD_STROKE)->setVisible(true);
                    }
                    if(pButton->getChildByTag(TAG_AWARD_NAME))
                    {
                        pButton->getChildByTag(TAG_AWARD_NAME)->setVisible(true);
                    }
                    
                    ++m_nCurrentExcludeCount;
                    changeFlopCount();
                    pButton->setTag(TAG_FRONT_FACE);
                }
            }
                break;
            case kAwardReceiveStatus:
            {
                m_pShuffle_Button = (UIButton *)pSender;
                if(m_pShuffle_Button->getTag() != TAG_CHOOSE_FACE)
                {
                    if(m_nCurrentAwardCount == 0)
                    {
                        if (m_nNeedJewelCount > 0) // 判断是否是消耗钻石翻牌
                        {
                            setConsumeJewelTips(kJewelConsumeForFlopAward, m_nNeedJewelCount);
                            return;
                        }
                    }
                    CCString *pStrMsg = CCString::createWithFormat(ASK_TURN_UP_CARD, 0);
                    std::string strMsg = pStrMsg->getCString();
                    Controller::getInstance()->sendMessage(strMsg.c_str(), true);
                    setAwardStatus(kAwardNotClickStatus);
                    setAwardActionStatus(kAwardActionNotClickStatus);
                }
            }
                break;
        
            default:
                break;
        }
        
    }
}
void PopUpUI::flopAwardConsumeJewelLogic()
{
    int jewelValue = m_pRoleByControlled->getSpriteAttribute()->getJewel();
    if(jewelValue < m_nNeedJewelCount)
    {
        ((YYScene*)getParent())->showTips(g_sLackJew, kTipJewCharge);
        return;
    }
    m_pUITouchGroup->setTouchEnabled(true);
    CCString *pStrMsg = CCString::createWithFormat(ASK_TURN_UP_CARD, 0);
    std::string strMsg = pStrMsg->getCString();
    Controller::getInstance()->sendMessage(strMsg.c_str(), true);
    setAwardStatus(kAwardNotClickStatus);
    setAwardActionStatus(kAwardActionNotClickStatus);
}
void PopUpUI::getAwardForChoiceCard(cocos2d::CCArray* array)
{
    if (!array)
        return;
    CCString *pStrJewel = (CCString *)array->objectAtIndex(1);
    if (pStrJewel)
    {
        m_nNeedJewelCount = pStrJewel->intValue();
    }
    
    std::string path = "";
    std::string name = "";
    int quality = 0;
    CCString *pStrAward = (CCString *)array->objectAtIndex(2);
    if (pStrAward)
    {
        CCArray *arr = split_string(pStrAward->getCString(),',');

        CCString *pStrIcon = (CCString *)arr->objectAtIndex(0);
        if (pStrIcon)
        {
            path = getFilePath(pStrIcon->m_sString, kDirTypeIcon, kFileSuffixPng);
        }

        CCString *pStrName = (CCString *)arr->objectAtIndex(3);
        if (pStrName)
        {
            name = pStrName->m_sString;
        }
        CCString *pStrQuality = (CCString *)arr->objectAtIndex(4);
        if (pStrName)
        {
            quality = pStrQuality->intValue();
        }

    }

    UIButton *pButton = m_pShuffle_Button;
    ImageView* image_Award = static_cast<ImageView*>(pButton->getChildByName("ImageView0"));
    image_Award->loadTexture(path.c_str());
    image_Award->setVisible(true);
    UIImageView* image_BackFace = static_cast<UIImageView*>(pButton->getChildByName("ImageView1"));
    image_BackFace->setVisible(false);
    UIImageView* image_Lock = static_cast<UIImageView*>(pButton->getChildByName("ImageView2"));
    image_Lock->setVisible(false);
    m_pAwardButtonArray->removeObject(pButton);  // 随机获取的时候需要跳过此位置
    pButton->setTag(TAG_CHOOSE_FACE);
    
    UIImageView* image_Price =static_cast<UIImageView*>(pButton->getChildByName("ImageView4"));
    image_Price->setVisible(false);  // 打开后取消 钻石消耗提示
    UIImageView* image_Jewel =static_cast<UIImageView*>(pButton->getChildByName("ImageView6"));
    image_Jewel->setVisible(false);  // 打开后取消 钻石消耗提示
    m_pRandomImage = UIImageView::create();
    string str = getFilePath("315", kDirTypeUI, kFileSuffixPng);
    m_pRandomImage->loadTexture(str.c_str());
    m_pRandomImage->setPosition(ccp(pButton->getPosition().x , pButton->getPosition().y - 10));
    UIPanel* panel3 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel3"));
    panel3->addChild(m_pRandomImage);
    
    Label *label = static_cast<Label *>(pButton->getChildByName("TextArea0"));
    label->setVisible(true);
    label->setText(name);
    setTextColorByQuality(label, quality);
    
    if (m_nCurrentAwardCount > 0)
        --m_nCurrentAwardCount;
    changeReceiveCount();
    UIPanel* panel = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton* accept_Button = static_cast<UIButton*>(panel->getChildByName("button1"));
    accept_Button->setVisible(true);
    if (m_nCurrentAwardCount == 0)
    {
        for(int i = 0;i < m_pAwardButtonArray->count();++i)
        {
            UIButton* awardButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
            UIImageView* image_Price =static_cast<UIImageView*>(awardButton->getChildByName("ImageView4"));
            image_Price->setVisible(false);
            UIImageView* image_Jewel =static_cast<UIImageView*>(awardButton->getChildByName("ImageView6"));
            image_Jewel->setVisible(true);
            
            UILabelAtlas* pLabel = UILabelAtlas::create();
            string str = getFilePath("188", kDirTypePic, kFileSuffixPng);
            //pLabel->setProperty(CCString::createWithFormat(g_sNeedJewelOpen,m_nNeedJewelCount)->getCString(), str.c_str(), 27, 35, "0");
            pLabel->setProperty(CCString::createWithFormat("%d", m_nNeedJewelCount)->getCString(), str.c_str(), 27, 35, "0");
            if(image_Jewel->getChildByTag(1))
            {
                UILabelAtlas* pTempLabel = (UILabelAtlas*)image_Jewel->getChildByTag(1);
                pTempLabel->removeFromParent();
            }
            image_Jewel->addChild(pLabel);
            pLabel->setAnchorPoint(ccp(0.5, 0.5));
            pLabel->setTag(1);
            pLabel->setZOrder(1);
        }
    }
    setAwardActionStatus(kAwardActionDefaultStatus);
    setAwardStatus(kAwardReceiveStatus);
}
void PopUpUI::setTextColorByQuality(cocos2d::gui::Label* label, int quality)
{
    int color;
    ccColor3B color3B;
    switch (quality)
    {
        case 2:
            color = 0xc0ff00;
            color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
            break;
        case 3:
            color = 0x00a4f1;
            color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
            break;
        case 4:
            color = 0xb705ea;
            color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
            break;
        case 5:
        {
            color = 0xffa800;
            color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
        }
            break;
        default:
            color = 0xd5d5d5;
            color3B = ccc3(color>>16,(color&0x00FF00)>>8,color&0x0000FF);
            break;
    }
    label->setColor(color3B);
}

void PopUpUI::processJewelAward(cocos2d::CCArray* array)
{
    m_nNeedJewelCount = ((CCString*)array->objectAtIndex(0))->intValue();
    for(int i=0;i < m_pAwardButtonArray->count();++i)
    {
        UIButton* awardButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
        UIImageView* image_Price =static_cast<UIImageView*>(awardButton->getChildByName("ImageView4"));
        image_Price->setVisible(false);
        UIImageView* image_Jewel =static_cast<UIImageView*>(awardButton->getChildByName("ImageView6"));
        image_Jewel->setVisible(true);
        
        UILabelAtlas* pLabel = UILabelAtlas::create();
        string str = getFilePath("188", kDirTypePic, kFileSuffixPng);
        pLabel->setProperty(CCString::createWithFormat(g_sNeedJewelOpen,m_nNeedJewelCount)->getCString(), str.c_str(), 27, 35, "0");
        if(image_Jewel->getChildByTag(1))
        {
            UILabelAtlas* pTempLabel = (UILabelAtlas*)image_Jewel->getChildByTag(1);
            pTempLabel->removeFromParent();
        }
        image_Jewel->addChild(pLabel);
        pLabel->setAnchorPoint(ccp(0.5,0.5));
        pLabel->setTag(1);
        pLabel->setZOrder(1);
        pLabel->setPosition(ccp(image_Jewel->getContentSize().width + 50,0));
    }
    m_nCurrentAwardCount = 1;
    setAwardStatus(kAwardReceiveStatus);
    setAwardActionStatus(kAwardActionDefaultStatus);
}

/**
 * 扣牌，去掉两个不想要的奖励
 */
void PopUpUI::menuExcludeCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    
    if (type == TOUCH_EVENT_ENDED)
    {
        m_pShuffle_Button = (UIButton*)pSender;
        
        int nIndex = 0;
        int nBackFace[2] = {-1};
        memset(nBackFace, -1, sizeof(nBackFace));
        for(int i = 0;i < m_pAwardButtonArray->count(); ++i)
        {
            UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
            if (pButton->getTag() == TAG_BACK_FACE)
            {
                if (nIndex >= 2)
                    break;
                nBackFace[nIndex] = i;
                ++nIndex;
            }
        }
        CCString *pStrMsg = CCString::createWithFormat(ASK_EXCLUDE_CARD, nBackFace[0], nBackFace[1]);
        Controller::getInstance()->sendMessage(pStrMsg->getCString(), true);
    }
}

void PopUpUI::startShuffleCard()
{
    float flopTime = 2.0;
    for(int i = 0;i < m_pAwardButtonArray->count(); ++i)
    {
        UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
        CCFiniteTimeAction* action1 = CCSequence::create(CCOrbitCamera::create(flopTime/2, 1, 0, 0, 90, 0, 0),CCCallFuncN::create(this, callfuncN_selector(PopUpUI::updateFlopCall)),CCOrbitCamera::create(flopTime/2, 1, 0, 90, 90, 0, 0),CCCallFuncN::create(this, callfuncN_selector(PopUpUI::updateCombineCall)),CCDelayTime::create(1.0f),CCCallFuncN::create(this, callfuncN_selector(PopUpUI::updateShuffleCall)),NULL);
        pButton->runAction(action1);
        Label * label = static_cast<Label *>(pButton->getChildByName("TextArea0"));
        label->setVisible(false);
    }
    setAwardStatus(kAwardNotClickStatus);
    m_pShuffle_Button->setVisible(false);
    UIPanel* panel = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
    UIButton* accept_Button = static_cast<UIButton*>(panel->getChildByName("button1"));
    accept_Button->setZOrder(m_pShuffle_Button->getZOrder() + 1);
    
    UIPanel* panel2 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel2"));
    UILabel* text_Description0 =static_cast<UILabel*>(panel2->getChildByName("TextArea0"));
    text_Description0->setText(g_sCurrentCouldReceive);
    UILabel* text_Description1 =static_cast<UILabel*>(panel2->getChildByName("TextArea2"));
    text_Description1->setText(g_sVIPFunctionInfo);
    changeReceiveCount();
}

void PopUpUI::updateShuffleCall(CCNode* pSender)
{
    if(getAwardActionStatus() == kAwardActionShuffleStatus) // 只能进行一次
    {
        return;
    }
    for(int i=0;i < m_pAwardButtonArray->count();)  // 在动作没结束前不能删除容器数据 放在动作结束之后
    {
        UIButton* pTempButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
        CCString * pAwardStr = (CCString *)m_pChooseAwardArray->objectAtIndex(i);
        if(pTempButton->getTag() == TAG_BACK_FACE)
        {
            m_pAwardButtonArray->removeObject(pTempButton);
            m_pChooseAwardArray->removeObject(pAwardStr);
        }
        else
        {
            i++;
            if(m_nMaxZorder < pTempButton->getZOrder())
            {
                m_nMaxZorder = pTempButton->getZOrder();
            }
        }
    }
    
    for(int i = 0; i < m_pAwardButtonArray->count(); i++)
    {
        int temp = random()% m_pAwardButtonArray->count();
        m_pAwardButtonArray->exchangeObjectAtIndex(i, temp);
        
    }
    setAwardActionStatus(kAwardActionShuffleStatus);
    UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(0);
    CCActionInterval*  move = CCMoveBy::create(0.25, ccp(- 200 , 0));
    CCFiniteTimeAction*  action = CCSequence::create(move,CCCallFuncND::create(this, callfuncND_selector(PopUpUI::updateZorderCall),(void*)0),move->reverse(), NULL);
    pButton->runAction(action);
    action->setTag(100);
}

void PopUpUI::updateZorderCall(CCNode* pSender,void* i)
{
    int index = (int)i;
    UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(index);
    pButton->setZOrder(m_nMaxZorder + 1 + index);
}

void PopUpUI::updateOverCall(CCNode* pSender,void* i)
{
    int index = (int)i;
    if(index == m_pAwardButtonArray->count() - 1 )
    {
        setAwardStatus(kAwardReceiveStatus);  // 能够点击牌状态开启
        UIPanel* panel = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel0"));
        UIButton* random_Button = static_cast<UIButton*>(panel->getChildByName("button2"));
        random_Button->setVisible(true);           // 暂时屏蔽随即按钮
        
        for(int i = 0; i < m_pAwardButtonArray->count(); i++)
        {
            UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
            UIImageView* image_Price =static_cast<UIImageView*>(pButton->getChildByName("ImageView4"));
            image_Price->setVisible(true);
            pButton->setScaleX(-1);
        }
    }
}

void PopUpUI::menuGoCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if(((UIWidget*)pSender)->isVisible())
        {
            removeChild(m_pUITouchGroup);
            m_pUITouchGroup = NULL;
            m_pAwardButtonArray->release();
            m_pAwardPositionArray->release();
            m_pChooseAwardArray->release();
            
            Controller::getInstance()->sendMessage(ASK_RETURN_CITY,false);
            Controller::getInstance()->switchGameStatus(kStatusLoading); // 设置游戏状态为loading状态
        }
    }
}

void PopUpUI::menuRandomCallback(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (isTouchUnable(pSender, type))
        {
            return;
        }
        if(!((UIWidget*)pSender)->isVisible())
        {
            return;
        }
        else if (getAwardActionStatus() == kAwardActionDefaultStatus)
        {
            if(m_nCurrentAwardCount == 0)
            {
                if (m_nNeedJewelCount > 0) // 判断是否是消耗钻石翻牌
                {
                    setConsumeJewelTips(kJewelConsumeForFlopAward, m_nNeedJewelCount);
                    return;
                }
            }
            setAwardActionStatus(kAwardActionRandomStatus);
            setAwardStatus(kAwardNotClickStatus);
            m_pRandomImage = UIImageView::create();
            string str = getFilePath("315", kDirTypeUI, kFileSuffixPng);
            m_pRandomImage->loadTexture(str.c_str());
            UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(0);
            m_pRandomImage->setPosition(ccp(pButton->getPosition().x , pButton->getPosition().y - 10));
            UIPanel* panel3 = static_cast<UIPanel*>(m_pUITouchGroup->getWidgetByName("Panel3"));
            panel3->addChild(m_pRandomImage);
        }
    }
}

void PopUpUI::updateFlopCall(CCNode* pSender)
{
    UIButton* pTempButton = (UIButton*)pSender;//m_pAwardButtonArray->objectAtIndex((int)i);
    if(pTempButton->getTag() == TAG_BACK_FACE){
        return;
    }
    UIImageView* image_Award = static_cast<UIImageView*>(pTempButton->getChildByName("ImageView0"));
    image_Award->setVisible(false);
    UIImageView* image_BackFace = static_cast<UIImageView*>(pTempButton->getChildByName("ImageView1"));
    image_BackFace->setVisible(true);
    UIImageView* image_Lock = static_cast<UIImageView*>(pTempButton->getChildByName("ImageView2"));
    image_Lock->setVisible(false);
    UIImageView* image_Exclude = static_cast<UIImageView*>(pTempButton->getChildByName("ImageView3"));
    image_Exclude->setVisible(false);
    UIImageView* image_Box = static_cast<UIImageView*>(pTempButton->getChildByName("ImageView5"));
    image_Box->setVisible(false);
    if(pTempButton->getChildByTag(TAG_AWARD_STROKE))
    {
        pTempButton->getChildByTag(TAG_AWARD_STROKE)->setVisible(false);
    }
    if(pTempButton->getChildByTag(TAG_AWARD_NAME))
    {
        pTempButton->getChildByTag(TAG_AWARD_NAME)->setVisible(false);
    }
}

void PopUpUI::updateCombineCall(CCNode* pSender)
{
    UIButton* pTempButton = (UIButton*)pSender;//m_pAwardButtonArray->objectAtIndex((int)i);
    if(pTempButton)
    {
        if(pTempButton->getTag() == TAG_BACK_FACE){
            pTempButton->removeFromParent();
            return;
        }
        pTempButton->runAction(CCMoveTo::create(0.5, ccp(m_pUITouchGroup->getContentSize().width / 2,m_pUITouchGroup->getContentSize().height / 2- pTempButton->getSize().height / 2) - m_cAdjustScreenPosition));
    }
}

void PopUpUI::updateAwardUI()
{
    switch (getAwardActionStatus())
    {
        case kAwardActionShuffleStatus:
        {
            static int i = 0;
            int shuffleCount = 4;
            if(m_pAwardButtonArray->count() < shuffleCount && i == m_pAwardButtonArray->count())
            { // 只让洗shuffleCount次牌 但是防止牌数过少低于每张牌洗牌次数(保护)
                i = 0;
            }
            UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
            if(i == shuffleCount-1 && pButton->getActionByTag(100) == NULL)  // 每张牌结束动作
            {
                setAwardActionStatus(kAwardActionRestoreStatus);
                i = 0;
                return;
            }
            
            CCFiniteTimeAction* action = NULL;
            CCActionInterval*  move = NULL;
            if(i%2 == 1) // 左右移动
            {
                move = CCMoveBy::create(0.25, ccp(- 200 , 0));
                action = CCSequence::create(move,CCCallFuncND::create(this, callfuncND_selector(PopUpUI::updateZorderCall),(void*)(i+1)),move->reverse(), NULL);
            }
            else
            {
                move = CCMoveBy::create(0.25, ccp(200 , 0));
                action = CCSequence::create(move,CCCallFuncND::create(this, callfuncND_selector(PopUpUI::updateZorderCall),(void*)(i+1)),move->reverse(), NULL);
            }
            
            if(pButton->getActionByTag(100) == NULL) // 左右洗牌动作标签
            {
                i++;
                pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
                pButton->runAction(action);
                action->setTag(100);
            }
        }
            break;
        case kAwardActionRestoreStatus:
        {
            for(int i = 0;i < m_pAwardButtonArray->count();++i)
            {
                UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(i);
                UIButton* pPosition = (UIButton*)m_pAwardPositionArray->objectAtIndex(i);
                pButton->runAction(CCSequence::create(CCMoveTo::create(0.5, pPosition->getPosition()),CCCallFuncND::create(this, callfuncND_selector(PopUpUI::updateOverCall),(void*)(i)), NULL));   // 等所有动作结束才能开启按钮
                if(i == m_pAwardButtonArray->count() - 1)
                {
                    setAwardActionStatus(kAwardActionDefaultStatus);
                }
            }
        }
            break;
        case kAwardActionRandomStatus:
        {
            if(m_nStartFrame < m_nIntervalFrame)
            {
                m_nStartFrame++;
            }
            else
            {
                if(m_nRandomCount == m_nRandomIndex)
                {
                    setAwardActionStatus(kAwardActionDefaultStatus);
                    setAwardStatus(kAwardReceiveStatus);
                    UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(m_nRandomCount);
                    m_pShuffle_Button = pButton;
                    m_nStartFrame = 0;
                    m_nRandomCount = 0;
                    m_nRandomIndex = -1;
                    CCString *pStrMsg = CCString::createWithFormat(ASK_TURN_UP_CARD, 0);
                    std::string strMsg = pStrMsg->getCString();
                    Controller::getInstance()->sendMessage(strMsg.c_str(), true);
                    break;
                }
                m_nStartFrame = 0;
                m_nRandomCount ++;
                if(m_nRandomCount == m_pAwardButtonArray->count()) // 到达最后一个时开始重新判断频率
                {
                    m_nIntervalFrame = m_nIntervalFrame/2 ;
                    if(m_nIntervalFrame == 0)
                    {
                        m_nIntervalFrame = INTERVAL_FRAME;
                        time_t t;
                        std::srand((unsigned int)time(&t)); // 随机数种子
                        m_nRandomIndex = std::rand() %  m_pAwardButtonArray->count();
                    }
                    m_nRandomCount =0;
                }
                UIButton* pButton = (UIButton*)m_pAwardButtonArray->objectAtIndex(m_nRandomCount);
                m_pRandomImage->setPosition(ccp(pButton->getPosition().x , pButton->getPosition().y - 10));
            }
        }
            break;
            
        default:
            break;
    }
}
#pragma mark 战斗评价

// 通用评分公式（和难度无关） =min（10 * min（max（200/实际掉血量，0.6），1.2）+ 10 * min（max（20 /被攻击数，0.6），1.2）+ 25 * min（max（combo数/ 66，0.6），1.2）+45 * min（max（150/通关时间，0.6），1.2）+ 10 * min（max（总伤害/40000，0.6），1.2），100）
// 通关后结算分数——(0,55)一星   (56,90)二星   (91,100)三星
void PopUpUI::stageRating()
{
    int currentTime = Controller::getInstance()->getCurrentSecond();
    int nStageDuration = ((YYScene *)getParent())->getStageDuration();
    nStageDuration += currentTime - ((YYScene *)getParent())->getCurrentTime();
    ((YYScene *)getParent())->setStageDuration(nStageDuration);
    int nHPDecrement = ((YYScene *)getParent())->getHPDecrement();
    int nHittedAmount = ((YYScene *)getParent())->getHittedAmount();
    int nMaxComboCount = ((YYScene *)getParent())->getMaxComboCount();
    int nStageDamage = ((YYScene *)getParent())->getStageDamage();
    float score = MIN(10 * MIN(MAX(200.0f / nHPDecrement, 0.6), 1.2) + 10 * MIN(MAX(20.0f / nHittedAmount, 0.6), 1.2) + 25 * MIN(MAX(nMaxComboCount / 66.0f, 0.6),12) + 45 * MIN(MAX(150.0f / nStageDuration, 0.6),1.2) + 10 * MIN(MAX(nStageDamage / 40000.0f, 0.6),1.2), 100);
    if (score < 56)
    {
        m_u8StageStartLevel = 1;
    }
    else if(score < 91)
    {
        m_u8StageStartLevel = 2;
    }
    else
    {
        m_u8StageStartLevel = 3;
    }
    ((YYScene *)getParent())->setStageStartLevel(m_u8StageStartLevel);
    
    m_pAwardString = ((YYScene *)getParent())->getAwardString();
    CCArray * array = split_string(m_pAwardString->getCString(), ';'); // 固定奖励
    
    m_u8ChoosedDifficultOptionIndex = ((YYScene *)getParent())->getChoosedStageDifficultIndex();
    // 按难度获得随机奖励
    m_pAwardDictionary = ((YYScene *)getParent())->getAwardDictionary();
    CCString * awardStr = (CCString *)m_pAwardDictionary->objectForKey(m_u8ChoosedDifficultOptionIndex);
    array->addObjectsFromArray(split_string(awardStr->getCString(), ';'));
    
    if (!m_pAwardArray)
    {
        m_pAwardArray = new CCArray(5);
    }
    else
    {
       m_pAwardArray->removeAllObjects();
    }
    m_u8ShowItemAmount = 4;//初始化待显示项的数量为星级（5个数字3颗星星）
    for (int i = 0; i < 4; i++)
    {
        CCArray * array1 = split_string(((CCString *)array->objectAtIndex(i))->getCString(),',');
        if(i % 4 <= m_u8StageStartLevel)
        {
            int rand = RANDOM_INT(0, 99);
            if (rand < ((CCString *)array1->objectAtIndex(2))->intValue())
            {
                m_pAwardArray->addObject(array->objectAtIndex(i));
                m_u8ShowItemAmount += 2;//每增加一个奖励，待显示项增加图标和数量两项
            }
        }
    }
    if (!m_pBonusEffectArray)
    {
        m_pBonusEffectArray = new CCArray(5);
    }
    else
    {
        m_pBonusEffectArray->removeAllObjects();
    }
    CCString* pString = (CCString*)m_pAwardDictionary->objectForKey(m_u8ChoosedDifficultOptionIndex);
    CCArray* pSeparateArray = split_string(pString->getCString(), ';'); // 分离最后两个属性
    m_pAwardArray->addObject(pSeparateArray->objectAtIndex(pSeparateArray->count() - 1));// 经验
    m_pAwardArray->addObject(pSeparateArray->objectAtIndex(pSeparateArray->count() - 2));// 金钱
}

void PopUpUI::initFightEvaluationUI()
{
    Controller::getInstance()->setGameStatus(kStatusUI);
    setUIType(kUIFightEvaluation);
    m_pRoleByControlled = ((YYScene *)getParent())->getControlUILayer()->getRoleByControlled();
    if (m_pUITouchGroup)
    {
        removeChild(m_pUITouchGroup);
        m_pUITouchGroup = NULL;
    }
    m_pUITouchPop = TouchGroup::create();
    CC_ASSERT(m_pUITouchPop);
    addChild(m_pUITouchPop);
    m_pUITouchPop->setPosition(m_cAdjustScreenPosition);
    std::string filePath = FIGHT_EVALUATION_UI_RESOURCE_FILE_NAME;
    m_pUITouchPop->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    //退出按键
    UIPanel* panel0 = static_cast<UIPanel*>(m_pUITouchPop->getWidgetByName("Panel0"));
    CC_ASSERT(panel0);
    UIButton *pButton_sure = static_cast<UIButton*>(panel0->getChildByName("Button_done"));
    CC_ASSERT(pButton_sure);
    pButton_sure->setTouchEnabled(true);
    pButton_sure->addTouchEventListener(this, toucheventselector(PopUpUI::fightEvaluationSureCallBack));
    
    // 清除按键状态
    ((YYScene *)getParent())->getControlUILayer()->clearKeyState();
    //stageRating();
    // 切换状态
    //Controller::getInstance()->switchGameStatus(kStatusFightResult);
    m_u8CurrentActiveItemIndex = 0;// 当前活动项为胜利出现时的动画
    //CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCSize winSize;
    winSize.width = 960;
    winSize.height = 640;
    const int bgWidth = 860,bgHeight = 540;
    int bgWidth2 = 780,bgHeight2 = 380;
    CCPoint initPosition = ccp((winSize.width - bgWidth) / 2, (winSize.height - bgHeight) / 2);
    int offsetX = initPosition.x + (bgWidth - bgWidth2) / 2.0f,offsetY = initPosition.y + (bgHeight - bgHeight2) / 2.0f;
    
    YYSprite * aniSprite = YYSprite::createWithActionSet("96", "96", kDirTypeEffect);
    aniSprite->setPosition(ccp(winSize.width / 2, offsetY + bgHeight2 - 70));
    m_pUITouchPop->addChild(aniSprite);
    aniSprite->setVisible(false);
    int tag = TAG_FIRST_ACTIVE_ITEM;
    aniSprite->setTag(tag++);
    aniSprite->setAction(0, false);
    
    CCLabelTTF *label = CCLabelTTF::create(g_sFightResult, DEFAULT_FONT_NAME, 30);
    float nameLabelWidth = label->getContentSize().width;
    const char *temp = CCString::createWithFormat("%d", ((YYScene *)getParent())->getStageDuration())->getCString();
    label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
    label->setPosition(ccp(offsetX + 30 + (bgWidth2 / 2 - 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 90));
    label->setAnchorPoint(ccp(0.5,1));
    m_pUITouchPop->addChild(label);
    label->setTag(tag++);
    label->setVisible(false);
    
    temp = CCString::createWithFormat("%d", ((YYScene *)getParent())->getMaxComboCount())->getCString();
    label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
    label->setPosition(ccp(offsetX + 30 + (bgWidth2 / 2 - 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 150));
    label->setAnchorPoint(ccp(0.5,1));
    m_pUITouchPop->addChild(label);
    label->setTag(tag++);
    label->setVisible(false);

    temp = CCString::createWithFormat("%d", ((YYScene *)getParent())->getKillCount())->getCString();
    label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
    label->setPosition(ccp(offsetX + 30 + (bgWidth2 / 2 - 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 210));
    label->setAnchorPoint(ccp(0.5,1));
    m_pUITouchPop->addChild(label);
    label->setTag(tag++);
    label->setVisible(false);

    m_u8StageStartLevel = ((YYScene *)getParent())->getStageStartLevel();
    for (int i = 0; i < 3; i++)
    {
        aniSprite = YYSprite::createWithActionSet("96", "96", kDirTypeEffect);
        aniSprite->setPosition(ccp((winSize.width + bgWidth2 / 2 + 30 + nameLabelWidth) / 2 - 78 + 78 * i, offsetY + bgHeight2 - 170));
        m_pUITouchPop->addChild(aniSprite);
        aniSprite->setTag(tag++);
        aniSprite->setVisible(false);
        if (i < m_u8StageStartLevel)
        {
            aniSprite->setAction(2, false);
        }
        else
        {
            aniSprite->setAction(1, false);
        }
    }
    int expAward = ((CCString *)m_pAwardArray->objectAtIndex(m_pAwardArray->count() - 2))->intValue();//经验奖励
    temp = CCString::createWithFormat("%d",expAward)->getCString();
    label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
    label->setPosition(ccp((winSize.width + bgWidth2 / 2 + 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 150));
    label->setAnchorPoint(ccp(0.5,1));
    m_pUITouchPop->addChild(label);
    label->setTag(tag++);
    label->setVisible(false);

    int bonus = ((CCString *)m_pAwardArray->objectAtIndex(m_pAwardArray->count() - 1))->intValue();//奖金
    temp = CCString::createWithFormat("%d",bonus)->getCString();
    label = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 25);
    label->setPosition(ccp((winSize.width + bgWidth2 / 2 + 30 + nameLabelWidth) / 2, offsetY + bgHeight2 - 210));
    label->setAnchorPoint(ccp(0.5,1));
    m_pUITouchPop->addChild(label);
    label->setTag(tag++);
    label->setVisible(false);
    
    for (int i = 0; i < 5; i++)
    {
        // 物品栏
        string str = getFilePath("208", kDirTypeUI, kFileSuffixPng);
        CCSprite * sprite = CCSprite::create(str.c_str());
       // m_pUITouchPop->addChild(sprite);
        
        float tempWidth = sprite->getContentSize().width;
        int interval = 26;
        float tempX = offsetX + 172 + label->getContentSize().width + tempWidth / 2 + i * (tempWidth + interval);
        sprite->setPosition(ccp(tempX,offsetY + bgHeight2 - 300));
        
        if (i < m_pAwardArray->count() - 2) {//m_pAwardArray最后两位是金钱、经验
            CCArray *array = split_string(((CCString *)m_pAwardArray->objectAtIndex(i))->getCString(), ',');
            // 物品图标
            const char *iconName = ((CCString *)array->objectAtIndex(0))->getCString();
            string str = getFilePath(iconName, kDirTypeIcon, kFileSuffixPng);
            sprite = CCSprite::create(str.c_str());
            sprite->setPosition(ccp(tempX,offsetY + bgHeight2 - 300));
            m_pUITouchPop->addChild(sprite);
            sprite->setTag(tag++);
            sprite->setVisible(false);
            
            // 奖励物品数量
            int num = ((CCString *)array->objectAtIndex(1))->intValue();
            temp = CCString::createWithFormat("X%d",num)->getCString();
            CCLabelTTF *label1 = CCLabelTTF::create(temp, DEFAULT_FONT_NAME, 20);
            label1->setColor(ccRED);
            label1->setPosition(ccp(tempX + (tempWidth + interval) / 2, offsetY + bgHeight2 - 300));
            m_pUITouchPop->addChild(label1);
            label1->setTag(tag++);
            label1->setVisible(false);
        }
    }
}
void PopUpUI::fightEvaluationSureCallBack(CCObject *pSender, cocos2d::gui::TouchEventType type)
{
    if (((YYScene *)getParent())->isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        const std::string filePath = getFilePath(CARD_EFFECT ,kDirTypeSound ,kFileSuffixWav);
        PLAY_EFFECT(filePath.c_str());
        if (isTouchUnable(pSender, type, false))
        {
            return;
        }
        if (m_u8CurrentActiveItemIndex < m_u8ShowItemAmount)
            return;
        showFlopAward();
    }
}
void PopUpUI::sendFightResultConfirmMessage()
{
    //std::string tempStr;
    m_u8DungeonsIndex = ((YYScene *)getParent())->getDungeonsIndex();
    m_u8ChoosedStageIndex = ((YYScene *)getParent())->getChoosedStageIndex();
    m_u8ChoosedDifficultOptionIndex = ((YYScene *)getParent())->getChoosedStageDifficultIndex();
    m_u8StageStartLevel = ((YYScene *)getParent())->getStageStartLevel();
    const char *temp = CCString::createWithFormat(SEND_FIGHT_RESULT, m_u8StageStartLevel)->getCString();
    Controller::getInstance()->sendMessage(temp,false);
}
void PopUpUI::updateFightResult()
{
    CCNode * node = m_pUITouchPop->getChildByTag(TAG_FIRST_ACTIVE_ITEM + m_u8CurrentActiveItemIndex);
    if (node)
    {
        node->setVisible(true);
        if (dynamic_cast<YYSprite *>(node))
        {
            if (((YYSprite * )node)->isActionOver())
            {
                m_u8CurrentActiveItemIndex++;
            }
            else
            {
                ((YYSprite * )node)->updateAnimation();
            }
        }
        else
        {
            m_u8CurrentActiveItemIndex++;
        }
        if(m_u8CurrentActiveItemIndex > 10)
        {
            if (m_u8CurrentActiveItemIndex % 2 == 1)
            {
                CCPoint position = m_pUITouchPop->getChildByTag(TAG_FIRST_ACTIVE_ITEM + m_u8CurrentActiveItemIndex - 2)->getPosition();
                position.x = position.x - 3;
                YYSprite * sprite = YYSprite::createWithActionSet("se6", "se6", kDirTypeEffect);
                sprite->setAction(0, false);
                sprite->setPosition(position);
                m_pUITouchPop->addChild(sprite);
                m_pBonusEffectArray->addObject(sprite);
                const std::string filePath = getFilePath(REWARDSTAR_EFFECT ,kDirTypeSound ,kFileSuffixWav);
                PLAY_EFFECT(filePath.c_str());
            }
        }
    }
    for (int i = 0; i < m_pBonusEffectArray->count();)
    {
        YYSprite * sprite = (YYSprite *)m_pBonusEffectArray->objectAtIndex(i);
        if (sprite->isActionOver())
        {
            m_pBonusEffectArray->removeObjectAtIndex(i);
        }
        else
        {
            sprite->update();
            i++;
        }
    }
}
