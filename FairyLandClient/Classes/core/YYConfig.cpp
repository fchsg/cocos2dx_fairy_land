#include "../common/Common.h"
#include "YYConfig.h"
#include "YYController.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#include <jni.h>
#endif

USING_NS_CC;
USING_NS_CC_EXT;

Config::Config()
{
    m_nRecentServerCount = 0;
    m_pServerListArray = new cocos2d::CCArray(15);
    m_nListServerIndex = 0;
    m_nListServerCount = 1;
    m_nGameServerIndex = 0;
}

Config::~Config()
{
    CC_SAFE_DELETE(m_pServerListArray);
}

static Config *s_Config = NULL;

Config * Config::getInstance()
{
    if (s_Config == NULL)
    {
        s_Config = new Config();
    }
    return s_Config;
}

void Config::loadConfig()
{
	CCUserDefault *ud = CCUserDefault::sharedUserDefault();
    m_sListServer = ud->getStringForKey("ListServer");
    if (m_sListServer.empty()||m_sChannel.compare("") == 0)
    {
        //m_sListServer = "alicloud,115.28.107.124,8000,0,0,1"; //外网
        m_sListServer = "alicloud,192.168.0.34,8000,0,0,1";     //内网
        ud->setStringForKey("ListServer",m_sListServer);
    }
	m_sUserID = ud->getStringForKey("UserID");
	m_sPassword = ud->getStringForKey("Password");
    m_sVersion = ud->getStringForKey("Version");
    if (m_sVersion.empty()||m_sVersion.compare("") == 0)
    {
        m_sVersion = "1.0.2";
        ud->setStringForKey("Version", m_sVersion);
    }
	m_sChannel = ud->getStringForKey("Channel");
    if (m_sChannel.empty()||m_sChannel.compare("") == 0)
    {
        m_sChannel = "01";
        ud->setStringForKey("Channel",m_sChannel);
    }
    m_nRecentServerCount = ud->getIntegerForKey("RecentServerCount");
	for(int i = 0;i < m_nRecentServerCount;i++)
	{
		char key[64];
		sprintf(key, "RecentServer%d",i);
		m_sRecentServer[i] = ud->getStringForKey(key);
	}
    m_bBGMusicPlay = ud->getBoolForKey("bgMusic",true);
    m_bEffectPlay  = ud->getBoolForKey("effectMusic",true);
    m_bSoundOff    = ud->getBoolForKey("soundoff",false);
    m_bAroundPlayerShow = ud->getBoolForKey("aroundplayer",true);
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    m_sUserPhoneInfo = getAndroidPhoneInfo(); //获取手机信息
    #else
    m_sUserPhoneInfo = "0_0_0_0";
    #endif
}

void Config::saveConfig()
{
	CCUserDefault *ud = CCUserDefault::sharedUserDefault();
    ud->setStringForKey("ListServer", m_sUserID);
    ud->setStringForKey("UserID", m_sUserID);
    ud->setStringForKey("Password", m_sPassword);
    ud->setStringForKey("Version", m_sVersion);
    ud->setStringForKey("Channel", m_sChannel);
    ud->setIntegerForKey("RecentServerCount", m_nRecentServerCount);
	for(int i = 0;i < m_nRecentServerCount;i++)
	{
		char key[64];
		sprintf(key, "RecentServer%d",i);
		ud->setStringForKey(key, m_sRecentServer[i]);
	}
    ud->setBoolForKey("bgMusic",m_bBGMusicPlay);
    ud->setBoolForKey("effectMusic",m_bEffectPlay);
    ud->setBoolForKey("soundoff",m_bSoundOff);
    ud->setBoolForKey("aroundplayer", m_bAroundPlayerShow);
}

LoginInfo Config::getLoginInfo()
{
    loginInfo = (LoginInfo){getUserID(),getPassword(),"0",getChannel(),Config::getVersion(),getPhoneInfo(),1};
    return loginInfo;
}

std::string Config::getUserID()
{
	return m_sUserID;
}

void Config::setUserID(std::string &u)
{
	m_sUserID = u;
	CCUserDefault::sharedUserDefault()->setStringForKey("UserID", m_sUserID);
}

//
std::string  Config::getPassword()
{
	return m_sPassword;
}

void Config::setPassword(std::string &p)
{
	m_sPassword = p;
	CCUserDefault::sharedUserDefault()->setStringForKey("Password", m_sPassword);
}

void Config::setVersion(std::string &str)
{
	m_sVersion = str;
    CCUserDefault::sharedUserDefault()->setStringForKey("Version", m_sVersion);
}

std::string Config::getVersion()
{
	return m_sVersion;
}

std::string Config::getRecentServer(int i)
{
	if(i < m_nRecentServerCount)
		return m_sRecentServer[i];
	return NULL;
}
std::string Config::getChannel()
{
    return m_sChannel;
}
void Config::setChannel(std::string &p)
{
    m_sChannel = p;
    CCUserDefault::sharedUserDefault()->setStringForKey("Channel", m_sChannel);
}
/**
 * 选中列表中的服务器重新排序
 */
void Config::sortRecentServerList(const char * serverInfo,int i)
{
    for (int j = i; j > 0; j--)
    {
        m_sRecentServer[j] = m_sRecentServer[j - 1];
    }
    m_sRecentServer[0] = serverInfo;
}

void Config::addServerInofToList(const char * serverInfo)
{
    // 检测是否已经被添加在最近玩过的服务器列表中
    for (int i = 0; i < m_nRecentServerCount; i++)
    {
        if (strcmp(m_sRecentServer[i].c_str(),serverInfo) == 0)
        {
            sortRecentServerList(serverInfo,i);//
            return;
        }
    }
    m_nRecentServerCount = m_nRecentServerCount >= RECENT_SERVER_MAX ? RECENT_SERVER_MAX : ++m_nRecentServerCount;
    for (int i = m_nRecentServerCount - 1; i > 0; i--)
    {
        m_sRecentServer[i] = m_sRecentServer[i-1];
    }
    m_sRecentServer[0] = serverInfo;
	saveConfig();
}

std::string Config::getListServer()
{
	return m_sListServer;
}

int Config::getGameServerCount()
{
    if (m_pServerListArray)
    {
        return m_pServerListArray->count();
    }
    else
    {
        return 0;
    }
}

void Config::setGameServer(cocos2d::CCArray *array)
{
    m_pServerListArray->removeAllObjects();
    m_pServerListArray->addObjectsFromArray(array);
    for (int i = 0; i < m_nRecentServerCount; i++)
    {
        bool isExist = false;
        for (int j = 0; j < array->count(); j++)
        {
            const char * serverInfo = static_cast<CCString *>(array->objectAtIndex(j))->getCString();
            if (strcmp(m_sRecentServer[i].c_str(), serverInfo) == 0)
            {
                isExist = true;
            }
        }
        if (!isExist)
        {
            for (int j = i; j < m_nRecentServerCount - 1; j++)
            {
                m_sRecentServer[j] = m_sRecentServer[j+1];
            }
            m_nRecentServerCount--;
        }
    }
    saveConfig();
    Controller::getInstance()->gotologinUI();
}

cocos2d::CCString * Config::getGameServer(int i)
{
    CCString *gameServerInfo = NULL;
	if(m_pServerListArray && i < m_pServerListArray->count())
		gameServerInfo = static_cast<CCString *>(m_pServerListArray->objectAtIndex(i));
	return gameServerInfo;
}

void Config::clearRoleInfo()
{
    
}

//=======================================
ServerItem::ServerItem(const char* line)
{
	std::string temp;
    cocos2d::CCArray * arr = split_string(line, ',');
    int index = 0;
    serverName = static_cast<CCString *>(arr->objectAtIndex(index++))->getCString();
    serverHost = static_cast<CCString *>(arr->objectAtIndex(index++))->getCString();
    serverPort = static_cast<CCString *>(arr->objectAtIndex(index++))->intValue();
    isNew = static_cast<CCString *>(arr->objectAtIndex(index++))->intValue() == 1;
    serverStatus = static_cast<CCString *>(arr->objectAtIndex(index++))->intValue();
    serverID = static_cast<CCString *>(arr->objectAtIndex(index++))->getCString();
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
std::string Config::getAndroidPhoneInfo()
{
    JniMethodInfo minfo;
    bool bIsHave = JniHelper::getStaticMethodInfo(minfo, "com/yourcompany/fairyland/FairyLand", "getPhoneInfo", "()Ljava/lang/String;");
    if (bIsHave)
    {
        jobject jObj = minfo.env->CallStaticObjectMethod(minfo.classID, minfo.methodID);     //调用函数
        jstring str = (jstring)jObj;
        std::string info = JniHelper::jstring2string(str);
        CCLOG("jni phone info %s",info.c_str());
        return info;
    }
}
#endif
