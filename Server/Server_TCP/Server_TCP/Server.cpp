#include "Server.h"

#include "LogSystem.h"

Server::Server()
{
}

Server::Server(int Family, int Type, int Protocol, ULONG EntryAcceptances, unsigned int Port)
{
	if(LauncheServer(Family, Type, Protocol, EntryAcceptances, Port) == (SOCKET_ERROR || INVALID_SOCKET))
		LogSystem::ConsoleLog("Server Launch Failed");
}

Server::~Server()
{
	if (m_State == true)
		CloseServer();
}

int Server::LauncheServer(int Family, int Type, int Protocol, ULONG S_Addr, unsigned int Port)
{
	if (m_State == true)
	{
		LogSystem::ConsoleLog("Error : Server Already Open");
		return SERVER_ALREADY_OPEN;
	}

	m_Family = Family;
	m_Type = Type;
	m_Protocol = Protocol;
	m_EntryAcceptances = S_Addr;
	m_Port = Port;

	//For start networking
	if (WSAStartup(MAKEWORD(2, 2), &m_Data) == SOCKET_ERROR)
	{
		LogSystem::ConsoleLog("Error : WSA Startup => SOCKET ERROR");
		return NETWORK_ERROR;
	}

	//Socket creation
	m_Socket = socket(m_Family, m_Type, m_Protocol);
	if (m_Socket == INVALID_SOCKET)
	{
		LogSystem::ConsoleLog("Error : Socket Creation => INVALID SOCKET");
		return NETWORK_ERROR;
	}

	m_Addr.sin_addr.S_un.S_addr = m_EntryAcceptances;
	// 0 for autimaticaly choose a open PORT
	m_Addr.sin_port = htons(m_Port);
	m_Addr.sin_family = m_Family;

	//Binding Address to Socket
	if (bind(m_Socket, (struct sockaddr *) &m_Addr, sizeof(m_Addr)) == SOCKET_ERROR)
	{
		LogSystem::ConsoleLog("Error : Socket Binding => SOCKET ERROR");
		return NETWORK_ERROR;
	}

	//BSet the server to listen
	if (listen(m_Socket, SOMAXCONN) == SOCKET_ERROR)
	{
		LogSystem::ConsoleLog("Error : Server Listen => SOCKET ERROR");
		return NETWORK_ERROR;
	}

	LogSystem::ConsoleLog("Server Lauched Succefuly");

	m_State = true;

	return SUCCES;
}

SOCKET Server::PendingOfConnection()
{
	sockaddr_in m_ConnectionAddress = { 0 };
	int len = sizeof(m_ConnectionAddress);

	SOCKET newClient;

	if (newClient = accept(m_Socket, (SOCKADDR*)& m_ConnectionAddress, &len) == INVALID_SOCKET)
	{
		LogSystem::ConsoleLog("Error : Invalid Socket");
		return INVALID_SOCKET;
	}
			
	return newClient;
}

int Server::CloseServer()
{
	if (m_State == true)
	{
		if (closesocket(m_Socket) == SOCKET_ERROR)
		{
			LogSystem::ConsoleLog("Error : Close Socket => SOCKET ERROR");
			return NETWORK_ERROR;
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			LogSystem::ConsoleLog("Error : WSA Cleanup => SOCKET ERROR");
			return NETWORK_ERROR;
		}

		m_State = false;

		return SUCCES;
	}
	LogSystem::ConsoleLog("Error : Server Already Open");
	return SERVER_ALREADY_OPEN;
}
