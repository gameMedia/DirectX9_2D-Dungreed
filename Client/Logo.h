#pragma once
#include "Scene.h"
class CSingleTexture;
class CLogo :
	public CScene
{
private:
	explicit CLogo();
public:
	~CLogo();

public:
	// CScene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;

public:
	static CLogo* Create();
public:
	static int Loading;

private:
	static unsigned int __stdcall LoadingResource(void* pParam);

private:
	CRITICAL_SECTION	m_CriticalSection;
	HANDLE	m_hLoadingThread;
	UNIT_INFO m_pBackGround;
	UNIT_INFO m_pBackLoadingBar;
	UNIT_INFO m_pLoadingBar;


};

