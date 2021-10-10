#pragma once
#include "Field.h"
class CMyRestaurant;
class CRestaurant :
	public CField
{
private:
	explicit CRestaurant();
public:
	virtual ~CRestaurant();

public:
	// CField을(를) 통해 상속됨
	virtual int Update() override;
	virtual void LateUpdate() override;
	virtual void Render() override;

private:
	virtual HRESULT Initialize() override;
	virtual HRESULT LateInitialize();
	virtual void Release() override;

public:
	static CRestaurant* Create();

	// CField을(를) 통해 상속됨
	virtual void Set_Map() override;

	// CField을(를) 통해 상속됨
	virtual D3DXVECTOR3 & Get_Pos() override;

private:
	CMyRestaurant* pwI;
	UNIT_INFO ShopImage;
};
