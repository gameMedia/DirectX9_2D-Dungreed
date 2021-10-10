#include "stdafx.h"
#include "PlayerDashCount.h"
#include "Player.h"

CPlayerDashCount::CPlayerDashCount()
{

}


CPlayerDashCount::~CPlayerDashCount()
{
}

int CPlayerDashCount::Update()
{
	return 0;
}

void CPlayerDashCount::LateUpdate()
{
}

void CPlayerDashCount::Render()
{
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

	float fCenterX = LifeBase->tImgInfo.Width*0.5f;
	float fCenterY = LifeBase->tImgInfo.Height*0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pPlayerLifeBase.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		LifeBase->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int i = 0; i < CPlayer::DashCount; ++i)
	{
		const TEX_INFO* DashCount = nullptr;

		D3DXMatrixScaling(&DashCountRender[i].matScale,
			DashCountRender[i].vSize.x,
			DashCountRender[i].vSize.y, 0.f);
		D3DXMatrixTranslation(&DashCountRender[i].matTrans,
			DashCountRender[i].vPos.x,
			DashCountRender[i].vPos.y, 0.f);
		DashCountRender[i].matWorld =
			DashCountRender[i].matScale*DashCountRender[i].matTrans;

		DashCount = m_pTextureMgr->GetTexInfo(
			DashCountRender[i].Texture.wstrObjectKey,
			DashCountRender[i].Texture.wstrStateKey, 0);
		NULL_CHECK(DashCount);

		float fCenterx1 = DashCount->tImgInfo.Width*0.5f;
		float fCentery1 = DashCount->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(DashCountRender[i].matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			DashCount->pTexture, nullptr, &D3DXVECTOR3(fCenterx1, fCentery1, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}
}

HRESULT CPlayerDashCount::Initialize()
{
	int Inven_Cnt_X = 0;
	int Inven_Cnt_Y = 0;
	float Inven_To_Inven_X_Gap = 40.f; //인벤첫번째칸과 두번쨰칸 사이의 갭은 50
	float Inven_To_Inven_Y_Gap = 105.f; //

	for (int i = 0; i < 12; i++)
	{
		DashCountRender[i].vPos.x = 24.f + (Inven_Cnt_X*Inven_To_Inven_X_Gap);
		DashCountRender[i].vPos.y = 80.f;
		DashCountRender[i].vSize = { 1.f,1.f,0.f };
		DashCountRender[i].Texture.wstrObjectKey = L"DashBar";
		DashCountRender[i].Texture.wstrStateKey = L"DashCount";
		DashCountRender[i].Texture.iIndex = 0;
		DashCountRender[i].vLength = { 36.f,16.f,0.f };
		const TEX_INFO* HpBar = nullptr;
		HpBar = CTextureMgr::Get_Instance()->GetTexInfo(
			DashCountRender[i].Texture.wstrObjectKey,
			DashCountRender[i].Texture.wstrStateKey,
			DashCountRender[i].Texture.iIndex);

		DashCountRender[i].vSize.x =
			DashCountRender[i].vLength.x / HpBar->tImgInfo.Width;
		DashCountRender[i].vSize.y =
			DashCountRender[i].vLength.y / HpBar->tImgInfo.Height;
		Inven_Cnt_X++;
	}


	

	/////////////////////////////////////////

	float Posx = 125.f;
	float Posy = 80.f;


	/////////////////////////////////////////////

	m_pPlayerLifeBase.vPos = { Posx,Posy,0.f };
	m_pPlayerLifeBase.vSize = { 1.f,1.f,0.f };
	m_pPlayerLifeBase.Texture.wstrObjectKey = L"DashBar";
	m_pPlayerLifeBase.Texture.wstrStateKey = L"DashCountBase";
	m_pPlayerLifeBase.Texture.iIndex = 0;
	m_pPlayerLifeBase.vLength = { 252.f, 24.f,0.f };
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

void CPlayerDashCount::Release()
{
}

CPlayerDashCount * CPlayerDashCount::Create()
{
	CPlayerDashCount* m_pInstance = new CPlayerDashCount;
	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);

	}
	return m_pInstance;
}
