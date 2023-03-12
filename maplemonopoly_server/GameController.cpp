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
	// 게임방에 있는 유저들
	std::vector<Session*> temp;
	int roomSq = 0;

	for (auto& item : App::GetInstance()->SessionsRef())
	{
		if (item->GetUser()->GetRoomSq() == _session->GetUser()->GetRoomSq())	
		{
			temp.push_back(item);
		}
	}

	// 대기 -> 게임 변경
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
		// 모든 인원이 접속 완료함 
		gameroom->GameStart();
		// 모든인원이 접속했다면 기본적인 방정보 세팅을 뿌려줌 (유저 닉네임, 방제목, 플레이어 픽)

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
	// 모든 인원이 접속 안했으면 아무것도 안함
}

void GameController::ExitGameUser(Session* _session)
{
	int roomSq = _session->GetUser()->GetRoomSq();
	_session->GetUser()->SetRoomSq(0);
	_session->GetUser()->SetLocation(Location::LOBBY_ROOM);
	WatingRoomDTO* wroom = App::GetInstance()->WatingRoomsRef().at(roomSq);
	wroom->UserOrdering(_session->GetUser());

	for (auto it = App::GetInstance()->RoomsRef().begin();  it != App::GetInstance()->RoomsRef().end(); ++it)
	{
		RoomDTO* room = *it;
		UserDTO* nextRegBy = nullptr;

		// 방장일떄 
		if (room->GetRoomSq() == roomSq)
		{
			if (room->GetRegID() == _session->GetUser()->GetUserId())
			{
				if (wroom->GetIndex() == 0) // 방에 아무도 없을때 방 데이터 전부 삭제
				{
					// 삭제 
					delete wroom;
					App::GetInstance()->WatingRoomsRef().erase(roomSq);
					App::GetInstance()->RoomsRef().erase(it);
					delete m_gameRooms.at(roomSq);
					delete room;
					LoadingController::GetInstance()->MoveClientLoading(_session, Scenetype::LOBBY_SCENE);
					return;
				}

				nextRegBy = wroom->GetUser(0);
				room->SetRegBy(nextRegBy->GetUsername());
				room->SetRegID(nextRegBy->GetUserId());
			}

			nextRegBy = wroom->GetUser(0);
			room->SetJoinCount(room->GetjoinCount() - 1);

			for (auto& s : App::GetInstance()->SessionsRef())
			{
				if (s->GetUser()->GetRoomSq() == roomSq)
				{
					std::vector<UserDTO> temp;
					for (int i = 0; i < wroom->GetIndex(); i++)
						temp.push_back(*wroom->GetUser(i));
					App::GetInstance()->SendPacket(s, (char*)temp.data(), CLIENT_GAME_EXIT_RESPONSE, PACKET_HEADER_SIZE + (wroom->GetIndex() * sizeof(UserDTO)), wroom->GetIndex());
				}
			}
			break;
		}
	}
	LoadingController::GetInstance()->MoveClientLoading(_session, Scenetype::LOBBY_SCENE);
}
