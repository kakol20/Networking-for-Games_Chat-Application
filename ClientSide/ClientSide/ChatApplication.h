#ifndef CHATAPP_H
#define CHATAPP_H

#include <sstream>
#include <thread>
#include <iostream>
#include <Windows.h>

#include "String.h"

#include <SDL.h>
#include <SDL_net.h>

class ChatApplication
{
public:
	ChatApplication();
	~ChatApplication();

	bool Run();

private:
	bool Init();

	bool ResolveHost();

	bool OpenSocket();

	void ReceiveText();

	void SendText();

	bool SendInfo();

	void CloseSocket();
	void Shutdown();

private:

	IPaddress m_IP;
	TCPsocket m_socket = nullptr;

	String m_name = "";
	int m_color = 7;

	bool m_exit = false;
};

#endif // !CHATAPP_H
