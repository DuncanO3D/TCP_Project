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

class Server
{
public:
	//Don't launche the server
	Server();
	//Launche the server
	Server(int Family, int Type, int Protocol, ULONG EntryAcceptances, unsigned int Port);
	~Server();

	#pragma region GETTER
	inline WSADATA GetData() const { return m_Data; }
	inline char* GetIP() const { return m_IP; }
	inline int GetFamily() const { return m_Family; }
	inline int GetType() const { return m_Type; }
	inline int GetProtocol() const { return m_Protocol; }
	inline ULONG GetEntryAcceptances() const { return m_EntryAcceptances; }
	inline int GetPort() const { return m_Port; }
	inline SOCKET GetSocket() const { return m_Socket; }
	inline sockaddr_in GetAddr() const { return m_Addr; }
	inline bool GetState() const { return m_State; }
	#pragma endregion

	int LauncheServer(int Family, int Type, int Protocol, ULONG EntryAcceptances, unsigned int Port);
	SOCKET PendingOfConnection();
	int CloseServer();

	static char* GetIP(sockaddr_in addr);

private:
	WSADATA m_Data;

	char* m_IP;

	int m_Family;
	int m_Type;
	int m_Protocol;
	ULONG m_EntryAcceptances;
	int m_Port;

	SOCKET m_Socket;
	sockaddr_in m_Addr;

	bool m_State = false;
};

