#include "stdafx.h"
#include "Stage.h"
#include "Terrain.h"



#include "Player.h"
#include "PlayerHpBar.h"
#include "DungeonGate.h"
#include "Monster.h"

#include "AbstractFactory.h"
#include "MonsterSubject.h"
#include "MonsterHpBar.h"

#include "ActMgr.h"
#include "ActCommand.h"
#include "Jump.h"
#include "MoveLeft.h"
#include "MoveRight.h"
#include "PressDown.h"
#include "Dash.h"
#include "Mouse.h"
#include "MyWeaponShop.h"


#include "InventoryIcon.h"
#include "Inventory.h"

#include "Bansh.h"
#include "Bat.h"

#include "FieldMgr.h"

CStage::CStage()
{
	InvenOnOff = false;
	eID = ONOFFUIID::END;
}


CStage::~CStage()
{
	Release();
}

void CStage::Update()
{
	//m_pTerrain->Update();

	if (CKeyMgr::Get_Instance()->KeyDown(KEY_V))
	{
		if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_OnOffInventory() == false)
		{// 닫혀 있다면열어줘라
			eID = ONOFFUIID::ONINVENTORY;
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_OnOff(true); // 열기
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ONOFFINVENTORYSOUND);
			CSoundMgr::Get_Instance()->PlaySound(L"MyOpenInventory.wav", CSoundMgr::ONOFFINVENTORYSOUND);

		}
		else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_OnOffInventory() == true)
		{// 열려있다면 닫아줘라
			eID = ONOFFUIID::OFFINVENTORY;
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_OnOff(false); // 열기
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::ONOFFINVENTORYSOUND);
			CSoundMgr::Get_Instance()->PlaySound(L"MyOpenInventory.wav", CSoundMgr::ONOFFINVENTORYSOUND);
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_InvenSetting(false);
		}
	}

	
	m_pActMgr->Handleler();
	CObjMgr::Get_Instance()->Update();
	CUIMgr::Get_Instance()->Update();
	COnOffUIMgr::Get_Instance()->Update(eID);
	
}

void CStage::LateUpdate()
{
	CObjMgr::Get_Instance()->LateUpdate();
	CUIMgr::Get_Instance()->LateUpdate();
	CScrollMgr::ScrollLock(4800.f, 1200.f); //스크롤 락
	COnOffUIMgr::Get_Instance()->LateUpdate(eID);
	
}

void CStage::Render()
{
	D3DXMatrixScaling(&RenderMap.matScale, RenderMap.vSize.x, RenderMap.vSize.y, 0.f);
	D3DXMatrixTranslation(&RenderMap.matTrans, RenderMap.vPos.x-CScrollMgr::GetScrollPos(0), RenderMap.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);
	RenderMap.matWorld = RenderMap.matScale*RenderMap.matTrans;
	

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Terrain", L"Map", 0);

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(RenderMap.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	CLineMgr::Get_Instance()->Render();
	CObjMgr::Get_Instance()->Render();
	CUIMgr::Get_Instance()->Render();
	COnOffUIMgr::Get_Instance()->Render(eID);
	

}

HRESULT CStage::Initialize()
{
	m_hDC = GetDC(g_hWND);

	CLineMgr::Get_Instance()->LoadData(L"../Data/TownLine2.dat");
	m_pActMgr = CActMgr::Create();

	//HRESULT hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::MTOWN,FIELDID::TOWN,)
	HRESULT hr = CObjMgr::Get_Instance()->Add_Object(OBJID::PLAYER, CAbstractFactory<CPlayer>::Create(400.f, 300.f));
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::MOUSE, CAbstractFactory<CMouse>::Create(700.f, 800.f));

	ICommandActor* pCommandActor = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	m_pActMgr->Set_Player(pCommandActor); //게임이 생성될때 플레이어의 정보를 ActMgr에 넘겨준다.


	//hr = CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER, CAbstractFactory<CMonster>::CreateAIActor(100.f, 100.f));
	//FAILED_CHECK_RETURN(hr, E_FAIL);



	
	
	
	
	
	
	
	
	
	
	
	
	
	//hr = CObjMgr::Get_Instance()->Add_Object(OBJID::UI, CAbstractFactory<CMonsterHpBar>::Create(600.f, 200.f));
	//FAILED_CHECK_RETURN(hr, E_FAIL);
	InitKey();
	
	RenderMap.vPos = { 2400.f,600.f,0.f };
	RenderMap.vSize = { 1.f,1.f,0.f };
	D3DXMatrixIdentity(&RenderMap.matScale);
	D3DXMatrixIdentity(&RenderMap.matTrans);
	D3DXMatrixIdentity(&RenderMap.matWorld);
	// 키 추가
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::TERRAIN, CAbstractFactory<CTerrain>::Create());
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = CObjMgr::Get_Instance()->Add_Object(OBJID::DUNGEONGATE, CAbstractFactory<CDungeonGate>::Create());
	FAILED_CHECK_RETURN(hr, E_FAIL);
	hr = dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/TownTileLast.dat");
	
	CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());


	hr = CUIMgr::Get_Instance()->Add_Object(UIID::INVENTORYICON, CAbstractFactory<CInventoryIcon>::CreateUI());
	FAILED_CHECK_RETURN(hr, E_FAIL);

	CSoundMgr::Get_Instance()->PlayBGM(L"MyTown.wav");
	


	return S_OK;
}

void CStage::Release()
{
	CObjMgr::Get_Instance()->Get_Instance()->Delete_Object(OBJID::BOSS);
	CObjMgr::Get_Instance()->Get_Instance()->Delete_Object(OBJID::ICEPILLAR);
	CSoundMgr::Get_Instance()->StopAll();
//	SafeDelete(m_pActMgr);
}

void CStage::InitKey()
{
	CActCommand* pCommandJump = new CJump;
	m_pActMgr->Set_KeyRegist_Down(VK_SPACE, pCommandJump);
	CActCommand* pCommandMoveLeft = new CMoveLeft;
	CActCommand* pCommandMoveRight = new CMoveRight;
	CActCommand* pCommandDown = new CPressDown;
	CActCommand* pCommandDash = new CDash;
	m_pActMgr->Set_KeyRegist(KEY_A, pCommandMoveLeft);
	m_pActMgr->Set_KeyRegist(KEY_D, pCommandMoveRight);
	m_pActMgr->Set_KeyRegist(KEY_S, pCommandDown);
	m_pActMgr->Set_KeyRegist(KEY_RBUTTON, pCommandDash);

}

CStage * CStage::Create()
{
	CStage* pInstance = new CStage;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
