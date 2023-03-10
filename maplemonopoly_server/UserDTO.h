#pragma once
/**************************************************
	UserDTO : ����� ���� Ŭ����
**************************************************/
class UserDTO
{
private:
	WCHAR    m_username[15] = L"";
	int		 m_userId = 0;
	int		 m_sung = 0;
	WORD	 m_roomSq = -0;
	Location m_location = Location::LOBBY_ROOM;
public:
	UserDTO(WCHAR* _username, Location _location);
	UserDTO(int _userid, WCHAR* _username, Location _location);
	UserDTO() {};
	UserDTO(const UserDTO& _copy);
	~UserDTO();

	WCHAR*	 GetUsername() { return m_username; }
	int	     GetSung() { return m_sung; }
	Location GetLocation() { return m_location; }
	int      GetUserId() { return m_userId; }
	void	 SetLocation(Location _location) { m_location = _location; }
	void     SetUserID(int _id) { m_userId = _id; }
	void	 SetRoomSq(int _roomSq) { m_roomSq = _roomSq; }
	int      GetRoomSq() { return m_roomSq; }
};
