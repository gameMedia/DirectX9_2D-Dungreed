#include "stdafx.h"
#include "BatBullet.h"
#include "AnimationImpl.h"
#include "LongAttackEffect.h"

CBatBullet::CBatBullet()
{
}


CBatBullet::~CBatBullet()
{
	Release();
}

const UNIT_INFO & CBatBullet::GetInfo() const
{
	// TODO: ¿©±â¿¡ ¹ÝÈ¯ ±¸¹®À» »ðÀÔÇÕ´Ï´Ù.
	return m_tUnit;
}

void CBatBullet::SetAngle(const float & Angle)
{
	m_fAngle = Angle;
}

void CBatBullet::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tUnit.vPos = vPos;
}

void CBatBullet::SetDir(const D3DXVECTOR3 & vDir)
{
	m_tUnit.vDir = vDir;
	D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);
}

void CBatBullet::RectColl(CObj * pObj)
{
	m_tInfo.iHp -= 6.f; // Á×¾î¶ó
}

int CBatBullet::Update()
{
	CObj::LateInitialize();
	CObj::UpdateRect();
	if (m_tInfo.iHp <= 0.f)
	{
		D3DXVECTOR3 vSize = { 2.f,2.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"RangeBallBullet", L"Hit", FRAME(0.f, 6.f), 3.f);

		CLongAttackEffect* pEffect = CLongAttackEffect::Create(pImpl, m_tUnit.vPos, vSize);

		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);
		return OBJ_DEAD;
	}
	if (m_tUnit.Texture.wstrStateKey == L"Create")
	{
		if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt - 2.f)
		{
			m_tUnit.Texture.wstrStateKey = L"Idle";
			m_tFrame.fFrameCnt = 1.f;
		}
	}
	else if (m_tUnit.Texture.wstrStateKey == L"Idle")
	{
		MoveBullet();
	}

	MakeWorldMatrix();


	return OBJ_NOEVENT;
}

void CBatBullet::LateUpdate()
{

	FrameMove(1.f);
}

void CBatBullet::Render()
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

HRESULT CBatBullet::Initialize()
{
	m_tUnit.vLook = { 1.f,0.f,0.f };
	m_tUnit.vLength = { 50.f,50.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"RangeBallBullet";
	m_tUnit.Texture.wstrStateKey = L"Create";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 5.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	// ¹ê½¬°¡ ½î´Â ÃÑ¾ËÀÇ Á¤º¸

	// ¹ÚÁã°¡ ½î´Â ÃÑ¾ËÀÇ Á¤º¸
	m_tInfo.iAtt = 7.f;
	m_tInfo.iHp = 4.f;
	m_tInfo.iGold = 0;
	return S_OK;
}

HRESULT CBatBullet::LateInitialize()
{
	return S_OK;
}

void CBatBullet::Release()
{
}

void CBatBullet::MoveBullet()
{
	m_tUnit.vPos += m_tUnit.vDir * 300.f * m_pTimeMgr->GetDelta();
}

void CBatBullet::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	

	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tUnit.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);
	D3DXMatrixRotationZ(&m_tUnit.matPlanetZ, D3DXToRadian(-m_fAngle)); // °øÀü
	D3DXVECTOR3 matTrans;
	

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matPlanetZ * m_tUnit.matTrans;
}

void CBatBullet::MakeLine()
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

CBatBullet * CBatBullet::Create(const D3DXVECTOR3 & vPos
	, const D3DXVECTOR3 vDir,const float Angle)
{
	CBatBullet* pInstance = new CBatBullet;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	
	D3DXVECTOR3 vRealPos;
	D3DXMATRIX matWorld, matRev, matTrans;
	D3DXMatrixTranslation(&matTrans, 60,0,0);
	D3DXMatrixRotationZ(&matRev, D3DXToRadian(Angle));
	matWorld = matTrans * matRev;
 	D3DXVec3TransformCoord(&vRealPos, &D3DXVECTOR3(0,0,0), &matWorld);
	vRealPos += vPos;
	pInstance->SetPos(vRealPos);

	pInstance->SetDir(vDir);
	//pInstance->SetAngle(Angle);
	return pInstance;
}
