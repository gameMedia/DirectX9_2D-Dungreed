#pragma once
#include "Obj.h"
class CMouse :
	public CObj
{
private:
	explicit CMouse();
public:
	virtual ~CMouse();
public:
	D3DXVECTOR3 Get_Pos() { return m_tUnit.vPos; }
	void Set_GripItem(bool _grip) { GripItem = _grip; }
	bool Get_GripItem() { return GripItem; }
	
	void Set_GripItemInfo(ITEM_DATA _GripedInfo, int iIndex) { GripedItemInfo = _GripedInfo; item_Index = iIndex; }
	ITEM_DATA Get_GripItemInfo() { return GripedItemInfo; }
	int GripedItem_Index() { return item_Index; }
public:
	// CObj을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;
private:
	virtual HRESULT Initialize() override;
	virtual void Release() override;
	void MakeWorldMatrix();
public:
	static CMouse* Create();

private:
	POINT pt;
	bool GripItem;
	ITEM_DATA GripedItemInfo;
	int item_Index;


};

