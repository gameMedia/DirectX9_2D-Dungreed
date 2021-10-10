#pragma once

class CActMgr;
class CScene
{
protected:
	explicit CScene();

public:
	virtual ~CScene();

public:
	virtual void Update() PURE;
	virtual void LateUpdate() = 0;
	virtual void Render() abstract;

protected:
	virtual HRESULT Initialize() PURE;
	virtual void Release() PURE;
	virtual void InitKey();
protected:
	CSceneMgr*		m_pSceneMgr;
	CTextureMgr*	m_pTextureMgr;
	CTimeMgr*		m_pTimeMgr;
	HDC				m_hDC;
	CObjMgr*		m_pObjectMgr;
	CActMgr*		m_pActMgr;
};

