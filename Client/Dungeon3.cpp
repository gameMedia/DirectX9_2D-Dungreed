#include "stdafx.h"
#include "Dungeon3.h"
#include "Terrain.h"
#include "AbstractFactory.h"
#include "Bansh.h"
#include "Bat.h"
CDungeon3::CDungeon3()
{
}


CDungeon3::~CDungeon3()
{
}

D3DXVECTOR3 & CDungeon3::Get_Pos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return playerPos;
}

int CDungeon3::Update()
{
	playerPos = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos;
	CField::LateInitialize();
	CObjMgr::Get_Instance()->Update();
	return 0;
}

void CDungeon3::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
	CScrollMgr::ScrollLock(2000.f, 1200.f); //스크롤 락
}

void CDungeon3::Render()
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

HRESULT CDungeon3::Initialize()
{
	
	return S_OK;
}

HRESULT CDungeon3::LateInitialize()
{
	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(174.f, 980.f);
	UNIT_INFO playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	CScrollMgr::Init_m_iScroll(playerInfo.vPos);
	HRESULT hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BANSH, CAbstractFactory<CBansh>::Create(585, 500));
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BANSH, CAbstractFactory<CBansh>::Create(485, 270));
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BANSH, CAbstractFactory<CBansh>::Create(1785, 500));
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BANSH, CAbstractFactory<CBansh>::Create(1785, 270));
	FAILED_CHECK_RETURN(hr, E_FAIL);

	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BAT, CAbstractFactory<CBat>::Create(1585, 270));
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BAT, CAbstractFactory<CBat>::Create(1185, 470));
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BAT, CAbstractFactory<CBat>::Create(885, 670));
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BAT, CAbstractFactory<CBat>::Create(585, 270));
	FAILED_CHECK_RETURN(hr, E_FAIL);


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

void CDungeon3::Release()
{
}

CDungeon3 * CDungeon3::Create()
{
	CDungeon3* m_pInstance = new CDungeon3;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}

	return m_pInstance;
}

void CDungeon3::Set_Map()
{
}
