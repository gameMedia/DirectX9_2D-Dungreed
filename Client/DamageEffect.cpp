#include "stdafx.h"
#include "DamageEffect.h"
#include "EffectImpl.h"

CDamageEffect::CDamageEffect(CEffectImpl* pImp)
	: CEffect(pImp)
{
	CreateTimeUpPos = 0.f;
	UpSpeed = 3.f;
	m_tEffect.vSize = { 1.f, 1.f, 0.f };

	
}


CDamageEffect::~CDamageEffect()
{
	Release();
}

int CDamageEffect::Update()
{
	CreateTimeUpPos += CTimeMgr::Get_Instance()->GetDelta();

	if (CreateTimeUpPos <= 0.2f)
	{ // 크기를 키웠다
		m_tEffect.vSize.x += 2.f * CTimeMgr::Get_Instance()->GetDelta();
		m_tEffect.vSize.y += 2.f * CTimeMgr::Get_Instance()->GetDelta();

	}
	else
	{ // 줄이자
		m_tEffect.vSize.x -= 2.f * CTimeMgr::Get_Instance()->GetDelta();
		m_tEffect.vSize.y -= 2.f * CTimeMgr::Get_Instance()->GetDelta();
		UpSpeed = 100.f;

	}

	if (m_tEffect.vSize.x <= 1.f)
	{
		m_tEffect.vSize.x = 1.f;
		m_tEffect.vSize.y = 1.f;
	}

	m_tEffect.vPos += m_tEffect.vLook * UpSpeed;

	D3DXMatrixScaling(&m_tEffect.matScale,
		m_tEffect.vSize.x,
		m_tEffect.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tEffect.matTrans,
		m_tEffect.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tEffect.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	m_tEffect.matWorld = m_tEffect.matScale * m_tEffect.matTrans;

	m_pImplementor->Update();

	if (CreateTimeUpPos >= 3.f)
		return OBJ_DEAD;
}

void CDamageEffect::LateUpdate()
{
}

void CDamageEffect::Render()
{
	m_pImplementor->Render(&m_tEffect.matWorld);
}

HRESULT CDamageEffect::Initialize()
{
	return S_OK;
}

HRESULT CDamageEffect::LateInitialize()
{
	return S_OK;
}

void CDamageEffect::Release()
{
}

void CDamageEffect::Set_Dir(float _Angle)
{
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tEffect.vPos.x - CScrollMgr::GetScrollPos(0),
		m_tEffect.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	// 각도에 해당하는 방향벡터구하기
	D3DXMATRIX matScale, matRotZ, matWorld;

	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f); // 크기행렬
	D3DXMatrixRotationZ(&matRotZ, D3DXToRadian(-_Angle));

	m_tEffect.vLook = { 0.f,-1.f,0.f };
	matWorld = matScale * matRotZ * m_tEffect.matTrans;
	D3DXVec3TransformNormal(&m_tEffect.vDir, &m_tEffect.vLook, &matWorld);
}

CDamageEffect * CDamageEffect::Create(
	CEffectImpl * pImp,
	const D3DXVECTOR3 & vPos,
	const D3DXVECTOR3 & vShowPos)
{
	CDamageEffect* m_pInstance = new CDamageEffect(pImp);

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	D3DXVECTOR3 Pos = vPos;
	Pos += vShowPos;
	m_pInstance->SetPos(Pos);

	return m_pInstance;
}
