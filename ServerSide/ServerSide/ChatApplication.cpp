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

	// Waiting for clients

	m_clients[m_clientIDCount] = new Client();
	
	std::thread thread1 = std::thread(&ChatApplication::WaitForClients, *this);

	thread1.detach();

	// --------- MAIN ---------

	while (!m_exit)
	{
		// Sending and receiving

		std::vector<int> disconnectedClients;

		for (auto it = m_clients.begin(); it != m_clients.end(); it++)
		{
			if (it->second->ClientConnected())
			{
				std::thread thread2 = std::thread(&ChatApplication::UpdateChat, it->first, *this);
				thread2.detach();
				//UpdateChat(it->first);
			}
		}

		// Check for disconnection of clients

		// Check for disconnection of server
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
		while (m_clients[m_clientIDCount]->GetSocket() == nullptr)
		{
			m_clients[m_clientIDCount]->ListenForClient(m_listenSocket);
		}

		std::thread t1(&Client::UpdateInfo, *m_clients[m_clientIDCount]);
		t1.detach();

		m_clientIDCount++;

		m_clients[m_clientIDCount] = new Client();
	}
}

void ChatApplication::Shutdown()
{
	SDLNet_Quit();
	SDL_Quit();
}

void ChatApplication::UpdateChat(int clientID)
{
	String message;

	m_clients[clientID]->ReceiveText(message);

	if (m_clients[clientID]->TextReceived())
	{
		if (message != "exit")
		{
			// Add other client's info - e.g. name and the color they chose
			String full = m_clients[clientID]->GetName();
			full += "$";

			std::stringstream strs;
			strs << m_clients[clientID]->GetColor();
			char colorAtt[2048] = { '\0' };
			strs >> colorAtt;

			full += colorAtt;
			full += "$";

			full += message;

			// sends it to other clients
			for (auto it = m_clients.begin(); it != m_clients.end(); it++)
			{
				if (it->first != clientID) // except for itself
				{
					it->second->SendText(full);
				}
			}

			HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hconsole, m_clients[clientID]->GetColor()); // sets color

			SetConsoleTextAttribute(hconsole, 15); // sets it back to white
			std::cout << ": " << message << std::endl;
		}
	}
}
