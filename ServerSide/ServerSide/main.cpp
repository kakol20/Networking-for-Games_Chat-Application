#include <iostream>
#include <sstream>
#include <Windows.h>

#include <SDL.h>
#include <SDL_net.h>

#include "String.h"

#include "Connection.h"

Connection host;

int main(int argc, char* argv[])
{
	if (!host.Init()) return -1;

	/*std::cout << "Enter the port: ";
	String portString;
	std::cin >> portString;
	int port = atoi(portString.GetString());*/

	if (!host.ResolveHost(42069)) return -1;

	host.OpenSocket();

	//host.WaitForClient();

	system("CLS");

	std::cout << "Client connected! Break the ice!" << std::endl << "Type 'end' to finish chat" << std::endl;

	// --------------- MAIN CHAT APP ---------------

	bool isRunning = false;

	String messageSent;
	String messageRecv;

	while (isRunning)
	{
		// https://cppmonastery.blogspot.com/2012/08/different-colors-in-console-window.html

		//HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//SetConsoleTextAttribute(hconsole, 0x0A); // green
		//std::cout << "Sever: ";

		//SetConsoleTextAttribute(hconsole, 0x0F); // white
		//std::cin >> messageSent;
		//std::cout << std::endl;
		//host.SendText(messageSent);

		//if (messageSent == "end")
		//{
		//	isRunning = false;
		//	break;
		//}

		//SetConsoleTextAttribute(hconsole, 0x0C); // red
		//std::cout << "Client: ";
		//if (!host.RecieveText(messageRecv)) break;

		//SetConsoleTextAttribute(hconsole, 0x0F); // white
		//std::cout << messageRecv;
		//std::cout << std::endl << std::endl;

		//if (messageRecv == "end")
		//{
		//	isRunning = false;
		//	break;
		//}
	}

	system("cls");

	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTextAttribute(hconsole, 7); // white

	//std::cout << "Test" << std::endl;

	// Turning char * to number
	if (true)
	{
		//HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//SetConsoleTextAttribute(hconsole, 0x0A); // green

		char input[] = "8<This is text";
		char* token;
		char* nextToken;

		token = strtok_s(input, "<", &nextToken);
		//token2 = strtok_s(input, "<", &nextToken2);

		std::stringstream strValue;
		strValue << token;

		int value;
		strValue >> value;

		String text = nextToken;

		SetConsoleTextAttribute(hconsole, value);
		std::cout << "Tester: ";

		SetConsoleTextAttribute(hconsole, 7); // white
		std::cout << text << std::endl;
	}

	system("pause");

	// --------------- END ---------------

	host.CloseSocket();
	host.Shutdown();

	system("pause");

	return 0;
}