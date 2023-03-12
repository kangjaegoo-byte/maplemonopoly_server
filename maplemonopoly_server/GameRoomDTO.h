#pragma once
/**************************************************
	GameRoomDTO : 게임 방 클래스
**************************************************/
class Session;
class WatingRoomDTO;
class GameRoomDTO
{
private:
	int  m_roomSq	 = 0;
	int  m_userCount = 0;
	bool m_end		= false;
	bool m_start	= false;

	int	 m_playerTurn = 0;
	std::vector<Session*> _sesions; //0:1p...
	WatingRoomDTO* _room;
public:

	void AddUserCount() { m_userCount++; };
	int GetUserCount() { return m_userCount; }
	void GameStart() { m_start = true; }


	GameRoomDTO(int _roomSq, bool _end, bool _start);
	~GameRoomDTO();
};

