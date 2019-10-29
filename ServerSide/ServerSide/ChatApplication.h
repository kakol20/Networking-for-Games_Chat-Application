#ifndef CHATAPPLICATION_H
#define CHATAPPLICATION_H

#include <iostream>
#include <Windows.h>
#include <thread>
#include <map>
#include <vector>

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

	void ReceiveText();

	void Shutdown();

private:

	std::vector<Client*> clients;

	std::vector<String*> chatHistory;

	bool m_exit = false;

	//Connection m_host;

	TCPsocket m_listenSocket;
	
	IPaddress m_IP;
};
#endif // !CHAT_APPLICATION_H
