#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
//Base Lib for network in c++
#include <WinSock2.h>
#include <WS2tcpip.h>
//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

class Client
{
public:
	Client();
	~Client();

	bool ConnectClient(const char* IP, unsigned short Port);
	void CloseClient();

	void SendToServer(const char* data);
	void SetToListen();

private:
	SOCKET m_Socket;	
	bool m_Connected;

	std::string m_ConnectedServer_Address;
	int m_ConnectedServer_Port;

	std::vector<std::thread*> m_LaunchedThreads;

	bool InitClient();
	bool InitSocket();
	bool ConnectToServer(const std::string& IP, unsigned short Port);

	void StopClient();

	void Send(const char* data);
	void Receive(std::vector<char*> buffer);
	
	void LaunchSendingThread(const char* data, Client* ThisClient);
	
	void LaunchListenThread(Client* ThisClient);

	void Disconnected();

	void FreeLaunchedTreads();
};