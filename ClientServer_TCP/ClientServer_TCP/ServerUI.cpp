#include "ServerUI.h"

ServerUI::ServerUI() : UI()
{
	m_Server = new TCP::Server();
	m_ServerUDP = new UDP::Server();
}

ServerUI::~ServerUI()
{
	if (m_Server != nullptr)
	{
		m_Server->StopServer();
		if (m_Server != nullptr)
			delete m_Server;
		m_Server = nullptr;
	}
	if (m_ServerUDP != nullptr)
	{
		m_ServerUDP->StopServer();
		if (m_ServerUDP != nullptr)
			delete m_ServerUDP;
		m_ServerUDP = nullptr;
	}
}

void ServerUI::DrawUI()
{
	DrawEntrySpeech();
	DrawChooseParameter();
	LaunchServer();
	DrawEndServer();
	Stop();
}

void ServerUI::DrawEntrySpeech()
{
	std::cout << "Welcome to the server launcher" << std::endl;
}

void ServerUI::DrawChooseParameter()
{
	std::cout << "Choose a port for the server" << std::endl;
	std::cin >> m_Port;
}

void ServerUI::LaunchServer()
{
	m_Server->LaunchServer(m_Port);
	//m_ServerUDP->LaunchServer(m_Port);
}

void ServerUI::DrawEndServer()
{
	std::cout << "Server closed" << std::endl;
	std::cout << "Returning to menu" << std::endl;
	Sleep(1000);
}
