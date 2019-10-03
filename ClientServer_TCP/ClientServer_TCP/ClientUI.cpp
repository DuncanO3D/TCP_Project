#include "ClientUI.h"

ClientUI::ClientUI()
{
	m_Client = new TCP::Client();
}

ClientUI::~ClientUI()
{
	if (m_Client != nullptr)
		delete m_Client;
}

void ClientUI::DrawUI()
{
	DrawEntrySpeech();
	DrawChooseParameter();
	Chat();
	Stop();
}

void ClientUI::DrawEntrySpeech()
{
	std::cout << "Welcome to the client launcher" << std::endl;
}

void ClientUI::DrawChooseParameter()
{
	bool Connected = false;
	do
	{
		std::cout << "Choose the server IP" << std::endl;
		std::cin >> m_IP;
		std::cout << "Choose the server Port" << std::endl;
		std::cin >> m_Port;
		std::cout << "Choose your name" << std::endl;
		std::cin >> m_Name;

		Connected = ConnectToServer();
		if (Connected == false)
			std::cout << "Connection failed" << std::endl;
		else
			m_Client->SetToListen();
	} while (Connected == false);
}

bool ClientUI::ConnectToServer()
{
	return m_Client->ConnectClient(m_IP, m_Port, m_Name);
}

void ClientUI::Chat()
{
	std::string Buffer;
	std::cout << "\"Close\" to return to the main menu" << std::endl;
	do
	{
		std::getline(std::cin, Buffer);

		if (Buffer.size() > 0 && Buffer != "Close")
		{
			m_Client->SendToServer(Buffer);
		}

	} while (m_Client->IsConnected() && Buffer != "Close");

	if (m_Client->IsConnected() == false)
		DrawEndServer();

	m_Client->CloseClient();
}

void ClientUI::DrawEndServer()
{
	std::cout << "Server closed" << std::endl;
	std::cout << "Returning to menu" << std::endl;
	Sleep(1000);
}
