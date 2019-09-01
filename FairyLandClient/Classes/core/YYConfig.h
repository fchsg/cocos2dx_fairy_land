#ifndef __FAIRLAND__CONFIG__
#define __FAIRLAND__CONFIG__

#include "cocos2d.h"

#define LIST_SERVER_MAX		4
#define RECENT_SERVER_MAX	4

/**
 渠道名称	渠道号
 UC	500001
 360	500002
 当乐	500003
 口袋巴士	500004
 手游天下	500005
 一起手游	500006
 3533	500007
 摩游游	500008
 OPPO	500009
 渠道1   500010
 渠道2   500011
 渠道3   500012
 */

struct LoginInfo
{
    std::string userID;//用户名
    std::string password;//密码
    std::string mail;//邮箱
    std::string channel;//渠道
    std::string version;//版本号
    std::string phoneInfo;//手机信息（包含机型，号码，运营商，imei）
    int encording;
};

class ServerItem: public cocos2d::CCObject
{
public:
    
	ServerItem(const char* line);
    ~ServerItem(){}
	std::string serverName;
	std::string serverHost;
    int16_t     serverPort;
    uint8_t     serverStatus;
    std::string serverID;
	bool		isNew;
};

class Config: public cocos2d::CCObject
{
private:
	std::string m_sUserID;			//用户账号
	std::string m_sPassword;		//密码
    std::string m_sVersion;         //版本号
    std::string m_sChannel;         //渠道
	std::string m_sListServer;		//游戏服务器列表服务器，用于获取游戏服务器列表
    int m_nListServerIndex;
    int m_nListServerCount;
    std::string m_sRecentServer[RECENT_SERVER_MAX];	//最后玩过的服务器
    int m_nRecentServerCount;
    cocos2d::CCArray *m_pServerListArray;	//游戏服务器
    int m_nGameServerIndex;
    bool m_bBGMusicPlay;
    bool m_bEffectPlay;
    bool m_bSoundOff;
    bool m_bAroundPlayerShow;
    std::string m_sUserPhoneInfo;   //用户手机信息
    LoginInfo loginInfo;      // 登录信息
    
public:
    
    Config();
    ~Config();
    
    static Config *getInstance();
    void loadConfig();						//加载配置
	void saveConfig();						//保存配置
    
    LoginInfo getLoginInfo();
	//
	std::string getUserID();					//获取 UserID
	void setUserID(std::string &u);
    std::string getPassword();
    void setPassword(std::string &p);
    std::string getVersion();
    void setVersion(std::string &str);
    std::string getChannel();
    void setChannel(std::string &p);
    std::string getRoleName();
    void setRoleName(const char *);
    std::string getRoleID();
    void setRoleID(const char *);
    int getProfessionIndex();				//获取职业
    void setProfessionIndex(int pi);
    void clearRoleInfo();
	
    std::string getListServer();
    
	inline void setRecentServerCount(int count)
    {
        m_nRecentServerCount = count;
    }
    
	inline int getRecentServerCount()
    {
        return m_nRecentServerCount;
    }
    std::string getRecentServer(int i);
    void addServerInofToList(const char * serverInfo);	//将选中的服务器信息添加到近期玩过的服务器列表中
    void sortRecentServerList(const char * serverInfo,int i);//选中了已经加入近期服务器列表的服务器则重新排序
    int getGameServerCount();
    void setGameServer(cocos2d::CCArray *array);
    cocos2d::CCString * getGameServer(int i);
    inline bool isBackgroundMusicPlay()
    {
        return m_bBGMusicPlay;
    }
    inline void setBackgroundMusicPlay(bool _bPlay)
    {
        m_bBGMusicPlay = _bPlay;
    }
    
    inline bool isEffectPlay()
    {
        return m_bEffectPlay;
    }
    inline void setEffectPlay(bool _bPlay)
    {
        m_bEffectPlay = _bPlay;
    }
    
    inline bool isSoundOff()
    {
        return m_bSoundOff;
    }
    inline void setSoundOff(bool _bOff)
    {
        m_bSoundOff = _bOff;
    }
    inline bool isAroundPlayerShow()
    {
        return m_bAroundPlayerShow;
    }
    inline void setAroundPlayerShow(bool _bVisible)
    {
        m_bAroundPlayerShow = _bVisible;
    }
    inline std::string getPhoneInfo()
    {
        return m_sUserPhoneInfo;
    }
    std::string getAndroidPhoneInfo();

};

#endif
