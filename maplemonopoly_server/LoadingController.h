#pragma once
class Session;
/**************************************************
	LoadingController : �ε� ��Ʈ�ѷ�
**************************************************/
class LoadingController
{
private:
	static LoadingController* m_instance;

public:
	static LoadingController* GetInstance();
	void MoveClientLoading(Session* _session, Scenetype _next);	/* Ŭ���̾�Ʈ �ε��������� �̵� */
};

