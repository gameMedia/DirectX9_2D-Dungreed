#include "stdafx.h"
#include "Logo.h"

int CLogo::Loading = 0;
CLogo::CLogo()
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Update()
{

}

void CLogo::LateUpdate()
{
}

void CLogo::Render()
{
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_RETURN)|| Loading == 134)
	{

		WaitForSingleObject(m_hLoadingThread, INFINITE);
		m_pSceneMgr->SceneChange(CSceneMgr::MAINMENU);
		return;
	}
	
	
	const TEX_INFO* BackGround = nullptr;
	const TEX_INFO* BackLoadingBar = nullptr;
	const TEX_INFO* LoadingBar = nullptr;

	BackGround = CTextureMgr::Get_Instance()->GetTexInfo(L"Main");
	NULL_CHECK(BackGround);
	BackLoadingBar = CTextureMgr::Get_Instance()->GetTexInfo(L"LoadingBarBase");
	NULL_CHECK(BackLoadingBar);
	LoadingBar = CTextureMgr::Get_Instance()->GetTexInfo(L"LoadingBar");
	NULL_CHECK(LoadingBar);

	m_pBackGround.vSize.x = 800.f / BackGround->tImgInfo.Width;
	m_pBackGround.vSize.y = 600.f / BackGround->tImgInfo.Height;
	
	m_pBackLoadingBar.vSize.x = 600.f / BackLoadingBar->tImgInfo.Width;
	m_pBackLoadingBar.vSize.y = 200.f / BackLoadingBar->tImgInfo.Height;
	
	
	D3DXMatrixScaling(&m_pBackGround.matScale, m_pBackGround.vSize.x, m_pBackGround.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_pBackGround.matTrans, 400.f, 300.f, 0.f);
	m_pBackGround.matWorld = m_pBackGround.matScale*m_pBackGround.matTrans;

	D3DXMatrixScaling(&m_pBackLoadingBar.matScale, m_pBackLoadingBar.vSize.x, m_pBackLoadingBar.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_pBackLoadingBar.matTrans, 400.f, 500.f, 0.f);
	m_pBackLoadingBar.matWorld = m_pBackLoadingBar.matScale*m_pBackLoadingBar.matTrans;
	
	m_pLoadingBar.vSize.x = 590.f / LoadingBar->tImgInfo.Width;
	m_pLoadingBar.vSize.y = 190.f / LoadingBar->tImgInfo.Height;

	D3DXMatrixScaling(&m_pLoadingBar.matScale, ( (1.f / 134) * Loading)*m_pLoadingBar.vSize.x, m_pLoadingBar.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_pLoadingBar.matTrans, 110.f, 400.f, 0.f);
	m_pLoadingBar.matWorld = m_pLoadingBar.matScale*m_pLoadingBar.matTrans;

	float BackGroundCenterX = BackGround->tImgInfo.Width * 0.5f;
	float BackGroundCenterY = BackGround->tImgInfo.Height * 0.5f;
	
	float BackLoadingBarX = BackLoadingBar->tImgInfo.Width*0.5f;
	float BackLoadingBarY = BackLoadingBar->tImgInfo.Height*0.5f;
	
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pBackGround.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		BackGround->pTexture, nullptr, &D3DXVECTOR3(BackGroundCenterX, BackGroundCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pBackLoadingBar.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		BackLoadingBar->pTexture, nullptr, &D3DXVECTOR3(BackLoadingBarX, BackLoadingBarY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	
	float LoadingBarX = LoadingBar->tImgInfo.Width*0.5f;
	float LoadingBarY = LoadingBar->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pLoadingBar.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		LoadingBar->pTexture, nullptr, /*&D3DXVECTOR3(LoadingBarX, LoadingBarY, 0.f)*/nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	D3DXMATRIX matTrans, matWorld, matScale;
	
	const wstring& wstrLoadingMessage = m_pTextureMgr->GetLoadingMessage();
	D3DXMatrixTranslation(&matTrans, 100.f, 400.f, 0.f);

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&matTrans);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(CDeviceMgr::Get_Instance()->GetSprite(), wstrLoadingMessage.c_str(),
		wstrLoadingMessage.length(), nullptr, 0, D3DCOLOR_ARGB(255, 0, 255, 0));

	
	
	


	
}

HRESULT CLogo::Initialize()
{
	/*m_pBackGround.vPos = { 0.f,0.f,0.f };
	m_pBackGround.vSize = { 1.f,1.f,0.f };
	m_pBackLoadingBar.vPos = { 0.f,0.f,0.f };
	m_pBackLoadingBar.vSize = { 1.f,1.f,0.f };*/

	InitializeCriticalSection(&m_CriticalSection);

	m_hLoadingThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingResource, this, 0, nullptr);
	HRESULT hr = CTextureMgr::Get_Instance()->LoadTexture(CTexture::TEX_SINGLE,
		L"../Texture/Logo/MenuIdle/Main.png",
		L"Main");
	hr = CTextureMgr::Get_Instance()->LoadTexture(CTexture::TEX_SINGLE,
		L"../Texture/Logo/MainLogo/MainLogo.png",
		L"MainLogo");
	hr = CTextureMgr::Get_Instance()->LoadTexture(CTexture::TEX_SINGLE,
		L"../Texture/Logo/LoadingBarBase/LoadingBarBase.png",
		L"LoadingBarBase");
	hr = CTextureMgr::Get_Instance()->LoadTexture(CTexture::TEX_SINGLE,
		L"../Texture/Logo/LoadingBar/LoadingBar.png",
		L"LoadingBar");

	return S_OK;
}

void CLogo::Release()
{
	CloseHandle(m_hLoadingThread);
	DeleteCriticalSection(&m_CriticalSection);
}

unsigned int CLogo::LoadingResource(void * pParam)
{
	CLogo* pLogo = reinterpret_cast<CLogo*>(pParam);
	
	EnterCriticalSection(&pLogo->m_CriticalSection);

	// 기본이미지들 로드
	HRESULT hr = pLogo->m_pTextureMgr->LoadFromImgPath(L"../Data/ImgPath.txt");
	FAILED_CHECK_RETURN(hr, 1);


	pLogo->m_pTextureMgr->SetLoadingMessage(L"로딩 완료");

	LeaveCriticalSection(&pLogo->m_CriticalSection);
	return 0;
}

CLogo * CLogo::Create()
{
	CLogo* pInstance = new CLogo;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	return pInstance;
}
