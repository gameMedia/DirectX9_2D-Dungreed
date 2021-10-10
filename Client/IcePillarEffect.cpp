#include "stdafx.h"
#include "IcePillarEffect.h"
#include "EffectImpl.h"

CIcePillarEffect::CIcePillarEffect(CEffectImpl* pImp, D3DXMATRIX pParentMatrix)
	:CEffect(pImp),
	m_pParentMatrix(pParentMatrix),
	m_fRevAngleZ(0.f),m_fRevSpd(0.f)
{
}


CIcePillarEffect::~CIcePillarEffect()
{
	Release();
}

int CIcePillarEffect::Update()
{
	CObj::LateInitialize();

	D3DXMATRIX matScroll;
	D3DXMatrixTranslation(&matScroll,
		-CScrollMgr::GetScrollPos(0),
		-CScrollMgr::GetScrollPos(1),
		0.f);
	//m_fRevAngleZ += m_fRevSpd * CTimeMgr::Get_Instance()->GetDelta();
	++m_fRevAngleZ;
	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans, m_tEffect.vPos.x, m_tEffect.vPos.y, 0.f);

	// 자전 단계에서 공전 각도를 상쇄 시키는 방법 1. (공전 각의 반대로 자전을 먹여라)
	// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRevAngleZ));
	D3DXMatrixRotationZ(&m_tEffect.matPlanetZ, D3DXToRadian(-m_fRevAngleZ));

	// 자전 단계에서 공전 각도를 상쇄 시키는 방법 2 (공전 행렬의 역행렬을 자전 단계에서 먹여라)
	// A행렬 * A행렬의 역행렬 = 항등행렬
	D3DXMatrixInverse(&m_tEffect.matRotZ, nullptr, &m_tEffect.matPlanetZ);

	m_tEffect.matWorld = m_tEffect.matScale * m_tEffect.matTrans * (m_pParentMatrix) * matScroll;


	return m_pImplementor->Update();


}

void CIcePillarEffect::LateUpdate()
{
}

void CIcePillarEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CIcePillarEffect::Initialize()
{
	m_tEffect.vPos = { 0.f, 0.f, 0.f }; // 부모의 x, y로부터 100, 100만큼 떨어져라!
	m_tEffect.vSize = { 1.f, 1.f, 0.f };
	m_tEffect.vLook = { 1.f, 0.f, 0.f };

	m_fRevSpd = 10.f; // 초당 90도씩 
	m_fRevAngleZ = 10;
	return S_OK;
}

HRESULT CIcePillarEffect::LateInitialize()
{
	return E_NOTIMPL;
}

void CIcePillarEffect::Release()
{
}

CIcePillarEffect * CIcePillarEffect::Create(
	CEffectImpl* pImp, D3DXMATRIX pParentMatrix)
{
	NULL_CHECK_RETURN(pImp, nullptr);


	CIcePillarEffect* pInstance = new CIcePillarEffect(pImp, pParentMatrix);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
