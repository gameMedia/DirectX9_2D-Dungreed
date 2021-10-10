#include "stdafx.h"
#include "StartButton.h"


CStartButton::CStartButton()
{
}


CStartButton::~CStartButton()
{
}

int CStartButton::Update()
{
	CUI::MakeWorldMatrix();

	CUI::UpdateRect();
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);

	D3DXVECTOR3 vMouse = {};
	vMouse.x = (float)pt.x;
	vMouse.y = (float)pt.y;

	if (PtInRect(&m_tRect, pt))
	{
		m_tUnit.Texture.wstrStateKey = L"PlayOn";
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON))
		{
			CSceneMgr::Get_Instance()->SceneChange(CSceneMgr::Get_Instance()->STAGE);
			return OBJ_DEAD;
		}
	}
	else if (false == PtInRect(&m_tRect, pt))
	{
		m_tUnit.Texture.wstrStateKey = L"PlayOff";
	}

	return OBJ_NOEVENT;
}

void CStartButton::LateUpdate()
{
}

void CStartButton::Render()
{
	const TEX_INFO* StartButton = nullptr;
	StartButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(StartButton);

	float CenterX = StartButton->tImgInfo.Width * 0.5f;
	float CenterY = StartButton->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		StartButton->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
}

HRESULT CStartButton::Initialize()
{
	m_tUnit.vPos = { 400.f,350.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"GameStart";
	m_tUnit.Texture.wstrStateKey = L"PlayOff";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 80.f,40.f,0.f };
	const TEX_INFO* StartButton = nullptr;
	StartButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / StartButton->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / StartButton->tImgInfo.Height;

	return S_OK;
}

void CStartButton::Release()
{

}

CStartButton * CStartButton::Create()
{
	CStartButton* m_pInstance = new CStartButton;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		
	}
	return m_pInstance;
}
