#include "stdafx.h"
#include "RevolverBullet.h"

#include "AnimationImpl.h"
#include "LongAttackEffect.h"
#include "Player.h"

#include "SubHand.h"

CRevolverBullet::CRevolverBullet()
{
}


CRevolverBullet::~CRevolverBullet()
{
	Release();
}

const UNIT_INFO & CRevolverBullet::GetInfo() const
{
	// TODO: ¿©±â¿¡ ¹ÝÈ¯ ±¸¹®À» »ðÀÔÇÕ´Ï´Ù.
	return m_tUnit;
}

void CRevolverBullet::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tUnit.vPos = vPos;
}

void CRevolverBullet::SetDir(const D3DXVECTOR3 & vDir)
{
	m_tUnit.vDir = vDir;
	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);
}

void CRevolverBullet::RectColl(CObj * pObj)
{
	m_bIsDead = true; // ÀÏ´Ü Åº¾Ë ÆÄ±«
	m_tInfo.iHp -= 6.f; // Á×¾î¶ó
}

int CRevolverBullet::Update()
{
	ITEM_DATA SubHandData = dynamic_cast<CSubHand*>(CObjMgr::Get_Instance()->Get_SubHand())->Get_Main();
	m_tItemData.iAtt = m_tInfo.iAtt + CPlayer::PlusAtt+SubHandData.iAtt;
	CObj::LateInitialize();
	CObj::UpdateRect();

	//if (0.f >= m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0) ||
	//	(float)WINCX <= m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0) ||
	//	0.f >= m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) ||
	//	(float)WINCY <= m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1))
	//{
	//	m_tInfo.iHp - 6.f;
	//}

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


	return OBJ_NOEVENT;
}

void CRevolverBullet::LateUpdate()
{
	FrameMove(1.f);
}

void CRevolverBullet::Render()
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

HRESULT CRevolverBullet::Initialize()
{
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vLength = { 20.f,20.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Revolver";
	m_tUnit.Texture.wstrStateKey = L"RevolverBullet";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 1.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, 0);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	// ¹ê½¬°¡ ½î´Â ÃÑ¾ËÀÇ Á¤º¸
	m_tInfo.iAtt = 50;
	m_tItemData.iAtt = 50;
	m_tInfo.iHp = 4.f;

	return S_OK;
}

HRESULT CRevolverBullet::LateInitialize()
{
	return S_OK;
}

void CRevolverBullet::Release()
{
}

void CRevolverBullet::MoveBullet()
{
	m_tUnit.vPos += m_tUnit.vDir * 1500.f * m_pTimeMgr->GetDelta();
}

void CRevolverBullet::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}

void CRevolverBullet::MakeLine()
{
}

CRevolverBullet * CRevolverBullet::Create(
	const D3DXVECTOR3 vPos,
	const D3DXVECTOR3 vDir)
{
	CRevolverBullet* pInstance = new CRevolverBullet;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);
	pInstance->SetDir(vDir);

	return pInstance;
}
