#pragma once

class ICommandActor;
class AIActor;
class CActCommand;



class CActMgr
{

private:
	explicit CActMgr();
public:
	~CActMgr();

public:
	HRESULT Initialize();
	void Handleler();
	void Release();

public:
	void Set_Player(ICommandActor*  _pPlayer) { m_pPlayer = _pPlayer; }  // �÷��̾��� ������ �޾Ƽ� �������
	void Set_KeyRegist(int _Key, CActCommand* _Command);
	void Set_KeyRegist_Down(int _Key, CActCommand* _Command);

public:
	static CActMgr* Create();
protected:
	bool lateInit;

	map<int, CActCommand*> map_Command; // ������ command regist�Ұž�
	map<int, CActCommand*> map_Command_Down; // ������ command regist�Ұž�

	
	ICommandActor* m_pPlayer; // ������ �÷��̾ �Ѱ��ٰ���


};

