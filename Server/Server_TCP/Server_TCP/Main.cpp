#include "Server.h"

void TestServer();

int main()
{
	//TestServer();
}

void TestServer()
{
	Server * MyServer = new Server();
	MyServer->LauncheServer(6666);

	sockaddr_in Client;
	MyServer->AcceptClients();

	system("PAUSE");

	MyServer->CloseServer();
	delete MyServer;
}
