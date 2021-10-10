#include "stdafx.h"
#include "ExitButton.h"


CExitButton::CExitButton()
{
}


CExitButton::~CExitButton()
{
}

int CExitButton::Update()
{
	if (m_bIsDead == true)
	{
		return OBJ_DEAD;
	}
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
		m_tUnit.Texture.wstrStateKey = L"ExitOn";

	}
	else if (false == PtInRect(&m_tRect, pt))
	{
		m_tUnit.Texture.wstrStateKey = L"ExitOff";
	}

	return OBJ_NOEVENT;
}

void CExitButton::LateUpdate()
{
}

void CExitButton::Render()
{
	const TEX_INFO* ExitButton = nullptr;
	ExitButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(ExitButton);

	float CenterX = ExitButton->tImgInfo.Width * 0.5f;
	float CenterY = ExitButton->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		ExitButton->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CExitButton::Initialize()
{
	m_tUnit.vPos = { 400.f,450.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Exit";
	m_tUnit.Texture.wstrStateKey = L"ExitOff";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 60.f,30.f,0.f };
	const TEX_INFO* SetButton = nullptr;
	SetButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);


	m_tUnit.vSize.x = m_tUnit.vLength.x / SetButton->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / SetButton->tImgInfo.Height;

	return S_OK;
}

void CExitButton::Release()
{
}

CExitButton * CExitButton::Create()
{
	CExitButton* m_pInstance = new CExitButton;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		m_pInstance = nullptr;
	}
	return m_pInstance;
}
