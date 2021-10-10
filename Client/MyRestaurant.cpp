#include "stdafx.h"
#include "MyRestaurant.h"
#include "Mouse.h"
#include "Inventory.h"
#include "Player.h"

bool CMyRestaurant::InnSetting = false;

CMyRestaurant::CMyRestaurant()
{
	ClickedNPC = false; // ClickedNPC가 되었는가 체크하자
	WeaponShopClicked = false;
	SetUpShopChang = false; // 상점창이 열렸는가?
}


CMyRestaurant::~CMyRestaurant()
{
}

int CMyRestaurant::Update()
{
	if (SetUpShopChang == true)
	{
		Mouse = dynamic_cast<CMouse*>(CObjMgr::Get_Instance()->Get_Mouse());
		//Mouse->G
		D3DXVECTOR3 MousePos = Mouse->Get_Pos();
		bool GripItem = Mouse->Get_GripItem();
		POINT pt = {};
		pt.x = MousePos.x;
		pt.y = MousePos.y;
		//cout << "x:" << pt.x << "y:" << pt.y << endl;
		if (PtInRect(&(m_tRect), pt))
		{
			InnSetting = true;

		}
		else
		{
			InnSetting = false;
		}

		for (int i = 0; i < 4; ++i)
		{
			if (PtInRect(&m_ShopItemSelected_RECT[i], pt))
			{
				if (CKeyMgr::Get_Instance()->KeyDown(KEY_LBUTTON)) // 만약 마우스를 눌렀을때
				{
					if (m_ShopItemSelected[i].Texture.wstrStateKey == L"ShopItem_Selected")
					{// 이미선택된 아이템을 한번 더 누르면?
						if (CPlayer::HungryFull >= CPlayer::Hungry + m_ShopItemSelected[i].ItemData.iGold
							&&m_ItemData[i].eatItem==true)
						{

							m_ItemData[i].eatItem = false;
							m_ShopItemSelected[i].Texture.wstrStateKey = L"ShopItem_UnSelected";
							CPlayer::Hungry += m_ShopItemSelected[i].ItemData.iGold;

							CPlayer::PlusAtt += m_ShopItemSelected[i].ItemData.iAtt;
							
							CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EATFOOD);
							CSoundMgr::Get_Instance()->PlaySound(L"MyFoodEat.wav", CSoundMgr::EATFOOD);
							

						}
					}
					else if (m_ShopItemSelected[i].Texture.wstrStateKey == L"ShopItem_UnSelected"
						&&m_ItemData[i].eatItem==true)
					{
						m_ShopItemSelected[i].Texture.wstrStateKey = L"ShopItem_Selected";
						if (i == 0)
						{
							m_ShopItemSelected[1].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[2].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[3].Texture.wstrStateKey = L"ShopItem_UnSelected";

						}
						else if (i == 1)
						{
							m_ShopItemSelected[0].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[2].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[3].Texture.wstrStateKey = L"ShopItem_UnSelected";
						}
						else if (i == 2)
						{
							m_ShopItemSelected[0].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[1].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[3].Texture.wstrStateKey = L"ShopItem_UnSelected";
						}
						else if (i == 3)
						{
							m_ShopItemSelected[0].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[1].Texture.wstrStateKey = L"ShopItem_UnSelected";
							m_ShopItemSelected[2].Texture.wstrStateKey = L"ShopItem_UnSelected";
						}
					}
				}
			}

		}
	}
	return 0;
}

void CMyRestaurant::LateUpdate()
{
	FrameMove(1.f);
	MakeWorldMatrix();
	if (SetUpShopChang == true)
	{
		//FrameMove(1.f);
		if (ItemDataOneTime == false)
		{
			SetItemData();
			ItemDataOneTime = true;
		}
	}
	RECT rc = {};
	PlayerRect = CObjMgr::Get_Instance()->Get_Player()->Get_Rect();
	if (IntersectRect(&rc, &PlayerRect, &m_NPC_RECT))
	{
		SetUpShopChang = true;
	}
	else
	{
		SetUpShopChang = false;
		InnSetting = false;
	}
}

void CMyRestaurant::Render()
{
	const TEX_INFO* NPCUI = nullptr;
	NPCUI = CTextureMgr::Get_Instance()->GetTexInfo(
		NPC.Texture.wstrObjectKey,
		NPC.Texture.wstrStateKey,
		(int)m_tFrame.fCurFrame);
	NULL_CHECK(NPCUI);

	float CenterX2 = NPCUI->tImgInfo.Width * 0.5f;
	float CenterY2 = NPCUI->tImgInfo.Height * 0.5f;

	CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(NPC.matWorld));
	CDeviceMgr::Get_Instance()->GetSprite()->Draw(
		NPCUI->pTexture, nullptr, &D3DXVECTOR3(CenterX2, CenterY2, 0.f),
		nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

	if (SetUpShopChang == true)
	{
		const TEX_INFO* ShopUI = nullptr;
		ShopUI = CTextureMgr::Get_Instance()->GetTexInfo(
			m_tUnit.Texture.wstrObjectKey, m_tUnit.Texture.wstrStateKey, m_tUnit.Texture.iIndex);
		NULL_CHECK(ShopUI);

		float CenterX = ShopUI->tImgInfo.Width * 0.5f;
		float CenterY = ShopUI->tImgInfo.Height * 0.5f;

		CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_tUnit.matWorld));
		CDeviceMgr::Get_Instance()->GetSprite()->Draw(
			ShopUI->pTexture, nullptr, &D3DXVECTOR3(CenterX, CenterY, 0.f),
			nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		// ShopChang
		for (int i = 0; i < 2; ++i)
		{
			const TEX_INFO* ShopChang = nullptr;
			ShopChang = CTextureMgr::Get_Instance()->GetTexInfo(
				m_ShopItemSelected[i].Texture.wstrObjectKey,
				m_ShopItemSelected[i].Texture.wstrStateKey,
				m_ShopItemSelected[i].Texture.iIndex);
			NULL_CHECK(ShopChang);

			float SCenterX = ShopChang->tImgInfo.Width*0.5f;
			float SCenterY = ShopChang->tImgInfo.Height*0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_ShopItemSelected[i].matWorld));
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				ShopChang->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// 아이템슬롯
		/*for (int j = 0; j < 2; ++j)
		{
			const TEX_INFO* InventorySlot = nullptr;
			InventorySlot = CTextureMgr::Get_Instance()->GetTexInfo(
				m_Inven_INFO[j].Texture.wstrObjectKey,
				m_Inven_INFO[j].Texture.wstrStateKey,
				m_Inven_INFO[j].Texture.iIndex);
			NULL_CHECK(InventorySlot);

			float SCenterX = InventorySlot->tImgInfo.Width*0.5f;
			float SCenterY = InventorySlot->tImgInfo.Height*0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_Inven_INFO[j].matWorld));
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				InventorySlot->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}*/


		// 아이템
		for (int i = 0; i < 2; ++i)
		{
			if (m_ItemData[i].eatItem == false)
				continue;
			const TEX_INFO* Item = nullptr;
			Item = CTextureMgr::Get_Instance()->GetTexInfo(
				m_ItemData[i].Texture.wstrObjectKey,
				m_ItemData[i].Texture.wstrStateKey,
				m_ItemData[i].Texture.iIndex);
			NULL_CHECK(Item);

			float SCenterX = Item->tImgInfo.Width*0.5f;
			float SCenterY = Item->tImgInfo.Height*0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_ItemData[i].matWorld));
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				Item->pTexture, nullptr, &D3DXVECTOR3(SCenterX, SCenterY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (m_ItemData[0].eatItem == true)
		{
			TCHAR szIndex0[MIN_STR] = L"";
			D3DXMATRIX TextMatWorld0, TextMatScale0, TextMatTrans0;
			D3DXMatrixScaling(&TextMatScale0, 1.f, 1.5f, 0.f);
			D3DXMatrixTranslation(&TextMatTrans0, 285.f, 200.f, 0.f);
			TextMatWorld0 = TextMatScale0*TextMatTrans0;

			swprintf_s(szIndex0, L"%d", m_ShopItemSelected[0].ItemData.iGold);
			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld0);
			CDeviceMgr::Get_Instance()->GetFont()->DrawText(
				CDeviceMgr::Get_Instance()->GetSprite(), szIndex0, lstrlen(szIndex0),
				nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		if (m_ItemData[1].eatItem == true)
		{
			TCHAR szIndex1[MIN_STR] = L"";
			D3DXMATRIX TextMatWorld1, TextMatScale1, TextMatTrans1;
			D3DXMatrixScaling(&TextMatScale1, 1.f, 1.5f, 0.f);
			D3DXMatrixTranslation(&TextMatTrans1, 285.f, 410.f, 0.f);
			TextMatWorld1 = TextMatScale1*TextMatTrans1;

			swprintf_s(szIndex1, L"%d", m_ShopItemSelected[1].ItemData.iGold);
			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&TextMatWorld1);
			CDeviceMgr::Get_Instance()->GetFont()->DrawText(
				CDeviceMgr::Get_Instance()->GetSprite(), szIndex1, lstrlen(szIndex1),
				nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		}







		const TEX_INFO* Mouse1 = nullptr;
		Mouse1 = CTextureMgr::Get_Instance()->GetTexInfo(
			L"Menu", L"Mouse", 1);

		m_pMenuMouse.vSize.x = 30.f / Mouse1->tImgInfo.Width;
		m_pMenuMouse.vSize.y = 30.f / Mouse1->tImgInfo.Height;

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

			float MouseX = Mouse1->tImgInfo.Width * 0.5f;
			float MouseY = Mouse1->tImgInfo.Height * 0.5f;

			CDeviceMgr::Get_Instance()->GetSprite()->SetTransform(&(m_pMenuMouse.matWorld));
			CDeviceMgr::Get_Instance()->GetSprite()->Draw(
				Mouse1->pTexture, nullptr, &D3DXVECTOR3(MouseX, MouseY, 0.f),
				nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

HRESULT CMyRestaurant::Initialize()
{
	NPC.vPos = { 1150.f, 1000.f,0.f };
	NPC.vSize = { 1.f,1.f,0.f };
	NPC.Texture.wstrObjectKey = L"Inn";
	NPC.Texture.wstrStateKey = L"Inn_NPC";
	NPC.Texture.iIndex = 0;
	m_tFrame.fCurFrame = 0.f;
	m_tFrame.fFrameCnt = 4.f;

	NPC.vLength = { 100.f,100.f,0.f };

	const TEX_INFO* NPCImage = nullptr;
	NPCImage = CTextureMgr::Get_Instance()->GetTexInfo(
		NPC.Texture.wstrObjectKey,
		NPC.Texture.wstrStateKey,
		NPC.Texture.iIndex);


	NPC.vSize.x = NPC.vLength.x / NPCImage->tImgInfo.Width;
	NPC.vSize.y = NPC.vLength.y / NPCImage->tImgInfo.Height;

	m_tUnit.vPos = { 200.f,300.f,0.f };
	m_tUnit.vSize = { 1.f,1.f,0.f };
	m_tUnit.Texture.wstrObjectKey = L"Shop";
	m_tUnit.Texture.wstrStateKey = L"ShopUI";
	m_tUnit.Texture.iIndex = 0;
	m_tUnit.vLength = { 350.f,500.f,0.f };
	const TEX_INFO* StartButton = nullptr;
	StartButton = CTextureMgr::Get_Instance()->GetTexInfo(
		m_tUnit.Texture.wstrObjectKey,
		m_tUnit.Texture.wstrStateKey,
		m_tUnit.Texture.iIndex);

	m_tUnit.vSize.x = m_tUnit.vLength.x / StartButton->tImgInfo.Width;
	m_tUnit.vSize.y = m_tUnit.vLength.y / StartButton->tImgInfo.Height;



	int Inven_Cnt_Y = 0;
	float Inven_To_Inven_Y_Gap = 205.f;
	for (int i = 0; i < 4; ++i)
	{
		m_Inven_INFO[i].Texture.wstrObjectKey = L"InventorySlot";
		m_Inven_INFO[i].Texture.wstrStateKey = L"ItemOff";
		m_Inven_INFO[i].Texture.iIndex = 0;
		m_Inven_INFO[i].vPos.x = 200.f;
		m_Inven_INFO[i].vPos.y = 200.f + (Inven_Cnt_Y*Inven_To_Inven_Y_Gap);
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

		Inven_Cnt_Y++;


	}

	Inven_Cnt_Y = 0;
	for (int i = 0; i < 4; ++i)
	{
		m_ShopItemSelected[i].Texture.wstrObjectKey = L"ShopSelectItem";
		m_ShopItemSelected[i].Texture.wstrStateKey = L"ShopItem_UnSelected";
		m_ShopItemSelected[i].Texture.iIndex = 0;
		m_ShopItemSelected[i].vPos.x = 200.f;
		m_ShopItemSelected[i].vPos.y = 200.f + (Inven_Cnt_Y*Inven_To_Inven_Y_Gap);
		m_ShopItemSelected[i].vLength.x = 300.f;
		m_ShopItemSelected[i].vLength.y = 100.f;
		m_ShopItemSelected[i].HoldingItem = FALSE; // 아이템이 들어있지 않다.

		const TEX_INFO* ShopChang = nullptr;
		ShopChang = CTextureMgr::Get_Instance()->GetTexInfo(
			m_ShopItemSelected[i].Texture.wstrObjectKey,
			m_ShopItemSelected[i].Texture.wstrStateKey,
			m_ShopItemSelected[i].Texture.iIndex);

		m_ShopItemSelected[i].vSize.x =
			m_ShopItemSelected[i].vLength.x / ShopChang->tImgInfo.Width;
		m_ShopItemSelected[i].vSize.y =
			m_ShopItemSelected[i].vLength.y / ShopChang->tImgInfo.Height;


		Inven_Cnt_Y++;


	}




	ItemDataOneTime = false;
	return S_OK;
}

void CMyRestaurant::Release()
{
}

void CMyRestaurant::FrameMove(float _AniSpeed)
{
	m_tFrame.fCurFrame += m_tFrame.fFrameCnt*CTimeMgr::Get_Instance()->GetDelta()*_AniSpeed;
	if (m_tFrame.fCurFrame >= m_tFrame.fFrameCnt)
		m_tFrame.fCurFrame = 0.f;
}

void CMyRestaurant::MakeWorldMatrix()
{
	D3DXMatrixScaling(&NPC.matScale,
		NPC.vSize.x,
		NPC.vSize.y, 0.f);
	D3DXMatrixTranslation(&NPC.matTrans,
		NPC.vPos.x - CScrollMgr::GetScrollPos(0),
		NPC.vPos.y - CScrollMgr::GetScrollPos(1), 0.f);

	NPC.matWorld = NPC.matScale * NPC.matTrans;

	m_NPC_RECT.left = LONG(NPC.vPos.x - (NPC.vLength.x * 0.5f));
	m_NPC_RECT.top = LONG(NPC.vPos.y - (LONG(NPC.vLength.y) >> 1));
	m_NPC_RECT.right = LONG(NPC.vPos.x + (NPC.vLength.x * 0.5f));
	m_NPC_RECT.bottom = LONG(NPC.vPos.y + (NPC.vLength.y * 0.5f));


	if (SetUpShopChang == true)
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

		for (int i = 0; i < 2; ++i)
		{
			D3DXMatrixScaling(&m_ShopItemSelected[i].matScale,
				m_ShopItemSelected[i].vSize.x,
				m_ShopItemSelected[i].vSize.y,
				0.f);
			D3DXMatrixTranslation(&m_ShopItemSelected[i].matTrans,
				m_ShopItemSelected[i].vPos.x,
				m_ShopItemSelected[i].vPos.y, 0.f);

			m_ShopItemSelected[i].matWorld =
				m_ShopItemSelected[i].matScale*
				m_ShopItemSelected[i].matTrans;
		}
		for (int i = 0; i < 2; ++i)
		{
			m_ShopItemSelected_RECT[i].left =
				LONG(m_ShopItemSelected[i].vPos.x -
				(m_ShopItemSelected[i].vLength.x * 0.5f));

			m_ShopItemSelected_RECT[i].top =
				LONG(m_ShopItemSelected[i].vPos.y -
				(LONG(m_ShopItemSelected[i].vLength.y) >> 1));

			m_ShopItemSelected_RECT[i].right =
				LONG(m_ShopItemSelected[i].vPos.x +
				(m_ShopItemSelected[i].vLength.x * 0.5f));

			m_ShopItemSelected_RECT[i].bottom =
				LONG(m_ShopItemSelected[i].vPos.y +
				(m_ShopItemSelected[i].vLength.y * 0.5f));
		}

		for (int i = 0; i < 2; ++i)
		{
			D3DXMatrixScaling(&m_Inven_INFO[i].matScale,
				m_Inven_INFO[i].vSize.x,
				m_Inven_INFO[i].vSize.y,
				0.f);
			D3DXMatrixTranslation(&m_Inven_INFO[i].matTrans,
				m_Inven_INFO[i].vPos.x,
				m_Inven_INFO[i].vPos.y, 0.f);

			m_Inven_INFO[i].matWorld =
				m_Inven_INFO[i].matScale*
				m_Inven_INFO[i].matTrans;
		}
		for (int i = 0; i < 2; ++i)
		{
			if (m_ItemData[i].eatItem == false)
				continue;
			D3DXMatrixScaling(&m_ItemData[i].matScale,
				m_ItemData[i].vSize.x, m_ItemData[i].vSize.y, 0.f);
			D3DXMatrixTranslation(&m_ItemData[i].matTrans,
				m_ItemData[i].vPos.x,
				m_ItemData[i].vPos.y, 0.f);

			m_ItemData[i].matWorld = m_ItemData[i].matScale * m_ItemData[i].matTrans;
		}
	}
}

void CMyRestaurant::SetItemData()
{
	// 햄버거 정보
	m_ItemData[0].iAtt = 30;
	m_ItemData[0].iGold = 5;
	m_ItemData[0].szName = L"Burger";
	m_ItemData[0].Texture.wstrObjectKey = L"InnFood";
	m_ItemData[0].Texture.wstrStateKey = L"Burger";
	m_ItemData[0].Texture.iIndex = 0;
	m_ItemData[0].vLength = m_Inven_INFO[0].vLength;
	m_ItemData[0].vSize = {
		m_Inven_INFO[0].vSize.x - 1.2f,
		m_Inven_INFO[0].vSize.y - 1.2f,
		0.f };
	m_ItemData[0].eType = ITEMTYPE::ATTACKWEAPON; // 주무기
	m_ItemData[0].eLength = ITEMINTER::LONG; // 원거리무기
	m_ItemData[0].eatItem = true;
	m_ItemData[0].Griped = false;
	m_ItemData[0].vPos = m_Inven_INFO[0].vPos;

	//m_ShopItemSelected[0].ItemData
	m_ShopItemSelected[0].ItemData.iAtt = 30;
	m_ShopItemSelected[0].ItemData.iGold = 5;
	m_ShopItemSelected[0].ItemData.szName = L"Burger";
	m_ShopItemSelected[0].ItemData.Texture.wstrObjectKey = L"InnFood";
	m_ShopItemSelected[0].ItemData.Texture.wstrStateKey = L"Burger";
	m_ShopItemSelected[0].ItemData.Texture.iIndex = 0;
	m_ShopItemSelected[0].ItemData.vLength = m_Inven_INFO[0].vLength;
	m_ShopItemSelected[0].ItemData.vSize = {
		m_Inven_INFO[0].vSize.x,
		m_Inven_INFO[0].vSize.y,
		0.f };

	m_ShopItemSelected[0].ItemData.eType = ITEMTYPE::ATTACKWEAPON; // 주무기
	m_ShopItemSelected[0].ItemData.eLength = ITEMINTER::LONG; // 원거리무기
	m_ShopItemSelected[0].ItemData.eatItem = true;
	m_ShopItemSelected[0].ItemData.Griped = false;
	m_ShopItemSelected[0].ItemData.vPos = m_Inven_INFO[0].vPos;


	// 불검 정보

	// item 정보
	m_ItemData[1].iAtt = 15;
	m_ItemData[1].iGold = 1;
	m_ItemData[1].szName = L"Choco";
	m_ItemData[1].Texture.wstrObjectKey = L"InnFood";
	m_ItemData[1].Texture.wstrStateKey = L"Choco";
	m_ItemData[1].Texture.iIndex = 0;
	m_ItemData[1].vLength = m_Inven_INFO[1].vLength;
	m_ItemData[1].vSize = {
		m_Inven_INFO[1].vSize.x,
		m_Inven_INFO[1].vSize.y - 1.2f,
		0.f };
	m_ItemData[1].eType = ITEMTYPE::ATTACKWEAPON; // 주무기
	m_ItemData[1].eLength = ITEMINTER::MELEE; // 근접무기
	m_ItemData[1].eatItem = true;
	m_ItemData[1].Griped = false;
	m_ItemData[1].vPos = m_Inven_INFO[1].vPos;
	/////////////////////////////
	m_ShopItemSelected[1].ItemData.iAtt = 15;
	m_ShopItemSelected[1].ItemData.iGold = 1;
	m_ShopItemSelected[1].ItemData.szName = L"Choco";
	m_ShopItemSelected[1].ItemData.Texture.wstrObjectKey = L"InnFood";
	m_ShopItemSelected[1].ItemData.Texture.wstrStateKey = L"Choco";
	m_ShopItemSelected[1].ItemData.Texture.iIndex = 0;
	m_ShopItemSelected[1].ItemData.vLength = m_Inven_INFO[1].vLength;
	m_ShopItemSelected[1].ItemData.vSize = {
		m_Inven_INFO[1].vSize.x,
		m_Inven_INFO[1].vSize.y,
		0.f };
	m_ShopItemSelected[1].ItemData.eType = ITEMTYPE::ATTACKWEAPON; // 주무기
	m_ShopItemSelected[1].ItemData.eLength = ITEMINTER::MELEE; // 근접무기
	m_ShopItemSelected[1].ItemData.eatItem = true;
	m_ShopItemSelected[1].ItemData.Griped = false;
	m_ShopItemSelected[1].ItemData.vPos = m_Inven_INFO[1].vPos;
}

CMyRestaurant * CMyRestaurant::Create()
{
	CMyRestaurant* m_pInstance = new CMyRestaurant;

	if (FAILED(m_pInstance->Initialize()))
	{
		SafeDelete(m_pInstance);
		return nullptr;
	}
	return m_pInstance;
}
