//
//  YYScene.cpp
//  XJ
//
//  Created by fuchenhao on 13-4-19. 
//
//
#include "SimpleAudioEngine.h"
#include "../common/Common.h"
#include "YYScene.h"
#include "YYLogo.h"
#include "YYLoading.h"
#include "YYCover.h"
#include "YYLoginUI.h"
#include "YYControlUILayer.h"
#include "YYSpriteLayer.h"
#include "YYLayer.h"
#include "YYPlayer.h"
#include "YYButton.h"
#include "YYDragRegion.h"
#include "YYCellLayer.h"
#include "YYSpriteData.h"
#include "YYSpriteAttribute.h"
#include "YYPopUpUI.h"
#include "YYTaskTargetData.h"
#include "YYGlobalData.h"
#include "YYTaskData.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

USING_NS_CC;
using namespace CocosDenshion;

// 场景分层 
#define BACKGROUND_LAYER 0
#define FARSCENERY_LAYER 1
#define MIDSCENERY_LAYER 2
#define NEARSCENERY_LAYER 3
#define ROAD_LAYER 4
#define CELL_LAYER 5
#define FIGHT_SPRITE_LAYER 6
#define CLOSE_LAYER 7
#define CONTROL_UI_LAYER 8
#define POP_UI_LAYER 9
#define TEMP_LAYER 10// logo/cover/loginUI
#define NET_CONNECTING_LAYER 15//用于表现网络连接中（转菊花）
#define TIPS_LAYER 16//提示信息层
#define LOADING_LAYER 20// 用于表现资源加载中

// 网络加载时播放的精灵动画 
#define TAG_NET_CONNECTING_MASKING 100

#define TAG_FIRST_MASKING 6 // 拖动身上装备第一个蒙版 
#define TAG_CLOSE_STAGE_UI 8// 关闭场景UI的图标 
#define TAG_ENTER_STAGE_BUTTON 10// 确定进入关卡按键 
#define TAG_RETURN_CITY_1 11/* 返回主城(副本中)*/

#define TIPS_WINDOW_JSON_FILE "NewProject_6"

#pragma mark 构造、析构、初始化 

YYScene::YYScene()
:m_pLoading(NULL)
,m_pSpriteLayer(NULL)// 战斗精灵层
,m_pControlUILayer(NULL)// 虚拟按键控制层 
,m_pPopUpUI(NULL)
,m_pSceneNameLabel(NULL)// 场景名称标签 
,m_pBackground(NULL)// 背景 
,m_pFarScenery(NULL)// 远景 
,m_pMidScenery(NULL)// 中景 
,m_pNearScenery(NULL)// 近景 
,m_pRoadLayer(NULL)// 道路 
,m_pCloseLayer(NULL)//
,m_pCellLayer(NULL)
,m_pBlockTile(NULL)// 单元格是否有障碍
,m_pTipPanel(NULL)// 提示信息面板
,m_ePreTipType(kTipDefault)
,m_eTipType(kTipDefault)
,m_sTipsInfo("")
,m_bNetLoading(false)// 是否联网加载中
,m_bTouchBegan(false)
,m_bTouchMoved(false)
,m_bTouchEnded(false)
,m_bVibrate(false)// 震屏 
,m_bCameraMovedByScript(false)// 脚本控制镜头移动
,m_bCameraLocked(false)// 镜头默认无锁定(跟随主角) 
,m_bCameraFollowSprite(false)
,m_nVibrateOffsetX(0)
,m_nVibrateOffsetY(0)
,m_nCurrentElementNum(-1)
,m_nFightResultDelayCounter(-1)//战斗评价延迟 
,m_SceneSize(CCSizeZero)
,m_ScreenDatumPoint(CCPointZero)
,m_u8DungeonStatus(-1)// 副本界面状态
,m_pTurntable(NULL)
,m_pTurntableBatchNode(NULL)
,m_bCloseChoosed(false)// 关闭关卡被选中 
,m_bStageItemScroll(false)// 关卡选项滚动 
,m_bStageItemChoosed(false)// 选中关卡选项 
,m_bEnter(false)
,m_bConfirm(false)
,m_pDungeonsTitelLabel(NULL)
,m_pDungeonsTitleFrame(NULL)
,m_pDungeonsDataArray(NULL)
,m_pIsChoosedDungeon(NULL)
,m_pStageDataArray(NULL)
,m_pAwardString(NULL)// 固定关卡奖励
,m_pAwardDictionary(NULL)// 随机关卡奖励
//,m_pAwardArray(NULL)// 战斗评价后获得的物品奖励
, m_nStageDuration(0)// 通关时间
, m_nStageDamage(0)// 造成的伤害 
, m_nHPDecrement(0)// 减血量 
, m_nHittedAmount(0)// 受击次数 
, m_nMaxComboCount(0)// 最大连击数 
, m_nKillCount(0)// 杀敌数量 
,m_u8StageStartLevel(0)// 关卡评级 
,m_u16TileColumnAmount(0)
,m_u16TileRowAmount(0)
,m_u16TileWidth(0)
,m_u16TileHeight(0)
,m_nCurrentTime(0)
,m_nVibrateCounter(0)
,m_nVibrateDuration(0)
,m_nDeadConfirmDuration(0)
,m_nDeadConfirmCounter(0)
,m_fCurrentScrollDistance(0)
,m_fMaxScrollDistance(0)
,m_u8DungeonAmount(0)
,m_u8DungeonsIndex(-1)
,m_fRotateAngle(0)
,m_fMaxAngle(0)
,m_nTurntableOffsetY(0)
,m_u8StageAmount(0)
,m_u8ChoosedStageIndex(0)
,m_u8UnlockDifficultOptions(0)
,m_u8ChoosedDifficultOptionIndex(0)
,m_u8ShowItemAmount(0)// 战斗评价界面全部显示项的数量 
{
}

YYScene::~YYScene()
{
    CC_SAFE_DELETE(m_pBlockTile);
    CC_SAFE_DELETE(m_pDungeonsDataArray);
    CC_SAFE_DELETE(m_pStageDataArray);
    CC_SAFE_DELETE_ARRAY(m_pIsChoosedDungeon);
    CC_SAFE_DELETE(m_pAwardDictionary);
    CC_SAFE_DELETE(m_pAwardString);
}


void YYScene::addLogo()
{
    m_pTempLayer = Logo::create();
    addChild(m_pTempLayer,TEMP_LAYER);
    initNetConnectLayer();
    initPopUpWindow();
}

void YYScene::addCover()
{
    removeChild(m_pTempLayer);
    m_pTempLayer = Cover::create();
    addChild(m_pTempLayer,TEMP_LAYER);
}

void YYScene::addLoginUI()
{
    if(m_pLoading)
    {
        removeChild(m_pLoading);
        m_pLoading = NULL;
    }
    removeChild(m_pTempLayer);
    m_pTempLayer = LoginUI::create();
    addChild(m_pTempLayer,TEMP_LAYER);
}

void YYScene::addLoadingLayer()
{
    removeChild(m_pTempLayer);
    m_pLoading = Loading::create();
    addChild(m_pLoading,LOADING_LAYER);
}

void YYScene::resetGameScene()
{
    removeChild(m_pBackground);
    removeChild(m_pFarScenery);
    removeChild(m_pMidScenery);
    removeChild(m_pNearScenery);
    removeChild(m_pRoadLayer);
    removeChild(m_pCloseLayer);
    removeChild(m_pSpriteLayer);
    removeChild(m_pControlUILayer);
    removeChild(m_pPopUpUI);
    m_pBackground = CCLayerColor::create();
    m_pBackground->setAnchorPoint(CCPointZero);
    m_pBackground->setPosition(CCPointZero);
    addChild(m_pBackground,BACKGROUND_LAYER);
    m_pFarScenery = CCLayer::create();
    m_pFarScenery->setAnchorPoint(CCPointZero);
    m_pFarScenery->setPosition(CCPointZero);
    addChild(m_pFarScenery,FARSCENERY_LAYER);
    m_pMidScenery = CCLayer::create();
    m_pMidScenery->setAnchorPoint(CCPointZero);
    m_pMidScenery->setPosition(CCPointZero);
    addChild(m_pMidScenery,MIDSCENERY_LAYER);
    m_pNearScenery = CCLayer::create();
    m_pNearScenery->setAnchorPoint(CCPointZero);
    m_pNearScenery->setPosition(CCPointZero);
    addChild(m_pNearScenery,NEARSCENERY_LAYER);
    m_pRoadLayer = CCLayer::create();
    m_pRoadLayer->setAnchorPoint(CCPointZero);
    m_pRoadLayer->setPosition(CCPointZero);
    addChild(m_pRoadLayer,ROAD_LAYER);
    m_pCloseLayer = CCLayer::create();
    m_pCloseLayer->setAnchorPoint(CCPointZero);
    m_pCloseLayer->setPosition(CCPointZero);
    addChild(m_pCloseLayer,CLOSE_LAYER);
    m_pSpriteLayer = YYSpriteLayer::create();
    m_pSpriteLayer->setAnchorPoint(CCPointZero);
    m_pSpriteLayer->setPosition(CCPointZero);
    addChild(m_pSpriteLayer,FIGHT_SPRITE_LAYER);
    m_pControlUILayer = YYControlUILayer::create();
    m_pControlUILayer->setAnchorPoint(CCPointZero);
    m_pControlUILayer->setPosition(CCPointZero);
    addChild(m_pControlUILayer,CONTROL_UI_LAYER);
    m_pPopUpUI = PopUpUI::create();
    m_pPopUpUI->setAnchorPoint(CCPointZero);
    m_pPopUpUI->setPosition(CCPointZero);
    addChild(m_pPopUpUI,POP_UI_LAYER);
    m_pControlUILayer = YYControlUILayer::create();
    m_pControlUILayer->setAnchorPoint(CCPointZero);
    m_pControlUILayer->setPosition(CCPointZero);
    addChild(m_pControlUILayer,CONTROL_UI_LAYER);
    m_pPopUpUI = PopUpUI::create();
    m_pPopUpUI->setAnchorPoint(CCPointZero);
    m_pPopUpUI->setPosition(CCPointZero);
    addChild(m_pPopUpUI,POP_UI_LAYER);
}

void YYScene::showGameScene()
{
    removeChild(m_pLoading);
}

void YYScene::onCmdLoadScene(cocos2d::CCArray *arr)
{
    do
    {
        int i = 0;
        CCString *str = static_cast<CCString *>(getNextElement(arr, i));
        CC_BREAK_IF(!str);
        if (strcmp(str->getCString(), "A") == 0)
        {
            resetGameScene();
            Controller::getInstance()->setLoadingOverStatus(kLOSGameScene);
            CCString *tempStr = static_cast<CCString *>(getNextElement(arr, i));
            CC_BREAK_IF(!tempStr);
            setSceneNameLabel(tempStr->getCString());
            tempStr = static_cast<CCString *>(getNextElement(arr, i));
            CC_BREAK_IF(!tempStr);
            Controller::getInstance()->setSceneType(tempStr->intValue());
            tempStr = static_cast<CCString *>(getNextElement(arr, i));
            CC_BREAK_IF(!tempStr);
            Controller::getInstance()->setLineIndex(tempStr->intValue());
        }
        else
        {
            arr->removeObjectAtIndex(0);
            setSceneElements(str->intValue(),arr);
        }
    }while(0);
}

void YYScene::setSceneData(cocos2d::CCArray *array)
{
    do
    {
        int i =0;
        CCString *tempStr = static_cast<CCString *>(getNextElement(array, i));
        CC_BREAK_IF(!tempStr);
        m_u16TileRowAmount = tempStr->intValue();// 行
        tempStr = static_cast<CCString *>(getNextElement(array, i));
        CC_BREAK_IF(!tempStr);
        m_u16TileColumnAmount = tempStr->intValue();// 列
        tempStr = static_cast<CCString *>(getNextElement(array, i));
        CC_BREAK_IF(!tempStr);
        int height = tempStr->intValue();// 场景像素高
        tempStr = static_cast<CCString *>(getNextElement(array, i));
        CC_BREAK_IF(!tempStr);
        int width = tempStr->intValue();// 场景像素宽
        setSceneSize(CCSizeMake(width,height));
        m_u16TileWidth = width / m_u16TileColumnAmount;
        m_u16TileHeight = Controller::getInstance()->getSceneCellTopY() / m_u16TileRowAmount;
    }while (0);
}

void YYScene::initNetConnectLayer()
{
    CCLayer *layer = CCLayer::create();
    layer->setAnchorPoint(CCPointZero);
    layer->setPosition(CCPointZero);
    addChild(layer,NET_CONNECTING_LAYER,TAG_NET_CONNECTING_MASKING);
    YYSprite * sprite = YYSprite::createWithActionSet("loading","loading0,loading1,loading2,loading3,loading4,loading5",kDirTypePic);
    CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
    sprite->setPosition(ccp(screenSize.width / 2.0f, screenSize.height / 2.0f));
    sprite->setAction(0,true);
    layer->addChild(sprite,0,1);
    layer->setVisible(false);
}

void YYScene::initPopUpWindow()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    TouchGroup *touchGroup = TouchGroup::create();
//    touchGroup->setAnchorPoint(CCPointZero);
    touchGroup->setPosition(ccp((winSize.width - STANDARD_SCENE_WIDTH) / 2,(winSize.height - STANDARD_SCENE_HEIGHT) / 2));
    addChild(touchGroup,TIPS_LAYER);
    touchGroup->setTag(TAG_POPUP_WINDOW);
    
    std::string filePath = getFilePath(TIPS_WINDOW_JSON_FILE,kDirTypeUI,kFileSuffixJson);
    touchGroup->addWidget(GUIReader::shareReader()->widgetFromJsonFile(filePath.c_str()));
    Layout * panel = static_cast<Layout *>(touchGroup->getWidgetByName("Panel"));
    for (int i = 0; i < panel->getChildrenCount(); i++)
    {
        Widget * widget= static_cast<Widget *>(panel->getChildren()->objectAtIndex(i));
        widget->setEnabled(false);
        widget->setVisible(false);
    }
}

/**
 * 设置场景元素
 */
void YYScene::setSceneElements(int layerIndex,CCArray * array)
{
    CCLayer * layer = NULL;
    switch (layerIndex)
    {
        case 0:
            layer = m_pBackground;
            break;
        case 1:
            layer = m_pFarScenery;
            break;
        case 2:
            layer = m_pMidScenery;
            break;
        case 3:
            layer = m_pNearScenery;
            break;
        case 4:
            layer = m_pRoadLayer;
            break;
        case 5:
            layer = m_pCloseLayer;
            break;
        default:
            break;
    }
    if (layerIndex == 0)
    {
        int n = ((CCString *)array->objectAtIndex(0))->intValue();
        if (n == 1) {
            int color = strtoul(((CCString *)array->objectAtIndex(1))->getCString(), NULL, 16);
            ccColor4B color4 = ccc4(color>>16, (color&0x00FF00)>>8, color&0x0000FF, 255);
            m_pBackground->initWithColor(color4);
            return;
        }
    }
    CCString * str = (CCString *)array->objectAtIndex(0);// 图片资源信息 
    CCArray * array0 = split_string(str->getCString(), ';');
    for (int i = 0; i < array0->count(); i++)
    {
        CCString * str1 = (CCString *)array0->objectAtIndex(i);// 单个图片信息 
        CCArray * array1 = split_string(str1->getCString(), '&');
        const char * pngFileName = ((CCString *)array1->objectAtIndex(0))->getCString();
        const std::string path = getFilePath(pngFileName, kDirTypeScene, kFileSuffixPng);
        for (int j = 1; j < array1->count(); j++)
        {
            CCString * position = (CCString *)array1->objectAtIndex(j);
            CCArray * array3 = split_string(position->getCString(), ',');
            int x = ((CCString *)array3->objectAtIndex(0))->intValue();
            int y = ((CCString *)array3->objectAtIndex(1))->intValue();
            int transform = ((CCString *)array3->objectAtIndex(2))->intValue();
            CCSprite * sprite = CCSprite::create(path.c_str());
            sprite->setPosition(ccp(x,y));
            sprite->setAnchorPoint(ccp(0,1));
            if (transform == 1)
            {
                sprite->setFlipX(true);
            }
            else if(transform == 2)
            {
                sprite->setFlipY(true);
            }
            layer-> addChild(sprite);
        }
    }
}

/**
 *
 */
void YYScene::setNetLoading(bool isNetLoading)
{
    m_bNetLoading = isNetLoading;
    CCLayer * layer = (CCLayer *) getChildByTag(TAG_NET_CONNECTING_MASKING);
    layer->setVisible(isNetLoading);
}

/**
 * 设置场景名
 */
void YYScene::setSceneNameLabel(const char * nameStr)
{
    m_pSceneNameLabel = CCLabelTTF::create(nameStr, DEFAULT_FONT_NAME, 24);
    m_pSceneNameLabel->setPosition(ccp(5 + m_pSceneNameLabel->getContentSize().width / 2, 5 + m_pSceneNameLabel->getContentSize().height / 2));
    CCRenderTexture * renderTex = Controller::getInstance()->createStroke(m_pSceneNameLabel, 2, ccBLACK);
    m_pControlUILayer->addChild(renderTex);
    m_pControlUILayer->addChild(m_pSceneNameLabel);
}

#pragma mark 触屏控制 

void YYScene::touchBegan(CCSet *touches)
{
    if (m_bNetLoading)
    {
        return;
    }
    CCTouch * touch = (CCTouch *)touches->anyObject();
    switch (Controller::getInstance()->getGameStatus())
    {
        case kStatusCover:
            dynamic_cast<Cover *>(m_pTempLayer)->touchBegan(touch);
            break;
        case kStatusUI:
            m_pPopUpUI->touchBegan(touch);
            break;
        case kStatusScene:
            m_pControlUILayer->touchBegan(touch);
            m_TempPoint =touch->getLocation();
            break;
        default:
            break;
    }
}

void YYScene::touchMoved(CCSet * touches)
{
    if (m_bNetLoading)
    {
        return;
    }
    CCTouch * touch = (CCTouch *)touches->anyObject();
    switch (Controller::getInstance()->getGameStatus())
    {
        case kStatusUI:
            m_pPopUpUI->touchMoved(touch);
            break;
        case kStatusScene:
            m_pControlUILayer->touchMoved(touch);
            break;
        default:
            break;
    }
}

void YYScene::touchEnded(CCSet * touches)
{
    CCTouch * touch = (CCTouch *)touches->anyObject();
    switch (Controller::getInstance()->getGameStatus())
    {
        case kStatusDead:
            m_CurTouchPoint = touch->getLocation();
            m_bTouchEnded = true;
            break;
        case kStatusUI:
            break;
        case kStatusScene:
            m_pControlUILayer->touchEnded(touch);
            break;
        default:
            break;
    }
}

#pragma mark 循环 

void YYScene::update(float f)
{
    if (m_bNetLoading)
    {
        ((YYSprite *)getChildByTag(TAG_NET_CONNECTING_MASKING)->getChildByTag(1))->updateAnimation();
    }
    switch (Controller::getInstance()->getGameStatus())
    {
        case kStatusLogo:
            dynamic_cast<Logo *>(m_pTempLayer)->update();
            break;
        case kStatusCover:
            dynamic_cast<Cover*>(m_pTempLayer)->update();
            break;
        case kStatusLogin:
            dynamic_cast<LoginUI*>(m_pTempLayer)->update(f);
            break;
        case kStatusLoading:
            m_pLoading->update();
            break;
        case kStatusUI:
        case kStatusScene:
            if (Controller::getInstance()->getGameStatus() == kStatusUI)
            {
                m_pPopUpUI->update();
            }
            m_pControlUILayer->update(f);
            m_pSpriteLayer->update();
            //
            updateCameraPosition();
            if(m_nFightResultDelayCounter >= 0 && m_nFightResultDelayCounter < 40)
            {
                m_nFightResultDelayCounter++;
            }
            else if(m_nFightResultDelayCounter == 40)
            {
                m_pPopUpUI->stageRating();
                m_pPopUpUI->sendFightResultConfirmMessage();
                //Controller::getInstance()->switchGameStatus(kStatusLoading); //fch 暂时屏蔽
                m_pPopUpUI->initFightEvaluationUI();
                m_nFightResultDelayCounter = -1;
            }
            break;
        case kStatusDead:
            m_pControlUILayer->update(f);
            m_pSpriteLayer->update();
            break;
        default:
            break;
    }
    
}

#pragma mark 更新屏幕镜头位置 

void YYScene::updateCameraPosition()
{
    // 脚本控制镜头移动 
    if (m_bCameraMovedByScript)
    {
        float offsetX = 0,offsetY = 0;
        if (m_ScreenDatumPoint.x <= m_CameraDestination.x - m_CameraOffsetPerFrame.width)
        {
            offsetX = m_CameraOffsetPerFrame.width;
        }
        else if (m_ScreenDatumPoint.x >= m_CameraDestination.x + m_CameraOffsetPerFrame.width)
        {
            offsetX = -m_CameraOffsetPerFrame.width;
        }
        else if (m_ScreenDatumPoint.x != m_CameraDestination.x)
        {
            offsetX = m_CameraDestination.x - m_ScreenDatumPoint.x;
        }
        if (m_ScreenDatumPoint.y <= m_CameraDestination.y - m_CameraOffsetPerFrame.height)
        {
            offsetY = m_CameraOffsetPerFrame.height;
        }
        else if (m_ScreenDatumPoint.y >= m_CameraDestination.y + m_CameraOffsetPerFrame.height)
        {
            offsetY = -m_CameraOffsetPerFrame.height;
        }
        else if(m_ScreenDatumPoint.y != m_CameraDestination.y)
        {
            offsetY = m_CameraDestination.y - m_ScreenDatumPoint.y;
        }
        if (offsetX != 0 || offsetY != 0)
        {
            m_ScreenDatumPoint = m_ScreenDatumPoint + ccp(offsetX,offsetY);
        }
        else
        {
            m_bCameraMovedByScript = false;
        }
    }

    // 屏幕未锁定，按主角位置更新屏幕基准点位置 
    if (!m_bCameraLocked || m_bCameraFollowSprite)
    {
        if (!m_bCameraLocked)
        {
            m_CameraFocusPosition = m_pSpriteLayer->getRoleByControlled()->getPosition();//玩家位置 
        }
        CCSize screenSize = CCDirector::sharedDirector()->getWinSize();
        float screenX = m_CameraFocusPosition.x - screenSize.width / 2;
        if (screenX < 0)
        {
            screenX = 0;
        }
        else if (getSceneSize().width > screenSize.width && screenX > getSceneSize().width - screenSize.width)
        {
            screenX = getSceneSize().width - screenSize.width;
        }

        if (m_bVibrate)
        {
            m_nVibrateCounter % 4 < 2 ? m_nVibrateOffsetX += 4 : m_nVibrateOffsetX -= 4;
            m_nVibrateCounter % 2 < 1 ? m_nVibrateOffsetY -= 4 : m_nVibrateOffsetY += 4;
            if (m_nVibrateCounter < m_nVibrateDuration)
            {
                m_nVibrateCounter ++;
            }
            else
            {
                m_nVibrateCounter = 0;
                m_nVibrateOffsetX = 0;
                m_nVibrateOffsetY = 0;
                m_bVibrate = false;
            }
        }
        setScreenDatumPoint(ccp(screenX + m_nVibrateOffsetX, m_nVibrateOffsetY));
    }
    // 当屏幕相对游戏场景有位移时，设置远景层相对位移为主角移动速度的一半 
    m_pFarScenery->setPosition(ccpNeg(m_ScreenDatumPoint) + ccp(m_ScreenDatumPoint.x / 2, 0));
    m_pMidScenery->setPosition(ccpNeg(m_ScreenDatumPoint));
    m_pNearScenery->setPosition(ccpNeg(m_ScreenDatumPoint));
    m_pRoadLayer->setPosition(ccpNeg(m_ScreenDatumPoint));
    m_pCloseLayer->setPosition(ccpNeg(m_ScreenDatumPoint));
    m_pSpriteLayer->setPosition(ccpNeg(m_ScreenDatumPoint));
}

/**
 * 启动震屏
 */
void YYScene::vibrate(int duration)
{
    m_bVibrate = true;
    m_nVibrateDuration = duration;
    m_nVibrateCounter = 0;
}

/**
 * 提示信息
 */
void YYScene::showTips(std::string tipInfo,TipType tipType)
{
    initPopUpWindow();
    if (tipType ==  kTipExit)
    {
        m_ePreTipType = m_eTipType;
    }
    else
    {
        m_sTipsInfo = tipInfo;
    }
    m_eTipType = tipType;

    TouchGroup *touchGroup = static_cast<TouchGroup *>(getChildByTag(TAG_POPUP_WINDOW));
    Layout * panel = static_cast<Layout *>(touchGroup->getWidgetByName("Panel"));
    if (m_eTipType == kTipNormal || m_eTipType == kTipHaseePayNomal
        || (m_eTipType == kTipDeadInfo && Controller::getInstance()->getSceneType() == WAR_ZONE)
        || (m_eTipType == kTipPlayTableWorldBossKill) || m_eTipType == kTipCoverConnectFailed
        || (m_eTipType == kTipWipeDungeon))
    {
        m_pTipPanel = static_cast<Layout *>(panel->getChildByName("Panel_a"));
        UIPanel *tipPanel = static_cast<Layout *>(panel->getChildByName("Panel_b"));
        tipPanel->setVisible(false);
        tipPanel->setEnabled(false);
    }
    else
    {
        UIPanel *tipPanel = static_cast<Layout *>(panel->getChildByName("Panel_a"));
        tipPanel->setVisible(false);
        tipPanel->setEnabled(false);
        m_pTipPanel = static_cast<Layout *>(panel->getChildByName("Panel_b"));
        m_pTipPanel->setZOrder(0);
        Button *button1 = static_cast<Button *>(m_pTipPanel->getChildByName("Button1"));
        button1->setTouchEnabled(true);
        button1->setTag(kButtonConfirmTips);
        button1->addTouchEventListener(this, toucheventselector(YYScene::confirmTips));
        if(m_eTipType == kTipExit)
        {
            m_pTipPanel->setZOrder(5);
        }
        if(m_eTipType == kTipRemoveProtoss)
        {
            button1->setTitleText(g_sRemove);
        }
        else if(m_eTipType == kTipGoldCharge || m_eTipType == kTipJewCharge)
        {
            button1->setTitleText(g_sCharge);
        }
        else if(m_eTipType == kTipWipeProtoss)
        {
            button1->setTitleText(g_sYes);
        }
        else
        {
            button1->setTitleText(g_sConfirm);
        }
    }
    if (m_eTipType == kTipHaseePayNomal || m_eTipType == kTipGoChargeForHaseePayUI)
    {
        m_pPopUpUI->getCardNumberEB()->setTouchEnabled(false);
        m_pPopUpUI->getCardPasswordEB()->setTouchEnabled(false);
    }

    m_pTipPanel->setEnabled(true);
    m_pTipPanel->setVisible(true);
    m_pTipPanel->setTouchEnabled(true);

    Button *button = static_cast<Button *>(m_pTipPanel->getChildByName("Button0"));
    button->setTouchEnabled(true);
    button->setTag(kButtonCancelTips);
    button->addTouchEventListener(this, toucheventselector(YYScene::cancelTips));
    if (m_eTipType == kTipExit || m_eTipType == kTipRoleDeleted || m_eTipType == kTipRoleDeletedCancel)
    {
        button->setTitleText(g_sCancel);
    }
    else if(m_eTipType == kTipWipeProtoss)
    {
        button->setTitleText(g_sNo);
    }
    ImageView * imageView = static_cast<ImageView *>(m_pTipPanel->getChildByName("ImageView"));
    Label *label_0 = static_cast<Label *>(imageView->getChildByName("Label_0"));
    label_0->setText(tipInfo);
    label_0->setVisible(true);
    if(Controller::getInstance()->getGameStatus() == kStatusLogin)
    {
        dynamic_cast<LoginUI *>(m_pTempLayer)->getLoginTouchGroup()->setTouchEnabled(false);
    }
    else if (Controller::getInstance()->getGameStatus() == kStatusUI)
    {
        m_pPopUpUI->getUITouchGroup()->setTouchEnabled(false);
        if (m_eTipType == kTipWipeDungeon)
        {
            m_pPopUpUI->getUITouchTop()->setTouchEnabled(false);
        }
    }
    
}

/**
 * 确认提示信息
 */
void YYScene::confirmTips(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_ENDED)
    {
        if (m_eTipType == kTipExit)
        {// 退出提示
            CCDirector::sharedDirector()->end();
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniMethodInfo minfo;//定义Jni函数信息结构体
            bool isExist = JniHelper::getStaticMethodInfo(minfo, "com/yourcompany/fairyland/FairyLand", "exitGame", "()V");
            if (isExist)
            {
                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);     //调用函数
                minfo.env->DeleteLocalRef(minfo.classID);
            }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
            return;
        }
        if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->isTouchUnable(pSender, type))
        {
            return;
        }
        m_pTipPanel->setEnabled(false);
        m_pTipPanel->setVisible(false);
        m_pTipPanel->setTouchEnabled(false);
        if(Controller::getInstance()->getGameStatus() == kStatusLogin)
        {
            dynamic_cast<LoginUI *>(m_pTempLayer)->getLoginTouchGroup()->setTouchEnabled(true);
            dynamic_cast<LoginUI *>(m_pTempLayer)->setRegisterEditBoxEnable(true);
            dynamic_cast<LoginUI *>(m_pTempLayer)->setLoginEditBoxEnable(true);
        }
        else if (Controller::getInstance()->getGameStatus() == kStatusUI)
        {
            m_pPopUpUI->getUITouchGroup()->setTouchEnabled(true);
        }
        if(m_eTipType == kTipRoleDeleted)
        {
            const char * cmd = CCString::createWithFormat(ASK_ROLE_DELETE,Controller::getInstance()->getRoleIndex() + 1)->getCString();
            Controller::getInstance()->sendMessage(cmd,true);
        }
        else if(m_eTipType == kTipRoleDeletedCancel)
        {
            const char * cmd = CCString::createWithFormat(ASK_ROLE_DELETE_CANCEL,Controller::getInstance()->getRoleIndex() + 1)->getCString();
            Controller::getInstance()->sendMessage(cmd,true);
        }else if (m_eTipType == kTipDeadInfo)
        {// 确认原地复活
            Controller::getInstance()->sendMessage(ASK_RELIVE,true);
        }
        else if (m_eTipType == kTipRemoveProtoss)
        {
            CCString *strMsg = CCString::createWithFormat("%s_%d", ASK_PROTOSS_REMOVE, m_pPopUpUI->getProtossEquipID());
            Controller::getInstance()->sendMessage(strMsg->getCString(),false);
        }
        else if (m_eTipType == kTipSwallowProtoss)
        {
            m_pPopUpUI->swallowProtoss();
        }
        else if(m_eTipType == kTipMergeProtoss)
        {
            Controller::getInstance()->sendMessage(ASK_ONEKEY_COMBO,true);
        }
        else if (m_eTipType == kTipGoldCharge)
        {
            m_pPopUpUI->goShopUI(kShopGem);
        }
        else if (m_eTipType == kTipGoldChargeForShopUI)
        {
            m_pPopUpUI->reSetShopDataInfo();
            m_pPopUpUI->refreshShopItemList(kShopGem);
        }
        else if(m_eTipType == kTipJewCharge)
        {
            if (m_pPopUpUI->getUIType() == kUIShop)
                m_pPopUpUI->reSetShopDataInfo();
            m_pPopUpUI->goChargeUI(kChargeInit);
        }
        else if (m_eTipType == kTipGoChargeForChargeUI)
        {
            m_pPopUpUI->sendMsgForSerialNumberInfo();
        }
        else if (m_eTipType == kTipGoChargeForHaseePayUI)
        {
            m_pPopUpUI->getCardNumberEB()->setTouchEnabled(true);
            m_pPopUpUI->getCardPasswordEB()->setTouchEnabled(true);
            m_pPopUpUI->sendMsgForSerialNumberInfo();
        }
        else if (m_eTipType == kTipPlayTableDungeonReset)
        {
            CCString *strMsg = CCString::createWithFormat(ASK_DUNGEON_RESET, m_pPopUpUI->getDungeonSelectLevel());
            Controller::getInstance()->sendMessage(strMsg->getCString(),true);
        }
        else if(m_eTipType == kTipWipeProtoss)
        {
            int nDifficut = m_pPopUpUI->getProtectProtossDifficulty();
            CCString* msg = CCString::createWithFormat(ASK_PROTECT_PROTOSS_WIPE,nDifficut);
            Controller::getInstance()->sendMessage(msg->getCString(),true);
            Controller::getInstance()->switchGameStatus(kStatusLoading);
            Controller::getInstance()->setDungeonType(kDungeonProtoss); //设置进入副本类型
        }
        m_eTipType = kTipDefault;
    }
}

/**
 * 隐藏提示信息
 */
void YYScene::cancelTips(CCObject* pSender, cocos2d::gui::TouchEventType type)
{
    if (isNetLoading())
    {
        return;
    }
    if (type == TOUCH_EVENT_BEGAN)
    {
        if (m_eTipType == kTipExit)
        {
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
            m_pTipPanel->setEnabled(false);
            m_pTipPanel->setVisible(false);
            m_pTipPanel->setTouchEnabled(false);
            if (m_ePreTipType != kTipDefault && !m_sTipsInfo.empty())
            {
                showTips(m_sTipsInfo,m_ePreTipType);
            }
            else //之前打开消费信息提示
            {
                TouchGroup *touchGroup = static_cast<TouchGroup *>(getChildByTag(TAG_POPUP_WINDOW));
                Layout * panel = static_cast<Layout *>(touchGroup->getWidgetByName("Panel"));
                Layout * panel_1 = static_cast<Layout *>(panel->getChildByName("Panel_1"));
                if(panel_1 && !panel_1->isVisible())
                {
                    if (Controller::getInstance()->getGameStatus() == kStatusUI)
                    {
                        m_pPopUpUI->getUITouchGroup()->setTouchEnabled(true);
                    }
                }
            }
            m_pTipPanel->setZOrder(0);
            return;
        }
        if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->isTouchUnable(pSender, type))
        {
            return;
        }
        m_pTipPanel->setEnabled(false);
        m_pTipPanel->setVisible(false);
        m_pTipPanel->setTouchEnabled(false);
        if(Controller::getInstance()->getGameStatus() == kStatusLogin)
        {
            dynamic_cast<LoginUI *>(m_pTempLayer)->getLoginTouchGroup()->setTouchEnabled(true);
            dynamic_cast<LoginUI *>(m_pTempLayer)->setRegisterEditBoxEnable(true);
            dynamic_cast<LoginUI *>(m_pTempLayer)->setLoginEditBoxEnable(true);
            dynamic_cast<LoginUI *>(m_pTempLayer)->setInputNameTouchEnabled(true);
        }
        if (Controller::getInstance()->getGameStatus() == kStatusUI)
        {
            m_pPopUpUI->getUITouchGroup()->setTouchEnabled(true);
            if(m_pPopUpUI->getUIType() == kUIConvertCode)  //兑换码 输入框触控
            {
                if (m_pPopUpUI->getConvertCodeEditBox())
                {
                    m_pPopUpUI->getConvertCodeEditBox()->setTouchEnabled(true);
                }
            }
            if (m_eTipType == kTipWipeDungeon)
            {
                m_pPopUpUI->getUITouchGroup()->setTouchEnabled(false);
                m_pPopUpUI->getUITouchTop()->setTouchEnabled(true);
            }
        }
        if (m_eTipType == kTipDeadInfo)
        {
            if (Controller::getInstance()->getSceneType() == WAR_ZONE)
            {
                m_pSpriteLayer->getRoleByControlled()->relive();
                Controller::getInstance()->setGameStatus(kStatusScene);
            }
            else
            {// 取消原地复活
                setFightResultDelayCounter(-1);
                Controller::getInstance()->sendMessage(ASK_RETURN_CITY,true);
                Controller::getInstance()->switchGameStatus(kStatusLoading);
            }
        }
        if (m_eTipType == kTipGoldChargeForShopUI)
        {
            m_pPopUpUI->reSetShopDataInfo();
        }
        else if(m_eTipType == kTipJewCharge)
        {
            if (kUIShop == m_pPopUpUI->getUIType())
            {
                m_pPopUpUI->reSetShopDataInfo();
            }
        }
        else if (m_eTipType == kTipHaseePayNomal || m_eTipType == kTipGoChargeForHaseePayUI)
        {
            m_pPopUpUI->getCardNumberEB()->setTouchEnabled(true);
            m_pPopUpUI->getCardPasswordEB()->setTouchEnabled(true);
        }
        else if(m_eTipType == kTipWipeProtoss)
        {
            int nDifficut = m_pPopUpUI->getProtectProtossDifficulty();
            CCString* msg = CCString::createWithFormat(ASK_PROTECT_PROTOSS_CHALLENGE,nDifficut);
            Controller::getInstance()->sendMessage(msg->getCString(),true);
            Controller::getInstance()->switchGameStatus(kStatusLoading);
            Controller::getInstance()->setDungeonType(kDungeonProtoss); //设置进入副本类型
        }
        else if(m_eTipType == kTipPlayTableWorldBossKill)
        {
            m_pControlUILayer->clearKeyState();//  清除按键状态
            Controller::getInstance()->sendMessage(ASK_WORLD_BOSS_RETURN,true);
        }
        else if(m_eTipType == kTipCoverConnectFailed)
        {
// cover 页面联网失败 退出游戏
            CCDirector::sharedDirector()->end();
            const std::string filePath = getFilePath(CLICKED_EFFECT ,kDirTypeSound ,kFileSuffixWav);
            PLAY_EFFECT(filePath.c_str());
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            JniMethodInfo minfo;//定义Jni函数信息结构体
            bool isExist = JniHelper::getStaticMethodInfo(minfo, "com/yourcompany/fairyland/FairyLand", "exitGame", "()V");
            if (isExist)
            {
                minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);     //调用函数
                minfo.env->DeleteLocalRef(minfo.classID);
            }
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            exit(0);
#endif
        }
        m_eTipType = kTipDefault;
    }
}
#pragma mark 设置单元格是否有障碍 

void YYScene::setBlockTile(cocos2d::CCArray *array)
{
    CC_SAFE_DELETE(m_pBlockTile);
    m_pBlockTile = new bool[getTileColumnAmount() * getTileRowAmount()];
    for (int i = 0; i < getTileColumnAmount() * getTileRowAmount(); i++)
    {
        m_pBlockTile[i] = false;
    }
    for (int i = 0; i < array->count(); i++)
    {
        const char * str = static_cast<CCString *>(array->objectAtIndex(i))->getCString();
        CCArray *array1 = split_string(str, ';');
        int startPos = static_cast<CCString *>(array1->objectAtIndex(0))->intValue() - 10;
        if (array1->count() > 1)
        {
            int endPos = static_cast<CCString *>(array1->objectAtIndex(1))->intValue() - 10;
            for (int j = startPos; j <= endPos; j++)
            {
                m_pBlockTile[j] = true;
            }
        }
        else
        {
            m_pBlockTile[startPos] = true;
        }
    }
}

/**
 * 根据行列数判断单元格是否有障碍
 */
bool YYScene::isBlockTile(uint16_t col,uint16_t row)
{
    return m_pBlockTile[row * getTileColumnAmount() + col];
}

void YYScene::addSprite(cocos2d::CCArray *array)// 添加精灵
{
    if(m_pSpriteLayer)
    {
        m_pSpriteLayer->loadSprites(array);//  登录
    }
}

void YYScene::removeSprite(cocos2d::CCArray *array)//删除精灵
{
    if(m_pSpriteLayer)
    {
        const char * spriteID = ((CCString *)array->objectAtIndex(0))->getCString();
        m_pSpriteLayer->removeSprites(spriteID);
    }
}
void YYScene::processSpriteMove(cocos2d::CCArray *array)//处理精灵移动
{
    if(m_pSpriteLayer)
    {
        m_pSpriteLayer->processSpriteMove(array);
    }
}
void YYScene::setDropOdds(cocos2d::CCArray *array)//设置掉率
{
    if(m_pSpriteLayer)
    {
        m_pSpriteLayer->setDropOdds(array);
    }
}

void YYScene::setRoleControlled(cocos2d::CCArray *array)//设置可控角色
{
    if(m_pSpriteLayer)
    {
        m_pSpriteLayer->setRoleControlled(array);
    }
}

//处理装备合成返回数据
void YYScene::updateEquipComposeUI(cocos2d::CCArray *array)
{
    CCString *str = static_cast<CCString *>(array->objectAtIndex(0));
    if (strcmp(str->getCString(), "1") == 0) // 合成条件满足开启特效
    {
        m_pPopUpUI->processEquipComposeSucceed();
    }
    else
    {
        m_pPopUpUI->updateComposeInterface(array);
    }
}

// 设置角色技能数据
void YYScene::setRoleSkillIndex(cocos2d::CCArray *array)
{
    if(m_pSpriteLayer)
    {
        m_pSpriteLayer->setSkillIndexData(array);
    }
}

void YYScene::receiveNPCTaskList(cocos2d::CCArray *array)
{
    if (m_pSpriteLayer)
    {
        const char *npcID = static_cast<CCString *>(array->objectAtIndex(0))->getCString();
        YYNPC *npc = static_cast<YYNPC *>(m_pSpriteLayer->getSpriteWithServerIDDict()->objectForKey(npcID));
        if (npc)
        {
            npc->getTaskDataArray()->removeAllObjects();
            array->removeObjectAtIndex(0);//remove npcID
            if (array->count() > 0)
            {
                npc->getTaskDataArray()->addObjectsFromArray(array);
            }
            if (m_pPopUpUI)
            {
                m_pPopUpUI->popUpDialogInterface(npc);
            }
        }
    }
}

void YYScene::processRoleRelive(cocos2d::CCArray *array)
{
    int type = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
    if (type == 0)
    {// 无复活道具
        int jewConsume = static_cast<CCString *>(array->objectAtIndex(1))->intValue();
        m_pPopUpUI->setConsumeJewelTips(kJewelConsumeForRelive, jewConsume);
    }
    else if(type == 1)
    {// 有复活道具
        if ( m_pSpriteLayer)
        {
            if ( m_pSpriteLayer->getRoleByControlled())
            {
                m_pSpriteLayer->getRoleByControlled()->relive();
                Controller::getInstance()->switchGameStatus(kStatusScene);
            }
        }
    }
    else if(type == 2)
    {
        showTips(string(g_sIf) + string(g_sReliveConfirm),kTipDeadInfo);
    }
}

void YYScene::receiveDungeonsData(cocos2d::CCArray *array)
{
    if (m_pPopUpUI)
    {
        m_pPopUpUI->initDungeonUI(array);
    }
}

void YYScene::receiveStageData(cocos2d::CCArray *array)
{
    if (array->count() > 0 && m_pPopUpUI)
    {
        m_pPopUpUI->initStageUI(array);
    }
}

void YYScene::receiveStageAwardData(cocos2d::CCArray *array)
{
    if (array->count() > 0 && m_pPopUpUI)
    {
        m_pPopUpUI->initStageAwardUI(array);
    }
}

void YYScene::receiveChatMessage(cocos2d::CCArray *array)
{
   m_pPopUpUI->dispatchChatMessage(array);
}

void YYScene::editItemNumber(cocos2d::CCArray *array)
{
    int type = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
    array->removeObjectAtIndex(0);
    GlobalData::getInstance()->itemConsume(array, type);
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() == kUIBag)
    {
        m_pPopUpUI->updateItemIconInBag(type);
    }
}

void YYScene::receivePropDetail(cocos2d::CCArray *array)// 接收物品信息
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() == kUIBag)
    {
        m_pPopUpUI->updateItemDetailInfo(array);
    }
}

void YYScene::receiveEquipDetail(cocos2d::CCArray *array)//接收装备信息
{
    m_pPopUpUI->updateEquipDetailInfo(array);
}

void YYScene::treasureContainerErrorTips(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() == kUIBag)
    {
        std::string message = static_cast<CCString *>(array->objectAtIndex(0))->getCString();
        m_pPopUpUI->showTreasureErrorTip(message);
    }
}

void YYScene::onCmdPutonEquip(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() == kUIBag)
    {
        int value = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
        if (value == 1)
        {
            m_pPopUpUI->putOnEquip();
        }
    }
}

void YYScene::reinforceFailedTips(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() == kUIBag)
    {
        int value = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
        m_pPopUpUI->showReinforceFailedTips(value);
    }
}

void YYScene::forgeFailedTips(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() == kUIBag)
    {
        int value = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
        m_pPopUpUI->showComposeFailedTips(value);
    }
}

void YYScene::forgeByJewel(cocos2d::CCArray *array)//用宝石锻造
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() == kUIBag)
    {
        int num = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
        m_pPopUpUI->setConsumeJewelTips(kJewelConsumeForCompose, num);
    }
}

/** TA[任务ID]_[任务名称]_[任务描述]_[[任务物品1名称],[任务物品1ICON],[任务物品1数量];[任务物品2名称],[任务物品2ICON],[任务物品2数量]]_[任务金钱]_[任务宝石]_[任务经验]_[任务类型] */
void YYScene::receiveTaskData(cocos2d::CCArray *array)//添加任务
{//  添加任务
    GlobalData::getInstance()->addTaskData(array);
    if(m_pControlUILayer->getTaskNumberLabel())
    { // 更新任务数
        ((CCLabelTTF*)m_pControlUILayer->getTaskNumberLabel())->setString(CCString::createWithFormat("%d",GlobalData::getInstance()->getTaskDataDict()->count())->getCString());
    }
}

void YYScene::equipReinforce(cocos2d::CCArray *array)//装备强化
{
    CCString *str = static_cast<CCString *>(array->objectAtIndex(0));
    if ( strcmp(str->getCString(), "A") == 0)
    {
        m_pPopUpUI->updateReinforceInterface(array);
    }
    else
    {
        int value = str->intValue();
        if(value == 0)
        {
            m_pPopUpUI->setReinforceSuccess(false);
        }
        else if(value == 1 || value == 2)
        {
            m_pPopUpUI->setReinforceSuccess(true);
        }
    }
}

//NL_index;icon;name;tab;num;des;amount;jew,2;money,3;日期type,日期;时间type,时间_....
void YYScene::setShopData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setShopDataInfo(array);
}

void YYScene::receiveAllShopDataFinished(cocos2d::CCArray *array)
{
    m_pPopUpUI->drawShopContent();
}

void YYScene::setChargeData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setChargeDataInfo(array);
}

void YYScene::setChargeSerialNubmer(cocos2d::CCArray *array)
{
    m_pPopUpUI->setChargeSerialNumberInfo(array);
}

// 签到界面的初始化
void YYScene::setSignUpData(cocos2d::CCArray *array)
{
    CCString *pStr = (CCString *)array->objectAtIndex(0);
    if (pStr->isEqual(CCString::create("sign")))
    {
        m_pPopUpUI->setSignUpDateInfo(array);
    }
    else if(pStr->intValue() == 7)
    {
        array->removeObjectAtIndex(0);
        m_pPopUpUI->setLoginAwardData(array);
    }
}
// 签到后
void YYScene::setSignUpFinishData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setSignUpFinishDataInfo(array);
}
// 宝箱物品
void YYScene::setSignUpBoxAwardData(cocos2d::CCArray *array)
{
    CCString *pStr = (CCString *)array->objectAtIndex(0);
    if (pStr->isEqual(CCString::create("sign")))
    {
        m_pPopUpUI->setSignUpBoxAwardDataInfo(array);
    }
    else if(pStr->intValue() == 7)
    {
        array->removeObjectAtIndex(0);
        if (array->count() > 0)
        {
            int index = static_cast<CCString *>(array->objectAtIndex(0))->intValue();
            m_pPopUpUI->updateLoginAwardUI(index);
        }
    }
}
// 活跃-数据
void YYScene::receiveActivityData(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI
        && m_pPopUpUI->getUIType() == kUIActivity)
    {
        m_pPopUpUI->updateActivityUI(array);
    }
}
// 成就 类型
void YYScene::receiveAchievementData(cocos2d::CCArray *array)
{
    m_pPopUpUI->receiveAchievementDataInfo(array);
}
// 成就 完成成就
void YYScene::setAchievementFinishedTipData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setAchievementTipDataInfo(array);
}
// 成就 获得奖励
void YYScene::setAchievementGetAwardData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setAchievementGetAwardDataInfo(array);
}
// 占灵获取数据结束
void YYScene::receiveProtossBaseDataFinished(cocos2d::CCArray *array)
{
    m_pPopUpUI->updateProtossBaseUI();
    m_pPopUpUI->setIsProtossInit(true);
}
// 占灵装备数据结束
void YYScene::receiveProtossEquipDataFinished(cocos2d::CCArray *array)
{
    if (m_pPopUpUI->getIsEnterProtossEquipUI())
    {
        m_pPopUpUI->updateProtossEquipUI();
        m_pPopUpUI->updateProtossEquipBagUI();
        m_pPopUpUI->setIsProtossEquipInit(true);
    }
}
// 占灵 等待收取列表
void YYScene::setProtossWaitRecData(cocos2d::CCArray *array)
{
    CCString *pStr = (CCString *)array->objectAtIndex(0);
    if (pStr->compare("F") == 0)
        receiveProtossBaseDataFinished(array);
    else if (pStr->compare("F1") != 0)
        m_pPopUpUI->setProtossWaitRecvData(array);
}
void YYScene::setProtossWaitRecOneData(cocos2d::CCArray *array)
{

    m_pPopUpUI->setProtossWaitRecvOneData(array);

}
// 占灵 星灵背包列表 PC_星灵所在位置;[星灵所有数据];星灵当前经验
void YYScene::setProtossBagData(cocos2d::CCArray *array)
{
    CCString *pStr = (CCString *)array->objectAtIndex(0);
    if (pStr->compare("F") == 0)
        receiveProtossEquipDataFinished(array);
    else if (pStr->compare("F1") != 0)
        m_pPopUpUI->setProtossBagData(array);
}
void YYScene::setProtossBagOneData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setProtossBagOneData(array);
}
// 占灵 发送已装备星灵列表 PE_星灵所在位置;[星灵所有数据];星灵当前经
void YYScene::setProtossEquipBagData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setProtossEquipData(array);
}
// 占灵 发送点亮按键位置 PG_3
void YYScene::setLightConstellationData(cocos2d::CCArray *array)
{
    if(array)
    {
        CCString *tpString = (CCString *)array->objectAtIndex(0);
        if(tpString != NULL)
        {
            m_pPopUpUI->setCurDevineIndex(tpString->intValue());
            if (m_pPopUpUI->getIsProtossInit())
            {
                m_pPopUpUI->updateProtossBaseUI(tpString->intValue());
                m_pPopUpUI->initProtossAction();
            }
        }
    }
}
// 占灵 星灵碎片数量 PH
void YYScene::setProtossPieceData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setProtossPieceData(array);
}
// 邮件
void YYScene::setMailListData(cocos2d::CCArray *array)
{
   if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIMail)
   {
       m_pPopUpUI->setMailListData(array);
   }
}
void YYScene::setMailDetailData(cocos2d::CCArray *array)
{
    m_pPopUpUI->setMailDetailData(array);
}
void YYScene::setMailNum(cocos2d::CCArray *array)
{
    m_pPopUpUI->setMailNum(((CCString *)array->objectAtIndex(0))->intValue());
}
void YYScene::getRankDataInfo(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIRankWar)
    {
        if((!array) || (array->count() != 100))
            return;
        CCDictionary* m_pDictRankData = m_pPopUpUI->getDictRankData();
        if (!m_pDictRankData)
            m_pDictRankData = new CCDictionary();
        m_pDictRankData->removeAllObjects();
        
        int nCount = array->count();
        
        int j = 0;
        CCArray* arrSum = CCArray::createWithCapacity(6);
        for (int i = 0; i < nCount; i++)        //每页(包含6条信息)作为dict一个对象存储
        {
            if (i != 0 && i%6 == 0)
            {
                m_pDictRankData->setObject(arrSum,j);
                arrSum = CCArray::createWithCapacity(6);
                j++;
            }
            CCString* pStrSeparate = (CCString *)array->objectAtIndex(i);
            CCArray* pArrayRankWarData = split_string(pStrSeparate->getCString(), ';');
            arrSum->addObject(pArrayRankWarData);
            
            if(i == nCount -1)
            {
                m_pDictRankData->setObject(arrSum,j);
            }
        }
        m_pPopUpUI->updateRankUI(m_pDictRankData,0);  //默认显示第一页
    }
}

//活动

void YYScene::receivePlayTableAH(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updatePlayTableAH(arr);
    }
}

void YYScene::receivePlayTableAI(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updatePlayTableAI(arr);
    }
}


void YYScene::receivePlayTableTT(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updatePlayTableTT(arr);
    }
}

void YYScene::receivePlayTableTR(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updatePlayTableTR(arr);
    }
}

void YYScene::receivePlayTableTD(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updatePlayTableTD(arr);
    }
}

void YYScene::receivePlayTableDR(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updatePlayTableDR(arr);
    }
}

void YYScene::receiveDungeonDO(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updateDungeonDO(arr);
    }
}

void YYScene::receiveDungeonDA(cocos2d::CCArray *arr)
{
    m_pPopUpUI->updateDungeonDA(arr);
}

void YYScene::receiveProtossTO(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updateProtectProtossTO(arr);
    }
}

void YYScene::receiveProtossTW(cocos2d::CCArray *arr)
{
    m_pPopUpUI->updateProtectProtossTW(arr);
}

void YYScene::receiveBossDungeonBL(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updateplayTableBossDungeonBL(arr);
    }
}

void YYScene::receiveBossDungeonBC(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updateplayTableBossDungeonBC(arr);
    }
}

void YYScene::receiveBossDungeonBA(cocos2d::CCArray *arr)
{
    m_pPopUpUI->updateplayTableBossDungeonBA(arr);
}

void YYScene::receiveWorldBossWC(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updateplayTableWorldBossWC(arr);
    }
}

void YYScene::receiveWorldBossWT(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updateplayTableWorldBossWT(arr);
    }
}

void YYScene::receiveWorldBossWB(cocos2d::CCArray *arr)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIPlayTable)
    {
        m_pPopUpUI->updateplayTableWorldBossWB(arr);
    }
}

void YYScene::receiveWorldBossWK(cocos2d::CCArray *arr)
{
    m_pPopUpUI->updateplayTableWorldBossWK(arr);
}

void YYScene::receiveWorldBossWD(cocos2d::CCArray *arr)
{
    m_pPopUpUI->updateplayTableWorldBossWD(arr);
}


//试炼
void YYScene::getTrainDataInfo(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUITrain)
    {
        m_pPopUpUI->updateTrainUI(array);
    }
}
//公告
void YYScene::receiveNoticeInfo(cocos2d::CCArray *array)
{
   m_pPopUpUI->initNoticeUI(array,true);
}

//用户中心
void YYScene::receiveUserCenterInfo(cocos2d::CCArray *array)
{
    if (Controller::getInstance()->getGameStatus() == kStatusUI && m_pPopUpUI->getUIType() ==kUIUserCenter)
    {
        m_pPopUpUI->updateUserCenterUI(array);
    }
}

//金币贝赛尔运行结束回调函数
void YYScene::funDestroy(CCNode* node)
{
	CCSprite* sprite = static_cast<CCSprite*>(node);
	sprite->removeFromParentAndCleanup(true);
}
