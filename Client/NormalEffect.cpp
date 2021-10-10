#include "stdafx.h"
#include "NormalEffect.h"
#include "EffectImpl.h"
#include "AnimationImpl.h"
CNormalEffect::CNormalEffect(CEffectImpl* pImp)
	:CEffect(pImp)
{
}


CNormalEffect::~CNormalEffect()
{
	Release();
}

int CNormalEffect::Update()
{
	CObj::LateInitialize();

	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans,
		m_tEffect.vPos.x-CScrollMgr::GetScrollPos(0),
		m_tEffect.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tEffect.matWorld = m_tEffect.matScale*m_tEffect.matTrans;

	return m_pImplementor->Update();
}

void CNormalEffect::LateUpdate()
{
	
}

void CNormalEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CNormalEffect::Initialize()
{

	m_tEffect.vSize = { 3.f,3.f,0.f };
	m_tEffect.vLook = { 1.f,0.f,0.f };
	return S_OK;
}

HRESULT CNormalEffect::LateInitialize()
{
	return S_OK;
}

void CNormalEffect::Release()
{
}

CNormalEffect* CNormalEffect::Create(
	CEffectImpl * pImp, const D3DXVECTOR3 & vPos)
{
	CNormalEffect* pInstance = new CNormalEffect(pImp);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);

	return pInstance;
}
