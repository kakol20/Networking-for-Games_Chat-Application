#include "Client.h"

Client::Client()
{
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

void Client::UpdateInfo()
{
	// Ask for name

	char name[2048] = { '\0' };

	if (SDLNet_TCP_Recv(m_socket, name, 2048) <= 0)
	{
		std::cout << "Error sending message" << std::endl;
	}

	m_name = name;

	char color[2048] = { '\0' };

	if (SDLNet_TCP_Recv(m_socket, color, 2048) <= 0)
	{
		std::cout << "Error sending message" << std::endl;
	}

	std::stringstream strValue;
	strValue << color;

	strValue >> m_colorID;

}

void Client::CloseSocket()
{
	SDLNet_TCP_Close(m_socket);
}

//String Client::ReceiveText()
//{
//	char message[2048] = { '\0' };
//
//	if (SDLNet_TCP_Recv(m_socket, message, 2048) <= 0)
//	{
//		std::cout << "Error sending message" << std::endl;
//	}
//
//	String result = message;
//
//	return message;
//}
