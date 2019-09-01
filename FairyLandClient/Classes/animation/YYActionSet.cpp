//
//  ActionSet.cpp
//  Datang
//
//  Created by fuchenhao on 13-2-4. 
//
//

#include "../common/FileReadManager.h"
#include "../common/Common.h"
#include "YYActionSet.h"

USING_NS_CC;

ActionSet::ActionSet()
:m_u8ImageAmount(0)
,m_u16ClipAmount(0)
,m_u16FrameAmount(0)
,m_u16ActionAmount(0)
,m_pImageRectCountArray(NULL)
,m_pImageRectDataArray(NULL)
,m_pActionDataArray(NULL)
,m_pFrameDataArray(NULL)
,m_pFrameIndexArray(NULL)
,m_pDelayArray(NULL)
,m_pOffsetXArray(NULL)
,m_pOffsetYArray(NULL)
{
}

ActionSet::~ActionSet()
{
    CC_SAFE_DELETE(m_pImageRectCountArray);
    CC_SAFE_DELETE(m_pImageRectDataArray);
    CC_SAFE_DELETE(m_pActionDataArray);
    CC_SAFE_DELETE(m_pFrameDataArray);
    CC_SAFE_DELETE(m_pFrameIndexArray);
    CC_SAFE_DELETE(m_pDelayArray);
    CC_SAFE_DELETE(m_pOffsetXArray);
    CC_SAFE_DELETE(m_pOffsetYArray);
}

bool ActionSet::init()
{
    return true;
}

ActionSet* ActionSet::createActionSetWithFile(const std::string& aniFileName)
{
    ActionSet *actionSet = ActionSet::create();
    FileReadManager * fileReadManager = new FileReadManager();
    fileReadManager->readFileData(aniFileName.c_str(),"r");
    fileReadManager->readShort();// 文件字节数？ 
    fileReadManager->readUTF();// 版本号? 
    actionSet->m_u16ActionAmount =  fileReadManager->readByte();// 包含动作数 
    actionSet->m_pActionDataArray = new CCArray(actionSet->m_u16ActionAmount);
    for (int i = 0; i < actionSet->m_u16ActionAmount; i++)
    {
        ActionData * actData = ActionData::create();
        actData->setStartFrameIndex(fileReadManager->readShort());// 起始帧序号 
        actData->setEndFrameIndex(fileReadManager->readShort());// 终止帧序号 
        actionSet->m_pActionDataArray->addObject(actData);
    }
    actionSet->m_u16FrameAmount = fileReadManager->readShort();// 总帧数 
    actionSet->m_pFrameIndexArray = new CCArray(actionSet->m_u16FrameAmount);
    actionSet->m_pDelayArray = new CCArray(actionSet->m_u16FrameAmount);
    actionSet->m_pOffsetXArray = new CCArray(actionSet->m_u16FrameAmount);
    actionSet->m_pOffsetYArray = new CCArray(actionSet->m_u16FrameAmount);
    int8_t delay = 1;
    int16_t frameIndex = 0,offX = 0,offY = 0;
    for (int i = 0; i < actionSet->m_u16FrameAmount; i++)
    {
        frameIndex = fileReadManager->readShort();// 帧序号 
        delay = fileReadManager->readByte(); // 延迟 
        offX = fileReadManager->readShort();// x轴方向位移 
        offY = fileReadManager->readShort();// y轴方向位移 
        actionSet->m_pFrameIndexArray->addObject(CCInteger::create(frameIndex));
        actionSet->m_pDelayArray->addObject(CCInteger::create(delay));
        actionSet->m_pOffsetXArray->addObject(CCInteger::create(offX));
        actionSet->m_pOffsetYArray->addObject(CCInteger::create(offY));
    }
    actionSet->m_u16ClipAmount = fileReadManager->readShort();// 组成帧的切片总数 
    int frameNum = fileReadManager->readShort();// 实际帧数 
    actionSet->m_pFrameDataArray = new CCArray(frameNum);
    for (int i = 0; i < frameNum; i++)
    {
        short clipCount = fileReadManager->readShort();// 每帧所包含的切片数量 
        FrameData *fd = FrameData::createFrameData(clipCount);
        
        for (int j = 0; j < clipCount; j++)
        {
            short rectIndex = fileReadManager->readShort();// 切块在图片中的索引
            // 图片切块在该帧内的位置信息 
            short x =fileReadManager->readShort();
            short y= fileReadManager->readShort();
            char transformType = fileReadManager->readByte();// 翻转类型 
            fd->getRectIndexArray()->addObject(CCInteger::create(rectIndex));
            fd->getRectXArray()->addObject(CCInteger::create(x));
            fd->getRectYArray()->addObject(CCInteger::create(y));
            fd->getTransformTypeArray()->addObject(CCInteger::create(transformType));
        }
        actionSet->m_pFrameDataArray->addObject(fd);
    }
    short imgClipCount = fileReadManager->readShort();// 总切片数 
    actionSet->m_pImageRectDataArray = new CCArray(imgClipCount);
    actionSet->m_u8ImageAmount = fileReadManager->readByte();// 图片数 
    actionSet->m_pImageRectCountArray = new CCArray(actionSet->m_u8ImageAmount);
    for (int i = 0; i < actionSet->m_u8ImageAmount; i++)
    {
        short rectNum = fileReadManager->readShort();// 单张图片的切片数 
        actionSet->m_pImageRectCountArray-> addObject(CCInteger::create(rectNum));
        for (int j = 0; j < rectNum; j++)
        {
            // 切片在图片上的位置信息 
            short u = fileReadManager->readShort();
            short v = fileReadManager->readShort();
            short width = fileReadManager->readShort();
            short height = fileReadManager->readShort();
            YYRect *rect = YYRect::createRect(u,v,width ,height,i);
            actionSet->m_pImageRectDataArray->addObject(rect);
        }
    }
    delete fileReadManager;
    return actionSet;
}
