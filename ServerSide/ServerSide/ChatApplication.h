#ifndef CHATAPPLICATION_H
#define CHATAPPLICATION_H

#include <map>
#include <thread>
#include <vector>
#include <sstream>
#include <iostream>
#include <Windows.h>

#include <SDL.h>
#include <SDL_net.h>

#include "Client.h"
#include "String.h"
//#include "Connection.h"

class ChatApplication
{
public:

	ChatApplication();
	~ChatApplication();

	bool Run();

private:
	bool Init();
	bool ResolveHost(int port);

	void WaitForClients();

	void CheckForServerExit();

	void UpdateChat(int clientID);

	void Shutdown();

private:

	//std::vector<Client*> clients;

	std::map<int, Client*> m_clients;

	bool m_exit = false;

	int m_clientIDCount = 0;

	//Connection m_host

	TCPsocket m_listenSocket;
	
	IPaddress m_IP;
};
#endif // !CHAT_APPLICATION_H
