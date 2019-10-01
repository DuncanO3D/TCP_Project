#include "Client.h"

int main()
{
	Client * MyClient = new Client();

	do
	{
		bool Connected = false;
		do {
			char IP[50] = "127.0.0.1";
			int Port = 6666;

			std::cout << "IP : ";
			std::cin >> IP;
			std::cout << "Port : ";
			std::cin >> Port;

			Connected = MyClient->ConnectClient(IP, Port);
		} while (Connected == false);

		MyClient->SetToListen();


		//do
		//{
		//	MyClient->SendToServer("Test");
		//	Sleep(250);
		//} while (true);

		std::cout << "Dialog : (End for end the program)" << std::endl;
		char Buffer[255] = "";
		do
		{
			std::cin.getline(Buffer, sizeof(Buffer));

			if (strcmp(Buffer, "End") == 0)
				break;
			MyClient->SendToServer(Buffer);
		} while (true);

		std::cout << "Disconnected to server" << std::endl;
	} while (true);

	MyClient->CloseClient();
	delete MyClient;
}
