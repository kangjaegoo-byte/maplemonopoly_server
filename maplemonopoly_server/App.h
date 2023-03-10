#pragma once
class Session;
class UserDTO;
class RoomDTO;
class WatingRoomDTO;
/**************************************************
	App : 어플리케이션 클래스
**************************************************/
class App
{
private:
	static App*					  m_instance;
	WSADATA						  m_wsaData;
	SOCKADDR_IN					  m_socketAddr;
	SOCKET						  m_listenSocket;
	fd_set						  m_fdSocketInfors;
	fd_set						  m_fdReads;
	std::list<Session*>			  m_sessions;
	std::list<RoomDTO*>			  m_rooms;
	std::map<int, WatingRoomDTO*> m_watingRoomSet;
public:
	App();
	~App();

	static App*			 GetInstance();
	void				 Init();
	bool				 Update();
	void				 Clean();
	void				 RecvPacket(Session* _session, char* _buffer);
	void				 SendPacket(Session* _session, char* _buffer, WORD _type, WORD _size, WORD _dataCnt);
	void				 DeleteInstance();

public:					 
	void				 Onconnect(SOCKET& _socket, SOCKADDR_IN& _addr);
	void				 Disconnect(SOCKET _socekt, std::list<Session*>::iterator _it);

	std::list<Session*>& SessionsRef() { return m_sessions; };
	std::list<RoomDTO*>& RoomsRef() { return m_rooms; };
	std::map<int, WatingRoomDTO*>& WatingRoomsRef() { return m_watingRoomSet; };

	void				 ConsoleSendLog(UserDTO* _user, WORD _type, WORD _size);
};

