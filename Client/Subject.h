#pragma once

class CObserver;
class CSubject
{
protected:
	CSubject();
	virtual ~CSubject();

public:
	void Subscribe(CObserver* pObserver); // 관찰자 등록
	void UnSubscribe(CObserver* pObserver); // 관찰자 해제
	void Notify(int iMessage, void* pData);

private:
	list<CObserver*> m_ObserverLst;
};

