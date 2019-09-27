#include "Server.h"
#include "LogSystem.h"

int main()
{
	Server * MyServer = new Server();
	MyServer->LauncheServer(6666);

	sockaddr_in Client;
	MyServer->AcceptClient(Client);

	system("PAUSE");

	MyServer->CloseServer();
	delete MyServer;
}