#include "Connection.h"

Connection::Connection()
{
}

Connection::~Connection()
{
}

bool Connection::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		std::cout << "Error initializing SDL\n";

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

bool Connection::ResolveHost(int port)
{
	if (SDLNet_ResolveHost(&m_IP, nullptr, port) == -1)
	{
		std::cout << "Error creating server IP" << std::endl;

		system("pause");

		return false;
	}

	return true;
}

void Connection::OpenSocket()
{
	m_listenSocket = SDLNet_TCP_Open(&m_IP);
}

void Connection::WaitForClient()
{
	while (m_clientSocket == nullptr)
	{
		m_clientSocket = SDLNet_TCP_Accept(m_listenSocket);

		std::cout << ".";

		SDL_Delay(500);
	}
	std::cout << std::endl;

	// one client for now
	SDLNet_TCP_Close(m_listenSocket);
}

bool Connection::SendText(const String& message)
{
	size_t length = message.Length();

	if (SDLNet_TCP_Send(m_clientSocket, message.GetString(), length) < (int)length)
	{
		std::cout << "Error sending message" << std::endl;
		system("pause");

		return false;
	}

	return true;
}

bool Connection::RecieveText(String& message)
{
	char response[2048] = { '\0' };

	if (SDLNet_TCP_Recv(m_clientSocket, response, 2048) <= 0)
	{
		std::cout << "Error recieving message" << std::endl;
		system("pause");

		return false;
	}

	message = response;

	return true;
}

void Connection::CloseSocket()
{
	SDLNet_TCP_Close(m_clientSocket);
}

void Connection::Shutdown()
{
	SDLNet_Quit();
	SDL_Quit();
}
