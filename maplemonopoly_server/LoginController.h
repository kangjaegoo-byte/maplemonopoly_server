#pragma once
class Session;
class UserDTO;
/**************************************************
	LoginController : 로그인 컨트롤러
**************************************************/
class LoginController
{
private:
	static LoginController* m_instance;

public:
	static LoginController* GetInstance();

	void MoveClientScene(Session* _session, Scenetype _next);	/* 클라이언트 로그인 페이지 이동 */
	void ClientLogin(Session* _session, UserDTO* _user);		/* 클라이언트 로그인 처리 */
};

