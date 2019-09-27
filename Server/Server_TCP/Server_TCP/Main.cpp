#include "Server.h"

int main()
{
	unsigned int ServerPort = 0;
	std::cout << "Choose port : ";
	std::cin >> ServerPort;
	std::cout << std::endl;

	Server * MyServer = new Server();
	MyServer->LaunchServer(ServerPort);

	MyServer->StopServer();
	delete MyServer;
}