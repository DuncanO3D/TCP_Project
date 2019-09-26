#pragma once

#include <iostream>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

//Simplification
#define IPV4 AF_INET
#define TCP SOCK_STREAM
#define TCP_SOCKET IPPROTO_TCP
#define ANY_SOURCE INADDR_ANY
#define SUCCES 1
#define NETWORK_ERROR -1
#define SERVER_ALREADY_OPEN -2
#define SERVER_ALREADY_CLOSE -3

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

class ServerTest
{
public:
	ServerTest();
	~ServerTest();

	int LauncheServer(int Family, int Type, int Protocol, ULONG EntryAcceptances, unsigned int Port);
	SOCKET PendingOfConnection();
	int CloseServer();

	static char* GetIP(sockaddr_in addr);

private:
	WSADATA wsaData;
	int iResult;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ClientSocket = INVALID_SOCKET;
	addrinfo *result = NULL;
	addrinfo hints;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
};

