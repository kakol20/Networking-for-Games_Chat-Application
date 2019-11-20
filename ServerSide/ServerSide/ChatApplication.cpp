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

	std::cout << "Waiting for first client to connect" << std::endl;

	// waits for one client to connect
	while (m_clientIDCount <= 0);

	// --------- MAIN ---------

	// Check for server exit

	std::thread thread2 = std::thread(&ChatApplication::CheckForServerExit, this);
	thread2.detach();

	while (!m_exit)
	{
		// Sending and receiving

		for (auto it = m_clients.begin(); it != m_clients.end(); it++)
		{
			if (!it->second->InThread() && it->second->ClientConnected()) // check if the client's update chat is already in a thread and it's connected
			{
				std::thread thread3 = std::thread(&ChatApplication::UpdateChat, this, it->first);
				thread3.detach();

				it->second->SetInThread(true);
			}
		}

		// Looks for a disconnected client and removes it from map container

		for (auto it = m_clients.begin(); it != m_clients.end(); it++)
		{
			if (it->second->IsDisconnecting())
			{
				m_clients[it->first]->CloseSocket();
				delete m_clients[it->first];
				m_clients[it->first] = nullptr;
			}
		}
	}

	// deletes all clients in map container
	for (auto it = m_clients.begin(); it != m_clients.end(); it++)
	{
		m_clients[it->first]->CloseSocket();
		delete m_clients[it->first];
		m_clients[it->first] = nullptr;
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

		m_clients[m_clientIDCount]->UpdateInfo();

		m_clientIDCount++;

		m_clients[m_clientIDCount] = new Client();
	}
}

void ChatApplication::CheckForServerExit()
{
	String input;

	while (input != "exit")
	{
		std::cin >> input;
	}

	m_exit = true;
}

void ChatApplication::Shutdown()
{
	SDLNet_Quit();
	SDL_Quit();
}

void ChatApplication::UpdateChat(int clientID)
{
	while (!m_clients[clientID]->IsDisconnecting())
	{
		String message;

		// receives text
		m_clients[clientID]->ReceiveText(message);

		// prints it for server
		HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hconsole, m_clients[clientID]->GetColor()); // sets color

		std::cout << m_clients[clientID]->GetName();

		SetConsoleTextAttribute(hconsole, 15); // sets it back to white
		std::cout << ": " << message << std::endl;

		// send text to other client

		if (m_clients.size() - 1 == 1) // checks if only 1 client is connected
		{
			String serverMsg = "Server$64$No other client connected"; // Name$Color$Message

			auto it = m_clients.begin();

			it->second->SendText(serverMsg);
		}
		else // else send it message to other clients
		{
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
				}
				else
				{
					break;
				}
			}
		}
	}
}