//
//  YYSprite.cpp
//  Datang
//
//  Created by fuchenhao on 13-2-5. 
//
//

#include "../common/Common.h"
#include "YYFrameData.h"
#include "YYRect.h"
#include "YYSprite.h"

USING_NS_CC;

YYSprite::YYSprite(void)
:m_u16SpriteType(-1)
,m_u8ActionStatus(0)
,m_u8FrameDuration(-1)
,m_n8FrameIntervalCount(0)
,m_u16CurrentFrameIndex(0)// 在帧列表中的序号 
,m_nScriptActionCounter(0)
,m_nScriptActionDuration(0)
,m_nActionFrameIndex(0)
,m_nDuration(0)
,m_n8DeleteCondition(0)
,m_bMoving(false)
,m_u8MoveType(0)
,m_bDeleted(false)
,m_bFaceToRight(false)// 精灵朝向 
,m_bClipMirror(false)// 根据精灵朝向设置 
,m_bSpriteMirror(false)// 是否镜像（倒退时设置为true） 
,m_bRepeatAction(false)
,m_bActionOver(false)
,m_bSetDefaultSize(false)
,m_bKeepCurrentFrame(false)
,m_bPause(false)
,m_bLocked(false)
,m_pActionSet(NULL)
,m_pCurrentActionData(NULL)
,m_pTalkLabel(NULL)
,m_pSpriteData(NULL)
,m_pSpriteID(NULL)
,m_pSpriteName(NULL)
,m_pNameLabel(NULL)
,m_pVisibleConditionArray(NULL)
{
    m_pBatchNodeDictionary = new CCDictionary();
    m_pPngFileNameDictionary = new CCDictionary();
    m_pYYSpriteChildren = new CCArray(5);
}

YYSprite::~YYSprite()
{
    CC_SAFE_DELETE(m_pSpriteID);
    CC_SAFE_DELETE(m_pSpriteName);
    CC_SAFE_DELETE(m_pPngFileNameDictionary);
    CC_SAFE_DELETE(m_pBatchNodeDictionary);
    CC_SAFE_RELEASE(m_pActionSet);
    CC_SAFE_DELETE(m_pYYSpriteChildren);
}

YYSprite * YYSprite::createWithActionSet(const std::string& aniFile,const std::string& pngFileName,DirectoryType directoryType){
    YYSprite * sprite = create();
    if (sprite != NULL)
    {
        sprite->initWithActionSet(aniFile,pngFileName,directoryType);
    }
    return sprite;
    
}

void YYSprite::initWithActionSet(const std::string& aniFile,const std::string& pngFileName,DirectoryType directoryType)
{
    CCArray* array = split_string(pngFileName.c_str(),',');
    array->insertObject(CCString::create(aniFile), 0);
    initWithActionSet(array, directoryType);
}

void YYSprite::initWithActionSet(const char * resourceName,DirectoryType directoryType)
{
    CCArray* array = split_string(resourceName,',');
    initWithActionSet(array, directoryType);
}

void YYSprite::initWithActionSet(CCArray * array,DirectoryType directoryType,bool isSetLayerIndex)
{
    const char * aniFileName = ((CCString *)array->objectAtIndex(0))->getCString();
    const std::string aniPath = getFilePath(aniFileName,directoryType,kFileSuffixAnu);
    setActionSet(GlobalData::getInstance()->getActionSet(aniPath));
    for (int i = 0; i < m_pActionSet->getImageAmount(); i++)
    {
        CCString * str = (CCString*)array->objectAtIndex(i + 1);
        CCString * nullStr = CCString::create("");
        if (str->isEqual(nullStr))
        {// 处理空图片
//            m_pPngFileNameDictionary->setObject(nullStr,i);
            continue;
        }
        else
        {
            std::string strPath = getFilePath(str->getCString(), directoryType, kFileSuffixPng);
            int layerIndex = 0;
            if (isSetLayerIndex)
            {
                CCArray * array1 = split_string(str->getCString(), '-');// 解析图片文件名获取层级关系 
                layerIndex = ((CCString *)array1->objectAtIndex(1))->intValue();
            }
            int capacity = ((CCInteger *)m_pActionSet->getImageRectCountArray()->objectAtIndex(i))->getValue();
            CCSpriteBatchNode * batchNode = CCSpriteBatchNode::create(strPath.c_str(),capacity << 1);
            m_pBatchNodeDictionary->setObject(batchNode,i);
            m_pPngFileNameDictionary->setObject(CCString::create(strPath),i);
            addChild(batchNode,layerIndex);
        }
    }
}

/**
 * 复制精灵
 */
YYSprite * YYSprite::copy(YYSprite * sprite)
{
    YYSprite * sprite1 = YYSprite::create();
    sprite1->setActionSet(sprite->getActionSet());
    for (int i = 0; i < sprite1->m_pActionSet->getImageAmount(); i++)
    {
        CCString * strPath = (CCString *)sprite->m_pPngFileNameDictionary->objectForKey(i);
        if (strPath)
        {
            sprite1->m_pPngFileNameDictionary->setObject(strPath, i);
            CCSpriteBatchNode * batchNode = (CCSpriteBatchNode *)sprite->m_pBatchNodeDictionary->objectForKey(i);
            int capacity = ((CCInteger *)sprite1->m_pActionSet->getImageRectCountArray()->objectAtIndex(i))->getValue();
            CCSpriteBatchNode * batchNode1 = CCSpriteBatchNode::createWithTexture(batchNode->getTexture(),capacity << 1);
            sprite1->m_pBatchNodeDictionary->setObject(batchNode1, i);
            int layerIndex = batchNode->getZOrder();
            sprite1->addChild(batchNode1,layerIndex);
        }
    }
    return sprite1;
}

/**
 *设置模块
 */
void YYSprite::setMoudle(int imgID ,CCRect rect, CCPoint position, unsigned char transform)
{
    CCString* pngFileName = (CCString*)m_pPngFileNameDictionary->objectForKey(imgID);
    if (!pngFileName) {// 处理空图片 
        return;
    }
    CCSprite *sprite = CCSprite::create(pngFileName->getCString(), rect);
    sprite->setPosition(position);
    sprite->setAnchorPoint(ccp(0, 1));
    if(transform == 1)
    {
        if (!m_bClipMirror)
        {
            sprite->setFlipX(true);
        }
    }
    else
    {
        if (m_bClipMirror)
        {
            sprite->setFlipX(true);
        }
        if (transform == 2)
        {
            sprite->setFlipY(true);
        }
    }
    CCSpriteBatchNode * batchNode = (CCSpriteBatchNode *)(m_pBatchNodeDictionary->objectForKey(imgID));
    batchNode->addChild(sprite);
}

void YYSprite::setMoudle(int imgID ,CCRect rect, CCPoint position, unsigned char transform,CCRect clipRect)
{
    CCString* pngFileName = (CCString*)m_pPngFileNameDictionary-> objectForKey(imgID);
    if (!pngFileName)
    {// 处理空图片 
        return;
    }
    float offsetMinX = 0;
    if (position.x - rect.size.width / 2.0f < clipRect.getMinX())
    {
        offsetMinX = clipRect.getMinX() - position.x + rect.size.width / 2.0f;
    }
    float offsetMinY = 0;
    if (position.y - rect.size.height / 2.0f < clipRect.getMinY())
    {
        offsetMinY = clipRect.getMinY() - position.y + rect.size.height / 2.0f;
    }
    float offsetMaxX = 0;
    if (position.x + rect.size.width / 2.0f > clipRect.getMaxX())
    {
        offsetMaxX = position.x + rect.size.width / 2.0f - clipRect.getMaxX();
    }
    float offsetMaxY = 0;
    if (position.y + rect.size.height / 2.0f > clipRect.getMaxY())
    {
        offsetMaxY = position.y + rect.size.height / 2.0f - clipRect.getMaxY();
    }
    float rectX = rect.getMinX() + offsetMinX;
    float rectY = rect.getMinY() + offsetMaxY;
    float width = rect.size.width - offsetMinX - offsetMaxX;
    float height = rect.size.height - offsetMinY - offsetMaxY;
    int x = position.x - rect.size.width / 2.0f + offsetMinX + width / 2.0f;
    int y = position.y - rect.size.height / 2.0f + offsetMinY + height / 2.0f;
    CCRect newRect = CCRectMake(rectX,rectY,width,height);
    CCSprite *sprite = CCSprite::create(pngFileName->getCString(), newRect);
    sprite->setPosition(ccp(x,y));
    if(transform == 1)
    {
        if (!m_bClipMirror)
        {
            sprite->setFlipX(true);
        }
    }
    else
    {
        if (m_bClipMirror)
        {
            sprite->setFlipX(true);
        }
        if (transform == 2)
        {
            sprite->setFlipY(true);
        }
    }
    
    CCSpriteBatchNode * batchNode = (CCSpriteBatchNode *)(m_pBatchNodeDictionary->objectForKey(imgID));
    batchNode->addChild(sprite);
}

/**
 * 清除上一帧切片
 */
void YYSprite::cleanupClipRect()
{
    CCArray * keyArray = m_pBatchNodeDictionary->allKeys();
    for (int i = 0; i < keyArray->count(); i++)
    {
        int key = ((CCInteger *)keyArray->objectAtIndex(i))->getValue();
        CCSpriteBatchNode * batchNode = dynamic_cast<CCSpriteBatchNode*>(m_pBatchNodeDictionary->objectForKey(key));
        batchNode->removeAllChildren();
    }
}

/**
 * 通过精灵帧数据设置精灵帧
 */
void YYSprite::setFrame(FrameData * frameData)
{
    cleanupClipRect();
    if(frameData->getRectAmount() <= 0)
    {
        return;
    }

    int up = 0,down = 0,left = 0,right = 0;
    for (int i = 0; i < frameData->getRectAmount();i++)
    {
        int rectID = ((CCInteger *)frameData->getRectIndexArray()->objectAtIndex(i))->getValue();
        YYRect *rect = (YYRect*)m_pActionSet->getImageRectDataArray()->objectAtIndex(rectID);
        float offsetX = ((CCInteger *)frameData->getRectXArray()->objectAtIndex(i))->getValue();
        float offsetY = ((CCInteger *)frameData->getRectYArray()->objectAtIndex(i))->getValue();
        uint8_t transformType = ((CCInteger*)frameData->getTransformTypeArray()->objectAtIndex(i))->getValue();
        CCRect rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
        float x = offsetX;
        if (m_bClipMirror)
        {
            x = -offsetX - rect->getWidth();
        }
        float y = -offsetY;
        if (offsetX < left)
        {
            left = offsetX;
        }
        if (offsetY < up)
        {
            up = offsetY;
        }
        if(offsetX + rect->getWidth() > right)
        {
            right = (offsetX + rect->getWidth());
        }
        if(offsetY + rect->getHeight() > down)
        {
            down = (offsetY + rect->getHeight());
        }
        uint8_t transform = transformType;
        if (transform != 0)
        {
            transform = (transformType & 0x07) >> 1;
        }
        setMoudle(rect->getImageIndex(), rect1, ccp(x,y), transform);
    }
    setSize(CCSizeMake(right - left, down - up));
    if (!m_bSetDefaultSize)
    {
        m_bSetDefaultSize = true;
        setDefaultSize(getSize());
    }
}

/**
 * 通过精灵帧数据设置精灵帧
 */
void YYSprite::setFrame(FrameData * frameData,CCRect clipRect)
{
    if(frameData->getRectAmount() <= 0)
    {
        return;
    }
    cleanupClipRect();
    int up = 0,down = 0,left = 0,right = 0;
    for (int i = 0; i < frameData->getRectAmount();i++)
    {
        int rectID = ((CCInteger *)frameData->getRectIndexArray()->objectAtIndex(i))->getValue();
        YYRect *rect = (YYRect*)m_pActionSet->getImageRectDataArray()->objectAtIndex(rectID);
        float offsetX = ((CCInteger *)frameData->getRectXArray()->objectAtIndex(i))->getValue();
        float offsetY = ((CCInteger *)frameData->getRectYArray()->objectAtIndex(i))->getValue();
        uint8_t transformType = ((CCInteger*)frameData->getTransformTypeArray()->objectAtIndex(i))->getValue();
        CCRect rect1 = CCRectMake(rect->getX(), rect->getY(), rect->getWidth(), rect->getHeight());
        CCPoint point = CCPointMake((offsetX + rect->getWidth() / 2.0f), -(offsetY + rect->getHeight() / 2.0f));
        if (m_bClipMirror)
        {
            point = CCPointMake((- offsetX - rect->getWidth() / 2.0f), -(offsetY + rect->getHeight() / 2.0f));
        }
        if (offsetX < left)
        {
            left = offsetX;
        }
        if (-offsetY > up)
        {
            up = -offsetY;
        }
        if(point.x+rect->getWidth() / 2.0f > right)
        {
            right = point.x+rect->getWidth() / 2.0f;
        }
        if(point.y - rect->getHeight() / 2.0f < down)
        {
            down = point.y - rect->getHeight();
        }
        uint8_t transform = transformType;
        if (transform != 0)
        {
            transform = (transformType & 0x07) >> 1;
        }
        CCRect clipInFrame = CCRectMake(point.x - rect->getWidth() / 2.0f, point.y - rect->getHeight() / 2.0f, rect->getWidth(), rect->getHeight());
        if (clipRect.intersectsRect(clipInFrame))
        {
            setMoudle(rect->getImageIndex() ,rect1, point ,transform,clipRect);
        }
    }
    int width = right - left;
    int height = up - down;
    if (clipRect.size.width < width)
    {
        width = clipRect.size.width;
    }
    if (clipRect.size.height < height)
    {
        height = clipRect.size.height;
    }
    setSize(CCSizeMake(width, height));
    if (!m_bSetDefaultSize)
    {
        m_bSetDefaultSize = true;
        setDefaultSize(getSize());
    }
}
/**
 * 通过精灵帧ID设置精灵帧
 */
void YYSprite::setFrame(uint16_t frameIndex)
{
    m_u16CurrentFrameIndex = frameIndex;
    m_nActionFrameIndex = 0;
    m_u8FrameDuration = ((CCInteger*)m_pActionSet->getDelayArray()->objectAtIndex(frameIndex))->getValue();
    uint16_t index = ((CCInteger*)m_pActionSet->getFrameIDArray()->objectAtIndex(frameIndex))->getValue();
    FrameData *fd = (FrameData*)m_pActionSet->getFrameDataArray()-> objectAtIndex(index);
    setFrame(fd);
}

/**
 * 通过精灵帧ID设置精灵帧
 */
void YYSprite::setFrame(uint16_t frameIndex,CCRect rect)
{
    m_u16CurrentFrameIndex = frameIndex;
    m_nActionFrameIndex = 0;
    m_u8FrameDuration = ((CCInteger*)m_pActionSet->getDelayArray()->objectAtIndex(frameIndex))->getValue();
    uint16_t index = ((CCInteger*)m_pActionSet->getFrameIDArray()->objectAtIndex(frameIndex))->getValue();
    FrameData *fd = (FrameData*)m_pActionSet->getFrameDataArray()-> objectAtIndex(index);
    setFrame(fd,rect);
}

/**
 * 通过精灵动画ID设置精灵动画
 */
void YYSprite::setAction(uint16_t animationIndex ,bool isRepeat)
{
    m_u8ActionIndex = animationIndex;
    m_bRepeatAction = isRepeat;
    m_bActionOver = false;
    m_pCurrentActionData = (ActionData *)m_pActionSet->getActionDataArray()->objectAtIndex(m_u8ActionIndex);
    m_n8FrameIntervalCount = -1;
    setFrame(m_pCurrentActionData->getStartFrameIndex());
}

/**
 * 通过精灵动作序号，持续帧数 设置精灵动画
 */
void YYSprite::setActionByScript(uint16_t animationIndex ,int duration, bool isKeepLastFrame)
{
    m_bPause = false;
    m_bActionOver = false;
    m_u8ActionIndex = animationIndex;
    m_nScriptActionDuration = duration;
    m_nScriptActionCounter = 0;
    m_bKeepCurrentFrame = isKeepLastFrame;
    m_pCurrentActionData = (ActionData *)m_pActionSet->getActionDataArray()->objectAtIndex(m_u8ActionIndex);
    uint16_t currentFrameIndex = m_pCurrentActionData->getStartFrameIndex();
    m_n8FrameIntervalCount = -1;
    setFrame(currentFrameIndex);
}

/**
 * 通过精灵动画ID设置精灵动画
 */
void YYSprite::setAction(uint16_t animationIndex ,bool isRepeat,CCRect clipRect)
{
    m_u8ActionIndex = animationIndex;
    m_bRepeatAction = isRepeat;
    m_bActionOver = false;
    m_pCurrentActionData = (ActionData *)m_pActionSet->getActionDataArray()->objectAtIndex(m_u8ActionIndex);
    m_u16CurrentFrameIndex = m_pCurrentActionData->getStartFrameIndex();
    m_n8FrameIntervalCount = -1;
    setFrame(m_u16CurrentFrameIndex,clipRect);
}

void YYSprite::setDuration(int duration)
{
    m_nDuration = duration;
    m_nCounter = 0;
}

void YYSprite::update(void)
{
    updateAnimation();

    // 遍历YYSprite *类型子节点 
    for (int i = 0; i < m_pYYSpriteChildren->count();)
    {
        YYSprite *sprite = (YYSprite *)m_pYYSpriteChildren->objectAtIndex(i);
        if(sprite)
        {
            if(sprite->m_bDeleted)
            {
                removeChild(sprite,true);
                m_pYYSpriteChildren->removeObjectAtIndex(i);
            }
            else
            {
                sprite->update();
                i++;
            }
        }
        else
        {
            i++;
        }
    }
    
    if (m_n8DeleteCondition == DELETE_ACTION_OVER)
    {
        if (isActionOver())
        {
            m_bDeleted = true;
        }
    }
    else if (m_n8DeleteCondition == DELETE_COUNTDOWN_TO_THE_END)
    {
        if (m_nDuration > 0)
        {
            if (m_nCounter < m_nDuration)
            {
                m_nCounter ++;
            }
            else
            {
                m_bDeleted = true;
            }
        }
    }
}

/**
 * 更新动画显示
 */
void YYSprite::updateAnimation()
{
    if (m_bPause)
    {
        return;
    }
    if(m_nScriptActionDuration > 0)
    {// 脚本指令调用的动作 
        if (m_nScriptActionCounter < m_nScriptActionDuration)
        {
            m_nScriptActionCounter ++;
        }
        else
        {
            m_nScriptActionCounter = 0;
            m_nScriptActionDuration = 0;
            if (m_bKeepCurrentFrame)
            {// 动作调用结束后保持播放当前帧 
                m_bPause = true;
            }
            else
            {
                setAction(ACTION_STAND0, false);
            }
        }
    }

    if (m_n8FrameIntervalCount < m_u8FrameDuration - 1)
    {
        m_n8FrameIntervalCount ++;
        m_nActionFrameIndex ++;
    }
    else
    {
        m_n8FrameIntervalCount = 0;
        if(m_u16CurrentFrameIndex < m_pCurrentActionData->getEndFrameIndex())
        {
            m_nActionFrameIndex ++;
            setFrame(++m_u16CurrentFrameIndex);
        }
        else
        {
            m_nActionFrameIndex = 0;
            m_bActionOver = true;
            if (m_bRepeatAction || m_nScriptActionDuration != 0)
            {
                setFrame(m_pCurrentActionData->getStartFrameIndex());
            }
        }
    }
}

void YYSprite::setFaceToRight(bool isFaceToRight)
{
    m_bFaceToRight = isFaceToRight;
    if (m_bSpriteMirror)
    {
        m_bClipMirror = m_bFaceToRight;
    }
    else
    {
        m_bClipMirror = !m_bFaceToRight;
    }
}

bool YYSprite::isFaceToRight()
{
    return m_bFaceToRight;
}

bool YYSprite::isActionOver()
{
    return m_bActionOver;
}

int8_t YYSprite::getActionStatus()
{
    return m_u8ActionStatus;
}

void YYSprite::setActionStatus(uint8_t state)
{
    m_u8ActionStatus = state;
}

bool YYSprite::isIntersectsRect(CCRect rect)
{
    uint16_t index = ((CCInteger*)m_pActionSet->getFrameIDArray()->objectAtIndex(m_u16CurrentFrameIndex))->getValue();
    FrameData * frameData = (FrameData*)m_pActionSet->getFrameDataArray()-> objectAtIndex(index);
    for (int i = 0; i < frameData->getRectAmount(); i++)
    {
        int rectID = ((CCInteger *)frameData->getRectIndexArray()-> objectAtIndex(i))->getValue();
        YYRect * imageRect = (YYRect *)m_pActionSet->getImageRectDataArray()->objectAtIndex(rectID);
        float offsetX = ((CCInteger *)frameData->getRectXArray()->objectAtIndex(i))->getValue();
        float offsetY = ((CCInteger *)frameData->getRectYArray()-> objectAtIndex(i))->getValue();
        float x = getPositionX() + offsetX;
        if (m_bClipMirror)
        {
            x = getPositionX() - offsetX - imageRect->getWidth();
        }
        float y = getPositionY() - offsetY - imageRect->getHeight();

        CCRect rect1 = CCRectMake(x, y, imageRect->getWidth(), imageRect->getHeight());
        if (rect.intersectsRect(rect1))
        {
            return true;
        }
    }
    return false;
}

bool YYSprite::isIntersectsSprite(YYSprite * sprite)
{
    uint16_t index = ((CCInteger*)m_pActionSet->getFrameIDArray()->objectAtIndex(m_u16CurrentFrameIndex))->getValue();
    FrameData * frameData = (FrameData*)m_pActionSet->getFrameDataArray()-> objectAtIndex(index);
    for (int i = 0; i < frameData->getRectAmount();i++)
    {
        int rectID = ((CCInteger *)frameData->getRectIndexArray()-> objectAtIndex(i))->getValue();
        YYRect * imageRect = (YYRect *)m_pActionSet->getImageRectDataArray() ->objectAtIndex(rectID);
        float offsetX = ((CCInteger *)frameData->getRectXArray()->objectAtIndex(i))->getValue();
        float offsetY = ((CCInteger *)frameData->getRectYArray()-> objectAtIndex(i))->getValue();
        float x = getPositionX() + offsetX;
        if (m_bClipMirror)
        {
            x = getPositionX() - offsetX - imageRect->getWidth();
        }
        float y = getPositionY() - offsetY - imageRect->getHeight();
        CCRect rect1 = CCRectMake(x, y, imageRect->getWidth(), imageRect->getHeight());
        if (sprite->isIntersectsRect(rect1))
        {
            return true;
        }
    }
    return false;
}

bool YYSprite::isTouched(cocos2d::CCPoint point)
{
    uint16_t index = ((CCInteger*)m_pActionSet->getFrameIDArray()->objectAtIndex(m_u16CurrentFrameIndex))->getValue();
    FrameData * frameData = (FrameData*)m_pActionSet->getFrameDataArray()-> objectAtIndex(index);
    for (int i = 0; i < frameData->getRectAmount();i++)
    {
        int rectID = ((CCInteger *)frameData->getRectIndexArray()-> objectAtIndex(i))->getValue();
        YYRect * imageRect = (YYRect *)m_pActionSet->getImageRectDataArray() ->objectAtIndex(rectID);
        float offsetX = ((CCInteger *)frameData->getRectXArray()->objectAtIndex(i))->getValue();
        float offsetY = ((CCInteger *)frameData->getRectYArray()-> objectAtIndex(i))->getValue();
        float x = getPositionX() + offsetX;
        if (m_bClipMirror)
        {
            x = getPositionX() - offsetX - imageRect->getWidth();
        }
        float y = getPositionY() - offsetY - imageRect->getHeight();
        CCRect rect1 = CCRectMake(x, y, imageRect->getWidth(), imageRect->getHeight());
        if (rect1.containsPoint(point))
        {
            return true;
        }
    }
    return false;
}
