#include "stdafx.h"
#include "Dungeon1.h"
#include "AbstractFactory.h"
#include "Terrain.h"

CDungeon1::CDungeon1()
{
}


CDungeon1::~CDungeon1()
{
}

int CDungeon1::Update()
{
	playerPos = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos;
	CField::LateInitialize();
	CObjMgr::Get_Instance()->Update();
	return 0;
}

void CDungeon1::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
	CScrollMgr::ScrollLock(1500.f, 1050.f); //스크롤 락
}

void CDungeon1::Render()
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
	CObjMgr::Get_Instance()->Render();
}

HRESULT CDungeon1::Initialize()
{

	return S_OK;
}

HRESULT CDungeon1::LateInitialize()
{
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(400.f, 800.f);
	UNIT_INFO playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();
	CScrollMgr::Init_m_iScroll(playerInfo.vPos);
	CLineMgr::Get_Instance()->LoadData(L"../Data/Dungeon1Line.dat");

	HRESULT hr = dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/Dungeon1Tile.dat");
	RenderMap.vSize = { 1.f,1.f,0.f };
	D3DXMatrixIdentity(&RenderMap.matScale);
	D3DXMatrixIdentity(&RenderMap.matTrans);
	D3DXMatrixIdentity(&RenderMap.matWorld);


	

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Terrain", L"Map", 1);
	RenderMap.MAP_SIZEX = 1500.f;
	RenderMap.MAP_SIZEY = 1050.f;

	RenderMap.vSize.x = RenderMap.MAP_SIZEX / pTexInfo->tImgInfo.Width;
	RenderMap.vSize.y = RenderMap.MAP_SIZEY / pTexInfo->tImgInfo.Height;

	CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());

	CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);

	return S_OK;
}

void CDungeon1::Release()
{
}

CDungeon1 * CDungeon1::Create()
{
	CDungeon1* m_pInstance = new CDungeon1;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}

	return m_pInstance;
}

void CDungeon1::Set_Map()
{
}

D3DXVECTOR3 & CDungeon1::Get_Pos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.

	return playerPos;
}
