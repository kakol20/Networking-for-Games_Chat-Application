#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <Windows.h>

#include <SDL.h>
#include <SDL_net.h>

#include "String.h"

class Connection
{
public:
	Connection();
	~Connection();

	bool Init();
	bool ResolveHost(const String& address, int port);

	bool OpenSocket();

	bool RecieveText(String& message);
	bool SendText(const String& message);

	void CloseSocket();
	void Shutdown();

private:

	IPaddress m_IP;
	TCPsocket m_socket = nullptr;
};

#endif // !CONNECTION_H
