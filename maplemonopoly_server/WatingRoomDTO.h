#pragma once
class UserDTO;
/**************************************************
	WatingRoomDTO : 대기방 클래스
**************************************************/
class WatingRoomDTO
{
	enum : int
	{
		P1 = 0,
		P2 = 1,
		P3 = 2,
		P4 = 3,
	};
private:
	UserDTO*		m_players[4];
	WCHAR			m_title [20] = L"";
	int				m_index = 0;
public:
	WatingRoomDTO() {};
	

	void     UserOrdering(UserDTO* _user);

	void	 SetTitle(WCHAR* _title) { wcscpy_s(m_title, _title); }
	void	 SetTitle(WCHAR* _title, int _dataSize) { memcpy(m_title, _title, _dataSize); }
	void	 AddUser(UserDTO* _user) { m_players[m_index] = _user; m_index++; }
	int		 GetIndex() { return m_index; }
	UserDTO* GetUser(int _index) { return m_players[_index]; }
	int		 FindUserById(int _userID);
	UserDTO** GetUsers() { return m_players; }
	WCHAR*	 GetTilte() { return m_title; }
	
};

