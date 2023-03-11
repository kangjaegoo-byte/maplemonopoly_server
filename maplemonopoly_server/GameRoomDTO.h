#pragma once
/**************************************************
	GameRoomDTO : 게임 방 클래스
**************************************************/
class GameRoomDTO
{
private:
	int  m_roomSq	 = 0;
	int  m_userCount = 0;
	bool m_end		= false;
	bool m_start	= false;
	
public:

	void AddUserCount() { m_userCount++; };
	int GetUserCount() { return m_userCount; }
	void GameStart() { m_start = true; }


	GameRoomDTO(int _roomSq, bool _end, bool _start);
	~GameRoomDTO();
};

