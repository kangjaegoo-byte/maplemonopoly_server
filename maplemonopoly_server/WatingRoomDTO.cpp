#include "pch.h"
#include "WatingRoomDTO.h"
#include "UserDTO.h"
void WatingRoomDTO::UserOrdering(UserDTO* _user)
{
	int i;
	for (i = 0; i < m_index; i++) 
	{
		if (m_players[i]->GetUserId() == _user->GetUserId())
		{
			m_players[i] = nullptr;
			break;
		}
	}

	while (i != P4)
	{
		std::swap(m_players[i], m_players[i + 1]);
		i++;
	}

	m_index--;
}
