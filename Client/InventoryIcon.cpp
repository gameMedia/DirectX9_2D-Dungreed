#include "stdafx.h"
#include "InventoryIcon.h"


CInventoryIcon::CInventoryIcon()
{
}


CInventoryIcon::~CInventoryIcon()
{
}

int CInventoryIcon::Update()
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
	
		if (CKeyMgr::Get_Instance()->KeyDown(VK_LBUTTON))
		{
			

		}
	}


	return OBJ_NOEVENT;
}

void CInventoryIcon::LateUpdate()
{
}

void CInventoryIcon::Render()
{
	const TEX_INFO* InventoryIcon = nullptr;
	InventoryIcon = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(InventoryIcon);

	float CenterX = InventoryIcon->tImgInfo.Width * 0.5f;
	float CenterY = InventoryIcon->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		InventoryIcon->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CInventoryIcon::Initialize()
{
	m_tUnit.vPos = { 700.f,50.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Inventory";
	m_tUnit.Texture.wstrStateKey = L"InventoryIcon";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 40.f,40.f,0.f };
	const TEX_INFO* StartButton = nullptr;
	StartButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / StartButton->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / StartButton->tImgInfo.Height;

	return S_OK;
}

void CInventoryIcon::Release()
{
}

void CInventoryIcon::MakeWorldMatrix()
{

	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}

CInventoryIcon * CInventoryIcon::Create()
{
	CInventoryIcon* m_pInstance = new CInventoryIcon;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);

	}
	return m_pInstance;
}
