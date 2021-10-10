#include "stdafx.h"
#include "NoAnimationImpl.h"


CNoAnimationImpl::CNoAnimationImpl(
	const wstring&wstrObjectKey,
	float fLifeTime,
	float _playSpeed)
	:CEffectImpl(wstrObjectKey),
	m_fLifeTime(fLifeTime),
	m_fTimeCount(0.f)
{
	playSpeed = _playSpeed;
}


CNoAnimationImpl::~CNoAnimationImpl()
{
}

int CNoAnimationImpl::Update()
{
	m_fTimeCount += CTimeMgr::Get_Instance()->GetDelta()*playSpeed;

	if (m_fLifeTime <= m_fTimeCount)
		return OBJ_DEAD;
	
	return OBJ_NOEVENT;
}

void CNoAnimationImpl::Render(const D3DXMATRIX * pWorldMatrix)
{
	const TEX_INFO* pTexInfo = CTextureMgr::Get_Instance()->GetTexInfo(
		m_wstrObjectKey, L"Cray", 7);
	NULL_CHECK(pTexInfo);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(pWorldMatrix);
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f), nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


}

CNoAnimationImpl * CNoAnimationImpl::Create(
	const wstring & wstrObjectKey,
	float fLifeTime,
	float _playSpeed)
{
	return new CNoAnimationImpl(wstrObjectKey, fLifeTime, _playSpeed);
}
