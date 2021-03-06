#include "ChatApplication.h"

ChatApplication::ChatApplication()
{
}

ChatApplication::~ChatApplication()
{
}

bool ChatApplication::Run()
{
	if (!Init()) return false;

	if (!ResolveHost()) return false;

	if (!OpenSocket()) return false;

	if (!SendInfo()) return false;

	// --------- MAIN ---------

	system("cls");

	m_exit = false;

	std::cin.ignore(1);

	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hconsole, 64);

	std::cout << "                                                                                                               " << std::endl;
	std::cout << ". . .     |                            --.--         --.--|             ,---.|         |        ,---.          " << std::endl;
	std::cout << "| | |,---.|    ,---.,---.,-.-.,---.      |  ,---.      |  |---.,---.    |    |---.,---.|---     |---|,---.,---." << std::endl;
	std::cout << "| | ||---'|    |    |   || | ||---'      |  |   |      |  |   ||---'    |    |   |,---||        |   ||   ||   |" << std::endl;
	std::cout << "`-'-'`---'`---'`---'`---'` ' '`---'      `  `---'      `  `   '`---'    `---'`   '`---^`---'    `   '|---'|---'" << std::endl;
	std::cout << "                                                                                                     |    |    " << std::endl;

	// recieving text

	SetConsoleTextAttribute(hconsole, 15);

	std::thread thread1 = std::thread(&ChatApplication::ReceiveText, this);
	thread1.detach();

	while (!m_exit)
	{
		// sending text
		SendText();

		std::cin.ignore(0);
	}

	// --------- END ---------

	CloseSocket();
	Shutdown();

	return true;
}

bool ChatApplication::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << "Error initializing SDL thr\n";

		system("pause");

		return false;
	}

	if (SDLNet_Init() == -1)
	{
		std::cout << "Error initializing SDL Net\n";

		system("pause");

		return false;
	}

	return true;
}

bool ChatApplication::ResolveHost()
{
	String address;
	std::cout << "Enter IP Adress: ";
	std::cin >> address;

	if (SDLNet_ResolveHost(&m_IP, address.GetString(), 1945) == -1)
	{
		std::cout << "Error resolving host" << std::endl;

		system("pause");

		return false;
	}

	return true;
}

bool ChatApplication::OpenSocket()
{
	m_socket = SDLNet_TCP_Open(&m_IP);

	if (!m_socket)
	{
		std::cout << "Error opening socket connection" << std::endl;

		system("pause");

		return false;
	}

	return true;
}

void ChatApplication::ReceiveText()
{
	while (!m_exit)
	{
		char response[2048] = { '\0' };

		//std::cin.ignore();

		if (SDLNet_TCP_Recv(m_socket, response, 2048) <= 0)
		{
			if (!m_exit)
			{
				std::cout << "Error recieving message" << std::endl;
				system("pause");

				m_exit = true;
			}
		}
		else
		{
			String messageStr = response;

			if (messageStr == "exit")
			{
				m_exit = true;
			}
			else
			{
				// parses recieved text
				/// A$B$C
				/// $ - Seperator
				/// A - The name of the client
				/// B - The color ID of the client
				/// C - The message

				int colorId = 15;

				char sep[] = "$";

				char* name;
				char* color;
				char* messageChar;

				char* next;

				name = strtok_s(response, sep, &next);
				color = strtok_s(NULL, sep, &next);
				messageChar = strtok_s(NULL, sep, &next);

				std::stringstream strs;
				strs << color;
				strs >> colorId;

				HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(hconsole, colorId);

				std::cout << name;

				SetConsoleTextAttribute(hconsole, 15); //white
				std::cout << " : " << messageChar << std::endl;
			}
		}
	}
}

void ChatApplication::SendText()
{
	String message = " ";

	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
	SetConsoleTextAttribute(hconsole, 7); //white
	//std::cout << " : ";

	std::cin >> message;

	//String temp = message;

	int length = message.Length() + 1;

	if (SDLNet_TCP_Send(m_socket, message.GetString(), length) < length)
	{
		std::cout << "Error sending message to server " << std::endl;

		m_exit = true;
		system("pause");
	}

	if (message == "exit")
	{
		m_exit = true;
	}
}

bool ChatApplication::SendInfo()
{
	system("cls");
	std::cout << "Enter your name: ";
	std::cin >> m_name;

	size_t length = m_name.Length() + 1;

	if (SDLNet_TCP_Send(m_socket, m_name.GetString(), length) < (int)length)
	{
		std::cout << "Error sending name" << std::endl;
		system("pause");

		return false;
	}

	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hconsole, 15);

	std::cout << std::endl << "Choose a color - " << std::endl;

	for (int i = 1; i < 15; i++)
	{
		SetConsoleTextAttribute(hconsole, i);
		std::cout << "Color #" << i << std::endl;
	}

	SetConsoleTextAttribute(hconsole, 15);

	std::cin >> m_color;

	char colorChar[4] = { '\0' };

	std::stringstream strs;
	strs << m_color;
	strs >> colorChar;

	String colorStr = colorChar;

	length = colorStr.Length() + 1;
	if (SDLNet_TCP_Send(m_socket, colorStr.GetString(), length) < (int)length)
	{
		std::cout << "Error sending color" << std::endl;
		system("pause");

		return false;
	}

	return true;
}

void ChatApplication::CloseSocket()
{
	SDLNet_TCP_Close(m_socket);
}

void ChatApplication::Shutdown()
{
	SDLNet_Quit();
	SDL_Quit();
}