//
//  FileReadManager.cpp
//  Datang
//
//  Created by fuchenhao on 13-2-5. 
//
//

#include "FileReadManager.h"

#include "platform/CCFileUtils.h"

USING_NS_CC;

FileReadManager::FileReadManager(void)
:m_pBuffer(NULL)
,m_ulBufferSize(0)
,m_ulBufferIndex(-1)
{
}

FileReadManager::~FileReadManager(void)
{
    CC_SAFE_DELETE(m_pBuffer);
}

unsigned long FileReadManager::readFileData(const char* pszFileName, const char* pszMode,bool isBigEndian)
{
	std::string fpath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszFileName);
    const char *filepath = fpath.c_str();
    m_pBuffer = CCFileUtils::sharedFileUtils()->getFileData(filepath, pszMode, &m_ulBufferSize);
    m_ulBufferIndex = 0;
    m_bBigEndian = isBigEndian;
	return m_ulBufferSize;
}

int FileReadManager::readInt()
{
    const uint8_t size = sizeof(int);
    CC_ASSERT(m_ulBufferIndex + size <= m_ulBufferSize);
    if (m_ulBufferIndex + size <= m_ulBufferSize)
    {
        if (m_bBigEndian) {
            unsigned char temp[size];
            memcpy(&temp,m_pBuffer + m_ulBufferIndex,size);
            m_ulBufferIndex += size;
            return temp[0] << 24 | temp[1] << 16 | temp[2] << 8 | temp[3];// 大端高位在前 
        }
        int i = 0;
        memcpy(&i,m_pBuffer + m_ulBufferIndex,size);
        m_ulBufferIndex += size;
        return i;
    }
    else
    {
        const char * str = "read Int error because of out of range";
        CCLOG("%s",str);
        //        throw str;
        return NULL;
    }
    
}
short FileReadManager::readShort()
{
    const uint8_t size = sizeof(short);
    CC_ASSERT(m_ulBufferIndex + size <= m_ulBufferSize);
    
    if (m_bBigEndian) {
        unsigned char temp[size];
        memcpy(&temp,m_pBuffer + m_ulBufferIndex,size);
        m_ulBufferIndex += size;
        return temp[0] << 8 | temp[1];
    }
    short s = 0;
    memcpy(&s,m_pBuffer + m_ulBufferIndex,size);
    m_ulBufferIndex += size;
    return s;
}
char FileReadManager::readByte()
{
    const uint8_t size = sizeof(unsigned char);
    CC_ASSERT(m_ulBufferIndex + size <= m_ulBufferSize);
    char c;
    memcpy(&c,m_pBuffer + m_ulBufferIndex,size);
    m_ulBufferIndex += size;
    return c;
}

bool FileReadManager::readBool()
{
    return readByte() == 0 ? false : true;
}

const char * FileReadManager::readUTF(bool isReadQTExportFile)
{
    // utf 8字符串,QT输出的二进制文件用4个字节表示字符串长度,java输出的二进制文件用前2个字节（16位）表示字符串的长度 
    short length = 0;
    if (isReadQTExportFile)
    {
        length = readInt();
    }
    else
    {
        length = readShort();
    }
    CC_ASSERT(m_ulBufferIndex + length <= m_ulBufferSize);
    
    char * str = new char[length + 1];
    memcpy(str,m_pBuffer + m_ulBufferIndex,length);
    m_ulBufferIndex += length;
    str[length] = '\0';
    CCString* str1 = CCString::create(str);
    CC_SAFE_DELETE_ARRAY(str);
    return str1->getCString();
}

float FileReadManager::readFloat()
{
    const uint8_t size = sizeof(float);
    CC_ASSERT(m_ulBufferIndex + size <= m_ulBufferSize);
    if (m_bBigEndian) {
        int i = readInt();
        int s = ((i >> 31) == 0) ? 1 : -1;
        int e = ((i >> 23) & 0xff);
        int m = (e == 0) ?
        (i & 0x7fffff) << 1 :
        (i & 0x7fffff) | 0x800000;
        return s * (float)m * powf(2,e);
    }
    float f = 0;
    memcpy(&f,m_pBuffer + m_ulBufferIndex,size);
    m_ulBufferIndex += size;
    return f;
}
