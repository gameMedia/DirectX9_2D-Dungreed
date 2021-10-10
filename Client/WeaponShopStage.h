#pragma once
#include "Field.h"
class CMyWeaponShop;
class CWeaponShopStage :
	public CField
{
private:
	explicit CWeaponShopStage();
public:
	virtual ~CWeaponShopStage();

public:
	// CField��(��) ���� ��ӵ�
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;

public:
	static CWeaponShopStage* Create();

	// CField��(��) ���� ��ӵ�
	virtual void Set_Map() override;

	// CField��(��) ���� ��ӵ�
	virtual D3DXVECTOR3 & Get_Pos() override;

private:
	CMyWeaponShop* pwI;
	UNIT_INFO ShopImage;
};

