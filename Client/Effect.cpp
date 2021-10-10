#include "stdafx.h"
#include "Effect.h"


CEffect::CEffect(CEffectImpl* pImp)
	:m_pImplementor(pImp)
{
	ZeroMemory(&m_tInfo, sizeof(INFO));
	D3DXMatrixIdentity(&m_tEffect.matWorld);
}


CEffect::~CEffect()
{
	Release();
}

void CEffect::SetPos(const D3DXVECTOR3 & vPos)
{
	m_tEffect.vPos = vPos;
}

void CEffect::SetSize(const D3DXVECTOR3 & vSize)
{
	m_tEffect.vSize = vSize;
}

void CEffect::SetScale(const D3DXMATRIX & _matScale)
{
	m_tEffect.matScale = _matScale;
}

void CEffect::SetTrans(const D3DXMATRIX & _matTrans)
{
	m_tEffect.matTrans = _matTrans;
}

void CEffect::SetRotZ(const D3DXMATRIX & _matRotZ)
{
	m_tEffect.matRotZ = _matRotZ;
}

void CEffect::SetmatPlanetZ(const D3DXMATRIX & _matPlanetZ)
{
	m_tEffect.matPlanetZ = _matPlanetZ;
}

void CEffect::SetLength(const D3DXVECTOR3 & _vLength)
{
	m_tEffect.vLength = _vLength;
}


void CEffect::Release()
{
	SafeDelete(m_pImplementor);
}
