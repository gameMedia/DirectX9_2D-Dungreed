#pragma once
#include "OnOffUI.h"
class CMouse;
class CPlayer;
class CInventory :
	public COnOffUI
{
private:
	explicit CInventory();
public:
	virtual ~CInventory();

public:
	INVENINFO* Get_InventorySlot() { return m_Inven_INFO; }
	ITEM_DATA* Get_ItemData() { return m_ItemData; }
	void Set_InventorySlotItem(ITEM_DATA _itemdata, int iIndex, bool _eat);
	void Set_HoldingItem(bool _bischeck, int iIndex) 
	{ m_Inven_INFO[iIndex].HoldingItem = _bischeck; }
public:
	// COnOffUI을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	void MakeWorldMatrix();
public:
	// 작업
	void Set_OnOff(bool _OnOff) { OnOffInventory = _OnOff; }
	bool& Get_OnOffInventory() { return OnOffInventory; }
	bool& Get_InvenSetting() { return InvenSetting; }
	void Set_InvenSetting(bool _set) { InvenSetting = _set; }
	void ClearItemData(int index);
private:
	void MoveItem();
public:
	static CInventory* Create();

private:
	RECT m_Inven_RECT[20];// 인벤토리 슬롯에 대한 렉트
	INVENINFO m_Inven_INFO[20];// 인벤토리 슬릇에 대한 데이터
	RECT m_Inven_ItemRECT[20];// 인벤토리 아이템에 대한 렉트
	ITEM_DATA m_ItemData[20]; //인벤토리에 아이템에 대한 데이터

	UNIT_INFO m_pMenuMouse;

	bool InitRevolver; // 처음에 리볼버 들고 시작하자

	bool OnOffInventory;
	bool ChangeMainItem;
	//// 주무기
	//INVENINFO m_AttackWeaponInven_INFO[2];
	//RECT m_AttackWeaphonInven_RECT[2];
	//ITEM_DATA m_MainItemData[2];
	//RECT m_Main_ItemRECT[2];

	//// 보조무기
	//INVENINFO m_SubWeaponInven_INFO[2];
	//RECT m_SubWeaphonInven_RECT[2];
	//ITEM_DATA m_SubItemData[2];
	//RECT m_Sub_ItemRECT[2];

	//// 아티펙트
	//INVENINFO m_ArtifactInven_INFO[4];
	//RECT m_ArtifactInven_RECT[4];
	//ITEM_DATA m_ArtifactItemData[4];
	//RECT m_Artifact_ItemRECT[4];

	CMouse* Mouse;
	D3DXVECTOR3 MousevDir;

	CPlayer* m_pPlayer;

	bool InvenSetting;
};

