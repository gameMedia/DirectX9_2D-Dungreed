#pragma once
class CMouse;
class CPlayer;
class CMyWeaponShop
{
private:
	CMyWeaponShop();
public:
	~CMyWeaponShop();

public:
	int Update();
	void LateUpdate();
	void Render();

private:
	HRESULT Initialize();
	void Release();
	void FrameMove(float _AniSpeed = 1.f);
	void MakeWorldMatrix();
	void SetItemData();

public:
	static CMyWeaponShop* Create();
	static bool ShopSetting;
private:


	RECT PlayerRect;


	UNIT_INFO ShopImage;
	
	UNIT_INFO NPC;
	RECT m_NPC_RECT;
	bool ClickedNPC;
	

	UNIT_INFO m_tUnit; // 상점 메인 창
	RECT m_tRect;

	INVENINFO m_Inven_INFO[4];	// 아이템 을 네개 정의하자
	RECT m_Inven_RECT[20];

	ITEM_DATA m_ItemData[4];	//
	RECT	  m_Inven_ItemRECT[4];	// 불검,방패,리젠링,부메랑
	
	INVENINFO m_ShopItemSelected[4];
	RECT	  m_ShopItemSelected_RECT[4];

	UNIT_INFO m_pMenuMouse;

	bool WeaponShopClicked;
	bool SetUpShopChang;	// npc를 누르면 상점창이 열린다.
	bool ItemDataOneTime;

	

	CMouse* Mouse;
	D3DXVECTOR3 MousevDir;

	FRAME m_tFrame;


};

