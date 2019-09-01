//
//  FileReadManager.h
//  Datang
//
//  Created by fuchenhao on 13-2-5. 
//
//

#ifndef __Datang__FileReadManager__
#define __Datang__FileReadManager__

#include "cocos2d.h"

class FileReadManager{
private:
	unsigned char * m_pBuffer;
	unsigned long m_ulBufferSize;
	unsigned long m_ulBufferIndex;
    bool m_bBigEndian;// 是否为大端读取(java输出的二进制文件默认以大端存储，qt默认为大端可修改为小端，c、c++、oc为小端）
public:
	FileReadManager(void);
	~FileReadManager(void);
	unsigned long readFileData(const char* pszFileName, const char* pszMode ,bool isBigEndian = true);
    int readInt();
    short readShort();
    char readByte();
    bool readBool();
    const char * readUTF(bool isReadQTExportFile = false/** 是否是读取QT导出的二进制文件 */);
    float readFloat();
};
#endif /* defined(__Datang__FileReadManager__) */
