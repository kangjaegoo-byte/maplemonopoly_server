#pragma once
class Session;
class GameRoomDTO;
/**************************************************
	GameController : ���� ��Ʈ�ѷ�
**************************************************/
class GameController
{
private:
	static GameController* m_instance;
	std::map<int, GameRoomDTO*> m_gameRooms;


public:
	static GameController* GetInstance();
	void   MoveClientGame(Session* _session); /* Ŭ���̾�Ʈ ������������ �̵� */
	void   EnterGameUser(Session* _session); /* Ŭ���̾�Ʈ ���� �ε��Ϸ� */
	void   ExitGameUser(Session* _session); /* Ŭ���̾�Ʈ ���� ���� */
};

