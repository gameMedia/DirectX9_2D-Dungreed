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

	// ���� �ܰ迡�� ���� ������ ��� ��Ű�� ��� 1. (���� ���� �ݴ�� ������ �Կ���)
	// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRevAngleZ));
 	D3DXMatrixRotationZ(&m_tEffect.matPlanetZ, D3DXToRadian(-m_fRevAngleZ));

	// ���� �ܰ迡�� ���� ������ ��� ��Ű�� ��� 2 (���� ����� ������� ���� �ܰ迡�� �Կ���)
	// A��� * A����� ����� = �׵����
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
	// �ڽ��� �������� �θ�κ��� ����Ѵ�.
	m_tEffect.vPos = { 0.f, 0.f, 0.f }; // �θ��� x, y�κ��� 100, 100��ŭ ��������!
	m_tEffect.vSize = { 1.f, 1.f, 0.f };
	m_tEffect.vLook = { 1.f, 0.f, 0.f };

	m_fRevSpd = 90.f; // �ʴ� 90���� 
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
