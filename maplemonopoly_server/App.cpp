#include "pch.h"
#include "App.h"
#include "Session.h"
#include "LoadingController.h"
#include "LoginController.h"
#include "LobbyController.h"
#include "UserDTO.h"
#include "RoomDTO.h"
#include "WatingRoomDTO.h"
App* App::m_instance = nullptr;

App::App()
{

}

App::~App()
{

}

App* App::GetInstance()
{
    if (m_instance == nullptr)
        m_instance = new App();

    return m_instance;
}

void App::Init()
{
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0)
        printf("Failed WSAStartup() \n");

    m_listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (m_listenSocket == INVALID_SOCKET)
        printf("Failed socket() \n");

    ::memset(&m_socketAddr, 0, sizeof(m_socketAddr));
    m_socketAddr.sin_family = AF_INET;
    inet_pton(AF_INET, IPADDRESS, &m_socketAddr.sin_addr);
    m_socketAddr.sin_port = htons(PORT);

    if (bind(m_listenSocket, (SOCKADDR*)&m_socketAddr, sizeof(m_socketAddr)) == SOCKET_ERROR)
        printf("Binding Error \n");

    if (listen(m_listenSocket, 5) == SOCKET_ERROR)
        printf("Listen Error \n");

    FD_ZERO(&m_fdSocketInfors);
    FD_SET(m_listenSocket, &m_fdSocketInfors);

}

bool App::Update()
{
    SOCKET socketClient;
    SOCKET socketTemp;

    m_fdReads = m_fdSocketInfors;

    int iRet = select(0, &m_fdReads, 0, 0, 0);

    if (iRet == SOCKET_ERROR)
        return false;

    for (int i = 0; i < m_fdReads.fd_count; i++)
    {
        socketTemp = m_fdReads.fd_array[i];

        if (socketTemp == m_listenSocket)
        {
            SOCKADDR_IN tempSockAddr;
            int addrSize = sizeof(tempSockAddr);
            SOCKET socketClient = accept(m_listenSocket, (SOCKADDR*)&tempSockAddr, &addrSize);
            FD_SET(socketClient, &m_fdSocketInfors);
            Onconnect(socketClient, tempSockAddr);
        }
        else
        {
            std::list<Session*>::iterator target;
            for (std::list<Session*>::iterator it = m_sessions.begin(); it != m_sessions.end(); ++it)
                if (**it == socketTemp)
                    target = it;

            int recvRet = (*target)->Recv();

            if (recvRet <= 0)
                Disconnect(socketTemp, target);
        }

    }

    return true;
}

void App::Clean()
{
	for (auto& session : m_sessions)
		delete session;

    for (auto& room : m_rooms)
        delete room;

	closesocket(m_listenSocket);
	WSACleanup();
}

void App::RecvPacket(Session* _session, char* _buffer)
{
    WORD* size = (WORD*)_buffer;
    int dataSize = *size - PACKET_HEADER_SIZE;
    size++;

    WORD* cnt = (WORD*)size;
    int dataCnt = *cnt;
    cnt++;

    WORD* type = (WORD*)cnt;
    int controllerType = *type;
    type++;

    char* dataPtr = (char*)type;


    switch (controllerType)
    {
    case CLIENT_LOADING_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_LOADING_REQUEST\n",dataSize);
        LoadingController::GetInstance()->MoveClientLoading(_session, static_cast<Scenetype>(* dataPtr));
        break;

    case CLIENT_NEXTSCENE_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_NEXTSCENE_REQUEST\n", dataSize);
        LoginController::GetInstance()->MoveClientScene(_session, static_cast<Scenetype>(*dataPtr));
        break;

    case CLIENT_LOGIN_PROCESS_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_LOGIN_PROCESS_REQUEST\n", dataSize);
        LoginController::GetInstance()->ClientLogin(_session, reinterpret_cast<UserDTO*>(dataPtr));
        break;

    case CLIENT_LOBBY_CHAT_MSG_SEND_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_LOBBY_CHAT_MSG_SEND_REQUEST\n", dataSize);
        LobbyController::GetInstance()->ChatMsgSend(_session, reinterpret_cast<WCHAR*>(dataPtr), dataSize);
        break;

    case CLIENT_LOBBY_DATA_ASYNC_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_LOBBY_DATA_ASYNC_REQUEST\n", dataSize);
        LobbyController::GetInstance()->LobbyDataAsync(_session);
        break;

    case CLIENT_LOBBY_CREATE_ROOM_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_LOBBY_CREATE_ROOM_REQUEST\n", dataSize);
        LobbyController::GetInstance()->CreateRoom(_session, reinterpret_cast<WCHAR*>(dataPtr), dataSize);
        break;

    case CLIENT_WATING_ROOM_ASYNCDATA:
        printf("Recv: packet size: %d, type: CLIENT_WATING_ROOM_ASYNCDATA\n", dataSize);
        LobbyController::GetInstance()->WatingRoomDataAsync(_session, static_cast<int>(*dataPtr));
        break;
    case CLIENT_ETNER_WATING_ROOM_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_ETNER_WATING_ROOM_REQUEST\n", dataSize);
        LobbyController::GetInstance()->WatingRoomEnterUser(_session, reinterpret_cast<RoomDTO*>(dataPtr));
        break;

    case CLIENT_WROOM_EXIT_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_WROOM_EXIT_REQUEST\n", dataSize);
        LobbyController::GetInstance()->WatingRoomExitUser(_session, static_cast<int>(*dataPtr));
        break;

    case CLIENT_WROOM_CHAT_MSG_SEND_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_WROOM_CHAT_MSG_SEND_REQUEST\n", dataSize);
        LobbyController::GetInstance()->ChatMsgWroomSend(_session, reinterpret_cast<WCHAR*>(dataPtr), dataSize);
        break;

    case CLIENT_CPICK_REQUEST:
        printf("Recv: packet size: %d, type: CLIENT_CPICK_REQUEST\n", dataSize);
        LobbyController::GetInstance()->CPick(_session, static_cast<int>(*dataPtr));
        break;
    }
}

void App::SendPacket(Session* _session, char* _buffer, WORD _type, WORD _size, WORD _dataCnt)
{
    Packet packet;
    memset(&packet, 0, sizeof(packet));
    packet._size = _size;
    packet._cnt = _dataCnt;
    packet._type = _type;
    if (_buffer != nullptr)
        memcpy(packet._data, _buffer, _size - PACKET_HEADER_SIZE);

    ConsoleSendLog(_session->GetUser(), _type, _size);
    ::send(_session->GetSocket(), (char*)&packet, _size, 0);
}

void App::DeleteInstance()
{
    delete m_instance;
}

void App::Onconnect(SOCKET& _socket, SOCKADDR_IN& _addr)
{
    Session* newSession = new Session(_socket, _addr);
    m_sessions.push_back(newSession);
}

void App::Disconnect(SOCKET _socekt, std::list<Session*>::iterator _it)
{
    Session* session = *_it;
    m_sessions.erase(_it);

    UserDTO* user = session->GetUser();

    bool owner = false;
    std::vector<Session*> roomMember;

    if (user)
    {
        int roomSq = user->GetRoomSq();
        if (roomSq != 0)
        {
     
            // 접속을 종료한 유저가 방에 있을떄
            for (auto it = App::GetInstance()->RoomsRef().begin(); it != App::GetInstance()->RoomsRef().end(); ++it)
            {
                // 접속을 종료한 유저가 방장일 경우
                if (roomSq == (*it)->GetRoomSq() && user->GetUserId() == (* it)->GetRegID())
                {
                    
                    delete (* it);
                    App::GetInstance()->RoomsRef().erase(it);

                    // TODO 방장이 방을 나갔으면 방에 인원 전부 로비로 강제 이동 
                    owner = true;

                    delete App::GetInstance()->WatingRoomsRef().at(roomSq);
                    App::GetInstance()->WatingRoomsRef().erase(roomSq);
                    break;
                }

                // 접속을 종료한 유저가 방장이 아니고 일반 사용자일  경우
                else if (roomSq == (*it)->GetRoomSq())
                {
                    // 유저 순서 조정 
                    (*it)->SetJoinCount((*it)->GetjoinCount() - 1);
                    App::GetInstance()->WatingRoomsRef().at(roomSq)->UserOrdering(user);
                }
            }
        }
    }

    if (owner)
    {
        std::vector<Session*> roomMember;
        for (auto& s : App::GetInstance()->SessionsRef())
        {
            if (user->GetRoomSq() == s->GetUser()->GetRoomSq())
            {
                s->GetUser()->SetRoomSq(0);
                s->GetUser()->SetLocation(Location::LOBBY_ROOM);
                roomMember.push_back(s);
            }
        }
        for (auto& session : roomMember)
            App::GetInstance()->SendPacket(session, nullptr, CLIENT_WROOM_BOOM_RESPONSE, PACKET_HEADER_SIZE, 0);
    }


    delete session;
    FD_CLR(_socekt, &m_fdSocketInfors);
}

void App::ConsoleSendLog(UserDTO* _user, WORD _type, WORD _size)
{
    switch (_type)
    {
    case CLIENT_LOADING_RESPONSE:
        wprintf(L"Send: target:%ws packet size: %d, type: CLIENT_LOADING_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_NEXTSCENE_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_NEXTSCENE_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_LOBBY_CHAT_MSG_SEND_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_LOBBY_CHAT_MSG_SEND_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_LOBBY_USERLIST_DATA_ASYNC_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_LOBBY_USERLIST_DATA_ASYNC_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_LOBBY_ROOMLIST_DATA_ASYNC_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_LOBBY_ROOMLIST_DATA_ASYNC_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_LOBBY_CREATE_ROOM_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_LOBBY_CREATE_ROOM_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_WATING_ROOM_USERLIST_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_WATING_ROOM_USERLIST_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_WATING_ROOM_TITLE_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_WATING_ROOM_TITLE_RESPONSE\n", _user->GetUsername(), _size);
        break;

    case CLIENT_WROOM_BOOM_RESPONSE:
        wprintf(L"Send: target:%s packet size: %d, type: CLIENT_WROOM_BOOM_RESPONSE\n", _user->GetUsername(), _size);
        break;
    }
}
