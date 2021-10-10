#include "stdafx.h"
#include "DungeonGate.h"
#include "NormalEffect.h"
#include "FieldChangeAni.h"

CDungeonGate::CDungeonGate()
{
}


CDungeonGate::~CDungeonGate()
{
	Release();
}

int CDungeonGate::Update()
{
	CObj::UpdateRect();
	CObj::MakeWorldMatrix();
	if (m_bIsDead == true)
	{// ¸¸¾à Á×À»½Ã
		CFieldChangeAni* pImpl = CFieldChangeAni::Create(
			L"DungeonGate", L"Gate", FRAME(0.f, 27.f), 0.5f);

		CNormalEffect* pEffect = CNormalEffect::Create(pImpl, m_tUnit.vPos);
		CObjMgr::Get_Instance()->Add_Object(OBJID::EFFECT, pEffect);

		return OBJ_DEAD;
		
	}


	return OBJ_NOEVENT;
}


void CDungeonGate::LateUpdate()
{                    
}

void CDungeonGate::Render()
{
}

HRESULT CDungeonGate::Initialize()
{
	m_tUnit.vPos = { 2967.f,939.f,0.f };
	m_tUnit.vLength = { 300.f,200.f,0.f };
	return S_OK;
}

void CDungeonGate::Release()
{
}

void CDungeonGate::RectColl(CObj* pObj)
{
	m_bIsDead = true;
}

CDungeonGate * CDungeonGate::Create()
{
	CDungeonGate* m_pInstance = new CDungeonGate;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
