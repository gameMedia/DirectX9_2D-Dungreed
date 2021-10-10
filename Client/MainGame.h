#pragma once
class CActMgr;

class CMainGame
{
private:
	explicit CMainGame();

public:
	~CMainGame();

public:
	void Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();

public:
	static CMainGame* Create();
private:
	//작업
	void InitKey();

public:


private:
	CSceneMgr* m_pSceneMgr;
private:
	
	CTimeMgr* m_pTimeMgr;
	CActMgr* m_pActMgr;
	HDC m_hDC;

	// fps 혹시 렉 ㅈㄴ 걸리면 fps확인 해야함
	float m_fTimeCount;
	int m_FPS;
	TCHAR m_szFPS[32];
	D3DXMATRIX FPSTrans;

	bool lateInit;
};

