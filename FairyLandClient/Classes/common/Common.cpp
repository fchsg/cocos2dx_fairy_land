//
//  Common.h
//  XJ
//
//  Created by ”⁄÷æ≈Ù on 13-2-7. 
//
//

#include "Common.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include <mach/mach.h>
#endif
USING_NS_CC;
using namespace std;

const std::string getFilePath(const std::string &fileName ,DirectoryType directoryType ,FileSuffix fileSuffix)
{
    std::string filePath = "";
    switch (directoryType)
    {
        case kDirTypeData:
            filePath = "data/";
            break;
        case kDirTypeScene:
            filePath = "scene/";
            break;
        case kDirTypeSprite:
            filePath = "sprite/";
            break;
        case kDirTypeUI:
            filePath = "ui/";
            break;
        case kDirTypeIcon:
            filePath = "icon/";
            break;
        case kDirTypeSound:
            filePath = "sound/";
            break;
        case kDirTypeEffect:
            filePath = "effect/";
            break;
        case kDirTypeCover:
            filePath = "cover/";
            break;
        case kDirTypePic:
            filePath = "pic/";
            break;
        default:
            break;
    }
    filePath.append(fileName);
    std::string defaultPath = cocos2d::CCFileUtils::sharedFileUtils()->getSearchPaths().at(0);
    defaultPath.append(filePath);
    switch (fileSuffix)
    {
        case kFileSuffixPng:
            filePath.append(".png");
            defaultPath.append(".png");
            break;
        case kFileSuffixAnu:
            filePath.append(".anu");
            defaultPath.append(".anu");
            break;
        case kFileSuffixJson:
            filePath.append(".json");
            defaultPath.append(".json");
            break;
        case kFileSuffixYbd:
            filePath.append(".ybd");
            defaultPath.append(".ybd");
            break;
        case kFileSuffixWav:
            filePath.append(".wav");
            defaultPath.append(".wav");
            break;
        case kFileSuffixMp3:
            filePath.append(".mp3");
            defaultPath.append(".mp3");
            break;
        case kFileSuffixPlist:
            filePath.append(".plist");
            defaultPath.append(".plist");
            break;
        default:
            break;
    }
    if(cocos2d::CCFileUtils::sharedFileUtils()->isFileExist(defaultPath.c_str()))
        return defaultPath;
    return filePath;
}

/*
 * 分割字符串
 * @src 待分割字符串
 * @token 分隔符
 * return 字符串数组
 */
cocos2d::CCArray * split_string(const char *src, char token){
    assert(src != NULL);
    std::vector<std::string> vec_result;
    char *start = (char *)src;
    char *end = NULL;
    int count = 0;
    while ( (end = strchr(start, token)) != NULL )
    {
        if ( end != start )
        {
            std::string buffer(start, end - start);
            vec_result.push_back(buffer);
            ++count;
        }
        start = end + 1;
        if(*start == token)
        {
            std::string buffer("");
            vec_result.push_back(buffer);
            ++count;
        }
    }
    if(start)
    {
        vec_result.push_back(start);
    }
    else
    {
        vec_result.push_back("");
    }
    ++count;
    cocos2d::CCArray * array = cocos2d::CCArray::createWithCapacity(count);
    for (int i = 0; i < count; i++)
    {
        std::string str = vec_result.at(i);
        array-> addObject(cocos2d::CCString::create(str));
    }
    vec_result.clear();
	return array;
}

/*
 * 分割字符串
 * @src 待分割字符串
 * @lineWidth 每行字符串最大宽度
 * @fontSize 字体大小
 * @token 分隔符
 * return 字符串数组
 */
cocos2d::CCArray * split_string(const char *src, float lineWidth,float fontSize,char token)
{
    assert(src != NULL);
    
    cocos2d::CCArray * array = cocos2d::CCArray::createWithCapacity(10);
    int length = strlen(src);
    float width = 0;
    char *start = (char *)src;// 起始字符 
    int count = 0;// 每行字符数  
    for (int i = 0; i < length;)
    {
        unsigned char c = (unsigned char)(*(src + i));
        if (c == token)
        {
            if (count > 0)
            {
                std::string buffer(start, count);
                array->addObject(cocos2d::CCString::create(buffer.c_str()));
                width = 0;
                count = 0;
            }
            i++;
            start = (char *)(src + i);
            continue;
        }
        else
        {
            int bytes;
            UTF8_NUMBER_OF_BYTES(c , bytes);
            CCAssert(bytes > 0, "inputed characters's number less than zero");
            cocos2d::CCLabelTTF * label = NULL;
            std::string tempStr(src + i, bytes);
            label = cocos2d::CCLabelTTF::create(tempStr.c_str(), DEFAULT_FONT_NAME, fontSize);
            if (width + label->getContentSize().width > lineWidth )
            {
                std::string buffer(start, count);
                array->addObject(cocos2d::CCString::create(buffer.c_str()));
                start = (char *)(src + i);
                width = label->getContentSize().width;
                count = 0;
            }
            else
            {
                width += label->getContentSize().width;
            }
            i += bytes;
            count += bytes;
            if (i == length)
            {// 结尾 
                std::string buffer(start, count);
                array->addObject(cocos2d::CCString::create(buffer.c_str()));
            }
        }
    }
    
    return array;
}

#define FACE_CHARACTER_BYTES 4

/*
 * 分割字符串
 * @src 待分割字符串
 * @lineWidth 每行字符串最大宽度
 * @fontSize 字体大小
 * @faceCharacter 表情对应的特殊字符
 * @token 分隔符
 * return 字符串数组
 */
cocos2d::CCArray * split_string(const char *src, float lineWidth,float fontSize,char faceCharacter,char token)
{
    assert(src != NULL);
    cocos2d::CCArray * array = cocos2d::CCArray::createWithCapacity(10);
    int length = strlen(src);
    float width = 0;
    char *start = (char *)src;
    int count = 0;
    cocos2d::CCArray * lineArray = cocos2d::CCArray::createWithCapacity(10);
    for (int i = 0; i < length;)
    {
        unsigned char c = (unsigned char)(*(src + i));
        int bytes;
        UTF8_NUMBER_OF_BYTES(c , bytes);
        CCAssert(bytes > 0, "inputed characters's number less than zero");
        if (c == token)
        {
            if (count > 0)
            {
                std::string buffer(start, count);
                lineArray->addObject(cocos2d::CCString::create(buffer.c_str()));
                array->addObject(lineArray);
                lineArray = cocos2d::CCArray::createWithCapacity(10);
                width = 0;
                count = 0;
            }
            i++;
        }
        else if (c == '#')
        {
            std::string buffer(c, FACE_CHARACTER_BYTES);
            cocos2d::CCSprite * sprite = cocos2d::CCSprite::create("");
            if (width + sprite->getContentSize().width > lineWidth)
            {
                array->addObject(lineArray);
                lineArray = cocos2d::CCArray::createWithCapacity(10);
                lineArray->addObject(sprite);
                width = sprite->getContentSize().width;
            }
            else
            {
                width += sprite->getContentSize().width;
                std::string buffer(start, count);
                lineArray->addObject(cocos2d::CCString::create(buffer.c_str()));
                lineArray->addObject(sprite);
            }
            start = (char *)(src + i + FACE_CHARACTER_BYTES);
            count = 0;
        }
        else
        {
            cocos2d::CCLabelTTF * label = NULL;
            std::string tempStr(src + i, bytes);
            label = cocos2d::CCLabelTTF::create(tempStr.c_str(), DEFAULT_FONT_NAME, fontSize);
            if (width + label->getContentSize().width > lineWidth )
            {
                if (count > 0)
                {
                    std::string buffer(start, count);
                    lineArray->addObject(cocos2d::CCString::create(buffer.c_str()));
                    array->addObject(lineArray);
                }
                lineArray = cocos2d::CCArray::createWithCapacity(10);
                start = (char *)(src + i);
                width = label->getContentSize().width;
                count = 0;
            }
            else
            {
                width += label->getContentSize().width;
            }
            i += bytes;
            count += bytes;
        }
        if (i == length)
        {
            if (count > 0)
            {
                std::string buffer(start, count);
                lineArray->addObject(cocos2d::CCString::create(buffer.c_str()));
            }
            array->addObject(lineArray);
        }
    }
    return array;
}


YYChatData * split_string(const char *src, float lineWidth,float fontSize,float initalWid,float lineHeight,int fontsize,std::string fontname,kChattingType chatType,char token,cocos2d::ccColor3B materialColor1, cocos2d::ccColor3B materialColor2)
{
    assert(src != NULL);
    YYChatData* pChatData = YYChatData::create();
    pChatData->setLineHeight(lineHeight);
    int length = strlen(src);
    float width = 0.0;
    int lineCount = 1;
    width += initalWid;
    char *start = (char *)src;
    int count = 0,firstByte = 0;
    cocos2d::CCLabelTTF * label = NULL;
    cocos2d::CCSprite * sprite = NULL;
    cocos2d::ccColor3B color = cocos2d::ccWHITE;
    pChatData->setLineAmount(lineCount);
    switch (chatType)
    {
        case kChattingSystemType:
            color = cocos2d::ccWHITE;
            break;
        case kChattingWorldType:
            color = cocos2d::ccYELLOW;
            break;
        case kChattingCityType:
            color = cocos2d::ccGREEN;
            break;
        case kChattingPriviteType:
            color = cocos2d::ccMAGENTA;
            break;
        case kChattingNoticeType:
            color = cocos2d::ccORANGE;
            break;
        default:
            break;
    }

    for (int i = firstByte; i < length;)
    {
        unsigned char c = (unsigned char)(*(src + i));
        if (c == token)
        {
            CCLOG("c == token");
            std::string tempStr(src + i, 4);
            std::string path = "ui/" + tempStr + ".png";
            CCLOG("tempStr =%s",tempStr.c_str());
            sprite = cocos2d::CCSprite::create(path.c_str());
            if(sprite)
            {
                std::string buffer(start, count);
                label = cocos2d::CCLabelTTF::create(buffer.c_str(), DEFAULT_FONT_NAME, fontSize);
                FontInfo pFontInfo;
                pFontInfo.name = buffer;
                pFontInfo.type = 1;
                pFontInfo.color = color;
                pFontInfo.fontSize = fontSize;
                pFontInfo.fontName = fontname;
                pFontInfo.point = ccp(width,lineHeight - (pChatData->getLineAmount()*lineHeight));
                pChatData->ChatArray.push_back(pFontInfo);
                width+= label->getContentSize().width;
                if (width + sprite->getContentSize().width > lineWidth )
                {
                    pChatData->setLineAmount(++lineCount);
                    width = initalWid;
                    FontInfo pFontInfo1;
                    pFontInfo1.name = path;
                    pFontInfo1.type = 2;
                    pFontInfo1.color = color;
                    pFontInfo.fontSize = fontSize;
                    pFontInfo.fontName = fontname;
                    pFontInfo1.point = ccp(width,lineHeight - (pChatData->getLineAmount()*lineHeight));
                    pChatData->ChatArray.push_back(pFontInfo1);
                }
                else
                {
                    FontInfo pFontInfo1;
                    pFontInfo1.name = path;
                    pFontInfo1.type = 2;
                    pFontInfo1.color = color;
                    pFontInfo.fontSize = fontSize;
                    pFontInfo.fontName = fontname;
                    pFontInfo1.point = ccp(width,lineHeight - (pChatData->getLineAmount()*lineHeight));
                    pChatData->ChatArray.push_back(pFontInfo1);
                }
                i += 4;
                start = (char *)(src + i);
                firstByte = i;
                width += sprite->getContentSize().width*1.6f;
                count = 0;
                continue;
            }
        }
        int bytes;
        UTF8_NUMBER_OF_BYTES(c , bytes);
        assert(bytes > 0);
        std::string tempStr(src + firstByte, i - firstByte + bytes);
        label = cocos2d::CCLabelTTF::create(tempStr.c_str(), DEFAULT_FONT_NAME, fontSize);
        if (width + label->getContentSize().width > lineWidth )  //多行
        {
            if(count)
            {
                std::string buffer(start, count);
                FontInfo pFontInfo;
                pFontInfo.name = buffer;
                pFontInfo.type = 1;
                pFontInfo.color = color;
                pFontInfo.fontSize = fontSize;
                pFontInfo.fontName = fontname;
                pFontInfo.point = ccp(width,lineHeight - (pChatData->getLineAmount()*lineHeight));
                pChatData->ChatArray.push_back(pFontInfo);
            }
            pChatData->setLineAmount(++lineCount);
            start = (char *)(src + i);
            firstByte = i;
            width = initalWid;
            count = 0;
        }
        else
        {
            i += bytes;
            count += bytes;
        }
        if (i == length)   //单行
        {
            if(count)
            {
                if (chatType == kChattingSystemType || chatType == kChattingPriviteType || chatType == kChattingNoticeType)
                {
                    ccColor3B colormateria[2] = {materialColor1,materialColor2};
                    parse_string(src, pChatData, colormateria, color, fontname, fontSize);
                }
                else
                {
                    std::string buffer(start, count);
                    FontInfo pFontInfo;
                    pFontInfo.name = buffer;
                    pFontInfo.type = 1;
                    pFontInfo.color = color;
                    pFontInfo.fontSize = fontSize;
                    pFontInfo.fontName = fontname;
                    pFontInfo.point = ccp(width,lineHeight - (pChatData->getLineAmount()*lineHeight));
                    pChatData->ChatArray.push_back(pFontInfo);
                }
            }
        }
    }
    return pChatData;
}

void parse_string(const char * src, YYChatData* data, ccColor3B color[], ccColor3B colorNormal, std::string fontName, float fontSize)
{
    if(src != NULL)
    {
        std::vector<std::string> vec_result;
        std::vector<bool> vec_flag;
        char *start = (char *)src;
        char *end   = NULL;
        int  count  = 0;
        int  nLen = strlen(src);
        bool bSwitch = true;
        while (count < nLen)
        {
            if((end = strchr(start, '[')) != NULL && bSwitch)
            {
                std::string buffer(start, end - start);
                vec_result.push_back(buffer);
                vec_flag.push_back(false);
                start = end + 1;
                count+=strlen(buffer.c_str())+1;
                bSwitch = false;
            }
            else if((end = strchr(start, ']')) != NULL && !bSwitch)
            {
                std::string buffer(start, end - start);
                vec_result.push_back(buffer);
                vec_flag.push_back(true);
                start = end + 1;
                count+=strlen(buffer.c_str())+1;
                bSwitch = true;
            }
            else
            {
                end = (char*)src + nLen;
                std::string buffer(start, end - start);
                vec_result.push_back(buffer);
                vec_flag.push_back(false);
                count+=strlen(buffer.c_str());
            }
        }
        
        int nColorIdx = 0;
        int width = 0;
        cocos2d::CCLabelTTF*  label = NULL;
        for (int i = 0; i < vec_result.size(); i++)
        {
            std::string str = vec_result.at(i);
            bool b = vec_flag.at(i);
            if (b)
            {
                FontInfo pFontInfo;
                pFontInfo.name = str;
                pFontInfo.type = 1;
                pFontInfo.color = color[nColorIdx++];
                pFontInfo.fontSize = fontSize;
                pFontInfo.fontName = fontName;
                pFontInfo.point = ccp(width,0);
                data->ChatArray.push_back(pFontInfo);
                
                label = cocos2d::CCLabelTTF::create(str.c_str(), DEFAULT_FONT_NAME, fontSize);
                width += label->getContentSize().width;
            }
            else
            {
                FontInfo pFontInfo;
                pFontInfo.name = str;
                pFontInfo.type = 1;
                pFontInfo.color = colorNormal;
                pFontInfo.fontSize = fontSize;
                pFontInfo.fontName = fontName;
                pFontInfo.point = ccp(width,0);
                data->ChatArray.push_back(pFontInfo);
                
                label = cocos2d::CCLabelTTF::create(str.c_str(), DEFAULT_FONT_NAME, fontSize);
                width += label->getContentSize().width;
            }
        }
    }
}

/*
 * @点
 * @圆心坐标
 * @圆半径
 * return 点是否在圆内
 */
bool isPointInCircle(cocos2d::CCPoint point,cocos2d::CCPoint centerPoint,int16_t radius)
{
    int distanceSquare = (point.x - centerPoint.x) * (point.x - centerPoint.x) + (point.y - centerPoint.y) * (point.y - centerPoint.y);
    if (distanceSquare < radius * radius) {
        return true;
    }
    return false;
}

/**
 * @str UI背景元素资源路径
 * @x UI背景左下角x坐标
 * @y UI背景左下角y坐标
 * @w UI背景宽
 * @h UI背景高
 * @tileNum 拼接UI背景所用元素块数量 0 整图背景 1 一个元素块（填充色块） 2两个元素块（角、边）3角、边、填充色块
 */
void drawBackground(const char * str,int x,int y,int w,int h,cocos2d::CCNode * parent,int tileNum){
    cocos2d::CCTexture2D *tex = cocos2d::CCTextureCache::sharedTextureCache()->addImage(str);
    if (tileNum == 0){
        cocos2d::CCSprite * sprite = cocos2d::CCSprite::createWithTexture(tex);
        sprite->setPosition(ccp(x + w / 2.0,y + h / 2.0));
        sprite->setScaleX(w/tex->getContentSize().width);
        sprite->setScaleY(h/tex->getContentSize().height);
        parent->addChild(sprite);
        return;
    }
    if(tex->getContentSize().width == w && tex->getContentSize().height == h){// ’˚Õº±≥æ∞ 
        cocos2d::CCSprite * sprite = cocos2d::CCSprite::createWithTexture(tex);
        sprite->setPosition(ccp(x + w / 2.0,y + h / 2.0));
        parent->addChild(sprite);
        return;
    }
    tex->setAliasTexParameters();
    //     tex->setAntiAliasTexParameters();
    float tileW = (tex->getContentSize().width) / tileNum;// ‘™ÀÿøÈ’˝≥£øÌ∂» 
    float tileH = tex->getContentSize().height;// ‘™ÀÿøÈ’˝≥£∏ﬂ∂»
    float clipTileW = tileW;
    float width = (w < tileW * 2) ? w / 2.0 : tileW;
    float height = (h < tileH * 2) ? h / 2.0 : tileH;
    int columnAmount = (w > width * 2) ? ((w - width * 2) / width) : 0;
    int rowAmount = (h > height * 2) ? ((h - height * 2) / height) : 0;
    float afterWidth = w - (columnAmount + 2) * tileW;//  £”‡Œ¥ÃÓ≥‰µƒøÌ∂» 
    float afterHeight = h - (rowAmount + 2) * tileH;//  £”‡Œ¥ÃÓ≥‰µƒ∏ﬂ∂»
    int tileSize = 4 + columnAmount * 2 + rowAmount * 2 + columnAmount * rowAmount + (afterWidth > 0 ? rowAmount + 2 : 0) + (afterHeight > 0 ?columnAmount + 2 : 0) + ((afterWidth > 0 && afterHeight > 0) ? 1 : 0);
    cocos2d::CCNode * node = NULL;
    if (dynamic_cast<cocos2d::CCSpriteBatchNode *>(parent)) {// »Áπ˚∏∏Ω⁄µ„ «CCSpriteBatchNode∂‘œÛ≤ª‘Ÿ¥¥Ω®batchNode 
        node = parent;
    }
    else
    {
        cocos2d::CCSpriteBatchNode * batchNode = cocos2d::CCSpriteBatchNode::createWithTexture(tex,tileSize);
        parent->addChild(batchNode);
        node = batchNode;
    }
    if (tileNum > 1)
    {
        // Ω« 
        cocos2d::CCSprite * corner = cocos2d::CCSprite::createWithTexture(tex, cocos2d::CCRectMake(0, 0, width , height));
        corner->setPosition(ccp(x + width / 2, y + h - height / 2));
        node->addChild(corner);
        corner = cocos2d::CCSprite::createWithTexture(tex, cocos2d::CCRectMake(0, 0, width , height));
        corner->setFlipX(true);
        corner->setPosition(ccp(x + w - width / 2, y + h - height / 2));
        node->addChild(corner);
        corner = cocos2d::CCSprite::createWithTexture(tex, cocos2d::CCRectMake(0, 0, width , height));
        corner->setFlipY(true);
        corner->setPosition(ccp(x + width / 2, y + height / 2));
        node->addChild(corner);
        corner = cocos2d::CCSprite::createWithTexture(tex, cocos2d::CCRectMake(0, 0, width , height));
        corner->setRotation(180);
        corner->setPosition(ccp(x + w - width / 2, y + height / 2));
        node->addChild(corner);
        // ±ﬂ 
        for (int i = 0; i < columnAmount; i++)
        {// …œœ¬¡Ω±ﬂ
            cocos2d::CCSprite * edge1 = cocos2d::CCSprite::createWithTexture(tex, cocos2d::CCRectMake(clipTileW, 0, tileW , height));
            edge1->setPosition(ccp(x + (tileW * 3 / 2) + i * tileW , y + h - height / 2));
            node->addChild(edge1);
            cocos2d::CCSprite * edge2 = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW, 0, tileW, height));
            edge2->setFlipY(true);
            edge2->setPosition(ccp(x + (tileW * 3 / 2) + i * tileW, y + height / 2));
            node->addChild(edge2);
        }
        for (int j = 0; j < rowAmount; j++)
        {// ◊Û”“¡Ω±ﬂ
            cocos2d::CCSprite * edge1 = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW, 0, tileH, width));
            edge1->setRotation(-90);
            edge1->setPosition(ccp(x + width / 2, y + h - tileH * 3 / 2 - j * tileH));
            node->addChild(edge1);
            cocos2d::CCSprite * edge2 = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW, 0, tileH, width));
            edge2->setRotation(90);
            edge2->setPosition(ccp(x + w - width / 2, y + h - tileH * 3 / 2 - j * tileH));
            node->addChild(edge2);
        }
    }
    if (tileNum != 2)
    {
        // ±≥æ∞ÃÓ≥‰‘™Àÿ 
        for (int i = 0; i < columnAmount; i++)
        {
            for (int j = 0; j < rowAmount; j++)
            {
                cocos2d::CCSprite * fillTile = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW * 2, 0, tileW, tileH));
                fillTile->setPosition(ccp(x + tileW * 3 / 2 + i * tileW, y + h - tileH * 3 / 2 - j * tileH));
                node->addChild(fillTile);
            }
        }
    }
    
    // ≤π¬© 
    if (afterWidth > 0)
    {
        if (tileNum != 2)
        {
            // ±≥æ∞ÃÓ≥‰‘™Àÿ 
            for (int j = 0; j < rowAmount; j++)
            {
                cocos2d::CCSprite * fillTile = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW * 2, 0, afterWidth, height));
                fillTile->setPosition(ccp(x + width + afterWidth / 2 + columnAmount * tileW, y + h - height * 3 / 2 - j * tileH));
                node->addChild(fillTile);
            }
        }
        if (tileNum > 1)
        {
            // …œ±ﬂ 
            cocos2d::CCSprite * edge1 = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW, 0, afterWidth, height));
            edge1->setPosition(ccp(x + width + afterWidth / 2 + columnAmount * tileW, y + h - height / 2));
            node->addChild(edge1);
            // œ¬±ﬂ 
            cocos2d::CCSprite * edge2 = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW, 0, afterWidth, height));
            edge2->setFlipY(true);
            edge2->setPosition(ccp(x + width + afterWidth / 2 + columnAmount * tileW, y + height / 2));
            node->addChild(edge2);
        }
    }
    if (afterHeight > 0)
    {
        if (tileNum != 2)
        {
            // ±≥æ∞ÃÓ≥‰‘™Àÿ 
            for (int i = 0; i < columnAmount; i++)
            {
                cocos2d::CCSprite * fillTile = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW * 2, 0, width, afterHeight));
                fillTile->setPosition(ccp(x + width * 3 / 2 + i * tileW, y + h - tileH - afterHeight / 2 - rowAmount * tileH));
                node->addChild(fillTile);
            }
        }
        if (tileNum > 1)
        {
            // ◊Û±ﬂ 
            cocos2d::CCSprite * edge1 = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW, 0, afterHeight, width));
            edge1->setRotation(-90);
            edge1->setPosition(ccp(x + width / 2, y + h - (rowAmount + 1)* tileH - afterHeight / 2));
            node->addChild(edge1);
            // ”“±ﬂ 
            cocos2d::CCSprite * edge2 = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW, 0, afterHeight, width));
            edge2->setRotation(90);
            edge2->setPosition(ccp(x + w - width / 2, y + h - (rowAmount + 1) * tileH - afterHeight / 2));
            node->addChild(edge2);
        }
    }
    if (tileNum != 2)
    {
        if (afterWidth > 0 && afterHeight > 0)
        { // ±≥æ∞ÃÓ≥‰‘™Àÿ
            cocos2d::CCSprite * fillTile = cocos2d::CCSprite::createWithTexture(tex,cocos2d::CCRectMake(clipTileW * 2, 0, afterWidth, afterHeight));
            fillTile->setPosition(ccp(x + width + afterWidth / 2 + columnAmount * tileW, y + h - height - afterHeight / 2 - rowAmount * tileH));
            node->addChild(fillTile);
        }
    }
}

#pragma mark 启动新线程 

int startRequestThread(pthread_t * threadID, thread_callfunc func, void *arg)
{
    //  Create the thread using POSIX routines.
    pthread_attr_t attr;
    
    int returnVal;
    returnVal = pthread_attr_init(&attr);
    assert(!returnVal);
    returnVal = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    assert(!returnVal);
    int threadSTatus = 0;
    
    threadSTatus = pthread_create(threadID, &attr, func, arg);
    returnVal = pthread_attr_destroy(&attr);
    assert(!returnVal);
    return threadSTatus;
}

CCObject *getNextElement(CCArray *arr,int& index)
{
    if (!arr || arr->count() <= index)
    {
        return NULL;
    }
    return arr->objectAtIndex(index++);
}

//   获取当前设备可用内存(单位：MB） 
double availableMemory()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    vm_statistics_data_t vmStats;
    mach_msg_type_number_t infoCount = HOST_VM_INFO_COUNT;
    kern_return_t kernReturn = host_statistics(mach_host_self(),
                                               HOST_VM_INFO,
                                               (host_info_t)&vmStats,
                                               &infoCount);
    if (kernReturn != KERN_SUCCESS)
    {
        return 0;
    }
    
    return ((vm_page_size *vmStats.free_count) / 1024.0) / 1024.0;
#else
    return 0;
#endif
}

//   获取当前任务所占用的内存（单位：MB） 
double usedMemory()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    task_basic_info_data_t taskInfo;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    kern_return_t kernReturn = task_info(mach_task_self(),
                                         TASK_BASIC_INFO,
                                         (task_info_t)&taskInfo,
                                         &infoCount);
    
    if (kernReturn != KERN_SUCCESS
        ) {
        return 0;
    }
    
    return taskInfo.resident_size / 1024.0 / 1024.0;
#else
    return 0;
#endif
}

void showAssertMsgBox(const char *objName,const char* file, int line)
{
	char msg[100];
	const char* filename = file;
	do{
		if(*file == '\\' || *file == '/')
			filename = file + 1;
	}while(*file++);

	sprintf(msg,"%s is null, %s ,%d", objName, filename, line);
	CCMessageBox(msg, "error");
}

const char* get_str_field(const char *s, char token, std::string &value)
{
	const char *p = s;
	if(p)
	{
		value.clear();
		while(*p && *p != token)
		{
			if(*p  == '\\')
				p++;
			value += *p;
			p++;
		}
		if( *p == 0)
			return 0;
		p++;
	}
	return p;
}
const char* get_str_field(const char *s, char token, char *valBuf, int valBufLen)
{
	const char *p = s;
	int count = 0;
	if(p)
	{
		while(*p && *p != token && count < valBufLen-1)
		{
			if(*p  == '\\')
				p++;
			valBuf[count++] = *p;
			p++;
		}
		valBuf[count] = 0;
		if( *p == 0)
			return 0;
		p++;
	}
	return p;
}

string ltrim(string& str) 
{ 
	if (str.find_first_not_of(" \n\r\t")!=string::npos){
		str = str.substr(str.find_first_not_of(" \n\r\t"));
	}
	return  str;
} 

string rtrim(string& str) 
{ 
	if (str.find_first_not_of("")!=string::npos){
		str = str.substr(0,str.find_last_not_of(" \n\r\t")+1); 
	}
	return str;
} 

string trim(string& str) 
{
    str = rtrim(str);
	return ltrim(str);
}
/**
 * 获取数字的前N位
 */
const int getFirstNBitNumber(int number,int n)
{
    if (pow(10 , n) > number)
    {
        return -1;
    }
    int num = number;
    while (num >= pow(10 , n))
    {
        num /= 10;
    }
    return num;
}

//static const int MAX_CHAR_LENGTH = 16*1024;
//char* str_splice(const char *format,cocos2d::CCArray *arr)
//{
//    va_list args;
//    va_start(args, format);
//    char buf[MAX_CHAR_LENGTH];
//    vsnprintf(buf, MAX_CHAR_LENGTH-3, format, args);
//    va_end(args);
//    return (char *)buf;
//}
