#pragma once
/**************************************************
	RoomDTO : ���� �� Ŭ����
**************************************************/
class RoomDTO
{
private:
	int			 m_sq;								// ���ȣ
	WCHAR		 m_title[20] = L"";					// ����
	WCHAR		 m_regBy[15] = L"";					// ����	
	WORD		 m_regById = 0;						// ���� USER ID
	int			 m_joinCount = 0;					// �ο�
	Roomstatus   m_status = HOLD;					// ����

public:
	RoomDTO(int _sq, WCHAR* _title, WCHAR* _regBy, int _joinCount) : m_sq(_sq), m_joinCount(_joinCount) { wcscpy_s(m_title, _title); wcscpy_s(m_regBy, _regBy); };
	RoomDTO() {};
	RoomDTO(RoomDTO* _room);
	~RoomDTO() {};

	WCHAR*		 GetTitle() { return m_title; }
	WCHAR*		 GetRegBy() { return m_regBy; }
	int			 GetjoinCount() { return m_joinCount; }
	int			 GetSq() { return m_sq; }
	Roomstatus   GetStatus() { return m_status; }
	void		 SetStatus(Roomstatus _status) { m_status = _status; }

	void		 SetTitle(WCHAR* _title) { wcscpy_s(m_title, _title); }
	void		 SetTitle(WCHAR* _title, int _dataSize) { memcpy(m_title, _title, _dataSize); }
	void		 SetRegBy(WCHAR* _regBy) { wcscpy_s(m_regBy, _regBy); }
	void		 SetJoinCount(int _count) { m_joinCount = _count; }
	int			 GetRoomSq() { return m_sq; }
	void		 CreateRoomSq();
	int			 GetRegID() { return m_regById; }
	void		 SetRegID(int _id) { m_regById = _id; }
};

