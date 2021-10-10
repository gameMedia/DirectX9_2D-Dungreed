#include "stdafx.h"
#include "WarningEffect.h"
#include "EffectImpl.h"

CWarningEffect::CWarningEffect(CEffectImpl* pImp)
	:CEffect(pImp)
{
}


CWarningEffect::~CWarningEffect()
{
	Release();
}

int CWarningEffect::Update()
{
	CObj::LateInitialize();

	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans,
		m_tEffect.vPos.x,
		m_tEffect.vPos.y, 0.f);

	m_tEffect.matWorld = m_tEffect.matScale*m_tEffect.matTrans;

	return m_pImplementor->Update();
}

void CWarningEffect::LateUpdate()
{
}

void CWarningEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CWarningEffect::Initialize()
{
	m_tEffect.vSize = { 2.f,2.f,0.f };
	m_tEffect.vPos = { 400.f,300.f,0.f };
	return S_OK;
}

HRESULT CWarningEffect::LateInitialize()
{
	return S_OK;
}

void CWarningEffect::Release()
{
}

CWarningEffect * CWarningEffect::Create(
	CEffectImpl * pImp,
	const D3DXVECTOR3 & vPos,
	const D3DXVECTOR3 & vSize)
{
	CWarningEffect* pInstance = new CWarningEffect(pImp);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);
	pInstance->SetSize(vSize);

	return pInstance;
}
