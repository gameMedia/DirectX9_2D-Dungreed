#include "stdafx.h"
#include "WeaponShopStage.h"
#include "Terrain.h"
#include "AbstractFactory.h"
#include "MyWeaponShop.h"

CWeaponShopStage::CWeaponShopStage()
{
}


CWeaponShopStage::~CWeaponShopStage()
{
}

D3DXVECTOR3 & CWeaponShopStage::Get_Pos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return playerPos;
}

int CWeaponShopStage::Update()
{
	CField::LateInitialize();
	playerPos = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos;
	CObjMgr::Get_Instance()->Update();
	if (pwI == nullptr)
		return 0;
	pwI->Update();
	return 0;
}

void CWeaponShopStage::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
	CScrollMgr::ScrollLock(2000.f, 1200.f); //스크롤 락
	if (pwI == nullptr)
		return;
	pwI->LateUpdate();
}

void CWeaponShopStage::Render()
{
	D3DXMatrixScaling(&RenderMap.matScale, RenderMap.vSize.x, RenderMap.vSize.y, 0.f);
	D3DXMatrixTranslation(&RenderMap.matTrans, RenderMap.vPos.x - CScrollMgr::GetScrollPos(0), RenderMap.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);
	RenderMap.matWorld = RenderMap.matScale*RenderMap.matTrans;


	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Terrain", L"Map", 1);
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(RenderMap.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, nullptr,
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CLineMgr::Get_Instance()->Render();

	D3DXMatrixScaling(&ShopImage.matScale,
		ShopImage.vSize.x,
		ShopImage.vSize.y, 0.f);
	D3DXMatrixTranslation(&ShopImage.matTrans,
		ShopImage.vPos.x - CScrollMgr::GetScrollPos(0),
		ShopImage.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	ShopImage.matWorld =
		ShopImage.matScale *
		ShopImage.matTrans;

	const TEX_INFO* Shopn = nullptr;
	Shopn = CTextureMgr::Get_Instance()->GetTexInfo(
	ShopImage.Texture.wstrObjectKey,
	ShopImage.Texture.wstrStateKey,
	ShopImage.Texture.iIndex);
	NULL_CHECK(Shopn);

	float CenterX1 = Shopn->tImgInfo.Width * 0.5f;
	float CenterY1 = Shopn->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(ShopImage.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
	Shopn->pTexture, nullptr, &D3DXVECTOR3(CenterX1, CenterY1, 0.f),
	nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	CObjMgr::Get_Instance()->Render();
	if (pwI != nullptr)
		pwI->Render();
}

HRESULT CWeaponShopStage::Initialize()
{
	return S_OK;
}

HRESULT CWeaponShopStage::LateInitialize()
{
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(174.f, 980.f);
	UNIT_INFO playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	CScrollMgr::Init_m_iScroll(playerInfo.vPos);

	ShopImage.vPos = { 1270.f,700.f,0.f };
	ShopImage.Texture.wstrObjectKey = L"Shop";
	ShopImage.Texture.wstrStateKey = L"ShopNormal";
	ShopImage.Texture.iIndex = 0;

	const TEX_INFO* Shopn = nullptr;
	Shopn = CTextureMgr::Get_Instance()->GetTexInfo(
		ShopImage.Texture.wstrObjectKey,
		ShopImage.Texture.wstrStateKey,
		ShopImage.Texture.iIndex);

	ShopImage.vLength = { 900.f,700.f,0.f };

	ShopImage.vSize.x = ShopImage.vLength.x / Shopn->tImgInfo.Width;
	ShopImage.vSize.y = ShopImage.vLength.y / Shopn->tImgInfo.Height;
	
	CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	
	RenderMap.vSize = { 1.f,1.f,0.f };
	D3DXMatrixIdentity(&RenderMap.matScale);
	D3DXMatrixIdentity(&RenderMap.matTrans);
	D3DXMatrixIdentity(&RenderMap.matWorld);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Terrain", L"Map", 1);
	RenderMap.MAP_SIZEX = 2000.f;
	RenderMap.MAP_SIZEY = 1200.f;

	RenderMap.vSize.x = RenderMap.MAP_SIZEX / pTexInfo->tImgInfo.Width;
	RenderMap.vSize.y = RenderMap.MAP_SIZEY / pTexInfo->tImgInfo.Height;

	//CSoundMgr::Get_Instance()->StopAll();
	//CSoundMgr::Get_Instance()->PlayBGM(L"MyShopStage.wav");
	
	pwI = CMyWeaponShop::Create();
	
	return S_OK;
}

void CWeaponShopStage::Release()
{
}

CWeaponShopStage * CWeaponShopStage::Create()
{
	CWeaponShopStage* m_pInstance = new CWeaponShopStage;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}

	return m_pInstance;
}

void CWeaponShopStage::Set_Map()
{
}
