#include "stdafx.h"
#include "BanshBullet.h"
#include "LongAttackEffect.h"
#include "AnimationImpl.h"

CBanshBullet::CBanshBullet()
{
}


CBanshBullet::~CBanshBullet()
{
	Release();
}

const UNIT_INFO & CBanshBullet::GetInfo() const
{
	// TODO: ¿©±â¿¡ ¹ÝÈ¯ ±¸¹®À» »ðÀÔÇÕ´Ï´Ù.
	return m_tUnit;
}

void CBanshBullet::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tUnit.vPos = vPos;
}

void CBanshBullet::SetDir(const D3DXVECTOR3 & vDir)
{
	m_tUnit.vDir = vDir;
	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);
}

void CBanshBullet::RectColl(CObj * pObj)
{
	m_bIsDead = true; // ÀÏ´Ü Åº¾Ë ÆÄ±«
	m_tInfo.iHp -= 6.f; // Á×¾î¶ó

}

int CBanshBullet::Update()
{
	CObj::LateInitialize();
	CObj::UpdateRect();
	if (m_tInfo.iHp<=0.f)
	{
		D3DXVECTOR3 vSize = { 2.f,2.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"BanshBullet", L"Dead", FRAME(0.f, 6.f), 3.f);

		CLongAttackEffect* pEffect = CLongAttackEffect::Create(pImpl, m_tUnit.vPos, vSize);

		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		return OBJ_DEAD;
	}

	MoveBullet();
	MakeWorldMatrix();


	return OBJ_NOEVENT;
}

void CBanshBullet::LateUpdate()
{
	FrameMove(1.f);
}

void CBanshBullet::Render()
{
	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey,
		(int)m_tFrame.fCurFrame);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	m_pDeviceMgr->GetSprite()->SetTransform(&m_tUnit.matWorld);
	m_pDeviceMgr->GetSprite()->Draw(pTexInfo->pTexture, nullptr,
		&D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CBanshBullet::Initialize()
{
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vLength = { 50.f,50.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"BanshBullet";
	m_tUnit.Texture.wstrStateKey = L"Idle";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 4.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	// ¹ê½¬°¡ ½î´Â ÃÑ¾ËÀÇ Á¤º¸
	m_tInfo.iAtt = 4;
	m_tInfo.iHp = 4.f;
	m_tInfo.iGold = 0;
	return S_OK;
}

HRESULT CBanshBullet::LateInitialize()
{
	return S_OK;
}

void CBanshBullet::Release()
{
}

void CBanshBullet::MoveBullet()
{
	m_tUnit.vPos += m_tUnit.vDir * 300.f * m_pTimeMgr->GetDelta();
}

void CBanshBullet::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;
}

void CBanshBullet::MakeLine()
{
	m_vOrigin[0] = {
		(-m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[1] = {
		(m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[2] = {
		(m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[3] = {
		(-m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
	m_vOrigin[4] = {
		(-m_tUnit.vLength.x*0.5f) + m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		(-m_tUnit.vLength.y*0.5f) + m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1) };
}

CBanshBullet * CBanshBullet::Create(
	const D3DXVECTOR3 & vPos,
	const D3DXVECTOR3 & vDir)
{
	CBanshBullet* pInstance = new CBanshBullet;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);
	pInstance->SetDir(vDir);

	return pInstance;
}
