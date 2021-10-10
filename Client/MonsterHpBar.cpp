#include "stdafx.h"
#include "MonsterHpBar.h"
#include "MonsterObserver.h"

CMonsterHpBar::CMonsterHpBar()
	:m_iMonsterHp(0),m_pObserver(nullptr)
{
}


CMonsterHpBar::~CMonsterHpBar()
{
	Release();
}

int CMonsterHpBar::Update()
{
	CObj::LateInitialize();
	return 0;
}

void CMonsterHpBar::LateUpdate()
{
}

void CMonsterHpBar::Render()
{
	TCHAR szPrint[MIN_STR] = L"";
	m_iMonsterHp = m_pObserver->GetMonsterHp();

	swprintf_s(szPrint, L"HP: %d", m_iMonsterHp);

	D3DXVECTOR3 POS = m_pObserver->GetMonsterPos();
	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, POS.x, POS.y, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
	m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(), szPrint, lstrlen(szPrint),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 0, 0));
}

HRESULT CMonsterHpBar::Initialize()
{
	m_pObserver = CMonsterObserver::Create();

	return S_OK;
}

HRESULT CMonsterHpBar::LateInitialize()
{
	return S_OK;
}

void CMonsterHpBar::Release()
{
	SafeDelete(m_pObserver);
}

CMonsterHpBar * CMonsterHpBar::Create()
{
	CMonsterHpBar* m_pInstance = new CMonsterHpBar;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	
	}
	return m_pInstance;
}
