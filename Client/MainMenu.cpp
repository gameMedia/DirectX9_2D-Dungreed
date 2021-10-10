#include "stdafx.h"
#include "MainMenu.h"
#include "AbstractFactory.h"
#include "SetButton.h"
#include "StartButton.h"
#include "ExitButton.h"
CMainMenu::CMainMenu()
{
}


CMainMenu::~CMainMenu()
{
	Release();
}

void CMainMenu::Update()
{
	CUIMgr::Get_Instance()->Update();
}

void CMainMenu::LateUpdate()
{
	CUIMgr::Get_Instance()->LateUpdate();
}

void CMainMenu::Render()
{
	const TEX_INFO* BackGround = nullptr;
	BackGround = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Menu", L"MenuIdle", 0);
	NULL_CHECK(BackGround);

	m_pBackGround.vSize.x = 800.f / BackGround->tImgInfo.Width;
	m_pBackGround.vSize.y = 600.f / BackGround->tImgInfo.Height;

	D3DXMatrixScaling(&m_pBackGround.matScale, m_pBackGround.vSize.x, m_pBackGround.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_pBackGround.matTrans, 400.f, 300.f, 0.f);
	m_pBackGround.matWorld = m_pBackGround.matScale*m_pBackGround.matTrans;

	float BackGroundCenterX = BackGround->tImgInfo.Width * 0.5f;
	float BackGroundCenterY = BackGround->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pBackGround.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		BackGround->pTexture, nullptr, &D3DXVECTOR3(BackGroundCenterX, BackGroundCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	CUIMgr::Get_Instance()->Render();

	const TEX_INFO* Mouse = nullptr;
	Mouse = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Menu", L"Mouse", 0);

	m_pMenuMouse.vSize.x = 30.f / Mouse->tImgInfo.Width;
	m_pMenuMouse.vSize.y = 30.f / Mouse->tImgInfo.Height;

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	m_pMenuMouse.vPos.x = pt.x;
	m_pMenuMouse.vPos.y = pt.y;

	D3DXMatrixScaling(&m_pMenuMouse.matScale,
		m_pMenuMouse.vSize.x, m_pMenuMouse.vSize.y, 0.f);
	
	D3DXMatrixTranslation(&m_pMenuMouse.matTrans,
		m_pMenuMouse.vPos.x, m_pMenuMouse.vPos.y, 0.f);
	
	m_pMenuMouse.matWorld = 
		m_pMenuMouse.matScale*m_pMenuMouse.matTrans;

	float MouseX = Mouse->tImgInfo.Width * 0.5f;
	float MouseY = Mouse->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pMenuMouse.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		Mouse->pTexture, nullptr, &D3DXVECTOR3(MouseX, MouseY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



}

HRESULT CMainMenu::Initialize()
{
	m_pBackGround.vPos = { 0.f,0.f,0.f };
	m_pBackGround.vSize = { 1.f,1.f,0.f };

	HRESULT hr = CUIMgr::Get_Instance()->Add_Object(UIID::SetButton, CAbstractFactory<CSetButton>::CreateUI());
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = CUIMgr::Get_Instance()->Add_Object(UIID::StartButton, CAbstractFactory<CStartButton>::CreateUI());
	FAILED_CHECK_RETURN(hr, E_FAIL);


	hr = CUIMgr::Get_Instance()->Add_Object(UIID::ExitButton, CAbstractFactory<CExitButton>::CreateUI());
	FAILED_CHECK_RETURN(hr, E_FAIL);

	CSoundMgr::Get_Instance()->PlayBGM(L"MyTitle.wav");
	return S_OK;
}

void CMainMenu::Release()
{
	dynamic_cast<CSetButton*>(CUIMgr::Get_Instance()->Get_SetButton())->Set_Dead(true);
	dynamic_cast<CExitButton*>(CUIMgr::Get_Instance()->Get_ExitButton())->Set_Dead(true);
	CSoundMgr::Get_Instance()->StopAll();
}

CMainMenu * CMainMenu::Create()
{
	CMainMenu* m_pInstance = new CMainMenu;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
