#include "stdafx.h"
#include "BuffEffect.h"
#include "EffectImpl.h"

CBuffEffect::CBuffEffect(CEffectImpl* pImp, const D3DXMATRIX* pParentMatrix)
	: CEffect(pImp),
	m_pParentMatrix(pParentMatrix),
	m_fRevAngleZ(0.f), m_fRevSpd(0.f)
{
}


CBuffEffect::~CBuffEffect()
{
	Release();
}

int CBuffEffect::Update()
{
	CObj::LateInitialize();

	
	m_fRevAngleZ += m_fRevSpd * CTimeMgr::Get_Instance()->GetDelta();
	//++m_fRevAngleZ;
	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans, m_tEffect.vPos.x, m_tEffect.vPos.y, 0.f);

	// 자전 단계에서 공전 각도를 상쇄 시키는 방법 1. (공전 각의 반대로 자전을 먹여라)
	// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRevAngleZ));
 	D3DXMatrixRotationZ(&m_tEffect.matPlanetZ, D3DXToRadian(-m_fRevAngleZ));

	// 자전 단계에서 공전 각도를 상쇄 시키는 방법 2 (공전 행렬의 역행렬을 자전 단계에서 먹여라)
	// A행렬 * A행렬의 역행렬 = 항등행렬
	D3DXMatrixInverse(&m_tEffect.matRotZ, nullptr, &m_tEffect.matPlanetZ);

	m_tEffect.matWorld = m_tEffect.matScale  *m_tEffect.matRotZ * m_tEffect.matTrans  * m_tEffect.matPlanetZ * (*m_pParentMatrix);


	return m_pImplementor->Update();
}

void CBuffEffect::LateUpdate()
{
}

void CBuffEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CBuffEffect::Initialize()
{
	// 자식의 포지션은 부모로부터 계산한다.
	m_tEffect.vPos = { 0.f, 0.f, 0.f }; // 부모의 x, y로부터 100, 100만큼 떨어져라!
	m_tEffect.vSize = { 1.f, 1.f, 0.f };
	m_tEffect.vLook = { 1.f, 0.f, 0.f };

	m_fRevSpd = 90.f; // 초당 90도씩 
	//m_fRevAngleZ = 10;
	return S_OK;
}

HRESULT CBuffEffect::LateInitialize()
{
	return S_OK;
}

void CBuffEffect::Release()
{
}

CBuffEffect* CBuffEffect::Create(
	CEffectImpl * pImp, const D3DXMATRIX* pParentMatrix)
{
	NULL_CHECK_RETURN(pImp, nullptr);
	NULL_CHECK_RETURN(pParentMatrix, nullptr);

	CBuffEffect* pInstance = new CBuffEffect(pImp, pParentMatrix);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
