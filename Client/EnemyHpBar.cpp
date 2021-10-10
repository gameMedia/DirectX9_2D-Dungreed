#include "stdafx.h"
#include "EnemyHpBar.h"
#include "Obj.h"

CEnemyHpBar::CEnemyHpBar()
{
}


CEnemyHpBar::~CEnemyHpBar()
{
	Release();
}

int CEnemyHpBar::Update()
{
	m_pMonster = m_pObj->Get_UnitData();
	if (m_pMonster.iHp <= 0)
	{
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;
}

void CEnemyHpBar::LateUpdate()
{
}

void CEnemyHpBar::Render()
{
	const TEX_INFO* HpBar = nullptr;
	m_pMonster = m_pObj->Get_UnitData();
	UNIT_INFO MonsterHpInfo;
	MonsterHpInfo = m_pObj->Get_Info();
	HpBar = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, 0);
	NULL_CHECK(HpBar);
	m_tUnit.vSize.x = MonsterHpInfo.vLength.x / HpBar->tImgInfo.Width;
	float HpBarSizeX = ((1.f / m_pMonster.iMaxHp)*m_pMonster.iHp) * m_tUnit.vSize.x;

	D3DXMatrixScaling(&m_tUnit.matScale,
		HpBarSizeX, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		MonsterHpInfo.vPos.x-(MonsterHpInfo.vLength.x*0.5f)-CScrollMgr::GetScrollPos(0),
		(MonsterHpInfo.vPos.y + (MonsterHpInfo.vLength.y*0.6f))
		-CScrollMgr::GetScrollPos(1), 0.f);
	m_tUnit.matWorld =
		m_tUnit.matScale*m_tUnit.matTrans;


	
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		HpBar->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	const TEX_INFO* LifeWave = nullptr;
}

HRESULT CEnemyHpBar::Initialize()
{
	m_tUnit.vPos = { 15.f,8.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"HpBar";
	m_tUnit.Texture.wstrStateKey = L"LifeBar";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 74.f,16.f,0.f };
	const TEX_INFO* HpBar = nullptr;
	HpBar = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / HpBar->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / HpBar->tImgInfo.Height;

	return S_OK;
}

void CEnemyHpBar::Release()
{
}

CEnemyHpBar * CEnemyHpBar::Create()
{
	CEnemyHpBar* m_pInstance = new CEnemyHpBar;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);

	}
	return m_pInstance;
}
