#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>

#include "String.h"

#include <SDL.h>
#include <SDL_net.h>

class Connection
{
public:
	Connection();
	~Connection();

	bool Init();
	bool ResolveHost(int port);

	void OpenSocket();

	void WaitForClient();

	bool SendText(const String& message);
	bool RecieveText(String& message);

	void CloseSocket();
	void Shutdown();

private:

	IPaddress m_IP;

	TCPsocket m_clientSocket = nullptr;
	TCPsocket m_listenSocket = nullptr;
};

#endif // !CONNECTION_H
