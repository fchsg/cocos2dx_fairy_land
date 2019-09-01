//
//  Common.h
//  XJ
//
//  Created by fuchenhao on 13-2-7. 
//
//

#ifndef XJ_Common_h
#define XJ_Common_h

#include <sys/types.h>
#include <pthread.h>
#include "cocos2d.h"
#include "StringRes.h"
#include "../core/YYChatData.h"

enum kChattingType
{
    kChattingWorldType = 0,   // 世界消息
    kChattingCityType,        // 城市消息
    kChattingSystemType,      // 系统消息
    kChattingNoticeType,      // 公告消息
    kChattingPriviteType,     // 自己可见
    kChattingNormalType       // 默认状态
};

// 游戏帧数(每秒逻辑循环执行的次数) 
#define FRAME_PER_SECOND 20

#ifdef WIN32
extern int random();
extern char* gb2312_utf8(const char* gb2312);

// #define random rand
#endif
#define RANDOM_INT(__MIN__, __MAX__) ((__MIN__) + random() % ((__MAX__ + 1) - (__MIN__)))

#define DEFAULT_FONT_NAME "Verdana-Bold"

#define UTF8_NUMBER_OF_BYTES(Char, Len)        \
if (Char < 128)                \
{                        \
Len = 1;                    \
}                        \
else if ((Char & 0xe0) == 0xc0)        \
{                        \
Len = 2;                    \
}                        \
else if ((Char & 0xf0) == 0xe0)        \
{                        \
Len = 3;                    \
}                        \
else if ((Char & 0xf8) == 0xf0)        \
{                        \
Len = 4;                    \
}                        \
else if ((Char & 0xfc) == 0xf8)        \
{                        \
Len = 5;                    \
}                        \
else if ((Char & 0xfe) == 0xfc)        \
{                        \
Len = 6;                    \
}                        \
else                        \
Len = -1;

#define UTF8_LENGTH(Char)            \
((Char) < 0x80 ? 1 :                \
((Char) < 0x800 ? 2 :            \
((Char) < 0x10000 ? 3 :            \
((Char) < 0x200000 ? 4 :            \
((Char) < 0x4000000 ? 5 : 6)))))

#define MAPRECT 32

#define STANDARD_SCENE_WIDTH 960

#define STANDARD_SCENE_HEIGHT 640

#define LOGIC_LAYER_HEIGHT 480// 逻辑层单元格标准高度 

enum {// 对齐方式 
    kAlignLeftTop = 0, // 左上 
    kAlignLeftMid,// 左中 
    kAlignLeftBottom, // 左下 
    kAlignMidTop,// 中上 
    kAlignCenter,// 中心 
    kAlignMidBottom,// 中下 
    kAlignRightTop,// 右上 
    kAlignRightMid,// 右中 
    kAlignRightBottom// 右下 
};

// ——————————登录信息—————————— 
#define KC "YoyO0DTXY1"
#define INS "DT300"
#define SP "00"
#define DEVICE "iphone"
// ——————————登录信息end——————————

#define DOWNLOADED_DIRECTORY "tempdir"//资源下载路径

enum DirectoryType{
    kDirTypeData = 0,
    kDirTypeScene = 1,
    kDirTypeSprite = 2,
    kDirTypeUI = 3,
    kDirTypeIcon = 4,
    kDirTypeSound = 5,
    kDirTypeEffect = 6,
    kDirTypeCover = 7,
    kDirTypePic = 8,
};

enum FileSuffix{
    kFileSuffixPng = 1,
    kFileSuffixAnu = 2,
    kFileSuffixJson = 3,
    kFileSuffixYbd = 4,
    kFileSuffixWav = 5,
    kFileSuffixMp3 = 6,
    kFileSuffixPlist = 7,
};

//背景音乐
#define COVER_MUSIC             "cover"
#define MAINCITY_MUSIC          "maincity"
#define BATTLE_MUSIC1           "battle1"
#define BATTLE_MUSIC2           "battle2"

//音效资源
#define CLICKED_EFFECT          "drop"  //常规按键点击按钮
#define MENU_EFFECT             "menu"  //点击右下角主菜单展开键
#define CLOSEUI_EFFECT          "closed" //通用关闭界面
#define UPDATE_EFFECT           "update" //角色升级
#define ATTACK3_EFFECT          "attack3" //战士普攻
#define ATTACK2_EFFECT          "attack2" //法师普攻
#define ATTACKED_EFFECT         "attacked" //主角受击
#define EQUIPSUCCESS_EFFECT     "equipsuccess" //主强化、锻造、打孔等成功
#define EQUIPFAILED_EFFECT      "equipfailed" //主强化、锻造、打孔等失败
#define TOUCHPROTOSS_EFFECT     "touchprotoss" //点击占灵
#define QUICKROTOSS_EFFECT      "quickprotoss" //一键占灵
#define QUICKGET_EFFECT         "quickget" //一件收取
#define QUICKSALE_EFFECT        "quicksale" //一键卖出
#define PUTEQUIP_EFFECT         "openbag" //佩戴装备
#define REWARDSTAR_EFFECT       "rewardstar" //战斗奖励界面星星落下
#define CARD_EFFECT             "menu" //点击翻牌
#define OPENTREASURE_EFFECT     "opentreasure" //打开宝箱 ???
#define PORTAL_EFFECT           "portal" //传送门
#define RECOVERHP_EFFECT        "recoverhp"  //回血
#define CUTTREE_EFFECT          "cuttree" //砍树

#define PLAY_EFFECT(a) if(Config::getInstance()->isEffectPlay()) \
CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(a)

#define PLAY_BGMUSIC(a) if (Config::getInstance()->isBackgroundMusicPlay()) \
CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(a, true)

extern const std::string getFilePath(const std::string &fileName, DirectoryType dirType,FileSuffix fileSuffix);
/*
 * 分割字符串
 * @src 待分割字符串
 * @token 分隔符
 * return 字符串数组
 */
extern cocos2d::CCArray * split_string(const char *src, char token);
/*
 * 分割字符串
 * @src 待分割字符串
 * @lineWidth 每行字符串最大宽度
 * @fontSize 字体大小
 * @token 分隔符
 * return 字符串数组
 */
extern cocos2d::CCArray * split_string(const char *src, float lineWidth,float fontSize,char token = NULL);

#define FACE_CHARACTER_BYTES 4// 定义表情对应的特殊字符占用的字节数 
extern cocos2d::CCString * getStringWithFormat(cocos2d::CCString * tempStr,cocos2d::CCArray *arr);
/*
 * 分割字符串
 * @src 待分割字符串
 * @lineWidth 每行字符串最大宽度
 * @fontSize 字体大小
 * @faceCharacter 表情对应的特殊字符
 * @token 分隔符
 * return 字符串数组
 */
extern cocos2d::CCArray * split_string(const char *src, float lineWidth,float fontSize,char faceCharacter,char token = NULL);

extern YYChatData * split_string(const char *src, float lineWidth,float fontSize,float initalWid,float rowHeight,int fontsize,std::string fontname,kChattingType chatType = kChattingNormalType,char token = NULL, cocos2d::ccColor3B materialColor1 = cocos2d::ccWHITE, cocos2d::ccColor3B materialColor2 = cocos2d::ccWHITE);

extern void parse_string(const char * src, YYChatData* data, cocos2d::ccColor3B color[], cocos2d::ccColor3B colorNormal, std::string fontName, float fontSize);
/*
 * @点
 * @圆心坐标
 * @圆半径
 * return 点是否在圆内
 */
extern bool isPointInCircle(cocos2d::CCPoint point,cocos2d::CCPoint centerPoint,int16_t radius);
/**
 * @str UI背景元素资源路径
 * @x UI背景左下角x坐标
 * @y UI背景左下角y坐标
 * @w UI背景宽
 * @h UI背景高
 * @tileNum 拼接UI背景所用元素块数量 0 整图背景 1 一个元素块（填充色块） 2两个元素块（角、边）3角、边、填充色块
 */
extern void drawBackground(const char * str,int x,int y,int w,int h,cocos2d::CCNode * parent,int tileNum);

#pragma mark 启动新线程 

typedef void * (thread_callfunc)(void*);

extern int startRequestThread(pthread_t * threadID, thread_callfunc func, void*arg);

extern cocos2d::CCObject *getNextElement(cocos2d::CCArray *array,int& index);

//   获取当前设备可用内存(单位：MB）
extern double availableMemory();
//   获取当前任务所占用的内存（单位：MB） 
extern double usedMemory();

extern void showAssertMsgBox(const char *objName,const char*file, int line);
#define YYASSERT_NULL(x)	if(x == NULL){showAssertMsgBox("" #x, __FILE__, __LINE__);return;}

// 截取字符串中 token 结尾的一段数据 
extern const char* get_str_field(const char *s, char token, std::string &value);
const char* get_str_field(const char *s, char token, char *valBuf, int valBufLen);
extern const int getFirstNBitNumber(int number,int n);
extern void graySprite(cocos2d::CCSprite *sprite);
//extern char* str_splice(const char *format,cocos2d::CCArray *arr);
#ifdef WIN32
extern int random();
#endif

#endif
