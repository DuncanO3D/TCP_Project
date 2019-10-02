#include "Server.h"

//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")
int main()
{
	unsigned int ServerPort = 6666;

	Server * MyServer = new Server();
	MyServer->LaunchServer(ServerPort);

	MyServer->StopServer();
	delete MyServer;
}