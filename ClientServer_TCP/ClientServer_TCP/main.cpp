//Lib Linkage
#pragma comment(lib, "Ws2_32.lib")

#include "StartupUI.h"

int main()
{
	StartupUI * MainMenu = new StartupUI();

	MainMenu->Launch();

	delete MainMenu;

	return 0;
}