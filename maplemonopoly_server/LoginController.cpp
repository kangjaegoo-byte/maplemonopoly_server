#include "pch.h"
#include "LoginController.h"
#include "App.h"
#include "Session.h"
#include "App.h"
#include "UserDTO.h"
#include "LoadingController.h"
LoginController* LoginController::m_instance = nullptr;

LoginController* LoginController::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new LoginController();

	return m_instance;
}

void LoginController::MoveClientScene(Session* _session, Scenetype _next)
{
	App::GetInstance()->SendPacket(_session, (char*)&_next, CLIENT_NEXTSCENE_RESPONSE, sizeof(_next) + PACKET_HEADER_SIZE, 0);
}

void LoginController::ClientLogin(Session* _session, UserDTO* _user)
{
	static int userId = 1;

	UserDTO* newUser = new UserDTO(*_user);
	newUser->SetUserID(userId++);
	std::list<Session*> sessions = App::GetInstance()->SessionsRef();
	_session->SetUser(newUser);
	
	Scenetype _next = Scenetype::LOBBY_SCENE;
	LoadingController::GetInstance()->MoveClientLoading(_session, _next);
}
