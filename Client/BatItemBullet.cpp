#include "stdafx.h"
#include "BatItemBullet.h"
#include "AnimationImpl.h"
#include "LongAttackEffect.h"

BatItemBullet::BatItemBullet()
{
}


BatItemBullet::~BatItemBullet()
{
	Release();
}

const UNIT_INFO & BatItemBullet::GetInfo() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_tUnit;
}



void BatItemBullet::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tUnit.vPos = vPos;
}



void BatItemBullet::SetDir(const D3DXVECTOR3 & vDir)
{
	m_tUnit.vDir = vDir;
	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);
}

void BatItemBullet::RectColl(CObj * pObj)
{
	m_tInfo.iHp -= 6.f; // 죽어라
}

int BatItemBullet::Update()
{
	CObj::LateInitialize();
	CObj::UpdateRect();

	if (m_tInfo.iHp <= 0.f)
	{
		D3DXVECTOR3 vSize = { 1.f,1.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"Revolver", L"RevolverBulletDestory", FRAME(0.f, 10.f), 3.f);

		CLongAttackEffect* pEffect = CLongAttackEffect::Create(pImpl, m_tUnit.vPos, vSize);

		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		return OBJ_DEAD;
	}

	MoveBullet();
	MakeWorldMatrix();

	return 0;
}

void BatItemBullet::LateUpdate()
{
}

void BatItemBullet::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey,
		0);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tUnit.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT BatItemBullet::Initialize()
{
	m_tUnit.vPos = { 0.f,0.f,0.f };
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vLength = { 60.f,60.f,0.f };
	m_tUnit.vSize = { 5.f,5.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Revolver";
	m_tUnit.Texture.wstrStateKey = L"RevolverBullet";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 1.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, 0);



	m_tItemData.iAtt = 30;
	m_tInfo.iHp = 4.f;
	return S_OK;
}

HRESULT BatItemBullet::LateInitialize()
{
	return E_NOTIMPL;
}

void BatItemBullet::Release()
{
}

void BatItemBullet::MoveBullet()
{
	m_tUnit.vPos += m_tUnit.vDir * 500.f * m_pTimeMgr->GetDelta();
}

void BatItemBullet::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);

	D3DXMATRIX matScroll;

	D3DXMatrixTranslation(&matScroll,
		-CScrollMgr::GetScrollPos(0),
		-CScrollMgr::GetScrollPos(1), 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans * matScroll;
}

void BatItemBullet::MakeLine()
{
}

BatItemBullet * BatItemBullet::Create(
	const D3DXVECTOR3 vPos,
	const D3DXVECTOR3 vDir)
{
	BatItemBullet* pInstance = new BatItemBullet;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);
	pInstance->SetDir(vDir);

	return pInstance;
}
