#include "Client.h"

Client::Client()
{
	m_Received = false;
	m_colorID = 0;
	m_name = "N/A";
	m_disconnecting = false;
	m_isConnected = false;

	m_socket = nullptr;

	m_isInThread = false;
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
	//m_isConnected = true;
}

void Client::ReceiveText(String& message)
{
	m_Received = false;

	char response[2048] = { '\0' };

	//std::cin.ignore();

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
	int length = message.Length() + 1;

	String test = message;
	if (test == "exit")
	{
		m_disconnecting = true;
	}

	if (SDLNet_TCP_Send(m_socket, full.GetString(), length) < length)
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
	ReceiveText(m_name);

	// Ask color they want to choose
	String color;
	ReceiveText(color);

	std::stringstream strValue;
	strValue << color.GetString();
	strValue >> m_colorID;

	HANDLE hconsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hconsole, m_colorID);

	std::cout << m_name;

	SetConsoleTextAttribute(hconsole, 15);

	std::cout << " is connected!" << std::endl;

	//std::cout << m_name << " is connected!" << std::endl;

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

bool Client::InThread() const
{
	return m_isInThread;
}

void Client::SetInThread(const bool flag)
{
	m_isInThread = flag;
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