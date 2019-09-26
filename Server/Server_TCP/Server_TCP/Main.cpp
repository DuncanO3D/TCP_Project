#include "Server.h"
#include "LogSystem.h"

int main()
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