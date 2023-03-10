#pragma once
class Session;
class RoomDTO;
/**************************************************
	LobbyController : 로비 컨트롤러
**************************************************/
class LobbyController
{
private:
	static LobbyController* m_instance;

public:
	static LobbyController* GetInstance();
	void ChatMsgSend(Session* _session, WCHAR* _msg, int _strSize);	/* 로비 채팅 */
	void LobbyDataAsync(Session* _session);	/* 로비화면 데이터 동기화 */
	void WatingRoomDataAsync(Session* _session, int _roomSq);	/* 대기실 데이터 동기화 */
	void CreateRoom(Session* _session, WCHAR* _title, int _dataSize);	/* 대기실 만들기 */
	void WatingRoomEnterUser(Session* _session, RoomDTO* _room);	/* 대기실 입장하기 */
	void WatingRoomExitUser(Session* _session, int _roomSq);	/* 대기실 나가기 */
	void ChatMsgWroomSend(Session* _session, WCHAR* _str, int _dataSize); /* 대기실 채팅 */
};

