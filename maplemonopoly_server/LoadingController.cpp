#include "pch.h"
#include "LoadingController.h"
#include "App.h"
LoadingController* LoadingController::m_instance = nullptr;

LoadingController* LoadingController::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new LoadingController();

	return m_instance;
}

void LoadingController::MoveClientLoading(Session* _session, Scenetype _next)
{
	App::GetInstance()->SendPacket(_session, (char*)&_next, CLIENT_LOADING_RESPONSE, sizeof(_next) + PACKET_HEADER_SIZE, 0);
}
