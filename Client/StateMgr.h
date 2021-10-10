#pragma once



class CStateMgr
{

	DEFINE_SINGLETONE(CStateMgr)
public:
	ENUM_STATE;
private:
	CStateMgr();
	~CStateMgr();
public:
	int& Get_eCurState() { return m_eCurState; }
public:
	void Initialize();
	bool StateCheck(int _State);
	void Set_StateMgr(int _State);
	
private:
	int m_eCurState;
	int m_eNextState;
	bool ActState;
};

