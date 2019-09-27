#include "Client.h"

int main()
{
	Client * MyClient = new Client();
	
	do { } while (MyClient->ConnectClient("127.0.0.1", 6666) == false);

	MyClient->SendToServer("Caca", sizeof("Caca"));
	MyClient->SetToListen();

	system("PAUSE");

	MyClient->CloseClient();
	delete MyClient;
}
