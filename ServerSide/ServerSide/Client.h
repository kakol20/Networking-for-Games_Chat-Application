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

	void ReceiveText(String& message);
	bool TextReceived() const;

	void SendText(const String& message);

	void UpdateInfo();

	bool IsDisconnecting() const;

	bool ClientConnected() const;

	const String GetName() const;
	const int GetColor() const;

	void CloseSocket();

private:

	String m_name;
	int m_colorID;

	bool m_Received;

	bool m_disconnecting;

	bool m_isConnected;

	TCPsocket m_socket;
};

#endif // !CLIENT_H