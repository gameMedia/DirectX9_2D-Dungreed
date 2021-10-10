#include "stdafx.h"
#include "LongAttackEffect.h"
#include "EffectImpl.h"

CLongAttackEffect::CLongAttackEffect(CEffectImpl* pImp)
	:CEffect(pImp)
{
}


CLongAttackEffect::~CLongAttackEffect()
{
	Release();
}

int CLongAttackEffect::Update()
{
	CObj::LateInitialize();

	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixRotationZ(&m_tEffect.matRotZ, D3DXToRadian(-45.f));
	D3DXMatrixTranslation(&m_tEffect.matTrans,
		m_tEffect.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tEffect.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tEffect.matWorld = m_tEffect.matScale*m_tEffect.matRotZ*m_tEffect.matTrans;

	return m_pImplementor->Update();
}

void CLongAttackEffect::LateUpdate()
{
}

void CLongAttackEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CLongAttackEffect::Initialize()
{
	m_tEffect.vSize = { 3.f,3.f,0.f };
	m_tEffect.vLook = { 1.f,0.f,0.f };
	return S_OK;
}

HRESULT CLongAttackEffect::LateInitialize()
{
	return S_OK;
}

void CLongAttackEffect::Release()
{
}

CLongAttackEffect * CLongAttackEffect::Create(
	CEffectImpl * pImp,
	const D3DXVECTOR3 & vPos,
	const D3DXVECTOR3 & vSize)
{
	CLongAttackEffect* pInstance = new CLongAttackEffect(pImp);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->SetPos(vPos);
	pInstance->SetSize(vSize);

	return pInstance;
}
