#pragma once
#include "Scene.h"
class CMainMenu :
	public CScene
{
private:
	explicit CMainMenu();
public:
	virtual ~CMainMenu();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CMainMenu* Create();

private:
	UNIT_INFO m_pBackGround;
	UNIT_INFO m_pMenuMouse;

};

