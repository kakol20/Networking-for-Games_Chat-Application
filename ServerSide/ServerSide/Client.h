#ifndef CLIENT_H
#define CLIENT_H

#include <sstream>

#include <SDL.h>
#include <SDL_net.h>

#include "String.h"

class Client
{
public:
	Client();
	~Client();

	const TCPsocket GetSocket() const;

	void ListenForClient(const TCPsocket& listenSocket);

	void UpdateInfo();

	void CloseSocket();

private:

	String m_name;
	int m_colorID;

	TCPsocket m_socket;
};

#endif // !CLIENT_H