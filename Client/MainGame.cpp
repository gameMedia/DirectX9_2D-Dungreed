#include "stdafx.h"
#include "MainGame.h"




CMainGame::CMainGame()
	:m_fTimeCount(0)
	,m_FPS(0)
	,m_szFPS(L"")
	,lateInit(false)
	,m_pSceneMgr(CSceneMgr::Get_Instance())
	,m_pTimeMgr(CTimeMgr::Get_Instance())
{
	D3DXMatrixIdentity(&FPSTrans);
}


CMainGame::~CMainGame()
{

	Release();
}

HRESULT CMainGame::Initialize()
{
	srand(unsigned int(time(NULL)));
	HRESULT hr = CDeviceMgr::Get_Instance()->InitDevice(
		g_hWND, WINCX, WINCY, CDeviceMgr::MODE_FULL);
	FAILED_CHECK_RETURN(hr, E_FAIL);


	ShowCursor(FALSE);



	return S_OK;
}

void CMainGame::Update()
{
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_M))
	{
		if (lateInit == false)
		{
			HRESULT hr = m_pSceneMgr->SceneChange(CSceneMgr::LOGO);

			lateInit = true;
		}
	}
		
	

	m_pTimeMgr->UpdateTime(); // 프로세스 시간을 굴려줘
	CKeyMgr::Get_Instance()->UpdateKey();
	m_pSceneMgr->Update();

}

void CMainGame::LateUpdate()
{
	m_pSceneMgr->LateUpdate();
	

}

void CMainGame::Render()
{
	//Rectangle(m_hDC, 0, 0, WINCX, WINCY);
	CDeviceMgr::Get_Instance()->Render_Begin();

	m_pSceneMgr->Reneder();
	
	

	CDeviceMgr::Get_Instance()->Render_End();
	
	
	
	
}

void CMainGame::Release()
{
	m_pSceneMgr->Destory_Instance();
	CObjMgr::Destroy_Instance();
	CTextureMgr::Get_Instance()->Destory_Instance();
	CDeviceMgr::Get_Instance()->Destory_Instance();
	m_pTimeMgr->Get_Instance()->Destory_Instance();
	//CKeyMgr::Get_Instance()->Destory_Instance();

	ReleaseDC(g_hWND, m_hDC);
}

CMainGame * CMainGame::Create()
{
	CMainGame* m_pInstance = new CMainGame;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}

void CMainGame::InitKey()
{
	// 점프

}
