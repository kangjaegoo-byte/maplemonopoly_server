#pragma once
class Session;
class RoomDTO;
/**************************************************
	LobbyController : �κ� ��Ʈ�ѷ�
**************************************************/
class LobbyController
{
private:
	static LobbyController* m_instance;

public:
	static LobbyController* GetInstance();
	void ChatMsgSend(Session* _session, WCHAR* _msg, int _strSize);	/* �κ� ä�� */
	void LobbyDataAsync(Session* _session);	/* �κ�ȭ�� ������ ����ȭ */
	void WatingRoomDataAsync(Session* _session, int _roomSq);	/* ���� ������ ����ȭ */
	void CreateRoom(Session* _session, WCHAR* _title, int _dataSize);	/* ���� ����� */
	void WatingRoomEnterUser(Session* _session, RoomDTO* _room);	/* ���� �����ϱ� */
	void WatingRoomExitUser(Session* _session, int _roomSq);	/* ���� ������ */
	void ChatMsgWroomSend(Session* _session, WCHAR* _str, int _dataSize); /* ���� ä�� */
};

