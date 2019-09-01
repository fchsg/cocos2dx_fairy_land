//
//  YYNPC.cpp
//  Fairland
//
//  Created by fuchenhao on 13-10-28. 
//
//

#include "YYNPC.h"
#include "YYScene.h"
#include "YYController.h"

USING_NS_CC;

YYNPC::YYNPC()
:m_nStandByDuration(200)// 待机动作出现周期 
,m_nStandByCounter(0)
,m_bStandBy(true)
{
    m_pTaskDataDict = new cocos2d::CCDictionary();
    m_pTaskDataArray = new CCArray(5);
    m_pScene = Controller::getInstance()->getScene();
}

YYNPC::~YYNPC()
{
    CC_SAFE_DELETE(m_pTaskDataDict);
    CC_SAFE_DELETE(m_pTaskDataArray);
}

void YYNPC::update(void)
{
    YYSprite::update();
    if (!m_bLocked && m_bStandBy)
    {
        if (m_nStandByCounter < m_nStandByDuration)
        {
            m_nStandByCounter ++;
        }
        else
        {
            m_bStandBy = false;
            setAction(ACTION_STAND1,false);
        }
    }
    if (m_bMoving)
    {
        move();
    }
    if (!m_bPause && m_bActionOver && !m_bRepeatAction)
    {
        switch (m_u8ActionIndex)
        {
            case ACTION_STAND0:
            case ACTION_STAND2:
            case ACTION_RUN:
            case ACTION_RUN1:
                m_u16CurrentFrameIndex = getCurrentActionData()->getStartFrameIndex();
                setFrame(m_u16CurrentFrameIndex);
                m_bActionOver = false;
                break;
            case ACTION_STAND1:
                setAction(ACTION_STAND0, false);
                m_bStandBy = true;
                m_nStandByCounter = 0;
                break;
            default:
                break;
        }
    }
}

void YYNPC::move(void)
{
    float offsetX = 0;
    float offsetY = 0;
    if(m_u8MoveType == MOVE_BY_SCRIPT)
    {
        float k = atan2f(getDestination().y - getPositionY(),getDestination().x - getPositionX());
        offsetX = m_nScriptSpeed * cos(k);
        offsetY = m_nScriptSpeed * sin(k);
        if (fabsf(offsetX) >= fabsf(getDestination().x - getPositionX()))
        {// 目的地与当前位置横坐标间距小于1帧的偏移量 
            offsetX = getDestination().x - getPositionX();
        }
        if (fabsf(offsetY) >= fabsf(getDestination().y - getPositionY()))
        {// 目的地与当前位置纵坐标间距小于1帧的偏移量 
            offsetY = getDestination().y - getPositionY();
        }
    }
    
    if (offsetX > 0)
    {
        setFaceToRight(true);
    }
    else if(offsetX < 0)
    {
        setFaceToRight(false);
    }
    CCPoint tempPosition = getPosition() + ccp(offsetX,offsetY);// 移动后的位置 
    if (tempPosition.x < 0 || tempPosition.x > m_pScene->getSceneSize().width)
    {
        m_bMoving = false;
        return;
    }
    if (getActionIndex() != ACTION_RUN)
    {
        setAction(ACTION_RUN,false);
    }
    // 地格碰撞
    int tempTileColCount,tempTileRowCount;// 移动后所在元素格列数/行数 
    tempTileColCount =  tempPosition.x / m_pScene->getTileWidth();
    tempTileRowCount = (Controller::getInstance()->getSceneCellTopY() - tempPosition.y) / m_pScene->getTileHeight();
    if (m_pScene->isBlockTile(tempTileColCount, tempTileRowCount))
    {// 移动后所在单元格有障碍 
        if (!m_pScene->isBlockTile(tempTileColCount, m_CurrentTile.y))
        {// 若只横向位移无阻碍 
            setPosition(ccpAdd(ccp(offsetX,0), getPosition()));// 横向位移
            getNameLabel()->setPosition(ccp(offsetX,0) + getNameLabel()->getPosition());
            m_CurrentTile.x = tempTileColCount;
        }
        else if(!m_pScene->isBlockTile(m_CurrentTile.x,tempTileRowCount))
        {// 若只纵向位移无阻碍 
            setPosition(ccpAdd(ccp(0,offsetY), getPosition()));// 纵向位移
            getNameLabel()->setPosition(ccp(0,offsetY) + getNameLabel()->getPosition());
            m_CurrentTile.y = tempTileRowCount;
        }
        else
        {//遇障碍无法位移 
            m_bMoving = false;
            return;
        }
    }
    else
    {// 无障碍 
        setPosition(tempPosition);
        getNameLabel()->setPosition(ccp(offsetX,offsetY) + getNameLabel()->getPosition());
        setCurrentTile(ccp(tempTileColCount,tempTileRowCount));
    }
    if (getDestination().x == getPositionX() && getDestination().y == getPositionY())
    {// 到达目的地 
        m_bMoving = false;
        setAction(ACTION_STAND0, false);
    }
}


void YYNPC::updateTaskStatus()
{
    getYYSpriteChildren()->removeAllObjects();
    removeChildByTag(10);
    MarkType markType = kMarkTypeNull;
    CCArray * arr = m_pTaskDataDict->allKeys();
    if(arr)
    {
        for (int i = 0 ; i < arr->count(); ++i)
        {
            int key = static_cast<CCInteger *>(arr->objectAtIndex(i))->getValue();
            TaskData * task = (TaskData *)m_pTaskDataDict->objectForKey(key);
            if(task->getStatus() == kTaskStatusOver)
            {
                markType = kMarkTypeQuestion;
                break;
            }
            else if(task->getStatus() == kTaskStatusAccept)
            {
                markType = kMarkTypeExcalmatory;
            }
        }
        if (markType == kMarkTypeQuestion)
        {// ?
            YYSprite *sprite = YYSprite::createWithActionSet("se26", "se26", kDirTypeEffect);
            sprite->setAction(0, true);
            addChild(sprite);
            sprite->setPosition(ccp(0,getDefaultSize().height + 30));
            sprite->setTag(10);
            getYYSpriteChildren()->addObject(sprite);
        }
        else if (markType == kMarkTypeExcalmatory)
        {// !
            YYSprite *sprite = YYSprite::createWithActionSet("se25", "se25", kDirTypeEffect);
            sprite->setAction(0, true);
            addChild(sprite);
            sprite->setPosition(ccp(0,getDefaultSize().height + 30));
            sprite->setTag(10);
            getYYSpriteChildren()->addObject(sprite);
        }
    }
}
