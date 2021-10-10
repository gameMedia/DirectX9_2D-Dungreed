#pragma once

class CScene;
class CSceneMgr
{
	DEFINE_SINGLETONE(CSceneMgr)

public:
	enum SCENE_TYPE	{LOGO, STAGE,MAINMENU,DUNGEON, END};
private:
	explicit CSceneMgr();
	~CSceneMgr();

public:
	CScene* Get_CurScene() const { return m_pScene; }

public:
	HRESULT	SceneChange(SCENE_TYPE eType);
	void Update();
	void LateUpdate();
	void Reneder();

private:
	void Release();

private:
	SCENE_TYPE	m_ePreScene;
	SCENE_TYPE	m_eCurScene;
	CScene*		m_pScene;
};

