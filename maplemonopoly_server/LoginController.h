#pragma once
class Session;
class UserDTO;
/**************************************************
	LoginController : �α��� ��Ʈ�ѷ�
**************************************************/
class LoginController
{
private:
	static LoginController* m_instance;

public:
	static LoginController* GetInstance();

	void MoveClientScene(Session* _session, Scenetype _next);	/* Ŭ���̾�Ʈ �α��� ������ �̵� */
	void ClientLogin(Session* _session, UserDTO* _user);		/* Ŭ���̾�Ʈ �α��� ó�� */
};

