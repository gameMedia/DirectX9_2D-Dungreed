#include "stdafx.h"
#include "DashEffect.h"
#include "EffectImpl.h"

CDashEffect::CDashEffect(CEffectImpl* pImp)
	:CEffect(pImp)
{
}


CDashEffect::~CDashEffect()
{
	Release();
}

int CDashEffect::Update()
{
	CObj::LateInitialize();

	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans,
		m_tEffect.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tEffect.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tEffect.matWorld = m_tEffect.matScale*m_tEffect.matTrans;

	return m_pImplementor->Update();
}

void CDashEffect::LateUpdate()
{
}

void CDashEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CDashEffect::Initialize()
{
	m_tEffect.vSize = { 3.f,3.f,0.f };
	m_tEffect.vLook = { 1.f,0.f,0.f };
	return S_OK;
}

HRESULT CDashEffect::LateInitialize()
{
	return S_OK;
}

void CDashEffect::Release()
{
}

CDashEffect * CDashEffect::Create(
	CEffectImpl * pImp, 
	const D3DXVECTOR3 & vPos,
	const D3DXVECTOR3 & vSize)
{
	CDashEffect* pInstance = new CDashEffect(pImp);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);
	pInstance->SetSize(vSize);

	return pInstance;
}
