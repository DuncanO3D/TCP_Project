#include "Client.h"

int main()
{
	Client * MyClient = new Client();
	
	char IP[50] = "127.0.0.1";
	int Port = 6666;

	std::cout << "IP : ";
	std::cin >> IP;
	std::cout << "Port : ";
	std::cin >> Port;

	do { } while (MyClient->ConnectClient(IP, Port) == false);
	MyClient->SetToListen();

	std::cout << "Dialog : (End for end the program)" << std::endl;
	char Buffer[255] = "";
	do
	{
		std::cin >> Buffer;
		if (strcmp(Buffer, "End") == 0)
			break;
		MyClient->SendToServer(Buffer);
	} while (true);
	
	MyClient->CloseClient();
	delete MyClient;
}
