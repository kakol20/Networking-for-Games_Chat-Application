#include "Client.h"

Client::Client()
{
	m_Received = false;
	m_colorID = 0;
	m_name = "N/A";
	m_disconnecting = false;
	m_isConnected = false;
}

Client::~Client()
{
}

const TCPsocket Client::GetSocket() const
{
	return m_socket;
}

void Client::ListenForClient(const TCPsocket& listenSocket)
{
	m_socket = SDLNet_TCP_Accept(listenSocket);
}

void Client::ReceiveText(String& message)
{
	m_Received = false;

	char response[2048] = { '\0' };

	if (SDLNet_TCP_Recv(m_socket, response, 2048) <= 0)
	{
		std::cout << "Error recieving message" << std::endl;
		system("pause");
	}
	else
	{
		message = response;
		m_Received = true;
	}
}

bool Client::TextReceived() const
{
	return m_Received;
}

void Client::SendText(const String& message)
{

	String full = message;
	size_t length = message.Length() + 1;

	String test = message;
	if (test == "exit")
	{
		

		m_disconnecting = true;
	}

	if (SDLNet_TCP_Send(m_socket, full.GetString(), length) < (int)length)
	{
		std::cout << "Error sending message to: ";
		std::cout << m_name;
		std::cout << std::endl;
		system("pause");
	}
}

void Client::UpdateInfo()
{
	// Ask for name

	char name[2048] = { '\0' };

	if (SDLNet_TCP_Recv(m_socket, name, 2048) <= 0)
	{
		std::cout << "Error sending message" << std::endl;
	}

	m_name = name;

	// ask color they want to choose
	char color[2048] = { '\0' };

	if (SDLNet_TCP_Recv(m_socket, color, 2048) <= 0)
	{
		std::cout << "Error sending message" << std::endl;
	}

	std::stringstream strValue;
	strValue << color;
	strValue >> m_colorID;

	std::cout << m_name << " is connected!" << std::endl;

	m_isConnected = true;
}

bool Client::IsDisconnecting() const
{
	return m_disconnecting;
}

bool Client::ClientConnected() const
{
	return m_isConnected;
}

const String Client::GetName() const
{
	return m_name;
}

const int Client::GetColor() const
{
	return m_colorID;
}

void Client::CloseSocket()
{
	SDLNet_TCP_Close(m_socket);
}