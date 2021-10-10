#include "stdafx.h"
#include "SetButton.h"


CSetButton::CSetButton()
{
}


CSetButton::~CSetButton()
{
}

int CSetButton::Update()
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
		m_tUnit.Texture.wstrStateKey = L"On";
	}
	else if(false==PtInRect(&m_tRect, pt))
	{
		m_tUnit.Texture.wstrStateKey = L"Off";
	}

	return OBJ_NOEVENT;
}

void CSetButton::LateUpdate()
{
}

void CSetButton::Render()
{
	const TEX_INFO* SetButton = nullptr;
	SetButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(SetButton);

	float CenterX = SetButton->tImgInfo.Width * 0.5f;
	float CenterY = SetButton->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		SetButton->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

}

HRESULT CSetButton::Initialize()
{
	m_tUnit.vPos = { 400.f,400.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Setting";
	m_tUnit.Texture.wstrStateKey = L"Off";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 60.f,30.f,0.f };
	const TEX_INFO* SetButton = nullptr;
	SetButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	
	
	m_tUnit.vSize.x = m_tUnit.vLength.x / SetButton->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / SetButton->tImgInfo.Height;

	return S_OK;
}

void CSetButton::Release()
{

}

CSetButton * CSetButton::Create()
{
	CSetButton* m_pInstance = new CSetButton;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		m_pInstance = nullptr;
	}
	return m_pInstance;
}
