#include "stdafx.h"
#include "StopEffect.h"
#include "EffectImpl.h"

CStopEffect::CStopEffect(CEffectImpl* pImp, D3DXMATRIX* pParentMatrix)
	:CEffect(pImp),
	m_pParentMatrix(pParentMatrix),
	m_fRevAngleZ(0.f), m_fRevSpd(0.f)
{
}


CStopEffect::~CStopEffect()
{
	Release();
}

int CStopEffect::Update()
{
	CObj::LateInitialize();


	//m_fRevAngleZ += m_fRevSpd * CTimeMgr::Get_Instance()->GetDelta();
	++m_fRevAngleZ;
	D3DXMatrixScaling(&m_tEffect.matScale, m_tEffect.vSize.x, m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans, 
		m_tEffect.vPos.x , 
		m_tEffect.vPos.y , 0.f);

	// ���� �ܰ迡�� ���� ������ ��� ��Ű�� ��� 1. (���� ���� �ݴ�� ������ �Կ���)
	// D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(m_fRevAngleZ));
	D3DXMatrixRotationZ(&m_tEffect.matPlanetZ, D3DXToRadian(-m_fRevAngleZ));

	// ���� �ܰ迡�� ���� ������ ��� ��Ű�� ��� 2 (���� ����� ������� ���� �ܰ迡�� �Կ���)
	// A��� * A����� ����� = �׵����
	D3DXMatrixInverse(&m_tEffect.matRotZ, nullptr, &m_tEffect.matPlanetZ);

	m_tEffect.matWorld = m_tEffect.matScale  *m_tEffect.matRotZ * m_tEffect.matTrans  * m_tEffect.matPlanetZ * *(m_pParentMatrix);


	return m_pImplementor->Update();
}

void CStopEffect::LateUpdate()
{
}

void CStopEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CStopEffect::Initialize()
{
	m_tEffect.vPos = { 0.f, 0.f, 0.f }; // �θ��� x, y�κ��� 100, 100��ŭ ��������!
	m_tEffect.vSize = { 1.f, 1.f, 0.f };
	m_tEffect.vLook = { 1.f, 0.f, 0.f };

	m_fRevSpd = 10.f; // �ʴ� 90���� 
	m_fRevAngleZ = 10;
	return S_OK;
}

HRESULT CStopEffect::LateInitialize()
{
	return E_NOTIMPL;
}

void CStopEffect::Release()
{
}

CStopEffect * CStopEffect::Create(
	CEffectImpl * pImp, D3DXMATRIX *pParentMatrix)
{

	NULL_CHECK_RETURN(pImp, nullptr);
	

	CStopEffect* pInstance = new CStopEffect(pImp, pParentMatrix);

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
