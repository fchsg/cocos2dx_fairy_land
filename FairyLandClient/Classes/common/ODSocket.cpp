//
//  ODSocket.cpp
//  Datang
//
//  Created by fuchenhao on 13-2-18.
//
//

#include "ODSocket.h"
#include "../common/Common.h"

#include <errno.h>
#ifdef WIN32
#pragma comment(lib, "wsock32")
#endif

#define RECIEVED_MESSAGE_MAX_LENGTH 8192	// 1048 recv每次接受的字符的长度上限(受设备数据链路层设计的限制)

USING_NS_CC;

static ODSocket * s_SingleSocket = NULL;

ODSocket::ODSocket(SOCKET sock)
{
	m_eNetStatus = kNetStatusDisConnect;
	m_sock = sock;
    m_eSocketStatus = kSocketServerClosed;
}

ODSocket * ODSocket::getInstance()
{
    if (s_SingleSocket == NULL)
    {
        s_SingleSocket = new ODSocket(INVALID_SOCKET);
        
    }
    return s_SingleSocket;
}

ODSocket::~ODSocket()
{
}

// this is just for windows
int ODSocket::Clean()
{
#ifdef WIN32
    return (WSACleanup());
#endif
    return 0;
}

ODSocket& ODSocket::operator = (SOCKET s)
{
	m_sock = s;
	return (*this);
}

ODSocket::operator SOCKET ()
{
	return m_sock;
}
// create a socket object win/lin is the same
//  af:
bool ODSocket::Create(int af, int type, int protocol)
{
	m_sock = socket(af, type, protocol);
	if ( m_sock == INVALID_SOCKET ) {
		return false;
	}
	return true;
}

bool ODSocket::Connect(const char* ip, unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = inet_addr(ip);
	svraddr.sin_port = htons(port);
    
	Create(AF_INET,SOCK_STREAM, 0);
	int ret = connect(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR )
    {
        CCLOG("connect failed");
		return false;
	}
    m_eNetStatus = kNetStatusConnected;
    startRequestThread(&posixThreadID, socketLoop,this); // 开启新线程
    pthread_detach(posixThreadID);
    return true;
}

bool ODSocket::Bind(unsigned short port)
{
	struct sockaddr_in svraddr;
	svraddr.sin_family = AF_INET;
	svraddr.sin_addr.s_addr = INADDR_ANY;
	svraddr.sin_port = htons(port);
    
	int opt =  1;
	if ( setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt)) < 0 )
		return false;
    
	int ret = bind(m_sock, (struct sockaddr*)&svraddr, sizeof(svraddr));
	if ( ret == SOCKET_ERROR )
    {
		return false;
	}
#if 0
#ifdef WIN32
    ioctlsocket(m_sock,FIONBIO,&opt);
#else
    ioctl(m_sock,FIONBIO,&opt);
#endif
#endif
	return true;
}
// for server
bool ODSocket::Listen(int backlog)
{
	int ret = listen(m_sock, backlog);
	if ( ret == SOCKET_ERROR )
    {
		return false;
	}
	return true;
}

bool ODSocket::Accept(ODSocket& s, char* fromip)
{
	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	SOCKET sock = accept(m_sock, (struct sockaddr*)&cliaddr, &addrlen);
	if ( sock == SOCKET_ERROR )
    {
		return false;
	}
    
	s = sock;
	if ( fromip != NULL )
		CCLOG(fromip, "%s", inet_ntoa(cliaddr.sin_addr));
    
	return true;
}

int ODSocket::Send(const char* buf, int len, int flags)
{
	int bytes;
	int count = 0;
	CCLOG("ODSocket::send %s",buf);
	while ( count < len )
    {
		bytes = send(m_sock, buf + count, len - count, flags);
		if ( bytes == -1 || bytes == 0 )
		{
            return -1;
		}
		count += bytes;
	}
	return count;
}

int ODSocket::Recv(char* buf, int len, int flags)
{
	return (recv(m_sock, buf, len, flags));
}

int ODSocket::Close()
{
	m_eNetStatus = kNetStatusDisConnect;
#ifdef WIN32
	return (closesocket(m_sock));
#else
    m_eSocketStatus = kSocketServerClosed;
	return (close(m_sock));
#endif
}

int ODSocket::GetError()
{
#ifdef WIN32
	return (WSAGetLastError());
#else
	return (errno);
#endif
}

bool ODSocket::DnsParse(const char* domain, char* ip)
{
	struct hostent* p;
	if ( (p = gethostbyname(domain)) == NULL )
		return false;
    
    CCLOG(ip,
          "%u.%u.%u.%u",
          (unsigned char)p->h_addr_list[0][0],
          (unsigned char)p->h_addr_list[0][1],
          (unsigned char)p->h_addr_list[0][2],
          (unsigned char)p->h_addr_list[0][3]);
	
	return true;
}

// do thread thing    
void * ODSocket::socketLoop(void * arg)
{
    char recvBuf[RECIEVED_MESSAGE_MAX_LENGTH + 1];
	ODSocket *sSocket = (ODSocket *)arg;
    int readBytes;
    SocketStatus socketStatus= sSocket->m_eSocketStatus;
	while (sSocket && sSocket->getNetStatus() == kNetStatusConnected)
    {
        readBytes = sSocket->Recv(recvBuf, RECIEVED_MESSAGE_MAX_LENGTH, 0);
        if ( readBytes > 0 )
        {
            recvBuf[readBytes]='\0';
            if (sSocket->getDelegate())
            {
                sSocket->getDelegate()->receivedMessage(recvBuf, readBytes);
            }
        }
        if (readBytes < 0)
        {
            break;
        }
    }

    if (sSocket->m_eSocketStatus == socketStatus)
    {
        CCLOG("socketLoop terminated");
        ODSocket::getInstance()->Close();
    }
    return NULL;
}
