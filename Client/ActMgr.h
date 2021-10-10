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
	void Set_Player(ICommandActor*  _pPlayer) { m_pPlayer = _pPlayer; }  // 플레이어의 정보를 받아서 담아주자
	void Set_KeyRegist(int _Key, CActCommand* _Command);
	void Set_KeyRegist_Down(int _Key, CActCommand* _Command);

public:
	static CActMgr* Create();
protected:
	bool lateInit;

	map<int, CActCommand*> map_Command; // 여따가 command regist할거야
	map<int, CActCommand*> map_Command_Down; // 여따가 command regist할거야

	
	ICommandActor* m_pPlayer; // 조종할 플레이어를 넘겨줄거임


};

