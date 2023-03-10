#include "pch.h"
#include "LobbyController.h"
#include "App.h"
#include "UserDTO.h"
#include "Session.h"
#include "RoomDTO.h"
#include "WatingRoomDTO.h"
LobbyController* LobbyController::m_instance = nullptr;

LobbyController* LobbyController::GetInstance()
{
	if (m_instance == nullptr)
		m_instance = new LobbyController();
	return m_instance;
}

void LobbyController::ChatMsgSend(Session* _session, WCHAR* _msg, int strSize)
{
	std::list<Session*> m_sessions = App::GetInstance()->SessionsRef();
	std::vector<Session*> temp;
	UserDTO* sendUser = _session->GetUser();
	Location location = sendUser->GetLocation();

	for (auto& session : m_sessions) 
	{
		if (session->GetUser() && session->GetUser()->GetLocation() == location)
			temp.push_back(session);
	}

	WCHAR text[15] = L"";
	WCHAR str[30] = L"";

	memcpy(text, _msg, strSize);
	swprintf_s(str, _countof(str), L"%s:%s", sendUser->GetUsername(), text);
	int str_size = wcslen(str) * sizeof(WCHAR);

	for (auto& to : temp)
		App::GetInstance()->SendPacket(to, (char*)str, CLIENT_LOBBY_CHAT_MSG_SEND_RESPONSE, str_size + PACKET_HEADER_SIZE, 1);
}

void LobbyController::LobbyDataAsync(Session* _session)
{
	std::list<Session*> sessions = App::GetInstance()->SessionsRef();
	std::list<RoomDTO*> rooms = App::GetInstance()->RoomsRef();
	
	{
		std::vector<UserDTO> userv;
		for (auto session : sessions)
		{
			if (session->GetUser())
			{
				UserDTO userDTO = *(session->GetUser());
				userv.push_back(userDTO);
			}
		}
		App::GetInstance()->SendPacket(_session, userv.size() > 0 ? (char*)&userv[0] : nullptr, CLIENT_LOBBY_USERLIST_DATA_ASYNC_RESPONSE, PACKET_HEADER_SIZE + (userv.size() * sizeof(UserDTO)), userv.size());
	}

	{
		std::vector<RoomDTO> roomv;
		for (auto room : rooms)
		{
			roomv.push_back(( * room));
		}

		App::GetInstance()->SendPacket(_session, roomv.size() > 0 ? (char*)&roomv[0] : nullptr, CLIENT_LOBBY_ROOMLIST_DATA_ASYNC_RESPONSE, PACKET_HEADER_SIZE + (roomv.size() * sizeof(RoomDTO)), roomv.size());
	}

}

void LobbyController::WatingRoomDataAsync(Session* _session, int _roomSq)
{
	std::vector<UserDTO> temp;
	auto iter = App::GetInstance()->WatingRoomsRef().find(_roomSq);
	
	if (iter != App::GetInstance()->WatingRoomsRef().end())
	{
		WatingRoomDTO* watingRoomData = iter->second;

		if (watingRoomData)
		{
			int index = watingRoomData->GetIndex();

			for (int i = 0; i < index; i++)
			{
				UserDTO* user = watingRoomData->GetUser(i);
				UserDTO  t = *user;
				temp.push_back(t);
			}

			App::GetInstance()->SendPacket(_session, (char*)&temp[0], CLIENT_WATING_ROOM_USERLIST_RESPONSE, PACKET_HEADER_SIZE + (temp.size() * sizeof(UserDTO)), temp.size());
			App::GetInstance()->SendPacket(_session, (char*)watingRoomData->GetTilte(), CLIENT_WATING_ROOM_TITLE_RESPONSE, PACKET_HEADER_SIZE + (sizeof(WCHAR) * wcslen(watingRoomData->GetTilte())), 1);
		}
	}
}

void LobbyController::CreateRoom(Session* _session, WCHAR* _title, int _dataSize)
{
	RoomDTO* createRoom = new RoomDTO();
	WatingRoomDTO* watingRoom = new WatingRoomDTO();

	createRoom->CreateRoomSq();
	createRoom->SetTitle(_title, _dataSize);
	createRoom->SetRegBy(_session->GetUser()->GetUsername());
	createRoom->SetRegID(_session->GetUser()->GetUserId());
	createRoom->SetJoinCount(createRoom->GetjoinCount() + 1);	

	std::list<RoomDTO*>& roomList = App::GetInstance()->RoomsRef();
	roomList.push_back(createRoom);

	_session->GetUser()->SetRoomSq(createRoom->GetRoomSq());
	_session->GetUser()->SetLocation(Location::WATING_ROOM);

	watingRoom->SetTitle(_title, _dataSize);
	watingRoom->AddUser(_session->GetUser());

	int roomSq = createRoom->GetRoomSq();

	App::GetInstance()->WatingRoomsRef().insert({ roomSq, watingRoom });
	App::GetInstance()->SendPacket(_session, (char*) & roomSq, CLIENT_LOBBY_CREATE_ROOM_RESPONSE, PACKET_HEADER_SIZE + sizeof(int), 0);
}

void LobbyController::WatingRoomEnterUser(Session* _session, RoomDTO* _room)
{
	// user 정보를 세팅한다... 
	_session->GetUser()->SetRoomSq(_room->GetSq());

	// 룸정보 갱신
	for (auto& item : App::GetInstance()->RoomsRef()) 
	{
		if (item->GetRoomSq() == _room->GetSq())
		{
			item->SetJoinCount(item->GetjoinCount() + 1);
			_session->GetUser()->SetLocation(Location::WATING_ROOM);
			break;
		}
	}
	// user를 해당방에 넣는다 
	App::GetInstance()->WatingRoomsRef().at(_room->GetSq())->AddUser(_session->GetUser());
}

void LobbyController::WatingRoomExitUser(Session* _session, int _roomSq)
{
	bool owner = false;
	int roomSq = _session->GetUser()->GetRoomSq();

	for (auto it = App::GetInstance()->RoomsRef().begin(); it != App::GetInstance()->RoomsRef().end(); ++it)
	{
		RoomDTO* room = *it;
		// 여기까지 왔으면 현재 세션에서 사용자가 있는 방
		if (room->GetRoomSq() == roomSq)
		{
			// 방장인가?
			if (room->GetRegID() == _session->GetUser()->GetUserId())
			{
				owner = true;
				// 방을 폭파 방에있는 사람들 다 퇴장
				delete room;
				App::GetInstance()->RoomsRef().erase(it);

				// TODO 방장이 방을 나갔으면 방에 인원 전부 로비로 강제 이동 
				delete App::GetInstance()->WatingRoomsRef().at(roomSq);
				App::GetInstance()->WatingRoomsRef().erase(roomSq);
				break;
			}
			else
			{
				// 해당 사람만 퇴장
				App::GetInstance()->WatingRoomsRef().at(roomSq)->UserOrdering(_session->GetUser());
				room->SetJoinCount(room->GetjoinCount() - 1);
			}
		}
	}
	_session->GetUser()->SetLocation(Location::LOBBY_ROOM);
	_session->GetUser()->SetRoomSq(0);

	if (owner)
	{
		std::vector<Session*> roomMember;
		for (auto& s : App::GetInstance()->SessionsRef())
		{
			if (s->GetUser() && roomSq == s->GetUser()->GetRoomSq())
			{
				s->GetUser()->SetRoomSq(0);
				s->GetUser()->SetLocation(Location::LOBBY_ROOM);
				roomMember.push_back(s);
			}
		}

		for (auto& session : roomMember)
			App::GetInstance()->SendPacket(session, nullptr, CLIENT_WROOM_BOOM_RESPONSE, PACKET_HEADER_SIZE, 0);
	}
}

void LobbyController::ChatMsgWroomSend(Session* _session, WCHAR* _msg, int strSize)
{
	std::list<Session*> m_sessions = App::GetInstance()->SessionsRef();
	std::vector<Session*> temp;
	
	int roomSq = _session->GetUser()->GetRoomSq();

	for (auto& session : m_sessions)
	{
		if (session->GetUser() && session->GetUser()->GetRoomSq() == roomSq)
			temp.push_back(session);
	}

	WCHAR text[15] = L"";
	WCHAR str[30] = L"";

	memcpy(text, _msg, strSize);
	swprintf_s(str, _countof(str), L"%s:%s", _session->GetUser()->GetUsername(), text);
	int str_size = wcslen(str) * sizeof(WCHAR);

	for (auto& to : temp)
		App::GetInstance()->SendPacket(to, (char*)str, CLIENT_WROOM_CHAT_MSG_SEND_RESPONSE, str_size + PACKET_HEADER_SIZE, 1);
}

void LobbyController::CPick(Session* _session, int _pick)
{
	_session->GetUser()->SetUserPick(_pick);
}
	