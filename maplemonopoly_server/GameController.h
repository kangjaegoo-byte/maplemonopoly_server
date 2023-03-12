#pragma once
class Session;
class GameRoomDTO;
/**************************************************
	GameController : 게임 컨트롤러
**************************************************/
class GameController
{
private:
	static GameController* m_instance;
	std::map<int, GameRoomDTO*> m_gameRooms;


public:
	static GameController* GetInstance();
	void   MoveClientGame(Session* _session); /* 클라이언트 게임페이지로 이동 */
	void   EnterGameUser(Session* _session); /* 클라이언트 게임 로딩완료 */
	void   ExitGameUser(Session* _session); /* 클라이언트 게임 종료 */
};

