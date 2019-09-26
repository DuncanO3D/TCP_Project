#include "ServerTest.h"



ServerTest::ServerTest()
{
}


ServerTest::~ServerTest()
{
}

int ServerTest::LauncheServer(int Family, int Type, int Protocol, ULONG EntryAcceptances, unsigned int Port)
{
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	//Set hints to a zero fill memory
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = Family;
	hints.ai_socktype = Type;
	hints.ai_protocol = Protocol;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	return 0;
}

SOCKET ServerTest::PendingOfConnection()
{
	sockaddr_in m_ConnectionAddress = { 0 };
	int len = sizeof(m_ConnectionAddress);

	// Accept a client socket
	ClientSocket = accept(ListenSocket, (SOCKADDR*)& m_ConnectionAddress, &len);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	return ClientSocket;
}

int ServerTest::CloseServer()
{
	// No longer need server socket
	closesocket(ListenSocket);
	// cleanup
	closesocket(ClientSocket);
	WSACleanup();
	return 0;
}

char * ServerTest::GetIP(sockaddr_in addr)
{
	return nullptr;
}
