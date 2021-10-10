#pragma once

class CObserver;
class CSubject
{
protected:
	CSubject();
	virtual ~CSubject();

public:
	void Subscribe(CObserver* pObserver); // ������ ���
	void UnSubscribe(CObserver* pObserver); // ������ ����
	void Notify(int iMessage, void* pData);

private:
	list<CObserver*> m_ObserverLst;
};

