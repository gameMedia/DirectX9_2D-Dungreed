#include "stdafx.h"
#include "PlayerHungryGauge.h"
#include "Player.h"

CPlayerHungryGauge::CPlayerHungryGauge()
{
}


CPlayerHungryGauge::~CPlayerHungryGauge()
{
}

int CPlayerHungryGauge::Update()
{
	return OBJ_NOEVENT;
}

void CPlayerHungryGauge::LateUpdate()
{
}

void CPlayerHungryGauge::Render()
{
	


	TCHAR szIndex[MIN_STR] = L"";
	TCHAR szIndex1[MIN_STR] = L"";
	const TEX_INFO* LifeBack = nullptr;

	D3DXMatrixScaling(&m_pPlayerLifeBack.matScale,
		m_pPlayerLifeBack.vSize.x, m_pPlayerLifeBack.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_pPlayerLifeBack.matTrans,
		m_pPlayerLifeBack.vPos.x,
		m_pPlayerLifeBack.vPos.y, 0.f);
	m_pPlayerLifeBack.matWorld =
		m_pPlayerLifeBack.matScale*m_pPlayerLifeBack.matTrans;

	LifeBack = m_pTextureMgr->GetTexInfo(
		m_pPlayerLifeBack.Texture.wstrObjectKey, m_pPlayerLifeBack.Texture.wstrStateKey, 0);
	NULL_CHECK(LifeBack);

	float fCenterX = LifeBack->tImgInfo.Width*0.5f;
	float fCenterY = LifeBack->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pPlayerLifeBack.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		LifeBack->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	const TEX_INFO* HpBar = nullptr;

	float HpBarSizeX = ((1.f / CPlayer::HungryFull)*CPlayer::Hungry)*m_tUnit.vSize.x;

	D3DXMatrixScaling(&m_tUnit.matScale,
		HpBarSizeX, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);
	m_tUnit.matWorld =
		m_tUnit.matScale*m_tUnit.matTrans;

	HpBar = m_pTextureMgr->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, 0);
	NULL_CHECK(HpBar);

	fCenterX = HpBar->tImgInfo.Width*0.5f;
	fCenterY = HpBar->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		HpBar->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	const TEX_INFO* LifeWave = nullptr;

	const TEX_INFO* LifeBase = nullptr;

	D3DXMatrixScaling(&m_pPlayerLifeBase.matScale,
		m_pPlayerLifeBase.vSize.x, m_pPlayerLifeBase.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_pPlayerLifeBase.matTrans,
		m_pPlayerLifeBase.vPos.x,
		m_pPlayerLifeBase.vPos.y, 0.f);
	m_pPlayerLifeBase.matWorld =
		m_pPlayerLifeBase.matScale*m_pPlayerLifeBase.matTrans;

	LifeBase = m_pTextureMgr->GetTexInfo(
		m_pPlayerLifeBase.Texture.wstrObjectKey, m_pPlayerLifeBase.Texture.wstrStateKey, 0);
	NULL_CHECK(LifeBase);

	fCenterX = LifeBase->tImgInfo.Width*0.5f;
	fCenterY = LifeBase->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pPlayerLifeBase.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		LifeBase->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));



	////폰트 출력
	D3DXMATRIX TextMatWorld, TextMatScale, TextMatTrans;
	D3DXMatrixScaling(&TextMatScale, 1.f, 1.5f, 0.f);
	D3DXMatrixTranslation(&TextMatTrans, 70.f, 515.f, 0.f);
	TextMatWorld = TextMatScale*TextMatTrans;
	swprintf_s(szIndex, L"%d", CPlayer::Hungry);
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMatrixScaling(&TextMatScale, 1.f, 1.5f, 0.f);
	D3DXMatrixTranslation(&TextMatTrans, 115.f, 515.f, 0.f);
	TextMatWorld = TextMatScale*TextMatTrans;
	swprintf_s(szIndex, L"/");
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMatrixScaling(&TextMatScale, 1.f, 1.5f, 0.f);
	D3DXMatrixTranslation(&TextMatTrans, 150.f, 515.f, 0.f);
	TextMatWorld = TextMatScale*TextMatTrans;


	swprintf_s(szIndex1, L"%d", CPlayer::HungryFull);
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		CDeviceMgr::Get_Instance()->GetSprite(), szIndex1, lstrlen(szIndex1),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	const TEX_INFO* Gogi = nullptr;

	D3DXMatrixScaling(&Icon.matScale,
		Icon.vSize.x, Icon.vSize.y, 0.f);
	D3DXMatrixTranslation(&Icon.matTrans,
		Icon.vPos.x,
		Icon.vPos.y, 0.f);
	Icon.matWorld =
		Icon.matScale*Icon.matTrans;

	Gogi = m_pTextureMgr->GetTexInfo(
		Icon.Texture.wstrObjectKey, Icon.Texture.wstrStateKey, 0);
	NULL_CHECK(Gogi);

	float fCenter1 = Gogi->tImgInfo.Width*0.5f;
	float fCenter2 = Gogi->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(Icon.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		Gogi->pTexture, nullptr, &D3DXVECTOR3(fCenter1, fCenter2, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
}

HRESULT CPlayerHungryGauge::Initialize()
{
	Icon.vPos = { 50.7f,500.5f,0.f };
	Icon.vSize = { 1.f,1.f,0.f };
	Icon.Texture.wstrObjectKey = L"HungryGauge";
	Icon.Texture.wstrStateKey = L"HungryIcon";
	Icon.Texture.iIndex = 0;
	Icon.vLength = { 50.f,50.f,0.f };
	const TEX_INFO* Gogi = nullptr;
	Gogi = CTextureMgr::Get_Instance()->GetTexInfo(
		Icon.Texture.wstrObjectKey,
		Icon.Texture.wstrStateKey,
		Icon.Texture.iIndex);

	Icon.vSize.x = Icon.vLength.x / Gogi->tImgInfo.Width;
	Icon.vSize.y = Icon.vLength.y / Gogi->tImgInfo.Height;

	float Lengthx = 148.f;
	float Lengthy = 64.f;

	m_tUnit.vPos = { 50.7f,500.5f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"HungryGauge";
	m_tUnit.Texture.wstrStateKey = L"HungryGauge";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { Lengthx,Lengthy,0.f };
	const TEX_INFO* HpBar = nullptr;
	HpBar = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / HpBar->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / HpBar->tImgInfo.Height;

	/////////////////////////////////////////

	float Posx = 125.f;
	float Posy = 530.f;
	m_pPlayerLifeBack.vPos = { Posx,Posy,0.f };
	m_pPlayerLifeBack.vSize = { 1.f,1.f,0.f };
	m_pPlayerLifeBack.Texture.wstrObjectKey = L"HpBar";
	m_pPlayerLifeBack.Texture.wstrStateKey = L"PlayerLifeBack";
	m_pPlayerLifeBack.Texture.iIndex = 0;
	m_pPlayerLifeBack.vLength = { Lengthx,Lengthy,0.f };
	const TEX_INFO* LifeBack = nullptr;
	LifeBack = CTextureMgr::Get_Instance()->GetTexInfo(
		m_pPlayerLifeBack.Texture.wstrObjectKey,
		m_pPlayerLifeBack.Texture.wstrStateKey,
		m_pPlayerLifeBack.Texture.iIndex);

	m_pPlayerLifeBack.vSize.x =
		m_pPlayerLifeBack.vLength.x / LifeBack->tImgInfo.Width;
	m_pPlayerLifeBack.vSize.y =
		m_pPlayerLifeBack.vLength.y / LifeBack->tImgInfo.Height;



	/////////////////////////////////////////////

	m_pPlayerLifeBase.vPos = { Posx,Posy,0.f };
	m_pPlayerLifeBase.vSize = { 1.f,1.f,0.f };
	m_pPlayerLifeBase.Texture.wstrObjectKey = L"HpBar";
	m_pPlayerLifeBase.Texture.wstrStateKey = L"PlayerLifeBase";
	m_pPlayerLifeBase.Texture.iIndex = 0;
	m_pPlayerLifeBase.vLength = { Lengthx + 4.f,Lengthy + 4.f,0.f };
	
	const TEX_INFO* LifeBase = nullptr;
	LifeBase = CTextureMgr::Get_Instance()->GetTexInfo(
		m_pPlayerLifeBase.Texture.wstrObjectKey,
		m_pPlayerLifeBase.Texture.wstrStateKey,
		m_pPlayerLifeBase.Texture.iIndex);

	m_pPlayerLifeBase.vSize.x =
		m_pPlayerLifeBase.vLength.x / LifeBase->tImgInfo.Width;
	m_pPlayerLifeBase.vSize.y =
		m_pPlayerLifeBase.vLength.y / LifeBase->tImgInfo.Height;
	return S_OK;
}

void CPlayerHungryGauge::Release()
{
}

CPlayerHungryGauge * CPlayerHungryGauge::Create()
{
	CPlayerHungryGauge* m_pInstance = new CPlayerHungryGauge;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);

	}
	return m_pInstance;
}
