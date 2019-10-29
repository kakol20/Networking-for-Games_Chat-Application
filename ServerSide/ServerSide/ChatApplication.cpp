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

	if (!ResolveHost(1945)) return false;

	m_listenSocket = SDLNet_TCP_Open(&m_IP);

	bool running = true;

	clients.push_back(new Client());

	// Waiting for clients
	
	std::thread thread1;
	thread1 = std::thread(&ChatApplication::WaitForClients, *this);

	thread1.detach();

	// --------- MAIN ---------

	while (!m_exit)
	{
		for (auto it = clients.begin(); it != clients.end(); it++)
		{

		}
	}

	// --------- END ---------

	Shutdown();

	return true;
}

bool ChatApplication::Init()
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

bool ChatApplication::ResolveHost(int port)
{
	if (SDLNet_ResolveHost(&m_IP, nullptr, port) == -1)
	{
		std::cout << "Error creating server IP" << std::endl;

		system("pause");

		return false;
	}

	return true;
}

void ChatApplication::WaitForClients()
{
	while (!m_exit)
	{
		while (clients.back()->GetSocket() == nullptr)
		{
			clients.back()->ListenForClient(m_listenSocket);
		}

		std::thread t1(&Client::UpdateInfo, *clients.back());

		t1.detach();

		clients.push_back(new Client());
	}	
}

void ChatApplication::ReceiveText()
{
	
}

void ChatApplication::Shutdown()
{
	SDLNet_Quit();
	SDL_Quit();
}
