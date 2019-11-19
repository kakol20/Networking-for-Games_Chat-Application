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
	
	std::thread thread1 = std::thread(&ChatApplication::WaitForClients, this);

	thread1.detach();

	// --------- MAIN ---------

	while (!m_exit)
	{
		std::vector<int> disconnectedClients;


		// Sending and receiving

		std::vector<std::thread> clientThreads; // Create a vector of threads for each client

		for (auto it = m_clients.begin(); it != m_clients.end(); it++)
		{
			if (it->second->ClientConnected())
			{
				clientThreads.push_back(std::thread(&ChatApplication::UpdateChat, this, it->first));

				if (it->second->IsDisconnecting())
				{
					disconnectedClients.push_back(it->first);
				}
			}
		}

		if (m_clients.size() - 1 == 1) // if there is only one client connected then server needs to send a message to that client
		{
			String message = "Server$7$No other client connected";

			clientThreads.push_back(std::thread(&Client::SendText, m_clients[0], message));

				//m_clients[0]->SendText(message);
		}

		// Join threads - wait for them to finish
		for (auto it = clientThreads.begin(); it != clientThreads.end(); it++)
		{
			if ((*it).joinable())
			{
				(*it).join();
			}
		}

		// Check for disconnection of clients

		for (auto it = disconnectedClients.begin(); it != disconnectedClients.end(); it++)
		{
			m_clients[*it]->CloseSocket();
			delete m_clients[*it];
			m_clients[*it] = nullptr;

			m_clients.erase(*it);
		}

		// TODO: Check for disconnection of server
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

		//m_clients[m_clientIDCount]->ListenForClient(m_listenSocket);

		//m_clients[m_clientIDCount]->

		m_clients[m_clientIDCount]->UpdateInfo();

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

			// create vector for each other client
			std::vector<std::thread> otherClients;

			// sends it to other clients
			for (auto it = m_clients.begin(); it != m_clients.end(); it++)
			{
				if (it->second->ClientConnected())
				{
					if (it->first != clientID) // except for itself
					{
						//it->second->SendText(full);

						otherClients.push_back(std::thread(&Client::SendText, it->second, full));
					}
				}
			}

			// waits for threads to finish
			for (auto it = otherClients.begin(); it != otherClients.end(); it++)
			{
				if ((*it).joinable())
				{
					(*it).join();
				}
			}

			HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hconsole, m_clients[clientID]->GetColor()); // sets color

			std::cout << m_clients[clientID]->GetName();

			SetConsoleTextAttribute(hconsole, 15); // sets it back to white
			std::cout << ": " << message << std::endl;
		}
	}
}
