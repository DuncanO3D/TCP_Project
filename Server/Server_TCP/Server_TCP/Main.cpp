#include "Server.h"

int main()
{
	Server * MyServer = new Server();
	MyServer->LaunchServer(6666);

	MyServer->StopServer();
	delete MyServer;
}

