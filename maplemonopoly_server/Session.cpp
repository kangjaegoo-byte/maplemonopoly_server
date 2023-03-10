#include "pch.h"
#include "Session.h"
#include "App.h"
Session::Session(SOCKET _socket, const SOCKADDR_IN& _sockAddr) : m_recvedSize(0), m_sockAddr(_sockAddr)
{
	memset(m_recvBuffer, 0, sizeof(m_recvBuffer));
	m_socket = _socket;
}

Session::~Session()
{
	closesocket(m_socket);
	if (m_userDTO)
		delete m_userDTO;
}

int Session::Recv(void)
{
	int recvSize = recv(m_socket, m_recvBuffer + m_recvedSize, sizeof(m_recvBuffer) - m_recvedSize, 0);

	if (recvSize >= 0)
	{
		m_recvedSize += recvSize;

		while (m_recvedSize >= 2)
		{
			WORD* header = (WORD*)m_recvBuffer;
			int packetSize = *header;

			if (m_recvedSize >= packetSize)
			{
				m_recvedSize -= packetSize;

				App::GetInstance()->RecvPacket(this, m_recvBuffer);

				if (m_recvedSize > 0)
					memcpy(m_recvBuffer, m_recvBuffer + packetSize, m_recvedSize);
			}
			else
				break;
		}
	}

	return recvSize;
}

SOCKET& Session::GetSocket()
{
	return m_socket;
}

void Session::SetUser(UserDTO* _user)
{
	m_userDTO = _user;
}

UserDTO* Session::GetUser()
{
	return m_userDTO;
}