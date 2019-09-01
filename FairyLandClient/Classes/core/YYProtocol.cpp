#include "YYProtocol.h"

/**
 *生成注册命令
 */
std::string Protocol::getRegisterMessage(LoginInfo message)
{
	char cmd[200];
    sprintf(cmd, ASK_REGISTER, message.userID.c_str(), message.password.c_str(),message.mail.c_str(),message.channel.c_str(),message.version.c_str(), message.phoneInfo.c_str(), message.encording);
	return cmd;
}

/**
 *生成登录命令
 */
std::string Protocol::getLoginMessage(LoginInfo message)
{
	char cmd[200];
    sprintf(cmd, ASK_LOGIN, message.userID.c_str(), message.password.c_str(),message.channel.c_str(),message.version.c_str(), message.phoneInfo.c_str(), message.encording);
	return cmd;
}

/**
 *生成登陆命令
 */
std::string Protocol::getReconnectMessage(LoginInfo message,int roleIndex)
{
	char cmd[200];
    sprintf(cmd,ASK_RECONNECT, message.userID.c_str(), roleIndex ,message.password.c_str(),message.channel.c_str(),message.version.c_str(), message.phoneInfo.c_str(), message.encording);
	return cmd;
}