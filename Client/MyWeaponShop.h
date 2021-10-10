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
	

	UNIT_INFO m_tUnit; // ���� ���� â
	RECT m_tRect;

	INVENINFO m_Inven_INFO[4];	// ������ �� �װ� ��������
	RECT m_Inven_RECT[20];

	ITEM_DATA m_ItemData[4];	//
	RECT	  m_Inven_ItemRECT[4];	// �Ұ�,����,������,�θ޶�
	
	INVENINFO m_ShopItemSelected[4];
	RECT	  m_ShopItemSelected_RECT[4];

	UNIT_INFO m_pMenuMouse;

	bool WeaponShopClicked;
	bool SetUpShopChang;	// npc�� ������ ����â�� ������.
	bool ItemDataOneTime;

	

	CMouse* Mouse;
	D3DXVECTOR3 MousevDir;

	FRAME m_tFrame;


};

