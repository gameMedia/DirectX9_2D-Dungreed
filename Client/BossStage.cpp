#include "stdafx.h"
#include "BossStage.h"
#include "AbstractFactory.h"
#include "Terrain.h"
#include "Boss.h"
CBossStage::CBossStage()
{
}


CBossStage::~CBossStage()
{
}

D3DXVECTOR3 & CBossStage::Get_Pos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return playerPos;
}

int CBossStage::Update()
{
	playerPos = CObjMgr::Get_Instance()->Get_Player()->Get_Info().vPos;
	CField::LateInitialize();
	CObjMgr::Get_Instance()->Update();
	return 0;
}

void CBossStage::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
	CScrollMgr::ScrollLock(2000.f, 1200.f); //스크롤 락
}

void CBossStage::Render()
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

HRESULT CBossStage::Initialize()
{

	return S_OK;
}

HRESULT CBossStage::LateInitialize()
{

	CObjMgr::Get_Instance()->Get_Player()->Set_Pos(174.f, 980.f);
	UNIT_INFO playerInfo = CObjMgr::Get_Instance()->Get_Player()->Get_Info();

	CScrollMgr::Init_m_iScroll(playerInfo.vPos);



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


	HRESULT hr = CObjMgr::Get_Instance()->Add_Object(OBJID::BOSS, CAbstractFactory<CBoss>::Create());
	FAILED_CHECK_RETURN(hr, E_FAIL);

	CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
	CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);


	CSoundMgr::Get_Instance()->StopAll();
	CSoundMgr::Get_Instance()->PlayBGM(L"MyIceBossStage.wav");
	return S_OK;
}

void CBossStage::Release()
{
}

CBossStage * CBossStage::Create()
{
	CBossStage* m_pInstance = new CBossStage;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}

	return m_pInstance;
}

void CBossStage::Set_Map()
{
}
