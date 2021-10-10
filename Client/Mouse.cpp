#include "stdafx.h"
#include "Mouse.h"


CMouse::CMouse()
	:GripItem(false)
{
}


CMouse::~CMouse()
{
}

int CMouse::Update()
{
	MakeWorldMatrix();
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	m_tUnit.vPos.x = (float)pt.x;
	m_tUnit.vPos.y = (float)pt.y;
	
	return OBJ_NOEVENT;
}

void CMouse::LateUpdate()
{
	//CObj::FrameMove();
	
}

void CMouse::Render()
{
	m_tUnit.Texture.wstrObjectKey = L"MouseCursor";
	m_tUnit.Texture.wstrStateKey = L"Normal";
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, 0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CMouse::Initialize()
{
	m_tUnit.vPos = { 700.f,800.f,0.f };
	m_tUnit.vSize = { 3.f,3.f,0.f };
	pt = {}; // ¸¶¿ì½º ÁÂÇ¥


	return S_OK;
}

void CMouse::Release()
{
}

void CMouse::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}

CMouse * CMouse::Create()
{
	CMouse* m_pInstance = new CMouse;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
