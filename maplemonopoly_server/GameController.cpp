#include "pch.h"
#include "GameController.h"
#include "App.h"
#include "Session.h"
#include "UserDTO.h"
#include "WatingRoomDTO.h"
#include "LoadingController.h"
#include "RoomDTO.h"
#include "GameRoomDTO.h"
GameController* GameController::m_instance = nullptr;

GameController* GameController::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new GameController();
	
	return m_instance;
}

void GameController::MoveClientGame(Session* _session)
{
	// ���ӹ濡 �ִ� ������
	std::vector<Session*> temp;
	int roomSq = 0;

	for (auto& item : App::GetInstance()->SessionsRef())
	{
		if (item->GetUser()->GetRoomSq() == _session->GetUser()->GetRoomSq())	
		{
			temp.push_back(item);
		}
	}

	// ��� -> ���� ����
	for (auto& room : App::GetInstance()->RoomsRef()) 
	{
		if (room->GetRoomSq() == _session->GetUser()->GetRoomSq())
		{
			if (room->GetRegID() != _session->GetUser()->GetUserId())
				return;

			room->SetStatus(GAME);
			roomSq = room->GetRoomSq();
			break;
		}
	}

	for (auto& s : temp)
	{
		s->GetUser()->SetLocation(Location::GAME_ROOM);
		LoadingController::GetInstance()->MoveClientLoading(s, Scenetype::GAME_SCENE);
		GameRoomDTO* gmDTO = new GameRoomDTO(roomSq, false, false);
		m_gameRooms.insert({ roomSq, gmDTO });
	}
}

void GameController::EnterGameUser(Session* _session)
{
	int roomSq = _session->GetUser()->GetRoomSq();
	
	GameRoomDTO* gameroom = m_gameRooms.at(roomSq);

	WatingRoomDTO* watingroom = App::GetInstance()->WatingRoomsRef().at(roomSq);
	
	int userCount = watingroom->GetIndex();

	gameroom->AddUserCount();

	if (userCount == gameroom->GetUserCount())
	{
		// ��� �ο��� ���� �Ϸ��� 
		gameroom->GameStart();
		// ����ο��� �����ߴٸ� �⺻���� ������ ������ �ѷ��� (���� �г���, ������, �÷��̾� ��)

		for (auto& s : App::GetInstance()->SessionsRef())
		{
			if (s->GetUser()->GetRoomSq() == roomSq)
			{
				std::vector<UserDTO> temp;
				for (int i = 0; i < watingroom->GetIndex(); i++) 
					temp.push_back(*watingroom->GetUser(i));

				App::GetInstance()->SendPacket(s, (char*)temp.data(), CLIENT_GAME_START_RESPONSE, PACKET_HEADER_SIZE + (watingroom->GetIndex() * sizeof(UserDTO)), watingroom->GetIndex());
			}
		}
	}
	// ��� �ο��� ���� �������� �ƹ��͵� ����
}
