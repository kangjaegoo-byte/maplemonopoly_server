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
	std::vector<Session*> m_sesions; //0:1p...
	WatingRoomDTO* m_room;
public:

	void AddUserCount() { m_userCount++; };
	int GetUserCount() { return m_userCount; }
	
	void GameStart() { m_start = true; }
	void AddSession(Session* _session) { m_sesions.push_back(_session); }
	void SetWatingRoomData(WatingRoomDTO* _wroom) { m_room = _wroom; }
		
	GameRoomDTO(int _roomSq, bool _end, bool _start);
	~GameRoomDTO();
};

