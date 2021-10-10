#include "stdafx.h"
#include "HitEffect.h"
#include "EffectImpl.h"

CHitEffect::CHitEffect(CEffectImpl* pImp, D3DXMATRIX* MatTrans)
	:CEffect(pImp),
	m_pParentTrans(MatTrans)
{
}


CHitEffect::~CHitEffect()
{
	Release();
}

int CHitEffect::Update()
{
	CObj::LateInitialize();



	D3DXMatrixScaling(
		&m_tEffect.matScale,
		m_tEffect.vSize.x,
		m_tEffect.vSize.y, 0.f);
	D3DXMatrixRotationZ(&m_tEffect.matRotZ, D3DXToRadian(-45.f));

	m_tEffect.matWorld =
		m_tEffect.matScale *m_tEffect.matRotZ*
		(*m_pParentTrans);



	return m_pImplementor->Update();
}

void CHitEffect::LateUpdate()
{
}

void CHitEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CHitEffect::Initialize()
{
	m_tEffect.vSize = { 3.f, 3.f, 0.f };
	return S_OK;
}

HRESULT CHitEffect::LateInitialize()
{
	return E_NOTIMPL;
}

void CHitEffect::Release()
{
}

CHitEffect * CHitEffect::Create(CEffectImpl * pImp,
	D3DXMATRIX * MatTrans)
{
	NULL_CHECK_RETURN(pImp, nullptr);

	CHitEffect* pInstance = new CHitEffect(pImp,MatTrans);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
