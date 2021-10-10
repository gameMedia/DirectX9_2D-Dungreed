#include "stdafx.h"
#include "IceBullet.h"
#include "AnimationImpl.h"
#include "LongAttackEffect.h"

CIceBullet::CIceBullet()
{
}


CIceBullet::~CIceBullet()
{
	Release();
}

const UNIT_INFO & CIceBullet::GetInfo() const
{
	// TODO: ¿©±â¿¡ ¹ÝÈ¯ ±¸¹®À» »ðÀÔÇÕ´Ï´Ù.
	return m_tUnit;
}

void CIceBullet::SetAngle(const float & Angle)
{
	m_fAngle = Angle;
}

void CIceBullet::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tUnit.vPos = vPos;
}

void CIceBullet::SetDir(const D3DXVECTOR3 & vDir)
{
	m_tUnit.vDir = vDir;
	//D3DXVec3Normalize(&m_tUnit.vDir, &m_tUnit.vDir);
}

void CIceBullet::SetParent(const D3DXMATRIX & _Parent)
{
	m_tUnit.matParent = _Parent;
}

void CIceBullet::SetSpeed(const float & Speed)
{
	BulletSpeed = Speed;
}

void CIceBullet::RectColl(CObj * pObj)
{
	if (pObj != nullptr)
	{
		if (pObj->Get_ItemData().eLength == ITEMINTER::MELEE)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ICEBULLETDEAD);
			CSoundMgr::Get_Instance()->PlaySound(L"MyIceBulletDead.wav", CSoundMgr::ICEBULLETDEAD);
		}
	}
	m_tInfo.iHp -= 6.f; // Á×¾î¶ó
}

int CIceBullet::Update()
{
	CObj::LateInitialize();
	CObj::UpdateRect();
	if (m_tInfo.iHp <= 0.f)
	{
		D3DXVECTOR3 vSize = { 2.f,2.f,0.f };
		CAnimationImpl* pImpl = CAnimationImpl::Create(
			L"IceBullet", L"DestoryIceBullet", FRAME(0.f, 3.f), 3.f);

		CLongAttackEffect* pEffect = CLongAttackEffect::Create(pImpl, m_tUnit.vPos, vSize);

		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);


		return OBJ_DEAD;
	}
	if (m_tUnit.Texture.wstrStateKey == L"CreateIceBullet")
	{
		if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt - 1.f)
		{
			m_tUnit.Texture.wstrStateKey = L"IceBulletIdle";
			m_tFrame.fFrameCnt = 0.f;
		}
	}
	else if (m_tUnit.Texture.wstrStateKey == L"IceBulletIdle")
	{
		MoveBullet();
	}

	MakeWorldMatrix();


	return OBJ_NOEVENT;
}

void CIceBullet::LateUpdate()
{

	FrameMove(3.f);
}

void CIceBullet::Render()
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

HRESULT CIceBullet::Initialize()
{
	m_tUnit.vLook = { 0.f,-1.f,0.f };
	m_tUnit.vLength = { 50.f,50.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.vPos = { 0.f,0.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"IceBullet";
	m_tUnit.Texture.wstrStateKey = L"CreateIceBullet";

	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 3.f;

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, (int)m_tFrame.fCurFrame);


	m_tUnit.vSize.x = m_tUnit.vLength.x / pTexInfo->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / pTexInfo->tImgInfo.Height;

	// ¹ê½¬°¡ ½î´Â ÃÑ¾ËÀÇ Á¤º¸

	// ¹ÚÁã°¡ ½î´Â ÃÑ¾ËÀÇ Á¤º¸
	m_tInfo.iAtt = 6.f;
	m_tInfo.iHp = 4.f;
	m_tInfo.iGold = 0;
	return S_OK;
}

HRESULT CIceBullet::LateInitialize()
{
	return S_OK;
}

void CIceBullet::Release()
{
}

void CIceBullet::MoveBullet()
{
	m_tUnit.vPos += m_tUnit.vDir * BulletSpeed * m_pTimeMgr->GetDelta();
}

void CIceBullet::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);


	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x ,
		m_tUnit.vPos.y , 0.f);
	float cos = D3DXVec3Dot(&m_tUnit.vLook, &m_tUnit.vDir);
	float mycos = D3DXToDegree(acosf(cos));
	if (m_tUnit.vDir.x > 0.f)
	{
		mycos *= -1.f;
	}
	D3DXMatrixRotationZ(&m_tUnit.matPlanetZ, D3DXToRadian(-mycos)); // °øÀü
	

	D3DXMatrixTranslation(&matScroll,
		-CScrollMgr::GetScrollPos(0),
		-CScrollMgr::GetScrollPos(1),
		0.f);

	D3DXVECTOR3 matTrans;


	m_tUnit.matWorld = m_tUnit.matScale* m_tUnit.matPlanetZ * m_tUnit.matTrans/** m_tUnit.matParent*/*matScroll;
}

void CIceBullet::MakeLine()
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

CIceBullet * CIceBullet::Create(
	const D3DXMATRIX ParentMatrix,
	const D3DXVECTOR3 vDir,
	const D3DXVECTOR3 vPos,
	const float Speed)
{
	CIceBullet* pInstance = new CIceBullet;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->SetSpeed(Speed);
	pInstance->SetParent(ParentMatrix);
	pInstance->SetDir(vDir);
	pInstance->SetPos(vPos);

	return pInstance;
}
