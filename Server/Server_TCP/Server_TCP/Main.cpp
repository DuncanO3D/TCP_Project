#include "Server.h"
#include "ServerTest.h"
#include "LogSystem.h"

void TestServer();
void TestServerTest();

int main()
{
	//TestServer();
	TestServerTest();
}

void TestServer()
{
	Server * MyServer = new	Server(IPV4, TCP, TCP_SOCKET, ANY_SOURCE, 9999);
	if (MyServer->GetState() == true)
	{
		SOCKET NewClient = MyServer->PendingOfConnection();
		LogSystem::ConsoleLog("NewClient");

		MyServer->CloseServer();
	}
	else
		LogSystem::ConsoleLog("Server Launch Failed");

	system("PAUSE");
}

void TestServerTest()
{
	ServerTest * MyServer = new	ServerTest();
	MyServer->LauncheServer(IPV4, TCP, TCP_SOCKET, ANY_SOURCE, 9999);
	MyServer->PendingOfConnection();
}