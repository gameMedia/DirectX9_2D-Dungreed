#include "stdafx.h"
#include "PlayerHpBar.h"
#include "Obj.h"

CPlayerHpBar::CPlayerHpBar()
{
}


CPlayerHpBar::~CPlayerHpBar()
{
}

int CPlayerHpBar::Update()
{
	return OBJ_NOEVENT;
}

void CPlayerHpBar::LateUpdate()
{
	CUI::FrameMove(1.f);
}

void CPlayerHpBar::Render()
{
	m_pPlayer = CObjMgr::Get_Instance()->Get_Player()->Get_UnitData();
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

	float HpBarSizeX = ((1.f / m_pPlayer.iMaxHp)*m_pPlayer.iHp)*m_tUnit.vSize.x;

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
	

	
	
	
	if (m_pPlayer.iHp < m_pPlayer.iMaxHp&& m_pPlayer.iHp >1)
	{ // 피가 떨어지면 출력되라
		D3DXMatrixScaling(&m_LifeWave.matScale,
			m_LifeWave.vSize.x, m_LifeWave.vSize.y, 0.f);
		D3DXMatrixTranslation(&m_LifeWave.matTrans,
			(m_tUnit.vPos.x+(148/m_pPlayer.iMaxHp)*m_pPlayer.iHp)-8.f,
			m_tUnit.vPos.y+12.f, 0.f);
		m_LifeWave.matWorld =
			m_LifeWave.matScale*m_LifeWave.matTrans;

		LifeWave = m_pTextureMgr->GetTexInfo(
			m_LifeWave.Texture.wstrObjectKey,
			m_LifeWave.Texture.wstrStateKey,
			(int)m_tFrame.fCurFrame);
		NULL_CHECK(LifeWave);

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_LifeWave.matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			LifeWave->pTexture, nullptr, nullptr,
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
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

	if (m_pPlayer.iHp <= 0)
	{
		m_pPlayer.iHp = 1;
	}

	////폰트 출력
	D3DXMATRIX TextMatWorld, TextMatScale,TextMatTrans;
	D3DXMatrixScaling(&TextMatScale, 1.f, 1.5f, 0.f);
	D3DXMatrixTranslation(&TextMatTrans, 20.f, 15.f, 0.f);
	TextMatWorld = TextMatScale*TextMatTrans;
	swprintf_s(szIndex, L"%d", m_pPlayer.iHp);
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMatrixScaling(&TextMatScale, 1.f, 1.5f, 0.f);
	D3DXMatrixTranslation(&TextMatTrans, 65.f, 15.f, 0.f);
	TextMatWorld = TextMatScale*TextMatTrans;
	swprintf_s(szIndex, L"/");
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));


	D3DXMatrixScaling(&TextMatScale, 1.f, 1.5f, 0.f);
	D3DXMatrixTranslation(&TextMatTrans, 100.f, 15.f, 0.f);
	TextMatWorld = TextMatScale*TextMatTrans;
	
	
	swprintf_s(szIndex1, L"%.0lf", m_pPlayer.iMaxHp);
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		CDeviceMgr::Get_Instance()->GetSprite(), szIndex1, lstrlen(szIndex1),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

}

HRESULT CPlayerHpBar::Initialize()
{
	float Lengthx = 148.f;
	float Lengthy = 64.f;

	m_tUnit.vPos = { 0.7f,0.5f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"HpBar";
	m_tUnit.Texture.wstrStateKey = L"LifeBar";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { Lengthx,Lengthy,0.f };
	const TEX_INFO* HpBar = nullptr;
	HpBar = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / HpBar->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / HpBar->tImgInfo.Height;

	/////////////////////////////////////////

	float Posx = 75.f;
	float Posy = 30.f;
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
	m_pPlayerLifeBase.vLength = { Lengthx+4.f,Lengthy+4.f,0.f };
	const TEX_INFO* LifeBase = nullptr;
	LifeBase = CTextureMgr::Get_Instance()->GetTexInfo(
		m_pPlayerLifeBase.Texture.wstrObjectKey,
		m_pPlayerLifeBase.Texture.wstrStateKey,
		m_pPlayerLifeBase.Texture.iIndex);

	m_pPlayerLifeBase.vSize.x = 
		m_pPlayerLifeBase.vLength.x / LifeBase->tImgInfo.Width;
	m_pPlayerLifeBase.vSize.y = 
		m_pPlayerLifeBase.vLength.y / LifeBase->tImgInfo.Height;
	


	/////////////////////////////////////
	float WaveLengthX = 20.f;
	float WaveLengthY = 40.f;
	m_LifeWave.vPos = { Posx,Posy,0.f };
	m_LifeWave.vSize = { 1.f,1.f,0.f };
	m_LifeWave.Texture.wstrObjectKey = L"HpBar";
	m_LifeWave.Texture.wstrStateKey = L"LifeWave";
	m_LifeWave.Texture.iIndex = 0;
	m_LifeWave.vLength = { WaveLengthX,WaveLengthY,0.f };
	const TEX_INFO* LifeWave = nullptr;
	LifeWave = CTextureMgr::Get_Instance()->GetTexInfo(
		m_LifeWave.Texture.wstrObjectKey,
		m_LifeWave.Texture.wstrStateKey,
		m_LifeWave.Texture.iIndex);

	m_LifeWave.vSize.x = 
		m_LifeWave.vLength.x / LifeWave->tImgInfo.Width;
	m_LifeWave.vSize.y = 
		m_LifeWave.vLength.y / LifeWave->tImgInfo.Height;
	
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 7.f;
	return S_OK;
}

void CPlayerHpBar::Release()
{
}

CPlayerHpBar * CPlayerHpBar::Create()
{
	CPlayerHpBar* m_pInstance = new CPlayerHpBar;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);

	}
	return m_pInstance;
}
