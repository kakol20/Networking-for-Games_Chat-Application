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

bool Connection::ResolveHost(const String& address, int port)
{
	if (SDLNet_ResolveHost(&m_IP, address.GetString(), port) == -1)
	{
		std::cout << "Error resolving host" << std::endl;

		system("pause");

		return false;
	}

	return true;
}

bool Connection::OpenSocket()
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

bool Connection::RecieveText(String& message)
{
	char response[2048] = { '\0' };

	if (SDLNet_TCP_Recv(m_socket, response, 2048) <= 0)
	{
		std::cout << "Error recieving message" << std::endl;
		system("pause");

		return false;
	}

	message = response;

	return true;
}

bool Connection::SendText(const String& message)
{
	size_t length = message.Length();

	if (SDLNet_TCP_Send(m_socket, message.GetString(), length) < (int)length)
	{
		std::cout << "Error sending message" << std::endl;
		system("pause");

		return false;
	}

	return true;
}

void Connection::CloseSocket()
{
	SDLNet_TCP_Close(m_socket);
}

void Connection::Shutdown()
{
	SDLNet_Quit();
	SDL_Quit();
}
