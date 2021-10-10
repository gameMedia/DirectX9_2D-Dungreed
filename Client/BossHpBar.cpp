#include "stdafx.h"
#include "BossHpBar.h"
#include "Obj.h"

CBossHpBar::CBossHpBar()
{
}


CBossHpBar::~CBossHpBar()
{
	Release();
}

int CBossHpBar::Update()
{
	m_pMonster = m_pObj->Get_UnitData();
	if (m_pMonster.iHp <= 0)
	{
		
	}
	return OBJ_NOEVENT;
}

void CBossHpBar::LateUpdate()
{
}

void CBossHpBar::Render()
{
	const TEX_INFO* Basee = nullptr;
	Basee = CTextureMgr::Get_Instance()->GetTexInfo(
		HpBarBase.Texture.wstrObjectKey,
		HpBarBase.Texture.wstrStateKey,
		HpBarBase.Texture.iIndex);

	D3DXMatrixScaling(&HpBarBase.matScale,
		HpBarBase.vSize.x, HpBarBase.vSize.y, 0.f);
	D3DXMatrixTranslation(&HpBarBase.matTrans,
		HpBarBase.vPos.x,
		HpBarBase.vPos.y,
		0.f);
	HpBarBase.matWorld =
		HpBarBase.matScale*HpBarBase.matTrans;

	const TEX_INFO* Icon = nullptr;
	Icon = CTextureMgr::Get_Instance()->GetTexInfo(
		BossIcon.Texture.wstrObjectKey,
		BossIcon.Texture.wstrStateKey,
		BossIcon.Texture.iIndex);

	D3DXMatrixScaling(&BossIcon.matScale,
		BossIcon.vSize.x, BossIcon.vSize.y, 0.f);
	D3DXMatrixTranslation(&BossIcon.matTrans,
		BossIcon.vPos.x,
		BossIcon.vPos.y,
		0.f);
	BossIcon.matWorld =
		BossIcon.matScale*BossIcon.matTrans;


	const TEX_INFO* HpBar = nullptr;
	m_pMonster = m_pObj->Get_UnitData();
	UNIT_INFO MonsterHpInfo;
	MonsterHpInfo = m_pObj->Get_Info();
	HpBar = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey, 0);
	NULL_CHECK(HpBar);
	
	float HpBarSizeX = ((1.f / m_pMonster.iMaxHp)*m_pMonster.iHp) * m_tUnit.vSize.x;

	D3DXMatrixScaling(&m_tUnit.matScale,
		HpBarSizeX, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y,
		0.f);
	m_tUnit.matWorld =
		m_tUnit.matScale*m_tUnit.matTrans;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(HpBarBase.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		Basee->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(BossIcon.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		Icon->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (m_pMonster.iHp > 0)
	{
		
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		HpBar->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));


	}
	const TEX_INFO* LifeWave = nullptr;
}

HRESULT CBossHpBar::Initialize()
{
	m_tUnit.vPos = { 250.f,480.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"HpBar";
	m_tUnit.Texture.wstrStateKey = L"LifeBar";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 350.f,95.f,0.f };
	const TEX_INFO* HpBar = nullptr;
	HpBar = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / HpBar->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / HpBar->tImgInfo.Height;

	
	HpBarBase.vPos = { 150.f,480.f,0.f };
	HpBarBase.vSize = { 1.f,1.f,0.f };
	HpBarBase.Texture.wstrObjectKey = L"Boss";
	HpBarBase.Texture.wstrStateKey = L"HpBarBase";
	HpBarBase.Texture.iIndex = 0;
	HpBarBase.vLength = { 450.f,100.f,0.f };
	
	const TEX_INFO* Basee = nullptr;
	Basee = CTextureMgr::Get_Instance()->GetTexInfo(
		HpBarBase.Texture.wstrObjectKey,
		HpBarBase.Texture.wstrStateKey,
		HpBarBase.Texture.iIndex);

	HpBarBase.vSize.x = HpBarBase.vLength.x / Basee->tImgInfo.Width;
	HpBarBase.vSize.y = HpBarBase.vLength.y / Basee->tImgInfo.Height;

	BossIcon.vPos = { 170.f,493.f,0.f };
	BossIcon.vSize = { 1.f,1.f,0.f };
	BossIcon.Texture.wstrObjectKey = L"Boss";
	BossIcon.Texture.wstrStateKey = L"BossIdle";
	BossIcon.Texture.iIndex = 0;
	BossIcon.vLength = { 80.f,70.f,0.f };
	const TEX_INFO* Icon = nullptr;
	Icon = CTextureMgr::Get_Instance()->GetTexInfo(
		BossIcon.Texture.wstrObjectKey,
		BossIcon.Texture.wstrStateKey,
		BossIcon.Texture.iIndex);

	BossIcon.vSize.x = BossIcon.vLength.x / Icon->tImgInfo.Width;
	BossIcon.vSize.y = BossIcon.vLength.y / Icon->tImgInfo.Height;
	return S_OK;
}

void CBossHpBar::Release()
{
}

CBossHpBar * CBossHpBar::Create()
{
	CBossHpBar* m_pInstance = new CBossHpBar;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);

	}
	return m_pInstance;
}
