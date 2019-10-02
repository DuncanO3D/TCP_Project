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

	bool ConnectClient(std::string IP, unsigned short Port, std::string LocalName);
	void CloseClient();

	void SendToServer(std::string data);
	void SetToListen();
	bool IsConnected();

private:
	SOCKET m_Socket;	
	std::string m_Name;
	bool m_Connected;

	std::string m_ConnectedServer_Address;
	int m_ConnectedServer_Port;

	std::vector<std::thread*> m_LaunchedThreads;

	bool InitClient();
	bool InitSocket();
	bool ConnectToServer(const std::string& IP, unsigned short Port);

	void StopClient();
		
	void ToServer_Thread(std::string Data, Client* ThisClient);
	void ToServer(std::string Data);

	void LaunchListenThread(Client* ThisClient);
	void Listen();

	void Disconnected();

	void FreeLaunchedTreads();
};