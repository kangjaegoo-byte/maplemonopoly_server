#include "pch.h"
#include "RoomDTO.h"

RoomDTO::RoomDTO(RoomDTO* _room)
{
	wcscpy_s(m_regBy, _room->GetRegBy());
	wcscpy_s(m_title, _room->GetTitle());
	m_sq = _room->GetSq();
	m_joinCount = _room->GetjoinCount();
	m_status = _room->GetStatus();
}

void RoomDTO::CreateRoomSq()
{
	static WORD roomSq = 1;
	m_sq = roomSq++;
}
