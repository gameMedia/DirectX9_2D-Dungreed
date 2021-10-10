#include "stdafx.h"
#include "Inventory.h"
#include "Mouse.h"
#include "Player.h"
#include "MainHand.h"
CInventory::CInventory()
	:OnOffInventory(false)
{
	InvenSetting = false;
	InitRevolver = false;
}


CInventory::~CInventory()
{
}


int CInventory::Update()
{
	if (InitRevolver == false)
	{
		Set_HoldingItem(true, 0);
		ITEM_DATA _revolver;
		


		_revolver.iAtt = 20;
		_revolver.iGold = 100;
		_revolver.szName = L"Revolver";
		_revolver.Texture.iIndex = 0;
		_revolver.Texture.wstrObjectKey = L"Revolver";
		_revolver.Texture.wstrStateKey = L"Rare";
		_revolver.vLength = {60.f,60.f,0.f};
		
		const TEX_INFO* Revolver = nullptr;
		Revolver = CTextureMgr::Get_Instance()->GetTexInfo(
			_revolver.Texture.wstrObjectKey,
			_revolver.Texture.wstrStateKey,
			_revolver.Texture.iIndex);

		_revolver.vSize.x = _revolver.vLength.x / Revolver->tImgInfo.Width;
		_revolver.vSize.y = _revolver.vLength.y / Revolver->tImgInfo.Height;
	
		_revolver.eType = ITEMTYPE::ATTACKWEAPON;
		_revolver.eLength = ITEMINTER::LONG;
		_revolver.iBlock = 0;


		Set_InventorySlotItem(_revolver, 0, true);
		InitRevolver = true;
	}

	MakeWorldMatrix();

	m_pPlayer = dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player()); // player을 가져온다.

	Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
	//Mouse->G
	D3DXVECTOR3 MousePos = Mouse->Get_Pos();
	bool GripItem = Mouse->Get_GripItem();
	POINT pt = {};
	pt.x = MousePos.x;
	pt.y = MousePos.y;

	if (PtInRect(&(m_tRect), pt))
	{
		InvenSetting = true;
	}
	else
	{
		InvenSetting = false;
	}

	// 인벤토리 창
	for (int i = 0; i < 20; ++i)
	{

		if (PtInRect(&m_Inven_RECT[i], pt))
		{// 마우스랑 그 해당 아이템이 충돌하고 있을 때		


			if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON)) // 만약 마우스를 눌렀을때
			{
				if (GripItem == false)
				{// 아이템을 들고 있지 않을때
					if (m_Inven_INFO[i].HoldingItem == true)
					{// 인벤토리창에 아이템이 등록된 상태라면
						m_ItemData[i].Griped = true; // 아이템을 잡힌상태로 바꿔준다.
						Mouse->Set_GripItem(true); // 마우스가 아이템을 집은상태로 바꿔주고
						Mouse->Set_GripItemInfo(m_ItemData[i], i); // 그 아이템의 정보를 마우스에 임시 보관해준다.

					}
				}
				else if (GripItem == true)
				{// 이미 들고있는 상태에서  누르면 2가지 경우가 있다
					// 그 인벤자리에 아이템이 있거나 없거나
					if (m_Inven_INFO[i].HoldingItem == false)
					{// 아이템창에 아이템이 등록되어 있지 않다면
						ITEM_DATA SwapData = Mouse->Get_GripItemInfo();
						int SwapItem_Index = Mouse->GripedItem_Index();
						if (i > 13 && SwapData.eType == ITEMTYPE::ATTACKWEAPON)
							continue;
						if (i > 15 && SwapData.eType == ITEMTYPE::SUBWEAPON)
							continue;
						if (i > 11 && i < 14 && SwapData.eType == ITEMTYPE::SUBWEAPON)
							continue;
						if (i > 11 && i < 16 && SwapData.eType == ITEMTYPE::ARTIFACT)
							continue;

						m_ItemData[SwapItem_Index].Griped = false;
						m_Inven_INFO[SwapItem_Index].HoldingItem = false;
						m_ItemData[SwapItem_Index].vPos = m_Inven_INFO[SwapItem_Index].vPos;
						m_ItemData[i] = m_ItemData[SwapItem_Index];
						m_ItemData[i].vPos = m_Inven_INFO[i].vPos;
						m_ItemData[i].Griped = false;

						ClearItemData(SwapItem_Index);

						m_Inven_INFO[i].HoldingItem = true;
						Mouse->Set_GripItem(false);// 통일적으로 아이템을 내려놓고
					}
					else if (m_Inven_INFO[i].HoldingItem == true)
					{// 아이템창에 아이템이 등록되어 있다면
						ITEM_DATA SwapData = Mouse->Get_GripItemInfo();
						int SwapItem_Index = Mouse->GripedItem_Index();
						if (11 < SwapItem_Index && m_ItemData[i].eType != SwapData.eType)
							continue;
						if (i > 13 && SwapData.eType == ITEMTYPE::ATTACKWEAPON)
							continue;
						if (i > 15 && SwapData.eType == ITEMTYPE::SUBWEAPON)
							continue;
						if (i > 11 && i < 14 && SwapData.eType == ITEMTYPE::SUBWEAPON)
							continue;
						if (i > 11 && i < 16 && SwapData.eType == ITEMTYPE::ARTIFACT)
							continue;
						if (i > 13 && SwapData.eType == ITEMTYPE::ATTACKWEAPON)
							continue;
						if (i > 15 && SwapData.eType == ITEMTYPE::SUBWEAPON)
							continue;
						if (i > 11 && i < 14 && SwapData.eType == ITEMTYPE::SUBWEAPON)
							continue;
						if (i > 11 && i < 16 && SwapData.eType == ITEMTYPE::ARTIFACT)
							continue;

						m_ItemData[SwapItem_Index] = m_ItemData[i];
						m_ItemData[SwapItem_Index].vPos = m_Inven_INFO[SwapItem_Index].vPos;
						m_ItemData[i] = SwapData;
						m_ItemData[i].vPos = m_Inven_INFO[i].vPos;
						m_ItemData[SwapItem_Index].Griped = false;
						m_ItemData[i].Griped = false;
						Mouse->Set_GripItem(false);// 통일적으로 아이템을 내려놓고
					}
				}

				CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PICKINVENITEM);
				CSoundMgr::Get_Instance()->PlaySound(L"MyPickUpItem.wav", CSoundMgr::PICKINVENITEM);
			}
		}
	}

	// 주무기 창 세팅

	// 보조무기 창

	// 아티펙트 창




	return OBJ_NOEVENT;
}

void CInventory::LateUpdate()
{
	MoveItem();

	
}

void CInventory::Render()
{
	const TEX_INFO* Inventory = nullptr;
	Inventory = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
	NULL_CHECK(Inventory);

	float CenterX = Inventory->tImgInfo.Width * 0.5f;
	float CenterY = Inventory->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		Inventory->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	for (int j = 0; j < 12; ++j)
	{
		const TEX_INFO* InventorySlot = nullptr;
		InventorySlot = CTextureMgr::Get_Instance()->GetTexInfo(
			m_Inven_INFO[j].Texture.wstrObjectKey, m_Inven_INFO[j].Texture.wstrStateKey, m_Inven_INFO[j].Texture.iIndex);
		NULL_CHECK(InventorySlot);

		float SCenterX = InventorySlot->tImgInfo.Width*0.5f;
		float SCenterY = InventorySlot->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_Inven_INFO[j].matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			InventorySlot->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	const TEX_INFO* Mouse = nullptr;
	Mouse = CTextureMgr::Get_Instance()->GetTexInfo(
		L"Menu", L"Mouse", 1);

	m_pMenuMouse.vSize.x = 30.f / Mouse->tImgInfo.Width;
	m_pMenuMouse.vSize.y = 30.f / Mouse->tImgInfo.Height;


	//// 주무기
	//for (int j = 12; j < 14; ++j)
	//{
	//	const TEX_INFO* MainSlot = nullptr;
	//	MainSlot = CTextureMgr::Get_Instance()->GetTexInfo(
	//		m_Inven_INFO[j].Texture.wstrObjectKey, m_Inven_INFO[j].Texture.wstrStateKey, m_Inven_INFO[j].Texture.iIndex);
	//	NULL_CHECK(MainSlot);

	//	float SCenterX = MainSlot->tImgInfo.Width*0.5f;
	//	float SCenterY = MainSlot->tImgInfo.Height*0.5f;

	//	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_Inven_INFO[j].matWorld));
	//	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
	//		MainSlot->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
	//		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//}
	//// 보조무기
	//for (int j = 14; j < 16; ++j)
	//{
	//	const TEX_INFO* SubSlot = nullptr;
	//	SubSlot = CTextureMgr::Get_Instance()->GetTexInfo(
	//		m_Inven_INFO[j].Texture.wstrObjectKey, m_Inven_INFO[j].Texture.wstrStateKey, m_Inven_INFO[j].Texture.iIndex);
	//	NULL_CHECK(SubSlot);

	//	float SCenterX = SubSlot->tImgInfo.Width*0.5f;
	//	float SCenterY = SubSlot->tImgInfo.Height*0.5f;

	//	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_Inven_INFO[j].matWorld));
	//	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
	//		SubSlot->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
	//		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	//}

	//

	// 특수 무기
	for (int j = 16; j < 20; ++j)
	{
		const TEX_INFO* ArtifactSlot = nullptr;
		ArtifactSlot = CTextureMgr::Get_Instance()->GetTexInfo(
			m_Inven_INFO[j].Texture.wstrObjectKey, m_Inven_INFO[j].Texture.wstrStateKey, m_Inven_INFO[j].Texture.iIndex);
		NULL_CHECK(ArtifactSlot);

		float SCenterX = ArtifactSlot->tImgInfo.Width*0.5f;
		float SCenterY = ArtifactSlot->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_Inven_INFO[j].matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			ArtifactSlot->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	}





	// 인벤아이템
	for (int k = 0; k < 12; ++k)
	{
		if (m_ItemData[k].eatItem == false)
			continue;
		const TEX_INFO* Item = nullptr;
		Item = CTextureMgr::Get_Instance()->GetTexInfo(
			m_ItemData[k].Texture.wstrObjectKey, m_ItemData[k].Texture.wstrStateKey, m_ItemData[k].Texture.iIndex);
		NULL_CHECK(Item);

		float SCenterX = Item->tImgInfo.Width*0.5f;
		float SCenterY = Item->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_ItemData[k].matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			Item->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 주무기 아이템
	for (int k = 12; k < 14; ++k)
	{
		if (m_ItemData[k].eatItem == false)
			continue;
		const TEX_INFO* MainItem = nullptr;
		MainItem = CTextureMgr::Get_Instance()->GetTexInfo(
			m_ItemData[k].Texture.wstrObjectKey, m_ItemData[k].Texture.wstrStateKey, m_ItemData[k].Texture.iIndex);
		NULL_CHECK(MainItem);

		float SCenterX = MainItem->tImgInfo.Width*0.5f;
		float SCenterY = MainItem->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_ItemData[k].matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			MainItem->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	//보조무기 아이템
	for (int k = 14; k < 16; ++k)
	{
		if (m_ItemData[k].eatItem == false)
			continue;
		const TEX_INFO* SubItem = nullptr;
		SubItem = CTextureMgr::Get_Instance()->GetTexInfo(
			m_ItemData[k].Texture.wstrObjectKey, m_ItemData[k].Texture.wstrStateKey, m_ItemData[k].Texture.iIndex);
		NULL_CHECK(SubItem);

		float SCenterX = SubItem->tImgInfo.Width*0.5f;
		float SCenterY = SubItem->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_ItemData[k].matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			SubItem->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	// 특수무기 아이템
	for (int k = 16; k < 20; ++k)
	{
		if (m_ItemData[k].eatItem == false)
			continue;
		const TEX_INFO* ArtifactItem = nullptr;
		ArtifactItem = CTextureMgr::Get_Instance()->GetTexInfo(
			m_ItemData[k].Texture.wstrObjectKey, m_ItemData[k].Texture.wstrStateKey, m_ItemData[k].Texture.iIndex);
		NULL_CHECK(ArtifactItem);

		float SCenterX = ArtifactItem->tImgInfo.Width*0.5f;
		float SCenterY = ArtifactItem->tImgInfo.Height*0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_ItemData[k].matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			ArtifactItem->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	TCHAR szIndex[MIN_STR] = L"";
	D3DXMATRIX TextMatWorld, TextMatScale, TextMatTrans;
	D3DXMatrixScaling(&TextMatScale, 1.f, 1.5f, 0.f);
	D3DXMatrixTranslation(&TextMatTrans, 702.f, 537.f, 0.f);
	TextMatWorld = TextMatScale*TextMatTrans;
	
	swprintf_s(szIndex, L"%d", CPlayer::Money);
	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld);
	CDeviceMgr::Get_Instance()->GetFont()->DrawText(
		CDeviceMgr::Get_Instance()->GetSprite(), szIndex, lstrlen(szIndex),
		nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWND, &pt);
	if (PtInRect(&m_tRect, pt))
	{

		m_pMenuMouse.vPos.x = pt.x;
		m_pMenuMouse.vPos.y = pt.y;

		D3DXMatrixScaling(&m_pMenuMouse.matScale,
			m_pMenuMouse.vSize.x, m_pMenuMouse.vSize.y, 0.f);

		D3DXMatrixTranslation(&m_pMenuMouse.matTrans,
			m_pMenuMouse.vPos.x, m_pMenuMouse.vPos.y, 0.f);

		m_pMenuMouse.matWorld =
			m_pMenuMouse.matScale*m_pMenuMouse.matTrans;

		float MouseX = Mouse->tImgInfo.Width * 0.5f;
		float MouseY = Mouse->tImgInfo.Height * 0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pMenuMouse.matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			Mouse->pTexture, nullptr, &D3DXVECTOR3(MouseX, MouseY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


}

HRESULT CInventory::Initialize()
{
	m_tUnit.vPos = { 650.f,300.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Inventory";
	m_tUnit.Texture.wstrStateKey = L"InventoryBase";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 300.f,600.f,0.f };
	const TEX_INFO* StartButton = nullptr;
	StartButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / StartButton->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / StartButton->tImgInfo.Height;


	int Inven_Cnt_X = 0;
	int Inven_Cnt_Y = 0;
	float Inven_To_Inven_X_Gap = 65.f; //인벤첫번째칸과 두번쨰칸 사이의 갭은 50
	float Inven_To_Inven_Y_Gap = 65.f; //인벤첫번째칸과 다섯번쨰칸 사이의 갭은 50
									   //인벤 1칸과 2칸의 간격차는 50이라고 가정. 
									   // InvenCnt가 1일때 * 50 은 50(두번째칸) , 2일떄 * 50 은 100 (세번쨰칸) 

	for (int i = 0; i < 12; i++)
	{
		m_Inven_INFO[i].Texture.wstrObjectKey = L"InventorySlot";
		m_Inven_INFO[i].Texture.wstrStateKey = L"ItemOff";
		m_Inven_INFO[i].Texture.iIndex = 0;
		m_Inven_INFO[i].vPos.x = 556.f + (Inven_Cnt_X*Inven_To_Inven_X_Gap);
		m_Inven_INFO[i].vPos.y = 330.f + (Inven_Cnt_Y*Inven_To_Inven_Y_Gap);
		m_Inven_INFO[i].vLength.x = 56.f;
		m_Inven_INFO[i].vLength.y = 56.f;
		m_Inven_INFO[i].HoldingItem = FALSE; // 아이템이 들어있지 않다.

		const TEX_INFO* InvenSlot = nullptr;
		InvenSlot = CTextureMgr::Get_Instance()->GetTexInfo(
			m_Inven_INFO[i].Texture.wstrObjectKey, m_Inven_INFO[i].Texture.wstrStateKey, m_Inven_INFO[i].Texture.iIndex);

		m_Inven_INFO[i].vSize.x = m_Inven_INFO[i].vLength.x / InvenSlot->tImgInfo.Width;
		m_Inven_INFO[i].vSize.y = m_Inven_INFO[i].vLength.y / InvenSlot->tImgInfo.Height;

		m_ItemData[i].eatItem = false;
		m_ItemData[i].Griped = false; // 마우스에 잡혀있는가?
		Inven_Cnt_X++;

		if ((i + 1) % 4 == 0)
		{
			Inven_Cnt_Y++;
			Inven_Cnt_X = 0;
		}
	}


	int Attack_Cnt_X = 0;
	float Attack_To_Attack_X_Gap = 130.f;

	// 주무기 창
	for (int j = 12; j < 14; ++j)
	{
		m_Inven_INFO[j].Texture.wstrObjectKey = L"InventorySlot";
		m_Inven_INFO[j].Texture.wstrStateKey = L"AtrifactSlot";
		m_Inven_INFO[j].Texture.iIndex = 0;
		m_Inven_INFO[j].vPos.x = 563.f + (Attack_Cnt_X*Attack_To_Attack_X_Gap);
		m_Inven_INFO[j].vPos.y = 140.f;
		m_Inven_INFO[j].vLength.x = 42.f;
		m_Inven_INFO[j].vLength.y = 60.f;
		m_Inven_INFO[j].HoldingItem = FALSE; // 아이템이 들어있지 않다.

		const TEX_INFO* MainSlot = nullptr;
		MainSlot = CTextureMgr::Get_Instance()->GetTexInfo(
			m_Inven_INFO[j].Texture.wstrObjectKey, m_Inven_INFO[j].Texture.wstrStateKey, m_Inven_INFO[j].Texture.iIndex);

		m_Inven_INFO[j].vSize.x = m_Inven_INFO[j].vLength.x / MainSlot->tImgInfo.Width;
		m_Inven_INFO[j].vSize.y = m_Inven_INFO[j].vLength.y / MainSlot->tImgInfo.Height;

		m_ItemData[j].eatItem = false;
		m_ItemData[j].Griped = false; // 마우스에 잡혀있는가?
		Attack_Cnt_X++;

	}

	int Sub_Cnt_X = 0;
	float Sub_To_Attack_X_Gap = 130.f;
	for (int k = 14; k < 16; ++k)
	{
		m_Inven_INFO[k].Texture.wstrObjectKey = L"InventorySlot";
		m_Inven_INFO[k].Texture.wstrStateKey = L"ItemOff";
		m_Inven_INFO[k].Texture.iIndex = 0;
		m_Inven_INFO[k].vPos.x = 616.f + (Sub_Cnt_X*Sub_To_Attack_X_Gap);
		m_Inven_INFO[k].vPos.y = 140.f;
		m_Inven_INFO[k].vLength.x = 42.f;
		m_Inven_INFO[k].vLength.y = 60.f;
		m_Inven_INFO[k].HoldingItem = FALSE; // 아이템이 들어있지 않다.

		const TEX_INFO* MainSlot = nullptr;
		MainSlot = CTextureMgr::Get_Instance()->GetTexInfo(
			m_Inven_INFO[k].Texture.wstrObjectKey, m_Inven_INFO[k].Texture.wstrStateKey, m_Inven_INFO[k].Texture.iIndex);

		m_Inven_INFO[k].vSize.x = m_Inven_INFO[k].vLength.x / MainSlot->tImgInfo.Width;
		m_Inven_INFO[k].vSize.y = m_Inven_INFO[k].vLength.y / MainSlot->tImgInfo.Height;

		m_ItemData[k].eatItem = false;
		m_ItemData[k].Griped = false; // 마우스에 잡혀있는가?
		Sub_Cnt_X++;
	}

	int Artifact_Cnt_X = 0;
	float Artifact_To_Artifact_X_Gap = 65.f;

	for (int u = 16; u < 20; ++u)
	{
		m_Inven_INFO[u].Texture.wstrObjectKey = L"InventorySlot";
		m_Inven_INFO[u].Texture.wstrStateKey = L"AtrifactSlot";
		m_Inven_INFO[u].Texture.iIndex = 0;
		m_Inven_INFO[u].vPos.x = 560.f + (Artifact_Cnt_X*Artifact_To_Artifact_X_Gap);
		m_Inven_INFO[u].vPos.y = 235.f;
		m_Inven_INFO[u].vLength.x = 60.f;
		m_Inven_INFO[u].vLength.y = 60.f;
		m_Inven_INFO[u].HoldingItem = FALSE; // 아이템이 들어있지 않다.

		const TEX_INFO* MainSlot = nullptr;
		MainSlot = CTextureMgr::Get_Instance()->GetTexInfo(
			m_Inven_INFO[u].Texture.wstrObjectKey, m_Inven_INFO[u].Texture.wstrStateKey, m_Inven_INFO[u].Texture.iIndex);

		m_Inven_INFO[u].vSize.x = m_Inven_INFO[u].vLength.x / MainSlot->tImgInfo.Width;
		m_Inven_INFO[u].vSize.y = m_Inven_INFO[u].vLength.y / MainSlot->tImgInfo.Height;

		m_ItemData[u].eatItem = false;
		m_ItemData[u].Griped = false; // 마우스에 잡혀있는가?
		Artifact_Cnt_X++;
	}

	return S_OK;
}

void CInventory::Release()
{
}

void CInventory::MakeWorldMatrix()
{
	D3DXMatrixScaling(&m_tUnit.matScale, m_tUnit.vSize.x, m_tUnit.vSize.y, 0.f);
	D3DXMatrixTranslation(&m_tUnit.matTrans,
		m_tUnit.vPos.x,
		m_tUnit.vPos.y, 0.f);

	m_tUnit.matWorld = m_tUnit.matScale * m_tUnit.matTrans;

	m_tRect.left = LONG(m_tUnit.vPos.x - (m_tUnit.vLength.x * 0.5f));
	m_tRect.top = LONG(m_tUnit.vPos.y - (LONG(m_tUnit.vLength.y) >> 1));
	m_tRect.right = LONG(m_tUnit.vPos.x + (m_tUnit.vLength.x * 0.5f));
	m_tRect.bottom = LONG(m_tUnit.vPos.y + (m_tUnit.vLength.y * 0.5f));



	for (int j = 0; j < 12; ++j)
	{
		D3DXMatrixScaling(&m_Inven_INFO[j].matScale, m_Inven_INFO[j].vSize.x, m_Inven_INFO[j].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_Inven_INFO[j].matTrans,
			m_Inven_INFO[j].vPos.x,
			m_Inven_INFO[j].vPos.y, 0.f);

		m_Inven_INFO[j].matWorld = m_Inven_INFO[j].matScale * m_Inven_INFO[j].matTrans;
	}
	for (int u = 0; u < 12; ++u)
	{
		if (m_ItemData[u].eatItem == false)
			continue;
		D3DXMatrixScaling(&m_ItemData[u].matScale,
			m_Inven_INFO[u].vSize.x, m_Inven_INFO[u].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_ItemData[u].matTrans,
			m_ItemData[u].vPos.x,
			m_ItemData[u].vPos.y, 0.f);

		m_ItemData[u].matWorld = m_ItemData[u].matScale * m_ItemData[u].matTrans;
	}

	for (int i = 0; i < 12; ++i)
	{
		m_Inven_RECT[i].left = LONG(m_Inven_INFO[i].vPos.x - (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_RECT[i].top = LONG(m_Inven_INFO[i].vPos.y - (LONG(m_Inven_INFO[i].vLength.y) >> 1));
		m_Inven_RECT[i].right = LONG(m_Inven_INFO[i].vPos.x + (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_RECT[i].bottom = LONG(m_Inven_INFO[i].vPos.y + (m_Inven_INFO[i].vLength.y * 0.5f));
	}
	for (int k = 0; k < 12; ++k)
	{
		m_Inven_ItemRECT[k].left = LONG(m_ItemData[k].vPos.x - (m_Inven_INFO[k].vLength.x * 0.5f));
		m_Inven_ItemRECT[k].top = LONG(m_ItemData[k].vPos.y - (LONG(m_Inven_INFO[k].vLength.y) >> 1));
		m_Inven_ItemRECT[k].right = LONG(m_ItemData[k].vPos.x + (m_Inven_INFO[k].vLength.x * 0.5f));
		m_Inven_ItemRECT[k].bottom = LONG(m_ItemData[k].vPos.y + (m_Inven_INFO[k].vLength.y * 0.5f));
	}

	//주무기
	for (int i = 12; i < 14; ++i)
	{
		D3DXMatrixScaling(&m_Inven_INFO[i].matScale, m_Inven_INFO[i].vSize.x, m_Inven_INFO[i].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_Inven_INFO[i].matTrans,
			m_Inven_INFO[i].vPos.x,
			m_Inven_INFO[i].vPos.y, 0.f);

		m_Inven_INFO[i].matWorld = m_Inven_INFO[i].matScale * m_Inven_INFO[i].matTrans;

	}
	for (int u = 12; u < 14; ++u)
	{
		if (m_ItemData[u].eatItem == false)
			continue;
		D3DXMatrixScaling(&m_ItemData[u].matScale,
			m_Inven_INFO[u].vSize.x, m_Inven_INFO[u].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_ItemData[u].matTrans,
			m_ItemData[u].vPos.x,
			m_ItemData[u].vPos.y, 0.f);

		m_ItemData[u].matWorld = m_ItemData[u].matScale * m_ItemData[u].matTrans;
	}

	// 보조무기
	for (int i = 14; i < 16; ++i)
	{
		D3DXMatrixScaling(&m_Inven_INFO[i].matScale, m_Inven_INFO[i].vSize.x, m_Inven_INFO[i].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_Inven_INFO[i].matTrans,
			m_Inven_INFO[i].vPos.x,
			m_Inven_INFO[i].vPos.y, 0.f);

		m_Inven_INFO[i].matWorld = m_Inven_INFO[i].matScale * m_Inven_INFO[i].matTrans;

	}
	for (int u = 14; u < 16; ++u)
	{
		if (m_ItemData[u].eatItem == false)
			continue;
		D3DXMatrixScaling(&m_ItemData[u].matScale,
			m_Inven_INFO[u].vSize.x, m_Inven_INFO[u].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_ItemData[u].matTrans,
			m_ItemData[u].vPos.x,
			m_ItemData[u].vPos.y, 0.f);

		m_ItemData[u].matWorld = m_ItemData[u].matScale * m_ItemData[u].matTrans;
	}

	// 아티팩트
	for (int i = 16; i < 20; ++i)
	{
		D3DXMatrixScaling(&m_Inven_INFO[i].matScale, m_Inven_INFO[i].vSize.x, m_Inven_INFO[i].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_Inven_INFO[i].matTrans,
			m_Inven_INFO[i].vPos.x,
			m_Inven_INFO[i].vPos.y, 0.f);

		m_Inven_INFO[i].matWorld = m_Inven_INFO[i].matScale * m_Inven_INFO[i].matTrans;

	}
	for (int u = 16; u < 20; ++u)
	{
		if (m_ItemData[u].eatItem == false)
			continue;
		D3DXMatrixScaling(&m_ItemData[u].matScale,
			m_Inven_INFO[u].vSize.x, m_Inven_INFO[u].vSize.y, 0.f);
		D3DXMatrixTranslation(&m_ItemData[u].matTrans,
			m_ItemData[u].vPos.x,
			m_ItemData[u].vPos.y, 0.f);

		m_ItemData[u].matWorld = m_ItemData[u].matScale * m_ItemData[u].matTrans;
	}


	for (int i = 12; i < 14; ++i)
	{

		// 주무기 인벤창
		m_Inven_RECT[i].left = LONG(m_Inven_INFO[i].vPos.x - (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_RECT[i].top = LONG(m_Inven_INFO[i].vPos.y - (LONG(m_Inven_INFO[i].vLength.y) >> 1));
		m_Inven_RECT[i].right = LONG(m_Inven_INFO[i].vPos.x + (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_RECT[i].bottom = LONG(m_Inven_INFO[i].vPos.y + (m_Inven_INFO[i].vLength.y * 0.5f));
		// 주무기 아이템 정보
		m_Inven_ItemRECT[i].left = LONG(m_ItemData[i].vPos.x - (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_ItemRECT[i].top = LONG(m_ItemData[i].vPos.y - (LONG(m_Inven_INFO[i].vLength.y) >> 1));
		m_Inven_ItemRECT[i].right = LONG(m_ItemData[i].vPos.x + (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_ItemRECT[i].bottom = LONG(m_ItemData[i].vPos.y + (m_Inven_INFO[i].vLength.y * 0.5f));
	}
	for (int i = 14; i < 16; ++i)
	{

		// 보조무기 인벤창
		m_Inven_RECT[i].left = LONG(m_Inven_INFO[i].vPos.x - (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_RECT[i].top = LONG(m_Inven_INFO[i].vPos.y - (LONG(m_Inven_INFO[i].vLength.y) >> 1));
		m_Inven_RECT[i].right = LONG(m_Inven_INFO[i].vPos.x + (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_RECT[i].bottom = LONG(m_Inven_INFO[i].vPos.y + (m_Inven_INFO[i].vLength.y * 0.5f));

		// 보조무기 아이템정보
		m_Inven_ItemRECT[i].left = LONG(m_ItemData[i].vPos.x - (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_ItemRECT[i].top = LONG(m_ItemData[i].vPos.y - (LONG(m_Inven_INFO[i].vLength.y) >> 1));
		m_Inven_ItemRECT[i].right = LONG(m_ItemData[i].vPos.x + (m_Inven_INFO[i].vLength.x * 0.5f));
		m_Inven_ItemRECT[i].bottom = LONG(m_ItemData[i].vPos.y + (m_Inven_INFO[i].vLength.y * 0.5f));
	}

	// 특수무기 인벤창
	for (int q = 16; q < 20; ++q)
	{
		m_Inven_RECT[q].left = LONG(m_Inven_INFO[q].vPos.x - (m_Inven_INFO[q].vLength.x * 0.5f));
		m_Inven_RECT[q].top = LONG(m_Inven_INFO[q].vPos.y - (LONG(m_Inven_INFO[q].vLength.y) >> 1));
		m_Inven_RECT[q].right = LONG(m_Inven_INFO[q].vPos.x + (m_Inven_INFO[q].vLength.x * 0.5f));
		m_Inven_RECT[q].bottom = LONG(m_Inven_INFO[q].vPos.y + (m_Inven_INFO[q].vLength.y * 0.5f));

		m_Inven_ItemRECT[q].left = LONG(m_ItemData[q].vPos.x - (m_Inven_INFO[q].vLength.x * 0.5f));
		m_Inven_ItemRECT[q].top = LONG(m_ItemData[q].vPos.y - (LONG(m_Inven_INFO[q].vLength.y) >> 1));
		m_Inven_ItemRECT[q].right = LONG(m_ItemData[q].vPos.x + (m_Inven_INFO[q].vLength.x * 0.5f));
		m_Inven_ItemRECT[q].bottom = LONG(m_ItemData[q].vPos.y + (m_Inven_INFO[q].vLength.y * 0.5f));
	}

}

void CInventory::MoveItem()
{
	for (int i = 0; i < 20; ++i)
	{
		if (m_ItemData[i].Griped == true)
		{
			m_ItemData[i].vPos.x = Mouse->Get_Pos().x;
			m_ItemData[i].vPos.y = Mouse->Get_Pos().y;
		}
	}
}

void CInventory::ClearItemData(int index)
{
	m_ItemData[index].eatItem = false;
	m_ItemData[index].Griped = false;
	m_ItemData[index].iAtt = 0;
	m_ItemData[index].iGold = 0;
	m_ItemData[index].iBlock = 0;
	m_ItemData[index].iPlusAtt = 0;
	m_ItemData[index].szName = L"";
	m_ItemData[index].Texture.iIndex = 0;
	m_ItemData[index].Texture.wstrObjectKey = L"";
	m_ItemData[index].Texture.wstrStateKey = L"";
	m_ItemData[index].vPos = m_Inven_INFO[index].vPos;
	m_ItemData[index].Griped = false;

}

CInventory * CInventory::Create()
{
	CInventory* m_pInstacne = new CInventory;
	if (FAILED(m_pInstacne->Initialize()))
	{
		SafeDelete(m_pInstacne);

	}
	return m_pInstacne;
}
void CInventory::Set_InventorySlotItem(ITEM_DATA _itemdata, int iIndex, bool _eat)
{// 아이템 정보를 받아준다.
	m_ItemData[iIndex].eatItem = _eat;
	m_ItemData[iIndex].iAtt = _itemdata.iAtt;
	m_ItemData[iIndex].iGold = _itemdata.iGold;
	m_ItemData[iIndex].szName = _itemdata.szName;
	m_ItemData[iIndex].Texture.iIndex = _itemdata.Texture.iIndex;
	m_ItemData[iIndex].Texture.wstrObjectKey = _itemdata.Texture.wstrObjectKey;
	m_ItemData[iIndex].Texture.wstrStateKey = _itemdata.Texture.wstrStateKey;
	m_ItemData[iIndex].vLength = _itemdata.vLength;
	m_ItemData[iIndex].vSize = _itemdata.vSize;
	m_ItemData[iIndex].vPos = m_Inven_INFO[iIndex].vPos;
	m_ItemData[iIndex].eType = _itemdata.eType;
	m_ItemData[iIndex].eLength = _itemdata.eLength;
	m_ItemData[iIndex].iBlock = _itemdata.iBlock;
}
