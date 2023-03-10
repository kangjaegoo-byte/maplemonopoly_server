#pragma once
class UserDTO;
/**************************************************
	Session : 클라이언트 세션
**************************************************/
class Session
{
private:
	SOCKET		m_socket;
	char		m_recvBuffer[256];
	int			m_recvedSize;
	SOCKADDR_IN	m_sockAddr;
	UserDTO*	m_userDTO = nullptr;

public:
	Session(SOCKET _socket, const SOCKADDR_IN& _sockAddr);
	~Session();

	bool operator==(const Session& other)
	{
		return m_socket == other.m_socket;
	}
	bool operator==(const SOCKET& other)
	{
		return m_socket == other;
	}

public:
	int		 Recv(void);
	SOCKET&  GetSocket();
	void	 SetUser(UserDTO* _user);
	UserDTO* GetUser();
};
