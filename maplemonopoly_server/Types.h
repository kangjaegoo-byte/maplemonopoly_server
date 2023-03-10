#pragma once
/*********************************************************
				데이터 타입을 설정합니다.
**********************************************************/
struct Packet
{
	WORD _size;			// Pakcet Size
	WORD _cnt;			// data cnt 
	WORD _type;			// data type
	char _data[1000];	// data
};

enum Scenetype : unsigned int
{
	LOADING_SCENE = 0,
	LOGIN_SCENE = 1,
	LOBBY_SCENE = 2,
	SCENE_COUNT = 3
};

/*********************************************************
				클라이언트 요청 타입
**********************************************************/
enum ControllerType : int 
{
	CLIENT_LOADING_REQUEST = 0,
	CLIENT_NEXTSCENE_REQUEST = 1,
	CLIENT_LOGIN_PROCESS_REQUEST = 2,
	CLIENT_LOBBY_CHAT_MSG_SEND_REQUEST = 3,
	CLIENT_LOBBY_DATA_ASYNC_REQUEST = 4,
	CLIENT_LOBBY_CREATE_ROOM_REQUEST = 5,
	CLIENT_WATING_ROOM_ASYNCDATA = 6,
	CLIENT_ETNER_WATING_ROOM_REQUEST = 7,
	CLIENT_WROOM_EXIT_REQUEST = 8,
	CLIENT_WROOM_CHAT_MSG_SEND_REQUEST = 9,
	CLIENT_CPICK_REQUEST = 10,
};

/*********************************************************
				서버 응답 타입
**********************************************************/
enum ServerResponse : int 
{
	CLIENT_LOADING_RESPONSE = 0,
	CLIENT_NEXTSCENE_RESPONSE = 1,
	CLIENT_LOBBY_CHAT_MSG_SEND_RESPONSE = 2,
	CLIENT_LOBBY_USERLIST_DATA_ASYNC_RESPONSE = 3,
	CLIENT_LOBBY_ROOMLIST_DATA_ASYNC_RESPONSE = 4,
	CLIENT_LOBBY_CREATE_ROOM_RESPONSE = 5,
	CLIENT_WATING_ROOM_USERLIST_RESPONSE = 6,
	CLIENT_WATING_ROOM_TITLE_RESPONSE = 7,
	CLIENT_WROOM_BOOM_RESPONSE = 8,
	CLIENT_WROOM_CHAT_MSG_SEND_RESPONSE = 9
};

enum Location : unsigned int
{
	LOBBY_ROOM = 0,
	WATING_ROOM = 1,
	GAME_ROOM = 2
};

enum Roomstatus : int
{
	HOLD = 0
};

enum CPick : int
{
	HONY_MURSHROOM = 0,
	ORANGE_MURSHROOM = 1,
	PIG = 2,
	NOUSER = 3
};