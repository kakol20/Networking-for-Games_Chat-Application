#include <iostream>
#include <Windows.h>

#include <SDL.h>
#include <SDL_net.h>

#include "String.h"
#include "Connection.h"

Connection server;

int main(int argc, char* argv[])
{
	if (!server.Init()) return -1;

	String address;
	std::cout << "Enter IP Address: ";
	std::cin >> address;

	/*String portString;
	std::cout << "Enter the port: ";
	std::cin >> portString;
	int port = atoi(portString.GetString());*/

	if (!server.ResolveHost(address, 42069)) return -1;

	if (!server.OpenSocket()) return -1;

	// --------------- MAIN CHAT APP ---------------

	bool isRunning = true;

	String messageSent;
	String messageRecv;

	std::cout << "Waiting for the server to send a message" << std::endl << "Type 'end' to finish chat" << std::endl;

	while (isRunning)
	{

		HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hconsole, 0x0A); // green
		std::cout << "Sever: ";

		SetConsoleTextAttribute(hconsole, 0x0F); // white
		if (!server.RecieveText(messageRecv)) break;
		std::cout << messageRecv;
		std::cout << std::endl << std::endl;

		if (messageRecv == "end")
		{
			isRunning = false;
			break;
		}

		SetConsoleTextAttribute(hconsole, 0x0C); // red
		std::cout << "Client: ";

		SetConsoleTextAttribute(hconsole, 0x0F); // white
		std::cin >> messageSent;
		std::cout << std::endl;
		if (!server.SendText(messageSent)) break;
		
		if (messageSent == "end")
		{
			isRunning = false;
			break;
		}
	}

	// ---------------

	server.CloseSocket();
	server.Shutdown();

	system("pause");

	return 0;
}