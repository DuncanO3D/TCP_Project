#pragma once

#include <iostream>
#include <string>
#include <vector>
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

	void SendToServer(const char* data, unsigned short len);
	void SetToListen();

private:
	SOCKET m_Socket;	
	bool m_Connected;

	bool InitClient();
	bool InitSocket();
	bool ConnectToServer(const std::string& IP, unsigned short Port);

	void StopClient();

	void Send(const unsigned char* data, unsigned short len);
	void Receive(std::vector<char*> buffer);


	void LaunchSendingThread(const unsigned char* data, unsigned short networklen, unsigned short len, Client* ThisClient);
	void SendingDone(const unsigned char* DataSend);
	void SendingError(const unsigned char* DataSend);


	void LaunchListenThread(Client* ThisClient);
	void ListenDone(std::vector<char*> DataBuffer);
	void ListenError(const char* Error);
};