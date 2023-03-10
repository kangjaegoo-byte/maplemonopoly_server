#pragma once
class Session;
/**************************************************
	LoadingController : 로딩 컨트롤러
**************************************************/
class LoadingController
{
private:
	static LoadingController* m_instance;

public:
	static LoadingController* GetInstance();
	void MoveClientLoading(Session* _session, Scenetype _next);	/* 클라이언트 로딩페이지로 이동 */
};

