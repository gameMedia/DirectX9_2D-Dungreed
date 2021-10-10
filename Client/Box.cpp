#include "stdafx.h"
#include "Box.h"
#include "Player.h"
#include "WingBoot.h"
#include "AbstractFactory.h"

CBox::CBox()
{
	InitiOneTime = false;
}


CBox::~CBox()
{
}

int CBox::Update()
{

	MakeWorldMatrix();
	UpdateRect();

	RECT rc = {};
	PlayerRect = CObjMgr::Get_Instance()->Get_Player()->Get_Rect();
	if (IntersectRect(&rc, &PlayerRect, &m_tRect))
	{
		if (CKeyMgr::Get_Instance()->KeyDown(KEY_K))
		{
			if (InitiOneTime == false)
			{
				m_tUnit.Texture.wstrStateKey = L"BossTresureOpened";
				CObjMgr::Get_Instance()->Add_Object(OBJID::WINGBOOT, CAbstractFactory<CWingBoot>::Create(m_tUnit.vPos.x, m_tUnit.vPos.y));

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::BOXOPEN);
				CSoundMgr::Get_Instance()->PlaySound(L"MyChest.wav", CSoundMgr::BOXOPEN);

				InitiOneTime = true;
			}
		}
	}

	return 0;
}

void CBox::LateUpdate()
{
	
}

void CBox::Render()
{
	const TEX_INFO* RegenRing = nullptr;
	RegenRing = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(RegenRing);

	float CenterX = RegenRing->tImgInfo.Width * 0.5f;
	float CenterY = RegenRing->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		RegenRing->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBox::Initialize()
{
	m_tUnit.vPos = { 1270.f,900.f,0.f };


	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"BossTreasure";
	m_tUnit.Texture.wstrStateKey = L"BossTresureClosed";

	m_tUnit.Texture.iIndex = 0;

	const TEX_INFO* Shopn = nullptr;
	Shopn = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey,
		m_tUnit.Texture.iIndex);

	m_tUnit.vLength = { 700.f,500.f,0.f };

	m_tUnit.vSize.x = m_tUnit.vLength.x / Shopn->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / Shopn->tImgInfo.Height;

	return S_OK;
}

void CBox::Release()
{
}

void CBox::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}

void CBox::UpdateRect()
{
	m_tRect.left = LONG(m_tUnit.vPos.x - (m_tUnit.vLength.x * 0.5f));
	m_tRect.top = LONG(m_tUnit.vPos.y - (m_tUnit.vLength.y * 0.5f));
	m_tRect.right = LONG(m_tUnit.vPos.x + (m_tUnit.vLength.x * 0.5f));
	m_tRect.bottom = LONG(m_tUnit.vPos.y + (m_tUnit.vLength.y * 0.5f));
}

CBox * CBox::Create()
{
	CBox* m_pInstance = new CBox;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
