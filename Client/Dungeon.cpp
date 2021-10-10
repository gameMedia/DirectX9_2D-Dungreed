#include "stdafx.h"
#include "Dungeon.h"
#include "Inventory.h"
#include "AbstractFactory.h"


#include "Dungeon1.h"
#include "Dungeon2.h"
#include "WeaponShopStage.h"
#include "Dungeon3.h"
#include "Restaurant.h"
#include "Dungeon4.h"
#include "ReinForce.h"
#include "BossStage.h"

#include "Player.h"

#include "ActMgr.h"
#include "ActCommand.h"
#include "Jump.h"
#include "MoveLeft.h"
#include "MoveRight.h"
#include "PressDown.h"
#include "Dash.h"
#include "Mouse.h"
#include "Terrain.h"

#include "AnimationImpl.h"
#include "WarningEffect.h"

CDungeon::CDungeon()
{
	eID = ONOFFUIID::OFFINVENTORY;
	Alpha = 255;
}


CDungeon::~CDungeon()
{
	Release();
}

void CDungeon::Update()
{
	if (CKeyMgr::Get_Instance()->KeyDown(KEY_V))
	{
		if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_OnOffInventory() == false)
		{// 닫혀 있다면열어줘라
			eID = ONOFFUIID::ONINVENTORY;
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_OnOff(true); // 열기

		}
		else if (dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Get_OnOffInventory() == true)
		{// 열려있다면 닫아줘라
			eID = ONOFFUIID::OFFINVENTORY;
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_OnOff(false); // 열기
			dynamic_cast<CInventory*>(COnOffUIMgr::Get_Instance()->Get_Inventory())->Set_InvenSetting(false);
		}
	}

	if (CKeyMgr::Get_Instance()->KeyDown(KEY_P))
	{
		Set_Field(FIELDID::RESTAURANT);
	}


	m_pActMgr->Handleler();



	CFieldMgr::Get_Instance()->Update(FieldType, FieldID);
	CUIMgr::Get_Instance()->Update();
	COnOffUIMgr::Get_Instance()->Update(eID);

}

void CDungeon::LateUpdate()
{

	CFieldMgr::Get_Instance()->LateUpdate(FieldType, FieldID);
	CUIMgr::Get_Instance()->LateUpdate();
	COnOffUIMgr::Get_Instance()->LateUpdate(eID);
}

void CDungeon::Render()
{

	CFieldMgr::Get_Instance()->Render(FieldType, FieldID);
	CUIMgr::Get_Instance()->Render();
	COnOffUIMgr::Get_Instance()->Render(eID);

	const TEX_INFO* pTexInfo = m_pTextureMgr->GetTexInfo(
		L"Sceen", L"SceenChange", 0);
	NULL_CHECK(pTexInfo);
	D3DXMATRIX matWorld, matScale, matTrans;
	D3DXVECTOR3 HitPos = { WINCX*0.5f,WINCY*0.5f,0.f };
	D3DXVECTOR3 HitSize = { 1.f,1.f,0.f };
	
	D3DXMatrixScaling(&matScale, HitSize.x, HitSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans,
		HitPos.x,
		HitPos.y, 0.f);

	matWorld = matScale * matTrans;

	float fCenterX = pTexInfo->tImgInfo.Width * 0.5f;
	float fCenterY = pTexInfo->tImgInfo.Height*0.5f;

	
		Alpha -= 10;
	
	if (Alpha <= 0)
	{
		Alpha = 0;
	}

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		pTexInfo->pTexture, nullptr, &D3DXVECTOR3(fCenterX, fCenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(Alpha, 255, 255, 255));

}

HRESULT CDungeon::Initialize()
{
	m_pActMgr = CActMgr::Create();
	FieldType = FIELDTYPE::INDUNGEON;
	FieldID = FIELDID::DUNGEON1;

	HRESULT hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON, FIELDID::DUNGEON1, CAbstractFactory<CDungeon1>::Create());
	hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON, FIELDID::DUNGEON2, CAbstractFactory<CDungeon2>::Create());
	hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON,FIELDID::WEAPON_SHOP, CAbstractFactory<CWeaponShopStage>::Create());
	hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON, FIELDID::DUNGEON3, CAbstractFactory<CDungeon3>::Create());
	hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON, FIELDID::RESTAURANT, CAbstractFactory<CRestaurant>::Create());
	hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON, FIELDID::DUNGEON4, CAbstractFactory<CDungeon4>::Create());
	hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON, FIELDID::REINFORCE, CAbstractFactory<CReinForce>::Create());
	hr = CFieldMgr::Get_Instance()->Add_Field(FIELDTYPE::INDUNGEON, FIELDID::BOSSMAP, CAbstractFactory<CBossStage>::Create());
	ICommandActor* pCommandActor = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player());
	m_pActMgr->Set_Player(pCommandActor); //게임이 생성될때 플레이어의 정보를 ActMgr에 넘겨준다.

	InitKey();

	CSoundMgr::Get_Instance()->PlayBGM(L"MyIceField.wav");

	return S_OK;
}

void CDungeon::Release()
{
}

void CDungeon::InitKey()
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

void CDungeon::Set_Field(FIELDID::ID _field)
{
	FieldID = _field;
	D3DXVECTOR3 playerpos;
	if (FieldID == FIELDID::DUNGEON1)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/Dungeon1Line.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/Dungeon1Tile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_Dungeon1()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MyIceField.wav");
		CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	}
	else if (FieldID == FIELDID::DUNGEON2)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/Dungeon2Line.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/Dungeon2Tile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_Dungeon2()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MyIceField.wav");
		CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	}
	else if (FieldID == FIELDID::WEAPON_SHOP)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/WeaponShopLine.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/WeaponShopTile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_WeaponShopStage()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MyShopStage.wav");
		CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	}
	else if (FieldID == FIELDID::DUNGEON3)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/Dungeon3Line.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/Dungeon3Tile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_Dungeon3()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MyIceField.wav");
		CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	}
	else if (FieldID == FIELDID::RESTAURANT)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/RestaurantLine.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/RestaurantTile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_Restaurant()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);

		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MyFoodShop.wav");
		CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	}
	else if (FieldID == FIELDID::DUNGEON4)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/Dungeon4Line.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/Dungeon4Tile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_Dungeon4()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MyIceField.wav");
	}
	else if (FieldID == FIELDID::REINFORCE)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/ReinForceLine.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/ReinForceTile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_Reinforce()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlayBGM(L"MyShopStage.wav");
		CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	}
	else if (FieldID == FIELDID::BOSSMAP)
	{
		CLineMgr::Get_Instance()->LoadData(L"../Data/BossStageLine.dat");
		dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->LoadTile(L"../Data/BossStageTile.dat");
		CObjMgr::Get_Instance()->Set_Tile(dynamic_cast<CTerrain*>(CObjMgr::Get_Instance()->Get_Terrain())->Get_OptionTile());
		playerpos = CFieldMgr::Get_Instance()->Get_BossStage()->Get_Pos();
		CObjMgr::Get_Instance()->Get_Player()->Set_Pos(playerpos.x, playerpos.y);
		CScrollMgr::Init_m_iScroll(playerpos);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::FIRESWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::NORMALSWORD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::REGENRING);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MAGICSHIELD);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BOMERANG);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::BATITEM);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::WINGBOOT);
		CObjMgr::Get_Instance()->Delete_Object(OBJID::MONEY);
	}

	Alpha = 255;
}

CDungeon * CDungeon::Create()
{
	CDungeon* pInstance = new CDungeon;

	if (FAILED(pInstance->Initialize()))
	{
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
