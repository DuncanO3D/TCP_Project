#include "Server.h"

int main()
{
	unsigned int ServerPort = 6666;

	Server * MyServer = new Server();
	MyServer->LaunchServer(ServerPort);

	MyServer->StopServer();
	delete MyServer;
}