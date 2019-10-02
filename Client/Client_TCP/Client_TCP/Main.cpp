#include "Client.h"

int main()
{
	Client * MyClient = new Client();

	do
	{
		bool Connected = false;
		do {
			std::string IP = "127.0.0.1";
			int Port = 6666;
			std::string Name = "";
		
			std::cout << "IP : ";
			std::cin >> IP;
			std::cout << "Port : ";
			std::cin >> Port;
			std::cout << "Name : ";
			std::cin >> Name;
		
			system("CLS");
			Connected = MyClient->ConnectClient(IP, Port, Name);
		} while (Connected == false);

		MyClient->SetToListen();
		
		std::cout << "Dialog : (End for end the program)" << std::endl;
		char Buffer[255] = "";

		while (MyClient->IsConnected())
		{
			std::cin.getline(Buffer, sizeof(Buffer));

			if (strcmp(Buffer, "End") == 0)
				break;

			if(Buffer[0] != '\0')
				MyClient->SendToServer(Buffer);
		}
		system("CLS");
		std::cout << "Disconnected to server" << std::endl;
	} while (true);

	MyClient->CloseClient();
	delete MyClient;
}
